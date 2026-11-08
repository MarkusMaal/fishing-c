#include "stats.h"

PlayerStats playerStats[8];

int loadStats(int idx)
{
    char buf[32];
    snprintf(buf, 32, "save/stats%d.dat", idx + 1);
    FILE *infile;
    infile = fopen(buf, "rb");
    if (infile == NULL)
    {
        initStats(idx);
        return 0;
    }
    unsigned int onlineSum;
    fseek(infile, 0, SEEK_SET);
    fread(&onlineSum, sizeof(unsigned int), 1, infile);
    fseek(infile, 4, SEEK_SET);
    fread(&playerStats[idx], sizeof(PlayerStats), 1, infile);
    unsigned int offlineSum = crc32((char *)&playerStats[idx], sizeof(PlayerStats));
    if (onlineSum != offlineSum)
    {
        initStats(idx);
        return 1;
    }
    fclose(infile);
    playSound("snd/load.mp3", false);
    return 0;
}

void initStats(int idx)
{
    playerStats[idx].caughtFishCnt = 0;
    playerStats[idx].cricketCount = 0;
    playerStats[idx].heaviestFish = 0;
    playerStats[idx].longestFish = 0;
    playerStats[idx].lostFishCnt = 0;
    playerStats[idx].playTime = 0;
    playerStats[idx].popCount = 0;
    playerStats[idx].saveCount = 0;
    playerStats[idx].throwCount = 0;
    playerStats[idx].totalLuck = 0;
    playerStats[idx].totalWeight = 0;
    playerStats[idx].biteCount = 0;
    playerStats[idx].lostFights = 0;
    playerStats[idx].wrongInputs = 0;
    playerStats[idx].correctInputs = 0;
    playerStats[idx].bootCount = 0;
    playerStats[idx].stageLoseCount = 0;
    playerStats[idx].stageWinCount = 0;
    memcpy(playerStats[idx].mapPlayCnt, (unsigned short[]){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 32);
}

void displayStats(int idx)
{
    char *notAvailableStr = "N/A";
    PlayerStats stats = playerStats[idx];
    unsigned short largestPlayCnt = 0;
    int largestPlayId = -1;
    int totalMapsPlayed = 0;
    for (int i = 0; i < mapsCount; i++)
    {
        totalMapsPlayed += stats.mapPlayCnt[i];
        if (stats.mapPlayCnt[i] <= largestPlayCnt)
            continue;
        largestPlayCnt = stats.mapPlayCnt[i];
        largestPlayId = i;
    }
    char *favMap = largestPlayId == -1 ? notAvailableStr : mapsList[largestPlayId].name;

    printf("\nFishing C > Main menu > Statistics\n\n");
    printf("Total fish weight: %.2f kg\n", (float)stats.totalWeight / 1000.0f);
    printf("Total fish caught: %d\n", stats.caughtFishCnt - stats.bootCount);
    printf("Boots: %d\n", stats.bootCount);
    printf("Lost fish: %d\n", stats.lostFishCnt);
    printf("Lost fights: %d\n\n", stats.lostFights);

    printf("Average luck: %d%%\n", (int)((float)stats.totalLuck / (float)stats.throwCount));
    printf("Total throws: %d\n\n", stats.throwCount);

    printf("Longest fish: %d cm\n", stats.longestFish);
    printf("Heaviest fish: %.2f kg\n\n", (float)stats.heaviestFish / 1000.0f);

    printf("Cricket sounds: %d / Pop sounds: %d\n", stats.cricketCount, stats.popCount);
    printf("Save count: %d\n", stats.saveCount);
    printf("Fight input accuracy: %.2f%%\n\n", (float)stats.correctInputs / (float)(stats.correctInputs + stats.wrongInputs) * 100.0f);

    printf("Total maps played: %d\n", totalMapsPlayed);
    printf("Favorite map: %s\n", favMap);
    printf("Wins: %d / Losses: %d", stats.stageWinCount, stats.stageLoseCount);
    if (stats.stageLoseCount + stats.stageLoseCount > 0)
    {
        printf(" (ratio: %.2f%%)", (float)stats.stageWinCount / (float)(stats.stageWinCount + stats.stageLoseCount) * 100.0f);
    }
    printf("\n\n");

    printf("Press any key to continue . . . ");
    readKey(false);
}

void saveStats(int idx)
{
    DIR *dir = opendir("save");
    if (ENOENT == errno)
        mkdir("save", 0777);
    else if (!dir)
    {
        clearScreen();
        printf("\nFatal error\n\nUnable to access save directory.\n\nMake sure the \"save\" directory is writable.\n\nData has not been saved\n\nPress any key to continue . . . ");
        readKey(false);
        return;
    }
    char buf[32];
    snprintf(buf, 32, "save/stats%d.dat", idx + 1);
    FILE *outfile;
    outfile = fopen(buf, "wb");
    if (outfile == NULL)
    {
        clearScreen();
        printf("\nFatal error\n\nSave procedure failed\n\nMake sure the \"save\" directory is writable.\n\nData has not been saved\n\nPress any key to continue . . . ");
        readKey(false);
        return;
    }
    unsigned int checksum = crc32((char *)&playerStats[idx], sizeof(PlayerStats));
    fwrite(&checksum, 4, 1, outfile);
    fwrite(&playerStats[idx], sizeof(PlayerStats), 1, outfile);
    fclose(outfile);
}