/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stm32f1xx.h>
#include <stm32f103xb.h>

void toggle(void);

int main(void)
{
	RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
	while(1){
		i++;
		if(i==1600000){
			toggle();
			i=0;
		}
	}
}

void toggle(void){
	*ODR ^=(1<<5); // cambiamos el estado del led de estado LD2
}

