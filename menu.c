#include "menu.h"

int ex = 0;
int last = 0;

void ret(int *sel)
{
    ex++;
    *sel = last;
    if (ex >= 1)
    {
        clearScreen();
        printf("Sad to see you go!\n");
        stopSound();
        sleep(2);
        unloadSoundEngine();
    }
}

void dummyFunc()
{
}

void devUnlock()
{
    for (int i = 0; i < 0x10; i++)
    {
        profiles[activeProfile].progression.mapUnlocks[i] |= 0x01;
    }
}

void actionTester(int *sel)
{
    clearScreen();
    ex--;
    int checkEx = ex + 1;
    last = *sel;
    *sel = 0;
    while (ex < checkEx)
    {
        MenuItem *menu = (MenuItem[]){
            (MenuItem){BUTTON, "Fishing menu", &dummy, -1, NULL, &testFishingMenu},
            (MenuItem){BUTTON, "Battle test", &dummy, -1, NULL, &testFishBattle},
            (MenuItem){BUTTON, "100%% unlock", &dummy, -1, NULL, &devUnlock},
            (MenuItem){BUTTON, "Return to previous menu", &dummy, -1, NULL, &ret},
        };
        void (*menuPtr)(int *);
        menuPtr = printMenu("Fishing C > Main menu > Developer > Action test", menu, 4, sel, (Color[]){BLACK, WHITE});
        menuPtr(sel);
    }
}

void keycodeTester()
{
    clearScreen();
    printf("\nKeycode reader (press RETURN to exit)\n\nPress any key...");
    while (true)
    {
        char *keycode = readKey();
        if (strcmp(keycode, "return") == 0)
            break;
        clearScreen();
        printf("\nKeycode reader (press RETURN to exit)\n\nCode: %s", keycode);
    }
}

void testFishingMenu(int *sel)
{
    fishingMenu("Fishing C > Main menu > Developer > Action test > Fishing menu", (int[]){*sel, dummy}, testItemEntry, dummyFunc, dummyFunc, (Color[]){BLACK, WHITE});
}

void testFishBattle()
{
    char buff[128];
    clearScreen();
    printf("\nPlease provide full path to .fsh file: ");
    if (fgets(buff, 128, stdin) == NULL)
        return;
    if (buff[strlen(buff) - 1] != '\n')
        return;
    buff[strlen(buff) - 1] = '\0'; // remove newline
    FishData fish = loadFish(buff);
    int *results = fightFish(fish, 100);
    clearScreen();

    printf("\nResults:\n\nWeight: %d g\nSize: %d cm\n\nPress any key to continue . . . ", fish.largeUnits ? *results * 1000 : *results, results[1]);
    readKey();
}

void showGlobalVariables()
{
    clearScreen();
    printf("\nFishing C > Main menu > Developer > Variables\n\nGlobal variable listing:\n\n");
    printf("Maps list:\n");
    for (int i = 0; i < mapsCount; i++)
    {
        printf("   %s (%s.map)\n", mapsList[i].name, mapsList[i].file);
        printf("     ID: %d (%s)\n", mapsList[i].idx, mapsList[i].secret ? "secret" : "not secret");
        printf("     Loading screen tip: %ld character(s) long\n", strlen(mapsList[i].tip));
    }
    printf("Miscellaneous:\n");
    printf("   Thread exit: %d\n", thread_exit);
    printf("   Dummy value: %d\n", dummy);
    printf("   BGM ptr: 0x%lx\n", (long)bgm);
    printf("   BGM thread ptr: 0x%lx\n", (long)bgmThread);

    printf("\nPress any key to continue . . . ");
    readKey();
    clearScreen();
    printf("\nFishing C > Main menu > Developer > Variables\n\nGlobal variable listing:\n\n");
    printf("Active profile:\n");
    printf("   ID: %d\n", activeProfile);
    printf("   Name: %s\n", profiles[activeProfile].options.name);
    printf("   Last save: %s @ %s\n", profiles[activeProfile].profileHeader.saveDate, profiles[activeProfile].profileHeader.saveTime);
    printf("\nPress any key to continue . . . ");
    readKey();
    printf("\n");
}

