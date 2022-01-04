#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_COMMANDS 500

typedef struct {
    int dims[3];
} Point;

typedef struct {
    Point min_max_corners[2];
} Cuboid;

typedef struct {
    Cuboid cuboid;
    bool on;
} CuboidCommand;

int min (int a, int b) {
    return a < b ? a : b;
}

int max (int a, int b) {
    return a > b ? a : b;
}

int comp (const void *ap, const void *bp) {
    int a = *((int *) ap);
    int b = *((int *) bp);
    if (a > b) {
        return 1;
    } else if (a < b) {
        return -1;
    } else {
        return 0;
    }
}

bool cuboid_intersection (Cuboid cuboid1, Cuboid cuboid2, Cuboid *intersection) {
    int all_dims[3][4];
    for (int d = 0; d < 3; ++d) {
        for (int c = 0; c < 2; ++c) {
            all_dims[d][c * 2] = cuboid1.min_max_corners[c].dims[d];
            all_dims[d][c * 2 + 1] = cuboid2.min_max_corners[c].dims[d];
        }
        // if not intersecting
        if (!(all_dims[d][3] >= all_dims[d][0] && all_dims[d][2] >= all_dims[d][1])) {
            return false;
        }
        qsort(all_dims[d], 4, sizeof (*all_dims[d]), comp);
    }

    Point intersect_dims_min;
    Point intersect_dims_max;
    for (int d = 0; d < 3; ++d) {
        intersect_dims_min.dims[d] = all_dims[d][1];
        intersect_dims_max.dims[d] = all_dims[d][2];
    }
    *intersection = (Cuboid) {
        { intersect_dims_min, intersect_dims_max }
    };
    return true;
}

long long cuboid_area (Cuboid cuboid) {
    long long area = 1;
    for (int d = 0; d < 3; ++d) {
        area *= (long long) abs(cuboid.min_max_corners[0].dims[d] - (cuboid.min_max_corners[1].dims[d] + 1));
    }
    return area;
}

int main () {
    FILE *stream = fopen("inputs/input22", "r");

    CuboidCommand cuboid_commands[MAX_COMMANDS];
    int cuboid_commands_len = 0;

    char type[4];
    int xs[3], ys[3], zs[3];
    while (fscanf(stream, "%s x=%d..%d,y=%d..%d,z=%d..%d", type, xs, xs + 1, ys, ys + 1, zs, zs + 1) != EOF) {
        bool valid = true;
        for (int i = 0; i < 3; ++i) {
            if (abs(xs[i]) > 50 || abs(ys[i]) > 50 || abs(zs[i]) > 50) {
                valid = false;
                break;
            }
        }
        if (valid) {
            cuboid_commands[cuboid_commands_len++] = (CuboidCommand) {
                (Cuboid) {
                    {
                        (Point) {
                            { min(xs[0], xs[1]), min(ys[0], ys[1]), min(zs[0], zs[1]) }
                        },
                        (Point) {
                            { max(xs[0], xs[1]), max(ys[0], ys[1]), max(zs[0], zs[1]) }
                        }
                    }
                },
                type[1] == 'n'
            };
        }
    }

    CuboidCommand simplified_commands[MAX_COMMANDS * 300]; // somewhat arbitrary size but eh
    int simplified_commands_len = 0;
    for (int i = 0; i < cuboid_commands_len; ++i) {
        CuboidCommand to_add[MAX_COMMANDS * 300];
        int to_add_len = 0;
        for (int j = 0; j < simplified_commands_len; ++j) {
            Cuboid intersection;
            if (cuboid_intersection(cuboid_commands[i].cuboid, simplified_commands[j].cuboid, &intersection)) {
                to_add[to_add_len++] = (CuboidCommand) {
                    intersection,
                    !simplified_commands[j].on,
                };
            }
        }
        if (cuboid_commands[i].on) {
            to_add[to_add_len++] = cuboid_commands[i];
        }
        for (int j = 0; j < to_add_len; ++j) {
            simplified_commands[simplified_commands_len++] = to_add[j];
        }
    }

    long long total_area = 0;
    for (int i = 0; i < simplified_commands_len; ++i) {
        long long area = cuboid_area(simplified_commands[i].cuboid);
        if (simplified_commands[i].on) {
            total_area += area;
        } else {
            total_area -= area;
        }
    }
    printf("%lld\n", total_area);

    return 0;
}
