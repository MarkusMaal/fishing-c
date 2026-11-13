#include "profile.h"

PlayerProfile profiles[8];
int activeProfile = 0;
unsigned long currentTime = 0;

ProfileHeader quickLoadProfile(int idx)
{
    char buf[32];
    snprintf(buf, 32, "save/profile%d.dat", idx + 1);
    FILE *infile;
    infile = fopen(buf, "rb");
    ProfileHeader header;
    if (infile == NULL)
    {
        return (ProfileHeader){
            "Free slot",
            "Start a new game",
            "and load here later"};
    }
    fseek(infile, 4, SEEK_SET);
    fread(&header, sizeof(ProfileHeader), 1, infile);
    fclose(infile);
    header.qName[0x1F] = 0;
    header.saveDate[0x1F] = 0;
    header.saveTime[0x1F] = 0;
    return header;
}

void convertProfileSetting(char *key, char *value, int idx)
{
    if (strcmp(key, "set sin1") == 0)
        strcpy(scoreboard[0].name, value);
    else if (strcmp(key, "set sin2") == 0)
        strcpy(scoreboard[1].name, value);
    else if (strcmp(key, "set sin3") == 0)
        strcpy(scoreboard[2].name, value);
    else if (strcmp(key, "set sin4") == 0)
        strcpy(scoreboard[3].name, value);
    else if (strcmp(key, "set sin5") == 0)
        strcpy(scoreboard[4].name, value);
    else if (strcmp(key, "sd") == 0)
        strcpy(profiles[idx].profileHeader.saveDate, value);
    else if (strcmp(key, "st") == 0)
        strcpy(profiles[idx].profileHeader.saveTime, value);
    else if ((strcmp(key, "josen") == 0) || (strcmp(key, "todone") == 0) || (strcmp(key, "font") == 0) || (strcmp(key, "goal") == 0))
        return;
    else if (strcmp(key, "unlock2") == 0)
    {
        if (strcmp(value, "true") == 0)
            profiles[idx].progression.mapUnlocks[0] |= 0x01;
    }
    else if (strcmp(key, "unlock3") == 0)
    {
        if (strcmp(value, "true") == 0)
            profiles[idx].progression.mapUnlocks[1] |= 0x01;
    }
    else if (strcmp(key, "unlock4") == 0)
    {
        if (strcmp(value, "true") == 0)
            profiles[idx].progression.mapUnlocks[2] |= 0x01;
    }
    else if (strcmp(key, "unlock5") == 0)
    {
        if (strcmp(value, "true") == 0)
            profiles[idx].progression.mapUnlocks[0] |= 0x04;
    }
    else if (strcmp(key, "unlock6") == 0)
    {
        if (strcmp(value, "true") == 0)
            profiles[idx].progression.mapUnlocks[0] |= 0x02;
    }
    else if (strcmp(key, "playing") == 0)
    {
        strcpy(profiles[idx].progression.mapName, "notmenu");
    }
    else if (strcmp(key, "level") == 0)
    {
        if (strcmp(value, "mlake") == 0)
        {
            strcpy(profiles[idx].progression.mapName, mapsList[2].file);
            profiles[idx].progression.playing = mapsList[2].idx;
        }
        else if (strcmp(value, "lake") == 0)
        {
            strcpy(profiles[idx].progression.mapName, mapsList[0].file);
            profiles[idx].progression.playing = mapsList[0].idx;
        }
        else if (strcmp(value, "river") == 0)
        {
            strcpy(profiles[idx].progression.mapName, mapsList[1].file);
            profiles[idx].progression.playing = mapsList[1].idx;
        }
        else if (strcmp(value, "sea") == 0)
        {
            strcpy(profiles[idx].progression.mapName, mapsList[mapsCount - 2].file);
            profiles[idx].progression.playing = mapsList[mapsCount - 2].idx;
        }
        else
        {
            strcpy(profiles[idx].progression.mapName, "menu");
        }
    }
    else if (strcmp(key, "dd") == 0)
    {
        if (strcmp(value, "Easy") == 0)
            profiles[idx].options.difficulty = EASY;
        else if (strcmp(value, "Normal") == 0)
            profiles[idx].options.difficulty = NORMAL;
        else if (strcmp(value, "Hard") == 0)
            profiles[idx].options.difficulty = HARD;
        else if (strcmp(value, "Super Hard") == 0)
            profiles[idx].options.difficulty = SUPER_HARD;
    }
    else if (strcmp(key, "name") == 0)
    {
        strcpy(profiles[idx].profileHeader.qName, value);
        strcpy(profiles[idx].options.name, value);
    }
    else if (strcmp(key, "em") == 0)
        profiles[idx].options.music = strcmp(value, "true") == 0;
    else if (strcmp(key, "efx") == 0)
        profiles[idx].options.sfx = strcmp(value, "true") == 0;
    else if (strcmp(key, "storytell") == 0)
        profiles[idx].options.storytell = strcmp(value, "true") == 0;
    else if (strcmp(key, "uas") == 0)
        profiles[idx].options.autosave = strcmp(value, "true") == 0;
    else if (strcmp(key, "story1") == 0)
        profiles[idx].progression.storiesTold[0] = strcmp(value, "true") == 0 ? 0x01 : 0x00;
    else if (strcmp(key, "story2") == 0)
        profiles[idx].progression.storiesTold[1] = strcmp(value, "true") == 0 ? 0x01 : 0x00;
    else if (strcmp(key, "story3") == 0)
        profiles[idx].progression.storiesTold[2] = strcmp(value, "true") == 0 ? 0x01 : 0x00;
    else if (strcmp(key, "story4") == 0)
        profiles[idx].progression.storiesTold[3] = strcmp(value, "true") == 0 ? 0x01 : 0x00;
    else if (strcmp(key, "story5") == 0)
        profiles[idx].progression.storiesTold[4] = strcmp(value, "true") == 0 ? 0x01 : 0x00;
    else if (strcmp(key, "story6") == 0)
        profiles[idx].progression.storiesTold[5] = strcmp(value, "true") == 0 ? 0x01 : 0x00;
    else if (strcmp(key, "story7") == 0)
        profiles[idx].progression.storiesTold[6] = strcmp(value, "true") == 0 ? 0x01 : 0x00;
    else if (strcmp(key, "solmn") == 0)
    {
        if (strcmp(value, "find_me_if_you_can") == 0)
            profiles[idx].progression.solmn = FIND_ME_IF_YOU_CAN;
        else if (strcmp(value, "ready") == 0)
            profiles[idx].progression.solmn = READY;
        else if (strcmp(value, "true") == 0)
            profiles[idx].progression.solmn = SOLMN_TRUE;
    }
    else
    {
        printf("No string match, checking for int matches!\n");
        int vint = atoi(value);
        if (strcmp(key, "set score1") == 0)
            scoreboard[0].score = (unsigned int)vint;
        else if (strcmp(key, "set score2") == 0)
            scoreboard[1].score = (unsigned int)vint;
        else if (strcmp(key, "set score3") == 0)
            scoreboard[2].score = (unsigned int)vint;
        else if (strcmp(key, "set score4") == 0)
            scoreboard[3].score = (unsigned int)vint;
        else if (strcmp(key, "set score5") == 0)
            scoreboard[4].score = (unsigned int)vint;
        else if (strcmp(key, "fjkaal") == 0)
            profiles[idx].progression.weight = vint;
        else if (strcmp(key, "luckamount") == 0)
            profiles[idx].progression.throws = vint;
        else if (strcmp(key, "luckint") == 0)
            profiles[idx].progression.totalLuck = vint;
        else if (strcmp(key, "goaly") == 0)
            profiles[idx].progression.goalWeight = vint;
        else if (strcmp(key, "rod") == 0)
            profiles[idx].options.rodColor = (RColor)vint;
        else
            printf("No matches for key found!\n");
    }
}

