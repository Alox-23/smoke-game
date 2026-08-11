#ifndef CORE_INPUT_H
#define CORE_INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define PLAYER_SPEED 400
#define PLAYER_JUMP_STRENGTH 700
#define GRAVITY 1500

typedef struct{
    float move_x, move_y;
    bool is_jump_pressed;
} InputState;

void input_poll_state(InputState* in_s);

#endif
