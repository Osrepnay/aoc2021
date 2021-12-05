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

bool swap_if_unordered (int *a, int *b) {
    if (*a > *b) {
        int a_temp = *a;
        *a = *b;
        *b = a_temp;
        return true;
    }
    return false;
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
        if (is_vertical(lines[i])) {
            int start = lines[i].point1.y;
            int end = lines[i].point2.y;
            swap_if_unordered(&start, &end);
            for (int j = start; j <= end; ++j) {
                ++big_assarray[lines[i].point1.x][j];
            }
        } else if (is_horizontal(lines[i])) {
            int start = lines[i].point1.x;
            int end = lines[i].point2.x;
            swap_if_unordered(&start, &end);
            for (int j = start; j <= end; ++j) {
                ++big_assarray[j][lines[i].point1.y];
            }
        } else {
            int start = lines[i].point1.x; // lines are 45 deg, x is used as reference point
            int end = lines[i].point2.x;
            if (start > end) {
                for (int j = start; j >= end; --j) {
                    if (lines[i].point1.y > lines[i].point2.y) {
                        ++big_assarray[j][lines[i].point1.y - (start - j)];
                    } else {
                        ++big_assarray[j][lines[i].point1.y + (start - j)];
                    }
                }
            } else {
                for (int j = start; j <= end; ++j) {
                    if (lines[i].point1.y > lines[i].point2.y) {
                        ++big_assarray[j][lines[i].point1.y - (j - start)];
                    } else {
                        ++big_assarray[j][lines[i].point1.y + (j - start)];
                    }
                }
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
