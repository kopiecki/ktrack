/*
 * @file   rng.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef NOISE_H_INCLUDED_
#define NOISE_H_INCLUDED_

#include <gsl/gsl_rng.h>

struct rng_gauss {
	gsl_rng* rng;
	double stddev;
	double mean;
};

int rng_gauss_init(struct rng_gauss* const n, double stddev, double mean);
void rng_gauss_destroy(struct rng_gauss* const n);

double rng_gauss_get(const struct rng_gauss* const n);

#endif /* NOISE_H_INCLUDED_ */

/**** END OF FILE ****/
