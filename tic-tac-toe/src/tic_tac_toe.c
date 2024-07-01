#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main(int argc, char* argv[])
{
    if (argc != 3){
        printf("You must pass SIZE of board and Winning Rule\n");
        return 1;
    }
    int field_size = atoi(argv[1]);
    int win_rule = atoi(argv[2]);
    Field* field = init_field(field_size);
    GameStatus state = CONTINUE;
    print_field(field);
    while (state != NO_MORE_MOVES && state != PLAYER_WON) {
        game_loop(field, &state, win_rule);
    }
    print_field(field);
    if (state == NO_MORE_MOVES){
        printf("The game is ended, no more moves!\n");
    } else if (state == PLAYER_WON) {
        printf("Player %c has won!!!\n", field->player);
    }
    return 0;
}
