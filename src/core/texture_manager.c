#include "texture_manager.h"

bool texture_manager_init (TextureManager *tm){
    if (!tm){
        LOG_ERROR("Invalid TextureManager argument");
        return true;
    }

    tm->loaded_texture_count = 0;

    return false;
}

SDL_Texture* texture_manager_load_texture(TextureManager *tm, SDL_Renderer* renderer, const char* texture_path){
    if (!tm){
        LOG_ERROR("Invalid TextureManager argument");
        return NULL;
    }

    //load the surface and texture
    SDL_Surface* temp_surface = IMG_Load(texture_path);
    if (!temp_surface){
        LOG_ERROR("%s", SDL_GetError());
        return NULL;
    }

    SDL_Texture* texture_pointer = SDL_CreateTextureFromSurface(renderer, temp_surface);
    if (!texture_pointer){
        SDL_FreeSurface(temp_surface);
        LOG_ERROR("%s", SDL_GetError());
        return NULL;
    }

    //add the loaded texture to the loaded_textures array and increment the loaded_textures_count
    tm->loaded_textures[tm->loaded_texture_count] = texture_pointer;
    tm->loaded_texture_count++;

    return texture_pointer;
}

void texture_manager_shutdown(TextureManager *tm){
    if (!tm){
        LOG_ERROR("Invalid TextureManager argument");
        return;
    }

    //iterate from the first texture to the last loaded texture and free them
    for (int texture_index = 0; texture_index < tm->loaded_texture_count; texture_index++){
        SDL_Texture* current_texture = tm->loaded_textures[texture_index];

        SDL_DestroyTexture(current_texture);
    }
}
