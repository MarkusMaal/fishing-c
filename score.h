#ifndef ARCADE_H
#define ARCADE_H
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include "utils.h"

typedef struct
{
    char name[0x20];
    unsigned int score;
} ScoreEntry;

extern ScoreEntry scoreboard[5];

void getScores();
void initScores();
void trySaveScores();
void showScores();
void checkScorePush(unsigned int newScore);

#endif