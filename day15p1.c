#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_HEIGHT 500

struct Point {
    int x;
    int y;
};

bool in_bounds (struct Point point, int size) {
    return point.x >= 0 && point.y >= 0 && point.x < size && point.y < size;
}

bool contains (struct Point point, struct Point points[], int points_len) {
    for (int i = 0; i < points_len; ++i) {
        if (points[i].x == point.x && points[i].y == point.y) {
            return true;
        }
    }
    return false;
}

int search (struct Point start, struct Point pos, int map[MAX_HEIGHT][MAX_HEIGHT], int size) {
    int changes[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    bool visited[MAX_HEIGHT][MAX_HEIGHT] = { 0 };
    visited[start.x][start.y] = true;
    struct Point to_visit[MAX_HEIGHT * MAX_HEIGHT]; // nodes at current depth
    to_visit[0] = start;
    int to_visit_len = 1;
    struct Point found[MAX_HEIGHT * MAX_HEIGHT]; // found nodes yet to be explored
    int found_len = 0;

    int distances[MAX_HEIGHT][MAX_HEIGHT] = { 0 };

    while (true) {
        for (int i = 0; i < to_visit_len; ++i) {
            visited[to_visit[i].x][to_visit[i].y] = true;
            for (int j = 0; j < 4; ++j) {
                int *curr_changes = changes[j];
                int new_x = to_visit[i].x + curr_changes[0];
                int new_y = to_visit[i].y + curr_changes[1];
                struct Point new_point = { new_x, new_y };
                if ((new_x == start.x && new_y == start.y) || !in_bounds(new_point, size)) {
                    continue;
                }

                int new_dist = distances[to_visit[i].x][to_visit[i].y] + map[new_x][new_y];
                if (distances[new_x][new_y] == 0 || new_dist < distances[new_x][new_y]) {
                    distances[new_x][new_y] = new_dist;
                }
                if (!visited[new_x][new_y] && !contains(new_point, found, found_len)) {
                    found[found_len++] = new_point;
                }
            }
        }
        if (found_len == 0) {
            break;
        }
        for (int i = 0; i < found_len; ++i) {
            to_visit[i] = found[i];
        }
        to_visit_len = found_len;
        found_len = 0;
    }
    return distances[pos.x][pos.y];
}

int main() {
    FILE *stream = fopen("inputs/input15", "r");

    int map[MAX_HEIGHT][MAX_HEIGHT];
    int curr_x = 0;
    int curr_y = 0;
    int size = 0;

    char c;
    while ((c = fgetc(stream)) != EOF) {
        if (c == '\n') {
            ++curr_y;
            size = curr_x;
            curr_x = 0;
        } else {
            map[curr_x++][curr_y] = c - '0';
        }
    }

    printf("%d\n", search((struct Point) {0, 0}, (struct Point) {size - 1, size - 1}, map, size));

    return 0;
}
