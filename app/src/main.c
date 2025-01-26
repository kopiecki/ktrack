/**
 * @file   main.c
 * @brief  Application entry point
 * @author Daniel Kopiecki
 * @date   2025-01-25
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

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
