#ifndef CODE_PID_APP_H_
#define CODE_PID_APP_H_

#include "zf_common_headfile.h"

/* 安全保护参数 */
#define TILT_ANGLE_THRESHOLD  45.0f  // 倾角保护阈值(度)

/* 平衡控制参数结构体 */
typedef struct {
    float balance_angle;      // 平衡角度 (°)
    float target_speed;       // 目标速度
    uint8 control_enable;     // 控制使能标志 (1-使能, 0-失能)
} balance_params_t;

/* 横滚角控制参数结构体 */
typedef struct {
    float target_roll;        // 目标横滚角 (°)
} roll_params_t;

/* PID 参数结构体 */
typedef struct {
    float kp;
    float ki;
    float kd;
    float out_min;
    float out_max;
    float integral_min;
    float integral_max;
} PidParams_t;

extern balance_params_t balance_params;
extern roll_params_t roll_params;

/* PID控制接口 */
void pid_app_init(void);                    // PID应用层初始化
void pid_speed_loop(void);                  // 速度环 (20ms调用一次)
void pid_angle_loop(void);                  // 角度环 (5ms调用一次)
void pid_gyro_loop(void);                   // 角速度环 (1ms调用一次)
void pid_roll_loop(void);                   // 横滚角控制循环 (20ms调用一次)

/* 参数设置接口 */
void pid_set_balance_angle(float angle);    // 设置平衡角
void pid_set_target_speed(float speed);     // 设置目标速度
void pid_set_target_roll(float roll);       // 设置目标横滚角
void pid_enable_control(uint8 enable);      // 使能/失能控制

#endif /* CODE_PID_APP_H_ */
