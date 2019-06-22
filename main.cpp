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
#include "pstwo.h"
#include "moto.h"
#include "mem_gpio.h"

using namespace std;

static void sigint_handler(int sig)
{
    
    cout << "--- quit the loop! ---" << endl;
    exit(0);
}

int main(int argc, char *argv[]){
/*
	if (gpio_mmap())
		return -1;

	f1c100s_gpio_set_pin_direction(2, 1);
    f1c100s_gpio_set_pin_direction(128+5, 1);

    f1c100s_gpio_set_pin_value(2, 1);
    f1c100s_gpio_set_pin_value(128+5, 1);
*/
	//bool status = 0;

	u_int8_t key = 0;
	u_int8_t l_lx = 0,l_ly = 0,l_rx = 0,l_ry = 0;
	u_int8_t lx,ly,rx,ry;
	int speed = 0;
	PS2_Init();
	PS2_SetInit();

	Moto moto;

	signal(SIGINT, sigint_handler);//信号处理

	while(1){

		key = PS2_DataKey();
		if(key != 0)               
		{
			if(key > 1)
				printf("key = %d\r\n",key);

			if(key == 12)
			{
				PS2_Vibration(0xFF,0x00);
				usleep(500000);
			}
			else if(key == 11)
			{
				PS2_Vibration(0x00,0xFF); 
				usleep(500000);
			}
			else
				PS2_Vibration(0x00,0x00); 
			
			switch(key)
			{
				case 5:;
				case 7:moto.servo(1500000);break;
				case 6:moto.servo(2000000);break;
				case 8:moto.servo(1000000);break;
			}
		}
		
		lx = PS2_AnologData(PSS_LX);
		ly = PS2_AnologData(PSS_LY);
		rx = PS2_AnologData(PSS_RX);
		ry = PS2_AnologData(PSS_RY);

		if(ry < 120){
			speed = (128 - ry)/128 * 1000000;
			moto.go(speed);
		}
		else if(ry > 135){
			speed = (ry -128)/128 * 1000000;
			moto.back(speed);
		}
		else{
			moto.stop();
		}
		if(l_lx != lx){
			l_lx = lx;
			printf("LX = %d\r\n",lx);
		}
		if(l_ly != ly){
			l_ly = ly;
			printf("LY = %d\r\n",ly);	
		}
		if(l_rx != rx){
			l_rx = rx;
			printf("RX = %d\r\n",rx);
		}
		if(l_ry != ry){
			l_ry = ry;
			printf("RY = %d\r\n",ry);
		}

		//status = !status;
		//Gpio::light(0,status);
		usleep(200000);
	}

	return 0;
}
