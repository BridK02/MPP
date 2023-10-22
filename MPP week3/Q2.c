//Check two given integers and return true (1) if one of them is 30 or if their sum is 30.
#include <stdio.h>
#include <stdbool.h>

int main() {
    int num1 = 15;
    int num2 = 20;
    bool result = (num1 == 30 || num2 == 30 || num1 + num2 == 30);

    if (result) {
        printf("True\n");
    } else {
        printf("False\n");
    }

    return 0;
}
