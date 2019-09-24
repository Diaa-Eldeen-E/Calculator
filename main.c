/*
 * main.c
 *
 *  Created on: Sep 21, 2019
 *      Author: DiaaEldeen
 */

#include "calculator.h"






void main(){

	LCD_Initialize();
	buttons_EK_setup();
	__enable_irq();

	while (1) {

	char* rawStr = keypad_get_string();

	int64_t result = eval(rawStr);


	if (int32_limit(result)) { ERROR_print("overflow error"); }

	LCD_Command(returnHome);

	if (ERR_FLAG == 1) ERR_FLAG = 0;
	else LCD_Print_int_X_Y(result, 1, 2);

	while(keypad_scan() != '#'); //w8 until we press =
	}

}


void GPIOJ_handler()
{
	char interruptStatus = GPIO_button->RIS;
	if (interruptStatus == P0){
		GPIO_button->ICR |= P0;
		LCD_Command(shiftDisplayLeft);
	}
	else if (interruptStatus == P1){
		GPIO_button->ICR |= P1;
		LCD_Command(shiftDisplayRight);
	}
	else if (interruptStatus == (P0 | P1) )
		GPIO_button->ICR |= P0 | P1;
}



void SystemInit(void)
{
	SCB->CPACR |= 0x00F00000;
}