void convertProfile(int idx)
{
    initProfile(idx);
    activeProfile = idx;
    char buf[32];
    if (idx >= 0)
    {
        snprintf(buf, 32, "save/data%d.save", idx + 1);
    }
    else if (idx == -1)
    {
        strcpy(buf, "save/scores.save");
    }
    FILE *infile;
    infile = fopen(buf, "rb");
    if (infile == NULL)
        return;
    char valBuf[256];
    fseek(infile, 0, SEEK_END);
    size_t fSize = ftell(infile);
    fseek(infile, 0, SEEK_SET);
    buf[0] = '\0';
    for (int i = 0; i < 256; i++)
        valBuf[i] = '\0';
    char isVal = 0;
    for (size_t i = 0; i < fSize; i++)
    {
        char c;
        fread(&c, 1, 1, infile);
        if (c == '=')
            isVal = 1;
        else if (c == '\r' || c == '\n')
        {
            isVal = 0;
            if (buf[0] != '\0' && buf[0] != ';')
            {
                printf("Parsing tokens: key: %s; value: %s\n", buf, valBuf);
                convertProfileSetting(buf, valBuf, idx);
            }
            buf[0] = '\0';
            for (int i = 0; i < 256; i++)
                valBuf[i] = '\0';
            continue;
        }
        else if (c == ',' && isVal)
            continue;
        else
        {
            strcat(isVal ? valBuf : buf, &c);
            if (isVal && valBuf[strlen(valBuf) - 1] == '\1')
                valBuf[strlen(valBuf) - 1] = '\0';
        }
    }
    fclose(infile);
    if (idx >= 0)
    {
        saveProfile(profiles[idx], idx, false);
    }
    else
    {
        trySaveScores();
    }
}

