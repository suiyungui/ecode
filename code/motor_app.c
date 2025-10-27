/*
 * motor_app.c
 *
 *  Created on: 2025年10月24日
 *      Author: suiyungui
 */
#include "motor_app.h"
#define MAX_DUTY            (30)                                                // 最大 测试 占空比

int8 duty = 30;
bool dir = true;

void motor_proc(void)
{
    // 此处编写需要循环执行的代码

   small_driver_set_duty(-duty * (PWM_DUTY_MAX / 100), duty * (PWM_DUTY_MAX / 100));   // 计算占空比输出

   printf("left speed:%d, right speed:%d\r\n", motor_value.receive_left_speed_data, motor_value.receive_right_speed_data);
    // 此处编写需要循环执行的代码
}



