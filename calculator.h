/*
 * calculator.h
 *
 *  Created on: Sep 21, 2019
 *      Author: DiaaEldeen
 */

#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "LCD.h"
#include "stdlib.h"
#include "limits.h"


extern uint8_t ERR_FLAG;


#define ERROR_print(string) {\
	LCD_clear;\
	LCD_Print_String_X_Y(string, 1, 1);\
	ERR_FLAG = 1;\
}


#define int32_limit(num) (num >= LONG_MAX || num <= LONG_MIN)


int64_t eval_ints(char* rawStr);
int8_t parse_ints(char* rawStr, int32_t* numArr, char* signArr);
int32_t calculate_ints(int32_t* numArr, char* signArr);
void removeSpaces(char *str);

char* keypad_get_string();


#endif /* CALCULATOR_H_ */
