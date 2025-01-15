/*
 * @file   kf.c
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "kf.h"

#include <gsl/gsl_blas.h>
#include <gsl/gsl_cblas.h>

// stateMatrix = np.zeros((4, 1), np.float32)  # [x, y, delta_x, delta_y]
// estimateCovariance = np.eye(stateMatrix.shape[0])
// transitionMatrix = np.array([[1, 0, 1, 0],[0, 1, 0, 1], [0, 0, 1, 0], [0, 0,
// 0, 1]], np.float32) processNoiseCov =
// np.array([[1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1]], np.float32) * 0.001
// measurementStateMatrix = np.zeros((2, 1), np.float32)
// observationMatrix = np.array([[1,0,0,0],[0,1,0,0]],np.float32)
// measurementNoiseCov = np.array([[1,0],[0,1]], np.float32) * 1

// auto x = Mat(4, 1);
// auto p = Mat::ones(4);
// auto q = Mat::ones(4) * 0.01;
// auto z = Mat(2, 1);
// auto h = Mat({{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}});
// auto b = Mat(4, 4);
// auto m = Mat(4, 1);
// auto f =
//         Mat({{1.0, 0.0, 1.0, 0.0},
//              {0.0, 1.0, 0.0, 1.0},
//              {0.0, 0.0, 1.0, 0.0},
//              {0.0, 0.0, 0.0, 1.0}});
// auto r =
//              Mat({{MEAS_NOISE_STDDEV * MEAS_NOISE_STDDEV, 0.0},
//                   {0.0, MEAS_NOISE_STDDEV * MEAS_NOISE_STDDEV}});

// Matrix predict()
// {
//         m_x = (m_f * m_x) + (m_b * m_m);

//         auto ft = Matrix::transpose(m_f);
//         m_p     = (m_f * m_p * ft) + m_q;

//         return m_x;
// }

// void update(const Matrix& z)
// {
//         auto ht = Matrix::transpose(m_h);
//         auto k  = m_p * ht * Matrix::inv((m_h * m_p * ht) + m_r);

//         m_x += k * (z - (m_h * m_x));
//         m_p = m_p - (k * m_h * m_p);
// }

// State
static gsl_vector x;

// Prediction step
static gsl_matrix f;

// Noise covariance
static gsl_matrix q;

// Reading
static gsl_matrix z;

//
static gsl_matrix h;

//
static gsl_matrix r;

// Measurement covariance
static gsl_matrix p;

// Control
static gsl_matrix b;

//
static gsl_matrix m;

int kf_init(void)
{
}

int kf_predict(struct kf* const kf)
{
	// x(k) = F(k)x(k-1) + B(k)u(k)

	// y = alpha * Ax + beta * y
	// fx = Fx
	int err = gsl_blas_dgemv(CblasNoTrans, 1.0, kf->f, kf->x, 0.0, kf->fx);
	if (err < 0)
		return err;

	// fx = Bu + fx
	err = gsl_blas_dgemv(CblasNoTrans, 1.0, kf->b, kf->u, 1.0, kf->fx);
	if (err < 0)
		return err;

	// x = fx
	err = gsl_blas_dcopy(kf->fx, kf->x);
	if (err < 0)
		return err;

	// P(k) = F(k)P(k-1)F^T(k) + Q(k)

	// C = alpha * AB + beta * C
	// fp = FP
	err = gsl_blas_dgemm(
		CblasNoTrans, CblasNoTrans, 1.0, kf->f, kf->p, 0.0, kf->fp);
	if (err < 0)
		return err;

	// fpft = FPF^T
	err = gsl_blas_dgemm(
		CblasNoTrans, CblasTrans, 1.0, kf->fp, kf->f, 1.0, kf->fpft);
	if (err < 0)
		return err;

	err = gsl_matrix_add(kf->fpft, kf->q);
	if (err < 0)
		return err;

	err = gsl_matrix_memcpy(kf->p, kf->fpft);
	if (err < 0)
		return err;

	return err;
}

int kf_update(struct kf* const kf, const gsl_vector* const z)
{
}

/**** END OF FILE ****/
