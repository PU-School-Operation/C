#define _CRT_SECURE_NO_WARNINGS
#include "library.h"

/* Global data definitions */
Book books[MAX_BOOKS];
int bookCount = 0;
Student students[MAX_STUDENTS];
int studentCount = 0;

/* ================================================================
 * Utility Functions
 * ================================================================ */

void trimNewline(char *str)
{
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
    {
        str[--len] = '\0';
    }
}

int isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year)
{
    static const int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year))
        return 29;
    return days[month - 1];
}

int dateToDays(const char *dateStr)
{
    int year, month, day;
    if (sscanf(dateStr, "%4d%2d%2d", &year, &month, &day) != 3)
        return -1;

    int total = 0;
    for (int y = 0; y < year; y++)
    {
        total += isLeapYear(y) ? 366 : 365;
    }
    for (int m = 1; m < month; m++)
    {
        total += daysInMonth(m, year);
    }
    total += day;
    return total;
}

int daysBetween(const char *date1, const char *date2)
{
    return dateToDays(date2) - dateToDays(date1);
}

void addDays(const char *startDate, int days, char *resultDate)
{
    int year, month, day;
    sscanf(startDate, "%4d%2d%2d", &year, &month, &day);

    day += days;
    while (day > daysInMonth(month, year))
    {
        day -= daysInMonth(month, year);
        month++;
        if (month > 12)
        {
            month = 1;
            year++;
        }
    }
    while (day < 1)
    {
        month--;
        if (month < 1)
        {
            month = 12;
            year--;
        }
        day += daysInMonth(month, year);
    }

    sprintf(resultDate, "%04d%02d%02d", year, month, day);
}

int isValidDate(const char *dateStr)
{
    if (strlen(dateStr) != 8)
        return 0;
    for (int i = 0; i < 8; i++)
        if (!isdigit(dateStr[i]))
            return 0;

    int year, month, day;
    sscanf(dateStr, "%4d%2d%2d", &year, &month, &day);
    if (month < 1 || month > 12)
        return 0;
    if (day < 1 || day > daysInMonth(month, year))
        return 0;
    return 1;
}

int getCurrentDate(char *dateStr)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    if (!tm)
    {
        strcpy(dateStr, "20260617");
        return 0;
    }
    sprintf(dateStr, "%04d%02d%02d",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
    return 1;
}

/* ================================================================
 * File I/O
 * ================================================================ */

/*
 * Parse a line in format: &Title& &Author& Year Edition ISBN CopyId [Borrower]
 *
 * The &...& fields: the closing & for each field is the LAST & followed by space or end-of-string,
 * because the content may contain & characters (e.g., "Deitel & Deitel").
 */
