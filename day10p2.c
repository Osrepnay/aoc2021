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

int comp (const void *ap, const void *bp) {
    long long a = *((long long*) ap);
    long long b = *((long long*) bp);
    if (a > b) {
        return 1;
    } else if (a < b) {
        return -1;
    } else {
        return 0;
    }
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

    long long completion_scores[1000];
    long long completion_scores_len = 0;
    for (int i = 0; i < lines_len; ++i) {
        char openings[500];
        int openings_len = 0;
        bool corrupted = false;
        for (int j = 0; lines[i][j] != '\0'; ++j) {
            if (lines[i][j] == '(' || lines[i][j] == '[' || lines[i][j] == '{' || lines[i][j] == '<') {
                openings[openings_len++] = lines[i][j];
            } else {
                if (!is_pair(openings[openings_len - 1], lines[i][j])) {
                    corrupted = true;
                    break;
                } else {
                    --openings_len;
                }
            }
        }

        if (!corrupted) {
            for (int j = openings_len - 1; j >= 0; --j) {
                completion_scores[completion_scores_len] *= 5;
                switch (openings[j]) {
                    case '(':
                        completion_scores[completion_scores_len] += 1;
                        break;
                    case '[':
                        completion_scores[completion_scores_len] += 2;
                        break;
                    case '{':
                        completion_scores[completion_scores_len] += 3;
                        break;
                    case '<':
                        completion_scores[completion_scores_len] += 4;
                        break;
                }
            }
            ++completion_scores_len;
        }
    }
    
    qsort(completion_scores, completion_scores_len, sizeof *completion_scores, comp);
    printf("%lld\n", completion_scores[completion_scores_len / 2]);

    return 0;
}
