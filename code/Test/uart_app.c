/*
 * uart_app.c
 *
 *  Created on: 2025年8月8日
 *      Author: uuiuiu
 */

#include "uart_app.h"


void uart_rec_init(void)
{
    uart_init(UART_1, 460800, UART1_TX_P02_2, UART1_RX_P02_3);             // 初始化串口

    uart_write_string(UART_1, "UART Text.");                                // 输出测试信息
    uart_write_byte(UART_1, '\r');                                          // 输出回车
    uart_write_byte(UART_1, '\n');                                          // 输出换行
}

//====================================================my_printf 自定义格式化输出====================================================
/**
 * @brief       基于UART_1的格式化输出函数，类似printf功能
 * @param       format      格式化字符串，支持printf标准格式
 * @param       ...         可变参数列表
 * @return      void
 * @note        此函数将格式化后的字符串通过UART_1输出
 *              使用前需要先调用uart_rec_init()初始化UART_1
 *              最大输出长度为512字符
 * Sample usage:    my_printf("Hello World!\r\n");
 *                  my_printf("Value: %d, String: %s\r\n", 123, "test");
 */
void my_printf(const char *format, ...)
{
    char buffer[512];                                                        // 格式化输出缓冲区
    va_list args;                                                           // 可变参数列表

    va_start(args, format);                                                 // 初始化可变参数列表
    vsprintf(buffer, format, args);                                         // 格式化字符串到缓冲区
    va_end(args);                                                           // 清理可变参数列表

    uart_write_string(UART_1, buffer);                                      // 通过UART_1输出格式化字符串
}






