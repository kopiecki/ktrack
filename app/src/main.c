/**
 * @file   main.c
 * @brief  Application entry point
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
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
