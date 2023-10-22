//Accept two integers and return true (1) if either one is 5 or their sum or difference is 5.
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // Include the <stdlib.h> header

int main() {
    int num1 = 14;
    int num2 = 6;
    int sum = num1 + num2;
    int diff = abs(num1 - num2); // Use the abs function

    bool result = (num1 == 5 || num2 == 5 || sum == 5 || diff == 5);

    if (result) {
        printf("True\n");
    } else {
        printf("False\n");
    }

    return 0;
}
