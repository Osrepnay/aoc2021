#include <limits.h>
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

    int window[3] = {0, 0, 0};
    int window_sum = 0;
    int num_filled = 0;
    int last_window_sum = INT_MAX;

    int num;
    while (fscanf(stream, "%d", &num) != EOF) {
        window[0] = num;
        window_sum += window[0];

        if (++num_filled >= 3) {
            if (window_sum > last_window_sum) {
                ++increasing;
            }
            last_window_sum = window_sum;

            window_sum -= window[2];
        }
        shift_int_array_forward(window, 3);
    }
    printf("%d\n", increasing);
    fclose(stream);
    return 0;
}
