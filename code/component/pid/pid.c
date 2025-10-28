#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "pid.h"

/* 内部私有函数 */
static void pid_formula_incremental(PID_T * _tpPID);
static void pid_formula_positional(PID_T * _tpPID);
static void pid_out_limit(PID_T * _tpPID);

/*******************************************************************************
 * @brief PID初始化函数（用于初始化PID结构体）
 * @param {PID_T *} _tpPID 指向PID结构体的指针
 * @param {float} _kp 比例系数
 * @param {float} _ki 积分系数
 * @param {float} _kd 微分系数
 * @param {float} _target 目标值
 * @param {float} _limit 输出限幅值
 * @return {*}
 * @note 使用前必须调用此函数初始化PID参数
 *******************************************************************************/
void pid_init(PID_T * _tpPID, float _kp, float _ki, float _kd, float _target, float _limit)
{
    _tpPID->kp = _kp;          // 比例
    _tpPID->ki = _ki;          // 积分
    _tpPID->kd = _kd;          // 微分
    _tpPID->target = _target;  // 目标值
    _tpPID->limit = _limit;    // 限幅值
    _tpPID->integral = 0;      // 积分累计值
    _tpPID->last_error = 0;    // 上次误差值
    _tpPID->last2_error = 0;   // 上上次误差值
    _tpPID->out = 0;           // 输出值存储
    _tpPID->p_out = 0;         // P输出分量
    _tpPID->i_out = 0;         // I输出分量
    _tpPID->d_out = 0;         // D输出分量
}

/*******************************************************************************
 * @brief 设置PID目标值
 * @param {PID_T *} _tpPID 指向PID结构体的指针
 * @param {float} _target 目标值
 * @return {*}
 * @note 用于动态调整PID控制器的目标值
 *******************************************************************************/
void pid_set_target(PID_T * _tpPID, float _target)
{
    _tpPID->target = _target;
}

/*******************************************************************************
 * @brief 设置PID参数
 * @param {PID_T *} _tpPID 指向PID结构体的指针
 * @param {float} _kp 比例系数
 * @param {float} _ki 积分系数
 * @param {float} _kd 微分系数
 * @return {*}
 * @note 用于动态调整PID参数
 *******************************************************************************/
void pid_set_params(PID_T * _tpPID, float _kp, float _ki, float _kd)
{
    _tpPID->kp = _kp;
    _tpPID->ki = _ki;
    _tpPID->kd = _kd;
}

/*******************************************************************************
 * @brief 设置PID输出限幅
 * @param {PID_T *} _tpPID 指向PID结构体的指针
 * @param {float} _limit 限幅值
 * @return {*}
 * @note 用于动态调整PID输出限幅
 *******************************************************************************/
void pid_set_limit(PID_T * _tpPID, float _limit)
{
    _tpPID->limit = _limit;
}

/*******************************************************************************
 * @brief 重置PID控制器
 * @param {PID_T *} _tpPID 指向PID结构体的指针
 * @return {*}
 * @note 重置所有历史数据到初始状态
 *******************************************************************************/
void pid_reset(PID_T * _tpPID)
{
    _tpPID->integral = 0;
    _tpPID->last_error = 0;
    _tpPID->last2_error = 0;
    _tpPID->out = 0;
    _tpPID->p_out = 0;
    _tpPID->i_out = 0;
    _tpPID->d_out = 0;
}

/*******************************************************************************
 * @brief 位置式PID计算
 * @param {PID_T *} _tpPID 指向PID结构体的指针
 * @param {float} _current 当前值
 * @return {float} PID控制器输出值
 * @note 位置式PID：P-比例性，I-消除稳态误差，D-抑制超调
 *******************************************************************************/
float pid_calculate_positional(PID_T * _tpPID, float _current)
{
    _tpPID->current = _current;
    pid_formula_positional(_tpPID);
    pid_out_limit(_tpPID);
    return _tpPID->out;
}

/*******************************************************************************
 * @brief 增量式PID计算
 * @param {PID_T *} _tpPID 指向PID结构体的指针
 * @param {float} _current 当前值
 * @return {float} PID控制器输出值
 * @note 增量式PID：P-快速性，I-消除稳态误差，D-预测性
 *******************************************************************************/
float pid_calculate_incremental(PID_T * _tpPID, float _current)
{
    _tpPID->current = _current;
    pid_formula_incremental(_tpPID);
    pid_out_limit(_tpPID);
    return _tpPID->out;
}

/* ***************************************************************************************************** PID相关的工具函数 ***************************************************************************************************** */
/*******************************************************************************
 * @brief 输出限幅函数
 * @param {PID_T *} _tpPID 指向PID结构体的指针
 * @return {*}
 * @note 防止控制器输出超出限制范围
 *******************************************************************************/
static void pid_out_limit(PID_T * _tpPID)
{
    if(_tpPID->out > _tpPID->limit)
        _tpPID->out = _tpPID->limit;
    else if(_tpPID->out < -_tpPID->limit)
        _tpPID->out = -_tpPID->limit;
}

/*******************************************************************************
 * @brief 增量式PID公式
 * @param {PID_T *} _tpPID  需要计算的PID对象指针
 * @return {*}
 * @note 增量式PID中：P-快速性，I-消除稳态误差，D-预测性
 *******************************************************************************/
static void pid_formula_incremental(PID_T * _tpPID)
{
    _tpPID->error = _tpPID->target - _tpPID->current;

    _tpPID->p_out = _tpPID->kp * (_tpPID->error - _tpPID->last_error);
    _tpPID->i_out = _tpPID->ki * _tpPID->error;
    _tpPID->d_out = _tpPID->kd * (_tpPID->error - 2 * _tpPID->last_error + _tpPID->last2_error);

    _tpPID->out += _tpPID->p_out + _tpPID->i_out + _tpPID->d_out;

    _tpPID->last2_error = _tpPID->last_error;
    _tpPID->last_error = _tpPID->error;
}

/*******************************************************************************
 * @brief 位置式PID公式
 * @param {PID_T *} _tpPID  需要计算的PID对象指针
 * @return {*}
 * @note 位置式PID中：P-比例性，I-消除稳态误差，D-抑制超调
 *******************************************************************************/
static void pid_formula_positional(PID_T * _tpPID)
{
    _tpPID->error = _tpPID->target - _tpPID->current;

    // 积分项计算(带限幅)
    _tpPID->integral += _tpPID->error;
    // 积分限幅需要在外部通过pid_app_limit_integral设置

    _tpPID->p_out = _tpPID->kp * _tpPID->error;
    _tpPID->i_out = _tpPID->ki * _tpPID->integral;
    _tpPID->d_out = _tpPID->kd * (_tpPID->error - _tpPID->last_error);

    _tpPID->out = _tpPID->p_out + _tpPID->i_out + _tpPID->d_out;

    _tpPID->last_error = _tpPID->error;
}

/**
 * @brief 限幅函数
 * @param value 输入值
 * @param min 最小值
 * @param max 最大值
 * @return 限幅后的值
 */
float pid_constrain(float value, float min, float max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else
        return value;
}

/**
 * @brief 积分限幅函数
 * @param pid PID控制器对象
 * @param min 最小值
 * @param max 最大值
 * @note 当使用位置式PID时，此函数需要被调用为了防止积分饱和导致切换到增量式PID时震荡
 */
void __attribute__((unused)) pid_app_limit_integral(PID_T *pid, float min, float max)
{
    if (pid->integral > max)
    {
        pid->integral = max;
    }
    else if (pid->integral < min)
    {
        pid->integral = min;
    }
}
