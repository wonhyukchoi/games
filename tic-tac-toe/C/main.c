#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIZE  3
#define EMPTY '_'

/* Displays the playing field */
void show_field(char array [SIZE][SIZE]){
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            printf("%c     ", array[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* Traverses the board.
 * If any elem is EMPTY, then board isn't full.
 * If it traverses w/o breaking, then it's full.
 */
bool board_not_full(char array[SIZE][SIZE]){
    for(int i=0; i<SIZE; i++){
        for (int j=0; j<SIZE; j++){
            if(array[i][j] == EMPTY){
                return true;
            }
        }
    }
    return false;
}

bool has_won(char array[SIZE][SIZE], char *winner){

    bool won;

    // Horizontal
    for(int i=0; i<SIZE; i++){
        *winner = array[i][0];
        won = true;
        for(int j=1; j<SIZE; j++){
            char elem = array[i][j];
            if(elem != *winner || elem == EMPTY) {
                won=false; break;
            }
        }
        if(won) return true;
    }

    // Vertical
    for(int j=0; j<SIZE; j++){
        *winner = array[0][j];
        won = true;
        for(int i=1; i<SIZE; i++){
            char elem = array[i][j];
            if(elem != *winner || elem == EMPTY) {
                won=false; break;
            }
        }
        if(won) return true;
    }

    return false;

}

/*
 * Gets long from stdin.
 * Reprompts on wrong input.
 * Uses recursion b/c I am lazy.
 */
long get_int(){
    int max_len = 80;
    char str[max_len];
    char *remaining;
    long number;

    fgets(str, max_len, stdin);
    number = strtol(str, &remaining, 10);

    if(*remaining != '\0' && *remaining != '\n'){
        printf("%sis not a proper number, "
               "please try again.\n", str);
        number = get_int();
    }
    return number;
}

/*
 * Gets row and column from stdin.
 * Re-prompts input if the block is already occupied.
 * (using recursion because I am lazy).
 * Inputs are assumed to be one-indexed.
 */
void get_coords(long* row, long* column
               ,char array[SIZE][SIZE]){

    printf("Enter your desired row:");
    *row = get_int();
    printf("Enter your desired column:");
    *column = get_int();

    // Checks if 2d array is already filled.
    // Converts one-indexing to zero-indexing.
    if(array[*row-1][*column-1] != EMPTY){
        printf("\nThat place is already filled."
               "Please try again.\n");
        get_coords(row, column, array);
    }

}

void play_game(){
    // Is there a better way to initialize?
    char field[SIZE][SIZE] = {EMPTY,EMPTY,EMPTY
                             ,EMPTY,EMPTY,EMPTY
                             ,EMPTY,EMPTY,EMPTY};
    bool player1 = true;
    char winner_symbol;
    char* winner;

    while(board_not_full(field)){

        long row, column;
        get_coords(&row, &column, field);

        // Don't use zero indexing for normal people.
        // If it's player 1, update it with 'X', otherwise 'O'.
        if(player1) field[row - 1][column - 1] = 'X';
        else field[row-1][column-1] = 'O';

        // Break loop & exit if game has been won.
        if(has_won(field, &winner_symbol)){
            winner = (winner_symbol=='X') ? "Player 1" : "Player 2";
            printf("%s has won!", winner);
            break;
        }
        player1 = !player1;
        show_field(field);
    }

    printf("\n *** Thanks for playing! ***\n");
}

int main() {

    play_game();

    return 0;

}
