#ifndef CORE_H
#define CORE_H

//////////////////////////////////////
// USER INTERFACE FUNCTIONS
//////////////////////////////////////

// Clear the standard input buffer
void clearInputBuffer(void);

// Wait for user to input the "enter" key to continue
void suspend(void);

//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// 1.Get integer input function
int inputInt(void);

// 2.Check is integer input positive function
int inputIntPositive(void);

// 3.Check is integer input in a range function
int inputIntRange(int minInputValue, int maxInputValue);

// 4.Get character input function
char inputCharOption(const char charValueOp[]);

// 5.Get string input function
void inputCString(char* word, int minChar, int maxChar);

// 6.Display formatted phone number function
void displayFormattedPhone(const char* strC);

#endif // !CORE_H