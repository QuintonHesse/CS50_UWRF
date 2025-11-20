#include <stdio.h>
#include <stdlib.h>

char board[9];
const unsigned short BOARD_SIZE = 9;

// this is a function prototype
void printBoard();
void initBoard();
int getInput(char player);


int main() {
    // this function prints a prompt for player X or O
     initBoard();

    char currentPlayer = 'X';

    while (1) {
        //clearScreen();
        printBoard();
        int index = getInput(currentPlayer);
        if (index == 9)
        {
            printf("Goodbye!\n");
            exit(0);
        }

        board[index] = currentPlayer;
        if (currentPlayer == 'X')
            currentPlayer = 'O';
        else
            currentPlayer = 'X';

    }
}

// an example function definition
// this function prints a Tic-Tac-Toe board
void printBoard() {
    printf("\n\n**************************\n\n");
    printf(" %c | %c | %c \n", board[0], board[1], board[2]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[3], board[4], board[5]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[6], board[7], board[8]);
}
// this function prints a prompt for player X or O
int getInput(char player) {
    char sposition[3];
    printf("\n\n**************************\n");
    printf("%c's turn!\nWhere would you like to go?\nPlease enter 0 through 8, or press 9 to quit.\n>", player);
    fgets(sposition, 3, stdin);

    // convert the string to an int
    int position = atoi(sposition);
    return position;
}
void initBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = '0' + i;
    }
}
