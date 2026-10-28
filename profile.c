#include "profile.h"

PlayerProfile profiles[8];
int activeProfile = 0;

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
            "And load here later"};
    }
    fseek(infile, 4, SEEK_SET);
    fread(&header, sizeof(ProfileHeader), 1, infile);
    fclose(infile);
    header.qName[0x1F] = 0;
    header.saveDate[0x1F] = 0;
    header.saveTime[0x1F] = 0;
    return header;
}

int loadProfile(int idx)
{
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
    return 0;
}
void saveProfile(PlayerProfile profile, int idx)
{
    time_t now = time(NULL);        // Get current time
    struct tm *t = localtime(&now); // Convert to local time structure
    for (int i = 0; i < 0x20; i++)
    {
        profile.profileHeader.qName[i] = profile.options.name[i];
    }
    snprintf(profile.profileHeader.saveDate, 32, "%02d.%02d.%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    snprintf(profile.profileHeader.saveTime, 32, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
    char buf[32];
    snprintf(buf, 32, "save/profile%d.dat", idx + 1);
    FILE *outfile;
    outfile = fopen(buf, "wb");
    unsigned int checksum = crc32((char *)&profile, sizeof(PlayerProfile));
    fwrite(&checksum, 4, 1, outfile);
    fwrite(&profile, sizeof(PlayerProfile), 1, outfile);
    fclose(outfile);
    playSound("snd/save.mp3", false);
}

void saveActiveProfile()
{
    time_t now = time(NULL);        // Get current time
    struct tm *t = localtime(&now); // Convert to local time structure
    snprintf(profiles[activeProfile].profileHeader.saveDate, 32, "%02d.%02d.%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    snprintf(profiles[activeProfile].profileHeader.saveTime, 32, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
    clearScreen();
    printf("\n\nFishing C > Main menu > Save game\n\n");
    printf(" Name: %s\n Date: %s\n Time: %s\n\n Do not close the console window as it\n may lead to data corruption\n",
           profiles[activeProfile].options.name,
           profiles[activeProfile].profileHeader.saveDate,
           profiles[activeProfile].profileHeader.saveTime);
    saveProfile(profiles[activeProfile], activeProfile);
    clearScreen();
}

void initProfile(int idx)
{
    profiles[idx] = (PlayerProfile){
        (ProfileHeader){
            "Free slot",
            "Start a new game",
            "And load here later"},
        (Progression){
            "menu",
            "",
            "",
            -1,
            0,
            0,
            0,
            0,
            "",
            FIND_ME_IF_YOU_CAN},
        (Options){
            true,
            true,
            true,
            true,
            NORMAL,
            R_RED,
            "Unnamed"},
        (LivewellRecord){},
        ""};
}

void profileMenu()
{
    int sel = 0;
    ProfileHeader *headers = malloc(sizeof(ProfileHeader) * 4);
    for (int i = 0; i < 4; i++)
    {
        headers[i] = quickLoadProfile(i);
    }
    char *buf = malloc(0x70 * 4);
    while (1 < 2)
    {
        MenuItem menu[5];
        for (int i = 0; i < 4; i++)
        {
            snprintf(buf + (0x70 * i), 0x70, "%d. %s\n   %s\n   %s\n", i + 1, headers[i].qName, headers[i].saveDate, headers[i].saveTime);
            menu[i] = (MenuItem){BUTTON, buf + (0x70 * i), &dummy, -1, NULL, &loadProfile};
        }
        menu[4] = (MenuItem){BUTTON, "5. Exit\n   Please don't go :(\n   It would be sad", &dummy, -1, NULL, &loadProfile};
        printMenu("Fishing C > Profiles", menu, 5, &sel, (Color[]){BLACK, WHITE});
        if (sel == 4)
        {
            activeProfile = -1;
            break;
        }
        activeProfile = sel;
        if (loadProfile(activeProfile) != 0)
        {
            clearScreen();
            printf("\nFishing C > Profiles > Corrupted data\n\nSave data is corrupted\n\nIt looks like the save file is not valid.\n");
            printf("Because of that, game cannot use this file\nas an accurate source to get data.\n\n");
            printf("To fix this, the save file must be deleted.\nThis will delete any progress you may have\nachieved.\n\n");
            printf("Would you like to delete this data now? (Y/N) ");
            char *rkey = readKey();
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
                rkey = readKey();
            }
            continue;
        }
        break;
    }
    free(buf);
    free(headers);
}

unsigned int crc32(const char *dataPtr, size_t dataSize)
{
    unsigned int crc = 0xffffffff;
    for (size_t i = 0; i < dataSize; i++)
    {
        char byte = dataPtr[i];
        crc = crc ^ byte;
        for (char j = 8; j > 0; --j)
        {
            crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
        }
        i++;
    }
    return crc ^ 0xffffffff;
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

char *getObString(int idx)
{
    char c = 0;
    Record r;
    FILE *infile;
    infile = fopen("txt/solmn.dat", "rb");
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