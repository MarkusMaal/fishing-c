#include "main.h"

int main()
{
    initProfiles();
    loadProfile(0);
    getMaps();
    cprintf("", (Color[]){BLACK, BLACK, BLACK, WHITE});
    clearScreen();
    initSoundEngine();
    playSound("snd/menu_bg.mp3", 1);
    rootMenu();
    return 0;
}
