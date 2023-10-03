#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SETFOREGROUND   "\x1b[91m"
#define SETBACKGROUND   "\x1b[49m"
#define RESETFOREGROUND "\x1b[39m\n"
#define RESETBACKGROUND "\x1b[49m\n"
#define RESETALL        "\x1b[39;49m\n"
#define NBOXES          9
#define SYMUSER         'X'
#define SYMSYSTEM       'O'

#pragma warning(disable : 4710)

typedef enum { ROW0, ROW1, ROW2, COL0, COL1, COL2, DIAG0, DIAG1 } axes_t;

typedef enum { USER, SYSTEM } player_t;

// A struct that contains game status.
typedef struct {
        player_t player;
        axes_t   axis;
} status_t;

void static __forceinline PrintBoard(char* board) {
    printf_s(
        SETFOREGROUND                         SETBACKGROUND
        "|-----------------|" RESETBACKGROUND SETBACKGROUND "|  %c  |  %c  |  %c  |" RESETBACKGROUND SETBACKGROUND
        "|-----------------|" RESETBACKGROUND SETBACKGROUND "|  %c  |  %c  |  %c  |" RESETBACKGROUND SETBACKGROUND
        "|-----------------|" RESETBACKGROUND SETBACKGROUND "|  %c  |  %c  |  %c  |" RESETBACKGROUND SETBACKGROUND
        "|-----------------|" RESETALL,
        board[0],
        board[1],
        board[2],
        board[3],
        board[4],
        board[5],
        board[6],
        board[7],
        board[8]
    );
    return;
}

// Will return -1 if the passed board is full.
int static __forceinline PickABox(char* board) {
    int empties[9] = { 0 };
    int offset     = 0;
    for (int i = 0; i < 10; ++i)
        if (board[i] == 32) empties[offset++] = i;
    // assert(offset != 0);
    return (offset == 0) ? -1 : empties[(rand() % offset)];
}

int static __forceinline GetValidatedUserInput(char* board) {
    int iput = 0;
    scanf_s("%d", &iput);
    assert(iput > 0 && iput < 10);
    if (board[iput] == 32) {
        return iput;
    } else {
        puts("Uh oh! That box is already occupied! Pick another!");
        return GetValidatedUserInput(board);
    }
}

status_t static __forceinline TestForWins(char* board) { }

void static __forceinline StartGame(char* board) {
    if (rand() % 2) {
        puts("User gets to make the first move!");
        while (true) {
            puts("Waiting for keystroke...");
            board[GetValidatedUserInput(board) - 1] = SYMUSER;
            PrintBoard(board);
            // _sleep(1000);
            board[PickABox(board)] = SYMSYSTEM;
            system("cls");
            PrintBoard(board);
        }
    } else {
        PickABox(board);
    }
    return;
}

int main(void) {
    char board[NBOXES] = { 32, 32, 32, 32, 32, 32, 32, 32, 32 }; // 0 causes misalignments in printing.

    StartGame(board);

    return 0;
}