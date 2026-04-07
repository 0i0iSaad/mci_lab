#include "pid.h"
#include <stdlib.h>

void PID_Init(PID_Handle_t* pid, float kp, float ki, float kd, float dt, float min_out, float max_out) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->dt = dt;
    pid->output_min = min_out;
    pid->output_max = max_out;
    pid->setpoint = 0.0f; // Upright position = 0 degrees
    pid->prev_error = 0.0f;
    pid->integral = 0.0f;
}

float PID_Update(PID_Handle_t* pid, float measured_value) {
    float error = pid->setpoint - measured_value;
   
    float p_term = pid->kp * error;
   
    pid->integral += error * pid->dt;
   
    if (pid->integral > pid->output_max / pid->ki) {
        pid->integral = pid->output_max / pid->ki;
    } else if (pid->integral < pid->output_min / pid->ki) {
        pid->integral = pid->output_min / pid->ki;
    }
   
    float i_term = pid->ki * pid->integral;
    float derivative = (error - pid->prev_error) / pid->dt;
    float d_term = pid->kd * derivative;
   
    pid->prev_error = error;
   
    float output = p_term + i_term + d_term;
   
    if (output > pid->output_max) {
        output = pid->output_max;
    } else if (output < pid->output_min) {
        output = pid->output_min;
    }
   
    return output;
}