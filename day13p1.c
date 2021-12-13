#include <stdbool.h>
#include <stdio.h>

#define MAX_WIDTH 2000
#define MAX_HEIGHT 2000
#define MAX_FOLDS 50

void fold (int pos, bool orient, bool paper[MAX_HEIGHT][MAX_WIDTH], int *paper_height, int *paper_width) {
    if (orient) { // vertical, something like x=5
        for (int i = 0; i < *paper_height; ++i) {
            for (int j = 0; j < pos; ++j) {
                paper[i][j] |= paper[i][pos * 2 - j];
            }
        }
        *paper_width = pos;
    } else { // horizontal
        for (int i = 0; i < pos; ++i) {
            for (int j = 0; j < *paper_width; ++j) {
                paper[i][j] |= paper[pos * 2 - i][j];
            }
        }
        *paper_height = pos;
    }
}

int main () {
    FILE *stream = fopen("inputs/input13", "r");

    bool paper[MAX_HEIGHT][MAX_WIDTH] = { 0 };
    int paper_height = MAX_HEIGHT;
    int paper_width = MAX_WIDTH;

    int x;
    int y;
    while (fscanf(stream, "%d,%d", &x, &y) == 2) {
        fgetc(stream);
        paper[y][x] = true;
    }

    int folds[MAX_FOLDS];
    bool folds_orientation[MAX_FOLDS]; // true means vertical, false means horizontal
    int folds_len = 0;

    char direction;
    int num;
    while (fscanf(stream, "fold along %c=%d\n", &direction, &num) == 2) {
        folds[folds_len] = num;
        folds_orientation[folds_len] = direction == 'x';
        ++folds_len;
    }

    fold(folds[0], folds_orientation[0], paper, &paper_height, &paper_width);

    int visible = 0;
    for (int i = 0; i < paper_height; ++i) {
        for (int j = 0; j < paper_width; ++j) {
            visible += paper[i][j];
        }
    }
    printf("%d\n", visible);

    return 0;
}