void soundTest()
{
    int max = 11;
    char *songs[max];
    songs[0] = "snd/click.mp3";
    songs[1] = "snd/click_2.mp3";
    songs[2] = "snd/crickets.mp3";
    songs[3] = "snd/error.mp3";
    songs[4] = "snd/load.mp3";
    songs[5] = "snd/negative.mp3";
    songs[6] = "snd/pop.mp3";
    songs[7] = "snd/positive.mp3";
    songs[8] = "snd/save.mp3";
    songs[9] = "snd/selection.mp3";
    songs[10] = "snd/submenu.mp3";
    int msel = 0;
    stopSound();
    while (true)
    {
        playSound(songs[msel], false);
        clearScreen();
        printf("\nFishing C > Main menu > Developer > Sound test\n\nCurrent sound: %s\n\nPress UP and DOWN to toggle sounds, press RETURN to exit\nPress any other key to repeat\n", songs[msel]);
        char *rkey = readKey();
        if (strcmp(rkey, "up") == 0)
        {
            msel--;
            if (msel < 0)
                msel = max - 1;
        }
        else if (strcmp(rkey, "down") == 0)
        {
            msel++;
            if (msel >= max)
                msel = 0;
        }
        else if (strcmp(rkey, "return") == 0)
        {
            break;
        }
    }
}

void musicTest()
{
    int max = 12;
    char *songs[max];
    songs[0] = "snd/falls_bg.mp3";
    songs[1] = "snd/jelly_bg.mp3";
    songs[2] = "snd/lake_bg.mp3";
    songs[3] = "snd/livewell.mp3";
    songs[4] = "snd/menu_bg.mp3";
    songs[5] = "snd/river_bg.mp3";
    songs[6] = "snd/salmon_bg.mp3";
    songs[7] = "snd/sea_bg.mp3";
    songs[8] = "snd/sea_boss.mp3";
    songs[9] = "snd/swamp_bg.mp3";
    songs[10] = "snd/weird_bg.mp3";
    songs[11] = "snd/win_stage.mp3";
    int msel = 0;
    while (true)
    {
        stopSound();
        playSound(songs[msel], true);
        clearScreen();
        printf("\nFishing C > Main menu > Developer > Music test\n\nCurrent song: %s\n\nPress UP and DOWN to toggle music, press RETURN to exit\nPress any other key to reset song\n", songs[msel]);
        char *rkey = readKey();
        if (strcmp(rkey, "up") == 0)
        {
            msel--;
            if (msel < 0)
                msel = max - 1;
        }
        else if (strcmp(rkey, "down") == 0)
        {
            msel++;
            if (msel >= max)
                msel = 0;
        }
        else if (strcmp(rkey, "return") == 0)
        {
            break;
        }
    }
}

void menuTest(int *sel)
{
    ex--;
    int checkEx = ex + 1;
    int selValue = 15;
    int cbValue = 0;
    int sliderValue = 5;
    int menuChoice = 0;
    last = *sel;
    *sel = 0;
    while (ex < checkEx)
    {
        MenuItem *menu = (MenuItem[]){
            (MenuItem){BUTTON, "Menu item 1", &dummy, -1, NULL, &testItemEntry},
            (MenuItem){BUTTON, "Menu item 2", &dummy, -1, NULL, &testItemEntry},
            (MenuItem){BUTTON, "Menu item 3", &dummy, -1, NULL, &testItemEntry},
            (MenuItem){CHOICE, "Choice", &menuChoice, 8, (char *[]){
                                                             "Lolfish",
                                                             "Crofish",
                                                             "Friendfish",
                                                             "Friendly trout",
                                                             "Crazy Eel",
                                                             "Big Catfish",
                                                             "Moony Bass",
                                                             "Boss Bass",
                                                         },
                       NULL},
            (MenuItem){DISABLED_BUTTON, "Disabled menu item", &dummy, -1, NULL, &menuTest},
            (MenuItem){SELECTOR, "Selector", &selValue, 128, NULL, NULL},
            (MenuItem){CHECKBOX, "Checkbox item", &cbValue, 2, NULL, NULL},
            (MenuItem){SLIDER, "Slider", &sliderValue, 10, NULL, NULL},
            (MenuItem){BUTTON, "Go back", &dummy, -1, NULL, &ret},
        };
        void (*menuPtr)(int *);
        menuPtr = printMenu("Fishing C > Main menu > Developer > Menu test", menu, 9, sel, (Color[]){BLACK, WHITE});
        menuPtr(sel);
    }
}

