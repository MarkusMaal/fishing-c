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
