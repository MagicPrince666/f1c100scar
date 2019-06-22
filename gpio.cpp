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
#include <unistd.h>
#include <iostream>

#include "gpio.h"

Gpio::Gpio(){
}

Gpio::~Gpio(){
}

int Gpio::light(int leds, bool status){
    return 0;
}

int Gpio::setup_gpio(int pin){
    FILE* set_export = NULL;
    sprintf(setpin,"%d",pin);
    set_export = fopen ("/sys/class/gpio/export", "w");
    if(set_export == NULL)printf ("Can't open /sys/class/gpio/export!\n");
    else {
        fprintf(set_export,setpin);
    }
    fclose(set_export);
    return 0;
}

int Gpio::set_gpio_out(int pin){
    sprintf(setpin, "/sys/class/gpio/gpio%d/direction", pin);
    FILE* set_dir = fopen (setpin, "w");
    if(set_dir == NULL)printf ("open %s error\n",setpin);
    else {
        fprintf(set_dir,"out");  
    }
    fclose(set_dir);
    return 0;
}

int Gpio::set_gpio_in(int pin){
    sprintf(setpin,"/sys/class/gpio/gpio%d/direction",pin);
    FILE* set_dir = fopen (setpin, "w");
    if(set_dir == NULL)printf ("open %s error\n",setpin);
    else {
        fprintf(set_dir,"in");  
    }
    fclose(set_dir);
    return 0;
}

int Gpio::open_gpio(int *fd , int pin){
    sprintf(setpin,"/sys/class/gpio/gpio%d/value",pin);
    *fd = open (setpin, O_RDWR);
    if(*fd <= 0){
        printf ("can not open %s\n",setpin);
        return -1;
    }
    return 0;
}

int Gpio::close_gpio(int *fd){
    if(*fd > 0)
        close(*fd);
    return 0;
}

int Gpio::gpio_init(int pin, bool io){
    FILE* set_export = NULL;

    sprintf(setpin, "/sys/class/gpio/gpio%d/direction", pin);
    if((access(setpin, F_OK)) == -1){//need creat 
        set_export = fopen ("/sys/class/gpio/export", "w");
        if(set_export == NULL){
            printf ("Can't open /sys/class/gpio/export!\n");
            return -1;
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
        return -2;
    }
    else {
        if(io){
            fprintf(set_export,"out");
        }else{
            fprintf(set_export,"in");
        }
    }
    fclose(set_export);

    return pin;
}

int Gpio::set_gpio_value(int gpio, int value)
{
	FILE *p = NULL;

	sprintf(setpin, "/sys/class/gpio/gpio%d/value", gpio);

	p = fopen(setpin, "w");

	if(p != NULL){
		fprintf(p, "%d", value);
		fclose(p);
	}

	return 0;
}

int Gpio::get_gpio_value(int gpio)
{
	FILE *p = NULL;
    char data = 0;
	int value = 1;

	sprintf(setpin, "/sys/class/gpio/gpio%d/value", gpio);
	p = fopen(setpin, "r");

	if(p != NULL){
		fseek(p , 0 , 0);
		fread(&data , 1, 1 ,p);

		if(data == '0'){
			value = 0;
		}else{
			value = 1;
		}
		fclose(p);
	} else {
		value = -1;
	}
	
	return value;
}