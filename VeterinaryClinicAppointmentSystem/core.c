#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "core.h"

//Clear the standard input buffer
void clearInputBuffer(void)
{
	//Discard all remaining char's from the standard input buffer:
	while (getchar() != '\n')
	{
		;
	}
	return;
}

// Wait for user to input the "enter" key to continue
void suspend(void)
{
	printf("<ENTER> to continue...");
	clearInputBuffer();
	putchar('\n');
	return;
}

// 1.Get integer input function definition
int inputInt(void) {
	int value, flage = 0;
	char newLine = 'x';
	do {
		scanf("%d%c", &value, &newLine);				//get input from user

		if (newLine != '\n') {							//in case the character is not End of the line
			clearInputBuffer();							//clear input buffer
			printf("Error! Input a whole number: ");
		}
		else {											//in case input is whole digit (not include any character)
			flage = 1;
		}
	} while (flage == 0);
	return value;
}

// 2.Check is integer input positive function definition
int inputIntPositive(void)
{
	int posInt, flag = 0;
	do {
		scanf("%d", &posInt);							//get input from user
		if (posInt < 1) {								//in case input is negative or zero
			printf("ERROR! Value must be > 0: ");
		}
		else {											//in case input is positive
			flag = 1;
		}
	} while (flag == 0);
	return posInt;
}

// 3.Check is integer input in a range function definition
int inputIntRange(int minInputValue, int maxInputValue)
{
	int rangeInt, flag = 0;
	do {
		rangeInt = inputInt();											//call the inputInt() function to get input and check for digit
		if (rangeInt < minInputValue || rangeInt > maxInputValue) {		//in case input is more than maxInput or less than minInput
			printf("ERROR! Value must be between %d and %d inclusive: ", minInputValue, maxInputValue);
		}
		else {												//in case iput is in the range
			flag = 1;
		}
	} while (flag == 0);
	return rangeInt;
}

// 4.Get character input function definition
char inputCharOption(const char charValueOp[])
{
	char charInput;
	int i, flag = 0, charCheck = 0;
	do {
		scanf(" %c", &charInput);							//get input from user
		for (i = 0; charValueOp[i] != '\0'; i++)			//looping for check input character is include in a word
		{
			if (charInput == charValueOp[i])				//in case the word has an input character inclusive
			{
				charCheck = 1;
			}
			//printf("%c %c\n", charInput, charValueOp[i]);
		}
		if (charCheck == 0)									//in case the word has not had an input character inclusive
		{
			printf("ERROR: Character must be one of [%s]: ", charValueOp);
		}
		else {
			flag = 1;
		}
	} while (flag == 0);
	clearInputBuffer();										//clear input buffer
	return charInput;
}

// 5.Get string input function definition
void inputCString(char* word, const int minChar, const int maxChar)
{
	int flag = 0;
	char chInput = 'c';

	do {
		int len = 0;

		while (chInput != '\n' && len <= maxChar)		//get input from keyboard until found '\n'
		{
			chInput = getchar();
			word[len] = chInput;
			len++;
			//printf("%d ", len);						//check length value
		}

		if (chInput == '\n' && len <= (maxChar + 1))	//add '\0' to the end of the string if input less than maxChar
		{
			len--;
			word[len] = '\0';
		}
		else {
			word[maxChar] = '\0';						//add '\0' to the end to ignore input more than maxChar
			clearInputBuffer();							//clear input buffer which more than maxChar
		}

		if (minChar == maxChar && len != minChar)		//in case minChar and maxChar are equal but lenght of input is not equal them
		{
			printf("ERROR: String length must be exactly %d chars: ", minChar);
			chInput = 'a';
		}
		else if (len > maxChar) {						//in case length is more than maxChar
			printf("ERROR: String length must be no more than %d chars: ", maxChar);
			chInput = 'a';
		}
		else if (len < minChar)							//in case length is less than minChar
		{
			printf("ERROR: String length must be between %d and %d chars: ", minChar, maxChar);
			chInput = 'a';
		}
		else											//in case length is correct (less than maxChar and more than minChar)
		{
			flag = 1;
		}
	} while (flag == 0);
	return;
}

// 6.Display formatted phone number function definition
void displayFormattedPhone(const char* strC)
{
	int i, j, length = 0;

	if (strC == NULL) {									//in case sttring equal 'NULL'
		printf("(___)___-____");
	}
	else {
		for (i = 0; strC[i] != '\0'; i++)				//count the length
		{
			if (isdigit(strC[i]) != 0) {				//test is digit or alphabet and count if it is digit
				length++;
				//printf("%d ", length);				//Check length for each loop
			}
		}
		//printf("%d ", length);
		if (length == 10)								//in case phone number is correct (10 digits of number)
		{
			printf("(");
			for (j = 0; j < 10; j++)
			{
				printf("%c", strC[j]);					//print number one by one
				if (j == 2)
					printf(")");
				else if (j == 5)
					printf("-");
			}
		}
		else											//in case phone number is incorrect
		{
			printf("(___)___-____");
		}
	}
	return;
}

// 7.Get number string input function
void inputCStringNumber(char* word, const int numChar)
{
	int flag = 0;
	char chInput = 'c';

	do {
		int len = 0;

		while (chInput != '\n' && len <= numChar)		//get input from keyboard until found '\n'
		{
			chInput = getchar();
			word[len] = chInput;
			len++;
		}

		if (chInput == '\n' && len <= (numChar + 1))	//add '\0' to the end of the string if input less than numChar
		{
			len--;
			word[len] = '\0';
		}
		else {
			word[numChar] = '\0';						//add '\0' to the end to ignore input more than numChar
			clearInputBuffer();							//clear input buffer which more than numChar
		}

		if (len != numChar)								//in case lenght of input is not equal numChar
		{
			printf("Invalid %d-digit number! Number: ", numChar);
			chInput = 'a';
		}
		else											//in case length is equal to numChar
		{
			flag = 1;
		}
	} while (flag == 0);
	return;
}