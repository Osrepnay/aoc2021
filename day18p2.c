#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_LINES 1000
#define MAX_PER_LINE 100

// either value or comma
typedef struct {
    bool is_comma;
    int val;
    bool on_left;
    int depth;
} PairElem;

int replace_with_pair (int left, int right, int idx, PairElem pairs[], int pairs_len) {
    PairElem at = pairs[idx];
    for (int i = pairs_len + 2 - 1; i > idx + 2; --i) {
        pairs[i] = pairs[i - 2];
    }
    pairs[idx] = (PairElem) { .is_comma = false, .val = left, .on_left = true, .depth = at.depth + 1 };
    pairs[idx + 1] = (PairElem) { .is_comma = true, .depth = at.depth + 1};
    pairs[idx + 2] = (PairElem) { .is_comma = false, .val = right, .on_left = false, .depth = at.depth + 1 };
    return pairs_len + 2;
}

int replace_with_val (int val, int pair_start_idx, PairElem pairs[], int pairs_len) {
    bool past_pair = false;
    for (int i = 0; i < pairs_len - 2; ++i) {
        if (i == pair_start_idx) {
            bool on_left = false;
            if (i == 0 || (i + 3 < pairs_len && pairs[i + 3].is_comma && pairs[i + 3].depth + 1 == pairs[i].depth)) {
                on_left = true;
            }
            pairs[i] = (PairElem) { .is_comma = false, .val = val, .on_left = on_left, .depth = pairs[i].depth - 1 };
            past_pair = true;
            ++i;
        }
        if (past_pair) {
            pairs[i] = pairs[i + 2];
        }
    }
    return pairs_len - 2;
}

bool explode (PairElem pairs[], int *pairs_len) {
    for (int i = 0; i < *pairs_len - 2; ++i) {
        if (pairs[i + 1].is_comma &&
                pairs[i].depth == 5 &&
                pairs[i + 2].depth == 5 &&
                pairs[i].on_left == true &&
                pairs[i + 2].on_left == false) {
            if (i > 0) {
                int j = i - 1;
                for (; pairs[j].is_comma; --j);
                pairs[j].val += pairs[i].val;
            }
            if (i + 2 < *pairs_len) {
                int j = i + 3;
                for (; pairs[j].is_comma; ++j);
                pairs[j].val += pairs[i + 2].val;
            }
            *pairs_len = replace_with_val(0, i, pairs, *pairs_len);
            return true;
        }
    }
    return false;
}

bool split (PairElem pairs[], int *pairs_len) {
    for (int i = 0; i < *pairs_len; ++i) {
        if (pairs[i].val >= 10) {
            int val = pairs[i].val;
            int left = floor(val / 2.0);
            int right = ceil(val / 2.0);
            *pairs_len = replace_with_pair(left, right, i, pairs, *pairs_len);
            return true;
        }
    }
    return false;
}

// similar to explode in that it replaces pairs with a number
// modifies pairs
int magnitude (PairElem pairs[], int *pairs_len) {
    while (*pairs_len > 1) {
        for (int i = 0; i < *pairs_len - 2; ++i) {
            if (pairs[i + 1].is_comma &&
                    pairs[i].depth == pairs[i + 2].depth &&
                    pairs[i].on_left == true &&
                    pairs[i + 2].on_left == false) {
                *pairs_len = replace_with_val(3 * pairs[i].val + 2 * pairs[i + 2].val, i, pairs, *pairs_len);
                break;
            }
        }
    }
    return pairs[0].val;
}

int main () {
    FILE *stream = fopen("inputs/input18", "r");

    PairElem all_pairs[MAX_LINES][MAX_PER_LINE];
    int all_pairs_lens[MAX_LINES];
    int all_pairs_len = 0;

    char line[MAX_PER_LINE];
    while (fscanf(stream, "%s", line) != EOF) {
        int curr_depth = 0;
        bool on_left = true;

        for (int i = 0; line[i] != '\0'; ++i) {
            switch (line[i]) {
                case '[':
                    on_left = true;
                    ++curr_depth;
                    break;
                case ',':
                    on_left = false;
                    all_pairs[all_pairs_len][all_pairs_lens[all_pairs_len]++] = (PairElem) {
                        .is_comma = true,
                        .depth = curr_depth
                    };
                    break;
                case ']':
                    --curr_depth;
                    break;
                default:
                    all_pairs[all_pairs_len][all_pairs_lens[all_pairs_len]++] = (PairElem) {
                        .is_comma = false,
                        .val = line[i] - '0',
                        .on_left = on_left,
                        .depth = curr_depth
                    };
                    break;
            }
        }
        ++all_pairs_len;
    }

    int largest_magnitude = INT_MIN;
    for (int i = 0; i < all_pairs_len; ++i) {
        for (int j = 0; j < all_pairs_len; ++j) {
            if (i == j) { continue; };

            PairElem pairs[MAX_PER_LINE];
            int pairs_len = 0;
            for (int k = 0; k < all_pairs_lens[i]; ++k) {
                pairs[pairs_len++] = all_pairs[i][k];
                ++pairs[pairs_len - 1].depth;
            }
            pairs[pairs_len++] = (PairElem) { .is_comma = true, .depth = 1 };
            for (int k = 0; k < all_pairs_lens[j]; ++k) {
                pairs[pairs_len++] = all_pairs[j][k];
                ++pairs[pairs_len - 1].depth;
            }
            // simplify
            while (true) {
                if (explode(pairs, &pairs_len)) {
                    continue;
                }
                if (split(pairs, &pairs_len)) {
                    continue;
                }
                break;
            }

            int pairs_magnitude = magnitude(pairs, &pairs_len);
            if (pairs_magnitude > largest_magnitude) {
                largest_magnitude = pairs_magnitude;
            }
        }
    }
    printf("%d\n", largest_magnitude);

    return 0;
}
