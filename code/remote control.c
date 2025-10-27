#include "zf_common_headfile.h"

uint32 last_tick = 0;
int mode = 0;

void remote_control(void) // 模式选择函数
{

    if (1 == uart_receiver.finsh_flag) // 数据处理完成标志
    {
        last_tick = uwtick; // uwtick计数
        if (1 == uart_receiver.state)
        {
            // printf("CH1-CH6 data: ");
            // for (int i = 0; i < 6; i++)
            // {
            //     printf("%d ", uart_receiver.channel[i]); // 串口输出6个通道数据
            // }
            // printf("\r\n");

            if (uart_receiver.channel[3] <= 200) // 0.stop    1.auto run    2.remote control
            {
                mode = 1;
            }

            else if (1600 < uart_receiver.channel[3])
            {
                mode = 2;
            }

            else if (500 < uart_receiver.channel[3] && uart_receiver.channel[3] < 1000)
            {
                mode = 0;
            }
//            printf("mode: %d\r\n", mode);
        }

        else
        {
            mode = 0;              // 遥控器断开时进入停止模式
            pid_enable_control(0); // 能使/失效控制
            printf("Remote control has been disconnected.\r\n");
        }
        uart_receiver.finsh_flag = 0;
    }
    if (uwtick - last_tick > 50)
    {
        pid_enable_control(0); // 能使/失效控制
    }
}
