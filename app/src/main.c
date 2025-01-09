/*
 * @file   main.c
 *
 * SPDX-FileCopyrightText: 2024 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#include <signal.h>
#include <stdatomic.h>

#include "engine.h"

#define UNUSED(v)     (void)(v)
#define ARR_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

static atomic_bool quit = false;
static const int sigs[] = { SIGINT, SIGTERM };

static void quit_handler(int signo)
{
	UNUSED(signo);

	quit = true;
}

int quit_handler_init(void)
{
	static struct sigaction sa;
	sa.sa_handler = quit_handler;

	int err = sigfillset(&sa.sa_mask);
	if (err < 0)
		return err;

	for (int i = 0; i < ARR_SIZE(sigs); i++) {
		err = sigaction(sigs[i], &sa, NULL);
		if (err < 0)
			return err;
	}

	return 0;
}

static const SDL_Color bg_col     = color_blue;
static const struct vec2 win_size = { 1000, 1000 };

static struct engine e;

int run(void)
{
	int err = engine_set_background(&e, &bg_col);
	if (err < 0)
		return err;

	quit_handler_init();

	while (!quit) {
		engine_render(&e);

		struct vec2 pos = engine_poll_mouse(&e, &quit);
		SDL_Log("%d, %d", pos.x, pos.y);
	}

	return 0;
}

int main(void)
{
	int err = engine_init(&e, "ktrack", win_size);
	if (err < 0)
		goto destroy;

	err = run();

destroy:
	engine_destroy(&e);

	return err;
}

/**** END OF FILE ****/
