#include "editor.h"

void editorMenu()
{
    if (loadMap("dev.map"))
        playSound("snd/load.mp3", false);
    int sel = 0;
    MenuItem menuItems[7] = {
        (MenuItem){BUTTON, "Map layout", &dummy, -1, NULL, &editorLayoutMenu},
        (MenuItem){BUTTON, "Objectives", &dummy, -1, NULL, &editorGoalMenu},
        (MenuItem){BUTTON, "Edit text", &dummy, -1, NULL, &editorTextMenu},
        (MenuItem){BUTTON, "Fish spawn rules", &dummy, -1, NULL, &editorSpawnMenu},
        (MenuItem){BUTTON, "Save map", &dummy, -1, NULL, &editorSaveMap},
        (MenuItem){BUTTON, "Test map", &dummy, -1, NULL, &testMap},
        (MenuItem){BUTTON, "Exit editor", &dummy, -1, NULL, &dummyFunc},
    };
    for (;;)
    {
        void (*menuPtr)(int *);
        menuPtr = printMenu("Map editor", menuItems, 7, &sel, (Color[]){BLACK, WHITE});
        if (sel == 6)
            break;
        menuPtr(&sel);
    }
}

void testMap()
{
    int backup = profiles[activeProfile].options.autosave;
    profiles[activeProfile].options.autosave = false;
    stopSound();
    char buf[64];
    snprintf(buf, 64, "snd/%s", mapMusic.filename[0]);
    playSound(buf, true);
    mapLoop(-1);
    chcolor((Color[]){BLACK, WHITE});
    for (int i = 0; i < 100; i++)
    {
        printf("%-100s", "");
    }
    locate(0, 0);
    stopSound();
    playSound("snd/menu_bg.mp3", true);
    profiles[activeProfile].options.autosave = backup;
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

    if (fwrite(&((Record){"GOAL", 0x14, 1, 2, 0}), sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapGoal, sizeof(Goal), 1, dMapFile) <= 0)
        goto saveErr;

    if (fwrite(&mapMusic.header, sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapMusic.filename, mapMusic.header.chunkSize * mapMusic.header.chunkCount, 1, dMapFile) <= 0)
        goto saveErr;

    if (fwrite(&((Record){"COLOR", 1, 2, 1, 0}), sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapColor, sizeof(mapColor), 1, dMapFile) <= 0)
        goto saveErr;
    fseek(dMapFile, 12, SEEK_CUR);

    if (fwrite(&mapFish.header, sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapFish.messages, mapFish.header.chunkSize * mapFish.header.chunkCount, 1, dMapFile) <= 0)
        goto saveErr;

    if (fwrite(&mapStatus.header, sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapStatus.messages, mapStatus.header.chunkSize * mapStatus.header.chunkCount, 1, dMapFile) <= 0)
        goto saveErr;

    if (fwrite(&((Record){"BOAT", 16, 1, 1, 0}), sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapBoat, sizeof(Boat), 1, dMapFile) <= 0)
        goto saveErr;
    fseek(dMapFile, 8, SEEK_CUR);

    if (fwrite(&mapMessage.header, sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapMessage.messages, mapMessage.header.chunkSize * mapMessage.header.chunkCount, 1, dMapFile) <= 0)
        goto saveErr;

    if (fwrite(&mapSpawn.header, sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapSpawn.entries, mapSpawn.header.chunkSize * mapSpawn.header.chunkCount, 1, dMapFile) <= 0)
        goto saveErr;

    if (fwrite(&mapLuck.header, sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapLuck.entries, mapLuck.header.chunkSize * mapLuck.header.chunkCount, 1, dMapFile) <= 0)
        goto saveErr;

    if (fwrite(&mapLore.header, sizeof(Record), 1, dMapFile) <= 0)
        goto saveErr;
    if (fwrite(&mapLore.messages, mapLore.header.chunkSize * mapLore.header.chunkCount, 1, dMapFile) <= 0)
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
        (MenuItem){CHOICE, "Color", &mapLayout.chunks[*selectedBlock].color, 16, colorChoices, &dummyFunc},
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
    char **colorChoices = (char *[]){"Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White", "Gray", "Light red", "Light green", "Light yellow", "Light blue", "Light magenta", "Light cyan", "Bright white"};
    int l = *sel;
    int *selectedBlock = malloc(4);
    *selectedBlock = 0;
    MenuItem menuItems[10] = {
        (MenuItem){SELECTOR, "Blocks", &mapLayout.header.chunkCount, 255, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Selected block", (short *)selectedBlock, 255, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Boat X", (short *)&mapBoat.x, 255, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Boat Y", (short *)&mapBoat.y, 255, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Edit block", &dummy, -1, NULL, &editorEditBlock},
        (MenuItem){BUTTON, "Edit water", &dummy, -1, NULL, &editorEditWater},
        (MenuItem){CHOICE, "Background color", &mapColor.bg, 16, colorChoices, &dummyFunc},
        (MenuItem){CHOICE, "Foreground color", &mapColor.fg, 16, colorChoices, &dummyFunc},
        (MenuItem){BUTTON, "Preview", &dummy, -1, NULL, &editorDrawLayout},
        (MenuItem){BUTTON, "Return to previous menu", &dummy, -1, NULL, &dummyFunc},
    };
    for (;;)
    {
        void (*menuPtr)(int *);
        menuPtr = printMenu("Map editor > Map layout", menuItems, 10, sel, (Color[]){BLACK, WHITE});
        if (*sel == 9)
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

char *editStringMenu(char *stringPtr, int chunkSize, char *topMsg, int multiline)
{
    clearScreen();
    printf("\nMap editor > Edit text > %s\n\nCurrent: %s\n\n", topMsg, stringPtr);
    int nI = 0;
    char *c = malloc(1);
    *c = '\0';
    char *lastName = malloc(chunkSize);
    strcpy(lastName, stringPtr);
    do
    {
        scanf("%c", c);
        if (*c == 8 && nI > 0)
        {
            lastName[nI] = '\0';
            lastName[nI - 1] = '\0';
            nI--;
        }
        if (nI < chunkSize - 1 && *c != 8)
        {
            lastName[nI] = (multiline || *c != '\n') ? *c : '\0';
            nI++;
        }
    } while (!multiline ? *c != '\n' : *c != 27);
    strcpy(stringPtr, lastName);
    free(c);
    return lastName;
}

void editorStatusTextMenu(int *sel, int idx)
{
    int l = *sel;
    *sel = 0;
    char **choices = (char *[]){"Total fish and precentage", "Goal \\w kilograms", "Enigmatic message", "Total fish \\w decimal points and percentage", "Remaining fish", "Percentage"};
    char **colorChoices = (char *[]){"Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White", "Gray", "Light red", "Light green", "Light yellow", "Light blue", "Light magenta", "Light cyan", "Bright white"};
    MenuItem menuItems[7] = {
        (MenuItem){CHOICE, "Message type", (short *)&mapStatus.messages[idx].idx, 5, choices, &dummyFunc},
        (MenuItem){SELECTOR, "Position X", (short *)&mapStatus.messages[idx].x, 32767, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Position Y", (short *)&mapStatus.messages[idx].y, 32767, NULL, &dummyFunc},
        (MenuItem){CHOICE, "Background color", &mapStatus.messages[idx].bg, 16, colorChoices, &dummyFunc},
        (MenuItem){CHOICE, "Foreground color", &mapStatus.messages[idx].fg, 16, colorChoices, &dummyFunc},
        (MenuItem){BUTTON, "Edit text", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Go back", &dummy, -1, NULL, &dummyFunc},
    };
    for (;;)
    {
        printMenu("Map editor > Edit text > Edit status message", menuItems, 7, sel, (Color[]){BLACK, WHITE});
        if (*sel == 5)
            editStringMenu((char *)&mapStatus.messages[idx].msg, 48, "Edit text", false);
        else if (*sel == 6)
        {
            *sel = l;
            break;
        }
    }
}

void editorTextMenu(int *sel)
{
    int l = *sel;
    *sel = 0;
    short *statusMessageIdx = malloc(2);
    *statusMessageIdx = 0;
    MenuItem menuItems[9] = {
        (MenuItem){BUTTON, "Edit lore", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Edit title", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Edit music", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){BUTTON, mapMusic.header.chunkCount == 1 ? "Add boss music" : "Edit boss music", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Status message count", &mapStatus.header.chunkCount, 32767, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Selected message", statusMessageIdx, 32767, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Edit status message", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Edit enigmatic message", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Go back", &dummy, -1, NULL, &dummyFunc},
    };
    for (;;)
    {
        printMenu("Map editor > Edit text", menuItems, 9, sel, (Color[]){BLACK, WHITE});
        switch (*sel)
        {
        case 0:
            if (mapLore.header.chunkCount - 1 < *statusMessageIdx)
                mapLore.header.chunkCount = *statusMessageIdx + 1;
            editStringMenu((char *)&mapLore.messages[*statusMessageIdx], mapLore.header.chunkSize, "Edit lore", true);
            break;
        case 1:
            editStringMenu((char *)&mapTitle.name, 0x20, "Edit title", false);
            mapTitle.header.chunkSize = (strlen(mapTitle.name) % 0x10 + 1) * 0x10;
            break;
        case 2:
            editStringMenu((char *)&mapMusic.filename, mapMusic.header.chunkSize, "Edit music", false);
            break;
        case 3:
            mapMusic.header.chunkCount = 2;
            editStringMenu((char *)&mapMusic.filename[1], mapMusic.header.chunkSize, "Edit boss music", false);
            break;
        case 6:
            editorStatusTextMenu(sel, *statusMessageIdx);
            break;
        case 7:
            if (mapMessage.header.chunkCount - 1 < *statusMessageIdx)
                mapMessage.header.chunkCount = *statusMessageIdx + 1;
            editStringMenu((char *)&mapMessage.messages[*statusMessageIdx], mapMessage.header.chunkSize, "Edit enigmatic message", false);
            break;
        default:
            break;
        }
        if (*sel == 8)
        {
            *sel = l;
            break;
        }
    }
    free(statusMessageIdx);
}

void editorSpawnGen(int *sel)
{
    int l = *sel;
    int rng = 0;
    *sel = 0;
    short *probabilityTable = malloc(2 * (mapFish.header.chunkCount + 1));
    for (int i = 0; i < mapFish.header.chunkCount + 1; i++)
        probabilityTable[i] = 0;
    MenuItem *menuItems = malloc(sizeof(MenuItem) * (mapFish.header.chunkCount + 4));

    for (int i = 0; i < mapFish.header.chunkCount; i++)
    {
        menuItems[i] = (MenuItem){SELECTOR, mapFish.messages[i], &probabilityTable[i], 100, NULL, &dummyFunc};
    }
    menuItems[mapFish.header.chunkCount] = (MenuItem){SELECTOR, "No fish", &probabilityTable[mapFish.header.chunkCount], 100, NULL, &dummyFunc};
    menuItems[mapFish.header.chunkCount + 1] = (MenuItem){BUTTON, "Generate with these probabilities", &dummy, -1, NULL, &dummyFunc};
    menuItems[mapFish.header.chunkCount + 2] = (MenuItem){BUTTON, "Generate luck table", &dummy, -1, NULL, &dummyFunc};
    menuItems[mapFish.header.chunkCount + 3] = (MenuItem){BUTTON, "Return to previous menu", &dummy, -1, NULL, &dummyFunc};

    for (;;)
    {
        printMenu("Map editor > Fish spawn rules > Generate spawn table\n\nThe RNG will choose a number between 1-100 for each spot. In here you can\nspecify starting which number a particular fish should spawn. The numbers\nMUST be in ascending order.", menuItems, mapFish.header.chunkCount + 4, sel, (Color[]){BLACK, WHITE});
        if (*sel == mapFish.header.chunkCount + 3)
        {
            *sel = l;
            break;
        }
        else if (*sel == mapFish.header.chunkCount + 2)
        {
            *sel = l;
            for (int i = 0; i < 400; i++)
            {
                mapLuck.entries[i] = rand() % 255;
            }
            break;
        }
        else if (*sel == mapFish.header.chunkCount + 1)
        {
            // generation code here
            for (int i = 0; i < 400; i++)
            {
                rng = rand() % 100;
                int maxId = -1;
                for (int j = 0; j < mapFish.header.chunkCount + 1; j++)
                {
                    if (probabilityTable[j] >= rng)
                    {
                        maxId = j;
                        break;
                    }
                }
                if (maxId >= mapFish.header.chunkCount)
                    maxId = -1;
                mapSpawn.entries[i] = maxId;
            }
            *sel = l;
            break;
        }
    }
    free(menuItems);
    free(probabilityTable);
}

void editorSpawnMenu(int *sel)
{
    int l = *sel;
    *sel = 0;
    short *fishIdx = malloc(2);
    *fishIdx = 0;
    MenuItem menuItems[6] = {
        (MenuItem){SELECTOR, "Fish count", &mapFish.header.chunkCount, 99, NULL, &dummyFunc},
        (MenuItem){SELECTOR, "Selected fish", fishIdx, 99, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Generate spawn/luck tables", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Display spawn/luck tables", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Specify fish file", &dummy, -1, NULL, &dummyFunc},
        (MenuItem){BUTTON, "Go back", &dummy, -1, NULL, &dummyFunc},
    };
    for (;;)
    {
        printMenu("Map editor > Fish spawn rules", menuItems, 6, sel, (Color[]){BLACK, WHITE});
        switch (*sel)
        {
        case 2:
            editorSpawnGen(sel);
            break;
        case 3:
            clearScreen();
            printf("\nMap editor > Fish spawn rules > Fish spawn table\n\n");
            for (int i = 0; i < 400; i++)
            {
                printf("%-4d", mapSpawn.entries[i]);
                if ((i + 1) % 20 == 0)
                    printf("\n");
            }
            printf("\nPress any key to continue . . . ");
            readKey(false);
            clearScreen();
            printf("\nMap editor > Fish spawn rules > Fish luck table\n\n");
            for (int i = 0; i < 400; i++)
            {
                printf("%-4d", mapLuck.entries[i]);
                if ((i + 1) % 20 == 0)
                    printf("\n");
            }
            printf("\nPress any key to continue . . . ");
            readKey(false);
            break;
        case 4:
            editStringMenu(mapFish.messages[*fishIdx], mapFish.header.chunkSize, "Specify fish file", false);
            break;
        default:
            break;
        }
        if (*sel == 5)
        {
            *sel = l;
            break;
        }
    }
}