#ifndef __GPIO_H__
#define __GPIO_H__

class Gpio
{
public:
    Gpio();
    ~Gpio();
    int light(int leds, bool status);
    int setup_gpio(int pin);
    int set_gpio_out( int pin);
    int set_gpio_in(int pin);
    int open_gpio(int *fd , int pin);
    int close_gpio(int *fd);
    int gpio_init(int *fd, int pin, bool io);

private:
    char setpin[64] = {0};
    int PE5 = -1;//PE5 = 32*4 + 5
	int PA2 = -1;//PE5 = 32*0 + 2
};

#endif
