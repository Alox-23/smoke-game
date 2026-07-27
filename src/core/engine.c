#include "engine.h"

bool engine_init(Engine *engine, const char *title){
    if (!engine){
        LOG_ERROR("Invalid Engine argument");
        return true;
    }

    LOG_INFO("Starting Engine initilaization");
    
    engine->running = false;

    //initialize SDL
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)){
        LOG_ERROR("%s", SDL_GetError());
        return true; //error happened
    }

    // Initialize SDL_image for PNG and JPG support
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        LOG_ERROR("%s", SDL_GetError());
        return true;
    }   

    //initialize SDL_Window
    engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    if (!engine->window){
        LOG_ERROR("%s", SDL_GetError());
        return true; //error happened
    }

    //initialize SDL_Renderer
    engine->renderer = SDL_CreateRenderer(engine->window, -1, 0);
    if (!engine->renderer){
        LOG_ERROR("%s", SDL_GetError());
        return true; //error happened
    }

    LOG_INFO("Sucsesfully initialized Engine");
    return false;
}

void engine_run(Engine *engine){
    if (!engine){
        LOG_ERROR("Invalid Engine argument");
        return; //invalid engine argument
    }

    LOG_INFO("Engine is running");

    engine->last_tick = SDL_GetTicks64();
    engine->running = true;

    while (engine->running){
        // timing stuff
        Uint64 now = SDL_GetTicks64();
        float delta_time = (now - engine->last_tick) / 1000.0f;
        engine->last_tick = now;

        //increment the tick for logger
        log_tick();

        // input and events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    engine_quit(engine);
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        engine_quit(engine);
                    }
                    break;
            }
        }

        // --- Render ---
        SDL_SetRenderDrawColor(engine->renderer, 20, 20, 30, 255);
        SDL_RenderClear(engine->renderer);
        
        SDL_RenderPresent(engine->renderer);
    }
}

void engine_quit(Engine *engine){
    if (!engine){
        LOG_ERROR("Invalid Engine argument");
        return; //invalid engine argument
    }

    engine->running = false;

    LOG_INFO("Engine has quit");
} 

void engine_shutdown(Engine *engine){
    if (!engine){
        LOG_ERROR("Invalid Engine argument");
        return; //invalid engine argument
    }

    SDL_DestroyWindow(engine->window);
    SDL_DestroyRenderer(engine->renderer);

    SDL_Quit();
    IMG_Quit();
    
    LOG_INFO("Sucsesfully shutdown the Engine");
}
