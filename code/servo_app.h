/*
 * servo_app.h
 *
 *  Created on: 2025年10月24日
 *      Author: suiyungui
 */

#ifndef CODE_SERVO_APP_H_
#define CODE_SERVO_APP_H_

#include "zf_common_headfile.h"

typedef struct
{
    pwm_channel_enum pwm_pin;         //引脚
    uint16_t control_frequency;         //频率
    int8_t steer_dir;                 //方向
    int16_t center_num;              //平衡位置
    int16_t now_location;           //当前位置
    int16_t now_duty;                   //占空比
    int16_t center_duty;

}steer_control_struct;

void steer_control_init(void);

#endif /* CODE_SERVO_APP_H_ */
