//Compute the sum of two given integers. If the sum is in the range 10..20 inclusive, return 30.
#include <stdio.h>
#include <stdbool.h>

int main() {
    int num1 = 5;
    int num2 = 7;
    int sum = num1 + num2;

    if (sum >= 10 && sum <= 20) {
        sum = 30;
    }

    printf("Sum: %d\n", sum);
    return 0;
}
