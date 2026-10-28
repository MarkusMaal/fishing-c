#ifndef FISH_H
#define FISH_H
#include "utils.h"

typedef enum
{
    LEFT,
    RIGHT
} MoveDirection;

typedef enum : char
{
    C,
    F,
    T,
    W,
    A,
    S,
    D,
    N,
    RANDOM,
    LAST,
    ANY,
    NONE,
    BEFORELAST
} Input;

typedef enum : char
{
    FORW1,
    FORW2,
    FORW3,
    UP1,
    UP2,
    UP3,
} MovementPattern;

typedef enum
{
    COMMON,
    UNCOMMON,
    SEMIRARE,
    RARE,
    LEGENDARY,
    ONE_OF_KIND
} RarityLevel;

typedef struct
{
    char name[0x18];
    int spawnCount;
    RarityLevel rarity;
    char spawnMaps[0x10];
} LivewellData;

typedef enum
{
    P_EASY,
    P_NORMAL,
    P_HARD,
    P_SUPER_HARD
} PlayerDifficulty;

typedef struct
{
    char geometry[4][0x10][0x40];
    char attackPatternGeometry[0x40];
    Input attackPatternInputs[0x40];
    char attackPatternSteps[0x40];
    MoveDirection attackDir;
    int fightLength;
    int cycleStart;
    int cycleLuck;
    int cycleLength;
    int spawnX;
    int spawnY;
    int weightMin;
    int weightMax;
    int lengthMin;
    int lengthMax;
    short largeUnits;
    short tolerance;
} FishData;

FishData loadFish(char *fileName);
int *fightFish(FishData fish, int luck, PlayerDifficulty difficulty);
LivewellData loadFishMeta(char *fileName);
#endif