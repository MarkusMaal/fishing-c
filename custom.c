#include "custom.h"

void cmLoader(CustomStage cust, char *map, char *environment)
{
    clearScreen();
    printf("\n ╔");
    for (int i = 0; i < 46; i++)
        printf("═");
    char *ldt = malloc(44);
    for (int i = 0; i < 44; i++)
        ldt[i] = 0;
    strcat(ldt, "Loading custom stage...");
    printf("╗\n ║ %-44s ║\n ╚", ldt);
    for (int i = 0; i < 46; i++)
        printf("═");
    cust.goalWeight += 1;
    printf("╝\n\n Environment: %s\n %s", environment, cust.arcadeRules ? "Arcade rules enable" : "");
    if (!cust.arcadeRules)
    {
        printf("Goal: %d kg", cust.goalWeight);
    }
    printf("\n");
    free(ldt);
    // enable custom mode for correct game logic
    profiles[activeProfile].progression.mode = CUSTOM;
    profiles[activeProfile].maxThrows = cust.arcadeRules ? cust.throwCount + 1 : 0;
    sleep(2);
    // load original map as is
    if (!loadMap(map))
    {
        unloadMap();
        playSound("snd/menu_bg.mp3", true);
        clearScreen();
        playSound("snd/error.mp3", false);
        printf("\nFatal loading error\n\nCannot access file \"%s\"\n\nReturning to previous menu\n\nPress any key to continue . . . ", map);
        readKey(false);
        return;
    }
    // modify goals
    mapGoal.grams = cust.goalWeight * 1000;
    mapGoal.type = cust.arcadeRules ? SANDBOX : WEIGHT;
    mapGoal.variation = 0;
    profiles[activeProfile].progression.goalWeight = mapGoal.grams;
    profiles[activeProfile].progression.weight = 0;
    for (int i = 0; i < 20; i++)
    {
        mapGoal.targets[i] = 0;
    }
    int fishCount = 0;
    for (int i = 0; i < 99; i++)
    {
        if (cust.fishToggles[i].enabled)
        {
            fishCount++;
        }
    }
    // modify fish
    mapFish.header.chunkCount = fishCount;
    int idx = 0;
    for (int i = 0; i < 99; i++)
    {
        if (cust.fishToggles[i].enabled)
            strcpy(mapFish.messages[idx++], cust.fishToggles[i].fishFile);
    }
    // modify spawn/luck table
    for (int i = 0; i < 400; i++)
    {
        mapSpawn.entries[i] = rand() % fishCount;
        mapLuck.entries[i] = rand() % 128;
    }

    // modify status messages
    if (!cust.arcadeRules)
    {
        strcpy(mapStatus.messages[0].msg, "Total fish: %.2f/%.2f kg (%d%% complete)");
        mapStatus.messages[0].idx = 3;
    }
    else
    {
        strcpy(mapStatus.messages[0].msg, "Total fish: %.2f kg (%d throws remaining)");
        mapStatus.messages[0].idx = 6;
        for (int i = 0; i < mapStatus.header.chunkCount; i++)
        {
            if (mapStatus.messages[i].idx == 1)
            {
                strcpy(mapStatus.messages[i].msg, "Arcade rules");
            }
        }
    }
    stopSound();
    char buf[32];
    chcolor((Color[]){mapColor.bg, mapColor.fg});
    for (int i = 0; i < 999; i++)
    {
        printf("   ");
    }
    clearScreen();
    snprintf(buf, 32, "snd/%s", *mapMusic.filename);
    playSound(buf, 1);
    profiles[activeProfile].progression.playing = cust.environmentId;
    strcpy(profiles[activeProfile].progression.mapName, "custom");
    mapLoop(cust.environmentId);
    stopSound();
    chcolor((Color[]){BLACK, WHITE});
    for (int i = 0; i < 999; i++)
    {
        printf("   ");
    }
    locate(0, 0);
    if (strcmp(profiles[activeProfile].progression.mapName, "win") == 0)
    {
        stopSound();
        playSound("snd/win_stage.mp3", false);
        clearScreen();
        int coverageC = 0;
        for (int i = 0; i < 400; i++)
            coverageC += ((unsigned char)profiles[activeProfile].localSpawnTable[i] > mapFish.header.chunkCount && (unsigned char)profiles[activeProfile].localSpawnTable[i] != 255) ? 1 : 0;
        int coveragePerc = coverageC * 100 / 400;
        printf("\nFishing C > You win!\n\nCongratulations!\n\n");
        printf("You have finished a custom game at the %s!\n\nTotal fish: %.2f kg\nAverage luck: %d%%\nCoverage: %d%%\n\n", mapTitle.name, (float)profiles[activeProfile].progression.weight / 1000.0f, profiles[activeProfile].progression.totalLuck / profiles[activeProfile].progression.throws, coveragePerc);
        printf("Press any key to continue . . . ");
        readKey(false);
    }
    stopSound();
    playSound("snd/menu_bg.mp3", true);
    strcpy(profiles[activeProfile].progression.mapName, "menu");
    profiles[activeProfile].progression.throws = 0;
    profiles[activeProfile].progression.weight = 0;
    profiles[activeProfile].progression.totalLuck = 0;
    profiles[activeProfile].progression.playing = -1;
    profiles[activeProfile].progression.mode = STORY;
    profiles[activeProfile].maxThrows = 0;
}