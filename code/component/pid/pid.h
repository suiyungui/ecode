#ifndef __PID_H
#define __PID_H

/* pid�ṹ�� */
typedef struct
{
    float kp;                   /* ���� */
    float ki;                   /* ���� */
    float kd;                   /* ΢�� */
    float target;               /* Ŀ��ֵ */
    float current;              /* ��ǰֵ */
    float out;                  /* ִ���� */
    float limit;                /* PID(out)����޷�ֵ */

    float error;                /* ��ǰ��� */
    float last_error;           /* ��һ����� */
    float last2_error;          /* ���ϴ���� */
    float last_out;             /* ��һ��ִ���� */
    float integral;             /* ���֣��ۼӣ� */
    float p_out,i_out,d_out;    /* ���������֡�΢��ֵ */
}PID_T;

/*
    �ṩ���û����õ�API
*/
/* PID��ʼ�� */
void pid_init(PID_T * _tpPID, float _kp, float _ki, float _kd, float _target, float _limit);

/* ����PIDĿ��ֵ */
void pid_set_target(PID_T * _tpPID, float _target);

/* ����PID���� */
void pid_set_params(PID_T * _tpPID, float _kp, float _ki, float _kd);

/* ����PID����޷� */
void pid_set_limit(PID_T * _tpPID, float _limit);

/* ����PID������ */
void pid_reset(PID_T * _tpPID);

/* ����λ��ʽPID */
float pid_calculate_positional(PID_T * _tpPID, float _current);

/* ��������ʽPID */
float pid_calculate_incremental(PID_T * _tpPID, float _current);

/* �޷����� */
float pid_constrain(float value, float min, float max);
  
/* �����޷����� */  
void __attribute__((unused)) pid_app_limit_integral(PID_T *pid, float min, float max);

#endif 
