#include <stdbool.h>
#include <stdio.h>

#define MAX_WIDTH 1000
#define MAX_HEIGHT 1000

void copy (char seafloor[MAX_WIDTH][MAX_HEIGHT], char newfloor[MAX_WIDTH][MAX_HEIGHT], int width, int height) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (newfloor[x][y] != 0) {
                seafloor[x][y] = newfloor[x][y];
            }
        }
    }
}

int main () {
    FILE *stream = fopen("inputs/input25", "r");

    char seafloor[MAX_WIDTH][MAX_HEIGHT];
    int height;
    int width;

    int curr_width = 0;
    int curr_height = 0;

    int c;
    while ((c = fgetc(stream)) != EOF) {
        if (c == '\n') {
            width = curr_width;
            ++curr_height;
            curr_width = 0;
        } else {
            seafloor[curr_width++][curr_height] = c;
        }
    }
    height = curr_height;

    printf("%d x %d\n", height, width);
    int steps = 0;
    bool moved;
    do {
        moved = false;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // printf("%d ", seafloor[x][y]);
            }
            // puts("");
        }
        char newfloor[MAX_WIDTH][MAX_HEIGHT] = { 0 };
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                if (seafloor[x][y] == '>') {
                    int new_x = x + 1;
                    if (new_x >= width) {
                        new_x = 0;
                    }
                    if (seafloor[new_x][y] == '.') {
                        newfloor[x][y] = '.';
                        newfloor[new_x][y] = '>';
                        moved = true;
                    }
                }
            }
        }
        copy(seafloor, newfloor, width, height);
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                if (seafloor[x][y] == 'v') {
                    int new_y = y + 1;
                    if (new_y >= height) {
                        new_y = 0;
                    }
                    if (seafloor[x][new_y] == '.') {
                        printf("%d %d\n", x, y);
                        newfloor[x][y] = '.';
                        newfloor[x][new_y] = 'v';
                        moved = true;
                    }
                }
            }
        }
        copy(seafloor, newfloor, width, height);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                printf("%c", seafloor[x][y]);
            }
            puts("");
        }
        puts("");
        ++steps;
    } while (moved);

    printf("%d\n", steps);

    return 0;
}
