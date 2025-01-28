/**
 * @file   sig_handler.c
 * @brief  Unix signals handler
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
 */

#include "engine/sig_handler.h"

#include <signal.h>

int sig_handler_init(const int* const sigs, size_t size, void (*handler)(int signo))
{
	static struct sigaction sa;
	sa.sa_handler = handler;

	int err = sigfillset(&sa.sa_mask);
	if (err < 0)
		return err;

	for (size_t i = 0; i < size; i++) {
		err = sigaction(sigs[i], &sa, NULL);
		if (err < 0)
			return err;
	}

	return 0;
}

/**** END OF FILE ****/
