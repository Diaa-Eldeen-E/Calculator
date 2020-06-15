/*
 * calculator.h
 *
 *  Created on: Sep 21, 2019
 *      Author: DiaaEldeen
 */

#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "stdlib.h"
#include "limits.h"

#include "handy.h"

extern uint8_t ERR_FLAG;





#define int32_limit(num) (num >= LONG_MAX || num <= LONG_MIN)


extern void ERROR_print(char* str);

int64_t eval_ints(char* rawStr);
int8_t parse_ints(char* rawStr, int32_t* numArr, char* signArr);
int32_t calculate_ints(int32_t* numArr, char* signArr);
void removeSpaces(char *str);



#endif /* CALCULATOR_H_ */
