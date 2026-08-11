#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include "../core/animation.h"

//Component structs
typedef struct{ float x, y, z; } Vector3;
typedef struct{ float x, y; } Vector2;
typedef struct{ int hp; } Health;
typedef AnimationState Animation;
typedef struct { SDL_Texture* texture; SDL_Rect src; SDL_Rect dst; } Sprite; 

//Constant defines
typedef unsigned long long ComponentFlag;
#define HAS_POSITION  (1ULL << 0) //This would equal to 000...01 but 64-bit
#define HAS_VELOCITY  (1ULL << 1) //This would equal to 000...10 but 64-bit
#define HAS_HEALTH    (1ULL << 2)
#define HAS_ANIMATION (1ULL << 3)
#define HAS_SPRITE    (1ULL << 4)

#endif
