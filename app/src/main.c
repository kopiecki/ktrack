/*
 * @file   main.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include "engine.h"

static struct engine e;

int main(void)
{
	int err = engine_init(&e, "ktrack", vec2_create(100, 100));
	if (err < 0)
		goto destroy;

	while (1) {
		engine_render(&e);
	}

destroy:
	engine_destroy(&e);

	return 0;
}

/**** END OF FILE ****/
