#ifndef STATS_H
#define STATS_H
#include "utils.h"
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

typedef struct
{
    unsigned long totalWeight;
    unsigned int caughtFishCnt;
    unsigned int lostFishCnt;
    unsigned int totalLuck;
    unsigned int throwCount;
    unsigned int longestFish;
    unsigned int heaviestFish;
    unsigned int cricketCount;
    unsigned int popCount;
    unsigned int saveCount;
    unsigned long playTime;
    unsigned int biteCount;
    unsigned int lostFights;
    unsigned int wrongInputs;
    unsigned int correctInputs;
    unsigned int bootCount;
    unsigned int stageWinCount;
    unsigned int stageLoseCount;
    unsigned short mapPlayCnt[0x10];
} PlayerStats;

extern PlayerStats playerStats[8];

int loadStats(int idx);
void initStats(int idx);
void displayStats(int idx);
void saveStats(int idx);

#endif