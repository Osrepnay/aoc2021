#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INS 300

struct Prog {
    char instructions[MAX_INS][3 + 1];
    char instructions_inputs[MAX_INS][2][5];
    int instructions_len;
};

int push (int val, int z[14], int z_len) {
    z[z_len++] = val;
    return z_len;
}

int pop (int _z[14], int z_len) {
    (void)_z;
    return --z_len;
}

int peek (int z[14], int z_len) {
    if (z_len >= 1) {
        return z[z_len - 1];
    } else {
        // technically inaccurate for a stack, but it's not even a real stack anyway so
        return 0;
    }
}

int main () {
    FILE *stream = fopen("inputs/input24", "r");

    struct Prog prog;
    prog.instructions_len = 0;
    while (fscanf(stream, "%s", prog.instructions[prog.instructions_len++]) != EOF) {
        fscanf(stream, "%s", prog.instructions_inputs[prog.instructions_len - 1][0]);
        if (strcmp(prog.instructions[prog.instructions_len - 1], "inp") != 0) {
            fscanf(stream, "%s", prog.instructions_inputs[prog.instructions_len - 1][1]);
        }
    }

    int as[14];
    bool pops[14];
    int cs[14];

    for (int i = 0; i < 14; ++i) {
        as[i] = atoi(prog.instructions_inputs[i * 18 + 5][1]);
        pops[i] = atoi(prog.instructions_inputs[i * 18 + 4][1]) == 26;
        cs[i] = atoi(prog.instructions_inputs[i * 18 + 15][1]);
    }

    int z[14];
    int z_from[14]; // index z was added
    int z_len = 0;
    int ws[14];
    for (int i = 0; i < 14; ++i) {
        int w = -1;

        int x_p = peek(z, z_len) + as[i];
        if (x_p > 9) {
            int from = peek(z_from, z_len);

            if (as[i] < 9 && 9 - as[i] > cs[from]) {
                ws[from] = 9 - as[i] - cs[from];
                z[z_len - 1] = ws[from] + cs[from];

                // retry
                --i;
                continue;
            } else {
                w = 9;
            }
        } else if (x_p < 1) {
            w = 9;
        } else {
            w = x_p;
        }

        int x = x_p != w;
        if (pops[i]) {
            z_len = pop(z, z_len);
        }
        if (x) {
            z_from[z_len] = i;
            z_len = push(cs[i] + w, z, z_len);
        }
        ws[i] = w;
    }
    for (int i = 0; i < 14; ++i) {
        printf("%d", ws[i]);
    }
    puts("");

    return 0;
}
