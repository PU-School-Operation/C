#include <stdio.h>

int main()
{
    int n;
    int sID[50], score[50];
    int queryID;
    int found = 0;

    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d %d", &sID[i], &score[i]);
    }

    scanf("%d", &queryID);

    for (int i = 0; i < n; i++)
    {
        if (sID[i] == queryID)
        {
            printf("%d\n", score[i]);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("ID not exist.\n");
    }

    return 0;
}
