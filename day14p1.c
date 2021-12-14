#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

int char_to_int (char c, bool sequential) {
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

char *step (char *template, char rules[26][26], bool sequential) {
    char *new_template = malloc(MAX_LEN * sizeof (char));
    int offset = 0;
    for (int i = 0; template[i + 1] != '\0'; ++i) {
        new_template[i + offset] = template[i];
        char ins = rules[char_to_int(template[i], sequential)][char_to_int(template[i + 1], sequential)];
        if (ins != 0) {
            new_template[i + (++offset)] = ins;
        }
        new_template[i + offset + 1] = template[i + 1];
    }
    return new_template;
}

int main () {
    bool sequential = is_sequential();

    FILE *stream = fopen("inputs/input14", "r");

    char *template = malloc(MAX_LEN * sizeof (char));
    fscanf(stream, "%s", template);

    char rules[26][26] = { 0 };

    char pair[3];
    char ins[2];
    while (fscanf(stream, "%s -> %s", pair, ins) != EOF) {
        int ints[2] = { char_to_int(pair[0], sequential), char_to_int(pair[1], sequential) };
        rules[ints[0]][ints[1]] = ins[0];
    }

    for (int i = 0; i < 10; ++i) {
        char *new_template = step(template, rules, sequential);
        free(template);
        template = new_template;
    }

    int times[26];
    for (int i = 0; i < 26; ++i) {
        times[i] = -1;
    }
    for (int i = 0; template[i] != '\0'; ++i) {
        ++times[char_to_int(template[i], sequential)];
    }

    int least = -1;
    int greatest = -1;
    for (int i = 0; i < 26; ++i) {
        if (times[i] == -1) {
            continue;
        }
        if (least == -1 || times[i] < times[least]) {
            least = i;
        }
        if (greatest == -1 || times[i] > times[greatest]) {
            greatest = i;
        }
    }
    printf("%d\n", times[greatest] - times[least]);

    return 0;
}

