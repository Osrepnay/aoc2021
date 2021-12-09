#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lowest_neighbor (int r, int c, int height, int width, int heightmap[500][500], int *lr, int *lc) {
    bool l_uninitialized = true;
    if (r >= 1) {
        if (l_uninitialized || heightmap[r - 1][c] < heightmap[*lr][*lc]) {
            *lr = r - 1;
            *lc = c;
            l_uninitialized = false;
        }
    }
    if (r + 1 < height) {
        if (l_uninitialized || heightmap[r + 1][c] < heightmap[*lr][*lc]) {
            *lr = r + 1;
            *lc = c;
            l_uninitialized = false;
        }
    }
    if (c >= 1) {
        if (l_uninitialized || heightmap[r][c - 1] < heightmap[*lr][*lc]) {
            *lr = r;
            *lc = c - 1;
            l_uninitialized = false;
        }
    }
    if (c + 1 < width) {
        if (l_uninitialized || heightmap[r][c + 1] < heightmap[*lr][*lc]) {
            *lr = r;
            *lc = c + 1;
            l_uninitialized = false;
        }
    }
}

void basin_size (int r, int c, int height, int width, int heightmap[500][500], bool flooded[500][500]) {
    if (flooded[r][c]) {
        return;
    }
    flooded[r][c] = true;
    int lr;
    int lc;
    if (r >= 1 && heightmap[r - 1][c] != 9) {
        lowest_neighbor(r - 1, c, height, width, heightmap, &lr, &lc);
        if (lr == r && lc == c) {
            basin_size(r - 1, c, height, width, heightmap, flooded);
        }
    }
    if (r + 1 < height && heightmap[r + 1][c] != 9) {
        lowest_neighbor(r + 1, c, height, width, heightmap, &lr, &lc);
        if (lr == r && lc == c) {
            basin_size(r + 1, c, height, width, heightmap, flooded);
        }
    }
    if (c >= 1 && heightmap[r][c - 1] != 9) {
        lowest_neighbor(r, c - 1, height, width, heightmap, &lr, &lc);
        if (lr == r && lc == c) {
            basin_size(r, c - 1, height, width, heightmap, flooded);
        }
    }
    if (c + 1 < width && heightmap[r][c + 1] != 9) {
        lowest_neighbor(r, c + 1, height, width, heightmap, &lr, &lc);
        if (lr == r && lc == c) {
            basin_size(r, c + 1, height, width, heightmap, flooded);
        }
    }
}

// opposite comp to sort in reverse; we need descending
int comp (const void *ap, const void *bp) {
    int a = *((int*) ap);
    int b = *((int*) bp);
    if (a > b) {
        return -1;
    } else if (a < b) {
        return 1;
    } else {
        return 0;
    }
}

int main () {
    FILE *stream = fopen("inputs/input9", "r");


    int heightmap[500][500];
    int width = 0;

    int height = 0;
    char line[501];
    while (fscanf(stream, "%s", line) != EOF) {
        if (height == 0) {
            width = strlen(line);
        }
        for (int i = 0; line[i] != '\0'; ++i) {
            heightmap[height][i] = line[i] - '0';
        }
        ++height;
    }

    int biggest[4] = { INT_MIN, INT_MIN, INT_MIN };
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            if ((r < 1 || heightmap[r][c] < heightmap[r - 1][c]) &&
                    (r + 1 >= height || heightmap[r][c] < heightmap[r + 1][c]) &&
                    (c < 1 || heightmap[r][c] < heightmap[r][c - 1]) &&
                    (c + 1 >= width || heightmap[r][c] < heightmap[r][c + 1])) {
                bool flooded[500][500] = { 0 };
                basin_size(r, c, height, width, heightmap, flooded);
                int size = 0;
                for (int i = 0; i < height; ++i) {
                    for (int j = 0; j < width; ++j) {
                        if (r == 0 && c == 1 && flooded[i][j]) {
                        }
                        size += flooded[i][j];
                    }
                }
                biggest[3] = size;
                qsort(biggest, 4, sizeof **heightmap, comp); // last element is ignored
            }
        }
    }

    printf("%d\n", biggest[0] * biggest[1] * biggest[2]);

    return 0;
}
