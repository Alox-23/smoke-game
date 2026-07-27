#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "log.h"

typedef struct{
    bool running;

    Uint64 last_tick;
    float delta_time;

    SDL_Renderer* renderer;
    SDL_Window* window;
    int window_width;
    int window_height;
} Engine;

bool engine_init(Engine *engine, const char *title); //called in main() - this initializes the engine's resourses
void engine_run(Engine *engine); //called in main() - start the mainloop
void engine_quit(Engine *engine); //called in engine_run() - just a way to exit the game without having to reinitalize resourses later
void engine_shutdown(Engine *engine); //called in main() - actually free's engine's resourses

#endif
