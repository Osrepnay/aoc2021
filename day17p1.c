#include <stdio.h>
#include <stdlib.h>

int main () {
    FILE *stream = fopen("inputs/input17", "r");
    int _x1, _x2, y1, y2;
    fscanf(stream, "target area: x=%d..%d, y=%d..%d", &_x1, &_x2, &y1, &y2);
    printf("%d\n", (abs(y1) * (abs(y1) - 1)) / 2);

    return 0;
}