void loadBooks(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Warning: cannot open %s, starting with empty book list.\n", filename);
        return;
    }

    char line[512];
    bookCount = 0;
    while (fgets(line, sizeof(line), fp) && bookCount < MAX_BOOKS)
    {
        trimNewline(line);
        if (strlen(line) == 0)
            continue;

        char title[TITLE_LEN], author[AUTHOR_LEN];
        char year[YEAR_LEN], edition[EDITION_LEN], isbn[ISBN_LEN], copyId[COPYID_LEN], borrower[BORROWER_LEN];
        title[0] = author[0] = borrower[0] = '\0';

        /* Strategy: scan for &...& pairs.
         * Find opening &, then find the LAST & followed by space/null for the closing.
         */
        /*
         * Strategy:
         * Field 1 (title) is enclosed in &...& where the closing & is the FIRST &
         * followed by space/null (title content never contains &).
         * Field 2 (author) is enclosed in &...& where the closing & is the LAST &
         * followed by space/null (author content may contain & like "Deitel & Deitel").
         */
        const char *p = line;

        /* --- Find all &...& field boundaries --- */
        /* Find all & positions in the line */
        const char *ampPositions[20];
        int ampCount = 0;
        const char *scan = line;
        while (*scan != '\0' && ampCount < 20)
        {
            if (*scan == '&')
            {
                ampPositions[ampCount++] = scan;
            }
            scan++;
        }

        if (ampCount < 4)
        {
            printf("Warning: not enough & markers in: %s\n", line);
            continue;
        }

        /* The first & opens title, the second & closes title (must be followed by space/null) */
        const char *openTitle = ampPositions[0];
        const char *closeTitle = ampPositions[1]; /* & followed by space */
        if (!(*(closeTitle + 1) == ' ' || *(closeTitle + 1) == '\0'))
        {
            printf("Warning: title closing & not followed by space in: %s\n", line);
            continue;
        }

        /* Title is between openTitle+1 and closeTitle */
        int len = (int)(closeTitle - (openTitle + 1));
        if (len >= TITLE_LEN)
            len = TITLE_LEN - 1;
        strncpy(title, openTitle + 1, len);
        title[len] = '\0';

        /* The last & followed by space/null closes the author field */
        const char *closeAuthor = NULL;
        for (int i = ampCount - 1; i >= 0; i--)
        {
            if (*(ampPositions[i] + 1) == ' ' || *(ampPositions[i] + 1) == '\0')
            {
                closeAuthor = ampPositions[i];
                break;
            }
        }
        if (!closeAuthor)
        {
            printf("Warning: cannot find author closing & in: %s\n", line);
            continue;
        }

        /* The opening & for author is the FIRST & after closeTitle */
        const char *openAuthor = NULL;
        for (int i = 0; i < ampCount; i++)
        {
            if (ampPositions[i] > closeTitle)
            {
                openAuthor = ampPositions[i];
                break;
            }
        }
        if (!openAuthor || openAuthor >= closeAuthor)
        {
            printf("Warning: cannot find author opening & in: %s\n", line);
            continue;
        }

        /* Author is between openAuthor+1 and closeAuthor */
        len = (int)(closeAuthor - (openAuthor + 1));
        if (len >= AUTHOR_LEN)
            len = AUTHOR_LEN - 1;
        strncpy(author, openAuthor + 1, len);
        author[len] = '\0';

        p = closeAuthor + 1;

        /* --- Parse remaining fields --- */
        int parsed = sscanf(p, "%s %s %s %s %s",
                            year, edition, isbn, copyId, borrower);
        if (parsed < 4)
        {
            /* Try without borrower (maybe borrower was parsed as part of copyId) */
            parsed = sscanf(p, "%s %s %s %s",
                            year, edition, isbn, copyId);
            borrower[0] = '\0';
        }

        if (parsed < 4)
        {
            printf("Warning: malformed line: %s\n", line);
            continue;
        }

        Book *b = &books[bookCount];
        memset(b, 0, sizeof(Book));
        strcpy(b->title, title);
        strcpy(b->author, author);
        strcpy(b->year, year);
        strcpy(b->edition, edition);
        strcpy(b->isbn, isbn);
        strcpy(b->copyId, copyId);
        if (strlen(borrower) > 0)
        {
            strcpy(b->borrower, borrower);
        }

        bookCount++;
    }
    fclose(fp);
    printf("Loaded %d books from %s.\n", bookCount, filename);
}

void loadStudents(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Warning: cannot open %s, starting with empty student list.\n", filename);
        return;
    }

    char line[512];
    studentCount = 0;
    while (fgets(line, sizeof(line), fp) && studentCount < MAX_STUDENTS)
    {
        trimNewline(line);
        if (strlen(line) == 0)
            continue;

        Student *s = &students[studentCount];
        memset(s, 0, sizeof(Student));

        char *token = strtok(line, " ");
        if (!token)
            continue;
        strcpy(s->account, token);

        token = strtok(NULL, " ");
        if (!token)
            continue;
        strcpy(s->password, token);

        s->borrowCount = 0;
        while (1)
        {
            char *isbnToken = strtok(NULL, " ");
            if (!isbnToken)
                break;
            char *dateToken = strtok(NULL, " ");
            if (!dateToken)
                break;

            if (s->borrowCount < MAX_BORROWED)
            {
                strcpy(s->borrowed[s->borrowCount].isbn, isbnToken);
                strcpy(s->borrowed[s->borrowCount].date, dateToken);
                s->borrowCount++;
            }
        }

        studentCount++;
    }
    fclose(fp);
    printf("Loaded %d students from %s.\n", studentCount, filename);
}

