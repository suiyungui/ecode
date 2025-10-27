/*
 * Scheduler.c
 *
 *  Created on: 2025年4月20日
 *      Author: suiyungui
 */

#include "Scheduler.h"


// 全局变量，用于存储任务数量
uint8_t task_num;
volatile uint32_t uwtick = 0;    // 系统毫秒节拍，由PIT中断更新

typedef struct {
    void (*task_func)(void);
    uint32_t rate_ms;
    uint32_t last_run;
} task_t;

// 静态任务数组，每个任务包含：任务执行周期（毫秒）、上次运行时间（毫秒）
static task_t scheduler_task[] =
{
//        {imu_test,5,0},    // 示例：10ms运行一次
//        {motor_proc,10,0},
        {remote_control,20,0},
        {mode_control,100,0}
};

/**
 * @brief 调度器初始化函数
 * 计算任务数组元素个数，将任务数量存储到 task_num 中
 */
void scheduler_init(void)
{
    // 计算任务数组元素个数，将任务数量存储到 task_num 中
    task_num = sizeof(scheduler_task) / sizeof(task_t);
}

/**
 * @brief 调度器运行函数
 * 遍历任务数组，检查是否有任务需要执行。若当前时间已经达到任务执行周期，则执行该任务并更新上次运行时间
 */
void scheduler_run(void)
{
    // 遍历任务数组中的所有任务
    for (uint8_t i = 0; i < task_num; i++)
    {
        // 获取当前的系统时间（毫秒）
        //uint32_t now_time = system_getval_ms();
        uint32_t now_time = uwtick;
        // 检查当前时间是否达到任务执行时间
        if (now_time >= scheduler_task[i].rate_ms + scheduler_task[i].last_run)
        {
            // 更新任务上次运行时间为当前时间
            scheduler_task[i].last_run = now_time;

            // 执行该任务
            scheduler_task[i].task_func();
        }
    }
}






