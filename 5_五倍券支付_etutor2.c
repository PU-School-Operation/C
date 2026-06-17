#include <stdio.h>

int main()
{
    int N;
    scanf("%d", &N);

    int best_n1 = 0, best_n2 = 0, best_n3 = 0;
    int max_n3 = -1, max_n2 = -1, max_n1 = -1;

    for (int n3 = 0; n3 <= 3; n3++)
    {
        for (int n2 = 0; n2 <= 2; n2++)
        {
            for (int n1 = 0; n1 <= 5; n1++)
            {
                int total = 1000 * n3 + 500 * n2 + 200 * n1;

                if (total <= N && total > 0)
                {

                    if (n3 > max_n3 ||
                        (n3 == max_n3 && n2 > max_n2) ||
                        (n3 == max_n3 && n2 == max_n2 && n1 > max_n1))
                    {
                        max_n3 = n3;
                        max_n2 = n2;
                        max_n1 = n1;
                        best_n1 = n1;
                        best_n2 = n2;
                        best_n3 = n3;
                    }
                }
            }
        }
    }

    printf("200-%d, 500-%d, 1000-%d\n", best_n1, best_n2, best_n3);

    return 0;
}
