#include "zf_common_headfile.h"

#pragma section all "cpu0_dsram"

int core0_main(void)
{
    clock_init(); // 获取时钟频率<务必保留>
    debug_init(); // 初始化默认的调试串口
    // 此处编写用户代码 例如外设初始化代码等
    imu_all_init();
    small_driver_uart_init(); // 初始化电机蓝牙通信串口
    steer_control_init();     // 舵机初始化
    pid_app_init();           // PID
//    wireless_uart_init();     // 无线串口初始化 不使用的时候需要注释掉
    uart_receiver_init();     // 遥控器初始化
    pit_ms_init(CCU60_CH0, 1);
    pit_ms_init(CCU60_CH1, 5);
    pit_ms_init(CCU61_CH0, 20);
    scheduler_init();

    cpu_wait_event_ready(); // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
        scheduler_run();

        // 此处编写需要循环执行的代码
    }
}
#pragma section all restore

// **************************** 代码区域 ****************************
