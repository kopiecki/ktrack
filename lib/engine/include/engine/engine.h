/**
 * @file   engine.h
 * @brief  Stupid SDL backend engine
 * @author Daniel Kopiecki
 * @date   2025-01-25
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef ENGINE_H_INCLUDED_
#define ENGINE_H_INCLUDED_

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#include "engine/common.h"
#include "engine/text.h"

/**
 * @brief Forward declaration for \p struct engine_ops
 */
struct engine;

/**
 * @brief Engine operation callbacks
 */
struct engine_ops {
	int (*on_key)(struct engine* const e, SDL_Keycode key);
	int (*on_mouse)(struct engine* const e, struct vec2 pos);
	int (*on_render)(struct engine* const e);
};

/**
 * @brief SDL backend engine representation
 */
struct engine {
	SDL_Window* win;              /**< SDL window reference */
	SDL_Renderer* rndr;           /**< SDL renderer reference*/
	const struct engine_ops* ops; /**< Operation callbacks*/
	SDL_Color bg_col;             /**< Background color*/
	SDL_Event ev;                 /**< Latest cached event*/
	u32 frame_delay_msec;         /**< Event/render loop delay*/
	bool running;                 /**< Running status*/
};

/**
 * @brief Initialize SDL backend
 *
 * @param[in] e                Instance pointer
 * @param[in] name             Window name
 * @param[in] size             Window size
 * @param[in] bg_col           Background color
 * @param[in] frame_delay_msec Event/render loop delay
 * @param[in] ops              Operation callbacks
 * @return int                 0 on success, negative error code otherwise
 */
int engine_init(
	struct engine* const e,
	const char* name,
	struct vec2 size,
	const SDL_Color* const bg_col,
	u32 frame_delay_msec,
	const struct engine_ops* const ops);

/**
 * @brief Destroy SDL backend
 *
 * @param[in] e Instance pointer
 */
void engine_destroy(struct engine* const e);

/**
 * @brief      Run event/render loop
 * @details    On exiting loop, engine destroys himself
 *
 * @param[in] e    Instance pointer
 * @return int     0 on sucess, negative error code otherwise
 */
int engine_run(struct engine* const e);

/**
 * @brief Draw rectangle on window
 *
 * @param[in] e    Instance pointer
 * @param[in] r    Rectangle to draw
 * @return int     0 on success, negative error code otherwise
 */
int engine_draw_rect(struct engine* const e, const struct rect* const r);

/**
 * @brief Draw text on window
 *
 * @param[in] e     Instance pointer
 * @param[in] t     Text to draw
 * @return int      0 on sucess, negative error code otherwise
 */
int engine_draw_text(struct engine* const e, struct text* const t);

#endif /* ENGINE_H_INCLUDED_ */

/**** END OF FILE ****/
