/*
 * Change Logs:
 * Date           Author       Notes
 * 2025-12-10     zhuqinsheng  the first version
 */
#ifndef ULOS_DEBUG_H
#define ULOS_DEBUG_H

#include "ul_config.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

void ul_assert_handler(const char *expr, const char *func, int line);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
