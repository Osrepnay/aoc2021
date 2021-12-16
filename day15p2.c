#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_HEIGHT 500 // actual max height unlike part 1

struct Point {
    int x;
    int y;
};

bool in_bounds (struct Point point, int size) {
    return point.x >= 0 && point.y >= 0 && point.x < (size * 5) && point.y < (size * 5);
}

int get_map (int x, int y, int map[MAX_HEIGHT][MAX_HEIGHT], int size) {
    if (x < size && y < size) {
        return map[x][y];
    } else {
        int factor_x = 0;
        int factor_y = 0;
        for (; x >= size * factor_x; ++factor_x);
        for (; y >= size * factor_y; ++factor_y);
        // factor x and y are 1 bigger than they should be
        --factor_x;
        --factor_y;
        int ret = map[x - size * factor_x][y - size * factor_y] + factor_x + factor_y;
        return ret > 9 ? ret - 9 : ret;
    }
}

struct LLNode {
    struct Point val;
    struct LLNode *next;
};

bool contains (struct Point point, struct LLNode *points) {
    for (; points != NULL; points = points->next) {
        if (points->val.x == point.x && points->val.y == point.y) {
            return true;
        }
    }
    return false;
}

int search (struct Point start, struct Point pos, int map[MAX_HEIGHT][MAX_HEIGHT], int size) {
    int changes[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    bool visited[MAX_HEIGHT][MAX_HEIGHT] = { 0 };
    visited[start.x][start.y] = true;

    struct LLNode *boundary = malloc(sizeof (struct LLNode));
    boundary->val = (struct Point) { 0, 0 };
    boundary->next = NULL;

    int distances[MAX_HEIGHT][MAX_HEIGHT] = { 0 };

    while (start.x != pos.x || start.y != pos.y) {
        for (int i = 0; i < 4; ++i) {
            int *curr_changes = changes[i];
            int new_x = start.x + curr_changes[0];
            int new_y = start.y + curr_changes[1];
            struct Point new_point = { new_x, new_y };
            if ((new_x == start.x && new_y == start.y) || !in_bounds(new_point, size) || visited[new_x][new_y]) {
                continue;
            }

            int new_dist = distances[start.x][start.y] + get_map(new_x, new_y, map, size);
            if (distances[new_x][new_y] == 0 || new_dist < distances[new_x][new_y]) {
                distances[new_x][new_y] = new_dist;
                if (!contains(new_point, boundary)) {
                    struct LLNode *new_boundary = malloc(sizeof (struct LLNode));
                    new_boundary->val = new_point;
                    new_boundary->next = boundary;
                    boundary = new_boundary;
                }
            }
        }
        visited[start.x][start.y] = true;

        struct Point min = (struct Point) {-1, -1};
        struct LLNode *curr_point = boundary;
        struct LLNode *prev = NULL;
        do {
            if (curr_point->val.x == start.x && curr_point->val.y == start.y) {
                if (prev == NULL) {
                    boundary = curr_point->next;
                } else {
                    prev->next = curr_point->next;
                }
                continue;
            }
            prev = curr_point;
            if (min.x == -1 || distances[curr_point->val.x][curr_point->val.y] < distances[min.x][min.y]) {
                min = curr_point->val;
            }
        } while ((curr_point = curr_point->next) != NULL);
        start = min;
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

    printf("%d\n", search((struct Point) {0, 0}, (struct Point) {size * 5 - 1, size * 5 - 1}, map, size));

    return 0;
}
