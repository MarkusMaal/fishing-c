#ifndef MAIN_H
#define MAIN_H
#include "menu.h"
#include "score.h"
#include "utils.h"

typedef enum
{
    NO_DEMO,
    SCORES,
    GAMEPLAY,
    SPLASH
} DemoState;

int main();
void titleScreen();
void splashScreen();
void demoPlay();

DemoState demo;

#endif