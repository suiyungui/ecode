#include "pid_app.h"
#include <math.h>  // 用于 fabs() 函数

/* -------------------- 静态PID控制器实例与中间变量 -------------------- */
static PID_T pid_speed;   // 速度环PID (20ms)
static PID_T pid_angle;   // 角度环PID (5ms)
static PID_T pid_gyro;    // 角速度环PID (1ms)
static PID_T pid_roll;    // 横滚角控制PID (增量式, 20ms)

static float angle_offset = 0.0f;  // 速度环输出的角度扰动 (°)
static float gyro_target  = 0.0f;  // 角度环输出的角速度目标 (°/s)
static float motor_pwm    = 0.0f;  // 角速度环输出的PWM

/* 舵机控制PWM变化量 */
static int16_t front_pwm_change = 0;  // 前侧舵机PWM变化量
static int16_t rear_pwm_change = 0;   // 后侧舵机PWM变化量
static int16_t left_pwm_change = 0;   // 左侧舵机PWM变化量
static int16_t right_pwm_change = 0;  // 右侧舵机PWM变化量

float angle_target_test = 0;
float current_speed_test = 0;
int16 left_duty = 0;
int16 right_duty = 0;

// 速度环参数
PidParams_t pid_params_speed = {
    .kp = 2.5f,
    .ki = 0.0f,
    .kd = 0.0f,
    .out_min = 0.0f,
    .out_max = 2000.0f,
    .integral_min = -2.0f,
    .integral_max = 2.0f,
};

// 角度环 PD 参数
PidParams_t pid_params_angle = {
    .kp = 65.0f,    //15
    .ki = 0.0f,
    .kd = 0.0f,
//    .kd = 20.0f,
    .out_min = 0.0f,
    .out_max = 20000.0f,
    .integral_min = 0.0f,
    .integral_max = 0.0f,
};

// 角速度环 PID 参数
PidParams_t pid_params_gyro = {
    .kp = 15.0f,  // 30
    .ki = 0.0f,
//    .kd = 20.0f,
    .kd = 0.0f,
    .out_min = 0.0f,
    .out_max = 2000.0f,
    .integral_min = -4000.0f,
    .integral_max = 4000.0f,
};

// 横滚角控制PID参数 (增量式PID)
PidParams_t pid_params_roll = {
    .kp = 50.0f,    // 横滚角控制比例系数
    .ki = 0.0f,     // 横滚角控制积分系数
    .kd = 0.0f,     // 横滚角控制微分系数
    .out_min = -2000.0f,  // PWM变化量下限
    .out_max = 2000.0f,   // PWM变化量上限
    .integral_min = -100.0f,  // 积分下限
    .integral_max = 100.0f,   // 积分上限
};

/* -------------------- 平衡参数实例（默认失能） -------------------- */
balance_params_t balance_params = {
    .balance_angle  = 0.0f,
    .target_speed   = 0.0f,
    .control_enable = 0
};

/* -------------------- 横滚角控制参数实例 -------------------- */
roll_params_t roll_params = {
    .target_roll = 0.0f  // 默认目标横滚角为0°（水平）
};

/* -------------------- 内部工具函数 -------------------- */
static inline void pid_app_stop_output(void)
{
    angle_offset = 0.0f;
    gyro_target  = 0.0f;
    motor_pwm    = 0.0f;
    front_pwm_change = 0;
    rear_pwm_change = 0;
    left_pwm_change = 0;
    right_pwm_change = 0;
    small_driver_set_duty(0, 0);
}

/*
 * 函数名称: servo_apply_control
 * 功    能: 舵机统一输出函数，叠加前后和左右控制
 * 调用周期: 在pid_pitch_loop中调用
 */
static void servo_apply_control(void)
{
    if (!balance_params.control_enable)
    {
        // 失能时让所有舵机回到平衡位置
        steer_1.now_location = steer_1.center_num;
        steer_2.now_location = steer_2.center_num;
        steer_3.now_location = steer_3.center_num;
        steer_4.now_location = steer_4.center_num;
    }
    else
    {
        // 叠加前后和左右控制，通过steer_dir处理方向
        steer_1.now_location = steer_1.center_num + (front_pwm_change + left_pwm_change) * steer_1.steer_dir;
        steer_2.now_location = steer_2.center_num + (front_pwm_change + right_pwm_change) * steer_2.steer_dir;
        steer_3.now_location = steer_3.center_num + (rear_pwm_change + left_pwm_change) * steer_3.steer_dir;
        steer_4.now_location = steer_4.center_num + (rear_pwm_change + right_pwm_change) * steer_4.steer_dir;

        // 限制舵机PWM范围（3200-5700）
        steer_1.now_location = (int16_t)pid_constrain(steer_1.now_location, 3200, 5700);
        steer_2.now_location = (int16_t)pid_constrain(steer_2.now_location, 3200, 5700);
        steer_3.now_location = (int16_t)pid_constrain(steer_3.now_location, 3200, 5700);
        steer_4.now_location = (int16_t)pid_constrain(steer_4.now_location, 3200, 5700);
    }

    // 输出PWM到舵机
    pwm_set_duty(steer_1.pwm_pin, steer_1.now_location);
    pwm_set_duty(steer_2.pwm_pin, steer_2.now_location);
    pwm_set_duty(steer_3.pwm_pin, steer_3.now_location);
    pwm_set_duty(steer_4.pwm_pin, steer_4.now_location);
}

