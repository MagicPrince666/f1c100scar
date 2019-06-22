#ifndef __MEM_GPIO_H__
#define __MEM_GPIO_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	

extern int gpio_mmap_fd;

int gpio_mmap(void);
int f1c100s_gpio_get_pin(int pin);
void f1c100s_gpio_set_pin_direction(int pin, int is_output);
void f1c100s_gpio_set_pin_value(int pin, int value);

#endif
