#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <iostream>

#include "moto.h"
#include "gpio.h"
#include "pwm.h"

//Gpio gpio_moto;
Pwm pwm_f1c100s;

Moto::Moto(void){
    
    gpio_init(&ena , 128 + 3, 1);//PE3
	gpio_init(&enb , 128 + 4, 1);//PE4
    write(ena,"1",1);
    write(enb,"1",1);

    pwm_f1c100s.setup_pwm(0); //pwm0
    pwm_f1c100s.pwm_polarity(0, 1);
    pwm_f1c100s.pwm_period(0, 1000000);
    pwm_f1c100s.pwm_duty_cycle(0, 500000);
    pwm_f1c100s.pwm_enable(0);

    pwm_f1c100s.setup_pwm(1); //pwm1
    pwm_f1c100s.pwm_polarity(1, 1);
    pwm_f1c100s.pwm_period(1, 20000000);
    pwm_f1c100s.pwm_duty_cycle(1, 1500000);
    pwm_f1c100s.pwm_enable(1);
    printf("init moto gpio and pwm\n");
}

Moto::~Moto(void){
    close(ena);
    close(enb);
    printf("close gpio\n");
    //printf("disable pwm\n");
}

int Moto::gpio_init(int *fd, int pin, bool io){
    FILE* set_export = NULL;

    sprintf(setpin, "/sys/class/gpio/gpio%d/direction", pin);
    if((access(setpin, F_OK)) == -1){//need creat 
        set_export = fopen ("/sys/class/gpio/export", "w");
        if(set_export == NULL){
            printf ("Can't open /sys/class/gpio/export!\n");
            return 1;
        }
        else {
            sprintf(setpin,"%d",pin);
            fprintf(set_export,setpin);
        }
        fclose(set_export);
    }

    set_export = fopen (setpin, "w");
    if(set_export == NULL){
        printf ("open %s error\n",setpin);
        return 2;
    }
    else {
        if(io){
            fprintf(set_export,"out");
        }else{
            fprintf(set_export,"in");
        }
    }
    fclose(set_export);

    sprintf(setpin,"/sys/class/gpio/gpio%d/value",pin);
    *fd = open (setpin, O_RDWR);
    if(*fd <= 0){
        printf ("can not open %s\n",setpin);
        return 3;
    }

    return 0;
}

int Moto::go(int speed){
    write(ena,"1",1);
    write(enb,"0",1);
    pwm_f1c100s.pwm_duty_cycle(0, speed);
    return 0;
}

int Moto::back(int speed){
    write(ena,"0",1);
    write(enb,"1",1);
    pwm_f1c100s.pwm_duty_cycle(0, speed);
    return 0;
}

int Moto::stop(void){
    write(ena,"1",1);
    write(enb,"1",1);
    pwm_f1c100s.pwm_duty_cycle(0, 0);
    return 0;
}

int Moto::servo(int angle){
    pwm_f1c100s.pwm_duty_cycle(1, angle);
    return 0;
}
