#ifndef CODE_EKF_H_
#define CODE_EKF_H_

#include "zf_common_headfile.h"


#define DEG_TO_RAD      (57.295779513082320876798154814105f)
#define dt              (0.001f)
#define K               (1.0f)


typedef struct
{
        float gyro_x;
        float gyro_y;
        float gyro_z;
        float acc_x;
        float acc_y;
        float acc_z;
}imu_t;

extern imu_t imu_data;



void EKF_Init(void);
void EKF_UpData(void);


#endif /* CODE_EKF_H_ */
