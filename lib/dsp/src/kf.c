/**
 * @file   kf.c
 * @brief  Kalman filter implementation
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
 */

#include "dsp/kf.h"

#include <assert.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix_double.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector_double.h>

static int alloc_from_arr(
	size_t rows, size_t cols, const double* const src, gsl_matrix** const dest)
{
	assert(src != NULL);
	assert(dest != NULL);
	assert(*dest != NULL);

	gsl_matrix_const_view view = gsl_matrix_const_view_array(src, rows, cols);

	*dest = gsl_matrix_alloc(rows, cols);
	if (*dest == NULL)
		return -ENOMEM;

	return gsl_matrix_memcpy(*dest, &view.matrix);
}

static int alloc_locals(struct kf* const kf)
{
	assert(kf != NULL);

	kf->z = gsl_vector_calloc(kf->n_meas);
	if (kf->z == NULL)
		return -ENOMEM;

	kf->u = gsl_vector_calloc(kf->n_state);
	if (kf->u == NULL)
		return -ENOMEM;

	kf->k = gsl_matrix_alloc(kf->n_state, kf->n_meas);
	if (kf->k == NULL)
		return -ENOMEM;

	return 0;
}

static void free_locals(struct kf* const kf)
{
	gsl_matrix_free(kf->k);
	gsl_vector_free(kf->z);
	gsl_vector_free(kf->u);
}

static int alloc_vars(struct kf* const kf)
{
	kf->x = gsl_vector_calloc(kf->n_state);
	if (kf->x == NULL)
		return -ENOMEM;

	kf->p = gsl_matrix_calloc(kf->n_state, kf->n_state);
	if (kf->p == NULL)
		return -ENOMEM;

	gsl_matrix_set_identity(kf->p);

	return 0;
}

static void free_vars(struct kf* const kf)
{
	assert(kf != NULL);

	gsl_vector_free(kf->x);
	gsl_matrix_free(kf->p);
}

static int alloc_usr(
	struct kf* const kf,
	const double* const f,
	const double* const h,
	const double* const q,
	const double* const r,
	const double* const b)
{
	int err = alloc_from_arr(kf->n_state, kf->n_state, f, &kf->f);
	if (err < 0)
		return err;

	err = alloc_from_arr(kf->n_meas, kf->n_state, h, &kf->h);
	if (err < 0)
		return err;

	err = alloc_from_arr(kf->n_state, kf->n_state, q, &kf->q);
	if (err < 0)
		return err;

	err = alloc_from_arr(kf->n_meas, kf->n_meas, r, &kf->r);
	if (err < 0)
		return err;

	return alloc_from_arr(kf->n_state, kf->n_state, b, &kf->b);
}

static void free_usr(struct kf* const kf)
{
	assert(kf != NULL);

	gsl_matrix_free(kf->f);
	gsl_matrix_free(kf->h);
	gsl_matrix_free(kf->q);
	gsl_matrix_free(kf->r);
	gsl_matrix_free(kf->b);
}

