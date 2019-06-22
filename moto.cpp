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
    
    ena = gpio_init(6*32 + 4, 1);//PG4 196
	enb = gpio_init(6*32 + 5, 1);//PG5 197
    
    fprintf(ena, "1");
    fprintf(enb, "1");

    printf("init ina an inb\n");

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
    fclose(ena);
    fclose(enb);
    printf("fclose gpio\n");
    //printf("disable pwm\n");
}

FILE *Moto::gpio_init(int pin, bool io){
    FILE* set_export = NULL;

    sprintf(setpin, "/sys/class/gpio/gpio%d/direction", pin);
    if((access(setpin, F_OK)) == -1){//need creat 
        set_export = fopen ("/sys/class/gpio/export", "w");
        if(set_export == NULL){
            printf ("Can't open /sys/class/gpio/export!\n");
            //return 1;
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
        //return 2;
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
    set_export = fopen (setpin, "rw");
    if(set_export == NULL){
        printf ("open %s error\n",setpin);
        //return 3;  
    }

    return set_export;
}

int Moto::go(int speed){
    fprintf(ena, "1");
    fprintf(enb, "0");
    pwm_f1c100s.pwm_duty_cycle(0, speed);
    return 0;
}

int Moto::back(int speed){
    fprintf(ena,"0");
    fprintf(enb,"1");
    pwm_f1c100s.pwm_duty_cycle(0, speed);
    return 0;
}

int Moto::stop(void){
    fprintf(ena,"1");
    fprintf(enb,"1");
    pwm_f1c100s.pwm_duty_cycle(0, 0);
    return 0;
}

int Moto::servo(int angle){
    pwm_f1c100s.pwm_duty_cycle(1, angle);
    return 0;
}
