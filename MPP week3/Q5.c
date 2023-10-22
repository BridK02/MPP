//Check if y is greater than x, and z is greater than y from three given integers x, y, z. Print appropriate messages using printf().
#include <stdio.h>
#include <stdbool.h>

int main() {
    int x = 10;
    int y = 15;
    int z = 20;

    if (y > x && z > y) {
        printf("y is greater than x, and z is greater than y.\n");
    } else {
        printf("Conditions not met.\n");
    }

    return 0;
}
