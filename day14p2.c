#include <stdbool.h>
#include <stdio.h>

#define MAX_LEN 100000

bool is_sequential () {
    char alphabet[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 26; ++i) {
        if (alphabet[i] != 'A' + i) {
            return false;
        }
    }
    return true;
}

// char as int
unsigned char char_to_int (char c, bool sequential) {
    if (sequential) {
        return c - 'A';
    } else {
        char alphabet[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        for (int i = 0; i < 26; ++i) {
            if (c == alphabet[i]) {
                return i;
            }
        }
        return -1;
    }
}

void step (long long template[26][26], unsigned char rules[26][26], long long times[26]) {
    long long new_template[26][26];
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            new_template[i][j] = template[i][j];
        }
    }

    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            unsigned char rule = rules[i][j];
            if (rule != 123) {
                if (template[i][j] != 0) {
                    new_template[i][rule] += template[i][j];
                    new_template[rule][j] += template[i][j];
                    times[rule] += template[i][j];
                    new_template[i][j] -= template[i][j];
                }
            }
        }
    }

    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            template[i][j] = new_template[i][j];
        }
    }
}

int main () {
    bool sequential = is_sequential();

    FILE *stream = fopen("inputs/input14", "r");

    char template_str[MAX_LEN + 2];
    fscanf(stream, "%s", template_str);

    unsigned char rules[26][26] = { 0 };
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            rules[i][j] = 123;
        }
    }

    char pair[3];
    char ins[2];
    while (fscanf(stream, "%s -> %s", pair, ins) != EOF) {
        int ints[2] = { char_to_int(pair[0], sequential), char_to_int(pair[1], sequential) };
        rules[ints[0]][ints[1]] = char_to_int(ins[0], sequential);
    }

    long long template[26][26] = { 0 };
    long long times[26] = { 0 };
    for (int i = 0; template_str[i] != '\0'; ++i) {
        unsigned char char0 = char_to_int(template_str[i], sequential);
        if (template_str[i + 1] != '\0') {
            unsigned char char1 = char_to_int(template_str[i + 1], sequential);
            ++template[char0][char1];
        }
        ++times[char0];
    }

    for (int i = 0; i < 40; ++i) {
        step(template, rules, times);
    }

    int least = -1;
    int greatest = -1;
    for (int i = 0; i < 26; ++i) {
        if (times[i] == 0) {
            continue;
        }
        if (least == -1 || times[i] < times[least]) {
            least = i;
        }
        if (greatest == -1 || times[i] > times[greatest]) {
            greatest = i;
        }
    }
    printf("%lld\n", times[greatest] - times[least]);

    return 0;
}

