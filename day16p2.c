#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_HEX_LEN 1500

// gets n digits in base
int get_n (char *str, int n, int base) {
    char tmp = str[n];
    str[n] = '\0';
    int ret = strtol(str, NULL, base);
    str[n] = tmp;
    return ret;
}

int main() {
    FILE *stream = fopen("inputs/input16", "r");

    char hex[MAX_HEX_LEN + 1];
    fscanf(stream, "%s", hex);

    char binary[MAX_HEX_LEN * 4 + 1];
    int binary_len = 0;
    for (int i = 0; hex[i] != '\0'; ++i) {
        int bin = get_n(hex + i, 1, 16);
        for (int j = 3; j >= 0; --j) {
            binary[binary_len++] = '0' + (bin >> j & 1);
        }
    }
    binary[binary_len] = '\0';

    int operators[MAX_HEX_LEN * 4];
    int operators_num_left[MAX_HEX_LEN * 4];
    // whether operators's counting type is based on bits, not packets
    bool operators_num_left_type[MAX_HEX_LEN * 4];
    long operators_args[MAX_HEX_LEN * 4][MAX_HEX_LEN / 10];
    int operators_args_lens[MAX_HEX_LEN * 4] = { 0 };
    int operators_len = 1;
    // fake surrounding operator for output storing
    operators[0] = -1;
    operators_num_left[0] = 1;
    operators_num_left_type[0] = false;
    for (int i = 0; i < binary_len; ) {
        if (strtol(binary + i, NULL, 2) == 0) { // rest is just padding
            break;
        }
        int orig_i = i;
        i += 3; // skip version number
        int type_id = get_n(binary + i, 3, 2);
        i += 3;

        if (type_id == 4) {
            char whole_number[MAX_HEX_LEN * 4 + 1];
            int whole_number_len = 0;
            int last_group_flag;
            do {
                last_group_flag = get_n(binary + i, 1, 2);
                ++i;
                int whole_number_part = get_n(binary + i, 4, 2); // allowed to be int because max bits is 4
                i += 4;
                for (int j = 3; j >= 0; --j) {
                    whole_number[whole_number_len++] = '0' + ((whole_number_part >> j) & 1);
                }
            } while (last_group_flag != 0);
            whole_number[whole_number_len] = '\0';
            long whole_number_dec = strtol(whole_number, NULL, 2);
            operators_args[operators_len - 1][operators_args_lens[operators_len - 1]++] = whole_number_dec;

            // update operator counts
            for (int j = 0; j < operators_len; ++j) {
                if (operators_num_left_type[j]) {
                    operators_num_left[j] -= i - orig_i;
                }
            }
            if (!operators_num_left_type[operators_len - 1]) {
                operators_num_left[operators_len - 1] -= 1;
            }
        } else {
            int len_type_id = get_n(binary + i, 1, 2);
            ++i;
            int args_size;
            bool args_type;
            if (len_type_id == 0) {
                args_type = true;
                args_size = get_n(binary + i, 15, 2);
                i += 15;
            } else {
                args_type = false;
                args_size = get_n(binary + i, 11, 2);
                i += 11;
            }

            // update operator counts
            for (int j = 0; j < operators_len; ++j) {
                if (operators_num_left_type[j]) {
                    operators_num_left[j] -= i - orig_i;
                }
            }
            // skip updating packets left because this whole op is subpacket

            // add new operator
            operators[operators_len] = type_id;
            operators_num_left[operators_len] = args_size;
            operators_num_left_type[operators_len] = args_type;
            ++operators_len;
        }

        // while instead of if to clear all operators, e.g. if completion of operator also completes outer operator
        while (operators_len > 0 && operators_num_left[operators_len - 1] <= 0) {
            --operators_len;
            // if surrounding operator exists and is using packet count decrease needed packets
            if (!operators_num_left_type[operators_len - 1]) {
                operators_num_left[operators_len - 1] -= 1;
            }
            long ret = -1;
            switch (operators[operators_len]) {
                case 0:
                {
                    long sum = 0;
                    for (int j = 0; j < operators_args_lens[operators_len]; ++j) {
                        sum += operators_args[operators_len][j];
                    }
                    ret = sum;
                    break;
                }
                case 1:
                {
                    long prod = 1;
                    for (int j = 0; j < operators_args_lens[operators_len]; ++j) {
                        prod *= operators_args[operators_len][j];
                    }
                    ret = prod;
                    break;
                }
                case 2:
                {
                    long min = LONG_MAX;
                    for (int j = 0; j < operators_args_lens[operators_len]; ++j) {
                        if (operators_args[operators_len][j] < min) {
                            min = operators_args[operators_len][j];
                        }
                    }
                    ret = min;
                    break;
                }
                case 3:
                {
                    long max = LONG_MIN;
                    for (int j = 0; j < operators_args_lens[operators_len]; ++j) {
                        if (operators_args[operators_len][j] > max) {
                            max = operators_args[operators_len][j];
                        }
                    }
                    ret = max;
                    break;
                }
                case 5:
                {
                    if (operators_args[operators_len][0] > operators_args[operators_len][1]) {
                        ret = 1;
                    } else {
                        ret = 0;
                    }
                    break;
                }
                case 6:
                {
                    if (operators_args[operators_len][0] < operators_args[operators_len][1]) {
                        ret = 1;
                    } else {
                        ret = 0;
                    }
                    break;
                }
                case 7:
                {
                    if (operators_args[operators_len][0] == operators_args[operators_len][1]) {
                        ret = 1;
                    } else {
                        ret = 0;
                    }
                    break;
                }
            }
            operators_args[operators_len - 1][operators_args_lens[operators_len - 1]++] = ret;
            // reset args lens
            operators_args_lens[operators_len] = 0;
        }
    }

    printf("%ld\n", operators_args[0][0]);

    return 0;
}
