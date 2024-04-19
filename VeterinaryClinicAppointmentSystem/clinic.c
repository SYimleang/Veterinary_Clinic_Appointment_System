#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "core.h"
#include "clinic.h"

//////////////////////////////////////
// DISPLAY FUNCTIONS
//////////////////////////////////////

// Display's the patient table header (table format)
void displayPatientTableHeader(void)
{
    printf("Pat.# Name            Phone#\n"
        "----- --------------- --------------------\n");
}

// Displays a single patient record in FMT_FORM | FMT_TABLE format
void displayPatientData(const struct Patient* patient, int fmt)
{
    if (fmt == FMT_FORM)
    {
        printf("Name  : %s\n"
            "Number: %05d\n"
            "Phone : ", patient->name, patient->patientNumber);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
    else
    {
        printf("%05d %-15s ", patient->patientNumber,
            patient->name);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
}

//////////////////////////////////////
// MENU & ITEM SELECTION FUNCTIONS
//////////////////////////////////////

// main menu
void menuMain(struct ClinicData* data)
{
    int selection;
    do {
        printf("Veterinary Clinic System\n"
            "=========================\n"
            "1) PATIENT     Management\n"
            "2) APPOINTMENT Management\n"
            "-------------------------\n"
            "0) Exit System\n"
            "-------------------------\n"
            "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');
        switch (selection)
        {
        case 0:
            printf("Are you sure you want to exit? (y|n): ");
            selection = !(inputCharOption("yn") == 'y');
            putchar('\n');
            if (!selection)
            {
                printf("Exiting system... Goodbye.\n\n");
            }
            break;
        case 1:
            menuPatient(data->patients, data->maxPatient);
            break;
        case 2:
            printf("<<< Feature not yet available >>>\n\n");
            break;
        }
    } while (selection);
}

// Menu: Patient Management
void menuPatient(struct Patient patient[], int max)
{
    int selection;
    do {
        printf("Patient Management\n"
            "=========================\n"
            "1) VIEW   Patient Data\n"
            "2) SEARCH Patients\n"
            "3) ADD    Patient\n"
            "4) EDIT   Patient\n"
            "5) REMOVE Patient\n"
            "-------------------------\n"
            "0) Previous menu\n"
            "-------------------------\n"
            "Selection: ");
        selection = inputIntRange(0, 5);
        putchar('\n');
        switch (selection)
        {
        case 1:
            displayAllPatients(patient, max, FMT_TABLE);
            suspend();
            break;
        case 2:
            searchPatientData(patient, max);
            break;
        case 3:
            addPatient(patient, max);
            suspend();
            break;
        case 4:
            editPatient(patient, max);
            break;
        case 5:
            removePatient(patient, max);
            suspend();
            break;
        }
    } while (selection);
}

// Menu: Patient edit
void menuPatientEdit(struct Patient* patient)
{
    int selection;
    do {
        printf("Edit Patient (%05d)\n"
            "=========================\n"
            "1) NAME : %s\n"
            "2) PHONE: ", patient->patientNumber, patient->name);
        displayFormattedPhone(patient->phone.number);
        printf("\n"
            "-------------------------\n"
            "0) Previous menu\n"
            "-------------------------\n"
            "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');
        if (selection == 1)
        {
            printf("Name  : ");
            inputCString(patient->name, 1, NAME_LEN);
            putchar('\n');
            printf("Patient record updated!\n\n");
        }
        else if (selection == 2)
        {
            inputPhoneData(&patient->phone);
            printf("Patient record updated!\n\n");
        }
    } while (selection);
}

// Display's all patient data in the FMT_FORM | FMT_TABLE format
void displayAllPatients(const struct Patient patient[], int max, int fmt)
{
    int i, foundRec = 0;
    if (fmt == FMT_TABLE) {
        displayPatientTableHeader();                        // Display Header of the table
    }

    for (i = 0; i < max; i++) {                             // Display patient's Data for each loop
        if (patient[i].patientNumber != 0) {
            displayPatientData(&patient[i], fmt);
            foundRec = 1;
        }
    }
    if (foundRec == 0)                                       // If no dara in  clinic record will display "No records found"
    {
        printf("*** No records found ***\n");
    }
    return;
}

// Search for a patient record based on patient number or phone number
void searchPatientData(const struct Patient patient[], int max) {
    int searchBy;

    do {                                                    // Looping untill found correct input user enter '0' for exit
        // Display menu for select options
        printf("Search Options\n");
        printf("==========================\n");
        printf("1) By patient number\n");
        printf("2) By phone number\n");
        printf("..........................\n");
        printf("0) Previous menu\n");
        printf("..........................\n");
        printf("Selection: ");
        scanf("%d", &searchBy);                             // Get user selection for checking conditions
        printf("\n");

        // If user select search by patient number
        if (searchBy == 1) {
            searchPatientByPatientNumber(patient, max);     // Call function for search by patient number
            clearInputBuffer();
            suspend();
        }
        // If user select search by patient phone number
        else if (searchBy == 2) {
            searchPatientByPhoneNumber(patient, max);       // Call function for search by patient phone number
            clearInputBuffer();
            suspend();
        }
    } while (searchBy != 0);                                // Exit loop when user input '0'
}

// Add a new patient record to the patient array
void addPatient(struct Patient patient[], int max) {
    int i, nextNumber = 0, index = 0;
    // Find a free record in the patient array by looping one by one
    for (i = 0; i < max; i++)
    {
        if (patient[i].patientNumber != 0) {                // If found empty record will count for use
            index++;
        }
    }
    // If no free record in the patient array was found, display an error message
    if (index >= max) {
        printf("ERROR: Patient listing is FULL!\n");
    }
    // If found empty record, then continue to adding record process
    else {
        // Assign the next unique patient number to the new record
        nextNumber = nextPatientNumber(patient, max);
        patient[index].patientNumber = nextNumber;
        inputPatient(patient + index);                      // Call input patient data function to add name and contact information
        printf("*** New patient record added ***\n");       // Confirm added message
        clearInputBuffer();
    }
    return;
}

// Edit a patient record from the patient array
void editPatient(struct Patient patient[], int max)
{
    int patientNum, location, index;
    // Prompt user to input patient's number that want to edit
    printf("Enter the patient number: ");
    scanf("%d", &patientNum);
    printf("\n");
    // Call finding patient number function to find matches records
    index = findPatientIndexByPatientNum(patientNum, patient, max);
    // If found matched record, then start editing by call edit menu
    if (index != -1)
    {
        location = findPatientIndexByPatientNum(patientNum, patient, max);
        menuPatientEdit(patient + location);
    }
    // If not found mathed record will display error message
    else
    {
        printf("ERROR: Patient record not found!\n");
    }
    return;
}

// Remove a patient record from the patient array
void removePatient(struct Patient patient[], int max)
{
    int patientNum, index;
    char confirm;
    // Prompt user to input patient's number that want to remove
    printf("Enter the patient number: ");
    scanf("%d", &patientNum);
    printf("\n");
    // Call finding patient number function to find matches records
    index = findPatientIndexByPatientNum(patientNum, patient, max);
    // If found matched record will start remove process
    if (index != -1)
    {
        displayPatientData(&patient[index], FMT_FORM);                              // Call display patient's data which matched with user's input
        printf("\n");
        // Remove confirm process
        printf("Are you sure you want to remove this patient record? (y/n): ");
        scanf(" %c", &confirm);
        // If user confirm will remove the record
        if (confirm == 'y') {
            patient[index].patientNumber = 0;                    // Assign the matched record to 'NULL'
            printf("Patient record has been removed!\n");       // Removed confirm message
        }
        // If user not confirm will cancel removing process
        else {
            printf("Operation aborted.\n");
        }
        clearInputBuffer();
    }
    // If not found mathed record will display error message
    else
    {
        printf("ERROR: Patient record not found!\n");
        clearInputBuffer();
    }
}


//////////////////////////////////////
// UTILITY FUNCTIONS
//////////////////////////////////////

// Search and display patient record by patient number (form)
void searchPatientByPatientNumber(const struct Patient patient[], int max)
{
    int patientNumber, index;
    // Prompt user to input patient's number that user looking for
    printf("Search by patient number: ");
    scanf("%d", &patientNumber);
    // Call find patient's number function and assign to "index"
    index = findPatientIndexByPatientNum(patientNumber, patient, max);
    printf("\n");
    // If found the matched record will display patient's record data
    if (index > 0)
    {
        displayPatientData(&patient[index], FMT_FORM);                      // Call diplay patient's record data function
    }
    // If not found the matched record will display message error
    else
        printf("*** No records found ***\n");
    return;
}

// Search and display patient records by phone number (tabular)
void searchPatientByPhoneNumber(const struct Patient patient[], int max)
{
    int i, index = 0;
    char patientPhoneNumber[PHONE_LEN + 1];
    clearInputBuffer();
    // Prompt user to input patient's phone number that user looking for
    printf("Search by phone number: ");
    scanf("%s", patientPhoneNumber);
    printf("\n");
    displayPatientTableHeader();                                            // Display table's header of the records
    // Looping for searching matches record with user's input
    for (i = 0; i < max; i++)
    {
        // If found the matched record with user's input, then display patient's record data
        if (strcmp(patientPhoneNumber, patient[i].phone.number) == 0) {
            displayPatientData(&patient[i], FMT_TABLE);
            index = 1;
        }
    }
    // If not found the matched record will display message error
    if (index == 0)
    {
        printf("\n*** No records found ***\n");
    }
    return;
}

// Get the next highest patient number
int nextPatientNumber(const struct Patient patient[], int max)
{
    int i, maxIndex = 0;
    // Looping for searching largest patient's number
    for (i = 0; i < max; i++)
    {
        // If patient's number is larger than previous record data, then assign "maxIndex" to current patient's number
        if (patient[i].patientNumber > patient[maxIndex].patientNumber)
        {
            maxIndex = i;
        }
    }
    // Return "maxIndex" add one (next number after largest patient's number)
    return patient[maxIndex].patientNumber + 1;
}

// Find the patient array index by patient number (returns -1 if not found)
int findPatientIndexByPatientNum(int patientNumber, const struct Patient patient[], int max)
{
    int i;
    // Looping for searching patient's number
    for (i = 0; i < max; i++)
    {
        // If found matched patient's number, then return that record
        if (patient[i].patientNumber == patientNumber)
        {
            return i;
        }
    }
    // If not found matched patient's number, then return '-1'
    return -1;
}

//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get user input for a new patient record
void inputPatient(struct Patient* patient)
{
    int i;
    char nameInput[NAME_LEN + 1];

    // Display choice of input menu
    printf("Patient Data Input\n");
    printf("------------------\n");
    printf("Number: %05d\n", patient->patientNumber);       // Display new patient's number which get from findind next patient's number function
    printf("Name  : ");
    //scanf("%s", nameInput);
    fgets(nameInput, sizeof(nameInput), stdin);             // Prompt user to input name of new patient to buffer variable
    // Looping for assign new patient's name record from input buffer
    for (i = 0; i < strlen(nameInput) - 1; i++)
    {
        patient->name[i] = nameInput[i];
    }
    printf("\n");
    // Call input phone function to add or edit patient's phone number
    inputPhoneData(&patient->phone);
    return;
}

// Get user input for phone contact information
void inputPhoneData(struct Phone* phone)
{
    int selection = 0;
    // Display type of description of patient's phone number for user to select
    printf("Phone Information\n");
    printf("-----------------\n");
    printf("How will the patient like to be contacted?\n");
    printf("1. Cell\n");
    printf("2. Home\n");
    printf("3. Work\n");
    printf("4. TBD\n");
    printf("Selection: ");
    scanf("%d", &selection);              // Prompt user to input selection choice
    printf("\n");

    // Case of type of selection
    switch (selection)
    {
    // If user selected '1' will assign phone's description to "CELL" and promt user to input phone number
    case 1:
        strncpy(phone->description, "CELL", PHONE_DESC_LEN);        // Assign phone's description to "CELL"

        printf("Contact: %s\n", phone->description);
        clearInputBuffer();
        printf("Number : ");
        scanf("%s", phone->number);        // Prompt user to input phone's number
        printf("\n");
        break;
    // If user selected '2' will assign phone's description to "HOME" and promt user to input phone number
    case 2:
        strcpy(phone->description, "HOME");
        printf("Contact: %s\n", phone->description);    // Assign phone's description to "HOME"
        clearInputBuffer();
        printf("Number : ");
        scanf("%s", phone->number);                     // Prompt user to input phone's number
        printf("\n");
        break;
    // If user selected '3' will assign phone's description to "WORK" and promt user to input phone number
    case 3:
        strcpy(phone->description, "WORK");

        printf("Contact: %s", phone->description);  // Assign phone's description to "WORK"
        printf("\n");
        clearInputBuffer();
        printf("Number : ");
        scanf("%s", phone->number);                 // Prompt user to input phone's number
        printf("\n");
        break;
    // If user selected '4' will assign phone's description to "TBD"
    case 4:
        strcpy(phone->description, "TBD");         // Assign phone description to "TBD"
        *phone->number = 0;                        // Assign phone number to 0
        break;
    // If user entered something else, then display error message
    default:
        printf("Error:\n");
        break;
    }

    return;
}