static int alloc_byprod(struct kf* const kf)
{
	kf->bp.fx = gsl_vector_alloc(kf->n_state);
	if (kf->bp.fx == NULL)
		return -ENOMEM;

	kf->bp.fp = gsl_matrix_alloc(kf->n_state, kf->n_state);
	if (kf->bp.fp == NULL)
		return -ENOMEM;

	kf->bp.fpft = gsl_matrix_alloc(kf->n_state, kf->n_state);
	if (kf->bp.fpft == NULL)
		return -ENOMEM;

	kf->bp.hx = gsl_vector_alloc(kf->n_meas);
	if (kf->bp.hx == NULL)
		return -ENOMEM;

	kf->bp.pht = gsl_matrix_alloc(kf->n_state, kf->n_meas);
	if (kf->bp.pht == NULL)
		return -ENOMEM;

	kf->bp.hphtr = gsl_matrix_alloc(kf->n_meas, kf->n_meas);
	if (kf->bp.hphtr == NULL)
		return -ENOMEM;

	kf->bp.perm = gsl_permutation_alloc(kf->n_meas);
	if (kf->bp.perm == NULL)
		return -ENOMEM;

	kf->bp.inv = gsl_matrix_alloc(kf->n_meas, kf->n_meas);
	if (kf->bp.inv == NULL)
		return -ENOMEM;

	kf->bp.hp = gsl_matrix_alloc(kf->n_meas, kf->n_state);
	if (kf->bp.hp == NULL)
		return -ENOMEM;

	kf->bp.khp = gsl_matrix_alloc(kf->n_state, kf->n_state);
	if (kf->bp.khp == NULL)
		return -ENOMEM;

	kf->bp.kz = gsl_vector_alloc(kf->n_state);
	if (kf->bp.kz == NULL)
		return -ENOMEM;

	return 0;
}

static void free_byprod(struct kf* const kf)
{
	assert(kf != NULL);

	gsl_vector_free(kf->bp.fx);
	gsl_vector_free(kf->bp.hx);
	gsl_vector_free(kf->bp.kz);

	gsl_matrix_free(kf->bp.fp);
	gsl_matrix_free(kf->bp.fpft);
	gsl_matrix_free(kf->bp.khp);
	gsl_matrix_free(kf->bp.hphtr);
	gsl_matrix_free(kf->bp.pht);
	gsl_matrix_free(kf->bp.hp);
	gsl_matrix_free(kf->bp.inv);

	gsl_permutation_free(kf->bp.perm);
}

static int calc_inv(struct kf* const kf)
{
	int signum;

	const int err = gsl_linalg_LU_decomp(kf->bp.hphtr, kf->bp.perm, &signum);
	if (err < 0)
		return err;

	return gsl_linalg_LU_invert(kf->bp.hphtr, kf->bp.perm, kf->bp.inv);
}

static int calc_k(struct kf* const kf)
{
	/**
	 * K = P(k)H(k)^T(H(k)P(k)H(k)^T + R(k))^-1
	 */

	int err = gsl_blas_dgemm(
		CblasNoTrans, CblasTrans, 1.0, kf->p, kf->h, 0.0, kf->bp.pht);
	if (err < 0)
		return err;

	err = gsl_blas_dgemm(
		CblasNoTrans, CblasNoTrans, 1.0, kf->h, kf->bp.pht, 0.0, kf->bp.hphtr);
	if (err < 0)
		return err;

	err = gsl_matrix_add(kf->bp.hphtr, kf->r);
	if (err < 0)
		return err;

	err = calc_inv(kf);
	if (err < 0)
		return err;

	return gsl_blas_dgemm(
		CblasNoTrans, CblasNoTrans, 1.0, kf->bp.pht, kf->bp.inv, 0.0, kf->k);
}

static int predict_x(struct kf* const kf, const double* const u)
{
	/**
	 * x(k) = F(k)x(k-1) + B(k)u(k)
	 */

	int err = gsl_blas_dgemv(CblasNoTrans, 1.0, kf->f, kf->x, 0.0, kf->bp.fx);
	if (err < 0)
		return err;

	gsl_vector_const_view uv = gsl_vector_const_view_array(u, kf->n_state);

	err = gsl_blas_dgemv(CblasNoTrans, 1.0, kf->b, &uv.vector, 1.0, kf->bp.fx);
	if (err < 0)
		return err;

	return gsl_vector_memcpy(kf->x, kf->bp.fx);
}

