#include <stdio.h>

int main()
{
    float h, w, bmi;

    scanf("%f %f", &h, &w);

    float h_m = h / 100.0;
    bmi = w / (h_m * h_m);

    printf("BMI = %.1f\n", bmi);

    if (bmi < 18.5)
    {
        printf("體重過輕\n");
    }
    else if (bmi < 24)
    {
        printf("體重正常\n");
    }
    else
    {
        printf("體重過重\n");
    }

    return 0;
}
