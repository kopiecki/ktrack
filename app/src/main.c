/*
 * @file   main.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include <SDL2/SDL_pixels.h>

#include "app.h"

static struct app app;

int main(void)
{
	const int err = app_run(&app);
	if (err < 0)
		return err;

	app_destroy(&app);

	return 0;
}

/**** END OF FILE ****/