static int predict_p(struct kf* const kf)
{
	/**
	 * P(k) = F(k)P(k-1)F(k)^T + Q(k)
	 */

	int err = gsl_blas_dgemm(
		CblasNoTrans, CblasNoTrans, 1.0, kf->f, kf->p, 0.0, kf->bp.fp);
	if (err < 0)
		return err;

	err = gsl_blas_dgemm(
		CblasNoTrans, CblasTrans, 1.0, kf->bp.fp, kf->f, 1.0, kf->bp.fpft);
	if (err < 0)
		return err;

	err = gsl_matrix_add(kf->bp.fpft, kf->q);
	if (err < 0)
		return err;

	err = gsl_matrix_memcpy(kf->p, kf->bp.fpft);
	if (err < 0)
		return err;

	gsl_matrix_set_zero(kf->bp.fpft);

	return 0;
}

static int update_x(struct kf* const kf, const double* const z)
{
	/**
	 * x(k) = x(k) + K(z(k) - H(k)x(k))
	 */

	int err = gsl_blas_dgemv(CblasNoTrans, 1.0, kf->h, kf->x, 0.0, kf->bp.hx);
	if (err < 0)
		return err;

	gsl_vector_const_view zv = gsl_vector_const_view_array(z, kf->n_meas);

	err = gsl_vector_memcpy(kf->z, &zv.vector);
	if (err < 0)
		return err;

	err = gsl_vector_sub(kf->z, kf->bp.hx);
	if (err < 0)
		return err;

	err = gsl_blas_dgemv(CblasNoTrans, 1.0, kf->k, kf->z, 0.0, kf->bp.kz);
	if (err < 0)
		return err;

	return gsl_vector_add(kf->x, kf->bp.kz);
}

static int update_p(struct kf* const kf)
{
	/**
	 * P(k) = P(k) - KH(k)P(k)
	 */

	int err = gsl_blas_dgemm(
		CblasNoTrans, CblasNoTrans, 1.0, kf->h, kf->p, 0.0, kf->bp.hp);
	if (err < 0)
		return err;

	err = gsl_blas_dgemm(
		CblasNoTrans, CblasNoTrans, 1.0, kf->k, kf->bp.hp, 0.0, kf->bp.khp);
	if (err < 0)
		return err;

	return gsl_matrix_sub(kf->p, kf->bp.khp);
}

int kf_init(
	struct kf* const kf,
	const double* const f,
	const double* const h,
	const double* const q,
	const double* const r,
	const double* const b,
	size_t n_state,
	size_t n_meas)
{
	assert(kf != NULL);
	assert(f != NULL);
	assert(h != NULL);
	assert(q != NULL);
	assert(r != NULL);
	assert(b != NULL);

	kf->n_state = n_state;
	kf->n_meas  = n_meas;

	int err = alloc_locals(kf);
	if (err < 0)
		return err;

	err = alloc_vars(kf);
	if (err < 0)
		return err;

	err = alloc_usr(kf, f, h, q, r, b);
	if (err < 0)
		return err;

	return alloc_byprod(kf);
}

void kf_destroy(struct kf* const kf)
{
	assert(kf != NULL);

	free_locals(kf);
	free_vars(kf);
	free_usr(kf);
	free_byprod(kf);
}

int kf_predict(struct kf* const kf, const double* const u)
{
	assert(kf != NULL);
	assert(u != NULL);

	int err = predict_x(kf, u);
	if (err < 0)
		return err;

	return predict_p(kf);
}

int kf_update(struct kf* const kf, const double* const z)
{
	assert(kf != NULL);
	assert(z != NULL);

	int err = calc_k(kf);
	if (err < 0)
		return err;

	err = update_x(kf, z);
	if (err < 0)
		return err;

	return update_p(kf);
}

void kf_get(const struct kf* const kf, double* const x)
{
	assert(kf != NULL);
	assert(x != NULL);

	for (size_t i = 0; i < kf->n_state; i++)
		x[i] = gsl_vector_get(kf->x, i);
}

void kf_reset(const struct kf* const kf)
{
	assert(kf != NULL);

	gsl_vector_set_zero(kf->x);
	gsl_matrix_set_zero(kf->p);
}

/**** END OF FILE ****/
