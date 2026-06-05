/*
 * Puzzle Lock
 * A small interactive C terminal game.
 *
 * Goal:
 * Guess the hidden 3-digit lock code.
 *
 * Feedback:
 * - Right place: correct digit in the correct position
 * - Wrong place: correct digit in the wrong position
 *
 * Build:
 *   gcc -std=c11 -O2 -Wall -Wextra -pedantic main.c -o puzzle_lock
 *
 * Run:
 *   ./puzzle_lock
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CODE_LENGTH 3
#define MAX_ATTEMPTS 10
#define INPUT_SIZE 128

static void remove_newline(char *text) {
    size_t length = strlen(text);

    if (length > 0 && text[length - 1] == '\n') {
        text[length - 1] = '\0';
    }
}

static int is_valid_guess(const char *guess) {
    if (strlen(guess) != CODE_LENGTH) {
        return 0;
    }

    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess[i] < '0' || guess[i] > '9') {
            return 0;
        }
    }

    return 1;
}

static void generate_secret_code(char code[CODE_LENGTH + 1]) {
    int random_number = rand() % 1000;
    snprintf(code, CODE_LENGTH + 1, "%03d", random_number);
}

static void calculate_feedback(
    const char secret_code[CODE_LENGTH + 1],
    const char guess[CODE_LENGTH + 1],
    int *right_place,
    int *wrong_place
) {
    int secret_digit_count[10] = {0};
    int guess_digit_count[10] = {0};

    *right_place = 0;
    *wrong_place = 0;

    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess[i] == secret_code[i]) {
            (*right_place)++;
        } else {
            secret_digit_count[secret_code[i] - '0']++;
            guess_digit_count[guess[i] - '0']++;
        }
    }

    for (int digit = 0; digit < 10; digit++) {
        *wrong_place += secret_digit_count[digit] < guess_digit_count[digit]
            ? secret_digit_count[digit]
            : guess_digit_count[digit];
    }
}

static void print_intro(void) {
    printf("\n====================================\n");
    printf("           Puzzle Lock\n");
    printf("====================================\n");
    printf("Guess the hidden 3-digit code.\n");
    printf("The code can include leading zeros.\n");
    printf("Example valid guesses: 042, 123, 900\n\n");

    printf("Feedback format:\n");
    printf("- Right place: correct digit and correct position\n");
    printf("- Wrong place: correct digit but wrong position\n\n");

    printf("You have %d attempts to unlock the puzzle.\n", MAX_ATTEMPTS);
    printf("------------------------------------\n");
}

int main(void) {
    char secret_code[CODE_LENGTH + 1];
    char input[INPUT_SIZE];

    srand((unsigned int) time(NULL));
    generate_secret_code(secret_code);

    print_intro();

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        char guess[CODE_LENGTH + 1];

        printf("\nAttempt %d/%d\n", attempt, MAX_ATTEMPTS);
        printf("Enter your guess: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nInput error. Exiting the game.\n");
            return 1;
        }

        remove_newline(input);

        if (!is_valid_guess(input)) {
            printf("Invalid guess. Please enter exactly 3 digits.\n");
            attempt--;
            continue;
        }

        strncpy(guess, input, CODE_LENGTH);
        guess[CODE_LENGTH] = '\0';

        int right_place = 0;
        int wrong_place = 0;

        calculate_feedback(secret_code, guess, &right_place, &wrong_place);

        if (right_place == CODE_LENGTH) {
            printf("\nUnlocked successfully!\n");
            printf("Secret code: %s\n", secret_code);
            printf("Solved in %d attempt(s).\n", attempt);
            return 0;
        }

        printf("Hint: %d right place, %d wrong place\n", right_place, wrong_place);
    }

    printf("\nGame over. You ran out of attempts.\n");
    printf("The secret code was: %s\n", secret_code);

    return 0;
}
