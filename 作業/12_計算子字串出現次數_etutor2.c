#include <stdio.h>
#include <string.h>

int main()
{
    char s1[52], s2[52];
    int count = 0;

    if (fgets(s1, sizeof(s1), stdin) == NULL)
    {
        return 0;
    }

    if (fgets(s2, sizeof(s2), stdin) == NULL)
    {
        return 0;
    }

    s1[strcspn(s1, "\n")] = '\0';
    s2[strcspn(s2, "\n")] = '\0';

    int len1 = (int)strlen(s1);
    int len2 = (int)strlen(s2);

    if (len2 == 0)
    {
        printf("出現次數=0\n");
        return 0;
    }

    for (int i = 0; i <= len1 - len2; i++)
    {
        int matched = 1;

        for (int j = 0; j < len2; j++)
        {
            if (s1[i + j] != s2[j])
            {
                matched = 0;
                break;
            }
        }

        if (matched)
        {
            printf("%d\n", i);
            count++;
        }
    }

    printf("出現次數=%d\n", count);

    return 0;
}