void devMenu(int *sel)
{
    ex--;
    int checkEx = ex + 1;
    last = *sel;
    *sel = 0;
    while (ex < checkEx)
    {
        MenuItem *menu = (MenuItem[]){
            (MenuItem){BUTTON, "Sound test", &dummy, -1, NULL, &soundTest},
            (MenuItem){BUTTON, "Music test", &dummy, -1, NULL, &musicTest},
            (MenuItem){BUTTON, "Variables", &dummy, -1, NULL, &showGlobalVariables},
            (MenuItem){BUTTON, "Keycode viewer", &dummy, -1, NULL, &keycodeTester},
            (MenuItem){BUTTON, "Action test", &dummy, -1, NULL, &actionTester},
            (MenuItem){BUTTON, "Character test", &dummy, -1, NULL, &testItemEntry},
            (MenuItem){BUTTON, "Menu test", &dummy, -1, NULL, &menuTest},
            (MenuItem){BUTTON, "Disable developer menu", &dummy, -1, NULL, &menuTest},
            (MenuItem){BUTTON, "Return to main menu", &dummy, -1, NULL, &ret},
        };
        void (*menuPtr)(int *);
        menuPtr = printMenu("Fishing C > Main menu > Developer", menu, 9, sel, (Color[]){BLACK, WHITE});
        menuPtr(sel);
    }
}

void testItemEntry(int *sel)
{
    clearScreen();
    printf("\nYou selected menu item %d\n\nPress any key to continue . . . ", *sel + 1);
    readKey();
}

void rootMenu()
{
    int sel = 0;
    while (!ex)
    {
        if (strcmp(profiles[activeProfile].options.name, "Unnamed") == 0)
            nameEntry();
        if (profiles[activeProfile].progression.playing != -1 && strcmp(profiles[activeProfile].progression.mapName, "menu") != 0)
        {
            char playingMap[8];
            strcpy(playingMap, profiles[activeProfile].progression.mapName);
            clearScreen();
            printf("\nFishing C > Profiles > Resume game\n\nWould you like to resume your game\n\nIt looks like you've left the game\nwhile still fishing on a %s, %s!\n\nWould you like to continue where you left off?\n\n[Y/N] ", profiles[activeProfile].progression.mapName + 1, profiles[activeProfile].options.name);
            char *rkey = readKey();
            while (true)
            {
                if (strcmp(rkey, "121") == 0 || strcmp(rkey, "121") == 0)
                {
                    playMap(&profiles[activeProfile].progression.playing);
                    break;
                }
                else if (strcmp(rkey, "110") == 0 || strcmp(rkey, "78") == 0)
                {
                    break;
                }
                rkey = readKey();
            }
        }
        MenuItem *menu = (MenuItem[]){
            (MenuItem){BUTTON, "Start", &dummy, -1, NULL, &startMenu},
            (MenuItem){BUTTON, "Save game", &dummy, -1, NULL, &saveActiveProfile},
            (MenuItem){BUTTON, "Change profile", &dummy, -1, NULL, &profileMenu},
            (MenuItem){BUTTON, "Settings", &dummy, -1, NULL, &settingsMenu},
            (MenuItem){DISABLED_BUTTON, "Livewell", &dummy, -1, NULL, &testItemEntry},
            (MenuItem){BUTTON, "Exit", &dummy, -1, NULL, &ret},
            (MenuItem){BUTTON, "Developer", &dummy, -1, NULL, &devMenu},
        };
        void (*menuPtr)(int *);
        void (*nargPtr)();
        menuPtr = printMenu("Fishing C > Main menu", menu, 7, &sel, (Color[]){BLACK, WHITE});
        if (sel == 1)
        {
            nargPtr = (void (*)(void))menuPtr;
            nargPtr();
        }
        else
        {
            menuPtr(&sel);
        }
        if (activeProfile == -1)
            ret(&sel);
        if (last == -1)
            sel = 0;
    }
}

char *genTip(char *mapTip)
{
    int tip = rand() % 11;
    switch (tip)
    {
    case 0:
        return "You have a better chance at catching different fish\n by changing the casting spot.";
    case 1:
        return "To see your current luck percent, press ESC during\n gameplay.";
    case 2:
        return "If you don't have a lot of luck you may end up with\n No fish or even get a boot.";
    case 4:
        return "If this game gets long, you can leave any time\n and resume exactly where you left off.";
    case 5:
        return "You can get more luck points by having better luck.";
    default:
        return mapTip;
    }
}

