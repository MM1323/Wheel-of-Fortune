/*****
 * Project 02: Wheel of Fortune
 * COSC 208, Introduction to Computer Systems, Fall 2020
 *****/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants for word list
#define WORDS_FILE "/usr/share/dict/words"
#define MAX_WORD_LENGTH 50

// Constants and variables for wheel
#define WHEEL_SPACES 24
int wheel_amounts[WHEEL_SPACES];
int MAX_AMOUNT = 900;
int MIN_AMOUNT = 100;
#define INCREMENT_AMOUNT    50

// Constants and variables for wheel spin GUI
#define DISPLAY_ROWS    5
#define DISPLAY_COLS (WHEEL_SPACES * 2 + 1)
char* wheel_row[DISPLAY_ROWS];
#define SPIN_VARIANCE   6
#define NSEC_PER_MS     1000000
#define SPIN_TIME       125
#define STOP_SPEED      50
int ENABLE_ANIMATION = 1;

// Constants for user input
#define MAX_INPUT_LENGTH    50

// Constants for game play
#define VOWEL_COST      250
#define NUM_PLAYERS     2
int RANDOM_SEED = 1;

// Constants for messages
#define ACTION_PROMPT   "Spin (1-5), Buy (B), Solve (S)? "
#define INVALID_ACTION  "Invalid choice"
#define INSUFFICIENT_FUNDS  "Insufficient funds"
#define INVALID_LETTER  "Invalid letter"
#define ABSENT_LETTER   "Not in the puzzle"
#define SOLUTION_PROMPT "Solution? "
#define INCORRECT_SOLUTION  "Incorrect solution"

// Structure for linked list
struct item {
    char *word;
    struct item *next;
};
typedef struct item item_t;

/*
 * Initializes the amounts and GUI for the wheel.
 * DO NOT MODIFY
 */
void initialize_wheel() {
    // Populate the spaces on the wheel with dollar amounts
    int choices = ((MAX_AMOUNT - MIN_AMOUNT) / INCREMENT_AMOUNT) + 1;
    for (int s = 0; s < WHEEL_SPACES; s++) {
        wheel_amounts[s] = random() % choices * INCREMENT_AMOUNT + MIN_AMOUNT;
    }

    // Determine the characters to display for the wheel
    for (int r = 0; r < DISPLAY_ROWS-1; r++) {
        wheel_row[r] = (char*)malloc(DISPLAY_COLS);
        for (int s = 0; s < WHEEL_SPACES; s++) {
            char text[DISPLAY_ROWS];
            snprintf(text, DISPLAY_ROWS, "%4d", wheel_amounts[s]);
            wheel_row[r][s*2] = ' ';
            wheel_row[r][s*2+1] = text[r];
        }
        wheel_row[r][DISPLAY_COLS-1] = '\0';
    }
    wheel_row[DISPLAY_ROWS-1] = (char*)malloc(DISPLAY_COLS);
    memset(wheel_row[DISPLAY_ROWS-1], ' ', DISPLAY_COLS);
    wheel_row[DISPLAY_ROWS-1][DISPLAY_COLS-1] = '\0';
}

/*
 * Displays a low-tech ASCII animation of a wheel spin.
 * @param force how much force to use in spinning the wheel
 * @return the amount on the slot where the wheel stops spinning
 * DO NOT MODIFY
 */
int spin_wheel(int force) {
    static int curr_space = 0;

    // Print spaces on wheel
    for (int r = 0; r < DISPLAY_ROWS-1; r++) {
        printf("%s\n",wheel_row[r]);
    }

    // Select how many spaces to advance
    int num_ticks = WHEEL_SPACES + force * SPIN_VARIANCE;
    num_ticks += (random() % SPIN_VARIANCE) - (SPIN_VARIANCE/2);

    // Animate wheel spin
    if (ENABLE_ANIMATION) {
        long start_speed = SPIN_TIME / num_ticks * NSEC_PER_MS;
        long slow_down = (STOP_SPEED * NSEC_PER_MS - start_speed) / num_ticks;
        struct timespec sleep_time = {0, start_speed };
        for (int t = 0; t < num_ticks; t++) {
            curr_space = (curr_space + 1) % WHEEL_SPACES;
            int col = (curr_space % WHEEL_SPACES) * 2 + 1;
            wheel_row[DISPLAY_ROWS-1][col] = '+';
            printf("%s\r",wheel_row[DISPLAY_ROWS-1]);
            fflush(stdout);
            wheel_row[DISPLAY_ROWS-1][col] = ' ';
            sleep_time.tv_nsec += slow_down;
            nanosleep(&sleep_time, NULL);
        }
        printf("\n\n");
    } else {
        curr_space = (curr_space + num_ticks) % WHEEL_SPACES;
        int col = (curr_space % WHEEL_SPACES) * 2 + 1;
        wheel_row[DISPLAY_ROWS-1][col] = '+';
        printf("%s\n\n",wheel_row[DISPLAY_ROWS-1]);
        wheel_row[DISPLAY_ROWS-1][col] = ' ';
    }

    // Return amount on space
    return wheel_amounts[curr_space];
}

/*
 * Adds an item to the front of the linked list of words.
 * @param list_head the first item in the list; NULL if the list is empty
 * @param word the word to add
 * @return the added item (i.e., the new first item in the list)
 * TODO: Write this method
 */
item_t *prepend_item(item_t *list_head, char *word) {
    return NULL;
}

/*
 * Loads a list of words from a file into a linked list. Words containing 
 * non-alpha characters are ignored. All words are stored in upper-case.
 * @param filepath path to the file of words
 * @param words_loaded populated with the number of words loaded from the file
 * @return the linked list of words; NULL if an error occurred
 * TODO: Write this method
 */
item_t *load_words(const char *filepath, int *words_loaded) {
    return NULL;
}

/*
 * Destroys a linked list and frees all memory it was using.
 * @param list the first item in the list; NULL if the list is empty
 * TODO: Write this method
 */
void free_words(item_t *list_head) {
}

/*
 * Chooses a random word from a linked list of words.
 * @param list_head the first item in the list; NULL if the list is empty
 * @param length the number of words in the list
 * @return the chosen word; NULL if an error occurred
 * TODO: Write this method
 */
char *choose_random_word(item_t *list_head, int length) {
    return "COLGATE";
}

/*
 * Play a single round of wheel of fortune.
 * @param word the puzzle
 * @return the number of the player who won
 * TODO: Write this method
 */
int play_round(const char *word) {
    return -1;
}

/*
 * Play wheel of fortune using words loaded from a file.
 */
int main() {
    // Initialize wheel
    initialize_wheel();

    // Select random seed
    //srandom(RANDOM_SEED); // Uncomment for deterministic words selection

    // Load words
    int numwords = 0;
    item_t *list_head = load_words(WORDS_FILE, &numwords);
    if (NULL == list_head) {
        printf("Failed to load words from %s\n", WORDS_FILE);
        return 1;
    }

    // Select a word
    char *word = choose_random_word(list_head, numwords);
    if (NULL == word) {
        printf("Failed to choose a word\n");
        return 1;
    }

    // Play game
    int winner = play_round(word);
    printf("Player %d solved the puzzle!\n", winner);

    // Clean up
    free_words(list_head);

    // Clean-up wheel_rows
    for (int r = 0; r < DISPLAY_ROWS; r++) {
        free(wheel_row[r]);
    }
}
