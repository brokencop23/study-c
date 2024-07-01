#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "game.h"

#define min(a, b) ( (a) < (b) ? (a) : (b) )
#define max(a, b) ( (a) > (b) ? (a) : (b) )

void clear_screen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

Field* init_field(size_t size)
{
    Field* field = malloc(sizeof(Field));
    char* cells = malloc(sizeof(char) * (size * size));
    field->cells = cells;
    field->size = size;
    field->capacity = size * size;
    field->player = 'X';
    return field;
}

void field_clean(Field* field)
{
    free(field->cells);
    free(field);
}

void print_field(Field* field)
{
    clear_screen();
    int n = field->size * field->size;
    for (int i = 0; i < n; i++){
        switch (field->cells[i]){
            case 'X':
                printf("X");
                break;
            case 'O':
                printf("O");
                break;
            default:
                printf("%d", i + 1);
        }
        if ((i + 1) % field->size == 0){
            printf("\n");
        } else {
            printf("\t");
        }
    }
    printf("\n===================\n");
}

void switch_player(Field* field)
{
    if (field->player == 'X'){
        field->player = 'O';
    } else {
        field->player = 'X';
    }
}

uint8_t check_win(Field* field, int pos, int n)
{
    int row = pos / field->size;
    int col = pos % field->size;
    int i = 0;
    int start = 0;
    int end = 0;
    int freq = 0;

    // check horizontal
    freq = 0;
    start = max(0, col - (n - 1));
    end = min(field->size, col + (n - 1));
    for (i = start; i <= end; i++){
        if (field->cells[row * field->size + i] == field->player){
            freq++;
            if (freq == n)
                return 1;
        }
    }

    // check vertical
    freq = 0;
    start = max(0, row - (n - 1));
    end = min(field->size, row + (n - 1));
    for (i = start; i <= end; i++){
        if (field->cells[i * field->size + col] == field->player){
            freq++;
            if (freq == n)
                return 1;
        }
    }

    // check diagonal
    freq = 0;
    start = max(-n + 1, max(-row, -col));
    end = min(n - 1, min(field->size - 1 - row, field->size - 1 - col));
    for (i = start; i <= end; i++){
        if (field->cells[(row + i) * field->size + (col + i)] == field->player){
            freq++;
            if (freq == n)
                return 1;
        }
    }

    // check adiagonal
    freq = 0;
    start = max(-n + 1, max(-row, col - (field->size - 1)));
    end = min(n - 1, min(field->size - n - row, col));
    for (i = start; i <= end; i++){
        if (field->cells[(row + i) * field->size + (col - i)] == field->player){
            freq++;
            if (freq == n)
                return 1;
        }
    }
   
    return 0;

}

int make_move(Field* field, GameStatus* state)
{
    int ch;
    printf("User %c move:", field->player);
    scanf("%d", &ch);
    if (ch > field->size * field->size){
        *state = OUT_OF_FIELD;
    } else if (field->cells[ch - 1] != '\0') {
        *state = CELL_IS_FILLED;
    } else {
        field->cells[ch - 1] = field->player;
        field->capacity--;
    }
    return ch - 1;
}

void show_error(GameStatus* state)
{
    switch (*state) {
        case OUT_OF_FIELD:
            printf("You picked the number of out field\n");
            break;
        case CELL_IS_FILLED:
            printf("This field is occupied!\n");
            break;
        default:
            printf("Unknown error happend!\n");
    }
    while(getchar() != '\n');
    getchar();
}

void game_loop(Field* field, GameStatus* state, int win_rule)
{
    int move_ind = 0;
    print_field(field);
    move_ind = make_move(field, state);
    if (*state == CONTINUE) {
        if (check_win(field, move_ind, win_rule) == 1) {
            *state = PLAYER_WON;
        } else if (field->capacity == 0){
            *state = NO_MORE_MOVES;
        } else {
            switch_player(field);
        }
    } else {
        show_error(state);
        *state = CONTINUE;
    }
}
