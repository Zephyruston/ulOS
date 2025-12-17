/*
 * Copyright (c) 2025 ulOS Community
 *
 * SPDX-License-Identifier: GPL-2.0-or-late
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-12-10     zhuqinsheng  the first version
 */
#include "ul_debug.h"
#include "stdio.h"

void ul_assert_handler(const char *expr, const char *func, int line)
{
    volatile char fake = 0;
    // 输出到串口
    ul_kprintf("\n!!! ASSERTION FAILED !!!\n");
    ul_kprintf("File: %s\n", func);    // 函数名
    ul_kprintf("Line: %d\n", line);    // 行号
    ul_kprintf("Expr: %s\n", expr);    // 失败的表达式 ← #EX 提供
    
    while(1) 
    {
        fake = 0;
    }
}
