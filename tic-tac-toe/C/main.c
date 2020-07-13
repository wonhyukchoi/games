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

bool has_won(char array[SIZE][SIZE]){
    return false;
}

/*
 * Gets long from stdin.
 * Prompts for retry on incorrect input.
 * Updates
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
 */
void get_coords(long* row, long* column){
    printf("Enter your desired row:");
    *row = get_int();
    printf("Enter your desired column:");
    *column = get_int();
}

void play_game(){
    // Is there a better way to initialize?
    char field[SIZE][SIZE] = {EMPTY,EMPTY,EMPTY
                             ,EMPTY,EMPTY,EMPTY
                             ,EMPTY,EMPTY,EMPTY};
    bool playerX = false;

    while(board_not_full(field)){
        show_field(field);

        long row, column;
        get_coords(&row, &column);
        // Don't use zero indexing for normal people.
        if(playerX) field[row-1][column-1] = 'X';
        else field[row-1][column-1] = 'O';

        playerX = !playerX;
    }

    printf("\n *** Thanks for playing! ***\n");
}

int main() {

    play_game();

    return 0;

}
