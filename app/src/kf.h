/*
 * @file   kf.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef KF_H_INCLUDED_
#define KF_H_INCLUDED_

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

struct kf {
	gsl_vector* x;
	gsl_vector* fx;
	gsl_vector* u;
	gsl_matrix* f;
	gsl_matrix* b;
	gsl_vector* bu;
	gsl_matrix* p;
	gsl_matrix* fp;
	gsl_matrix* fpft;
	gsl_matrix* q;
};

int kf_init(void);

int kf_predict(struct kf* const kf);
int kf_update(struct kf* const kf, const gsl_vector* const z)

#endif /* KF_H_INCLUDED_ */

	/**** END OF FILE ****/
