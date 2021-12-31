#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SCANNERS 50
#define MAX_SCANNERS_POINTS 50

typedef struct {
    int dims[3];
} Point;

Point rotate (Point point, int new_orientation[6]) {
    Point new_point;
    new_point.dims[0] = new_orientation[3 + 0] * point.dims[new_orientation[0] - 1];
    new_point.dims[1] = new_orientation[3 + 1] * point.dims[new_orientation[1] - 1];
    new_point.dims[2] = new_orientation[3 + 2] * point.dims[new_orientation[2] - 1];
    return new_point;
}

bool points_eq (Point p1, Point p2) {
    return p1.dims[0] == p2.dims[0] && p1.dims[1] == p2.dims[1] && p1.dims[2] == p2.dims[2];
}

bool contains (int elem, int elems[], int elems_len) {
    for (int i = 0; i < elems_len; ++i) {
        if (elems[i] == elem) {
            return true;
        }
    }
    return false;
}

int dims_to_int (int dims[3]) {
    return 100 * dims[0] + 10 * dims[1] + dims[2];
}

int num_intersections (Point p1s[], int p1s_len, Point p2s[], int p2s_len) {
    int intersections_len = 0;
    for (int i = 0; i < p1s_len; ++i) {
        for (int j = 0; j < p2s_len; ++j) {
            if (points_eq(p1s[i], p2s[j])) {
                ++intersections_len;
                break;
            }
        }
    }
    return intersections_len;
}

int manhattan (Point p1, Point p2) {
    return abs(p1.dims[0] - p2.dims[0]) + abs(p1.dims[1] - p2.dims[1]) + abs(p1.dims[2] - p2.dims[2]);
}

int main () {
    FILE *stream = fopen("inputs/input19", "r");

    Point beacons[MAX_SCANNERS][MAX_SCANNERS_POINTS];
    int beacons_lens[MAX_SCANNERS];
    int scanners_len = 0;

    int curr_scanner;
    while (fscanf(stream, "%*s %*s %d %*s", &curr_scanner) == 1) {
        beacons_lens[scanners_len] = 0;
        while (
                fscanf(
                    stream,
                    "%d,%d,%d",
                    beacons[scanners_len][beacons_lens[scanners_len]].dims,
                    beacons[scanners_len][beacons_lens[scanners_len]].dims + 1,
                    beacons[scanners_len][beacons_lens[scanners_len]].dims + 2
                ) == 3) {
            ++beacons_lens[scanners_len];
        }
        ++scanners_len;
    }

    // last 3 in every orientation are the multiplier for the first 3
    int orientations_onedir[8][6] = {
        { 1, 2, 3, 1,  1,  1 },
        { 1, 3, 2, 1,  1, -1 },
        { 1, 2, 3, 1, -1, -1 },
        { 1, 3, 2, 1, -1,  1 },
        { 1, 2, 3, -1,  1, -1 },
        { 1, 3, 2, -1, -1, -1 },
        { 1, 2, 3, -1, -1,  1 },
        { 1, 3, 2, -1,  1,  1 }
    };
    int orientations[24][6];
    int orientations_len = 0;
    for (int o = 0; o < 8; ++o) {
        for (int a = 0; a < 3; ++a) {
            for (int i = 0; i < 3; ++i) {
                int new_orientation = orientations_onedir[o][i] + a;
                if (new_orientation > 3) {
                    new_orientation -= 3;
                }
                orientations[orientations_len][i] = new_orientation;
            }
            for (int i = 3; i < 6; ++i) {
                orientations[orientations_len][i] = orientations_onedir[o][i];
            }
            ++orientations_len;
        }
    }

    int scanners_found[MAX_SCANNERS];
    Point centers[MAX_SCANNERS]; // same length as scanners_found
    int scanners_found_len = 1;

continue_search:
    while (scanners_found_len < scanners_len) {
        for (int s = 0; s < scanners_len; ++s) {
            if (contains(s, scanners_found, scanners_found_len)) {
                continue;
            }
            for (int sfr = 0; sfr < scanners_found_len; ++sfr) {
                int sf = scanners_found[sfr];

                for (int o = 0; o < 24; ++o) {
                    for (int sb = 0; sb < beacons_lens[s]; ++sb) {
                        Point point = rotate(beacons[s][sb], orientations[o]);
                        for (int sfb = 0; sfb < beacons_lens[sf]; ++sfb) {
                            Point point_found = beacons[sf][sfb];

                            Point point_center;
                            for (int i = 0; i < 3; ++i) {
                                point_center.dims[i] = point_found.dims[i] - point.dims[i];
                            }
                            Point new_beacons[MAX_SCANNERS_POINTS];
                            for (int i = 0; i < beacons_lens[s]; ++i) {
                                new_beacons[i] = rotate(beacons[s][i], orientations[o]);
                                for (int j = 0; j < 3; ++j) {
                                    new_beacons[i].dims[j] += point_center.dims[j];
                                }
                            }
                            if (num_intersections(new_beacons, beacons_lens[s], beacons[sf], beacons_lens[sf]) >= 12) {
                                for (int i = 0; i < beacons_lens[s]; ++i) {
                                    for (int j = 0; j < beacons_lens[sf]; ++j) {
                                        if (points_eq(new_beacons[i], beacons[sf][j])) {
                                            break;
                                        }
                                    }
                                }
                                for (int i = 0; i < beacons_lens[s]; ++i) {
                                    beacons[s][i] = new_beacons[i];
                                }
                                centers[scanners_found_len] = point_center;
                                scanners_found[scanners_found_len++] = s;
                                goto continue_search;
                            }
                        }
                    }
                }
            }
        }
    }

    int max_dist = INT_MIN;
    for (int i = 0; i < scanners_found_len - 1; ++i) {
        for (int j = i + 1; j < scanners_found_len; ++j) {
            int dist = manhattan(centers[i], centers[j]);
            if (dist > max_dist) {
                max_dist = dist;
            }
        }
    }
    printf("%d\n", max_dist);

    return 0;
}
