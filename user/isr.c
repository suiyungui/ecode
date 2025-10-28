/*********************************************************************************************************************
 * TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
 * Copyright (c) 2022 SEEKFREE 逐飞科技
 *
 * 本文件是 TC264 开源库的一部分
 *
 * TC264 开源库 是免费软件
 * 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
 * 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
 *
 * 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
 * 甚至没有隐含的适销性或适合特定用途的保证
 * 更多细节请参见 GPL
 *
 * 您应该在收到本开源库的同时收到一份 GPL 的副本
 * 如果没有，请参阅<https://www.gnu.org/licenses/>
 *
 * 额外注明：
 * 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
 * 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
 * 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
 * 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
 *
 * 文件名称          isr
 * 公司名称          成都逐飞科技有限公司
 * 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
 * 开发环境          ADS v1.9.20
 * 适用平台          TC264D
 * 店铺链接          https://seekfree.taobao.com/
 *
 * 修改记录
 * 日期              作者                备注
 * 2022-09-15       pudding            first version
 ********************************************************************************************************************/

#include "isr_config.h"
#include "isr.h"

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0); // 允许中断嵌套
    pit_clear_flag(CCU60_CH0);
    uwtick++; // 1ms计时
    imu_proc();

    pid_gyro_loop();
}
IFX_INTERRUPT(cc60_pit_ch1_isr, 0, CCU6_0_CH1_ISR_PRIORITY)
{
    interrupt_global_enable(0); // 允许中断嵌套
    pit_clear_flag(CCU60_CH1);

    pid_angle_loop(); // 角度环
}

IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0); // 允许中断嵌套
    pit_clear_flag(CCU61_CH0);

    pid_speed_loop(); // 速度环 + 前后俯仰控制
//    pid_roll_loop();  // 横滚角控制（左右高度调节）
}
IFX_INTERRUPT(cc61_pit_ch1_isr, 0, CCU6_1_CH1_ISR_PRIORITY)
{
    interrupt_global_enable(0); // 允许中断嵌套
    pit_clear_flag(CCU61_CH1);


}
// **************************** PIT中断处理 ****************************

// **************************** 外部中断处理 ****************************
IFX_INTERRUPT(exti_ch0_ch4_isr, 0, EXTI_CH0_CH4_INT_PRIO)
{
    interrupt_global_enable(0);            // 允许中断嵌套
    if (exti_flag_get(ERU_CH0_REQ0_P15_4)) // 通道0中断
    {
        exti_flag_clear(ERU_CH0_REQ0_P15_4);
    }

    if (exti_flag_get(ERU_CH4_REQ13_P15_5)) // 通道4中断
    {
        exti_flag_clear(ERU_CH4_REQ13_P15_5);
    }
}

IFX_INTERRUPT(exti_ch1_ch5_isr, 0, EXTI_CH1_CH5_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套

    if (exti_flag_get(ERU_CH1_REQ10_P14_3)) // 通道1中断
    {
        exti_flag_clear(ERU_CH1_REQ10_P14_3);

        tof_module_exti_handler(); // ToF 模块 INT 引脚中断
    }

    if (exti_flag_get(ERU_CH5_REQ1_P15_8)) // 通道5中断
    {
        exti_flag_clear(ERU_CH5_REQ1_P15_8);
    }
}

// 摄像头pclk管脚上默认占用了 2通道外部中断并送到DMA通道，取消注释前请确保无其他应用
// IFX_INTERRUPT(exti_ch2_ch6_isr, 0, EXTI_CH2_CH6_INT_PRIO)
// {
//  interrupt_global_enable(0);                     // 允许中断嵌套
//  if(exti_flag_get(ERU_CH2_REQ7_P00_4))           // 通道2中断
//  {
//      exti_flag_clear(ERU_CH2_REQ7_P00_4);
//  }
//  if(exti_flag_get(ERU_CH6_REQ9_P20_0))           // 通道6中断
//  {
//      exti_flag_clear(ERU_CH6_REQ9_P20_0);
//  }
// }
IFX_INTERRUPT(exti_ch3_ch7_isr, 0, EXTI_CH3_CH7_INT_PRIO)
{
    interrupt_global_enable(0);            // 允许中断嵌套
    if (exti_flag_get(ERU_CH3_REQ6_P02_0)) // 通道3中断
    {
        exti_flag_clear(ERU_CH3_REQ6_P02_0);
        camera_vsync_handler(); // 摄像头帧同步统一处理函数
    }
    if (exti_flag_get(ERU_CH7_REQ16_P15_1)) // 通道7中断
    {
        exti_flag_clear(ERU_CH7_REQ16_P15_1);
    }
}
// **************************** 外部中断处理 ****************************

// **************************** DMA中断处理 ****************************
IFX_INTERRUPT(dma_ch5_isr, 0, DMA_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套
    camera_dma_handler();       // 摄像头数据完成处理函数
}
// **************************** DMA中断处理 ****************************

// **************************** 串口中断处理 ****************************
// 串口0默认用为调试串口
IFX_INTERRUPT(uart0_tx_isr, 0, UART0_TX_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套
}
IFX_INTERRUPT(uart0_rx_isr, 0, UART0_RX_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套

#if DEBUG_UART_USE_INTERRUPT   // 是否使用 debug 串口中断
    debug_interrupr_handler(); // 使用 debug 串口中断回调函数处理 debug 数据  注意：该函数会被 debug 串口接管占用
#endif                         // 如果修改了 DEBUG_UART_INDEX 但未启用对应串口的中断功能，请统一关闭
}

// 串口1默认用为摄像头数据传输
IFX_INTERRUPT(uart1_tx_isr, 0, UART1_TX_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套
}
IFX_INTERRUPT(uart1_rx_isr, 0, UART1_RX_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套
    camera_uart_handler();      // 摄像头数据接收统一处理函数
}

// 串口2默认用为无线收发模块
IFX_INTERRUPT(uart2_tx_isr, 0, UART2_TX_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套
}

IFX_INTERRUPT(uart2_rx_isr, 0, UART2_RX_INT_PRIO)  // 将串口二放置另一个CPU处理
{
    interrupt_global_enable(0);     // 允许中断嵌套
    wireless_module_uart_handler(); // 无线模块统一处理函数
}
// 串口3默认用为GPS定位模块
IFX_INTERRUPT(uart3_tx_isr, 0, UART3_TX_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套
}

IFX_INTERRUPT(uart3_rx_isr, 0, UART3_RX_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套
    gnss_uart_callback();       // GNSS接收回调函数

    uart_control_callback();
}

// 串口通讯错误处理
IFX_INTERRUPT(uart0_er_isr, 0, UART0_ER_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套
    IfxAsclin_Asc_isrError(&uart0_handle);
}
IFX_INTERRUPT(uart1_er_isr, 0, UART1_ER_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套
    IfxAsclin_Asc_isrError(&uart1_handle);
}
IFX_INTERRUPT(uart2_er_isr, 0, UART2_ER_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套
    IfxAsclin_Asc_isrError(&uart2_handle);
}
IFX_INTERRUPT(uart3_er_isr, 0, UART3_ER_INT_PRIO)
{
    interrupt_global_enable(0); // 允许中断嵌套
    IfxAsclin_Asc_isrError(&uart3_handle);
}
