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

    int versions_total = 0;
    for (int i = 0; i < binary_len; ++i) {
        if (strtol(binary + i, NULL, 2) == 0) { // rest is just padding
            break;
        }
        int version = get_n(binary + i, 3, 2);
        i += 3;
        versions_total += version;
        int type_id = get_n(binary + i, 3, 2);
        i += 3;

        if(type_id == 4) { // literal
            int last_group_flag;
            do {
                last_group_flag = get_n(binary + i, 1, 2);
                i += 5;
            } while (last_group_flag != 0);
            --i;
        } else {
            int len_type_id = get_n(binary + i, 1, 2);
            if (len_type_id == 0) {
                i += 15;
            } else {
                i += 11;
            }
        }
    }

    printf("%d\n", versions_total);

    return 0;
}