void saveStudents(const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        printf("Error: cannot write to %s.\n", filename);
        return;
    }

    for (int i = 0; i < studentCount; i++)
    {
        Student *s = &students[i];
        fprintf(fp, "%s %s", s->account, s->password);
        for (int j = 0; j < s->borrowCount; j++)
        {
            fprintf(fp, " %s %s", s->borrowed[j].isbn, s->borrowed[j].date);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void saveBooks(const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        printf("Error: cannot write to %s.\n", filename);
        return;
    }

    for (int i = 0; i < bookCount; i++)
    {
        Book *b = &books[i];
        fprintf(fp, "&%s& &%s& %s %s %s %s",
                b->title, b->author, b->year, b->edition, b->isbn, b->copyId);
        if (strlen(b->borrower) > 0)
        {
            fprintf(fp, " %s", b->borrower);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

/* ================================================================
 * Student helpers
 * ================================================================ */

int findStudentByAccount(const char *account)
{
    for (int i = 0; i < studentCount; i++)
    {
        if (strcmp(students[i].account, account) == 0)
            return i;
    }
    return -1;
}

int authenticateStudent(const char *account, const char *password)
{
    int idx = findStudentByAccount(account);
    if (idx == -1)
        return -1;
    if (strcmp(students[idx].password, password) != 0)
        return -1;
    return idx;
}

int hasOverdueBooks(int studentIdx, const char *currentDate)
{
    Student *s = &students[studentIdx];
    for (int i = 0; i < s->borrowCount; i++)
    {
        int days = daysBetween(s->borrowed[i].date, currentDate);
        if (days > 30)
            return 1;
    }
    return 0;
}

/* ================================================================
 * Book helpers
 * ================================================================ */

int findBookByIsbnAndCopy(const char *isbn, const char *copyId)
{
    for (int i = 0; i < bookCount; i++)
    {
        if (strcmp(books[i].isbn, isbn) == 0 && strcmp(books[i].copyId, copyId) == 0)
            return i;
    }
    return -1;
}

int findBooksByTitle(const char *title, int *resultIndices, int maxResults)
{
    int count = 0;
    for (int i = 0; i < bookCount && count < maxResults; i++)
    {
        if (strstr(books[i].title, title) != NULL)
        {
            resultIndices[count++] = i;
        }
    }
    return count;
}

int findBooksByAuthor(const char *author, int *resultIndices, int maxResults)
{
    int count = 0;
    for (int i = 0; i < bookCount && count < maxResults; i++)
    {
        if (strstr(books[i].author, author) != NULL)
        {
            resultIndices[count++] = i;
        }
    }
    return count;
}

int findBooksByIsbn(const char *isbn, int *resultIndices, int maxResults)
{
    int count = 0;
    for (int i = 0; i < bookCount && count < maxResults; i++)
    {
        if (strcmp(books[i].isbn, isbn) == 0)
        {
            resultIndices[count++] = i;
        }
    }
    return count;
}

int isBookAvailable(int bookIdx)
{
    return strlen(books[bookIdx].borrower) == 0;
}

static void printBookInfo(int idx)
{
    Book *b = &books[idx];
    printf("  Title: %s\n", b->title);
    printf("  Author: %s\n", b->author);
    printf("  Year: %s\n", b->year);
    printf("  Edition: %s\n", b->edition);
    printf("  ISBN: %s\n", b->isbn);
    printf("  Copy: %s\n", b->copyId);
    printf("  Status: %s\n", isBookAvailable(idx) ? "Available" : "Borrowed");
}

/* ================================================================
 * Command Implementations
 * ================================================================ */

void cmdBorrowBook(const char *currentDate)
{
    char account[ACCOUNT_LEN], password[PASSWORD_LEN];
    printf("=== Borrow Book ===\n");
    printf("Account: ");
    fgets(account, sizeof(account), stdin);
    trimNewline(account);
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    trimNewline(password);

    int sIdx = authenticateStudent(account, password);
    if (sIdx == -1)
    {
        printf("Authentication failed. Access denied.\n");
        return;
    }

    if (hasOverdueBooks(sIdx, currentDate))
    {
        printf("You have overdue books. Cannot borrow until they are returned.\n");
        return;
    }

    char cont = 'y';
    while (cont == 'y' || cont == 'Y')
    {
        char input[TITLE_LEN];
        char copyId[COPYID_LEN];
        printf("Enter book title or ISBN: ");
        fgets(input, sizeof(input), stdin);
        trimNewline(input);
        printf("Enter copy ID: ");
        fgets(copyId, sizeof(copyId), stdin);
        trimNewline(copyId);

        int foundIdx = -1;
        for (int i = 0; i < bookCount; i++)
        {
            if (strcmp(books[i].isbn, input) == 0 && strcmp(books[i].copyId, copyId) == 0)
            {
                foundIdx = i;
                break;
            }
        }
        if (foundIdx == -1)
        {
            int indices[MAX_BOOKS];
            int cnt = findBooksByTitle(input, indices, MAX_BOOKS);
            for (int i = 0; i < cnt; i++)
            {
                if (strcmp(books[indices[i]].copyId, copyId) == 0)
                {
                    foundIdx = indices[i];
                    break;
                }
            }
        }

        if (foundIdx == -1)
        {
            printf("Book not found.\n");
        }
        else if (!isBookAvailable(foundIdx))
        {
            printf("This copy is already borrowed. Cannot borrow.\n");
        }
        else
        {
            strcpy(books[foundIdx].borrower, account);
            Student *s = &students[sIdx];
            if (s->borrowCount < MAX_BORROWED)
            {
                strcpy(s->borrowed[s->borrowCount].isbn, books[foundIdx].isbn);
                strcpy(s->borrowed[s->borrowCount].date, currentDate);
                s->borrowCount++;
            }
            char dueDate[DATE_LEN];
            addDays(currentDate, 30, dueDate);
            printf("Book borrowed successfully!\n");
            printf("Due date: %s\n", dueDate);
        }

        printf("Continue borrowing? (Y/N): ");
        char resp[10];
        fgets(resp, sizeof(resp), stdin);
        trimNewline(resp);
        cont = resp[0];
    }

    printf("Logged out.\n");
}

void cmdReturnBook(void)
{
    char input[TITLE_LEN];
    char copyId[COPYID_LEN];
    printf("=== Return Book ===\n");
    printf("Enter book title or ISBN: ");
    fgets(input, sizeof(input), stdin);
    trimNewline(input);
    printf("Enter copy ID: ");
    fgets(copyId, sizeof(copyId), stdin);
    trimNewline(copyId);

    int foundIdx = -1;
    for (int i = 0; i < bookCount; i++)
    {
        if (strcmp(books[i].isbn, input) == 0 && strcmp(books[i].copyId, copyId) == 0)
        {
            foundIdx = i;
            break;
        }
    }
    if (foundIdx == -1)
    {
        int indices[MAX_BOOKS];
        int cnt = findBooksByTitle(input, indices, MAX_BOOKS);
        for (int i = 0; i < cnt; i++)
        {
            if (strcmp(books[indices[i]].copyId, copyId) == 0)
            {
                foundIdx = indices[i];
                break;
            }
        }
    }

    if (foundIdx == -1)
    {
        printf("Book not found.\n");
        return;
    }

    if (isBookAvailable(foundIdx))
    {
        printf("This book is not currently borrowed.\n");
        return;
    }

    char *borrowerAccount = books[foundIdx].borrower;
    int sIdx = findStudentByAccount(borrowerAccount);
    if (sIdx != -1)
    {
        Student *s = &students[sIdx];
        for (int i = 0; i < s->borrowCount; i++)
        {
            if (strcmp(s->borrowed[i].isbn, books[foundIdx].isbn) == 0)
            {
                for (int j = i; j < s->borrowCount - 1; j++)
                {
                    s->borrowed[j] = s->borrowed[j + 1];
                }
                s->borrowCount--;
                break;
            }
        }
    }

    books[foundIdx].borrower[0] = '\0';
    printf("Book returned successfully.\n");
}

void cmdAddBook(void)
{
    Book newBook;
    memset(&newBook, 0, sizeof(Book));

    printf("=== Add Book ===\n");
    printf("Title: ");
    fgets(newBook.title, sizeof(newBook.title), stdin);
    trimNewline(newBook.title);

    printf("Author: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    trimNewline(newBook.author);

    printf("Year: ");
    fgets(newBook.year, sizeof(newBook.year), stdin);
    trimNewline(newBook.year);

    printf("Edition: ");
    fgets(newBook.edition, sizeof(newBook.edition), stdin);
    trimNewline(newBook.edition);

    printf("ISBN: ");
    fgets(newBook.isbn, sizeof(newBook.isbn), stdin);
    trimNewline(newBook.isbn);

    printf("Copy ID: ");
    fgets(newBook.copyId, sizeof(newBook.copyId), stdin);
    trimNewline(newBook.copyId);

    int dupeIdx = findBookByIsbnAndCopy(newBook.isbn, newBook.copyId);
    if (dupeIdx != -1)
    {
        printf("Error: Book with same ISBN and Copy ID already exists.\n");
        return;
    }

    if (bookCount >= MAX_BOOKS)
    {
        printf("Error: Book storage is full.\n");
        return;
    }

    newBook.borrower[0] = '\0';
    books[bookCount] = newBook;
    bookCount++;
    printf("Book added successfully.\n");
}

void cmdDeleteBook(void)
{
    char isbn[ISBN_LEN], copyId[COPYID_LEN];
    printf("=== Delete Book ===\n");
    printf("ISBN: ");
    fgets(isbn, sizeof(isbn), stdin);
    trimNewline(isbn);
    printf("Copy ID: ");
    fgets(copyId, sizeof(copyId), stdin);
    trimNewline(copyId);

    int idx = findBookByIsbnAndCopy(isbn, copyId);
    if (idx == -1)
    {
        printf("Book not found.\n");
        return;
    }

    if (!isBookAvailable(idx))
    {
        printf("Error: Cannot delete a book that is currently borrowed.\n");
        return;
    }

    printf("Are you sure you want to delete this book? (Y/N): ");
    char resp[10];
    fgets(resp, sizeof(resp), stdin);
    trimNewline(resp);
    if (resp[0] != 'Y' && resp[0] != 'y')
    {
        printf("Deletion cancelled.\n");
        return;
    }

    for (int i = idx; i < bookCount - 1; i++)
    {
        books[i] = books[i + 1];
    }
    bookCount--;
    printf("Book deleted successfully.\n");
}

void cmdQueryBook(void)
{
    printf("=== Query Books ===\n");
    printf("Search by:\n");
    printf("  1. Title\n");
    printf("  2. Author\n");
    printf("  3. ISBN\n");
    printf("Choice: ");
    char choice[10];
    fgets(choice, sizeof(choice), stdin);
    trimNewline(choice);

    char query[TITLE_LEN];
    printf("Enter search keyword: ");
    fgets(query, sizeof(query), stdin);
    trimNewline(query);

    int indices[MAX_BOOKS];
    int count = 0;

    if (strcmp(choice, "1") == 0)
    {
        count = findBooksByTitle(query, indices, MAX_BOOKS);
    }
    else if (strcmp(choice, "2") == 0)
    {
        count = findBooksByAuthor(query, indices, MAX_BOOKS);
    }
    else if (strcmp(choice, "3") == 0)
    {
        count = findBooksByIsbn(query, indices, MAX_BOOKS);
    }
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    if (count == 0)
    {
        printf("No books found.\n");
        return;
    }

    printf("Found %d result(s):\n", count);
    for (int i = 0; i < count; i++)
    {
        printf("---\n");
        printBookInfo(indices[i]);
    }
}

void cmdViewMyBorrows(void)
{
    char account[ACCOUNT_LEN], password[PASSWORD_LEN];
    printf("=== View My Borrows ===\n");
    printf("Account: ");
    fgets(account, sizeof(account), stdin);
    trimNewline(account);
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    trimNewline(password);

    int sIdx = authenticateStudent(account, password);
    if (sIdx == -1)
    {
        printf("Authentication failed.\n");
        return;
    }

    Student *s = &students[sIdx];
    if (s->borrowCount == 0)
    {
        printf("You have no borrowed books.\n");
        return;
    }

    printf("Borrowed books for %s:\n", account);
    for (int i = 0; i < s->borrowCount; i++)
    {
        int bIdx = -1;
        for (int j = 0; j < bookCount; j++)
        {
            if (strcmp(books[j].isbn, s->borrowed[i].isbn) == 0)
            {
                bIdx = j;
                break;
            }
        }
        printf("--- Borrow %d ---\n", i + 1);
        if (bIdx != -1)
        {
            printf("  Title: %s\n", books[bIdx].title);
            printf("  Author: %s\n", books[bIdx].author);
            printf("  ISBN: %s\n", books[bIdx].isbn);
            for (int k = 0; k < bookCount; k++)
            {
                if (strcmp(books[k].isbn, s->borrowed[i].isbn) == 0 &&
                    strcmp(books[k].borrower, account) == 0)
                {
                    printf("  Copy ID: %s\n", books[k].copyId);
                    break;
                }
            }
        }
        else
        {
            printf("  ISBN: %s\n", s->borrowed[i].isbn);
        }
        printf("  Borrow Date: %s\n", s->borrowed[i].date);
    }
}

void cmdDumpAll(void)
{
    printf("=== All Books (Total: %d) ===\n", bookCount);
    for (int i = 0; i < bookCount; i++)
    {
        printf("--- Book %d ---\n", i + 1);
        printBookInfo(i);
    }
}