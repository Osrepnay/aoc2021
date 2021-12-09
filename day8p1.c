#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
    FILE *stream = fopen("inputs/input8", "r");

    char *digits[1000][10];
    int digits_len = 0;
    char *outputs[1000][4];
    int outputs_len = 0;
    while (fgetc(stream) != EOF) {
        for (int i = 0; i < 10; ++i) {
            digits[digits_len][i] = malloc(8 * sizeof (char));
            fscanf(stream, "%s", digits[digits_len][i]);
        }
        ++digits_len;

        char pipe[2];
        fscanf(stream, "%s", pipe);

        for (int i = 0; i < 4; ++i) {
            outputs[outputs_len][i] = malloc(8 * sizeof (char));
            fscanf(stream, "%s", outputs[outputs_len][i]);
        }
        ++outputs_len;
    }

    int special = 0; // number of 1, 4, 7, or 8 found in outputs
    for (int i = 0; i < outputs_len; ++i) {
        for (int j = 0; j < 4; ++j) {
            int digit_length = strlen(outputs[i][j]);
            if (digit_length == 2 || digit_length == 3 || digit_length == 4 || digit_length == 7) {
                ++special;
            }
        }
    }

    printf("%d\n", special);

    fclose(stream);
    return 0;
}
