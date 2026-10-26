
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for STDIN_FILENO */
#include <termios.h>
#include <string.h>
#include "simpleaudio.h"
#include <pthread.h>
#include <signal.h>
typedef struct
{
    char name[20];
    char file[8];
    int idx;
    int secret;
    char tip[0x50];
} MapsDef;

typedef struct
{
    char label[8];
    short chunkSize;
    short chunkCount;
    short total;
    short lastMarker;
} Record;

typedef enum
{
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    LBLACK,
    LRED,
    LGREEN,
    LYELLOW,
    LBLUE,
    LMAGENTA,
    LCYAN,
    LWHITE,
} Color;

typedef enum
{
    BUTTON,
    DISABLED_BUTTON,
    CHOICE,
    SELECTOR,
    CHECKBOX,
    SLIDER
} MenuClass;

typedef struct
{
    MenuClass cl;
    char *text;
    int *value;
    int maxValue;
    char **choices;
    void *action;
} MenuItem;

typedef struct
{
    t_sound *sound;
    char *fileName;
} LoopArgs;

extern MapsDef *mapsList;
extern int mapsCount;
extern t_sound *bgm;
extern pthread_t bgmThread;
extern volatile int thread_exit;
extern int dummy;

/**
 * Reads a key from the user and returns it as a string value
 * @returns Either numeric string or descriptive string depending on the kind of key pressed
 */
char *readKey();

/**
 * Prints the string with the specified background and foreground color
 * @param text String to print
 * @param colors Background and foreground colors
 */
void cprintf(char *text, Color *colors);

/**
 * Clears everything that was previously on the screen
 */
void clearScreen();

/**
 * Displays a specified menu
 * @param title Menu title (shown at the top)
 * @param items Menu items array
 * @param max The number of menu items to show (should be equal to the length of items in most cases)
 * @param sel Selected item pointer
 * @param defaultColors Current background/foreground color (used for restoring the color when displaying grayed out menu items)
 */
void *printMenu(char *title, MenuItem *items, int max, int *sel, Color *defaultColors);

/**
 * Initializes the sound engine. Should only be called once during runtime.
 */
void initSoundEngine();

/**
 * Unloads the sound engine. Should only be called when closing the program.
 */
void unloadSoundEngine();

/**
 * Plays the specified sound file
 * @param filename Full path to sound file
 * @param loop Whether or not this sound should be looped (mainly useful for background music)
 */
void playSound(char *filename, char loop);

/**
 * Internal thread function for looping sound playback. Do not call this function from the main thread!
 * @param arg Full path to sound file
 */
void *loopSound(void *arg);

/**
 * Stops the looping sound
 */
void stopSound();

/**
 * Loads maps/maps.def file to memory (metadata for maps for displaying them in the start menu)
 */
void getMaps();

/**
 * Re-locate the text cursor to the specified position
 */
void locate(int row, int col);

/**
 * Find and replace
 * @param str haystack
 * @param find needle
 * @param replace replacement string
 */
char *strrpl(char *str, char *find, char *replace);

#endif