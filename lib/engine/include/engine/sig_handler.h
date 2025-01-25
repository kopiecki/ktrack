/**
 * @file   sig_handler.h
 * @brief  Linux signals handler
 * @author Daniel Kopiecki
 * @date   2025-01-25
 *
 * SPDX-FileCopyrightText: 2025 Daniel Kopiecki <daniel.kopiecki@protonmail.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef SIG_HANDLER_H_INCLUDED_
#define SIG_HANDLER_H_INCLUDED_

#include <stddef.h>

/**
 * @brief Initialize signals handler
 *
 * @param sigs    Array of signals
 * @param size    Size of \p sigs
 * @param handler Signals handler
 * @return int    0 on success, negative error code otherwise
 */
int sig_handler_init(const int* const sigs, size_t size, void (*handler)(int signo));

#endif /* SIG_HANDLER_H_INCLUDED_ */

/**** END OF FILE ****/
