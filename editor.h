#ifndef EDITOR_H
#define EDITOR_H
#include "map.h"
#include "utils.h"

void editorMenu();
void testMap();
void editorSaveMap();
void editorDrawLayout();
void editorEditWater();
void editorEditBlock(int *selectedBlock);
void editorLayoutMenu(int *sel);
void editorGoalMenu(int *sel);
void editorTextMenu(int *sel);
void editorStatusTextMenu(int *sel, int idx);
char *editStringMenu(char *stringPtr, int chunkSize, char *topMsg, int multiline);
void editorSpawnGen(int *sel);
void editorSpawnMenu(int *sel);
#endif