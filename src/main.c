/*
 * main.c
 *
 *  Created on: Sep 21, 2019
 *      Author: DiaaEldeen
 */

#include "calculator.h"


uint32_t F_SysClk = 80000000;



int main(){

	LCD_Initialize();
	buttons_EK_setup();
	__enable_irq();

	while (1) {

		char* rawStr = keypad_get_string();

		int64_t result = eval_ints(rawStr);


		if (int32_limit(result)) { ERROR_print("overflow error"); }

		LCD_Command(returnHome);

		if (ERR_FLAG == 1) ERR_FLAG = 0;
		else LCD_Print_int_X_Y(result, 1, 2);

		while(keypad_scan() != '#'); //w8 until we press =
	}

	return 0;
}


// LCD display control buttons
void GPIOJ_handler() {

	uint32_t interruptStatus = GPIO_button->RIS;

	if (interruptStatus == P0){		// First button

		GPIO_button->ICR |= P0;
		LCD_Command(shiftDisplayLeft);
	}
	else if (interruptStatus == P1){	// Second button

		GPIO_button->ICR |= P1;
		LCD_Command(shiftDisplayRight);
	}
	else if (interruptStatus == (P0 | P1) )
		GPIO_button->ICR |= P0 | P1;
}



