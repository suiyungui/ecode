/*
 * uart_app.c
 *
 *  Created on: 2025��8��8��
 *      Author: uuiuiu
 */

#include "uart_app.h"


void uart_rec_init(void)
{
    uart_init(UART_1, 460800, UART1_TX_P02_2, UART1_RX_P02_3);             // ��ʼ������

    uart_write_string(UART_1, "UART Text.");                                // ���������Ϣ
    uart_write_byte(UART_1, '\r');                                          // ����س�
    uart_write_byte(UART_1, '\n');                                          // �������
}

//====================================================my_printf �Զ����ʽ�����====================================================
/**
 * @brief       ����UART_1�ĸ�ʽ���������������printf����
 * @param       format      ��ʽ���ַ�����֧��printf��׼��ʽ
 * @param       ...         �ɱ�����б�
 * @return      void
 * @note        �˺�������ʽ������ַ���ͨ��UART_1���
 *              ʹ��ǰ��Ҫ�ȵ���uart_rec_init()��ʼ��UART_1
 *              ����������Ϊ512�ַ�
 * Sample usage:    my_printf("Hello World!\r\n");
 *                  my_printf("Value: %d, String: %s\r\n", 123, "test");
 */
void my_printf(const char *format, ...)
{
    char buffer[512];                                                        // ��ʽ�����������
    va_list args;                                                           // �ɱ�����б�

    va_start(args, format);                                                 // ��ʼ���ɱ�����б�
    vsprintf(buffer, format, args);                                         // ��ʽ���ַ�����������
    va_end(args);                                                           // ����ɱ�����б�

    uart_write_string(UART_1, buffer);                                      // ͨ��UART_1�����ʽ���ַ���
}






