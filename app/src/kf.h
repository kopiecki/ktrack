/*
 * @file   kf.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef KF_H_INCLUDED_
#define KF_H_INCLUDED_

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

/**
 * struct kf - Kalman filter representation
 * @n_state: Number of states
 * @n_meas:  Measurement size
 * @x:       State vector
 * @z:       Measurement vector
 * @u:       Control-input vector
 * @f:       State transition model
 * @h:       Observation model
 * @q:       Process noise covariance
 * @r:       Observation noise covariance
 * @b:       Control-input model
 * @p:       Covariance estimate
 * @k:       Kalman gain
 * @bp:      Preallocated calculation by-products
 */
struct kf {
	size_t n_state;
	size_t n_meas;

	gsl_vector* x;
	gsl_vector* z;
	gsl_vector* u;

	gsl_matrix* f;
	gsl_matrix* h;
	gsl_matrix* q;
	gsl_matrix* r;
	gsl_matrix* b;
	gsl_matrix* p;
	gsl_matrix* k;

	struct {
		gsl_vector* fx;
		gsl_vector* hx;
		gsl_vector* kz;
		gsl_matrix* fp;
		gsl_matrix* fpft;
		gsl_matrix* khp;
		gsl_matrix* hphtr;
		gsl_matrix* pht;
		gsl_matrix* hp;
		gsl_matrix* inv;
		gsl_permutation* perm;
	} bp;
};

/**
 * @brief Initialize Kalman filter
 *
 * @param kf      Instance pointer
 * @param f       State transition model (F matrix, n_state x n_state)
 * @param h       Observation model (H matrix, n_meas x n_state)
 * @param q       Process noise covariance (Q matrix, n_state x n_state)
 * @param r       Observation noise covariance (R matrix, n_meas x n_meas)
 * @param b       Control-input model (B matrix, n_state x n_state)
 * @param n_state State vector size (x vector, n_state)
 * @param n_meas  Measurement vector size (z vector, n_meas)
 * @return int    0 on sucess, negative error code otherwise
 */
int kf_init(
	struct kf* const kf,
	const double* const f,
	const double* const h,
	const double* const q,
	const double* const r,
	const double* const b,
	size_t n_state,
	size_t n_meas);

/**
 * @brief Destroy Kalman filter
 *
 * @param kf Instance pointer
 */
void kf_destroy(struct kf* const kf);

/**
 * @brief Perform prediction step
 *
 * @param kf   Instance pointer
 * @param u    Control-input vector (must be n_state size)
 * @return int 0 on success, negative error code otherwise
 */
int kf_predict(struct kf* const kf, const double* const u);

/**
 * @brief Perform update step
 *
 * @param kf   Instance pointer
 * @param z    Measurement vector (must be n_meas size)
 * @return int 0 on success, negative error code otherwise
 */
int kf_update(struct kf* const kf, const double* const z);

/**
 * @brief Obtain Kalman state prediction
 *
 * @param kf Instance pointer
 * @param x  Array in which state prediction will be copied
 */
void kf_get(const struct kf* const kf, double* const x);

#endif /* KF_H_INCLUDED_ */

/**** END OF FILE ****/
