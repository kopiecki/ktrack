/*
 * @file   common.h
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki
 * SPDX-License-Identifier: MIT
 */

#ifndef COMMON_H_INCLUDED_
#define COMMON_H_INCLUDED_

#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef int64_t i64;
typedef int32_t i32;
typedef int8_t i8;

struct vec2 {
	i32 x;
	i32 y;
};

static inline struct vec2 vec2_create(i32 x, i32 y)
{
	return (struct vec2) {
		.x = x,
		.y = y,
	};
}

static inline struct vec2 vec2_add_v(const struct vec2 lhs, const struct vec2 rhs)
{
	return vec2_create(lhs.x + rhs.x, lhs.y + rhs.y);
}

static inline bool vec2_eq(const struct vec2 lhs, const struct vec2 rhs)
{
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

#endif /* COMMON_H_INCLUDED_ */

/**** END OF FILE ****/
