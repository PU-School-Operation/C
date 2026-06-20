#include <stdio.h>

typedef struct
{
    char id[9];
    int total;
} Student;

int main()
{
    int n;
    Student stu[50];

    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        int s1, s2, s3;
        scanf("%8s %d %d %d", stu[i].id, &s1, &s2, &s3);
        stu[i].total = s1 + s2 + s3;
    }

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (stu[j].total > stu[j + 1].total)
            {
                Student temp = stu[j];
                stu[j] = stu[j + 1];
                stu[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        printf("%s %d\n", stu[i].id, stu[i].total);
    }

    return 0;
}
