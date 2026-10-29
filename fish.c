#include "fish.h"
FishData loadFish(char *fileName)
{

    FILE *infile;
    infile = fopen(fileName, "rb");
    if (infile == NULL)
        return (FishData){};
    Record rStruct;
    FishData fish;
    int c;
    while ((c = fgetc(infile)) != EOF)
    {
        fseek(infile, -1, SEEK_CUR);
        fread(&rStruct, sizeof(Record), 1, infile);
        int next = ftell(infile) + rStruct.total * 0x10;
        if (strcmp(rStruct.label, "FSHDATA") == 0)
        {
            if (fread(&fish, rStruct.chunkSize, rStruct.chunkCount, infile) != 1)
            {
                printf("Error reading fish data\n");
                readKey(false);
            }
        }
        fseek(infile, next, SEEK_SET);
    }
    return fish;
}
LivewellData loadFishMeta(char *fileName)
{

    FILE *infile;
    infile = fopen(fileName, "rb");
    Record rStruct;
    LivewellData lwell = (LivewellData){
        "MISSINGNAME",
        0,
        COMMON,
        "", ""};
    if (strcmp(profiles[activeProfile].progression.mapName, "menu"))
        strcpy(lwell.name, fileName);
    if (infile == NULL)
        return lwell;
    int c;
    while ((c = fgetc(infile)) != EOF)
    {
        fseek(infile, -1, SEEK_CUR);
        fread(&rStruct, sizeof(Record), 1, infile);
        int next = ftell(infile) + rStruct.total * 0x10;
        if (strcmp(rStruct.label, "LWELL") == 0)
        {
            if (fread(&lwell, sizeof(LivewellData), 1, infile) != 1)
            {
                printf("Error reading fish data\n");
                readKey(false);
            }
        }
        fseek(infile, next, SEEK_SET);
    }
    return lwell;
}

