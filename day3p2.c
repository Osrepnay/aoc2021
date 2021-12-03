#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_BITS 12

// WARNING: bit is reversed (ltr)
int filterOx (int bins[], int bins_len, int new_bins[], int bit) {
    int bit_set_times = 0;
    for (int i = 0; i < bins_len; ++i) {
            if ((bins[i] >> (NUM_BITS - bit - 1)) & 1) {
                bit_set_times++;
            }
    }
    int most_common = bit_set_times * 2 >= bins_len;
    int new_bins_len = 0;
    for (int i = 0; i < bins_len; ++i) {
        if (((bins[i] >> (NUM_BITS - bit - 1)) & 1) == (most_common & 1)) {
            new_bins[new_bins_len++] = bins[i];
        }
    }
    return new_bins_len;
}

int filterCO (int bins[], int bins_len, int new_bins[], int bit) {
    int bit_set_times = 0;
    for (int i = 0; i < bins_len; ++i) {
            if ((bins[i] >> (NUM_BITS - bit - 1)) & 1) {
                bit_set_times++;
            }
    }
    int least_common = bit_set_times * 2 < bins_len;
    int new_bins_len = 0;
    for (int i = 0; i < bins_len; ++i) {
        if (((bins[i] >> (NUM_BITS - bit - 1)) & 1) == (least_common & 1)) {
            new_bins[new_bins_len++] = bins[i];
        }
    }
    return new_bins_len;
}

int main () {
    FILE *stream = fopen("inputs/input3", "r");
    
    int bins[1000];
    int bins_len = 0;

    char line[NUM_BITS + 1];
    while (fscanf(stream, "%s", line) != EOF) {
        bins[bins_len++] = strtol(line, NULL, 2);
    }

    int ox_bins[1000];
    int ox_bins_len;
    for (; ox_bins_len < bins_len; ++ox_bins_len) {
        ox_bins[ox_bins_len] = bins[ox_bins_len];
    }
    for (int i = 0; ox_bins_len > 1; ++i) {
        ox_bins_len = filterOx(ox_bins, ox_bins_len, ox_bins, i);
    }

    int co_bins[1000];
    int co_bins_len;
    for (; co_bins_len < bins_len; ++co_bins_len) {
        co_bins[co_bins_len] = bins[co_bins_len];
    }
    for (int i = 0; co_bins_len > 1; ++i) {
        co_bins_len = filterCO(co_bins, co_bins_len, co_bins, i);
    }
    printf("%d\n", co_bins[0] * ox_bins[0]);
    
    fclose(stream);
    return 0;
}
