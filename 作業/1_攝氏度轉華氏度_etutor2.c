#include <stdio.h>

int main()
{
    float x, y;

    // 輸入攝氏溫度
    scanf("%f", &x);

    // 計算華氏溫度
    y = x * 9 / 5 + 32;

    // 輸出結果（小數點後一位）
    printf("%.1f\n", y);

    return 0;
}