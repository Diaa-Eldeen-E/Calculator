/*
 * main.c
 *
 *  Created on: Sep 21, 2019
 *      Author: DiaaEldeen
 */

#include "calculator.h"
#include "UI.h"


uint32_t F_SysClk = 80000000;



int main(){

	clock_setup_MOSC(F_SysClk/1000000UL);

//	UI_LCD();	// LCD calculator user interface
	UI_UART();	// UART calculator user interface


	return 0;
}





