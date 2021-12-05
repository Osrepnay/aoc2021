#include <stdbool.h>
#include <stdio.h>

struct Point {
    int x;
    int y;
};

struct Line {
    struct Point point1;
    struct Point point2;
};

bool is_vertical (struct Line line) {
    return line.point1.x == line.point2.x;
}

bool is_horizontal (struct Line line) {
    return line.point1.y == line.point2.y;
}

void gen_points (struct Line line, int *smallest, int *largest) {
    if (is_vertical(line)) {
        *smallest = line.point1.y;
        *largest = line.point2.y;
        if (line.point1.y > line.point2.y) {
            *smallest = line.point2.y;
            *largest = line.point1.y;
        }
    } else {
        *smallest = line.point1.x;
        *largest = line.point2.x;
        if (line.point1.x > line.point2.x) {
            *smallest = line.point2.x;
            *largest = line.point1.x;
        }
    }
}

int main () {
    FILE *stream = fopen("inputs/input5", "r");

    struct Line lines[5000];
    int lines_len = 0;

    while (fscanf(stream, "%d,%d -> %d,%d",
                &lines[lines_len].point1.x,
                &lines[lines_len].point1.y,
                &lines[lines_len].point2.x,
                &lines[lines_len].point2.y) != EOF) { ++lines_len; }

    int big_assarray[1000][1000] = { 0 };
    for (int i = 0; i < lines_len; ++i) {
        if (!is_vertical(lines[i]) && !is_horizontal(lines[i])) { continue; }
        int start = -1;
        int end = -1;
        gen_points(lines[i], &start, &end);
        for (int j = start; j <= end; ++j) {
            if (is_vertical(lines[i])) {
                ++big_assarray[lines[i].point1.x][j];
            } else {
                ++big_assarray[j][lines[i].point1.y];
            }
        }
    }
    int num_intersections = 0;
    for (int x = 0; x < 1000; ++x) {
        for (int y = 0; y < 1000; ++y) {
            if (big_assarray[x][y] > 1) {
                ++num_intersections;
            }
        }
    }
    printf("%d\n", num_intersections);

    fclose(stream);
    return 0;
}
