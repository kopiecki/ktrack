/*
 * @file   noise.c
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "noise.h"

#include <assert.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

int noise_init(struct noise* const n, double stddev)
{
	assert(n != NULL);

	n->rng = gsl_rng_alloc(gsl_rng_mt19937);
	if (n->rng == NULL)
		return -ENOMEM;

	n->stddev = stddev;

	return 0;
}

void noise_destroy(struct noise* const n)
{
	assert(n != NULL);

	gsl_rng_free(n->rng);
}

double noise_get(struct noise* const n)
{
	return gsl_ran_gaussian(n->rng, n->stddev);
}

/**** END OF FILE ****/
