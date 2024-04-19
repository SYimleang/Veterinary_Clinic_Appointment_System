void test01_inputInt(void);
void test02_inputIntPositive(void);
void test03_inputIntRange(void);
void test04_inputIntRange(void);
void test05_inputCharOption(void);
void test06_inputCString(void);
void test07_inputCString(void);
void test08_inputCString(void);
void test09_displayPhone(void);

// Clear the standard input buffer
void clearInputBuffer(void);

// Wait for user to input the "enter" key to continue
void suspend(void);

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