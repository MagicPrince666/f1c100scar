#ifndef __PWM_H__
#define __PWM_H__

class Pwm
{
public:
    int setup_pwm(int pin);
    int pwm_enable(int pin);
    int pwm_polarity(int pin , bool value);
    int pwm_period(int pin, int value);
    int pwm_duty_cycle(int pin, int value);

private:
    char setpin[64] = {0};

};

#endif
