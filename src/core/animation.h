#ifndef CORE_ANIMATION
#define CORE_ANIMATION

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "texture_manager.h"
#include "log.h"

#define MAX_ANIMATION_FRAMES 32
#define MAX_ANIMATION_CLIPS 32
#define MAX_ANIMATION_CLIP_NAME_CHARS 128
#define INVALID_CLIP_INDEX UINT32_MAX

typedef struct{
    SDL_Texture* atlas;
    SDL_Rect frames[MAX_ANIMATION_FRAMES];
    int frame_count;
    int current_frame_index;

    Uint32 last_time;
    Uint32 between_time;

    bool vertical_frames;
} AnimationClip;

typedef struct{
    AnimationClip clips[MAX_ANIMATION_CLIPS];
    uint32_t active_clip_index;
    int clip_count;
} AnimationState;

bool animation_init_clip(AnimationClip* ac, SDL_Texture* atlas);
bool animation_load_frames(AnimationClip* ac, Uint32 between_time, int frame_count, int frame_sizex, int frame_sizey, int frame_startx, int frame_starty);
int animation_load_clip(AnimationState* as, AnimationClip* ac);
bool animation_play_clip(AnimationState* as, int clip_index);
void animation_update_sate(AnimationState* as, float delat_time);
SDL_Rect* animation_get_rect(AnimationState* as);
SDL_Texture* animation_get_texture(AnimationState* as);

#endif
