/*****
 * Project 02: Wheel of Fortune
 * COSC 208, Introduction to Computer Systems, Fall 2020
 *****/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

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
int ENABLE_ANIMATION = 0;

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
    int curr_space = 0;

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
        printf("The col %d\n", col);
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
    if (list_head -> word == NULL) { // is their is no head make one
        list_head -> word = strdup(word);
        list_head -> next = NULL;
        return list_head;  // return the head
    } else { // create the next word struct
        item_t *nexts = malloc(sizeof(item_t));
        (*nexts).word = strdup(word);
        (*nexts).next = list_head;
        return nexts;  // return the added word aka the struct iteam_t
    } 
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
    // Load the file and error is no path is found
    FILE *infile = fopen(filepath, "r");
    if (infile == NULL) {
    printf("Error: unable to open file %s\n", "input.txt");
    exit(1);
    }
    // create a struct + the empty head
    item_t *list_head = malloc(sizeof(item_t));
    list_head -> word = NULL;
    list_head -> next = NULL;

    char line[100];
    while (fgets(line, 100, infile) != NULL) {
        int i;
        int alpha_check = 0;
        for(i = 0; i < strlen(line) - 1; i++) {
            if (isalpha(line[i]) == 0) {
                alpha_check = 1; //means their is a letter that is not an alpha
            } else {
                line[i] = toupper(line[i]); //convert letter to upper case
            }
        } 
        line[i] = '\0'; //Strip the New Line Charater
        
        if (list_head -> word == NULL) { //set the head
            list_head = prepend_item(list_head, line); 
        } else if (alpha_check == 0) { //if all letters are alpha put in list
            list_head = prepend_item(list_head, line); 
            *words_loaded = *words_loaded + 1; 
        }
        alpha_check = 0;        
    }
	fclose(infile); 
    return list_head;
}

/*
 * Destroys a linked list and frees all memory it was using.
 * @param list the first item in the list; NULL if the list is empty
 * TODO: Write this method
 */
 //This is unfinished. Still need to fix it. 
void free_words(item_t *list_head) {
    item_t *tmp;
    while (list_head != NULL) { //If head is null, reached the end of the list
        tmp = list_head; 
        free(list_head->word);
        list_head = list_head->next; //head now points to the next item in the list
        free(tmp); //de-allocates the memory for the head item in list
       
    }
}


/*
 * Chooses a random word from a linked list of words.
 * @param list_head the first item in the list; NULL if the list is empty
 * @param length the number of words in the list
 * @return the chosen word; NULL if an error occurred
 * TODO: Write this method
 */
char *choose_random_word(item_t *list_head, int length) {
    int n = length + 1;
    char state1[32];
    initstate(time(NULL), state1, sizeof(state1));
    setstate(state1);
    //Produces random number used to step through list
    while (n > length) {
         n = random();
    }
    //Step through linked list until word that corresponds with n is found 
    int count = 0;
    while (count < n) {
        list_head = list_head->next;
        count++;
    }
    printf("Random word is %s\n", list_head->word);
    return list_head->word;
    // return "COLGATE"; // TESTER
}

/*
 * Prints out current player's earnings
 * @param player turn and earnings 
 */
void player_turn(int turn, int earnings) {
    printf("Player %d's turn\n", turn);
    printf("Player %d's earnings: $%d\n", turn, earnings); 
}

//Creates initial puzzle 
char *make_puzzle(const char *word, char *puzzle) {
    int length = (int)strlen(word);
     for (int i = 0; i < length; i++) {
        puzzle[i] = '_';
    }
    return puzzle;
}

//prints puzzle for each turn
void print_puzzle(const char *word, char *puzzle) {
    int length = (int)strlen(word);
    for (int k = 0; k < length; k++){
        printf("%c ", puzzle[k]);
    }
    printf("\n");
}

char buy_vowel(char *past_vowel_guesses, const char *word) {
    char input[MAX_INPUT_LENGTH];
    char vowels[] = {'A', 'E', 'I', 'O', 'U'};
    bool is_vowel = false;
    // Print the vowels not already guessed
    printf("Vowel ( ");
    for (int i = 0; i < 5; i++) {
        bool guesses_vowel = true;
        for (int u = 0; u < 5;  u++){
            if (vowels[i] == past_vowel_guesses[u]) {
                guesses_vowel = false;
            }
        }
        if (guesses_vowel) {
            printf("%c ", vowels[i]);
        }
    }
    printf(")? ");
    //Gets users guess
    fgets(input,MAX_INPUT_LENGTH,stdin); 
    char player_guess = toupper(input[0]); //Converts input to uppercase
    //Test the User's Guess
    if (isalpha(player_guess) == 0 || strlen(input) - 1 > 1) { //if guess is a letter or >1
        return '4';
    }
    for (int x = 0; x < 5; x++) { //check if vowel was already guessed
        if (past_vowel_guesses[x] == player_guess) {
            return '4';
        }
        if (vowels[x] == player_guess) { //Check if guess is a vowel
            is_vowel = true;
        }
    }
    //Test where the guess is in the the answer
    if (is_vowel) {
        return player_guess;
    } else {
        return '4'; //Returns if input is not a vowel
    }
}

