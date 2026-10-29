#include "main.h"

int main()
{
    demo = NO_DEMO;
    for (int i = 0; i < 8; i++)
    {
        initProfile(i);
    }
    getMaps();
    cprintf("", (Color[]){BLACK, BLACK, BLACK, WHITE});
    clearScreen();
    initSoundEngine();
    splashScreen();
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

void splashScreen()
{
    Color colors[10] = {BLACK, LBLACK, BLACK, WHITE, BLACK, LWHITE, BLACK, WHITE, BLACK, LBLACK};
    FILE *copyFile = fopen("txt/splash.txt", "r");
    char *buffer = 0;
    long length;
    if (copyFile)
    {
        fseek(copyFile, 0, SEEK_END);
        length = ftell(copyFile);
        fseek(copyFile, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
            fread(buffer, 1, length, copyFile);
        fclose(copyFile);
    }
    playSound("snd/load.mp3", false);
    for (int cIdx = 0; cIdx < ((profiles[activeProfile].progression.solmn == READY) ? 12 : 8); cIdx += 2)
    {
        locate(0, 0);
        chcolor(&colors[cIdx]);
        printf(buffer);
        if (cIdx != 4)
            usleep(66667);
        else
            sleep(1);
    }
    clearScreen();
    sleep(1);
    if (buffer)
        free(buffer);
}

void titleScreen()
{
    FishData *fsh = malloc(sizeof(FishData));
    for (long unsigned int i = 0; i < sizeof(FishData); i++)
        ((char *)fsh)[i] = 0;
    *fsh = loadFish(rand() % 100 == 2 ? "fsh/lama.fsh" : "fsh/friendlytrout.fsh");
    chcolor((Color[]){BLUE, LWHITE});
    for (int i = 0; i < 999; i++)
        printf("%-99s", "");
    clearScreen();
    printf("    \n");
    int padSize = 28;
    char *pad = malloc(padSize + 1);
    for (int i = 0; i < padSize; i++)
    {
        pad[i] = ' ';
    }
    pad[padSize] = '\0';
    for (int i = 0; i < 16; i++)
    {
        printf("%s%s", pad, fsh->geometry[0][i]);
    }

    long start_time = clock();
    locate(16, padSize + 7);
    printf("Fishing C 2.0");

    char *rKey;
    int even = true;
    free(pad);
    free(fsh);
    while (true)
    {
        rKey = readKey(true);
        if (strcmp(rKey, "1") != 0)
            break;
        if ((clock() - start_time) / CLOCKS_PER_SEC * 1000 > 30000)
        {
            switch (demo)
            {
            case NO_DEMO:
                demo = SPLASH;
                chcolor((Color[]){BLACK, WHITE});
                locate(0, 0);
                for (int i = 0; i < 999; i++)
                    printf("%-99s", "");
                locate(0, 0);
                stopSound();
                splashScreen();
                playSound("snd/menu_bg.mp3", true);
                titleScreen();
                break;
            default:
                break;
            }
            break;
        }
        locate(18, padSize + 1);
        printf(even ? "Press any key to start..." : "                         ");
        even = !even;
        fflush(stdout);
    }
    chcolor((Color[]){BLACK, WHITE});
    locate(0, 0);
    for (int i = 0; i < 999; i++)
        printf("%-99s", "");
    locate(0, 0);
    demo = NO_DEMO;
}
