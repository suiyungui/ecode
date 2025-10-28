#include "zf_common_headfile.h"
int16 speed_duty = 0;

void mode_control(void) // 不同模式对应其控制方式的函数
{
    if (mode == 2) // 遥控模式
    {
        if (500 <= uart_receiver.channel[2])
        {
            pid_set_target_speed(0); // 设置目标速度
        }
        else
        {
            if (fabs(uart_receiver.channel[1] - 992) > 200 && uart_receiver.channel[1] != 0)
                speed_duty = -(uart_receiver.channel[1] - 992) / 2;

            else
                speed_duty = 0;
        }
        // if (uart_receiver.channel[0] != 0)
        //     servo_duty = -(uart_receiver.channel[0] - 992) / 8; // 通过全局变量设置
        // else
        //     servo_duty = 0;
    }
    // else if (mode == 1) // 自动运行模式
    // {
    //     // PID控制器参数
    //     static float kp = 2.0;           // 比例系数
    //     static float ki = 0.1;           // 积分系数
    //     static float kd = 0.5;           // 微分系数
    //     static float target_angle = 0.0; // 目标角度(度)
    //     static float last_error = 0.0;   // 上次误差
    //     static float integral = 0.0;     // 积分值
    //     static uint32 last_time = 0;     // 上次时间

    //     // 读取当前时间(毫秒)
    //     uint32 current_time = uwtick;
    //     float dt = (current_time - last_time) * 0.001; // 转换为秒
    //     if (dt <= 0)
    //         dt = 0.001; // 防止除0

    //     // 读取当前角度
    //     float current_angle = heading_angle_z[0];   // 当前角度
    //     float error = target_angle - current_angle; // 误差

    //     // 角度归一化(-180, 180]
    //     while (error > 180)
    //         error -= 360;
    //     while (error <= -180)
    //         error += 360;

    //     // PID计算
    //     float proportional = kp * error;                   // 比例项
    //     integral += error * dt;                            // 积分项累加
    //     integral = Limit_Min_Max(integral, -50, 50);       // 积分限幅
    //     float integral_term = ki * integral;               // 积分项
    //     float derivative = kd * (error - last_error) / dt; // 微分项

    //     // PID输出
    //     float pid_output = proportional + integral_term + derivative;

    //     // 输出控制值
    //     servo_duty = (int)Limit_Min_Max(-pid_output, -100, 100);
    //     // 更新历史值
    //     last_error = error;
    //     last_time = current_time;

    //     // 油门控制测试
    //     if (500 <= uart_receiver.channel[2])
    //     {
    //         speed_duty = 0;
    //     }
    //     else
    //     {
    //         if (fabs(uart_receiver.channel[1] - 992) > 200 && uart_receiver.channel[1] != 0)
    //             speed_duty = (uart_receiver.channel[1] - 992) * 3;
    //         else
    //             speed_duty = 0;
    //     }
    // }

    else if (mode == 0) // 停止模式
    {
        speed_duty = 0;
        // servo_duty = 0; // 通过全局变量设置
    }
//     printf("speed:%d,mode:%d\r\n",speed_duty,mode);
     pid_set_target_speed(speed_duty); // 设置目标速度
}
