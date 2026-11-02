#include "editor.h"

void editorMenu()
{
    loadMap("dev.map");
    int sel = 0;
    MenuItem menuItems[6] = {
        (MenuItem){BUTTON, "Map layout", &dummy, -1, NULL, &editorLayoutMenu},
        (MenuItem){BUTTON, "Objectives", &dummy, -1, NULL, &editorGoalMenu},
        (MenuItem){DISABLED_BUTTON, "Edit text", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){DISABLED_BUTTON, "Fish spawn rules", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Save map", &dummy, -1, NULL, &editorSaveMap},
        (MenuItem){BUTTON, "Exit editor", &dummy, -1, NULL, &dummyFunc},
    };
    for (;;)
    {
        void (*menuPtr)(int *);
        menuPtr = printMenu("Map editor", menuItems, 6, &sel, (Color[]){BLACK, WHITE});
        if (sel == 5)
            break;
        menuPtr(&sel);
    }
}

void editorSaveMap()
{
    int totalCnt;
    FILE *dMapFile;

    dMapFile = fopen("dev.map", "wb");
    if (dMapFile == NULL)
        goto saveErr;
    totalCnt = strlen(mapTitle.name);
    while (totalCnt % 0x10 != 0)
        totalCnt++;
    mapTitle.header.chunkSize = totalCnt;
    mapTitle.header.total = totalCnt / 0x10;
    if (fwrite(&mapTitle.header, sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapTitle.name, totalCnt, 1, dMapFile) <= 0)
        goto saveErr;
    totalCnt = mapLayout.header.chunkSize * mapLayout.header.chunkCount;
    while (totalCnt % 0x10 != 0)
    {
        totalCnt++;
    }
    mapLayout.header.total = totalCnt / 0x10;
    if (fwrite(&mapLayout.header, sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapLayout.chunks, totalCnt, 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&((Record){"WATER", 4, 1, 1, 0}), sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapWater, sizeof(Water), 1, dMapFile) <= 0)
        goto saveErr;
    fseek(dMapFile, 8, SEEK_CUR);

    if (fwrite(&((Record){"BOAT", 16, 1, 1, 0}), sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapBoat, sizeof(Boat), 1, dMapFile) <= 0)
        goto saveErr;
    fseek(dMapFile, 8, SEEK_CUR);
    if (fwrite(&((Record){"GOAL", 0x14, 1, 2, 0}), sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapGoal, sizeof(Goal), 1, dMapFile) <= 0)
        goto saveErr;

    playSound("snd/save.mp3", false);
    fclose(dMapFile);
    return;
saveErr:
    clearScreen();
    printf("\nFatal error\n\nFailed to save map!\n\nPress any key to continue . . . ");
    readKey(false);
    fclose(dMapFile);
}

void editorDrawLayout()
{
    clearScreen();
    drawMap(-1, -1);
    chcolor((Color[]){BLACK, WHITE});
    printf("\n\nPreview with water boundaries\n");
    printf("\nPress any key to continue . . . \n");
    readKey(false);
    clearScreen();
    int x = rand() % 20 + 1;
    int y = rand() % 20 + 1;
    drawMap(x, y);
    chcolor((Color[]){BLACK, WHITE});
    printf("\n\nPreview with throw (%d, %d)\n", x, y);
    printf("\nPress any key to continue . . . \n");
    readKey(false);
}

void editorEditBlock(int *selectedBlock)
{
    char **colorChoices = (char *[]){"Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White", "Gray", "Light red", "Light green", "Light yellow", "Light blue", "Light magenta", "Light cyan", "Bright white"};
    MenuItem menuItems[3] = {
        (MenuItem){CHOICE, "Color", &mapLayout.chunks[*selectedBlock].color, 15, colorChoices, &dummyFunc},
        (MenuItem){SELECTOR, "Repeat count", (short *)&mapLayout.chunks[*selectedBlock].count, 32767, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Return to previous menu", &dummy, -1, NULL, &dummyFunc},
    };
    int sel = 0;
    for (;;)
    {
        char title[64];
        snprintf(title, 64, "Map editor > Map layout > Editing block %d", *selectedBlock);
        printMenu(title, menuItems, 3, &sel, (Color[]){BLACK, WHITE});
        if (sel == 2)
            break;
    }
}

void editorEditWater()
{
    MenuItem menuItems[5] = {
        (MenuItem){SELECTOR, "X", &mapWater.x, 32767, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Y", &mapWater.y, 32767, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Width", &mapWater.w, 32767, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Height", &mapWater.h, 32767, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Return to previous menu", &dummy, -1, NULL, &dummyFunc},
    };
    int sel = 0;
    for (;;)
    {
        printMenu("Map editor > Map layout > Editing water", menuItems, 5, &sel, (Color[]){BLACK, WHITE});
        if (sel == 4)
            break;
    }
}

void editorLayoutMenu(int *sel)
{
    int l = *sel;
    int *selectedBlock = malloc(4);
    *selectedBlock = 0;
    MenuItem menuItems[8] = {
        (MenuItem){SELECTOR, "Blocks", &mapLayout.header.chunkCount, 255, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Selected block", (short *)selectedBlock, 255, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Boat X", (short *)&mapBoat.x, 255, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Boat Y", (short *)&mapBoat.y, 255, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Edit block", &dummy, -1, NULL, &editorEditBlock},
        (MenuItem){BUTTON, "Edit water", &dummy, -1, NULL, &editorEditWater},
        (MenuItem){BUTTON, "Preview", &dummy, -1, NULL, &editorDrawLayout},
        (MenuItem){BUTTON, "Return to previous menu", &dummy, -1, NULL, &dummyFunc},
    };
    for (;;)
    {
        void (*menuPtr)(int *);
        menuPtr = printMenu("Map editor > Map layout", menuItems, 8, sel, (Color[]){BLACK, WHITE});
        if (*sel == 7)
        {
            *sel = l;
            break;
        }
        menuPtr(*sel == 4 ? selectedBlock : sel);
    }
    free(selectedBlock);
}

void editorGoalMenu(int *sel)
{
    int l = *sel;
    short *mapWeight = malloc(2);
    char **typeChoices = (char *[]){"Weight", "Boss", "Target", "Sandbox"};
    *mapWeight = mapGoal.grams / 1000;
    MenuItem menuItems[4] = {
        (MenuItem){CHOICE, "Goal type", (short *)&mapGoal.type, 4, typeChoices, &dummyFunc},
        (MenuItem){SELECTOR, "Kilograms", mapWeight, 32767, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Variation", (short *)&mapGoal.variation, 32767, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Return to previous menu", &dummy, -1, NULL, &dummyFunc},
    };
    for (;;)
    {
        void (*menuPtr)(int *);
        menuPtr = printMenu("Map editor > Objectives", menuItems, 4, sel, (Color[]){BLACK, WHITE});
        if (*sel == 3)
        {
            *sel = l;
            break;
        }
        menuPtr(sel);
    }
    mapGoal.grams = *mapWeight * 1000;
    free(mapWeight);
}