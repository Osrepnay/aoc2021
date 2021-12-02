#include <stdio.h>
#include <stdlib.h>

// shifts whole array forward one index, setting last element as head
void shift_int_array_forward (int arr[], int len) {
    int last_elem = arr[len - 1];
    int last = arr[0]; // last element for loop came across
    for (int i = 1; i < len; ++i) {
        int temp = arr[i];
        arr[i] = last;
        last = temp;
    }
    arr[0] = last_elem;
}

int main () {
    FILE* stream = fopen("inputs/input1", "r");

    int increasing = 0;

    int curr_window[3] = {0, 0, 0};
    int curr_window_sum = 0;
    int num_filled = 0;
    int last_window_sum = 100000;

    char curr_num[5];
    int curr_num_idx = 0;

    int current;
    while ((current = getc(stream)) != EOF) {
        if (current == '\n') {
            curr_num[curr_num_idx] = '\0';
            int num = atoi(curr_num);
            curr_num_idx = 0;

            curr_window[0] = num;
            curr_window_sum += curr_window[0];

            if (++num_filled >= 3) {
                if (curr_window_sum > last_window_sum) {
                    ++increasing;
                }
                last_window_sum = curr_window_sum;

                curr_window_sum -= curr_window[2];
            }
            shift_int_array_forward(curr_window, 3);
        } else {
            curr_num[curr_num_idx++] = current;
        }
    }
    printf("%d\n", increasing);
    fclose(stream);
    return 0;
}
