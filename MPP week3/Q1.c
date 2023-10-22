//Calculate the absolute difference between n and 51. If n is greater than 51, return triple the absolute difference.//
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n = -10;
    int absolute_diff = abs(n - 51);

    if (n > 51) {
        absolute_diff *= 3;
    }

    printf("Absolute difference = %d\n", absolute_diff);
    return 0;
}
