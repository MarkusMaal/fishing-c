#ifndef MAP_H
#define MAP_H
#include "menu.h"
#include "utils.h"
#include "fish.h"

typedef struct
{
    Record header;
    char name[0x10];
} Title;

typedef struct
{
    unsigned short count;
    Color color;
} LayoutChunk;

typedef struct
{
    Record header;
    LayoutChunk chunks[255];
} Layout;

typedef struct
{
    short y;
    short x;
    short w;
    short h;
} Water;

typedef enum
{
    WEIGHT,
    BOSS,
    TARGET
} GoalType;

typedef struct
{
    GoalType type;
    unsigned int grams;
    unsigned int variation;
    char targets[20];
} Goal;

typedef struct
{
    Record header;
    char filename[0x10];
} Music;

typedef struct
{
    Color bg;
    Color fg;
} GlobalColor;

typedef struct
{
    char msg[0x30];
    int idx;
    int x;
    int y;
    int padding;
} StMsgEntry;

typedef struct
{
    Record header;
    StMsgEntry messages[3];
} StatusMsg;

typedef struct
{
    int x;
    int y;
} Boat;

typedef struct
{
    Record header;
    char messages[99][0x10];
} Fish;

typedef struct
{
    Record header;
    char messages[99][0x10];
} Msg;

typedef struct
{
    Record header;
    char messages[99][0x100];
} Lore;

typedef struct
{
    Record header;
    unsigned char entries[0x330];
} ByteTable;

extern Title mapTitle;
extern Layout mapLayout;
extern Water mapWater;
extern Goal mapGoal;
extern Music mapMusic;
extern GlobalColor mapColor;
extern Fish mapFish;
extern StatusMsg mapStatus;
extern Boat mapBoat;
extern Msg mapMessage;
extern ByteTable mapSpawn;
extern ByteTable mapLuck;
extern Lore mapLore;

/**
 * Loads the map from a specified location to memory
 * @param fileName .map file to load from (must be full path including the file extension)
 * @note In order for the game to not crash, the map file must exist and it should have the following sections:
 *       TITLE, LAYOUT, WATER, GOAL, MUSIC, COLOR, FISH, STATUS, BOAT, MESSAGE, SPAWN, LUCK, LORE
 */
void loadMap(char *fileName);

/**
 * Draws the map with the rod tip at the specified location
 * @param x Rod tip location X value (accepted range: 1-20)
 * @param y Rod tip location Y value (accepted range: 1-20)
 */
void drawMap(int x, int y);

/**
 * Main game loop that runs while the map is being played
 * @param mapId ID for the currently played map (for storytelling purposes)
 */
void mapLoop(int mapId);

/**
 * Safely unload the map from memory
 */
void unloadMap();

/**
 * Exit map function. This should only be called as a pointer from the fishing menu
 */
void exitMap();

/**
 * Cast function. This should only be called as a pointer from the fishing menu
 * @param defaults Rod tip X/Y coordinates
 */
void cast(int *defaults);

/**
 * Display map lore to the user. This should only be called as a pointer from the fishing menu
 */
void loreDisplay();

#endif