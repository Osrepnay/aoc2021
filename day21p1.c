#include <stdbool.h>
#include <stdio.h>

int roll_die (int *die, int *die_count) {
    *die_count += 3;
    int score = 0;
    for (int i = 0; i < 3; ++i) {
        if (*die > 100) {
            *die = 1;
        }
        score += *die;
        ++*die;
    }
    return score;
}

int main () {
    FILE *stream = fopen("inputs/input21", "r");

    int player1, player2;
    fscanf(stream, "Player 1 starting position: %d\n", &player1);
    fscanf(stream, "Player 2 starting position: %d", &player2);

    int die = 1;
    int die_count = 0;

    int player1_score = 0;
    int player2_score = 0;

    bool player1_won;
    while (true) {
        int player1_move = (roll_die(&die, &die_count) - 1) % 10 + 1;
        player1 = (player1 + player1_move - 1) % 10 + 1; // + and - so modulo doesn't cut off 10
        player1_score += player1;
        if (player1_score >= 1000) {
            player1_won = true;
            break;
        }

        int player2_move = (roll_die(&die, &die_count) - 1) % 10 + 1;
        player2 = (player2 + player2_move - 1) % 10 + 1;
        player2_score += player2;
        if (player2_score >= 1000) {
            player1_won = false;
            break;
        }
    }

    if (player1_won) {
        printf("%d\n", player2_score * die_count);
    } else {
        printf("%d\n", player1_score * die_count);
    }

    return 0;
}
