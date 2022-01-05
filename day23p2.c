#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MOVES ((11 + 4 * 2) * (11 + 4 * 2))

typedef struct {
    bool in_hallway;
    int at; // serves as both room and hallway, depending on in_hallway
    int depth;
} Position;

typedef struct {
    Position from;
    Position to;
    int distance;
} Move;

bool outside_room (int hallway_pos) {
    return hallway_pos % 2 == 0 && hallway_pos != 0 && hallway_pos != 10;
}

int amphipod_room (char amphipod) {
    switch (amphipod) {
        case 'A':
            return 0;
        case 'B':
            return 1;
        case 'C':
            return 2;
        case 'D':
            return 3;
        default:
            return -1;
    }
}

int min (int a, int b) {
    return a < b ? a : b;
}

int max (int a, int b) {
    return a > b ? a : b;
}

int first_non_val (char val, char vals[], int vals_len) {
    for (int i = 0; i < vals_len; ++i) {
        if (vals[i] != val) {
            return i;
        }
    }
    return -1;
}

int get_moves (char rooms[4][4], char hallway[11], Move moves[MAX_MOVES]) {
    int moves_len = 0;

    // from room to hallway
    for (int r = 0; r < 4; ++r) {
        Position from = (Position) {
            false,
            r,
            first_non_val('.', rooms[r], 4)
        };
        if (from.depth == -1) {
            continue;
        }

        // if already in correct room and all positions below are of the correct type
        bool all_correct = true;
        for (int d = from.depth; d < 4; ++d) {
            if (amphipod_room(rooms[r][d]) != r) {
                all_correct = false;
                break;
            }
        }
        if (all_correct) {
            continue;
        }

        int distance = from.depth + 1;
        // left to right
        for (int h = r * 2 + 2 + 1; h < 11 && hallway[h] == '.'; ++h) {
            ++distance; // at beginning because we start counting right outside the room
            if (!outside_room(h)) {
                moves[moves_len++] = (Move) {
                    from,
                    (Position) {
                        true,
                        h,
                        -1
                    },
                    distance
                };
            }
        }
        distance = from.depth + 1;
        // right to left
        for (int h = r * 2 + 2 - 1; h >= 0 && hallway[h] == '.'; --h) {
            ++distance;
            if (!outside_room(h)) {
                moves[moves_len++] = (Move) {
                    from,
                    (Position) {
                        true,
                        h,
                        -1
                    },
                    distance
                };
            }
        }
    }

    // from hallway to room
    for (int h = 0; h < 11; ++h) {
        if (hallway[h] == '.') {
            continue;
        }
        Position from = (Position) {
            true,
            h,
            -1
        };

        int correct_room = amphipod_room(hallway[h]);
        // hallway position directly outside room
        int correct_room_outside = correct_room * 2 + 2;
        // check that there are no obstructions
        bool blocked = false;
        for (int ho = min(correct_room_outside, h + 1); ho < max(correct_room_outside, h); ++ho) {
            if (hallway[ho] != '.') {
                blocked = true;
                break;
            }
        }

        if (!blocked) {
            int first_filled = first_non_val('.', rooms[correct_room], 4);

            // skip if target room isn't all correctly filled
            if (first_filled != -1) {
                bool all_correct = true;
                for (int d = first_filled; d < 4; ++d) {
                    if (amphipod_room(rooms[correct_room][d]) != correct_room) {
                        all_correct = false;
                        break;
                    }
                }
                if (!all_correct) {
                    continue;
                }
            }

            // if available space
            if (first_filled != 0) {
                if (first_filled == -1) {
                    first_filled = 4;
                }
                moves[moves_len++] = (Move) {
                    from,
                    (Position) {
                        false,
                        correct_room,
                        first_filled - 1
                    },
                    abs(correct_room_outside - h) + first_filled
                };
            }
        }
    }
    return moves_len;
}

char get_pos (Position pos, char rooms[4][4], char hallway[11]) {
    if (pos.in_hallway) {
        return hallway[pos.at];
    } else {
        return rooms[pos.at][pos.depth];
    }
}

void set_pos (char val, Position pos, char rooms[4][4], char hallway[11]) {
    if (pos.in_hallway) {
        hallway[pos.at] = val;
    } else {
        rooms[pos.at][pos.depth] = val;
    }
}

int amphipod_energy (char amphipod) {
    switch (amphipod) {
        case 'A':
            return 1;
        case 'B':
            return 10;
        case 'C':
            return 100;
        case 'D':
            return 1000;
        default:
            return -1;
    }
}

bool done (char rooms[4][4]) {
    char amphipods[4] = { 'A', 'B', 'C', 'D' };
    for (int r = 0; r < 4; ++r) {
        for (int d = 0; d < 4; ++d) {
            if (rooms[r][d] != amphipods[r]) {
                return false;
            }
        }
    }
    return true;
}

int search (char rooms[4][4], char hallway[11]) {
    if (done(rooms)) {
        return 0;
    }
    Move moves[MAX_MOVES];
    int moves_len = get_moves(rooms, hallway, moves);

    int score = INT_MAX;
    for (int i = 0; i < moves_len; ++i) {
        char amphipod = get_pos(moves[i].from, rooms, hallway);

        set_pos(get_pos(moves[i].to, rooms, hallway), moves[i].from, rooms, hallway);
        set_pos(amphipod, moves[i].to, rooms, hallway);

        int move_score = search(rooms, hallway);
        if (move_score != INT_MAX) {
            move_score += amphipod_energy(amphipod) * moves[i].distance;
        }

        set_pos(get_pos(moves[i].from, rooms, hallway), moves[i].to, rooms, hallway);
        set_pos(amphipod, moves[i].from, rooms, hallway);
        if (move_score < score) {
            score = move_score;
        }
    }
    return score;
}

int main () {
    FILE *stream = fopen("inputs/input23", "r");

    fscanf(stream, "%*s");

    char hallway[11];
    char hallway_str[11 + 2 + 1];
    fscanf(stream, "%s", hallway_str);
    for (int i = 1; i < 12; ++i) {
        hallway[i - 1] = hallway_str[i];
    }

    char rooms[4][4]; // rooms[0][0] gets outer amphipod in leftmost room

    char room_rows[2][13 + 1];
    fscanf(stream, "%s\n%s", room_rows[0], room_rows[1]);
    char folded_rows[2][4] = {
        { 'D', 'C', 'B', 'A' },
        { 'D', 'B', 'A', 'C' }
    };

    for (int i = 0; i < 4; ++i) {
        rooms[i][0] = room_rows[0][i * 2 + 3];
    }
    for (int f = 0; f < 2; ++f) {
        for (int i = 0; i < 4; ++i) {
            rooms[i][f + 1] = folded_rows[f][i];
        }
    }
    for (int i = 0; i < 4; ++i) {
        rooms[i][3] = room_rows[1][i * 2 + 1];
    }

    printf("%d\n", search(rooms, hallway));

    return 0;
}
