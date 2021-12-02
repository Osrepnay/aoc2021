#include <stdio.h>

int main () {
    FILE *stream = fopen("inputs/input2", "r");

    int horiz_pos = 0;
    int depth = 0;
    int aim = 0;

    char direction[8];
    int num;
    while (fscanf(stream, "%s %d", direction, &num) != EOF) {
        switch (direction[0]) {
            case 'f':
                horiz_pos += num;
                depth += aim * num;
                break;
            case 'd':
                aim += num;
                break;
            case 'u':
                aim -= num;
                break;
        }
    }
    printf("%d\n", horiz_pos * depth);
    fclose(stream);
    return 0;
}
