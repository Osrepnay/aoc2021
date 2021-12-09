#include <math.h>
#include <stdio.h>

int num_set (unsigned char num) {
    int set = 0;
    for (int i = 0; i < 7; ++i) {
        set += (num >> i) & 1;
    }
    return set;
}

int num_overlapping (unsigned char digit1, unsigned char digit2) {
    return num_set(digit1 & digit2);
}

int main () {
    FILE *stream = fopen("inputs/input8", "r");

    unsigned char digits[1000][10];
    unsigned char outputs[1000][4];
    int lines = 0;

    int temp;
    int temp2;
    while ((temp = fgetc(stream)) != EOF) {
        // cursed but
        if ((temp2 = fgetc(stream)) == EOF) {
            ungetc(temp2, stream);
            ungetc(temp, stream);
            break;
        }
        ungetc(temp2, stream);
        ungetc(temp, stream);

        for (int i = 0; i < 10; ++i) {
            char digit[8];
            fscanf(stream, "%s", digit);
            for (int j = 0; digit[j] != '\0'; ++j) {
                digits[lines][i] |= 1 << (digit[j]  - 'a');
            }
        }

        char pipe[2];
        fscanf(stream, "%s", pipe);

        for (int i = 0; i < 4; ++i) {
            char digit[8];
            fscanf(stream, "%s", digit);
            for (int j = 0; digit[j] != '\0'; ++j) {
                outputs[lines][i] |= 1 << (digit[j]  - 'a');
            }
        }
        ++lines;
    }

    int sum = 0;
    for (int i = 0; i < lines; ++i) {
        // start figuring out numbers
        unsigned char digits_mappings[10];
        for (int pj = 0; pj < 14; ++pj) {
            int j = pj;
            unsigned char *arr = digits[i];
            if (pj >= 10) {
                j = pj - 10;
                arr = outputs[i];
            }
            switch (num_set(arr[j])) {
                case 2:
                    digits_mappings[1] = arr[j];
                    break;
                case 3:
                    digits_mappings[7] = arr[j];
                    break;
                case 4:
                    digits_mappings[4] = arr[j];
                    break;
                case 7:
                    digits_mappings[8] = arr[j];
                    break;
            }
        }

        int output_digits[4] = { 0 };
        for (int j = 0; j < 4; ++j) {
            switch (num_set(outputs[i][j])) {
                case 2: // 1
                    output_digits[j] = 1;
                    break;
                case 3: // 7
                    output_digits[j] = 7;
                    break;
                case 4: // 4
                    output_digits[j] = 4;
                    break;
                case 5: // 2, 3, 5
                    // check if 3
                    if (num_overlapping(digits_mappings[1], outputs[i][j]) == 2) {
                        output_digits[j] = 3;
                    } else if (num_overlapping(digits_mappings[4], outputs[i][j]) == 3) { // decide between 2 and 5
                        output_digits[j] = 5;
                    } else {
                        output_digits[j] = 2;
                    }
                    break;
                case 6: // 6, 9 (nice)
                    if (num_overlapping(digits_mappings[7], outputs[i][j]) == 2) {
                        output_digits[j] = 6;
                    } else if (num_overlapping(digits_mappings[4], outputs[i][j]) == 4) {
                        output_digits[j] = 9;
                    } else {
                        output_digits[j] = 0;
                    }
                    break;
                case 7: // 8
                    output_digits[j] = 8;
                    break;
            }
        }
        for (int j = 0; j < 4; ++j) {
            sum += 1000 / (pow(10, j)) * output_digits[j];
        }
    }

    printf("%d\n", sum);

    fclose(stream);
    return 0;
}
