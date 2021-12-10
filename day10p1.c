#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_pair (char open, char close) {
    switch (open) {
        case '(':
            return close == ')';
        case '[':
            return close == ']';
        case '{':
            return close == '}';
        case '<':
            return close == '>';
    }
    return false;
}

int main () {
    FILE *stream = fopen("inputs/input10", "r");

    char *lines[1000];
    int lines_len = 0;

    char line[500];
    while (fscanf(stream, "%s", line) != EOF) {
        lines[lines_len] = malloc(500 * sizeof *line);
        strcpy(lines[lines_len++], line);
    }

    int corrupted_score = 0;
    for (int i = 0; i < lines_len; ++i) {
        char openings[500];
        int openings_len = 0;
        for (int j = 0; lines[i][j] != '\0'; ++j) {
            if (lines[i][j] == '(' || lines[i][j] == '[' || lines[i][j] == '{' || lines[i][j] == '<') {
                openings[openings_len++] = lines[i][j];
            } else {
                if (!is_pair(openings[openings_len - 1], lines[i][j])) {
                    switch (lines[i][j]) {
                        case ')':
                            corrupted_score += 3;
                            break;
                        case ']':
                            corrupted_score += 57;
                            break;
                        case '}':
                            corrupted_score += 1197;
                            break;
                        case '>':
                            corrupted_score += 25137;
                            break;
                    }
                    break;
                } else {
                    --openings_len;
                }
            }
        }
    }

    printf("%d\n", corrupted_score);

    return 0;
}
