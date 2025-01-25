/**
 * @file   rng.c
 * @brief  Random number generator with gaussian (normal) distribution
 * @author Daniel Kopiecki
 * @date   2025-01-25
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#include "dsp/rng.h"

#include <assert.h>
#include <gsl/gsl_randist.h>

int rng_gauss_init(struct rng_gauss* const n, double stddev, double mean)
{
	assert(n != NULL);

	n->rng = gsl_rng_alloc(gsl_rng_mt19937);
	if (n->rng == NULL)
		return -ENOMEM;

	n->stddev = stddev;
	n->mean   = mean;

	return 0;
}

void rng_gauss_destroy(struct rng_gauss* const n)
{
	assert(n != NULL);

	gsl_rng_free(n->rng);
}

double rng_gauss_get(const struct rng_gauss* const n)
{
	return gsl_ran_gaussian(n->rng, n->stddev) + n->mean;
}

/**** END OF FILE ****/