char guess_consonant(char *past_consonant_guesses, const char *word) {
    // printf("Hit\n");
    char input[MAX_INPUT_LENGTH];
    char consonants[] = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 
    'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z'};
    char vowels[] = {'A', 'E', 'I', 'O', 'U'};    
    // Print the consonants that have not already been guessed
    printf("Consonants ( ");
    for (int i = 0; i < 21; i++) {
        bool guesses_consonant = true;
        for (int u = 0; u < 21;  u++){
            if (consonants[i] == past_consonant_guesses[u]) {
                guesses_consonant = false;
            }
        }
        if (guesses_consonant) {
            printf("%c ", consonants[i]);
        }
    }
    printf(")? ");
    //Gets users guess)
    fgets(input,MAX_INPUT_LENGTH,stdin); 
    char player_guess = toupper(input[0]); //Converts input to uppercase
    //Test the User's Guess
    if (isalpha(player_guess) == 0 || strlen(input) - 1 > 1) { //if guess is not a letter or >1      
        return '6';
    }
    //Test to make sure user entered a consonant 
    for (int x = 0; x < 5; x++) { //check if vowel was guessed
        if (vowels[x] == player_guess) {
            return '6';
        }
    }
    //printf("Valid input (aka it's a consonant)\n"); //DEBUGGER
    for (int x = 0; x < 22; x++) { //check if consonant was already guessed
        if (past_consonant_guesses[x] == player_guess) {
            printf(INVALID_LETTER);
            printf("\n");
            return '4';
        }
        if (consonants[x] == player_guess) { //Check if guess is a consonant
            return player_guess;
        }
    }
    return '6';
}

bool solve_puzzle(const char *word) {
    int length = strlen(word);
    char player_guess[MAX_WORD_LENGTH+1];
    printf(SOLUTION_PROMPT);
    fgets(player_guess,MAX_WORD_LENGTH+1,stdin);
    for (int i = 0; i < length; i++){
        player_guess[i] = toupper(player_guess[i]); //convert letter to upper case
        if (player_guess[i] != word[i]) {
            return false;
        }
    }
    return true;
}

bool check_solved(const char * word, char * puzzle) {
    int length = strlen(word);
    // printf("The length is %d\n", length);
    for (int i = 0; i < length; i ++){
        if (word[i] != puzzle[i]){
            return false;
        }
    }
    return true;

}

/*
 * Play a single round of wheel of fortune.
 * @param word the puzzle
 * @return the number of the player who won
 * TODO: Write this method
 */
