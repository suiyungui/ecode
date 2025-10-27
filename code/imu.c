/*
 * imu.c
 *
 *  Created on:
 *      Author: suiyungui
 */
#include "imu.h"
#include "ekf.h"
#include "matrix.h"

float roll_kalman = 0;
float pitch_kalman = 0;
float yaw_kalman = 0;
float gyro_x_rate = 0;
float gyro_y_rate = 0;
void usart_send(float roll, float pitch, float yaw, uint8_t fusion_sta)
{
    uint8_t buffer[15];  // 数据帧缓冲区
    uint8_t sumcheck = 0; // 校验和变量
    uint8_t addcheck = 0; // 附加校验变量
    uint8_t index = 0;    // 缓冲区索引

    // 将欧拉角转换为 int16，放大100倍
    int16_t roll_int = (int16_t)(roll * 100.0f);
    int16_t pitch_int = (int16_t)(pitch * 100.0f);
    int16_t yaw_int = (int16_t)(yaw * 100.0f);

    // 帧头 (0xAB)
    buffer[index++] = 0xAB;
    // 源地址 (设置为 0xDC, 上位机控制端默认地址)
    buffer[index++] = 0xDC;
    // 目标地址 (0xFE, 广播地址)
    buffer[index++] = 0xFE;
    // 数据类型 (ID: 0x03 表示姿态融合欧拉角格式)
    buffer[index++] = 0x03;
    // 数据长度 (7字节数据)
    buffer[index++] = 7;
    buffer[index++] = 0;  // 数据长度高字节为0

    // 欧拉角数据 (int16, 放大100倍)
    buffer[index++] = (uint8_t)(roll_int & 0xFF);
    buffer[index++] = (uint8_t)((roll_int >> 8) & 0xFF);
    buffer[index++] = (uint8_t)(pitch_int & 0xFF);
    buffer[index++] = (uint8_t)((pitch_int >> 8) & 0xFF);
    buffer[index++] = (uint8_t)(yaw_int & 0xFF);
    buffer[index++] = (uint8_t)((yaw_int >> 8) & 0xFF);

    // 融合状态 (uint8)
    buffer[index++] = fusion_sta;

    // 计算校验和和附加校验值 (从帧头开始到DATA结束)
    for (int i = 0; i < index; i++)
    {
        sumcheck += buffer[i];
        addcheck += sumcheck;
    }

    // 添加校验和和附加校验值
    buffer[index++] = sumcheck;
    buffer[index++] = addcheck;

    // 发送数据帧
    for (int i = 0; i < index; i++)
    {
        printf("%c", buffer[i]);
    }
}
void imu_all_init()
{
    EKF_Init();
    imu660ra_init();
}

void imu_proc(void)// 姿态解算处理
{
    EKF_UpData();
    // 保存全局欧拉角变量供外部使用
    roll_kalman = euler_angle.roll;
    pitch_kalman = euler_angle.pitch;
    yaw_kalman = euler_angle.yaw;
    // 角速度转换为角度/秒（rad/s * 180/PI = deg/s）
    gyro_x_rate = imu_data.gyro_x * DEG_TO_RAD;
    gyro_y_rate = imu_data.gyro_y * DEG_TO_RAD;
}
/* 上位机测试 */
void imu_test(void)
{
    usart_send(roll_kalman, pitch_kalman, yaw_kalman, 1);
}



