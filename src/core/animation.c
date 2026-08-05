#include "animation.h"
#include <string.h>

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
    ac->freeze_frame = -1;

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

int animation_load_clip(AnimationState* as, AnimationClip ac, char* name){
    if (!as){
        LOG_ERROR("Invalid AnimationState or AnimationClip");
        return INVALID_CLIP_INDEX;
    }

    as->clips[as->clip_count] = ac;
    as->clip_names[as->clip_count] = name;
    int index = as->clip_count;
    as->clip_count++;

    return index;
}

bool animation_play_clip(AnimationState* as, int clip_index){
    if (!as){
        LOG_ERROR("Invalid AnimationState");
        return false;
    }
    if (clip_index < 0 || clip_index >= as->clip_count){
        LOG_ERROR("clip_index %d out of range (clip_count %d)",
                  clip_index, as->clip_count);
        return false;
    }

    if (as->active_clip_index == clip_index) return true;   /* already playing, don't restart */

    AnimationClip* ac = &as->clips[clip_index];
    ac->current_frame_index = 0;
    ac->last_time = SDL_GetTicks();

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

    if (ac->current_frame_index == ac->freeze_frame){
        //current frame is a freeze frame, so no update needs to happen
        if (as->unfreeze); //if we should unfreeze current frame then we dont terminate this function
        else return;
    }

    else {
        as->unfreeze = false; //just making sure its false when no freeze happened :)
    }

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

int animation_get_id_by_name(AnimationState* as, char* name){
    if (!as){
        LOG_ERROR("Invalid AnimationState argument");
        return INVALID_CLIP_INDEX;
    }

    for (int i = 0; i < as->clip_count; i++){
        if (strcmp(as->clip_names[i], name) == 0){
            return i; //found the index of the animation with the correct name 
        }
    }

    return INVALID_CLIP_INDEX;
}


bool animation_set_freeze_on_frame(AnimationState* as, char* clip_name, int frame){
    if (!as || !clip_name){
        LOG_ERROR("Invalid AnimationState or clip_name is invalid");
        return false;
    }

    int id = animation_get_id_by_name(as, clip_name);

    as->clips[id].freeze_frame = frame; //set that clips freeze frame to the value passed to the func

    return true;
}

bool animation_unfreeze(AnimationState* as){
    if (!as){
        LOG_ERROR("Invalid AnimationState argument");
        return false;
    }

    if (!as->unfreeze){
         as->unfreeze = true; //only unfreeze the fram if its actualy frozen
        LOG_INFO("Unfreezing frame...");
    }
    return true;
}