/* -------------------- 接口实现 -------------------- */

/**
 * @brief 初始化所有PID控制器
 * @param None
 * @retval None
 */
void pid_app_init(void)
{
    // 初始化速度环PID
    // pid_init参数: kp, ki, kd, target, limit
    pid_init(&pid_speed,
             pid_params_speed.kp, pid_params_speed.ki, pid_params_speed.kd,
             0.0f,                      // target: 初始目标速度为0
             pid_params_speed.out_max); // limit: 使用out_max作为对称限幅值
    pid_app_limit_integral(&pid_speed,
                          pid_params_speed.integral_min,
                          pid_params_speed.integral_max);

    // 初始化角度环PD (无积分项)
    // pid_init参数: kp, ki, kd, target, limit
    pid_init(&pid_angle,
             pid_params_angle.kp, pid_params_angle.ki, pid_params_angle.kd,
             0.0f,                       // target: 初始目标角度为0
             pid_params_angle.out_max);  // limit: 使用out_max作为对称限幅值

    // 初始化角速度环PID
    // pid_init参数: kp, ki, kd, target, limit
    pid_init(&pid_gyro,
             pid_params_gyro.kp, pid_params_gyro.ki, pid_params_gyro.kd,
             0.0f,                      // target: 初始目标角速度为0
             pid_params_gyro.out_max);  // limit: 使用out_max作为对称限幅值
    pid_app_limit_integral(&pid_gyro,
                          pid_params_gyro.integral_min,
                          pid_params_gyro.integral_max);

    // 初始化横滚角控制PID（增量式）
    // pid_init参数: kp, ki, kd, target, limit
    pid_init(&pid_roll,
             pid_params_roll.kp, pid_params_roll.ki, pid_params_roll.kd,
             0.0f,                      // target: 初始目标为0
             pid_params_roll.out_max);  // limit: 使用out_max作为对称限幅值
    pid_app_limit_integral(&pid_roll,
                          pid_params_roll.integral_min,
                          pid_params_roll.integral_max);

    // 清零内部变量并关闭输出
    pid_reset(&pid_speed);
    pid_reset(&pid_angle);
    pid_reset(&pid_gyro);
    pid_reset(&pid_roll);
    pid_app_stop_output();
}

/*
 * 函数名称: pid_pitch_loop (原pid_leg_loop)
 * 功    能: 前后俯仰控制，应用前后和左右控制到舵机
 * 调用周期: 20ms 周期性调用（在pid_speed_loop中调用）
 */
void pid_pitch_loop(void)
{
    // 调用统一舵机输出函数，叠加前后和左右控制
    servo_apply_control();
}

/*
 * 函数名称: pid_roll_loop
 * 功    能: 横滚角控制，使用增量式PID控制左右舵机高度
 * 调用周期: 20ms 周期性调用（与速度环同步）
 */
void pid_roll_loop(void)
{
    if (!balance_params.control_enable)
    {
        left_pwm_change = 0;
        right_pwm_change = 0;
        return;
    }

    // 计算横滚角误差
    float roll_error = roll_params.target_roll - roll_kalman;

    // 增量式PID计算
    pid_set_target(&pid_roll, 0.0f);  // 增量式PID目标设为0
    float roll_pwm = pid_calculate_incremental(&pid_roll, roll_error);

    // 限幅
    roll_pwm = pid_constrain(roll_pwm, pid_params_roll.out_min, pid_params_roll.out_max);

    // 映射到左右舵机PWM变化量
    if (roll_error > 0) {
        // 车体右倾（roll_error > 0），需要左侧下压修正
        left_pwm_change = (int16_t)roll_pwm;
        right_pwm_change = 0;
    } else if (roll_error < 0) {
        // 车体左倾（roll_error < 0），需要右侧下压修正
        left_pwm_change = 0;
        right_pwm_change = (int16_t)(-roll_pwm);
    } else {
        // 接近平衡，保持当前状态
        left_pwm_change = 0;
        right_pwm_change = 0;
    }
}

