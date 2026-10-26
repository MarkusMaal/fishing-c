#ifndef PROFILE_H
#define PROFILE_H
#include "utils.h"
#include <time.h>

typedef enum
{
    EASY,
    NORMAL,
    HARD,
    SUPER_HARD
} Difficulty;

typedef enum
{
    R_BLUE,
    R_GREEN,
    R_AQUA,
    R_RED,
    R_PURPLE,
    R_GOLD,
    R_WHITE,
    R_GRAY,
    R_LBLUE,
    R_LIME,
    R_CYAN,
    R_LRED,
    R_MAGENTA,
    R_YELLOW,
} RColor;

typedef struct
{
    int music;     // play music
    int sfx;       // play sound effects
    int autosave;  // enable automatic saving of profile during gameplay
    int storytell; // display lore when playing a stage for the first time

    Difficulty difficulty;
    RColor rodColor;

    // 32 bytes
    char name[0x20];
} Options;

typedef enum
{
    FIND_ME_IF_YOU_CAN,
    READY,
    TRUE
} Salmon;

typedef struct
{
    char mapName[0x8];
    char mapUnlocks[0x10];
    char storiesTold[0x10]; // love this name lol
    int playing;
    int weight;
    int goalWeight;
    int totalLuck;
    int throws;
    char targets[20];
    Salmon solmn;
} Progression;

typedef struct
{
    char qName[0x20]; // only use for profile select menu
    char saveDate[0x20];
    char saveTime[0x20];
} ProfileHeader;

typedef struct
{
    ProfileHeader profileHeader;
    Progression progression;
    Options options;
    unsigned char spawnTable[0x190];
} PlayerProfile;

extern PlayerProfile profiles[8];
extern int activeProfile;

ProfileHeader quickLoadProfile(int idx);
int loadProfile(int idx);
void saveProfile(PlayerProfile profile, int idx);
void initProfile(int idx);
void saveActiveProfile();
void profileMenu();
unsigned int crc32(const char *dataPtr, size_t dataSize);

#endif