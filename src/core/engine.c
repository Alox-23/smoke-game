#include "engine.h"
#include "animation.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

bool engine_init(Engine *engine, const char *title){
    if (!engine || !title){
        LOG_ERROR("Invalid argument to engine_init");
        return false;
    }

    LOG_INFO("Starting engine initialization");

    *engine = (Engine){0};

    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0){
        LOG_ERROR("SDL_Init: %s", SDL_GetError());
        return false;
    }

    const int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags){
        LOG_ERROR("IMG_Init: %s", IMG_GetError());
        goto fail_sdl;
    }

    engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    if (!engine->window){
        LOG_ERROR("SDL_CreateWindow: %s", SDL_GetError());
        goto fail_img;
    }

    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!engine->renderer){
        LOG_ERROR("SDL_CreateRenderer: %s", SDL_GetError());
        goto fail_window;
    }

    if (!texture_manager_init(&engine->texture_manager)){
        LOG_ERROR("Failed to initialize TextureManager");
        goto fail_renderer;
    }

    if (!world_init(&engine->world)){
        LOG_ERROR("Failed to initialize World");
        goto fail_renderer;
    }

    engine->test = texture_manager_load_texture(&engine->texture_manager, engine->renderer, "assets/realistic/TEST7B.bmp");
    SDL_Texture* test2 = texture_manager_load_texture(&engine->texture_manager, engine->renderer, "assets/mana_seed/character_base/char_a_p1/char_a_p1_0bas_humn_v00.png");
    engine->player = world_create_entity(&engine->world, HAS_POSITION | HAS_TEXTURE | HAS_VELOCITY | HAS_HEALTH | HAS_ANIMATION | HAS_SPRITE); 
    engine->world.positions[engine->player] = (Position){0, 0, 0};
    engine->world.velocities[engine->player] = (Velocity){0, 0, 0};
    engine->world.healths[engine->player].hp = 100;

    engine->world.sprites[engine->player].dst.w = 200;
    engine->world.sprites[engine->player].dst.h = 200;
    
    AnimationState as = {};
    AnimationClip ac = {};
    animation_init_clip(&ac, test2);
    animation_load_frames(&ac, 500, 6, 64, 64, 0, 4);
    int walk_down = animation_load_clip(&as, ac, "walk_down");
    
    animation_init_clip(&ac, test2);
    animation_load_frames(&ac, 500, 6, 64, 64, 0, 6);
    int walk_right = animation_load_clip(&as, ac, "walk_right");
    
    animation_init_clip(&ac, test2);
    animation_load_frames(&ac, 500, 6, 64, 64, 0, 5);
    int walk_up = animation_load_clip(&as, ac, "walk_up");
    
    animation_init_clip(&ac, test2);
    animation_load_frames(&ac, 500, 6, 64, 64, 0, 7);
    int walk_left = animation_load_clip(&as, ac, "walk_left");

    animation_init_clip(&ac, test2);
    animation_load_frames(&ac, 500, 6, 64, 64, 0, 0);
    int idle = animation_load_clip(&as, ac, "walk_down");

    animation_init_clip(&ac, test2);
    animation_load_frames(&ac, 500, 1, 64, 64, 6, 1);
    int jump_up = animation_load_clip(&as, ac, "jump_up");

    animation_init_clip(&ac, test2);
    animation_load_frames(&ac, 500, 1, 64, 64, 6, 2);
    int jump_right= animation_load_clip(&as, ac, "jump_right");

    animation_init_clip(&ac, test2);
    animation_load_frames(&ac, 500, 1 , 64, 64, 6, 3);
    int jump_left= animation_load_clip(&as, ac, "jump_left");

    animation_init_clip(&ac, test2);
    animation_load_frames(&ac, 500, 1, 64, 64, 6, 0);
    int jump_down = animation_load_clip(&as, ac, "jump_down");

    animation_play_clip(&as, animation_get_id_by_name(&as, "walk_down"));

    engine->world.animations[engine->player] = as;

    engine->running = false;

    LOG_INFO("Successfully initialized engine");
    return true;

fail_renderer:
    SDL_DestroyRenderer(engine->renderer);
fail_window:
    SDL_DestroyWindow(engine->window);
fail_img:
    IMG_Quit();
fail_sdl:
    SDL_Quit();
    *engine = (Engine){0};
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
        engine_tick(engine);
        engine_render(engine);
        engine_update(engine);
    }
}

void engine_tick(Engine *engine){
    if (!engine){
        LOG_ERROR("Invalid Engine argument");
        return;
    }

    // timing stuff
    Uint64 now = SDL_GetTicks64();
    engine->delta_time = (now - engine->last_tick) / 1000.0f;
    engine->last_tick = now;

    //increment the tick for logger
    log_tick();
}

void engine_render(Engine *engine){
    if (!engine){
        LOG_ERROR("Invalid Engine argument");
        return;
    }
    // --- Render ---
    SDL_SetRenderDrawColor(engine->renderer, 20, 20, 30, 255);
    SDL_RenderClear(engine->renderer);

    SDL_RenderCopy(engine->renderer, engine->test, NULL, NULL);

    world_render_system(&engine->world, engine->renderer);

    SDL_RenderPresent(engine->renderer);
}

void engine_update(Engine *engine){
    if (!engine){
        LOG_ERROR("Invalid Engine argument");
        return;
    }

    input_poll_state(&engine->input_state);

    world_input_system_entity(&engine->world, engine->player, &engine->input_state);
    world_health_system(&engine->world);
    world_movement_system(&engine->world, engine->delta_time);
    world_animation_system(&engine->world, engine->delta_time);

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

    if (engine->renderer) SDL_DestroyRenderer(engine->renderer);
    if (engine->window) SDL_DestroyWindow(engine->window);
    texture_manager_shutdown(&engine->texture_manager);
    
    IMG_Quit();
    SDL_Quit();

    world_shutdown(&engine->world);
    
    LOG_INFO("Sucsesfully shutdown the Engine");
}
