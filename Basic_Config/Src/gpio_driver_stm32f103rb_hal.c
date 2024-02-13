/*
 * gpio_driver_stm32f103rb_hal.c
 *
 *  Created on: Feb 10, 2024
 *      Author: diego
 */
#include <stdint.h>
#include "gpio_driver_stm32f103rb_hal.h"

// Bandera que indica que se configuro en output
uint8_t OutPut=0;

/* Funciones privadas de configuracion */
static void gpio_enable_clock_peripheral(GPIO_Handler_t *pGPIOHandler);
static void gpio_config_mode(GPIO_Handler_t *pGPIOHandler);
static void gpio_config_output_type(GPIO_Handler_t *pGPIOHandler);
static void gpio_config_output_speed(GPIO_Handler_t *pGPIOHandler);
static void gpio_config_pullup_pulldown(GPIO_Handler_t *pGPIOHanler);
static void gpio_config_alternate_function(GPIO_Handler_t *pGPIOHandler);


/**
 * Para cualquier periferco, hay varios pasos
 * que se deben de hacer para configurar:
 *
 * -Activar la señal de reloj del periferico
 * -Luego se debe cargar la configuracion del
 * pin que sebe de hacer dependiendo del caso
 * de utilizacion.
 */



void gpio_Config(GPIO_Handler_t *pGPIOHAndler){
	//Reiniciamos la bandera de Output
	OutPut=1;
	// 1) Activamos la señal de reloj del los registros GPIO
	gpio_enable_clock_peripheral(pGPIOHandelr);
	/**
	 * Es esencial primero activar la señal de reloj para luego
	 * si quiera intentar cargar cualquier configuracion en los
	 * registros del peripherico
	 */
	// 2) Configuramos el registro
	gpio_handler_mode(pGPIOHandler);

	// 3) Configuramos la forma de salida
	gpio_config_output_type(pGPIOHandler);


	// 5) Configuramos si se desea pull-up, pull-down or flotante
	gpio_config_pullup_pulldown(pGPIOHandler);

	// 6) Configuramos las funciones alternativas
	gpio_config_alternate_function(pGPIOHandler);

}//Fin de la activacion y configuracion del GPIO

/*
 * Enableclock signal for specific GPIOx port
 */

void gpio_enable_clock_peripheral(GPIO_Handler_t *pGPIOHandler){

	// verificamos para GPIOA
	if(pGPIOHandler->pGPIOA==GPIOA){
		// Escribimos 1 en la posicion correspondiente al GPIOA
		RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;

	}
	//verificamos para GPIOB
	else if(pGPIOHandler->pGPIOx==GPIOB){
		// Escribimos 1 en la posicion correspondiente al GPIOB
		RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;
	}
	//verificamos para GPIOC
	else if (pGPIOHandler->pGPIOx==GPIOC){
		//Escribimos 1 en la posicion correspondiente al GPIOC
		RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;
	}
	else if (pGPIOHandler->pGPIOx==GPIOD){
		//Escribimos 1 en la posicion correspondiente al GPIOD
		RCC->APB2ENR|=RCC_APB2ENR_IOPDEN;
	}
	else if (pGPIOHandler->pGPIOx==GPIOE){
		//Escribimos 1 en la posicion correspondiente al GPIOE
		RCC->APB2ENR|=RCC_APB2ENR_IOPEEN;
	}
}//ACtivacion de la señal de reloj para el puerto a utilzar



