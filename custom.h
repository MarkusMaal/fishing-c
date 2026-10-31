#ifndef CUSTOM_H
#define CUSTOM_H
#include "utils.h"
#include "map.h"

typedef struct
{
    char fishFile[0x20];
    char fishName[0x20];
    int enabled;
} FishToggle;

typedef struct
{
    int arcadeRules;
    int goalWeight;
    int environmentId;
    int throwCount;
    FishToggle fishToggles[99];
} CustomStage;

/**
 * Loads a custom stage and plays it
 * @param cust Custom stage data (settings on the custom game menu)
 * @param map Full path to the map file (e.g. maps/blake.map)
 * @param environment Human-friendly name of the map (e.g. Beginner's Lake)
 */
void cmLoader(CustomStage cust, char *map, char *environment);

#endif