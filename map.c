#include "map.h"

Title mapTitle;
Layout mapLayout;
Water mapWater;
Goal mapGoal;
Music mapMusic;
GlobalColor mapColor;
Fish mapFish;
StatusMsg mapStatus;
Boat mapBoat;
Msg mapMessage;
ByteTable mapSpawn;
ByteTable mapLuck;
Lore mapLore;

void loadMap(char *fileName)
{
    FILE *infile;
    infile = fopen(fileName, "rb");
    Record rStruct;
    int c;
    while ((c = fgetc(infile)) != EOF)
    {
        fseek(infile, -1, SEEK_CUR);
        fread(&rStruct, sizeof(Record), 1, infile);
        int next = ftell(infile) + rStruct.total * 0x10;
        printf("%s: %x %x %x\n", rStruct.label, rStruct.chunkSize, rStruct.chunkCount, rStruct.total);
        if (strcmp(rStruct.label, "TITLE") == 0)
        {
            mapTitle.header = rStruct;
            if (fread(&mapTitle.name, rStruct.chunkSize, rStruct.chunkCount, infile) != 1)
            {
                printf("Error reading map name\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "LAYOUT") == 0)
        {
            mapLayout.header = rStruct;
            if (fread(&mapLayout.chunks, rStruct.chunkCount * rStruct.chunkSize, 1, infile) != 1)
            {
                printf("Error reading map geometry\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "WATER") == 0)
        {
            if (fread(&mapWater, sizeof(mapWater), 1, infile) != 1)
            {
                printf("Error reading map water\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "GOAL") == 0)
        {
            if (fread(&mapGoal, sizeof(mapGoal), 1, infile) != 1)
            {
                printf("Error reading map water\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "MUSIC") == 0)
        {
            mapMusic.header = rStruct;
            if (fread(&mapMusic.filename, sizeof(mapMusic.filename), 1, infile) != 1)
            {
                printf("Error reading map music\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "COLOR") == 0)
        {
            if (fread(&mapColor, sizeof(mapColor), 1, infile) != 1)
            {
                printf("Error reading global colors\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "FISH") == 0)
        {
            mapFish.header = rStruct;
            if (fread(&mapFish.messages, rStruct.chunkCount * rStruct.chunkSize, 1, infile) != 1)
            {
                printf("Error reading fish entries\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "STATUS") == 0)
        {
            mapStatus.header = rStruct;
            if (fread(&mapStatus.messages, rStruct.chunkSize * rStruct.chunkCount, 1, infile) != 1)
            {
                printf("Error reading status messages\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "BOAT") == 0)
        {
            if (fread(&mapBoat, sizeof(Boat), 1, infile) != 1)
            {
                printf("Error reading boat information\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "MESSAGE") == 0)
        {
            mapMessage.header = rStruct;
            if (fread(&mapMessage.messages, sizeof(mapMessage.messages), 1, infile) != 1)
            {
                printf("Error reading secondary messages\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "SPAWN") == 0)
        {
            mapSpawn.header = rStruct;
            if (fread(&mapSpawn.entries, sizeof(mapSpawn.entries), 1, infile) != 1)
            {
                printf("Error reading fish spawn table\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "LUCK") == 0)
        {
            mapLuck.header = rStruct;
            if (fread(&mapLuck.entries, sizeof(mapLuck.entries), 1, infile) != 1)
            {
                printf("Error reading luck table\n");
                readKey();
            }
        }
        else if (strcmp(rStruct.label, "LORE") == 0)
        {
            mapLore.header = rStruct;
            if (fread(&mapLore.messages, rStruct.chunkSize * rStruct.chunkCount, 1, infile) != 1)
            {
                printf("Error reading map lore\n");
                readKey();
            }
        }
        else
        {
            printf("INVALID\n");
            readKey();
        }
        fseek(infile, next, SEEK_SET);
    }
}

void drawMap(int dx, int dy)
{
    clearScreen();
    int drawnChars = 0;
    int bait_y = (float)dy / 20.0f * (float)(mapWater.h - 2.0f);
    int bait_x = (float)dx / 20.0f * ((float)(mapWater.w - 12.0f));
    int y;
    int x;
    for (int i = 0; i < mapLayout.header.chunkCount; i++)
    {
        LayoutChunk ch = mapLayout.chunks[i];
        for (int j = 0; j < ch.count; j++)
        {
            y = drawnChars / 81;
            x = drawnChars % 81;
            if (((x == mapBoat.x || x == mapBoat.x + 1) && y >= mapBoat.y + 3 && y < mapBoat.y + 7) ||
                ((x == mapBoat.x + 2) && ((y == mapBoat.y + 2) || (y == mapBoat.y + 1))) ||
                ((x == mapBoat.x + 3 || x == mapBoat.x + 4) && (y == mapBoat.y + 1)) ||
                ((x == mapBoat.x + 4 || x == mapBoat.x + 5 || x == mapBoat.x + 6) && (y == mapBoat.y)))
            {
                cprintf("█", (Color[]){mapColor.bg, RED, mapColor.bg, mapColor.fg});
            }
            else if (x >= mapBoat.x && x < mapBoat.x + 3 && y > mapBoat.y && y < mapBoat.y + 6)
            {
                cprintf("█", (Color[]){mapColor.bg, LWHITE, mapColor.bg, mapColor.fg});
            }
            else if (y > mapWater.y && y < mapWater.y + mapWater.h && x > mapWater.x && x < mapWater.x + mapWater.w)
            {
                int water_x = x - (mapWater.x + 1);
                int water_y = y - (mapWater.y + 1);
                cprintf("█", (Color[]){mapColor.bg, (water_x >= bait_x) && (water_y == bait_y) && (water_x < bait_x + 5) ? LRED : BLUE, mapColor.bg, mapColor.fg});
            }
            else
            {
                cprintf("█", (Color[]){mapColor.bg, ch.color, mapColor.bg, mapColor.fg});
            }
            drawnChars++;
            if (drawnChars % 81 == 0)
            {
                printf("\n");
            }
        }
    }
    for (int i = 0; i < mapStatus.header.chunkCount; i++)
    {
        locate(mapStatus.messages[i].y, mapStatus.messages[i].x);
        switch (mapStatus.messages[i].idx)
        {
        case 0:
            printf(mapStatus.messages[i].msg, profiles[activeProfile].progression.weight / 1000, (int)((float)profiles[activeProfile].progression.weight / (float)profiles[activeProfile].progression.goalWeight * 100.f));
            break;
        case 1:
            printf(mapStatus.messages[i].msg, profiles[activeProfile].progression.goalWeight / 1000);
            break;
        default:
            break;
        }
    }
    locate(y, x);
    printf("\n");
}

void mapLoop(int mapId)
{
    if (profiles[activeProfile].progression.playing == -1)
    {
        profiles[activeProfile].progression.playing = mapId;
        profiles[activeProfile].progression.goalWeight = mapGoal.grams + (int)(((float)(rand() % 101) / 100.0f) * (float)mapGoal.variation * 2.0f - (float)mapGoal.variation);
    }
    else
    {
        memcpy(mapSpawn.entries, profiles[activeProfile].spawnTable, 0x190);
    }
    while (true)
    {
        memcpy(profiles[activeProfile].spawnTable, mapSpawn.entries, 0x190);
        if (profiles[activeProfile].options.autosave)
        {
            saveProfile(profiles[activeProfile], activeProfile);
        }
        if (profiles[activeProfile].options.storytell && !profiles[activeProfile].progression.storiesTold[mapId] & 0x01)
        {
            loreDisplay();
            profiles[activeProfile].progression.storiesTold[mapId] |= 0x01;
        }
        int tx = rand() % 20;
        int ty = rand() % 20;
        if (profiles[activeProfile].progression.goalWeight <= profiles[activeProfile].progression.weight)
        {
            profiles[activeProfile].progression.mapName[0] = 'w';
            profiles[activeProfile].progression.mapName[1] = 'i';
            profiles[activeProfile].progression.mapName[2] = 'n';
            profiles[activeProfile].progression.mapName[3] = '\0';
            break;
        }
        int sel = fishingMenu(mapTitle.name, (int[]){tx, ty}, cast, loreDisplay, exitMap, (Color[]){mapColor.bg, mapColor.fg});
        if (sel == 5)
        {
            break;
        }
    }
}

void exitMap()
{
    unloadMap();
}

void loreDisplay()
{
    for (int i = 0; i < mapLore.header.chunkCount; i++)
    {
        clearScreen();
        printf("%sPress any key to continue . . . ", mapLore.messages[i]);
        readKey();
    }
}

void cast(int *values)
{
    int tries;
    int luck;
    int roll;
    int tx = *values;
    int ty = values[1];
    luck = rand() % 101;
    profiles[activeProfile].progression.throws++;
    profiles[activeProfile].progression.totalLuck += luck;
    if (luck > 100)
        luck = 100;
    int spotId = tx + ty * 20;
    drawMap(tx, ty);
    locate(mapStatus.messages[2].y, mapStatus.messages[2].x);
    printf(mapStatus.messages[2].msg, mapMessage.messages[rand() % 4]);
    fflush(stdout);
    // printf("[DEBUG] Fish chance: %d%%\n", luck);
    int spotLuck = mapLuck.entries[spotId];
    tries = ty + 1;
    int playedCricketSound = false;
    int playedPopSound = false;
    for (int i = 0; i < tries; i++)
    {
        // printf("[DEBUG] Running roll attempt %d/%d\n", i + 1, tries);
        sleep((int)((100.0 - (float)luck) / 100.0f) * rand() % 20 + 1);
        roll = rand() % 101;
        if (roll < luck)
        {
            if (!playedPopSound && luck > 70)
            {
                playedPopSound = true;
                playSound("snd/pop.mp3", false);
            }
            roll = rand() % 256;
            if ((strcmp(mapFish.messages[(int)mapSpawn.entries[spotId]], "boot") == 0) && (luck > 50))
            {
                // printf("[DEBUG] Removed boot from %d\n", spotId);
                mapSpawn.entries[spotId] = 255;
            }
            if (mapSpawn.entries[spotId] == 255)
            {
                // printf("[DEBUG] No fish at %d\n", spotId);
                locate(mapStatus.messages[2].y, mapStatus.messages[2].x);
                printf(mapStatus.messages[2].msg, mapMessage.messages[rand() % 4]);
                fflush(stdout);
            }
            if (spotLuck < roll && (int)mapSpawn.entries[spotId] != 255)
            {
                // printf("[DEBUG] Required luck %d at %d\n", spotLuck, spotId);
                locate(mapStatus.messages[2].y, mapStatus.messages[2].x);
                printf(mapStatus.messages[2].msg, mapMessage.messages[4]);
                fflush(stdout);
                int lost = 0;
                for (int i = 0; i < 10; i++)
                {
                    // printf("[DEBUG] Running luck cycles at %d (%d/10)\n", spotId, i + 1);
                    roll = rand() % 256;
                    if (roll < spotLuck)
                    {
                        // printf("[DEBUG] Reducing spot luck at %d\n", spotId);
                        spotLuck = (float)spotLuck * 0.8f;
                        if (spotLuck > luck)
                        {
                            lost = 1;
                            break;
                        }
                    }
                    sleep((int)((100.0 - (float)luck) / 100.0f) * rand() % 20 + 1);
                }
                if (lost)
                {
                    // printf("[DEBUG] Fish lost at %d\n", spotId);
                    locate(mapStatus.messages[2].y, mapStatus.messages[2].x);
                    printf(mapStatus.messages[2].msg, mapMessage.messages[3]);
                    fflush(stdout);
                    sleep((int)((100.0 - (float)luck) / 100.0f) * rand() % 20 + 1);
                    continue;
                }
                clearScreen();
                printf("SPOT %d\n", spotId);
                printf("FISH (ID: %d)\n", mapSpawn.entries[spotId]);
                // printf("[DEBUG] Rolled %d at %d\n", mapSpawn.entries[spotId], spotId);
                char *fishName = mapFish.messages[(int)mapSpawn.entries[spotId]];
                char buf[32];
                snprintf(buf, 32, "fsh/%s.fsh", fishName);
                FishData fish = loadFish(buf);
                int *result = fightFish(fish, luck);
                if (*result != 0)
                    mapSpawn.entries[spotId] = 255;
                profiles[activeProfile].progression.weight += *result * (fish.largeUnits ? 1000 : 1);
                return;
            }
        }
        else
        {
            locate(mapStatus.messages[2].y, mapStatus.messages[2].x);
            printf(mapStatus.messages[2].msg, mapMessage.messages[rand() % 4]);
            if (luck < 10 && !playedCricketSound)
            {
                playSound("snd/crickets.mp3", false);
                playedCricketSound = true;
                // printf("[DEBUG] Unlucky at %d\n", spotId);
            }
            fflush(stdout);
        }
        ty--;
        if (ty < 0)
            ty = 0;
    }
    clearScreen();
}

void unloadMap()
{
    for (int i = 0; i < 999; i++)
    {
        cprintf("   ", (Color[]){BLACK, WHITE, BLACK, WHITE});
        clearScreen();
    }
    locate(0, 0);
    for (int i = 0; i < 999; i++)
    {
        printf(" ");
    }
    locate(0, 0);

    printf("\n ╔");
    for (int i = 0; i < 46; i++)
        printf("═");
    char *ldt = "Unloading...";
    printf("╗\n ║ %-44s ║\n ╚", ldt);
    for (int i = 0; i < 46; i++)
        printf("═");
    printf("╝\n\n Tip: %s\n", "You can play the game again without losing\n progress by creating a new profile.");
    stopSound();
    char *file = "menu\0";
    for (int i = 0; i < (int)strlen(file) + 1; i++)
        profiles[activeProfile].progression.mapName[i] = file[i];
    profiles[activeProfile].progression.totalLuck = 0;
    profiles[activeProfile].progression.throws = 0;
    profiles[activeProfile].progression.goalWeight = 0;
    profiles[activeProfile].progression.weight = 0;
    profiles[activeProfile].progression.playing = -1;
    /*free(mapTitle.name);
    free(&mapLayout);
    free(&mapFish);
    free(&mapStatus);
    free(&mapMessage);
    free(&mapSpawn);
    free(&mapLuck);
    free(&mapLore);*/
}