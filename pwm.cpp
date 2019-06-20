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

#include "pwm.h"

int Pwm::setup_pwm(int pin){
    FILE* set_export = NULL;
    sprintf(setpin,"%d",pin);
    set_export = fopen ("/sys/class/pwm/pwmchip0/export", "w");
    if(set_export == NULL)printf ("Can\'t open /sys/class/pwm/pwmchip0/export\n");
    else {
        fprintf(set_export,setpin);
    }
    fclose(set_export);
    return 0;
}

int Pwm::pwm_enable(int pin){
    FILE* set_enable = NULL;
    sprintf(setpin,"/sys/class/pwm/pwmchip0/pwm%d/enable",pin);
    set_enable = fopen (setpin, "w");
    if(set_enable == NULL)printf ("open %s error\n",setpin);
    else fprintf(set_enable,"out");
    fclose(set_enable);
    return 0;
}

int Pwm::pwm_polarity(int pin , bool value){
    FILE* set_polarity = NULL;
    sprintf(setpin,"/sys/class/pwm/pwmchip0/pwm%d/polarity",pin);
    set_polarity = fopen (setpin, "w");
    if(set_polarity == NULL)printf ("open %s error\n",setpin);
    else {
        if(!value){
            fprintf(set_polarity,"normal");
        }
        else{
            fprintf(set_polarity,"inversed");
        }
        
    }
    fclose(set_polarity);
    return 0;
}

int Pwm::pwm_period(int pin, int value){
    FILE* set_period = NULL;
    sprintf(setpin,"/sys/class/pwm/pwmchip0/pwm%d/period",pin);
    set_period = fopen (setpin, "w");
    if(set_period == NULL)printf ("open %s error\n",setpin);
    else {
        fprintf(set_period,"%d",value);  
    }
    fclose(set_period);
    return 0;
}

int Pwm::pwm_duty_cycle(int pin, int value){
    FILE* set_duty_cycle = NULL;
    sprintf(setpin,"/sys/class/pwm/pwmchip0/pwm%d/duty_cycle",pin);
    set_duty_cycle = fopen (setpin, "w");
    if(set_duty_cycle == NULL)printf ("open %s error\n",setpin);
    else {
        fprintf(set_duty_cycle,"%d",value);  
    }
    fclose(set_duty_cycle);
    return 0;
}
