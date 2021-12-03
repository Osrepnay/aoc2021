#include <stdio.h>
#include <stdlib.h>
#define NUM_BITS 12

int main () {
    FILE *stream = fopen("inputs/input3", "r");
    
    int bins[1000];
    int bins_len = 0;

    char line[NUM_BITS + 1];
    while (fscanf(stream, "%s", line) != EOF) {
        bins[bins_len++] = strtol(line, NULL, 2);
    }

    int gamma = 0;
    int epsilon = 0;
    int bit_set_times[NUM_BITS] = {0};
    for (int i = 0; i < bins_len; ++i) {
        for (int j = 0; j < NUM_BITS; ++j) {
            if ((bins[i] >> j) & 1) {
                bit_set_times[j]++;
            }
        }
    }
    for (int i = 0; i < NUM_BITS; ++i) {
        if (bit_set_times[i] * 2 > bins_len) {
            gamma |= 1 << i;
        } else {
            epsilon |= 1 << i;
        }
    }
    printf("%d\n", gamma * epsilon);
    
    fclose(stream);
    return 0;
}
