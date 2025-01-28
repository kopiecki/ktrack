/**
 * @file   types.h
 * @brief  Commonly used types and macros
 * @author Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * @date   2025-01-28
 *
 * @copyright Copyright (c) 2025 Daniel Kopiecki
 * @par       License: MIT
 */

#ifndef TYPES_H_INCLUDED_
#define TYPES_H_INCLUDED_

#include <stdint.h>

#define UNUSED(v)     (void)(v)
#define ARR_SIZE(arr) (size_t)(sizeof((arr)) / sizeof((arr)[0]))

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef int64_t i64;
typedef int32_t i32;
typedef int8_t i8;

#endif /* TYPES_H_INCLUDED_ */

/**** END OF FILE ****/
