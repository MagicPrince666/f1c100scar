#include "mem_gpio.h"

#define MMAP_PATH	"/dev/mem"

#define REG_BASE 0x01C20800
#define MAP_SIZE   0x258

#define F1C100S_GPIO_DIR_IN		0
#define F1C100S_GPIO_DIR_OUT		1

static uint8_t* gpio_mmap_reg = NULL;
int gpio_mmap_fd = 0;

int gpio_mmap(void)
{
	if ((gpio_mmap_fd = open(MMAP_PATH, O_RDWR)) < 0) {
		fprintf(stderr, "unable to open mmap file");
		return -1;
	}

	gpio_mmap_reg = (uint8_t*) mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE,
		MAP_FILE | MAP_SHARED, gpio_mmap_fd, REG_BASE);
	if (gpio_mmap_reg == MAP_FAILED) {
		perror("foo");
		fprintf(stderr, "failed to mmap");
		gpio_mmap_reg = NULL;
		close(gpio_mmap_fd);
		return -1;
	}

	return 0;
}

int f1c100s_gpio_get_pin(int pin)
{
	uint32_t tmp = 0;

	if (pin <= 31) {//PA
		tmp = *(volatile uint32_t *)(gpio_mmap_reg + 0x10);
		tmp = (tmp >> pin) & 1u;
	} else if (pin <= 63) {//PB
		tmp = *(volatile uint32_t *)(gpio_mmap_reg + 0x24 + 10);
		tmp = (tmp >> (pin-32)) & 1u;
	} else if (pin <= 95) {//PC
		tmp = *(volatile uint32_t *)(gpio_mmap_reg + 2*0x24 + 10);
		tmp = (tmp >> (pin-64)) & 1u;
	} else if (pin <= 127){//PD
        tmp = *(volatile uint32_t *)(gpio_mmap_reg + 3*0x24 + 10);
		tmp = (tmp >> (pin-96)) & 1u;
    } else if (pin <= 159){//PE
        tmp = *(volatile uint32_t *)(gpio_mmap_reg + 4*0x24 + 10);
		tmp = (tmp >> (pin-128)) & 1u;
    } else if (pin <= 191){//PF
        tmp = *(volatile uint32_t *)(gpio_mmap_reg + 5*0x24 + 10);
		tmp = (tmp >> (pin-160)) & 1u;
    }
	return tmp;

}

void f1c100s_gpio_set_pin_direction(int pin, int is_output)
{
	uint32_t tmp;

	if (pin <= 31) {//PA
		tmp = *(volatile uint32_t *)(gpio_mmap_reg + 0x0 + 0x4*(pin/4));
		if (is_output){
            tmp &=  ~(7u << (pin*4));
			tmp |=  (1u << (pin*4));
        } else {
			tmp &= ~(1u << (pin*4));
        }
		*(volatile uint32_t *)(gpio_mmap_reg + 0x0 + 0x4*(pin/4)) = tmp;
	} else if (pin <= 63) {//PB
		tmp = *(volatile uint32_t *)(gpio_mmap_reg + 0x24 + 0x4*((pin-32)/4));
		if (is_output){
            tmp &=  ~(7u << (4*(pin-32)));
			tmp |=  (1u << (4*(pin-32)));
        } else {
			tmp &= ~(1u << (4*(pin-32)));
        }
		*(volatile uint32_t *)(gpio_mmap_reg + 0x24 + 0x4*((pin-32)/4)) = tmp;
	} else if (pin <= 95) {//PC
		tmp = *(volatile uint32_t *)(gpio_mmap_reg + 2*0x24 + 0x4*((pin-64)/4));
		if (is_output){
            tmp &=  (7u << (4*(pin-64)));
			tmp |=  (1u << (4*(pin-64)));
        } else {
			tmp &= ~(1u << (4*(pin-64)));
        }
		*(volatile uint32_t *)(gpio_mmap_reg + 2*0x24 + 0x4*((pin-64)/4)) = tmp;
	} else if (pin <= 127) {//PD
		tmp = *(volatile uint32_t *)(gpio_mmap_reg + 3*0x24 + 0x4*((pin-96)/4));
		if (is_output){
			tmp |=  (1u << (4*(pin-96)));
        } else {
			tmp &= ~(1u << (4*(pin-96)));
        }
		*(volatile uint32_t *)(gpio_mmap_reg + 3*0x24 + 0x4*((pin-96)/4)) = tmp;
	} else if (pin <= 159) {//PE
		tmp = *(volatile uint32_t *)(gpio_mmap_reg + 4*0x24 + 0x4*((pin-128)/4));
		if (is_output){
			tmp |=  (1u << (pin-128));
        } else {
			tmp &= ~(1u << (pin-128));
        }
		*(volatile uint32_t *)(gpio_mmap_reg + 4*0x24 + 0x4*((pin-128)/4)) = tmp;
	} else if (pin <= 191) {//PF
		tmp = *(volatile uint32_t *)(gpio_mmap_reg + 5*0x24 + 0x4*((pin-160)/4));
		if (is_output){
			tmp |=  (1u << (pin-160));
        } else {
			tmp &= ~(1u << (pin-160));
        }
		*(volatile uint32_t *)(gpio_mmap_reg + 5*0x24 + 0x4*((pin-160)/4)) = tmp;
	}
}

void f1c100s_gpio_set_pin_value(int pin, int value)
{
	uint32_t tmp;

	if (pin <= 31) {//PA
		tmp = (1u << pin);
		if (value)
			*(volatile uint32_t *)(gpio_mmap_reg + 0x10) = tmp;
		else
			*(volatile uint32_t *)(gpio_mmap_reg + 0x10) = tmp;
	} else if (pin <= 63) {//PB
		tmp = (1u << (pin-32));
		if (value)
			*(volatile uint32_t *)(gpio_mmap_reg + 0x24 + 0x10) = tmp;
		else
			*(volatile uint32_t *)(gpio_mmap_reg + 0x24 + 0x10) = tmp;
	} else if (pin <= 95) {//PC
		tmp = (1u << (pin-64));
		if (value)
			*(volatile uint32_t *)(gpio_mmap_reg + 2*0x24 + 0x10) = tmp;
		else
			*(volatile uint32_t *)(gpio_mmap_reg + 2*0x24 + 0x10) = tmp;
	} else if (pin <= 127) {//PD
		tmp = (1u << (pin-64));
		if (value)
			*(volatile uint32_t *)(gpio_mmap_reg + 3*0x24 + 0x10) = tmp;
		else
			*(volatile uint32_t *)(gpio_mmap_reg + 3*0x24 + 0x10) = tmp;
	} else if (pin <= 159) {//PE
		tmp = (1u << (pin-64));
		if (value)
			*(volatile uint32_t *)(gpio_mmap_reg + 4*0x24 + 0x10) = tmp;
		else
			*(volatile uint32_t *)(gpio_mmap_reg + 4*0x24 + 0x10) = tmp;
	} else if (pin <= 191) {//PF
		tmp = (1u << (pin-64));
		if (value)
			*(volatile uint32_t *)(gpio_mmap_reg + 5*0x24 + 0x10) = tmp;
		else
			*(volatile uint32_t *)(gpio_mmap_reg + 5*0x24 + 0x10) = tmp;
	}
}