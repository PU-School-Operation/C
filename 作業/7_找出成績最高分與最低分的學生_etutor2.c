#include <stdio.h>

int main()
{
    int n;
    int sID, score;
    int maxID, maxScore;
    int minID, minScore;

    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d %d", &sID, &score);

        if (i == 0)
        {
            maxID = sID;
            maxScore = score;
            minID = sID;
            minScore = score;
        }
        else
        {
            if (score > maxScore)
            {
                maxScore = score;
                maxID = sID;
            }

            if (score < minScore)
            {
                minScore = score;
                minID = sID;
            }
        }
    }

    printf("%d %d\n", maxID, maxScore);
    printf("%d %d\n", minID, minScore);

    return 0;
}
