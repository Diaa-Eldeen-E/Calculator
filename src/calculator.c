/*
 * calculator.c
 *
 *  Created on: Sep 21, 2019
 *      Author: DiaaEldeen
 */

#include "calculator.h"





// A wrapper function for performing the calculations
int64_t eval_ints(char* rawStr) {

	int32_t numArr[20];
	char signArr[20];

	if ( parse_ints(rawStr, numArr, signArr) == -1 )
		return -1;

	return calculate_ints(numArr, signArr);
}



/* This functions takes a parsed input
 * An array of integer numbers numArr
 * and a corresponding array of signs (- + / *)
 * Returns the result of the numerical calculation
 * It performs a lot of error checks and sets the ERR_FLAG bit if an
 * error occurred
 */
int32_t calculate_ints(int32_t* numArr, char* signArr) {

	long long result =0;
	long results[20];

	uint8_t RI =0, i=0;
	for (i=0; signArr[i] != '\0'; i++) {

		if (signArr[i] == '+' || signArr[i] == '-')
			results[RI++] = numArr[i];

		else if (signArr[i] == '*') {

			//check for multiplication over flow
			if (int32_limit((long long) numArr[i] * numArr[i+1])) {
				ERROR_print("overflow error");
				return -1;
			}

			numArr[i+1] *= numArr[i];
		}

		else if (signArr[i] == '/') {

			//check for divide by 0
			if (numArr[i+1] == 0) {
				ERROR_print("divide by 0 error");
				return -1;
			}

			//check for division over flow
			if (int32_limit((long long) numArr[i] / numArr[i+1])) {
				ERROR_print("overflow error");
				return -1;
			}

			numArr[i+1] = numArr[i] / numArr[i+1];
		}
	}

	results[RI++] = numArr[i];

	for (i=0; i<RI; i++) {

		result += results[i];	// Add the sub results

		//check for addition over flow
		if (int32_limit(result)) {
			ERROR_print("overflow error");
			return -1;
		}
	}

	return result;
}



/* This function takes the rawStr for the numerical calculation as input
 * and parses it to numbers and signs.
 * It outputs a numbers array containing all the numbers separated by a sign
 * and the corresponding sign array containing the supported signs (- + * /).
 * It performs a lot of error checks and sets the ERR_FLAG bit if an
 * error occurred
 */
int8_t parse_ints(char* rawStr, int32_t* numArr, char* signArr) {

	removeSpaces(rawStr);

	// The string must start with a number
	if( (!(IS_DIGIT(*rawStr))) && *rawStr != '-') {
		ERROR_print("Invalid input");
		return -1;
	}

	uint8_t numArrIdx =0, signArrIdx =0;

	int32_t iNum;	// The number converted from the string
	char* pcRemainder;	// The remaining string after converting the number

	do	{

		iNum = strtol(rawStr, &pcRemainder, 10);	// Convert to long

		// Overflow check (strtol returns LONG_MAX in case of overflow)
		if (int32_limit(iNum)) {
			ERROR_print("overflow error");
			return -1;
		}

		numArr[numArrIdx++] = iNum;

		// Put the signs in the sign array
		while (! (IS_DIGIT(*pcRemainder))) {

			if (*pcRemainder == '\0')
				break;	//stop if the end of the string is reached

			else if ( *pcRemainder == '-') {

				if ( pcRemainder[-1] != '*' && pcRemainder[-1] != '/' )
					signArr[signArrIdx++] = '-';

				if ( ! (IS_DIGIT(pcRemainder[1])) ) {
					ERROR_print("multiple signs error");
					return -1;
				}

				break;
			}

			else if ( *pcRemainder == '+' ) {

				if ( ! (IS_DIGIT(pcRemainder[1])) ) { //if the next is NAN
					ERROR_print("multiple signs error");
					return -1;
				}

				signArr[signArrIdx++] = '+';
				pcRemainder++;
			}

			else if ( *pcRemainder == '*' ) {

				//if the next is NAN nor '-'
				if ( (! (IS_DIGIT(pcRemainder[1]))) && pcRemainder[1] != '-' ) {
					ERROR_print("multiple signs error");
					return -1;
				}

				signArr[signArrIdx++] = '*';
				pcRemainder++;
			}

			else if ( *pcRemainder == '/' ) {

				if ((! (IS_DIGIT(pcRemainder[1]))) && pcRemainder[1] != '-'){
					ERROR_print("multiple signs error"); //if the next is NAN nor '-'
					return -1;
				}

				signArr[signArrIdx++] = '/';
				pcRemainder++;
			}

			else {
				ERROR_print("wrong input");
				return -1;
			}

		}

		if (*pcRemainder == '\0') {
			signArr[signArrIdx] = '\0';
			break;
		}

		rawStr = pcRemainder;

	}while(1);

	return 0;	//success
}


// Function to remove all white spaces from a given string
void removeSpaces(char *str) {

	int count = 0; // To keep track of non-space character count

	/* Traverse the given string.
	 * If current character is not space, then place it at index 'count++'
	 */
	uint8_t i=0;
	for (i=0; str[i]; i++)
		if (str[i] != ' ')
			str[count++] = str[i];

	str[count] = '\0';	// Null terminator
}


