#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

int main () {
    FILE *stream = fopen("inputs/input7", "r");

    int horiz[5000];
    int horiz_len = 0;
    do {
        fscanf(stream, "%d", horiz + (horiz_len++));
    } while (fgetc(stream) == ',');

    int min = INT_MAX;
    int max = INT_MIN;
    for (int i = 0; i < horiz_len; ++i) {
        if (horiz[i] > max) {
            max = horiz[i];
        } else if (horiz[i] < min) {
            min = horiz[i];
        }
    }

    int min_fuel = INT_MAX;
    for (int i = min; i <= max; ++i) {
        int fuel_needed = 0;
        for (int j = 0; j < horiz_len; ++j) {
            int diff = abs(horiz[j] - i);
            fuel_needed += (diff * (diff + 1)) / 2;
        }
        if (fuel_needed < min_fuel) {
            min_fuel = fuel_needed;
        }
    }
    printf("%d\n", min_fuel);

    fclose(stream);
    return 0;
}
