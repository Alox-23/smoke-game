#ifndef CORE_INPUT_H
#define CORE_INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define PLAYER_SPEED 300
#define PLAYER_JUMP_STRENGTH 650
#define GRAVITY 2000

typedef struct{
    float move_x, move_y;
    bool is_jump_pressed;
} InputState;

void input_poll_state(InputState* in_s);

#endif
