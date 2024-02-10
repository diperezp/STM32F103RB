/*
 * gpio_driver_stm32f103rb_hal.h
 * Description: driver de los periferico input y output
 * de proposito general}
 *
 *  Created on: Feb 10, 2024
 *      Author: diego
 */

#ifndef GPIO_DRIVER_STM32F103RB_HAL_H_
#define GPIO_DRIVER_STM32F103RB_HAL_H_

#include <stdint.h>
#include <stm32f1xx.h>
#include <stm32f103xb.h>


// Parametros de activacion
enum{
	GPIO_PIN_RESET=0,
	GPIO_PIN_SET,
};

// Tipos de Funcionamiento del pin

enum{
	GPIO_MODE_IN=0,
	GPIO_MODE_OUT,
	GPIO_MODE_ALTFN,
	GPIO_MODE_ANALOG
};

// Tipo de salida del pin
enum{
	GPIO_OTYPE_PUSHPULL=0,
	GPIO_OTYPE_OPENDRAIN,
};

//Velocidad del Salida del pin

enum{
	GPIO_OSPEED_LOW=0,
	GPIO_OSPEED_MEDIUM,
	GPIO_OSPEED_FAST,
	GPIO_OSPEED_HIGH
};

// GPIOx_PUPDR
enum{
	GPIO_PUPDR_NOTHING =0,
	GPIO_PUPDR_PULLUP,
	GPIO_PUPDR_PULLDOWN,
	GPIO_PUPDR_RESERVED
};

// Definicion de los nombres de los pines

enum{
	PIN_0 =0,
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7,
	PIN_8,
	PIN_9,
	PIN_10,
	PIN_11,
	PIN_12,
	PIN_13,
	PIN_14,
	PIN_15
};

// Asignacion del tipo de funcion alternativa que se desea utilizar


enum{
	AF0=0,
};


/* Datos y Configuracion del pin que se desea utilizar*/


typedef struct{

	uint8_t GPIO_PinNumber;   			//Pin de trabajo
	uint8_t GPIO_PinMode;				//Modo del pin
	uint8_t GPIO_PinOutputSpeed;		//Velocidad de salida
	uint8_t GPIO_PinPuPdControl;		//Tipo de resistencia
	uint8_t GPIO_PinOutpuType;			//Tipo de salida del pin
	uint8_t GPIO_PinAltFunMode;			//Funcion alternativa configurada

}GPIO_PinConfig_t;


/* Controlador del Pin*/

typedef struct{
	GPIO_TypeDef       *pGPIOx;
	GPIO_PinConfig		pinConfig;

}GPIO_Handler_t;



/* Definimos las funciones que se desea crear para el control de los pines */

void gpio_Config(GPIO_Handñer_t *pGPIOHandler);
void gpio_WritePin(GPIO_Handler_t *pGPIOHandler,uint8_t newState);
void gpio_TooglePin(GPIO_Handler_t *pPinHandler);
uint8_t gpio_ReadPin(GPIO_Handlr_t *pPinHandler);

#endif /* GPIO_DRIVER_STM32F103RB_HAL_H_ */