void nameEntry()
{
    clearScreen();
    if (strcmp(profiles[activeProfile].options.name, "Unnamed") == 0)
    {
        printf("\nFishing C > Name entry\n\n");
    }
    else
    {
        printf("\nFishing C > Main menu > Settings > Change name\n\nCurrent name: %s\n\n", profiles[activeProfile].options.name);
    }
    printf("Please enter your name...\n\n");
    int nI = 0;
    char c;
    char *lastName = malloc(0x20);
    strcpy(lastName, profiles[activeProfile].options.name);
    do
    {
        scanf("%c", &c);
        if (c == 8 && nI > 0)
        {
            profiles[activeProfile].options.name[nI] = '\0';
            profiles[activeProfile].options.name[nI - 1] = '\0';
            nI--;
        }
        if (nI < 0x1F && c != 8)
        {
            profiles[activeProfile].options.name[nI] = c != '\n' ? c : '\0';
            nI++;
        }
    } while (c != '\n');
    // use last name if left empty (or the default "Unnamed")
    if (strlen(profiles[activeProfile].options.name) == 0)
    {
        strcpy(profiles[activeProfile].options.name, lastName);
    }
    free(lastName);
    playSound("snd/submenu.mp3", false);
}

void playMap(int *sel)
{
    clearScreen();
    printf("\n ╔");
    for (int i = 0; i < 46; i++)
        printf("═");
    char *ldt = malloc(44);
    for (int i = 0; i < 44; i++)
        ldt[i] = 0;
    strcat(ldt, "Loading ");
    strcat(ldt, mapsList[*sel].name);
    strcat(ldt, "...");
    printf("╗\n ║ %-44s ║\n ╚", ldt);
    for (int i = 0; i < 46; i++)
        printf("═");
    printf("╝\n\n Tip: %s\n", genTip(mapsList[*sel].tip));
    free(ldt);
    stopSound();
    sleep(2);
    playSound("snd/load.mp3", 0);
    char buf[32];
    snprintf(buf, 32, "maps/%s.map", mapsList[*sel].file);
    for (int i = 5; i < 32; i++)
    {
        char mc = buf[i];
        if (mc == 0x2E)
            mc = 0;
        profiles[activeProfile].progression.mapName[i - 5] = mc;
        if (mc == 0)
            break;
    }
    loadMap(buf);
    for (int i = 0; i < 999; i++)
    {
        cprintf("   ", (Color[]){mapColor.bg, mapColor.fg, mapColor.bg, mapColor.fg});
        clearScreen();
    }
    snprintf(buf, 32, "snd/%s", mapMusic.filename);
    playSound(buf, 1);
    printf("Loaded map: %s\n", mapTitle.name);
    mapLoop(*sel);
    if (strcmp(profiles[activeProfile].progression.mapName, "win") == 0 && profiles[activeProfile].progression.throws > 0) // we check this to avoid floating point exception (it bad)
    {
        stopSound();
        playSound("snd/win_stage.mp3", false);
        profiles[activeProfile]
            .progression.mapUnlocks[mapsList[*sel].idx] = profiles[activeProfile]
                                                              .progression.mapUnlocks[mapsList[*sel].idx] |
                                                          1;
        clearScreen();
        printf("\nFishing C > You win!\n\nCongratulations!\n\n");
        printf("You have finished the %s!\n\nTotal fish: %.2f kg\nAverage luck: %d%%\n\n", mapTitle.name, (float)profiles[activeProfile].progression.weight / 1000.0f, profiles[activeProfile].progression.totalLuck / profiles[activeProfile].progression.throws);
        printf("Press any key to continue . . . ");
        readKey();
    }
    unloadMap();
    playSound("snd/menu_bg.mp3", 1);
    clearScreen();
}

void startMenu(int *sel)
{
    ex--;
    int checkEx = ex + 1;
    last = *sel;
    *sel = 0;
    MenuItem menu[mapsCount];
    for (int i = 0; i < mapsCount; i++)
    {
        char unlock = true;
        if (i > 0)
        {
            unlock = profiles[activeProfile].progression.mapUnlocks[i - 1] & 0x01;
        }
        if (!mapsList[i].secret || profiles[activeProfile].progression.solmn == READY)
        {
            char *nameBuf = malloc(32);
            strcpy(nameBuf, mapsList[i].name);
            menu[i] = (MenuItem){unlock ? BUTTON : DISABLED_BUTTON, unlock ? nameBuf : "Locked", &dummy, -1, NULL, &playMap};
        }
    }
    menu[mapsCount - 1] = (MenuItem){BUTTON, "Go back", &dummy, -1, NULL, &ret};
    while (ex < checkEx)
    {
        void (*menuPtr)(int *);
        menuPtr = printMenu("Fishing C > Main menu > Start game", menu, mapsCount, sel, (Color[]){BLACK, WHITE});
        menuPtr(sel);
    }
}

