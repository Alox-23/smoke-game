#include "animation.h"

bool animation_init_clip(AnimationClip* ac, SDL_Texture* atlas){
    if (!ac){
        LOG_ERROR("Invalid AnimationClip argument");
return false;
    }
    if (!atlas){
        LOG_ERROR("Invalid SDL_Texture argument");
        return false;
    }

    ac->atlas = atlas;

    ac->frame_count = 0;

    ac->vertical_frames = false;

    return true;
}

bool animation_load_frames(AnimationClip* ac, Uint32 between_time, int frame_count, int frame_sizex, int frame_sizey, int frame_startx, int frame_starty){
    if (!ac){
        LOG_ERROR("Invalid AnimationClip argument");
        return false;
    }
    if (frame_count <= 0 || frame_sizex <= 0 || frame_sizey <=0){
        LOG_ERROR("Invalid frame dimensions and frame positions. Must be <= 0");
        return false;
    }
    if (frame_count > 32){
        LOG_ERROR("Inalid frame count. Must be < %d", MAX_ANIMATION_FRAMES);
        return false;
    }

    ac->between_time = between_time;
    ac->frame_count = frame_count;

    for (int i = 0; i < frame_count; i++){
        SDL_Rect r;
       
        if (ac->vertical_frames){
            r.x = frame_startx * frame_sizex;
            r.y = frame_starty * frame_sizey + i * frame_sizey;
        }
        else{
            r.x = frame_startx * frame_sizex + i * frame_sizex;
            r.y = frame_starty * frame_sizey;
        }

        r.h = frame_sizey;
        r.w = frame_sizex;

        ac->frames[i] = r; 
    }

    return true;
}

int animation_load_clip(AnimationState* as, AnimationClip* ac){
    if (!as || !ac){
        LOG_ERROR("Invalid AnimationState or AnimationClip");
        return INVALID_CLIP_INDEX;
    }

    as->clips[as->clip_count] = *ac;
    int index = as->clip_count;
    as->clip_count++;

    return index;
}

bool animation_play_clip(AnimationState* as, int clip_index){
    if (!as){
        LOG_ERROR("Invalid AnimationState");
        return false;
    }

    as->active_clip_index = clip_index;
    
    return true;
}

void animation_update_sate(AnimationState* as, float delat_time){ 
    if (!as){
        LOG_ERROR("Invalid AnimationState argument");
        return;
    }

    AnimationClip* ac = &as->clips[as->active_clip_index];
    Uint32 now = SDL_GetTicks();

    if (now - ac->last_time > ac->between_time){
        ac->current_frame_index++;
        ac->last_time = now; 
    }

    if (ac->current_frame_index > ac->frame_count-1){
        ac->current_frame_index = 0;
    }
    
}

SDL_Rect* animation_get_rect(AnimationState* as){
    if (!as){
        LOG_ERROR("Invalid AnimationState");
        return NULL;
    }

    AnimationClip* ac = &as->clips[as->active_clip_index];

    return &ac->frames[ac->current_frame_index];
}

SDL_Texture* animation_get_texture(AnimationState* as){
    if (!as){
        LOG_ERROR("Invalid AnimationState");
        return NULL;
    }

    AnimationClip ac = as->clips[as->active_clip_index];

    return ac.atlas;
}
