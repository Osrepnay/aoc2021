#include <stdlib.h>
#include <stdio.h>

int comp (const void *ap, const void *bp) {
    int a = *((int*) ap);
    int b = *((int*) bp);
    if (a > b) {
        return 1;
    } else if (a < b) {
        return -1;
    } else {
        return 0;
    }
}

int main () {
    FILE *stream = fopen("inputs/input7", "r");

    int horiz[5000];
    int horiz_len = 0;
    do {
        fscanf(stream, "%d", horiz + (horiz_len++));
    } while (fgetc(stream) == ',');
    qsort(horiz, horiz_len, sizeof (*horiz), comp);

    int median = horiz[horiz_len / 2]; // input length is always even

    int fuel_needed = 0;
    for (int i = 0; i < horiz_len; ++i) {
        fuel_needed += abs(horiz[i] - median);
    }
    printf("%d\n", fuel_needed);

    fclose(stream);
    return 0;
}
