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

#ifdef SPI_HARD
#include "spi.h"
#else
#include "gpio.h"
#endif

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

#ifndef SPI_HARD
int ps2_ack = -1;
int ps2_data = -1;
int ps2_cmd = -1;
int ps2_att = -1;
int ps2_clk = -1;

Gpio gpio_ps2;
#endif

void PS2_Init(void)
{
#ifdef SPI_HARD
	SPI_Open();//硬件SPI初始化
#else
	gpio_ps2.gpio_init(&ps2_ack , 0, 0);//PA0
	gpio_ps2.gpio_init(&ps2_data , 128 + 10, 0);//PE10
	gpio_ps2.gpio_init(&ps2_cmd , 128 + 8, 1);//PE8
	gpio_ps2.gpio_init(&ps2_att , 128 + 7, 1);//PE7
	gpio_ps2.gpio_init(&ps2_clk , 128 + 9, 1);//PE9
#endif							  
}

//
void PS2_Cmd(u_int8_t CMD)
{
#ifdef SPI_HARD
	write(g_SPI_Fd, &CMD, 1);
#else
	volatile u_int16_t ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			PS2_JOYPAD_CMND_1;                   //
		}
		else PS2_JOYPAD_CMND_0;

		PS2_JOYPAD_CLOCK_1;                        //
		delay_us(5);
		PS2_JOYPAD_CLOCK_0;
		delay_us(5);
		PS2_JOYPAD_CLOCK_1;
		int data;
		read(ps2_data,&data,1);
		if(data == '1')
			Data[1] = ref|Data[1];
	}
#endif
	delay_us(16);
}


u_int8_t PS2_RedLight(void)
{
//	PS2_JOYPAD_ATT_0;
	PS2_Cmd(Comd[0]);  
	PS2_Cmd(Comd[1]);  
//	PS2_JOYPAD_ATT_1;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}

void PS2_ReadData(void)
{
#ifdef SPI_HARD
	PS2_Cmd(Comd[0]);  
	PS2_Cmd(Comd[1]);
	read(g_SPI_Fd,&Data,2);
#else
	volatile u_int8_t byte=0;
	volatile u_int16_t ref=0x01;

	PS2_JOYPAD_ATT_0;

	PS2_Cmd(Comd[0]);  
	PS2_Cmd(Comd[1]);  

	for(byte=2;byte<9;byte++)
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			PS2_JOYPAD_CLOCK_1;
			delay_us(5);
			PS2_JOYPAD_CLOCK_0;
			delay_us(5);
			PS2_JOYPAD_CLOCK_1;
			int data;
			read(ps2_data,&data,1);
			if(data == '1')
		      Data[byte] = ref|Data[byte];
		}
        delay_us(16);
	}
	PS2_JOYPAD_ATT_1;
#endif	
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
//	PS2_JOYPAD_ATT_0;
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
//	PS2_JOYPAD_ATT_1;
	delay_us(16);  
}
//short poll
void PS2_ShortPoll(void)
{
//	PS2_JOYPAD_ATT_0;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
//	PS2_JOYPAD_ATT_1;
	delay_us(16);	
}

void PS2_EnterConfing(void)
{
//	PS2_JOYPAD_ATT_0;
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
//	PS2_JOYPAD_ATT_1;
	delay_us(16);
}


void PS2_TurnOnAnalogMode(void)
{
//	PS2_JOYPAD_ATT_0;
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
//	PS2_JOYPAD_ATT_1;
	delay_us(16);
}

void PS2_VibrationMode(void)
{
//	PS2_JOYPAD_ATT_0;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
//	PS2_JOYPAD_ATT_1;
	delay_us(16);	
}
//
void PS2_ExitConfing(void)
{
//    PS2_JOYPAD_ATT_0;
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
//	PS2_JOYPAD_ATT_1;
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