int play_round(const char *word) {
    int length = strlen(word) + 1;
    char *puzzle = malloc(sizeof(char) * length+1); 
    memset(puzzle, '\0', length+1);
    puzzle = make_puzzle(word, puzzle);
    // Used in buy_vowel
    char past_vowel_guesses[6];
    memset(past_vowel_guesses, '\0', 6);
    int buy_guesses = 0;
    // Used in guess_consonant
    int guesses = 0;
    char *past_consonant_guesses = malloc(sizeof(char) * 22); 
    memset(past_consonant_guesses, '\0', 22); 
    // Used to track players' turn
    int turn = 1;
    int player1_earn = 0;
    int player2_earn = 0;
    int * earnings; //= malloc(sizeof(int));
    while (puzzle != word) {
        if (turn == 1) {
            earnings = &player1_earn;
            player_turn(turn, *earnings);
        }
        if (turn == 2) {
            earnings = &player2_earn;
            player_turn(turn, *earnings);
        }
        char player_action;
        char input[MAX_INPUT_LENGTH];
        int wrong_input = 1;
        const char spin[] = "12345";
        bool show_puzzle = true;
        while (wrong_input==1) {
            bool in_word = false;
            if (show_puzzle) { //won't print if INVALID CHOICE occurs
                print_puzzle(word, puzzle);
                printf("\n");
            }
            printf(ACTION_PROMPT);
            fgets(input,MAX_INPUT_LENGTH,stdin); 
            player_action=input[0];
            player_action = toupper(player_action); //Converts input to uppercase
            if (player_action == 'B') {
                if ((*earnings) - VOWEL_COST < 0) {
                    printf(INSUFFICIENT_FUNDS);
                    printf("\n");
                    printf("Player %d's earnings: $%d\n\n", turn, *earnings);
                    turn = turn == 1 ? 2 : 1; //SWITCHES TURNS
                    wrong_input = 0;
                } else {
                    char vowel_answer = buy_vowel(past_vowel_guesses, word);                    
                    if (vowel_answer == '4'){
                        printf(INVALID_LETTER);
                        printf("\n\n");
                        turn = turn == 1 ? 2 : 1; //SWITCHES TURNS
                        wrong_input = 0;
                    } else {
                        for (int y = 0; y < strlen(word); y++) { //add letter to puzzle and to past guesses & subtract earnings
                            if (word[y] == vowel_answer) {
                                puzzle[y] = vowel_answer;
                                past_vowel_guesses[buy_guesses] = vowel_answer;    
                                *earnings = (*earnings) - VOWEL_COST;
                                in_word = true; //tells use that letter is in puzzle
                                buy_guesses += 1;
                                printf("Player %d's earnings: $%d\n\n", turn, *earnings);
                                if (check_solved(word, puzzle)){ //Check if the puzzle is solved
                                    return turn;
                                }
                             }
                        }
                        if (!in_word) { //add letter to past guesses and display error + switch turn
                            past_vowel_guesses[buy_guesses] = vowel_answer;
                            printf(ABSENT_LETTER);
                            printf("\n");
                            printf("Player %d's earnings: $%d\n\n", turn, *earnings);
                            buy_guesses += 1;
                            turn = turn == 1 ? 2 : 1; //SWITCHES TURNS
                            wrong_input = 0;
                        }
                    }               
                }          
            } else if (player_action == 'S') {  
                bool solved = solve_puzzle(word);
                if (solved == false) {
                    print_puzzle(word, puzzle);
                    turn = turn == 1 ? 2 : 1; //SWITCHES TURNS
                    wrong_input = 0;
                } else {
                    print_puzzle(word, puzzle); 
                    printf("\n\n");
                    return turn;
                }
            } else if (strchr(spin, player_action) != NULL) { //player selected a number (1-5)
                // printf("The player inputed %d \n", atoi(&player_action));
                int speed = atoi(input);
                printf("%d\n", speed);
                int potential_earnings = spin_wheel(speed);
                // int potential_earnings = 500;
                char consonant = guess_consonant(past_consonant_guesses, word);
                if (consonant == '4') {
                    turn = turn == 1 ? 2 : 1; //SWITCHES TURNS
                    wrong_input = 0;
                } else if (consonant == '6') {
                        printf(INVALID_ACTION);
                        printf("\n");
                        show_puzzle = false;
                        turn = turn == 1 ? 2 : 1; //SWITCHES TURNS
                        wrong_input = 0;
                } else  {
                    for (int y = 0; y < strlen(word); y++) {
                         if (word[y] == consonant) {
                            // printf("Definitely in word at %d index.\n", y);                                                        
                            // printf("able to change partial solution.\n");
                            // printf("number of guesses: %d.\n", guesses);
                            puzzle[y] = consonant;
                            past_consonant_guesses[guesses] = consonant;
                            in_word = true;
                            *earnings = (*earnings) + potential_earnings;
                            guesses += 1;
                            printf("Player %d's earnings: $%d\n\n", turn, *earnings);
                            if (check_solved(word, puzzle)){ //Check if it was solved
                                return turn;
                            }
                        }
                    }
                    if (!in_word) {
                        past_consonant_guesses[guesses] = consonant;
                        printf(ABSENT_LETTER);
                        printf("\n");
                        printf("Player %d's earnings: $%d\n\n", turn, *earnings);
                        guesses += 1;
                        turn = turn == 1 ? 2 : 1; //SWITCHES TURNS
                        wrong_input = 0;
                    }
                }
            } else {
                printf(INVALID_ACTION);
                printf("\n");
                show_puzzle = false;
            }
        }
    }
    return turn;
}

/*
 * Play wheel of fortune using words loaded from a file.
 */
int main() {
    // Initialize wheel
    initialize_wheel();

    // Select random seed
    srandom(RANDOM_SEED); // Uncomment for deterministic words selection

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
