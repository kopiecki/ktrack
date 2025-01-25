/**
 * @file   circ_buf.h
 * @brief  Trivial circural buffer implementation
 * @author Daniel Kopiecki
 * @date   2025-01-25
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef CIRC_BUF_H_INCLUDED_
#define CIRC_BUF_H_INCLUDED_

#include <stddef.h>

#include "engine/common.h"

/**
 * @brief Circural buffer representation
 */
struct circ_buf {
	struct vec2* x;   /**< Data*/
	size_t mask;      /**< Mask used for proper indexing*/
	size_t idx;       /**< Current index*/
	size_t curr_size; /**< Current size*/
};

/**
 * @brief Initialize circural buffer
 *
 * @param[in] cb   Instance pointer
 * @param[in] size Circural buffer size
 * @return int     0 on success, negative error code otherwise
 */
int circ_buf_init(struct circ_buf* const cb, size_t size);

/**
 * @brief Destroy circural buffer
 *
 * @param[in] cb Instance pointer
 */
void circ_buf_destroy(struct circ_buf* const cb);

/**
 * @brief Add new data to the buffer
 *
 * @param[in] cb Instance pointer
 * @param[in] v  Data to add
 */
void circ_buf_push(struct circ_buf* const cb, struct vec2 v);

/**
 * @brief Clear out circural buffer
 * @details After calling this function, whole data together with current size
 *          will be set to 0
 *
 * @param[in] cb Instance pointer
 */
void circ_buf_reset(struct circ_buf* const cb);

/**
 * @brief Get data from circural buffer
 *
 * @param[in] cb       Instance pointer
 * @param[in] i        Index of the data
 * @return struct vec2 Return data
 */
static inline struct vec2 circ_buf_get(const struct circ_buf* const cb, size_t i)
{
	return cb->x[(cb->idx + i) & cb->mask];
}

/**
 * @brief Get last data from the buffer
 *
 * @param[in] cb       Instance pointer
 * @return struct vec2 Last added data
 */
static inline struct vec2 circ_buf_last(const struct circ_buf* const cb)
{
	return circ_buf_get(cb, 0);
}

/**
 * @brief Get buffer capacity (allocated data size)
 *
 * @param[in] cb  Instance pointer
 * @return size_t Buffer capacity
 */
static inline size_t circ_buf_capacity(const struct circ_buf* const cb)
{
	return cb->mask + 1;
}

/**
 * @brief Get current buffer size
 * @details Current size is equal to number of pushed elements
 *
 * @param[in] cb  Instance pointer
 * @return size_t Current size
 */
static inline size_t circ_buf_curr_size(const struct circ_buf* const cb)
{
	return cb->curr_size;
}

#endif /* CIRC_BUF_H_INCLUDED_ */

/**** END OF FILE ****/
