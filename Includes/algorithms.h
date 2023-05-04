#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_
typedef struct
{
    float kp;
    float ki;
    float kd;
} pid_coeffs_struct;

float incrPID(float target, float feedback, pid_coeffs_struct *coeffs);

#endif // !_ALGORITHMS_H_