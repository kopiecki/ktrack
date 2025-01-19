/*
 * @file   main.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include <SDL2/SDL_pixels.h>

#include "tracker.h"

static struct tracker tr;

int main(void)
{
	const int err = tracker_run(&tr);
	if (err < 0)
		return err;

	tracker_destroy(&tr);

	return 0;
}

/**** END OF FILE ****/
