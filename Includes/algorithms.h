#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

// #include "arm_math.h"
typedef struct
{
    float kp;
    float ki;
    float kd;
} pid_coeffs_struct;

float incrPID(float target, float feedback, pid_coeffs_struct *coeffs);
float firFilter(float value, int sequence_length);

#endif // !_ALGORITHMS_H_