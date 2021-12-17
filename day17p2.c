#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool between (int a, int x, int b) {
    if (a <= x && x <= b) {
        return true;
    } else if (a >= x && x >= b) {
        return true;
    }
    return false;
}

int main () {
    FILE *stream = fopen("inputs/input17", "r");

    int x1, x2, y1, y2;
    fscanf(stream, "target area: x=%d..%d, y=%d..%d", &x1, &x2, &y1, &y2);

    // brute force, shouldn't be too bad though
    int num_works = 0;
    for (int i = 0; i <= x2; ++i) {
        for (int j = y1; j < abs(y1); ++j) {
            int curr_x = 0;
            int curr_y = 0;
            int change_x = i;
            int change_y = j;

            bool works = false;
            while (curr_x <= x2 && curr_y >= y1) {
                curr_y += change_y--;
                curr_x += change_x;
                if (change_x > 0) {
                    --change_x;
                }
                if (between(x1, curr_x, x2) && between(y1, curr_y, y2)) {
                    works = true;
                    break;
                }
            }
            if (works) {
                ++num_works;
            }
        }
    }
    printf("%d\n", num_works);

    return 0;
}
