#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    char s[1000];
    int i, j, isPalindrome = 1;

    fgets(s, sizeof(s), stdin);

    s[strcspn(s, "\n")] = '\0';

    i = 0;
    j = strlen(s) - 1;

    while (i < j)
    {
        if (tolower((unsigned char)s[i]) != tolower((unsigned char)s[j]))
        {
            isPalindrome = 0;
            break;
        }
        i++;
        j--;
    }

    if (isPalindrome)
    {
        printf("YES\n");
    }
    else
    {
        printf("NO\n");
    }

    return 0;
}
