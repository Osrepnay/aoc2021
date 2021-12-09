#include <stdio.h>
#include <string.h>

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

    int risk_sum = 0;
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            if ((r < 1 || heightmap[r][c] < heightmap[r - 1][c]) &&
                    (r + 1 >= height || heightmap[r][c] < heightmap[r + 1][c]) &&
                    (c < 1 || heightmap[r][c] < heightmap[r][c - 1]) &&
                    (c + 1 >= width || heightmap[r][c] < heightmap[r][c + 1])) {
                risk_sum += heightmap[r][c] + 1;
            }
        }
    }

    printf("%d\n", risk_sum);

    return 0;
}
