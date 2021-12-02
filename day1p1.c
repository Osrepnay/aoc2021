#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main () {
    FILE* stream = fopen("inputs/input1", "r");
    int increasing = 0;
    int previous = INT_MAX;
    int num = 0;
    while (fscanf(stream, "%d", &num) != EOF) {
        if (num > previous) {
            increasing++;
        }
        previous = num;
    }
    printf("%d\n", increasing);
    fclose(stream);
    return 0;
}
