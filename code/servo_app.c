/*
 * servo_app.c
 *
 *  Created on: 2025
 *      Author: suiyungui
 */
#include "servo_app.h"

steer_control_struct steer_1;
steer_control_struct steer_2;
steer_control_struct steer_3;
steer_control_struct steer_4;
int16 steer_angle = 0;

void steer_control_init(void)
{
    /* 每个舵机的范围都是3200-5700 steer_dir为1代表增加pwm 舵机向下转动 为-1代表增加pwm舵机向上转动
     * steer3和4为后侧左右舵机，steer1和2为前方左右舵机
     *
     *
     *  */
    steer_4.pwm_pin = ATOM1_CH0_P21_2; // 后方右侧   TC264为后  0为竖直向上
    steer_4.control_frequency = 300;
    steer_4.steer_dir = 1;
    steer_4.center_num = 3200; // 减小向上   3200

    steer_3.pwm_pin = ATOM1_CH1_P21_3; // 后方左侧
    steer_3.control_frequency = 300;
    steer_3.steer_dir = -1;
    steer_3.center_num = 5700; // 放大向上   5700

    steer_1.pwm_pin = ATOM1_CH2_P21_4; // 前方左侧
    steer_1.control_frequency = 300;
    steer_1.steer_dir = 1;
    steer_1.center_num = 3200; // 减小向上   3200

    steer_2.pwm_pin = ATOM1_CH3_P21_5; // 前方右侧
    steer_2.control_frequency = 300;
    steer_2.steer_dir = -1;
    steer_2.center_num = 5700; // 增大向上 5700

    steer_1.now_location = steer_1.center_num;
    steer_2.now_location = steer_2.center_num;
    steer_3.now_location = steer_3.center_num;
    steer_4.now_location = steer_4.center_num;

    pwm_init(steer_1.pwm_pin, steer_1.control_frequency, steer_1.now_location);
    pwm_init(steer_2.pwm_pin, steer_2.control_frequency, steer_2.now_location);
    pwm_init(steer_3.pwm_pin, steer_3.control_frequency, steer_3.now_location);
    pwm_init(steer_4.pwm_pin, steer_4.control_frequency, steer_4.now_location);
}
