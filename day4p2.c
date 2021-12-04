#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Square {
    int r;
    int c;
};

// finds and marks all values of num in bingo, returns marked positions
int mark_all (int num, int board[5][5], bool board_marked[5][5], struct Square squares_marked[]) {
    int num_marked = 0;
    int counter = 0;
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            if (board[r][c] == num) {
                board_marked[r][c] = true;
                squares_marked[counter].r = r;
                squares_marked[counter++].c = c;
                ++num_marked;
            }
        }
    }
    return num_marked;
}

// checks for bingoes involving a square
bool has_bingo (struct Square square, bool board_marked[5][5]) {
    // horizontally
    bool all_set = true;
    for (int c = 0; c < 5; ++c) {
        if (!board_marked[square.r][c]) {
            all_set = false;
        }
    }
    if (all_set) {
        return true;
    }

    // vertically
    all_set = true;
    for (int r = 0; r < 5; ++r) {
        if (!board_marked[r][square.c]) {
            all_set = false;
        }
    }
    if (all_set) {
        return true;
    }

    return false;
}

int main () {
    FILE *stream = fopen("inputs/input4", "r");

    int bingo_nums[500];
    int bingo_nums_len = 0;
    int last;
    do {
        fscanf(stream, "%d", bingo_nums + (bingo_nums_len++));
    } while ((last = fgetc(stream)) == ',');
    ungetc(last, stream);


    int boards[500][5][5]; // board, row, column
    int boards_len = 0;

    while (true) {
        // rows
        for (int r = 0; r < 5; ++r) {
            // columns
            for (int c = 0; c < 5; ++c) {
                int num;
                if (fscanf(stream, "%d", &num) == EOF) {
                    goto bigloop_exit;
                }
                boards[boards_len][r][c] = num;
            }
        }
        ++boards_len;
    }
    bigloop_exit:

    bool boards_marked[500][5][5] = { 0 };
    bool boards_bingoed[500] = { 0 };
    int last_bingoed_idx = -1;
    int last_bingoed_idx_bingo_idx = -1;
    for (int i = 0; i < bingo_nums_len; ++i) {
        for (int j = 0; j < boards_len; ++j) {
            if (boards_bingoed[j]) {
                continue;
            }
            struct Square marked[25];
            int marked_len;
            if ((marked_len = mark_all(bingo_nums[i], boards[j], boards_marked[j], marked)) != 0) {
                for (int l = 0; l < marked_len; ++l) {
                    if (has_bingo(marked[l], boards_marked[j])) {
                        last_bingoed_idx = j;
                        last_bingoed_idx_bingo_idx = i;
                        boards_bingoed[j] = true;
                    }
                }
            }
        }
    }
    int sum = 0;
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            if (!boards_marked[last_bingoed_idx][r][c]) {
                sum += boards[last_bingoed_idx][r][c];
            }
        }
    }
    printf("%d\n", sum * bingo_nums[last_bingoed_idx_bingo_idx]);
    fclose(stream);
    return 0;
}
