# Project 02: Wheel of Fortune

## Overview
In this project, you'll implement a simple, text-based version of the game Wheel of Fortune.

### Learning objectives
After completing this project, you should be able to:
* Read/create/modify strings in C
* Read files in C
* Use pointers in C to indirectly access values
* Determine when functions should take pointers as parameters and/or return a pointer
* Write C programs that dynamically allocate and free heap memory
* Use valgrind to diagnose memory problems in C programs

### Important tips
* Read the entirety of the project description before you write any code.
* Work on the project over multiple sessions. Do not try to complete the project in a single session on the day (before) the project is due. This will give you time to think about how to solve problems, allow you to ask questions, and result in better outcomes.
* Ensure you follow good program design, coding, testing, and debugging practices (details below).

## Getting started 
If you are unfamiliar with the game, I recommend you download the Wheel of Fortune game for [iOS](https://apps.apple.com/us/app/wheel-of-fortune-free-play-game-show-word-puzzles/id898040123) or [Android](https://play.google.com/store/apps/details?id=com.scopely.wheeloffortune&hl=en_IN) and play a few rounds.

Visit [this page](https://classroom.github.com/g/nwKA8Xs6) to create and obtain access to a private git repository containing the starter code for the project. After the repository is created, you should clone your repository on a birds server.

The code you need to write for this lab will be added to the `wheel.c` file included in your repository. When you want to compile your code, run the command
```bash
$ make
```
which will compile the code using the commands in the `Makefile` included in your repository.

To play the game, run the command:
```bash
$ ./wheel
```

## Part 1: Obtaining a word

### Reading the file of words
To obtain a set of possible words for the game, you should read the file `/usr/share/dict/words` (it's a standard file on Linux and UNIX systems).  In this file, each line contains a separate word.  Beware that when you read a line from a file using `fgets`, the last character in the line is almost always a newline character (`\n`); you should strip this character. See [_Dive Into Systems_ Section 2.8](https://diveintosystems.org/antora/diveintosystems/1.0/C_depth/IO.html) to learn how to read files in C.

The words you load from the file should be added to a linked list. You should only include words that contain only alphabetic characters, so any words with possessives or plurals that have an apostrophe should be ignored.  Furthermore, you should convert all characters to uppercase before adding a word to the list. 

There is a definition of a linked list struct and some function prototypes (`load_words`, `prepend_item`, and `free_words`)  in `wheel.c` to help you get started. You **must** use the provided struct definition and function prototypes. You may want to copy (and modify) code from [Lab 04](https://github.com/colgate-cosc208-fall20/lab04)

This load_words function takes two parameters—a string containing the path to the file of words and a pointer to an int—and returns the linked list of words.  The second parameter to the function (pointer to an int) should be set to the number of words that are contained in the linked list (i.e., you should modify the int pointed to by the parameter to set it to the number of words loaded from the file).

When your program completes, it should free the entire contents of the linked list using the free_words function. You will need to call malloc in prepend_item to allocate new linked list entries, and in free_words your program you should free all these linked list entries. 

### Useful functions
You can (and should) use built-in C functions to help with the above tasks.  For example:
* `fopen` can be used to open a file
* `fgets` can be used to read a line from the file
* `fclose` is used to close a file
* `isalpha` can be used to test whether a character is alphabetic
* `toupper` can be used to convert a single letter to uppercase
You can also use other built-in C functions; there are no restrictions on what functions you may use.

### Choosing a word
After loading the words into the linked list, you should randomly choose one word from the list.  One approach you can take is to choose a random integer `n` (where `n` is less than or equal to the number of words in the list) and "walk" along the linked list until you've passed `n` nodes.  To choose a random integer in C, you can use the built-in `random()` function (header file is `<stdlib.h>`), which returns a random integer between 0 and 2^31-1. (There are other built-in functions for getting random numbers, but `random()` is probably the easiest one for you to use.)

There's a function named `choose_random_word` that you can complete to finish this part of the project.

## Part 2: Gameplay
You should complete the function `play_round` in `wheel.c`. The function takes the secret word (as a const C string) as a parameter, and returns the number of the winning player. The function's signature must not be modified. Also, you should not need to modify the main function, but you can if you need to.

The game will have two players (as opposed to the usual three). Player 1 automatically goes first. 

### Turns
At the beginning of each turn, you should display the following messages (on two separate lines), replacing `X` with `1` or `2` (depending on whose turn it is) and `Y` with the player’s current earnings:
```
Player X's turn
Player X's earnings: $Y
```

Then display the current state of the puzzle. If a letter in the word has already been guessed print the letter, otherwise print an underscore. For example:
```
C O _ G _ T E
```

Next, ask the current player whether they would like to: spin the wheel with a particular force (selected from a range of 1 to 5), buy a vowel, or solve the puzzle. Use the constant ACTION_PROMPT to display the choices. 

The player should enter an appropriate character---`1`, `2`, `3`, `4`, `5`, `B` (for buy), or `S` (for solve). The C standard library function `fgets` is a good choice for getting keyboard input. If the player enters an invalid character or more than one character, then display the error message defined by the constant `INVALID_ACTION` and prompt the user for a new choice.

### Spinning and guessing a consonant
If the player chooses to spin, you should:
1. Determine how much they will earn for each occurrence of the consonant they guess. Invoke the provided `spin_wheel` function to display a low-tech ASCII animation of a wheel spin; the function returns the amount on the slot where the wheel stops spinning.
2. Ask for a consonant that has not already been guessed. You should display the list of unguessed consonants when you prompt the user for input. You should accept uppercase or lowercase letters. If the player enters a non-consonant character, more than one character, or a consonant that has already been guessed, then display the error message defined by the constant `INVALID_LETTER` and end the turn.
3. Check if the consonant is present in the puzzle. If the consonant is present, then award the player the amount displayed on the wheel for each occurrence of the chosen consonant. The player gets another turn for a correct guess. Otherwise, display the error message defined by the constant `ABSENT_LETTER` and end the turn.

### Buying a vowel
If the player chooses to buy a vowel, you should:
1. Check if the player has enough money (at least $250) to buy a vowel. If not, display the error message defined by the constant `INSUFFICIENT_FUNDS` and end the turn.
2. Ask for a vowel that has not already been guessed. You should display the list of unguessed vowels when you prompt the user for input. You should accept uppercase or lowercase letters. If the player enters a non-vowel character, more than one character, or a vowel that has already been guessed, then display the error message defined by the constant `INVALID_LETTER` and end the turn.
3. Deduct $250 from the player’s earnings.
4. Check if the vowel is present in the puzzle. If the vowel is present, then the player gets another turn. Otherwise, display the error message defined by the constant `ABSENT_LETTER` and end the turn.

### Solving the puzzle
If the player chooses to solve the puzzle, you should:
1. Ask for a solution using the constant `SOLUTION_PROMPT`. The case of the letters in the string does not matter. 
2. If the player enters a correct solution, then return the current player’s number from the play_round function. Otherwise, the turn ends and play passes to the next player.

### End of a turn
At the end of each turn, you should display the player’s current earnings, regardless of whether or not the player gets another turn.

## Program design

You **must follow good program design and coding practices**, including:
* Using the constants that are defined at the top of `wheel.c` (e.g., `VOWEL_COST` and `INVALID_ACTION`) --- Do not put the raw numbers or strings (e.g., `250` or `"Invalid choice"`) directly in your code. Using constants makes the code easier to read and modify.
* Using multiple functions --- Do not put all of your code for part 2 in the `play_round` function. You should use multiple functions, where each function: is Short, does One thing, takes Few parameters, and maintains a single level of Abstraction. In other words, follow the _SOFA_ criteria from COSC 101.
* Freeing all heap-allocated memory before exiting --- Run `valgrind` to confirm all memory is freed and no other memory errors exist in your program.
* Properly indenting your code --- Recall that indentation is not semantically significant in C, but it makes your code much easier to read.
* Including comments --- Each function (except for `main`), must be preceeded by a short comment that describes what the function does. The body of the function must also include comments; generally, you should include a comment before each conditional statement, loop, and set of statements that perform some calculation. **Do not** include a comment for every line of code, and **do not** simply restate the code.
* Making multiple commits to your git repository --- Do not wait until your entire program is working before you commit it to your git repository. You should commit your code each time you write and debug a piece of functionality (e.g., after writing and testing code to check the binary formula, after writing and testing the code for extending the length of operands, etc.)

## Testing and debugging your code
Play several games to test your code. If you want to run your code multiple times with the same word, then uncomment the call to `srandom` in the main function. Make sure your output matches the example shown below. All input prompts and error messages should use the exact phrases shown below, most of which are defined as constants at the top of the provided `wheel.c` file. It is not essential for the white space (i.e., spaces and blank lines) to match exactly 

The first step in debugging your code is to fix any errors or warnings output by the compiler. You may be tempted to ignore the warnings, but they almost always mean there is a bug in your code. 

If your program crashes (e.g., with a segmentation fault), then run your program with `valgrind` (see [Lab 4](https://github.com/colgate-cosc208-fall20/lab04#testing-and-debugging-your-code) for instructions) to help locate the problem.

If your program still doesn't work as expected after you have fixed all compiler warnings and memory errors, then add `printf` statements in strategic locations to help you understand what your program is doing.

## Submission instructions
You should **commit and push** your updated `wheel.c` file to your git repository. However, as noted above, do not wait until your entire program is working before you commit it to your git repository; you should commit your code each time you write and debug a piece of functionality. You should visit the GitHub page for your repository and view the log and the `wheel.c` file to confirm you have correctly committed and pushed your changes.

## Example Game
```
Player 1's turn
Player 1's earnings: $0

 _ _ _ _ _ _ _

Spin (1-5), Buy (B), Solve (S)? 1

 6 8 5 1 8 2 2 8 6 2 3 8 2 2 7 3 6 7 4 8 1 5 2 3
 0 5 5 0 5 5 5 0 5 0 5 0 5 0 0 5 5 5 0 0 5 0 0 5
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
         +

Consonant ( B C D F G H J K L M N P Q R S T V W X Y Z )? T
Player 1's earnings: $850

 _ _ _ _ _ T _

Spin (1-5), Buy (B), Solve (S)? B
Vowel ( A E I O U )? E
Player 1's earnings: $600

 _ _ _ _ _ T E

Spin (1-5), Buy (B), Solve (S)? 2

 6 8 5 1 8 2 2 8 6 2 3 8 2 2 7 3 6 7 4 8 1 5 2 3
 0 5 5 0 5 5 5 0 5 0 5 0 5 0 0 5 5 5 0 0 5 0 0 5
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
                                   +

Consonant ( B C D F G H J K L M N P Q R S V W X Y Z )? R
Not in the puzzle
Player 1's earnings: $600

 _ _ _ _ _ T E

Player 2's turn
Player 2's earnings: $0
Spin (1-5), Buy (B), Solve (S)? spin
Invalid choice
Spin (1-5), Buy (B), Solve (S)? 0
Invalid choice
Spin (1-5), Buy (B), Solve (S)? 6
Invalid choice
Spin (1-5), Buy (B), Solve (S)? 5

 6 8 5 1 8 2 2 8 6 2 3 8 2 2 7 3 6 7 4 8 1 5 2 3
 0 5 5 0 5 5 5 0 5 0 5 0 5 0 0 5 5 5 0 0 5 0 0 5
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
                                               +

Consonant ( B C D F G H J K L M N P Q S V W X Y Z )? a  
Invalid letter
Player 2's earnings: $0

 _ _ _ _ _ T E

Player 1's turn
Player 1's earnings: $600
Spin (1-5), Buy (B), Solve (S)? 3
                                                
 6 8 5 1 8 2 2 8 6 2 3 8 2 2 7 3 6 7 4 8 1 5 2 3
 0 5 5 0 5 5 5 0 5 0 5 0 5 0 0 5 5 5 0 0 5 0 0 5
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
                               +                

Consonant ( B C D F G H J K L M N P Q S V W X Y Z )? xyz
Invalid letter
Player 1's earnings: $600

 _ _ _ _ _ T E

Player 2's turn
Player 2's earnings: $0
Spin (1-5), Buy (B), Solve (S)? 4
                                                
 6 8 5 1 8 2 2 8 6 2 3 8 2 2 7 3 6 7 4 8 1 5 2 3
 0 5 5 0 5 5 5 0 5 0 5 0 5 0 0 5 5 5 0 0 5 0 0 5
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
                                   +            

Consonant ( B C D F G H J K L M N P Q S V W X Y Z )? T
Invalid letter
Player 2's earnings: $0

 _ _ _ _ _ T E

Player 1's turn
Player 1's earnings: $600
Spin (1-5), Buy (B), Solve (S)? B
Vowel ( A I O U )? u
Not in the puzzle
Player 1's earnings: $350

 _ _ _ _ _ T E

Player 2's turn
Player 2's earnings: $0
Spin (1-5), Buy (B), Solve (S)? B
Insufficient funds
Player 2's earnings: $0

 _ _ _ _ _ T E

Player 1's turn
Player 1's earnings: $350
Spin (1-5), Buy (B), Solve (S)? S
Solution? Colgate
Player 1's earnings: $350

 C O L G A T E

Player 1 solved the puzzle!
```
