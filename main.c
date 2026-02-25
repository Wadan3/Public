/*
  puzzle_lock.c
  A tiny interactive puzzle: guess the 3-digit lock code using hints.

  Rules:
  - Code is 3 digits (000-999).
  - After each guess, you get:
      • "right place" = correct digit in correct position
      • "wrong place" = correct digit but in a different position
  - You have limited attempts.

  Build:
    gcc -std=c11 -O2 -Wall -Wextra -pedantic puzzle_lock.c -o puzzle_lock
  Run:
    ./puzzle_lock.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CODE_LEN 3
#define MAX_ATTEMPTS 10

static void strip_newline(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') {
        s[n - 1] = '\0';
    }
}

static int all_digits_len(const char *s, size_t len) {
    if (strlen(s) != len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
    }
    return 1;
}

/* Compute bulls/cows style feedback:
   right_place: digit matches at same index
   wrong_place: digit exists in code but different index (counted without double counting)
*/
static void score_guess(const char code[CODE_LEN + 1],
                        const char guess[CODE_LEN + 1],
                        int *right_place,
                        int *wrong_place) {
    int code_count[10] = {0};
    int guess_count[10] = {0};

    *right_place = 0;
    *wrong_place = 0;

    // First pass: count exact matches
    for (int i = 0; i < CODE_LEN; i++) {
        if (guess[i] == code[i]) {
            (*right_place)++;
        } else {
            code_count[code[i] - '0']++;
            guess_count[guess[i] - '0']++;
        }
    }

    // Second pass: count digits that exist but are misplaced
    for (int d = 0; d < 10; d++) {
        *wrong_place += (code_count[d] < guess_count[d]) ? code_count[d] : guess_count[d];
    }
}

static void generate_code(char out[CODE_LEN + 1]) {
    int value = rand() % 1000; // 000..999
    // Ensure leading zeros by formatting width=3
    snprintf(out, CODE_LEN + 1, "%03d", value);
}

int main(void) {
    srand((unsigned)time(NULL));

    char code[CODE_LEN + 1];
    generate_code(code);

    puts("🔐 Puzzle Lock (C)");
    puts("Guess the 3-digit code (000-999).");
    puts("Hint format: [right place] [wrong place]");
    puts("Example: 1 2 means 1 digit correct spot, 2 digits correct but wrong spots.");
    puts("");

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        char buf[128];
        char guess[CODE_LEN + 1];

        printf("Attempt %d/%d > ", attempt, MAX_ATTEMPTS);
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            return 1;
        }
        strip_newline(buf);

        if (!all_digits_len(buf, CODE_LEN)) {
            puts("Please enter exactly 3 digits (e.g., 042).");
            attempt--; // don't consume an attempt for invalid input
            continue;
        }

        memcpy(guess, buf, CODE_LEN);
        guess[CODE_LEN] = '\0';

        int right_place = 0, wrong_place = 0;
        score_guess(code, guess, &right_place, &wrong_place);

        if (right_place == CODE_LEN) {
            printf("✅ Unlocked! Code was %s. Solved in %d attempt(s).\n", code, attempt);
            return 0;
        }

        printf("Hint: %d %d\n\n", right_place, wrong_place);
    }

    printf("❌ Out of attempts. The code was %s.\n", code);
    return 0;
}
