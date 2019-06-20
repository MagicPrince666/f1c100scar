#ifndef __GPIO_H__
#define __GPIO_H__

class Gpio
{

public:
    int setup_gpio(int pin);
    int set_gpio_out( int pin);
    int set_gpio_in(int pin);
    int open_gpio(int *fd , int pin);
    int close_gpio(int *fd);
    int gpio_init(int *fd, int pin);

private:
    char setpin[64] = {0};

};

#endif