void gpio_config_mode(GPIO_Handler_t *pGPIOHandler){
	/**
	 * Configuration mode                   |CNF1|CNF0|MODE1|MODE0|
	 * _____________________________________|____|____|_____|_____|
	 * 						 |puss-pull		|	 |  0 |			  |
	 * General purpose output|Open-drain	|  0 |  1 |			  |
	 * ______________________|______________|____|____|    >00	  |
	 * Alternate Function    |Puss-pull     |	 |  0 | 		  |
	 * Output				 |Open-drain	|  1 |	1 |			  |
	 * ______________________|______________|____|____|___________|
	 * 						 |Analog		|	 |  0 |			  |
	 *  					 |Input_floating|  0 |  1 |           |
	 * 						 |______________|____|____|     00    |
	 * Input				 |Inputpulldown	|    |    |           |
	 * 						 |Input pull-up |  0 |  0 |			  |
	 * ______________________|______________|____|____|___________|
	 *
	 */

	/* Elegimos el modo en el que se quiere que actue el puerto I/O
	 * configuramos los registros CRL y CRH los cuales contiene los
	 * pines del 0 al 7 y del 8 - 15 respectivamente del GPIO a configurar
	 */
	if((pGPIOHandler->pinConfig.GPIO_PinMode==GPIO_MODE_IN)||(pGPIOHandler->pinConfig.GPIO_PinMode==GPIO_MODE_ANALOG)){
		//Configuramos si es entrada o entrada analoga

		if(pGPIOHandler->pinConfig.GPIO_PinNumber<8){
			pGPIOHandler->pGPIOx->CRL&= ~(3 << pGPIOHandler->pinConfig.GPIO_PinNumber*4); // MODE0-MODE7
		}
		else{
			pGPIOHandler->pGPIOx->CRL&= ~(3 << (pGPIOHandler->pinConfig.GPIO_PinNumber-8)*4); //MODE8-MODE15
		}
	}


	/** los bits MODE0 y MODE1 permiten configuar la velocidad de salida del PIN_x
	 *  MODE[1,0]==01 Maximum output speed 10 MHz
	 *  MODE[1,0]==10 Maximum output speed 2 MHz
	 *  MODE[1,0]==10 Maximum output speed 50 MHz
	 */


	else{
		OutPut=1;
		if(pGPIOHandler->pinConfig.GPIO_PinMode==GPIO_MODE_ALTFN){
			if(pGPIOHandler->pinConfig.GPIO_PinNumber<8){
				pGPIOHandler->pGPIOx->CRL|= (8 << pGPIOHandler->pinConfig.GPIO_PinNumber*4); // MODE0-MODE7
			}
			else{
				pGPIOHandler->pGPIOx->CRL|= (8<< (pGPIOHandler->pinConfig.GPIO_PinNumber-8)*4); //MODE8-MODE15

		}
		if(pGPIOHandler->pinConfig.GPIO_PinNumber<8){
			pGPIOHandler->pGPIOx->CRL|= (pGPIOHandler->pinConfig.GPIO_PinOutputSpeed << pGPIOHandler->pinConfig.GPIO_PinNumber*4); // MODE0-MODE7
		}
		else{
			pGPIOHandler->pGPIOx->CRL|= (pGPIOHandler->pinConfig.GPIO_PinOutputSpeed  << (pGPIOHandler->pinConfig.GPIO_PinNumber-8)*4); //MODE8-MODE15
		}
	}
}
}

void gpio_config_output_type(GPIO_Handler_t *pGPIOHandler){
	// En este condigo se configura el tipo de salida que tendra el puerto pusspull o opendrain
	if(OutPut){
		switch(pGPIOHandler->pinConfig.GPIO_PinOutpuType){
		case GPIO_OTYPE_PUSHPULL:{
			if(pGPIOHandler->pinConfig.GPIO_PinNumber<8){
				pGPIOHandler->pGPIOx->CRL&= ~(4<< pGPIOHandler->pinConfig.GPIO_PinNumber*4); // PIN_0 - PIN_7
			}
			else{
				pGPIOHandler->pGPIOx->CRL&= ~(4<< (pGPIOHandler->pinConfig.GPIO_PinNumber-8)*4); // PIN_8 PIN_15
			}
		}
		case GPIO_OTYPE_OPENDRAIN:{
			if(pGPIOHandler->pinConfig.GPIO_PinNumber<8){
				pGPIOHandler->pGPIOx->CRL|= (4<< pGPIOHandler->pinConfig.GPIO_PinNumber*4); // PIN_0 - PIN_7
			}
			else{
				pGPIOHandler->pGPIOx->CRL|= (4<< (pGPIOHandler->pinConfig.GPIO_PinNumber-8)*4); // PIN_8 PIN_15
			}
		}
		default:{
			__NOP();

		}

		}
	}

}

void gpio_config_pullup_pulldown(GPIO_Handler_t *pGPIOHandler){
	if(!OutPut){
		switch(pGPIOHandler->pinConfig.GPIO_PinPuPdControl){
		case GPIO_PUPDR_NOTHING:{

		}
		}
	}
}
