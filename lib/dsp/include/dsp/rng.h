/**
 * @file   rng.h
 * @brief  Random number generator with gaussian (normal) distribution
 * @author Daniel Kopiecki
 * @date   2025-01-25
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef RNG_H_INCLUDED_
#define RNG_H_INCLUDED_

#include <gsl/gsl_rng.h>

/**
 * @brief Random number generator with gaussian (normal distribution)
 */
struct rng_gauss {
	gsl_rng* rng;  /**< GSL generic number generator*/
	double stddev; /**< Standard deviation*/
	double mean;   /**< Mean*/
};

/**
 * @brief Initialize random number generator
 *
 * @param[in] n      Instance pointer
 * @param[in] stddev Standard deviation
 * @param[in] mean   Mean
 * @return int       0 on success, negative error code otherwise
 */
int rng_gauss_init(struct rng_gauss* const n, double stddev, double mean);

/**
 * @brief Destroy random number generator
 *
 * @param[in] n Instance pointer
 */
void rng_gauss_destroy(struct rng_gauss* const n);

/**
 * @brief Get random number with gaussian distribution
 *
 * @param[in] n   Instance pointer
 * @return double Random number
 */
double rng_gauss_get(const struct rng_gauss* const n);

#endif /* RNG_H_INCLUDED_ */

/**** END OF FILE ****/
