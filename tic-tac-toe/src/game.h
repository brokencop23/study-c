#ifndef _game_h
#define _game_h

#include <stdint.h>

typedef enum {
    CONTINUE,
    NO_MORE_MOVES,
    PLAYER_WON,
    OUT_OF_FIELD,
    CELL_IS_FILLED
} GameStatus;

typedef struct {
    char* cells;
    size_t size;
    size_t capacity;
    char player;
} Field;

Field* init_field(size_t size);

void field_clean(Field* field);

void clear_screen();

void print_field(Field* field);

void switch_player(Field* field);

uint8_t check_win(Field* field, int pos, int n);

int make_move(Field* field, GameStatus* state);

void show_error(GameStatus* code);

void game_loop(Field *field, GameStatus* state, int win_rule);


#endif
