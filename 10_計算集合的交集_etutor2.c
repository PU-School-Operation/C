#include <stdio.h>

int contains(int arr[], int size, int value)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == value)
        {
            return 1;
        }
    }
    return 0;
}

int main()
{
    int a[10], b[10];
    int inter[10];
    int interCount = 0;

    for (int i = 0; i < 10; i++)
    {
        scanf("%d", &a[i]);
    }

    for (int i = 0; i < 10; i++)
    {
        scanf("%d", &b[i]);
    }

    for (int i = 0; i < 10; i++)
    {
        if (contains(b, 10, a[i]) && !contains(inter, interCount, a[i]))
        {
            inter[interCount++] = a[i];
        }
    }

    for (int i = 0; i < interCount - 1; i++)
    {
        for (int j = i + 1; j < interCount; j++)
        {
            if (inter[i] > inter[j])
            {
                int temp = inter[i];
                inter[i] = inter[j];
                inter[j] = temp;
            }
        }
    }

    for (int i = 0; i < interCount; i++)
    {
        if (i > 0)
        {
            printf(" ");
        }
        printf("%d", inter[i]);
    }
    printf("\n");

    return 0;
}
