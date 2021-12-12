#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cave {
    char *name;
    int connections[50];
    int connections_len;
};

int find_node_with_name (char *name, struct Cave nodes[], int len) {
    for (int i = 0; i < len; ++i) {
        if (strcmp(nodes[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

bool contains (int elem, int arr[], int arr_len) {
    for (int i = 0; i < arr_len; ++i) {
        if (elem == arr[i]) {
            return true;
        }
    }
    return false;
}

void num_paths (struct Cave nodes[], int nodes_len, int *paths, int start, int end, int went_to[], int went_to_len, int doubled) {
    for (int i = 0; i < nodes[start].connections_len; ++i) {
        int connection = nodes[start].connections[i];
        if (connection == end) {
            if (doubled < 0) { // if double isn't active
                ++(*paths);
                continue;
            } else { // doubled active, which means that it ended without doubling, which means this is the same thing as not using a double, which duplicates paths
                continue;
            }
        }
        if (strcmp(nodes[connection].name, "start") == 0) {
            continue;
        }
        if (islower(nodes[connection].name[0]) && contains(connection, went_to, went_to_len) && doubled != connection) {
            continue;
        }

        went_to[went_to_len] = connection;
        if (islower(nodes[connection].name[0]) && doubled == -1) {
            num_paths (nodes, nodes_len, paths, connection, end, went_to, went_to_len + 1, -1);
            num_paths (nodes, nodes_len, paths, connection, end, went_to, went_to_len + 1, connection);
        } else {
            if (doubled == connection) {
                num_paths (nodes, nodes_len, paths, connection, end, went_to, went_to_len + 1, -2);
            } else {
                num_paths (nodes, nodes_len, paths, connection, end, went_to, went_to_len + 1, doubled);
            }
        }
    }
}

int main () {
    FILE *stream = fopen("inputs/input12", "r");

    struct Cave nodes[50];
    int nodes_len = 0;

    char cave1[10];
    char cave2[10];
    while (fscanf(stream, "%[^-\n]-%s\n", cave1, cave2) != EOF) {
        int cave1_idx = find_node_with_name(cave1, nodes, nodes_len);
        int cave2_idx = find_node_with_name(cave2, nodes, nodes_len);
        if (cave1_idx == -1) {
            nodes[nodes_len].name = malloc(50 * sizeof (*cave1));
            strcpy(nodes[nodes_len].name, cave1);
            nodes[nodes_len].connections_len = 0;
            ++nodes_len;
        }
        if (cave2_idx == -1) {
            nodes[nodes_len].name = malloc(50 * sizeof (*cave2));
            strcpy(nodes[nodes_len].name, cave2);
            nodes[nodes_len].connections_len = 0;
            ++nodes_len;
        }
        cave1_idx = find_node_with_name(cave1, nodes, nodes_len);
        cave2_idx = find_node_with_name(cave2, nodes, nodes_len);
        nodes[cave1_idx].connections[nodes[cave1_idx].connections_len++] = cave2_idx;
        nodes[cave2_idx].connections[nodes[cave2_idx].connections_len++] = cave1_idx;
    }

    int start = find_node_with_name("start", nodes, nodes_len);
    int end = find_node_with_name("end", nodes, nodes_len);
    int paths = 0;
    int went_to[50];
    went_to[0] = start;
    num_paths (nodes, nodes_len, &paths, start, end, went_to, 1, -1);
    printf("%d\n", paths);

    return 0;
}
