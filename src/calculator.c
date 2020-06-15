/*
 * calculator.c
 *
 *  Created on: Sep 21, 2019
 *      Author: DiaaEldeen
 */

#include "calculator.h"



extern uint8_t ERR_FLAG;
uint8_t ERR_FLAG = 0;



int64_t eval_ints(char* rawStr){
	int32_t numArr[20];
	char signArr[20];

	if (parse_ints(rawStr, numArr, signArr ) == -1) return -1;
	return calculate_ints(numArr, signArr);
}

int32_t calculate_ints(int32_t* numArr, char* signArr) {
	long long result =0, test =0;
	long results[20];

	uint8_t RI =0, i=0;
	for (i=0; signArr[i] != '\0'; i++) {
		if (signArr[i] == '+' || signArr[i] == '-') {
			results[RI++] = numArr[i];
		}
		else if (signArr[i] == '*') {
			//check for over flow
			test = (long long) numArr[i] * numArr[i+1];
			if (int32_limit(test)) {ERROR_print("overflow error"); return -1;}
			//
			numArr[i+1] *= numArr[i];
		}
		else if (signArr[i] == '/') {
			//check for divide by 0
			if (numArr[i+1] == 0) {ERROR_print("divide by 0 error"); return -1;}
			//check for over flow
			test = (long long) numArr[i] / numArr[i+1];
			if (int32_limit(test)) {ERROR_print("overflow error"); return -1;}
			//
			numArr[i+1] = numArr[i] / numArr[i+1];
		}
	}
	results[RI++] = numArr[i];

	for (i=0; i<RI; i++) {
		result += results[i];
	}
	return result;
}

//take the rawStr as input, and outputs the numArr containing individual numbers
//in order and signArr containing individual signs (- + * /) in order
//it also performs error checks
int8_t parse_ints(char* rawStr, int32_t* numArr, char* signArr){

	removeSpaces(rawStr);

	char* pRemainder;
	uint8_t NAI =0, SAI =0;

	long ret = strtol(rawStr, &pRemainder, 10);
	if (int32_limit(ret)) {ERROR_print("overflow error"); return -1;}
	numArr[NAI++] = ret;

	while (1) {
		while (*pRemainder > '9' || *pRemainder < '0') {
			if (*pRemainder == '\0') break;	//stop when the end of the string is reached
			else if ( *pRemainder == '-') {
				if ( pRemainder[-1] != '*' && pRemainder[-1] != '/' )
					signArr[SAI++] = '-';
				if ( pRemainder[1] > '9' || pRemainder[1] < '0' ){
					ERROR_print("multiple signs error"); //if the next is NAN
					return -1;
				}
				break;
			}
			else if ( *pRemainder == '+' ) {
				if ( pRemainder[1] > '9' || pRemainder[1] < '0' ){
					ERROR_print("multiple signs error"); //if the next is NAN
					return -1;
				}
				signArr[SAI++] = '+';
				*pRemainder++;
			}
			else if ( *pRemainder == '*' ) {
				if ( (pRemainder[1] > '9' || pRemainder[1] < '0') && pRemainder[1] != '-' ){
					ERROR_print("multiple signs error"); //if the next is NAN nor '-'
					return -1;
				}
				signArr[SAI++] = '*';
				*pRemainder++;
			}
			else if ( *pRemainder == '/' ) {
				if ((pRemainder[1] > '9' || pRemainder[1] < '0') && pRemainder[1] != '-'){
					ERROR_print("multiple signs error"); //if the next is NAN nor '-'
					return -1;
				}
				signArr[SAI++] = '/';
				*pRemainder++;
			}
			else {ERROR_print("wrong input"); return -1;}
		}

		if (*pRemainder == '\0') {
			signArr[SAI] = '\0';
			break;
		}

		rawStr = pRemainder;
		ret = strtol(rawStr, &pRemainder, 10);

		if (int32_limit(ret)) {ERROR_print("overflow error"); return -1;}
		numArr[NAI++] = ret;
	}
	return 0;	//success
}


// Function to remove all spaces from a given string
void removeSpaces(char *str)
{
	int count = 0; // To keep track of non-space character count

	// Traverse the given string. If current character
	// is not space, then place it at index 'count++'
	uint8_t i=0;
	for (i=0; str[i]; i++)
		if (str[i] != ' ')
			str[count++] = str[i];

	str[count] = '\0';
}


