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

#include "pstwo.h"
#include "gpio.h"

u_int16_t Handkey;
u_int8_t Comd[2]={0x01,0x42};	
u_int8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //
u_int16_t MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
};	//

int ps2_data = -1;
int ps2_cmd = -1;
int ps2_att = -1;
int ps2_clk = -1;

Gpio gpio_ps2;

void PS2_Init(void)
{
	ps2_data = gpio_ps2.gpio_init(6*32, 0);	//PG0 192
	ps2_cmd = gpio_ps2.gpio_init(6*32 + 1, 1);	//PG1 193
	ps2_att = gpio_ps2.gpio_init(6*32 + 2, 1);	//PG2 194
	ps2_clk = gpio_ps2.gpio_init(6*32 + 3, 1);	//PG3 195

	gpio_ps2.set_gpio_value(ps2_data, 1);
	gpio_ps2.set_gpio_value(ps2_cmd, 0);
	gpio_ps2.set_gpio_value(ps2_att, 1);
	gpio_ps2.set_gpio_value(ps2_clk, 1);

	printf("init PS2 gpio\n");							  
}
void PS2_Cmd(u_int8_t CMD)
{
	volatile u_int16_t ref = 0x01;
	Data[1] = 0;
	for(ref = 0x01; ref < 0x0100; ref <<= 1)
	{
		if(ref&CMD){
			gpio_ps2.set_gpio_value(ps2_cmd, 1);                   //
		} else {
			gpio_ps2.set_gpio_value(ps2_cmd, 0);
		}

		gpio_ps2.set_gpio_value(ps2_clk,1);                        //
		delay_us(5);
		gpio_ps2.set_gpio_value(ps2_clk,0);
		delay_us(5);
		gpio_ps2.set_gpio_value(ps2_clk,1);

		if(gpio_ps2.get_gpio_value(ps2_data))
			Data[1] = ref|Data[1];
	}
	delay_us(16);
}


u_int8_t PS2_RedLight(void)
{
	gpio_ps2.set_gpio_value(ps2_att, 0);
	PS2_Cmd(Comd[0]);  
	PS2_Cmd(Comd[1]);  
	gpio_ps2.set_gpio_value(ps2_att, 1);
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}

void PS2_ReadData(void)
{
	volatile u_int8_t byte=0;
	volatile u_int16_t ref=0x01;

	gpio_ps2.set_gpio_value(ps2_att, 0);

	PS2_Cmd(Comd[0]);  
	PS2_Cmd(Comd[1]);  

	for(byte = 2; byte < 9; byte++)
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			gpio_ps2.set_gpio_value(ps2_clk,1);
			delay_us(5);
			gpio_ps2.set_gpio_value(ps2_clk,0);
			delay_us(5);
			gpio_ps2.set_gpio_value(ps2_clk,1);
			
			if(gpio_ps2.get_gpio_value(ps2_data))
		      Data[byte] = ref|Data[byte];
		}
        delay_us(16);
	}
	gpio_ps2.set_gpio_value(ps2_att, 1);	
}


u_int8_t PS2_DataKey()
{
	u_int8_t index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;         
}


u_int8_t PS2_AnologData(u_int8_t button)
{
	return Data[button];
}


void PS2_ClearData()
{
	u_int8_t a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}


void PS2_Vibration(u_int8_t motor1, u_int8_t motor2)
{
	gpio_ps2.set_gpio_value(ps2_att, 0);
	delay_us(16);
  	PS2_Cmd(0x01);  //
	PS2_Cmd(0x42);  //
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	gpio_ps2.set_gpio_value(ps2_att, 1);
	delay_us(16);  
}
//short poll
void PS2_ShortPoll(void)
{
	gpio_ps2.set_gpio_value(ps2_att,0);
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	gpio_ps2.set_gpio_value(ps2_att,1);
	delay_us(16);	
}

void PS2_EnterConfing(void)
{
  	gpio_ps2.set_gpio_value(ps2_att,0);
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	gpio_ps2.set_gpio_value(ps2_att,1);
	delay_us(16);
}


void PS2_TurnOnAnalogMode(void)
{
	gpio_ps2.set_gpio_value(ps2_att,0);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00 
	PS2_Cmd(0xEE); //Ox03
				   //0xEE
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	gpio_ps2.set_gpio_value(ps2_att,1);
	delay_us(16);
}

void PS2_VibrationMode(void)
{
	gpio_ps2.set_gpio_value(ps2_att,0);
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	gpio_ps2.set_gpio_value(ps2_att,1);
	delay_us(16);	
}
//
void PS2_ExitConfing(void)
{
    gpio_ps2.set_gpio_value(ps2_att,0);
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	gpio_ps2.set_gpio_value(ps2_att,1);
	delay_us(16);
}

void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		
	PS2_TurnOnAnalogMode();	
	PS2_VibrationMode();	
	PS2_ExitConfing();	
}