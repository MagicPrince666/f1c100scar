#ifndef __MOTO_H__
#define __MOTO_H__

class Moto
{
public:
   Moto(void);
   ~Moto(void);
   int go(int speed);
   int back(int speed);
   int stop(void);
   int servo(int angle);
   int gpio_init(int *fd, int pin, bool io);

private:
    char setpin[64] = {0};
    int ena = -1;
    int enb = -1;

};

#endif