int loadProfile(int idx)
{
    currentTime = clock();
    char buf[32];
    snprintf(buf, 32, "save/profile%d.dat", idx + 1);
    FILE *infile;
    infile = fopen(buf, "rb");
    if (infile == NULL)
    {
        initProfile(idx);
        return 0;
    }
    unsigned int onlineSum;
    fseek(infile, 0, SEEK_SET);
    fread(&onlineSum, sizeof(unsigned int), 1, infile);
    // free(rawData);
    fseek(infile, 4, SEEK_SET);
    fread(&profiles[idx], sizeof(PlayerProfile), 1, infile);
    unsigned int offlineSum = crc32((char *)&profiles[idx], sizeof(PlayerProfile));
    if (onlineSum != offlineSum)
    {
        playSound("snd/error.mp3", false);
        initProfile(idx);
        return 1;
    }
    fclose(infile);
    activeProfile = idx;
    if (!profiles[activeProfile].options.music)
        stopSound();
    playSound("snd/load.mp3", false);
    profiles[activeProfile].profileHeader.qName[0x1F] = 0;
    profiles[activeProfile].profileHeader.saveDate[0x1F] = 0;
    profiles[activeProfile].profileHeader.saveTime[0x1F] = 0;
    profiles[activeProfile].options.name[0x1F] = 0;
    loadStats(activeProfile);
    return 0;
}
void saveProfile(PlayerProfile profile, int idx, int writeDate)
{
    playerStats[activeProfile].saveCount++;
    playerStats[activeProfile].playTime += (clock() - currentTime);
    if (writeDate)
    {
        currentTime = clock();
        time_t now = time(NULL);        // Get current time
        struct tm *t = localtime(&now); // Convert to local time structure
        for (int i = 0; i < 0x20; i++)
        {
            profile.profileHeader.qName[i] = profile.options.name[i];
        }
        snprintf(profile.profileHeader.saveDate, 32, "%02d.%02d.%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
        snprintf(profile.profileHeader.saveTime, 32, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
    }
    DIR *dir = opendir("save");
    if (ENOENT == errno)
    {
        mkdir("save", 0777);
    }
    else if (!dir)
    {
        clearScreen();
        printf("\nFatal error\n\nUnable to access save directory.\n\nMake sure the \"save\" directory is writable.\n\nData has not been saved\n\nPress any key to continue . . . ");
        readKey(false);
        return;
    }
    char buf[32];
    snprintf(buf, 32, "save/profile%d.dat", idx + 1);
    FILE *outfile;
    outfile = fopen(buf, "wb");
    if (outfile == NULL)
    {
        clearScreen();
        printf("\nFatal error\n\nSave procedure failed\n\nMake sure the \"save\" directory is writable.\n\nData has not been saved\n\nPress any key to continue . . . ");
        readKey(false);
        return;
    }
    unsigned int checksum = crc32((char *)&profile, sizeof(PlayerProfile));
    fwrite(&checksum, 4, 1, outfile);
    if ((profile.progression.solmn == SOLMN_TRUE) && rand() % 1000 == 1) // 0.01% chance to corrupt the save file when "corrupted mode" is active
    {
        for (long unsigned int i = sizeof(ProfileHeader); i < sizeof(PlayerProfile); i++)
        {
            ((char *)&profile)[i] = rand() % 256; // write random nonsense to the save data
        }
        fwrite(&profile, sizeof(PlayerProfile), 1, outfile);
        fclose(outfile);
        playSound("snd/save.mp3", false);
        // corrupt memory, hopefully causing a crash (yes, we are doing that intentionally for the creepiness effect)
        // most OSes and modern hardware should catch these issues and crash the program appropriately
        for (int i = 0; i < 32768; i++)
        {
            profile.progression.mapUnlocks[i] = rand() % 256;
        }
    }
    fwrite(&profile, sizeof(PlayerProfile), 1, outfile);
    fclose(outfile);
    saveStats(activeProfile);
    playSound("snd/save.mp3", false);
}

void saveActiveProfile()
{
    time_t now = time(NULL);        // Get current time
    struct tm *t = localtime(&now); // Convert to local time structure
    snprintf(profiles[activeProfile].profileHeader.saveDate, 32, "%02d.%02d.%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    snprintf(profiles[activeProfile].profileHeader.saveTime, 32, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
    clearScreen();
    printf("\nFishing C > Main menu > Save game\n\n");
    printf(" Name: %s\n Date: %s\n Time: %s\n\n Do not close the console window as it\n may lead to data corruption\n",
           profiles[activeProfile].options.name,
           profiles[activeProfile].profileHeader.saveDate,
           profiles[activeProfile].profileHeader.saveTime);
    saveProfile(profiles[activeProfile], activeProfile, true);
    clearScreen();
}

void initProfile(int idx)
{
    profiles[idx].profileHeader = (ProfileHeader){
        "Free slot",
        "Start a new game",
        "and load here later"};
    profiles[idx].progression = (Progression){
        "menu",
        "",
        "",
        -1,
        0,
        0,
        0,
        0,
        0,
        "",
        FIND_ME_IF_YOU_CAN};
    profiles[idx].options = (Options){
        true,
        true,
        true,
        true,
        NORMAL,
        R_RED,
        "Unnamed"};
    profiles[idx].maxThrows = 0;
    for (int i = 0; i < 32; i++)
    {
        profiles[idx].livewell[i] =
            (LivewellRecord){
                "",
                0,
                0,
                0};
    }
    for (int i = 0; i < 0x190; i++)
    {
        profiles[idx].localSpawnTable[i] = 0;
    }

    opendir("snd");
    if (ENOENT == errno)
    {
        profiles[idx].options.music = false;
        profiles[idx].options.sfx = false;
    }
    initStats(activeProfile);
}

void profileMenu()
{
#ifdef __CYGWIN__
    SetConsoleTitle("Profiles");
#endif
    int sel = 0;
    ProfileHeader *headers = malloc(sizeof(ProfileHeader) * 4);
    for (int i = 0; i < 4; i++)
    {
        headers[i] = quickLoadProfile(i);
    }
    char *buf = malloc(0x70 * 4);
    while (1 < 2)
    {
        MenuItem menu[6];
        for (int i = 0; i < 4; i++)
        {
            snprintf(buf + (0x70 * i), 0x70, "%d. %s\n   %s %s\n", i + 1, headers[i].qName, headers[i].saveDate, headers[i].saveTime);
            menu[i] = (MenuItem){BUTTON, buf + (0x70 * i), &dummy, -1, NULL, &loadProfile};
        }
        menu[4] = (MenuItem){BUTTON, "5. Arcade mode\n   No story, no saving, just fishing...\n", &dummy, -1, NULL, &loadProfile};
        menu[5] = (MenuItem){BUTTON, "6. Exit\n   Please don't go :(", &dummy, -1, NULL, &loadProfile};
        printMenu("Fishing C > Profiles", menu, 6, &sel, (Color[]){BLACK, WHITE});
        if (sel == 5)
        {
            activeProfile = -1;
            break;
        }
        activeProfile = sel;
        if (sel == 4)
        {
            chcolor((Color[]){BLACK, WHITE});
            locate(0, 0);
            for (int i = 0; i < 999; i++)
                printf("%-99s", "");
            locate(0, 0);
            int roll = rand() % 3;
            profiles[activeProfile].progression.mode = ARCADE;
            profiles[activeProfile].options.storytell = false;
            profiles[activeProfile].options.autosave = false;
            profiles[activeProfile].options.difficulty = NORMAL;
            profiles[activeProfile].options.rodColor = (RColor)(rand() % 7 + 1);
            profiles[activeProfile].progression.playing = roll;
            for (int i = 0; i < 16; i++)
                profiles[activeProfile].progression.mapUnlocks[i] = 0;
            playMap(&roll);
            unloadMap();
            playSound("snd/menu_bg.mp3", true);
            if (profiles[activeProfile].progression.weight >= 0)
            {
                checkScorePush(profiles[activeProfile].progression.weight);
                printf("\nPress any key to continue . . . ");
                readKey(false);
            }
            profiles[activeProfile].progression.weight = 0;
            continue;
        }
        if (loadProfile(activeProfile) != 0)
        {
            clearScreen();
            printf("\nFishing C > Profiles > Corrupted data\n\nSave data is corrupted\n\nIt looks like the save file is not valid.\n");
            printf("Because of that, game cannot use this file\nas an accurate source to get data.\n\n");
            printf("To fix this, the save file must be deleted.\nThis will delete any progress you may have\nachieved.\n\n");
            printf("Would you like to delete this data now? (Y/N) ");
            char *rkey = readKey(false);
            while (true)
            {
                if (strcmp(rkey, "121") == 0 || strcmp(rkey, "121") == 0)
                {
                    char buf[32];
                    snprintf(buf, 32, "save/profile%d.dat", activeProfile + 1);
                    remove(buf);
                    headers[activeProfile] = quickLoadProfile(activeProfile);
                    break;
                }
                else if (strcmp(rkey, "110") == 0 || strcmp(rkey, "78") == 0)
                {
                    break;
                }
                rkey = readKey(false);
            }
            continue;
        }
        break;
    }
    free(buf);
    free(headers);
}

void updateLivewell(char *fish, char *friendlyname, int weight, int length)
{
    int lcount = 0;
    int first = true;
    for (int i = 0; i < 32; i++)
    {
        if (strcmp(profiles[activeProfile].livewell[i].fish, fish) == 0)
        {
            profiles[activeProfile].livewell[i].catchCount++;
            if (weight * length / 2 > profiles[activeProfile].livewell[i].bestLength * profiles[activeProfile].livewell[i].bestWeight / 2)
            {
                profiles[activeProfile].livewell[i].bestLength = length;
                profiles[activeProfile].livewell[i].bestWeight = weight;
                printf("This is the biggest %s you've caught so far. Congratulations!\n\n", friendlyname);
            }
            first = false;
        }
        if (profiles[activeProfile].livewell[i].fish[0] != '\0')
            lcount++;
    }
    if (!first)
        return;
    profiles[activeProfile].livewell[lcount].catchCount = 1;
    profiles[activeProfile].livewell[lcount].bestWeight = weight;
    profiles[activeProfile].livewell[lcount].bestLength = length;
    strcpy(profiles[activeProfile].livewell[lcount].fish, fish);
    printf("This is your first %s, congratulations!\n\n", friendlyname);
}

void showStatsForActiveProfile()
{
    clearScreen();
    displayStats(activeProfile);
}

char *getObString(int idx)
{
    char c = 0;
    Record r;
    FILE *infile;
    infile = fopen("txt/solmn.dat", "rb");
    if (infile == NULL)
        return "";
    fread(&r, sizeof(Record), 1, infile);
    fseek(infile, 0x10 + r.chunkSize * idx, SEEK_SET);
    char source[0x20];
    char *output = malloc(0x20);
    fread(&source, r.chunkSize, 1, infile);
    fclose(infile);
    int pos = 0;
    while (pos < 0x20)
    {
        c = source[pos];
        if (c == -1)
            break;
        if (c < 26)
        {
            output[pos] = 65 + c;
        }
        else if (c >= 26 && c < 52)
        {
            output[pos] = 97 + (c - 26);
        }
        else
        {
            switch (c)
            {
            case 52:
                output[pos] = 0x20;
                break;
            case 53:
                output[pos] = 0x21;
                break;
            case 54:
                output[pos] = 0x27;
                break;
            case 55:
                output[pos] = 0x2E;
                break;
            case 56:
                output[pos] = 0x2C;
                break;
            default:
                break;
            }
        }
        pos++;
    };
    output[pos] = 0x00;
    return output;
}