void toggleSetting(int *sel)
{
    if (*sel == 0)
    {
        stopSound();
        if (profiles[activeProfile].options.music == true)
        {
            playSound("snd/menu_bg.mp3", true);
        }
    }
}

void settingsMenu(int *sel)
{
    ex--;
    int checkEx = ex + 1;
    last = *sel;
    *sel = 0;
    while (ex < checkEx)
    {
        MenuItem *menu = (MenuItem[]){
            (MenuItem){CHECKBOX, "Enable music", &profiles[activeProfile].options.music, 2, NULL, &toggleSetting},
            (MenuItem){CHECKBOX, "Enable SFX", &profiles[activeProfile].options.sfx, 2, NULL, &toggleSetting},
            (MenuItem){CHECKBOX, "Use autosave", &profiles[activeProfile].options.autosave, 2, NULL, &toggleSetting},
            (MenuItem){CHECKBOX, "Enable story/instructions", &profiles[activeProfile].options.storytell, 2, NULL, &toggleSetting},
            (MenuItem){CHOICE, "Difficulty", (int *)&profiles[activeProfile].options.difficulty, 3, (char *[]){"Easy", "Normal", "Hard"}, &ret},
            (MenuItem){CHOICE, "Rod color", (int *)&profiles[activeProfile].options.rodColor, 14, (char *[]){"Blue", "Green", "Aqua", "Red", "Purple", "Gold", "White", "Gray", "Light blue", "Lime", "Cyan", "Bright red", "Magenta", "Yellow"}, &ret},
            (MenuItem){BUTTON, "Change name", &dummy, -1, NULL, &nameEntry},
            (MenuItem){BUTTON, "Delete profile", &dummy, -1, NULL, &ret},
            (MenuItem){BUTTON, "Go back to main menu", &dummy, -1, NULL, &ret},
        };
        void (*menuPtr)(int *);
        menuPtr = printMenu("Fishing C > Main menu > Settings", menu, 9, sel, (Color[]){BLACK, WHITE});
        menuPtr(sel);
    }
}

int fishingMenu(char *title, int *defaults, void *fCast, void *fLore, void *fExit, Color *colors)
{
    int sel = 0;
    while (1 < 2)
    {
        MenuItem menu[7] =
            {
                (MenuItem){BUTTON, "Cast", &dummy, -1, NULL, fCast},
                (MenuItem){SELECTOR, "Distance", defaults, 20, NULL, NULL},
                (MenuItem){SELECTOR, "Angle", &defaults[1], 20, NULL, NULL},
                (MenuItem){BUTTON, "Save game", &dummy, -1, NULL, &saveActiveProfile},
                (MenuItem){BUTTON, "Lore", &dummy, -1, NULL, fLore},
                (MenuItem){BUTTON, "Scouting map", &dummy, -1, NULL, &dummyFunc},
                (MenuItem){BUTTON, "Exit", &dummy, -1, NULL, fExit},
        };
        void (*menuPtr)(int *);
        void (*nargPtr)();
        menuPtr = printMenu(title, menu, 7, &sel, colors);
        if (sel == 5)
        {
            clearScreen();
            char *labels = "0123456789ABCDEFGHIJ";
            printf("DA%s\n", labels);
            for (int i = 0; i < 20; i++)
            {
                printf("%c ", labels[i]);
                for (int j = 0; j < 20; j++)
                {
                    printf("%c", mapSpawn.entries[i * 20 + j] == 252 ? 'B' : mapSpawn.entries[i * 20 + j] == 253 ? 'N'
                                                                         : mapSpawn.entries[i * 20 + j] == 254   ? 'F'
                                                                                                                 : ((j % 2) == 0 ? (i % 2 == 0 ? '_' : ' ') : (i % 2 == 0 ? ' ' : '_')));
                }
                printf("\n");
            }
            printf("\nPress any key to continue . . . ");
            readKey();
            clearScreen();
            continue;
        }
        if (sel == 0)
            menuPtr(defaults);
        if (sel != 0)
        {
            nargPtr = (void (*)(void))menuPtr;
            nargPtr();
        }
        if (sel == 0 || sel == 6)
        {
            break;
        }
    }
    return sel;
}