int *fightFish(FishData fish, int luck, PlayerDifficulty difficulty, char *fileName)
{
    if (fish.fightLength == 0)
    {
        clearScreen();
        playSound("snd/error.mp3", false);
        printf("\nFatal error\n\nCannot access file \"%s\"\n\nNo result data will be returned from this catch\n\nPress any key to continue . . . ", fileName);
        readKey(false);
        return (int[]){0, 0};
    }
    int fishX = fish.spawnX;
    int fishY = fish.spawnY;
    int weight = (float)luck / 100.0f * (fish.weightMax - fish.weightMin) + fish.weightMin;
    int length = (float)luck / 100.0f * (fish.lengthMax - fish.lengthMin) + fish.lengthMin;
    int life = fish.tolerance;
    char *llastKey = malloc(0x10);
    char *lastKey = malloc(0x10);
    char lastKeyId = 0;
    char llastKeyId = 0;
    char pad[fishX + 1];
    for (int i = 0; i < fish.fightLength; i++)
    {
        // return to cycle start in case of bad luck (mainly for boss fish)
        if (fish.cycleLuck != 0 && i == fish.cycleStart + fish.cycleLength)
        {
            int roll = rand() % 101;
            roll += (int)((float)luck / 100.0f * 10.0f);
            if (difficulty == P_SUPER_HARD)
                roll -= 10;
            else if (difficulty == P_HARD)
                roll -= 5;
            else if (difficulty == P_EASY)
                roll += 2;
            if (roll > fish.cycleLuck)
            {
                // printf("CYCLE!");
                i = fish.cycleStart;
                continue;
            }
        }

        // start padding based on fish position

        // draw fish at current position
        clearScreen();
        locate(0, 0);
        for (int y = 0; y < fishY; y++)
        {
            printf("        \n");
        }
        int gI = fish.attackPatternGeometry[i];
        for (int cI = 0; cI < fishX; cI++)
            pad[cI] = ' ';
        pad[fishX] = '\0';
        int fishTempY = fishY;
        for (int r = 0; r < 16; r++)
        {
            if (fishTempY < 0)
            {
                fishTempY++;
                continue;
            }
            if (fish.geometry[gI][r][0] != '\0')
            {
                printf("%s%s", strlen(pad) != 0 ? pad : "", fish.geometry[gI][r]);
            }
        }
        char requestedInput = fish.attackPatternInputs[i];
        int remainder;
        switch (difficulty)
        {
        case P_EASY:
            remainder = 2;
            break;
        case P_NORMAL:
            remainder = 3;
            break;
        case P_HARD:
            remainder = 7;
            break;
        case P_SUPER_HARD:
            remainder = 8;
            break;
        }
        if (fish.attackPatternInputs[i] == 8)
        {
            requestedInput = rand() % remainder;
        }
        if (fish.attackPatternInputs[i] == 9)
        {
            requestedInput = lastKeyId;
        }
        if (fish.attackPatternInputs[i] == 9)
        {
            requestedInput = lastKeyId;
        }
        if (fish.attackPatternInputs[i] == 12)
        {
            requestedInput = llastKeyId;
        }
        char *inputLabels[0x20] = {
            "Press C to catch",
            "Press F to catch",
            "Press T to catch",
            "Press W to catch",
            "Press A to catch",
            "Press S to catch",
            "Press D to catch",
            "Press N to catch",
            "",
            "",
            "Hold any key to catch",
            ""};
        if (strcmp(inputLabels[(int)requestedInput], "") != 0)
            printf("\n\n%s:", inputLabels[(int)requestedInput]);
        if (requestedInput == NONE) // mainly for the "catching fish" animation
        {
            usleep(66667);
            goto skipread;
        }
        char *rKey = "1";
        long start_time = clock();
        if (profiles[activeProfile].progression.mode == DEMO)
            sleep(1);
        if (profiles[activeProfile].progression.mode != DEMO)
        {
            while (true)
            {
                rKey = readKey(true);
                if (strcmp(rKey, "1") != 0)
                    break;
                if ((clock() - start_time) / CLOCKS_PER_SEC * 1000 > (2000 - (int)profiles[activeProfile].options.difficulty * 500))
                    break;
                fflush(stdout);
            }
        }
        else if (rand() % 4 == 1)
        {
            goto skipread;
        }
        switch (requestedInput)
        {
        case ANY:
        case RANDOM:
            break;
        case LAST:
            if (strcmp(llastKey, rKey) != 0)
                life--;
            break;
        case BEFORELAST:
            if (strcmp(lastKey, rKey) != 0)
                life--;
            break;
        case C:
            if (strcmp("99", rKey) != 0)
                life--;
            break;
        case F:
            if (strcmp("102", rKey) != 0)
                life--;
            break;
        case T:
            if (strcmp("116", rKey) != 0)
                life--;
            break;
        case W:
            if (strcmp("119", rKey) != 0)
                life--;
            break;
        case A:
            if (strcmp("97", rKey) != 0)
                life--;
            break;
        case S:
            if (strcmp("115", rKey) != 0)
                life--;
            break;
        case D:
            if (strcmp("100", rKey) != 0)
                life--;
            break;
        }
    skipread:
        if (life <= 0)
        {
            playSound("snd/negative.mp3", false);
            fishY = 0;
            while (fish.attackDir == LEFT ? fishX > -64 : 2 < 1)
            {
                clearScreen();
                fishX -= 2;
                for (int cI = 0; cI < fishX; cI++)
                    pad[cI] = ' ';
                pad[fishX] = '\0';
                int lenSum = 0;
                for (int r = 0; r < 16; r++)
                {
                    if (fishX >= 0)
                    {
                        printf("%s%s", pad, fish.geometry[gI][r]);
                        lenSum = 1;
                    }
                    else
                    {
                        lenSum += strlen(fish.geometry[gI][0] + (-fishX));
                        printf("%s%s", pad, fish.geometry[gI][r] + (-fishX));
                    }
                }
                if (lenSum == 0)
                    break;
                usleep(66667);
            }
            // free(lastKey);
            clearScreen();
            printf("\n\n    Oh no\n    you lost the fish!\n\n\n");
            printf("Press RETURN to continue . . . ");
            while (profiles[activeProfile].progression.mode != DEMO && strcmp(readKey(false), "return") != 0)
            {
            }
            if (profiles[activeProfile].progression.mode == DEMO)
                sleep(1);
            return (int[]){0, 0};
        }
        if ((char)fish.attackPatternSteps[i] > -1)
        {
            fishX += (fish.attackDir == LEFT ? -1 : 1) * fish.attackPatternSteps[i];
        }
        else
        {
            fishY += (char)fish.attackPatternSteps[i];
        }
        llastKeyId = lastKeyId;
        lastKeyId = requestedInput;
        llastKey = lastKey;
        lastKey = rKey;
    }
    clearScreen();
    playSound("snd/positive.mp3", false);
    locate(0, 0);
    printf("         \n\n   You got a\n    fish!!!\n\n\n\n");
    usleep(1200000);
    clearScreen();
    for (int i = 0; i < 0x10; i++)
    {
        printf((char *)fish.geometry[3][i], weight, length);
    }
    printf("\n");
    LivewellData fishMeta = loadFishMeta(fileName);
    if (strcmp(fishMeta.name, "MISSINGNAME") != 0 && profiles[activeProfile].progression.mode != ARCADE)
    {
        updateLivewell(fileName, fishMeta.name, weight * (fish.largeUnits ? 1000 : 1), length);
    }
    printf("Press RETURN continue . . . ");
    while (profiles[activeProfile].progression.mode != DEMO && strcmp(readKey(false), "return") != 0)
    {
    }
    if (profiles[activeProfile].progression.mode == DEMO)
        sleep(1);
    clearScreen();
    return (int[]){weight, length};
}