#ifndef CORE_TEXTURE_MANAGER_H
#define CORE_TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdbool.h>

#include "log.h"

#define MAX_TEXTURE_COUNT 256
#define MAX_TEXTURE_PATH_CHARS 256 

typedef struct {
    char loaded_texture_paths[MAX_TEXTURE_COUNT][MAX_TEXTURE_PATH_CHARS]; //the array contains the directories of all loaded textures
    SDL_Texture* loaded_textures[MAX_TEXTURE_COUNT]; //the array contains pointers to SDL textures (some of them will be initialized some of them won't)
    int loaded_texture_count;
} TextureManager;

bool texture_manager_init (TextureManager *tm);
SDL_Texture* texture_manager_load_texture(TextureManager *tm, SDL_Renderer* renderer, const char* texture_path);
void texture_manager_shutdown(TextureManager *tm);

#endif
