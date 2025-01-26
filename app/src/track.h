/**
 * @file   track.h
 * @brief  Track representation
 * @author Daniel Kopiecki
 * @date   2025-01-26
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef TRACK_H_INCLUDED_
#define TRACK_H_INCLUDED_

#include "circ_buf.h"
#include "engine/engine.h"

/**
 * @brief Aggregated track configuration
 */
struct track_cfg {
	const char* const name;                  /**< Track name*/
	SDL_Color drawing_col;                   /**< Track color (mouse + text)*/
	const struct font_info* const text_font; /**< Text font info*/
	struct vec2 text_pos;                    /**< Text position on window*/
	struct vec2 sample_dims; /**< Sample dimensions (samples are rects)*/
	size_t cbuf_size;        /**< Circural buffer size*/
};

/**
 * @brief Track representation
 */
struct track {
	const char* name;     /**< Track name */
	struct circ_buf cbuf; /**< Circural buffer*/
	struct text text;     /**< Preallocated text*/
	struct rect sample;   /**< Preallocated sample*/
};

/**
 * @brief Initialize track
 *
 * @param[in] tr  Instance pointer
 * @param[in] cfg Configuration
 * @return int    0 on success, negative errno otherwise
 */
int track_init(struct track* const tr, const struct track_cfg* const cfg);

/**
 * @brief Destroy track
 *
 * @param[in] tr Instance pointer
 */
void track_destroy(struct track* const tr);

/**
 * @brief Draw track on window using SDL engine
 *
 * @param[in] tr    Instance pointer
 * @param[in] e     SDL backend reference
 * @return int      0 on success, negative errno otherwise
 */
int track_draw(struct track* const tr, struct engine* const e);

/**
 * @brief Put track information text on the window using SDL engine
 *
 * @param[in] tr  Instance pointer
 * @param[in] e   SDL backend reference
 * @return int    0 on success, negative errno otherwise
 */
int track_report(struct track* const tr, struct engine* const e);

/**
 * @brief Reset track
 *
 * @param[in] tr Instance pointer
 */
static inline void track_reset(struct track* const tr)
{
	circ_buf_reset(&tr->cbuf);
}

/**
 * @brief Add new element to the track
 *
 * @param[in] tr  Instance pointer
 * @param[in] pos Element to add
 */
static inline void track_update(struct track* const tr, struct vec2 v)
{
	circ_buf_push(&tr->cbuf, v);
}

#endif /* TRACK_H_INCLUDED_ */

/**** END OF FILE ****/
