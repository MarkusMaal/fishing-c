#include "main.h"

int main()
{
    for (int i = 0; i < 8; i++)
    {
        initProfile(i);
    }
    getMaps();
    cprintf("", (Color[]){BLACK, BLACK, BLACK, WHITE});
    clearScreen();
    initSoundEngine();
    playSound("snd/menu_bg.mp3", 1);
    titleScreen();
    profileMenu();
    if (activeProfile == -1)
    {
        clearScreen();
        printf("Sad to see you go!\n");
        stopSound();
        sleep(2);
        unloadSoundEngine();
        return 0;
    }
    rootMenu();
    return 0;
}

void titleScreen()
{
    FishData fsh = loadFish(rand() % 100 == 2 ? "fsh/lama.fsh" : "fsh/friendlytrout.fsh");
    chcolor((Color[]){BLUE, LWHITE});
    for (int i = 0; i < 999; i++)
        printf("%-99s", "");
    clearScreen();
    printf("    \n");
    char *pad = "               ";
    for (int i = 0; i < 16; i++)
    {
        printf("%s%s", pad, fsh.geometry[0][i]);
    }

    locate(16, 22);
    printf("Fishing C 2.0");

    char *rKey;
    int even = true;
    while (true)
    {
        rKey = readKey(true);
        if (strcmp(rKey, "1") != 0)
            break;
        locate(18, 16);
        printf(even ? "Press any key to start..." : "                         ");
        even = !even;
        fflush(stdout);
    }
    chcolor((Color[]){BLACK, WHITE});
    locate(0, 0);
    for (int i = 0; i < 999; i++)
        printf("%-99s", "");
    locate(0, 0);
}
