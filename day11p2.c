#include <stdbool.h>
#include <stdio.h>

bool in_bounds (int x, int y) {
    return x >= 0 && y >= 0 && x < 10 && y < 10;
}

int main () {
    FILE *stream = fopen("inputs/input11", "r");

    int octipi[10][10];

    int curr_width = 0;
    int curr_height = 0;
    char c;
    while ((c = fgetc(stream)) != EOF) {
        if (c != '\n') {
            octipi[curr_height][curr_width++] = c - '0';
        } else {
            curr_width = 0;
            ++curr_height;
        }
    }

    int step;
    for (int s = 1; ; ++s) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                ++octipi[i][j];
            }
        }

        int flashes = 0;
        bool flashed[10][10] = { 0 }; // here because octipi can't flash twice in a single turn
        bool has_flashed;
        do {
            has_flashed = false;
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                    if (octipi[i][j] > 9) {
                        ++flashes;
                        has_flashed = true;
                        octipi[i][j] = 0;
                        flashed[i][j] = true;
                        for (int dx = -1; dx <= 1; ++dx) {
                            for (int dy = -1; dy <= 1; ++dy) {
                                if (!(dx == 0 && dy == 0) && in_bounds(i + dx, j + dy) && !flashed[i + dx][j + dy]) {
                                    ++octipi[i + dx][j + dy];
                                }
                            }
                        }
                    }
                }
            }
        } while (has_flashed);

        if (flashes >= 100) {
            step = s;
            break;
        }
    }
    printf("%d\n", step);
}
