#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    int sID, s1, s2, s3;
    int bestID = 0;
    double bestAvg = -1.0;

    for (int i = 0; i < n; i++)
    {
        scanf("%d %d %d %d", &sID, &s1, &s2, &s3);
        double avg = (s1 + s2 + s3) / 3.0;

        if (avg > bestAvg)
        {
            bestAvg = avg;
            bestID = sID;
        }
    }

    printf("%d %.1f\n", bestID, bestAvg);

    return 0;
}
