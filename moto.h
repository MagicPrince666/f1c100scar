#ifndef __MOTO_H__
#define __MOTO_H__

class Moto
{
public:
   void Moto_Init(void);
   int go(int speed);
   int back(int speed);
   int stop(void);
   int servo(int angle);

private:
    char setpin[64] = {0};
    int ena = -1;
    int enb = -1;

};

#endif
