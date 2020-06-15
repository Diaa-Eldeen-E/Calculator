/*
 * UI.c
 *
 *  Created on: Jun 14, 2020
 *      Author: Diaa Eldeen
 */


#include "UI.h"



void ERROR_print(char* str) {

	LCD_clear;
	LCD_Print_String_X_Y(str, 1, 1);
	ERR_FLAG = 1;
}


void UI_LCD() {


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
}


// Get a numerical calculation string from the keypad (ends with '=')
char* keypad_get_string(){

	LCD_clear

	uint8_t pStringSize = 8;
	char* pString = malloc(pStringSize);
	char keyPressed =0;

	int8_t i =0;
	for (i =0; i<39; i++)
	{
		//double allocated space  whenever current space is full
		if( (i+2) >= pStringSize ) {
			 pStringSize *=2;
			pString = realloc(pString, pStringSize);
		}
		keyPressed=keypad_scan();

		if (keyPressed =='#'){
			pString[i] = '\0';
			break;
		}
		else if (keyPressed =='A') keyPressed = '/';
		else if (keyPressed =='B') keyPressed = '*';
		else if (keyPressed =='C') keyPressed = '+';
		else if (keyPressed =='D') keyPressed = '-';
		else if (keyPressed =='*') {
			free (pString);
			return keypad_get_string();
		}

		LCD_Print_Char_X_Y(keyPressed, i+1, 1);
		if (i>10) LCD_Command(shiftDisplayLeft);
		pString[i] = keyPressed;
	}

	return pString;
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
