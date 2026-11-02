#ifndef EDITOR_H
#define EDITOR_H
#include "map.h"
#include "utils.h"

void editorMenu();
void editorSaveMap();
void editorDrawLayout();
void editorEditWater();
void editorEditBlock(int *selectedBlock);
void editorLayoutMenu(int *sel);
void editorGoalMenu(int *sel);

#endif