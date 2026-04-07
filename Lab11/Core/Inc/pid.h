#ifndef PID_H
#define PID_H

typedef struct {
    float kp; // Proportional gain
    float ki; // Integral gain  
    float kd; // Derivative gain
   
    float setpoint; // Desired angle (0 degrees for upright)
    float prev_error; // Previous error for derivative
    float integral; // Integral accumulator
    float output_min; // Minimum output limit
    float output_max; // Maximum output limit
   
    float dt; // Sample time
} PID_Handle_t;

void PID_Init(PID_Handle_t* pid, float kp, float ki, float kd, float dt, float min_out, float max_out);
float PID_Update(PID_Handle_t* pid, float measured_value);

#endif

