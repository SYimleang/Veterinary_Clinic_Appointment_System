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

// Display's appointment schedule headers (date-specific or all records)
void displayScheduleTableHeader(const struct Date* date, int isAllRecords)
{
    printf("Clinic Appointments for the Date: ");

    if (isAllRecords)
    {
        printf("<ALL>\n\n");
        printf("Date       Time  Pat.# Name            Phone#\n"
            "---------- ----- ----- --------------- --------------------\n");
    }
    else
    {
        printf("%04d-%02d-%02d\n\n", date->year, date->month, date->day);
        printf("Time  Pat.# Name            Phone#\n"
            "----- ----- --------------- --------------------\n");
    }
}

// Display a single appointment record with patient info. in tabular format
void displayScheduleData(const struct Patient* patient,
    const struct Appointment* appoint,
    int includeDateField)
{
    if (includeDateField)
    {
        printf("%04d-%02d-%02d ", appoint->date.year, appoint->date.month,
            appoint->date.day);
    }
    printf("%02d:%02d %05d %-15s ", appoint->time.hour, appoint->time.min,
        patient->patientNumber, patient->name);

    displayFormattedPhone(patient->phone.number);

    printf(" (%s)\n", patient->phone.description);
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
            menuAppointment(data);
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

// Menu: Appointment Management
void menuAppointment(struct ClinicData* data)
{
    int selection;

    do {
        printf("Appointment Management\n"
            "==============================\n"
            "1) VIEW   ALL Appointments\n"
            "2) VIEW   Appointments by DATE\n"
            "3) ADD    Appointment\n"
            "4) REMOVE Appointment\n"
            "------------------------------\n"
            "0) Previous menu\n"
            "------------------------------\n"
            "Selection: ");
        selection = inputIntRange(0, 4);
        putchar('\n');
        switch (selection)
        {
        case 1:
            viewAllAppointments(data);  // ToDo: You will need to create this function!
            suspend();
            break;
        case 2:
            viewAppointmentSchedule(data);  // ToDo: You will need to create this function!
            suspend();
            break;
        case 3:
            addAppointment(data->appointments, data->maxAppointments,
                data->patients, data->maxPatient);  // ToDo: You will need to create this function!
            suspend();
            break;
        case 4:
            removeAppointment(data->appointments, data->maxAppointments,
                data->patients, data->maxPatient);  // ToDo: You will need to create this function!
            suspend();
            break;
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
    printf("\n");
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
    int i, nextNumber = 0, index = 0, location = 0;

    // Find a free record in the patient array by looping one by one
    for (i = 0; i < max && location == 0; i++)
    {
        location = 0;
        if (patient[i].patientNumber == 0) {                // If found empty record will count for use
            index = i;
            location = 1;
        }
    }


    // If no free record in the patient array was found, display an error message
    if (location == 0) {
        printf("ERROR: Patient listing is FULL!\n\n");
    }

    // If found empty record, then continue to adding record process
    else {
        // Assign the next unique patient number to the new record
        nextNumber = nextPatientNumber(patient, max);
        patient[index].patientNumber = nextNumber;

        inputPatient(patient + index);                      // Call input patient data function to add name and contact information

        printf("*** New patient record added ***\n");       // Confirm added message
        printf("\n");
    }
    return;
}

// Edit a patient record from the patient array
void editPatient(struct Patient patient[], int max)
{
    int patientNum, location, index;

    // Prompt user to input patient's number that want to edit
    printf("Enter the patient number: ");
    patientNum = inputIntPositive();
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
        printf("ERROR: Patient record not found!\n\n");
    }
    return;
}

// Remove a patient record from the patient array
void removePatient(struct Patient patient[], int max)
{
    int patientNum, index, flag = 1;
    char confirm, ch;

    // Prompt user to input patient's number that want to remove
    printf("Enter the patient number: ");
    patientNum = inputIntPositive();
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

        do {
            scanf("%c%c", &confirm, &ch);

            // If user confirm will remove the record
            if ((confirm == 'y' || confirm == 'Y') && (ch == '\n'))
            {
                patient[index].patientNumber = 0;                    // Assign the matched record to 'NULL'
                printf("Patient record has been removed!\n\n");       // Removed confirm message
                flag = 0;
            }

            // If user not confirm will cancel removing process
            else if ((confirm == 'n' || confirm == 'N') && (ch == '\n'))
            {
                printf("Operation aborted.\n");
                flag = 0;
                clearInputBuffer();
            }

            // If user not enter 'y' or 'n' will repeat asking
            else
            {
                clearInputBuffer();
                printf("ERROR: Character must be one of [yn]: ");
            }
        } while (flag == 1);
    }

    // If not found mathed record will display error message
    else
    {
        printf("ERROR: Patient record not found!\n");
        clearInputBuffer();
        printf("\n");
    }
}

//////////////////////////////////////
// MANDATORY FUNCTIONS
//////////////////////////////////////

// View ALL scheduled appointments--------------------------------------------------------------------------------------------------------------------------
// Todo:
void viewAllAppointments(struct ClinicData* data)
{
    int i, j;

    sorting(data->appointments, data->maxAppointments);

    displayScheduleTableHeader(NULL, 1);

    for (i = 0; i < data->maxAppointments; i++)
    {
        for (j = 0; j < data->maxPatient; j++)
        {
            if ((data->appointments[i].patientNumber && data->patients[j].patientNumber)
                && (data->appointments[i].patientNumber == data->patients[j].patientNumber))
            {
                displayScheduleData(&data->patients[j], &data->appointments[i], 1);
            }
        }
    }
    printf("\n");

    return;
}

// View appointment schedule for the user input date
// Todo:
void viewAppointmentSchedule(struct ClinicData* data)
{
    struct Date appointDate;
    int i, j, leapYear = 0, endDay = 31;

    printf("Year        : ");
    appointDate.year = inputIntPositive();

    if (appointDate.year % 400 == 0 || (appointDate.year % 100 != 0 && appointDate.year % 4 == 0))
    {
        leapYear = 1;
    }

    printf("Month (1-12): ");
    appointDate.month = inputIntRange(1, 12);

    if (appointDate.month == 2 && leapYear == 1)
    {
        endDay = 29;
    }
    else if (appointDate.month == 2)
    {
        endDay = 28;
    }
    else if (appointDate.month == 4 || appointDate.month == 6 || appointDate.month == 9 || appointDate.month == 11)
    {
        endDay = 30;
    }
    else
    {
        endDay = 31;
    }

    printf("Day (1-%d)  : ", endDay);
    appointDate.day = inputIntRange(1, endDay);
    printf("\n");

    sorting(data->appointments, data->maxAppointments);

    displayScheduleTableHeader(&appointDate, 0);

    for (i = 0; i < data->maxAppointments; i++)
    {
        for (j = 0; j < data->maxPatient; j++)
        {
            if ((data->appointments[i].patientNumber && data->patients[j].patientNumber)
                && (data->appointments[i].patientNumber == data->patients[j].patientNumber)
                && (data->appointments[i].date.year == appointDate.year && data->appointments[i].date.month == appointDate.month && data->appointments[i].date.day == appointDate.day))
            {
                displayScheduleData(&data->patients[j], &data->appointments[i], 0);
            }
        }
    }

    printf("\n");

    return;
}

// Add an appointment record to the appointment array
// Todo:
void addAppointment(struct Appointment* appoint, int maxAppoint, struct Patient* patient, int maxPatient)
{
    int patientNum, index;
    int leapYear, endDay;
    int i, availableTime, repeat = 1, availableNext = 0;

    struct Date appointDate;
    struct Time appointTime;

    printf("Patient Number: ");
    patientNum = inputIntPositive();
    index = findPatientIndexByPatientNum(patientNum, patient, maxPatient);

    if (index >= 0) {
        while (repeat == 1)
        {
            availableTime = 1;

            printf("Year        : ");
            appointDate.year = inputIntPositive();

            if (appointDate.year % 400 == 0 || (appointDate.year % 100 != 0 && appointDate.year % 4 == 0))
            {
                leapYear = 1;
            }

            printf("Month (1-12): ");
            appointDate.month = inputIntRange(1, 12);

            if (appointDate.month == 2 && leapYear == 1)
            {
                endDay = 29;
            }
            else if (appointDate.month == 2)
            {
                endDay = 28;
            }
            else if (appointDate.month == 4 || appointDate.month == 6 || appointDate.month == 9 || appointDate.month == 11)
            {
                endDay = 30;
            }
            else
            {
                endDay = 31;
            }

            printf("Day (1-%d)  : ", endDay);
            appointDate.day = inputIntRange(1, endDay);

            printf("Hour (0-23)  : ");
            appointTime.hour = inputIntRange(0, 23);

            printf("Minute (0-59): ");
            appointTime.min = inputIntRange(0, 59);

            while ((appointTime.hour < TIME_START || appointTime.hour > TIME_CLOSE) || (appointTime.min % TIME_INTERVAL != 0) || (appointTime.hour == TIME_CLOSE && appointTime.min != 0))
            {
                printf("ERROR: Time must be between %02d:00 and %02d:00 in %02d minute intervals.\n\n", TIME_START, TIME_CLOSE, TIME_INTERVAL);

                printf("Hour (0-23)  : ");
                appointTime.hour = inputIntRange(0, 23);

                printf("Minute (0-59): ");
                appointTime.min = inputIntRange(0, 59);
            }

            for (i = 0; i < maxAppoint; i++)
            {
                if (appointDate.year == appoint[i].date.year && appointDate.month == appoint[i].date.month && appointDate.day == appoint[i].date.day
                    && appointTime.hour == appoint[i].time.hour && appointTime.min == appoint[i].time.min)
                {
                    availableTime = 0;
                }
            }

            if (availableTime == 0)
            {
                printf("\nERROR: Appointment timeslot is not available!\n\n");
            }
            else
            {


                for (index = 0; availableNext == 0 && index < maxAppoint; index++)
                {
                    if (appoint[index].patientNumber < 1)
                    {
                        availableNext = 1;
                    }
                }

                appoint[index].date = appointDate;
                appoint[index].time = appointTime;
                appoint[index].patientNumber = patientNum;
                printf("\n*** Appointment scheduled! ***\n\n");
                repeat = 0;
            }
        }
    }
    else
    {
        printf("\nERROR: Patient record not found!\n\n");
    }
    return;
}

// Remove an appointment record from the appointment array
// Todo:
void removeAppointment(struct Appointment* appoint, int maxAppoint, struct Patient* patient, int maxPatient)
{
    int patientNum, index;
    int leapYear, endDay;
    int i, availableValid = 0;
    struct Date appointDate;

    printf("Patient Number: ");
    patientNum = inputIntPositive();


    index = findPatientIndexByPatientNum(patientNum, patient, maxPatient);

    if (index >= 0) {
        printf("Year        : ");
        appointDate.year = inputIntPositive();

        if (appointDate.year % 400 == 0 || (appointDate.year % 100 != 0 && appointDate.year % 4 == 0))
        {
            leapYear = 1;
        }

        printf("Month (1-12): ");
        appointDate.month = inputIntRange(1, 12);

        if (appointDate.month == 2 && leapYear == 1)
        {
            endDay = 29;
        }
        else if (appointDate.month == 2)
        {
            endDay = 28;
        }
        else if (appointDate.month == 4 || appointDate.month == 6 || appointDate.month == 9 || appointDate.month == 11)
        {
            endDay = 30;
        }
        else
        {
            endDay = 31;
        }

        printf("Day (1-%d)  : ", endDay);
        appointDate.day = inputIntRange(1, endDay);

        for (i = 0; availableValid == 0 && i < maxAppoint; i++)
        {
            if ((appoint[i].patientNumber == patientNum) && (appoint[i].date.year == appointDate.year) && (appoint[i].date.month == appointDate.month) && (appoint[i].date.day == appointDate.day))
            {
                availableValid = 1;
            }
        }

        if (i - 1 >= 0)
        {
            printf("\n");
            displayPatientData(&patient[index], FMT_FORM);
            printf("Are you sure you want to remove this appointment (y,n): ");

            if (inputCharOption("yn") == 'y')
            {
                appoint[i - 1].patientNumber = 0;
                printf("\nAppointment record has been removed!\n\n");
            }
            else
            {
                printf("ERROR: No appointment for this date!\n\n");
            }
        }
    }
    else
    {
        printf("ERROR: Patient record not found!\n\n");
        clearInputBuffer();
    }
    return;
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
    patientNumber = inputIntPositive();

    // Call find patient's number function and assign to "index"
    index = findPatientIndexByPatientNum(patientNumber, patient, max);
    printf("\n");

    // If found the matched record will display patient's record data
    if (index > 0)
    {
        displayPatientData(&patient[index], FMT_FORM);                      // Call diplay patient's record data function
        printf("\n");
    }

    // If not found the matched record will display message error
    else
        printf("*** No records found ***\n\n");
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
    printf("\n");

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
    void searchPatientByPhoneNumber(const struct Patient patient[], int max);

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
    // Display choice of input menu
    printf("Patient Data Input\n");
    printf("------------------\n");
    printf("Number: %05d\n", patient->patientNumber);       // Display new patient's number which get from findind next patient's number function
    printf("Name  : ");
    inputCString(patient->name, 1, NAME_LEN);               // Call input C String for get input of the name of new patient
    printf("\n");


    inputPhoneData(&patient->phone);                        // Call input phone function to add or edit patient's phone number

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
    scanf("%d", &selection);                                            // Prompt user to input selection choice
    printf("\n");

    // Case of type of selection
    switch (selection)
    {

        // If user selected '1' will assign phone's description to "CELL" and promt user to input phone number
    case 1:
        strncpy(phone->description, "CELL", PHONE_DESC_LEN);        // Assign phone's description to "CELL"

        printf("Contact: %s\n", phone->description);
        clearInputBuffer();

        // Prompt user to input phone's number
        printf("Number : ");
        inputCStringNumber(phone->number, 10);

        printf("\n");
        break;

        // If user selected '2' will assign phone's description to "HOME" and promt user to input phone number
    case 2:
        strcpy(phone->description, "HOME");

        printf("Contact: %s\n", phone->description);                // Assign phone's description to "HOME"
        clearInputBuffer();

        // Prompt user to input phone's number
        printf("Number : ");
        inputCString(phone->number, 10, 10);

        printf("\n");
        break;

        // If user selected '3' will assign phone's description to "WORK" and promt user to input phone number
    case 3:
        strcpy(phone->description, "WORK");

        printf("Contact: %s", phone->description);                  // Assign phone's description to "WORK"
        printf("\n");
        clearInputBuffer();

        // Prompt user to input phone's number
        printf("Number : ");
        inputCString(phone->number, 10, 10);

        printf("\n");
        break;

        // If user selected '4' will assign phone's description to "TBD"
    case 4:
        strcpy(phone->description, "TBD");         // Assign phone description to "TBD"

        *phone->number = 0;                                         // Assign phone number to 0

        break;

        // If user entered something else, then display error message
    default:
        printf("Error:\n");
        break;
    }

    return;
}

//////////////////////////////////////
// FILE FUNCTIONS
//////////////////////////////////////

// Import patient data from file into a Patient array (returns # of records read)--------------------------------------------------------------------------
/// ToDo:
int importPatients(const char* datafile, struct Patient patients[], int max)
{
    int count = 0;

    FILE* fp = NULL;

    fp = fopen(datafile, "r");

    if (fp != NULL)
    {
        while (count < max) {
            fscanf(fp, "%d|%[^|]|%[^|]|%[^\n]", &patients[count].patientNumber, patients[count].name, patients[count].phone.description, patients[count].phone.number);
            count++;
        }
        fclose(fp);
    }
    else
    {
        printf("Failed to open file\n");
    }

    return count;
}

// Import appointment data from file into an Appointment array (returns # of records read)
// ToDo:
int importAppointments(const char* datafile, struct Appointment appoints[], int max)
{
    int i, count = 0;                                                                                                                           // Do I need to do anything with it??

    FILE* fp = NULL;

    fp = fopen(datafile, "r");

    if (fp != NULL)
    {
        for (i = 0; i < max && !feof(fp); i++) {
            fscanf(fp, "%d,%d,%d,%d,%d,%d", &appoints[i].patientNumber, &appoints[i].date.year, &appoints[i].date.month, &appoints[i].date.day, &appoints[i].time.hour, &appoints[i].time.min);

            if (!feof(fp)) {
                count++;
            }
        }
        fclose(fp);
    }
    else
    {
        printf("Failed to open file\n");
    }

    return count;
}

// Sorting appointment data
void sorting(struct Appointment* appoints, int maxAppoints)
{
    int i, j;
    struct Appointment buffer;

    for (i = 0; i < maxAppoints; i++)
    {
        appoints[i].time.min = (appoints[i].date.year * 12 * 30 * 24 * 60)
            + (appoints[i].date.month * 30 * 24 * 60)
            + (appoints[i].date.day * 24 * 60)
            + (appoints[i].time.hour * 60)
            + appoints[i].time.min;
    }

    for (i = maxAppoints - 1; i > 0; i--)
    {
        for (j = 0; j < i; j++)
        {
            if (appoints[j].time.min > appoints[j + 1].time.min)
            {
                buffer = appoints[j];
                appoints[j] = appoints[j + 1];
                appoints[j + 1] = buffer;
            }
        }
    }

    for (i = 0; i < maxAppoints; i++)
    {
        appoints[i].time.min = appoints[i].time.min
            - (appoints[i].date.year * 12 * 30 * 24 * 60)
            - (appoints[i].date.month * 30 * 24 * 60)
            - (appoints[i].date.day * 24 * 60)
            - (appoints[i].time.hour * 60);
    }
    return;
}