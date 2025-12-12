/*
 * Change Logs:
 * Date           Author       Notes
 * 2025-12-10     zhuqinsheng  the first version
 */
#include "ulOS_debug.h"
#include "stdio.h"

void ul_assert_handler(const char *expr, const char *func, int line)
{
    volatile char fake = 0;
    // 输出到串口
    printf("\n!!! ASSERTION FAILED !!!\n");
    printf("File: %s\n", func);    // 函数名
    printf("Line: %d\n", line);    // 行号
    printf("Expr: %s\n", expr);    // 失败的表达式 ← #EX 提供
    
    while(1) 
    {
        fake = 0;
    }
}