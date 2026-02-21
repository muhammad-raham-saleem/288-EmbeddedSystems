/*
 * button.h
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton
 *
 * @edit: Phillip Jones 05/30/2019 : Removed uneeded helper functions
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
/*#define GPIO_PORTE_DATA_R (*((volatile unsigned long *)0x400243FC))*/

//initialize the push buttons
void button_init();


///Non-blocking call
///Returns highest value button being pressed, 0 if no button pressed
uint8_t button_getButton();


#endif /* BUTTON_H_ */
