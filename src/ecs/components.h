#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

//Component structs
typedef struct{ float x, y, z; } Position;
typedef struct{ float dx, dy, dz; } Velocity;
typedef struct{ int hp; } Health;

//Constant defines
typedef unsigned long long ComponentFlag;
#define HAS_POSITION (1ULL << 0) //This would equal to 000...01 but 64-bit
#define HAS_VELOCITY (1ULL << 1) //This would equal to 000...10 but 64-bit
#define HAS_HEALTH   (1ULL << 2)

#endif
