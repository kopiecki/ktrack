/**
 * @file   kf.h
 * @brief  Kalman filter implementation
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
 */

#ifndef KF_H_INCLUDED_
#define KF_H_INCLUDED_

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

/**
 * @brief Calculation by-products
 */
struct kf_byprod {
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
};

/**
 * @brief Kalman filter representation
 */
struct kf {
	size_t n_state;      /**< Number of states*/
	size_t n_meas;       /**< Measurement size*/
	gsl_vector* x;       /**< State vector*/
	gsl_vector* z;       /**< Measurement vector*/
	gsl_vector* u;       /**< Control-input vector*/
	gsl_matrix* f;       /**< State transition model*/
	gsl_matrix* h;       /**< Observation model*/
	gsl_matrix* q;       /**< Process noise covariance*/
	gsl_matrix* r;       /**< Observation noise covariance*/
	gsl_matrix* b;       /**< Control-input model*/
	gsl_matrix* p;       /**< Covariance estimate*/
	gsl_matrix* k;       /**< Kalman gain*/
	struct kf_byprod bp; /**< Preallocated calculation by-products*/
};

/**
 * @brief Initialize Kalman filter
 *
 * @param[in] kf      Instance pointer
 * @param[in] f       State transition model (F matrix, n_state x n_state)
 * @param[in] h       Observation model (H matrix, n_meas x n_state)
 * @param[in] q       Process noise covariance (Q matrix, n_state x n_state)
 * @param[in] r       Observation noise covariance (R matrix, n_meas x n_meas)
 * @param[in] b       Control-input model (B matrix, n_state x n_state)
 * @param[in] n_state State vector size (x vector, n_state)
 * @param[in] n_meas  Measurement vector size (z vector, n_meas)
 * @return int        0 on sucess, negative error code otherwise
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
 * @param[in] kf Instance pointer
 */
void kf_destroy(struct kf* const kf);

/**
 * @brief Perform prediction step
 *
 * @param[in] kf   Instance pointer
 * @param[in] u    Control-input vector (must be n_state size)
 * @return int     0 on success, negative error code otherwise
 */
int kf_predict(struct kf* const kf, const double* const u);

/**
 * @brief Perform update step
 *
 * @param[in] kf   Instance pointer
 * @param[in] z    Measurement vector (must be n_meas size)
 * @return int     0 on success, negative error code otherwise
 */
int kf_update(struct kf* const kf, const double* const z);

/**
 * @brief Obtain Kalman state prediction
 *
 * @param[in]  kf Instance pointer
 * @param[out] x  Array in which state prediction will be copied
 */
void kf_get(const struct kf* const kf, double* const x);

/**
 * @brief Reset Kalman state. This function sets current state prediction X and
 * prediction matrix P to zero.
 *
 * @param[in] kf Instance pointer
 */
void kf_reset(const struct kf* const kf);

#endif /* KF_H_INCLUDED_ */

/**** END OF FILE ****/
