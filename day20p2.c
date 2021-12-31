#include <stdbool.h>
#include <stdio.h>

#define MAX_WIDTH 500
#define MAX_HEIGHT 500
#define OFFSET 50

bool get_board (
        int x, int y,
        bool board[MAX_WIDTH + OFFSET * 2][MAX_HEIGHT + OFFSET * 2],
        int width, int height,
        bool rest, int offset_used) {
    if (x < -offset_used || y < -offset_used || x >= width + offset_used || y >= height + offset_used) {
        return rest;
    } else {
        return board[x + OFFSET][y + OFFSET];
    }
}

int main () {
    FILE *stream = fopen("inputs/input20", "r");

    char algo_str[512 + 1];
    fscanf(stream, "%s", algo_str);
    bool algo[512];
    for (int i = 0; i < 512; ++i) {
        algo[i] = algo_str[i] == '#';
    }

    int offset_used = 0;
    bool rest = false;
    bool board[MAX_WIDTH + OFFSET * 2][MAX_HEIGHT + OFFSET * 2] = { 0 };

    int width;
    int height = 0;
    char curr_line[MAX_WIDTH + 1];
    while (fscanf(stream, "%s", curr_line) != EOF) {
        for (int i = 0; curr_line[i] != '\0'; ++i) {
            board[i + OFFSET][height + OFFSET] = curr_line[i] == '#';
            width = i + 1;
        }
        ++height;
    }

    for (int t = 0; t < 50; ++t) {
        bool new_board[MAX_WIDTH + OFFSET * 2][MAX_HEIGHT + OFFSET * 2] = { 0 };
        for (int x = -offset_used - 1; x < width + offset_used + 1; ++x) {
            for (int y = -offset_used - 1; y < height + offset_used + 1; ++y) {
                int idx = 0;
                for (int sy = -1; sy <= 1; ++sy) {
                    for (int sx = -1; sx <= 1; ++sx) {
                        idx |= get_board(x + sx, y + sy, board, width, height, rest, offset_used)
                            << (8 - ((sy + 1) * 3 + (sx + 1)));
                    }
                }
                new_board[x + OFFSET][y + OFFSET] = algo[idx];
            }
        }
        for (int i = 0; i < MAX_WIDTH + OFFSET * 2; ++i) {
            for (int j = 0; j < MAX_HEIGHT + OFFSET * 2; ++j) {
                board[i][j] = new_board[i][j];
            }
        }
        if (rest) {
            rest = algo[512 - 1];
        } else {
            rest = algo[0];
        }
        ++offset_used;
    }

    int num_lit = 0;
    for (int i = 0; i < MAX_WIDTH + OFFSET * 2; ++i) {
        for (int j = 0; j < MAX_HEIGHT + OFFSET * 2; ++j) {
            num_lit += board[i][j];
        }
    }
    printf("%d\n", num_lit);

    return 0;
}
