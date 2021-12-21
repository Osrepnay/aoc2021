#include <stdbool.h>
#include <stdio.h>

const int ROLLS[10] = { 3, 4, 5, 5, 6, 7, 6, 7, 8, 9 };
const int MULTIPLIERS[10] = { 1, 3, 3, 3, 6, 3, 1, 3, 3, 1 };

void do_turns (int player1, int player1_score, int player2, int player2_score, int roll, bool on_player1, long long *player1_wins, long long *player2_wins, long long multiplier) {
    if (*player2_wins < 0) {
        printf("oh no\n");
    }
    if (on_player1) {
        player1 = (player1 + roll - 1) % 10 + 1;
        player1_score += player1;
        if (player1_score >= 21) {
            *player1_wins += multiplier;
            return;
        }
    } else {
        player2 = (player2 + roll - 1) % 10 + 1;
        player2_score += player2;
        if (player2_score >= 21) {
            *player2_wins += multiplier;
            return;
        }
    }

    for (int i = 0; i < 10; ++i) {
        do_turns(player1, player1_score, player2, player2_score, ROLLS[i], !on_player1, player1_wins, player2_wins, multiplier * MULTIPLIERS[i]);
    }
}

int main () {
    FILE *stream = fopen("inputs/input21", "r");

    int player1, player2;
    fscanf(stream, "Player 1 starting position: %d\n", &player1);
    fscanf(stream, "Player 2 starting position: %d", &player2);
    int player1_score = 0;
    int player2_score = 0;

    long long player1_wins = 0;
    long long player2_wins = 0;

    for (int i = 0; i < 10; ++i) {
        do_turns(player1, player1_score, player2, player2_score, ROLLS[i], true, &player1_wins, &player2_wins, MULTIPLIERS[i]);
    }

    if (player1_wins > player2_wins) {
        printf("%lld\n", player1_wins);
    } else {
        printf("%lld\n", player2_wins);
    }

    return 0;
}
