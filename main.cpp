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

#include "gpio.h"
#include "pwm.h"

int main(int argc, char *argv[]){

	Gpio gpio_f1c100s;
	Pwm pwm_f1c100s;

	int PE5 = -1;//PE5 = 32*4 + 5
	int PA2 = -1;//PE5 = 32*0 + 2
	gpio_f1c100s.gpio_init(&PE5 , 128 + 5, 1);
	gpio_f1c100s.gpio_init(&PA2 , 2, 1);

	bool status = 0;
	while(1){
		status = !status;
		status == 0 ? write(PE5,"0",1) : write(PE5,"1",1);
		status == 0 ? write(PA2,"1",1) : write(PA2,"0",1);
		sleep(1);
	}

	return 0;
}
