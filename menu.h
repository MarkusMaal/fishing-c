#ifndef MENU_DEV_H
#define MENU_DEV_H
#include "utils.h"
#include "map.h"
#include "profile.h"
extern int enableDevMenu;

/**
 * Return to previous menu
 * @param sel Menu selection pointer, which will get restored to previous value
 * @note If called from the root menu, this will exit the game instead
 */
void ret(int *idx);

/**
 * Loads a specified map
 * @param sel Selected map index
 */
void playMap(int *sel);

/**
 * Test menu (subset of developer menus)
 * @param sel Previous menu selection pointer
 */
void menuTest(int *sel);

/**
 * Developer menu
 * @param sel Previous menu selection pointer
 */
void devMenu(int *sel);

/**
 * Dummy screen displaying the selected menu item
 * @param sel Previous menu selection pointer
 */
void testItemEntry(int *sel);

/**
 * "Start game" menu where the user can choose a map to play
 * @param sel Previous menu selection pointer
 */
void startMenu(int *sel);

/**
 * The main menu
 */
void rootMenu();

/**
 * Displays a fishing menu, where the user can choose where to throw, save the game, quit the game, etc.
 *
 * @param title Text to show at the top of the menu
 * @param defaults Default values throwing x/y values (2 ints)
 * @param fCast Casting function pointer with signature `void fn(int *values)`
 * @param fLore Lore function pointer with signature `void fn(int *values)`
 * @param fExit Exit function pointer with signature `void fn(int *values)`
 * @param colors Background and foreground color of the map (required for rendering grayed out menu items properly)
 */
int fishingMenu(char *title, int *defaults, void *fCast, void *fLore, void *fExit, Color *colors);

void keycodeTester();
void soundTest();
void musicTest();
void actionTester(int *sel);
void showGlobalVariables();
void testFishingMenu(int *sel);
void testFishBattle();
void settingsMenu(int *sel);
void dummyFunc();
void toggleSetting(int *sel);
void nameEntry();
void devUnlock();
void deDev(int *sel);
#endif