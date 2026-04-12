#include <stdio.h>
#include <string.h>
#include <ctype.h>

int letterValue(char ch)
{
    switch (toupper((unsigned char)ch))
    {
    case 'A':
        return 10;
    case 'B':
        return 11;
    case 'C':
        return 12;
    case 'D':
        return 13;
    case 'E':
        return 14;
    case 'F':
        return 15;
    case 'G':
        return 16;
    case 'H':
        return 17;
    case 'I':
        return 34;
    case 'J':
        return 18;
    case 'K':
        return 19;
    case 'L':
        return 20;
    case 'M':
        return 21;
    case 'N':
        return 22;
    case 'O':
        return 35;
    case 'P':
        return 23;
    case 'Q':
        return 24;
    case 'R':
        return 25;
    case 'S':
        return 26;
    case 'T':
        return 27;
    case 'U':
        return 28;
    case 'V':
        return 29;
    case 'W':
        return 32;
    case 'X':
        return 30;
    case 'Y':
        return 31;
    case 'Z':
        return 33;
    default:
        return -1;
    }
}

int main()
{
    char id[100];
    int len, i, sum, code;

    if (fgets(id, sizeof(id), stdin) == NULL)
    {
        return 0;
    }

    id[strcspn(id, "\n")] = '\0';
    len = strlen(id);

    if (len != 10 || !isalpha((unsigned char)id[0]))
    {
        printf("Invalid ID\n");
        return 0;
    }

    for (i = 1; i < 10; i++)
    {
        if (!isdigit((unsigned char)id[i]))
        {
            printf("Invalid ID\n");
            return 0;
        }
    }

    code = letterValue(id[0]);
    if (code == -1)
    {
        printf("Invalid ID\n");
        return 0;
    }

    sum = code / 10 + (code % 10) * 9;

    for (i = 1; i < 9; i++)
    {
        sum += (id[i] - '0') * (9 - i);
    }

    sum += (id[9] - '0');

    if (sum % 10 == 0)
    {
        printf("Valid ID\n");
    }
    else
    {
        printf("Invalid ID\n");
    }

    return 0;
}
