#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_
struct pid_coeffs
{
    float kp;
    float ki;
    float kd;
};
float incrPID(float target, float feedback, struct pid_coeffs *coeffs);

#endif // !_ALGORITHMS_H_