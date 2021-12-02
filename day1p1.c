#include <stdio.h>
#include <stdlib.h>

int main () {
    FILE* stream = fopen("inputs/input1", "r");
    int increasing = 0;
    int previous = 100000;
    int current;
    char curr_num[5];
    int curr_num_idx = 0;
    while ((current = getc(stream)) != EOF) {
        if (current == '\n') {
            curr_num[curr_num_idx] = '\0';
            printf("%s\n", curr_num);
            int num = atoi(curr_num);
            if (num > previous) {
                increasing++;
            }
            previous = num;
            curr_num_idx = 0;
        } else {
            curr_num[curr_num_idx++] = current;
        }
    }
    printf("%d\n", increasing);
    fclose(stream);
    return 0;
}
