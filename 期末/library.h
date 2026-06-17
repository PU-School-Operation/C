#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_BOOKS 200
#define MAX_STUDENTS 100
#define MAX_BORROWED 50

#define TITLE_LEN 128
#define AUTHOR_LEN 50
#define YEAR_LEN 5
#define EDITION_LEN 4
#define ISBN_LEN 14
#define COPYID_LEN 4
#define BORROWER_LEN 20
#define ACCOUNT_LEN 20
#define PASSWORD_LEN 20
#define DATE_LEN 9

/* Book structure */
typedef struct {
    char title[TITLE_LEN];
    char author[AUTHOR_LEN];
    char year[YEAR_LEN];
    char edition[EDITION_LEN];
    char isbn[ISBN_LEN];
    char copyId[COPYID_LEN];
    char borrower[BORROWER_LEN]; /* empty string means available */
} Book;

/* Borrowed record inside Student */
typedef struct {
    char isbn[ISBN_LEN];
    char date[DATE_LEN]; /* YYYYMMDD */
} BorrowRecord;

/* Student structure */
typedef struct {
    char account[ACCOUNT_LEN];
    char password[PASSWORD_LEN];
    int borrowCount;
    BorrowRecord borrowed[MAX_BORROWED];
} Student;

/* Global data */
extern Book books[MAX_BOOKS];
extern int bookCount;
extern Student students[MAX_STUDENTS];
extern int studentCount;

/* File I/O */
void loadBooks(const char *filename);
void loadStudents(const char *filename);
void saveStudents(const char *filename);
void saveBooks(const char *filename);

/* Utility */
int isLeapYear(int year);
int daysInMonth(int month, int year);
int dateToDays(const char *dateStr);
int daysBetween(const char *date1, const char *date2);
void addDays(const char *startDate, int days, char *resultDate);
int isValidDate(const char *dateStr);
void trimNewline(char *str);
void removeAmpersand(char *dest, const char *src);
int getCurrentDate(char *dateStr);

/* Student helpers */
int findStudentByAccount(const char *account);
int authenticateStudent(const char *account, const char *password);
int hasOverdueBooks(int studentIdx, const char *currentDate);

/* Book helpers */
int findBookByIsbnAndCopy(const char *isbn, const char *copyId);
int findBooksByTitle(const char *title, int *resultIndices, int maxResults);
int findBooksByAuthor(const char *author, int *resultIndices, int maxResults);
int findBooksByIsbn(const char *isbn, int *resultIndices, int maxResults);
int isBookAvailable(int bookIdx);

/* Command functions */
void cmdBorrowBook(const char *currentDate);
void cmdReturnBook(void);
void cmdAddBook(void);
void cmdDeleteBook(void);
void cmdQueryBook(void);
void cmdViewMyBorrows(void);
void cmdDumpAll(void);

#endif /* LIBRARY_H */