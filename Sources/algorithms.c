#include "algorithms.h"
float incrPID(float target, float feedback, pid_coeffs_struct *coeffs)
{
    float error = target - feedback;
    static float error_l1 = 0.0;
    static float error_l2 = 0.0;
    float p_term, i_term, d_term;
    p_term = error - error_l1;
    i_term = error;
    d_term = error - 2 * error_l1 + error_l2;
    error_l2 = error_l1;
    error_l1 = error;
    if (error > 1)
        return 7000;
    else
        return 1000;
    return (coeffs->kp * p_term + coeffs->ki * i_term + coeffs->kd * d_term);
}