/*
 * 函数名称: pid_speed_loop
 * 功    能: 速度环PID控制，输出角度扰动，直接映射到前后舵机PWM变化量
 * 调用周期: 20ms 周期性调用
 */
void pid_speed_loop(void)
{
    if (!balance_params.control_enable)
    {
        angle_offset = 0.0f;
        front_pwm_change = 0;
        rear_pwm_change = 0;
        return;
    }

    // 计算平均前进速度 (左轮+右轮)/2
    float current_speed = ((float)motor_value.receive_left_speed_data -
                          (float)motor_value.receive_right_speed_data) * 0.5f;
    current_speed_test = current_speed;  // 保存原始速度用于调试观察

    // 速度环PID计算，直接输出舵机PWM变化量
    pid_set_target(&pid_speed, balance_params.target_speed);
    angle_offset = pid_calculate_positional(&pid_speed, current_speed);

    // 根据angle_offset正负分配到前/后舵机
    if (angle_offset > 0) {
        // 需要加速：前侧舵机下压
        front_pwm_change = (int16_t)angle_offset;
        rear_pwm_change = 0;
    } else {
        // 需要减速：后侧舵机下压
        front_pwm_change = 0;
        rear_pwm_change = (int16_t)(-angle_offset);
    }

    // 应用前后俯仰控制到舵机
    pid_pitch_loop();
}

/*
 * 函数名称: pid_angle_loop
 * 功    能: 角度环控制，计算角速度目标 gyro_target
 * 调用周期: 5ms 周期性调用
 */
void pid_angle_loop(void)
{
    if (!balance_params.control_enable)
    {
        gyro_target = 0.0f;
        return;
    }
    // 角度目标 = 设定平衡角 + 速度环角度扰动
    float angle_target = balance_params.balance_angle;
    angle_target_test = angle_target;
    // 设置目标角度并计算角度环PID，输出角速度目标
    pid_set_target(&pid_angle, angle_target);
    gyro_target = pid_calculate_positional(&pid_angle, pitch_kalman);
}

/*
 * 函数名称: pid_gyro_loop
 * 功    能: 角速度环控制，计算电机PWM并输出
 * 调用周期: 1ms 周期性调用
 */
void pid_gyro_loop(void)
{
    // 倾角保护: 超过阈值时立即失能
    /*
    if (fabs(pitch_kalman) > TILT_ANGLE_THRESHOLD)
    {
        pid_enable_control(0);
        return;
    }
    */
    if (!balance_params.control_enable)
    {
        pid_app_stop_output();
        return;
    }

    // 设置角速度目标并计算角速度环PID，输出PWM
    pid_set_target(&pid_gyro, gyro_target);
    motor_pwm = -pid_calculate_positional(&pid_gyro, gyro_y_rate);

    left_duty = -motor_pwm;
    right_duty = motor_pwm;

    small_driver_set_duty(left_duty, right_duty);


//    if(uwtick % 5 == 0)
//        JustFloat_Test_four((motor_value.receive_left_speed_data - motor_value.receive_right_speed_data) * 0.5f,angle_target_test,pitch_kalman,angle_offset);
//         JustFloat_Test_two(current_speed_test,angle_offset);
}


/*
 * 函数名称: pid_enable_control
 * 功    能: 使能/失能三环控制；失能时清空PID历史并停转
 * 调用周期: 根据需要调用
 */
void pid_enable_control(uint8 enable)
{
    balance_params.control_enable = enable ? 1 : 0;
    if (!balance_params.control_enable)
    {
        // 清空四个PID历史数据，防止积分等历史项影响
        pid_reset(&pid_speed);
        pid_reset(&pid_angle);
        pid_reset(&pid_gyro);
        pid_reset(&pid_roll);

        // 立即停止输出
        pid_app_stop_output();
    }
}

/*
 * 函数名称: pid_set_balance_angle
 * 功    能: 设置平衡角目标
 * 调用周期: 根据需要调用
 */
void pid_set_balance_angle(float angle)
{
    balance_params.balance_angle = angle;
}

/*
 * 函数名称: pid_set_target_speed
 * 功    能: 设置目标速度
 * 调用周期: 根据需要调用
 */
void pid_set_target_speed(float speed)
{
    balance_params.target_speed = speed;
}

/*
 * 函数名称: pid_set_target_roll
 * 功    能: 设置目标横滚角
 * 调用周期: 根据需要调用
 */
void pid_set_target_roll(float roll)
{
    roll_params.target_roll = roll;
}
