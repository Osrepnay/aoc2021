#include <stdio.h>

int main () {
    FILE *stream = fopen("inputs/input6", "r");

    long long num_lanternfish_with_days[9] = { 0 }; // index represents how many days a lanternfish has left

    int lanternfish_num;
    do {
        fscanf(stream, "%d", &lanternfish_num);
        ++num_lanternfish_with_days[lanternfish_num];
    } while (fgetc(stream) == ',');

    for (int i = 0; i < 256; ++i) {
        long long reproducing = num_lanternfish_with_days[0];

        for (int j = 0; j < 8; ++j) {
            num_lanternfish_with_days[j] = num_lanternfish_with_days[j + 1];
        }
        num_lanternfish_with_days[8] = reproducing;
        num_lanternfish_with_days[6] += reproducing;
    }

    long long sum = 0;
    for (int i = 0; i < 9; ++i) {
        sum += num_lanternfish_with_days[i];
    }

    printf("%lld\n", sum);

    fclose(stream);
    return 0;
}
