#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "log.h"
#include "texture_manager.h"
#include "../ecs/systems.h"

typedef struct{
    bool running;

    Uint64 last_tick;
    float delta_time;

    SDL_Renderer* renderer;
    SDL_Window* window;
    int window_width;
    int window_height;
    TextureManager texture_manager;
    SDL_Texture *test;

    World world;
    Entity player;
} Engine;

bool engine_init(Engine *engine, const char *title); //called in main() - this initializes the engine's resourses
void engine_run(Engine *engine); //called in main() - start the mainloop
void engine_tick(Engine *engine);
void engine_render(Engine *engine);
void engine_update(Engine *engine);
void engine_quit(Engine *engine); //called in engine_run() - just a way to exit the game without having to reinitalize resourses later
void engine_shutdown(Engine *engine); //called in main() - actually free's engine's resourses

#endif
