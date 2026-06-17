#include "library.h"

void showMenu(void) {
    printf("\n==================================\n");
    printf("   Library Management System\n");
    printf("==================================\n");
    printf("  0. Borrow Book\n");
    printf("  1. Return Book\n");
    printf("  2. Add Book\n");
    printf("  3. Delete Book\n");
    printf("  4. Query Books\n");
    printf("  5. View My Borrows\n");
    printf("  6. Dump All Books\n");
    printf("  7. Exit\n");
    printf("==================================\n");
    printf("Enter command: ");
}

int main(void) {
    char currentDate[DATE_LEN];

    /* Load data from files */
    loadBooks("Books.txt");
    loadStudents("Students.txt");

    /* Get current date */
    getCurrentDate(currentDate);

    printf("\nLibrary Management System started.\n");
    printf("Current date: %s\n", currentDate);

    char cmd[10];
    while (1) {
        showMenu();
        fgets(cmd, sizeof(cmd), stdin);
        trimNewline(cmd);

        if (strcmp(cmd, "7") == 0) {
            /* Before exit, save data */
            saveBooks("Books.txt");
            saveStudents("Students.txt");
            printf("Data saved. Goodbye!\n");
            break;
        }

        /* Debug: allow user to enter a custom "today" date at any time by typing "date" */
        if (strcmp(cmd, "date") == 0) {
            char newDate[DATE_LEN];
            printf("Enter today's date (YYYYMMDD): ");
            fgets(newDate, sizeof(newDate), stdin);
            trimNewline(newDate);
            if (isValidDate(newDate)) {
                strcpy(currentDate, newDate);
                printf("Current date set to: %s\n", currentDate);
            } else {
                printf("Invalid date format. Use YYYYMMDD.\n");
            }
            continue;
        }

        if (strcmp(cmd, "0") == 0) {
            cmdBorrowBook(currentDate);
        } else if (strcmp(cmd, "1") == 0) {
            cmdReturnBook();
        } else if (strcmp(cmd, "2") == 0) {
            cmdAddBook();
        } else if (strcmp(cmd, "3") == 0) {
            cmdDeleteBook();
        } else if (strcmp(cmd, "4") == 0) {
            cmdQueryBook();
        } else if (strcmp(cmd, "5") == 0) {
            cmdViewMyBorrows();
        } else if (strcmp(cmd, "6") == 0) {
            cmdDumpAll();
        } else {
            printf("Invalid command. Please enter 0-7, or 'date' to set date.\n");
        }
    }

    return 0;
}