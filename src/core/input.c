#include "input.h"

void input_poll_state(InputState* state){
    const Uint8* keys =  SDL_GetKeyboardState(NULL);

    state->move_x = 0;
    state->move_y = 0;
    state->is_jump_pressed = false;

    if (keys[SDL_SCANCODE_A]) state->move_x -= 1;
    if (keys[SDL_SCANCODE_S]) state->move_y -= 1;
    if (keys[SDL_SCANCODE_D]) state->move_x += 1;
    if (keys[SDL_SCANCODE_W]) state->move_y += 1;

    if (keys[SDL_SCANCODE_SPACE]) state->is_jump_pressed = true;
}
