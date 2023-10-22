//Check if two or more non-negative given integers have the same rightmost digit.
#include <stdio.h>
#include <stdbool.h>

int main() {
    int num1 = 25;
    int num2 = 15;

    int last_digit1 = num1 % 10;
    int last_digit2 = num2 % 10;

    if (last_digit1 == last_digit2) {
        printf("Both numbers have the same rightmost digit.\n");
    } else {
        printf("Numbers do not have the same rightmost digit.\n");
    }

    return 0;
}
