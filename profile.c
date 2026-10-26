#include "profile.h"

PlayerProfile profiles[8];
int activeProfile = 0;

void loadProfile(int idx)
{
    char buf[32];
    snprintf(buf, 32, "save/profile%d.dat", idx + 1);
    FILE *infile;
    infile = fopen(buf, "rb");
    if (infile == NULL)
    {
        initProfiles();
        return;
    }
    fread(&profiles[idx], sizeof(PlayerProfile), 1, infile);
    fclose(infile);
    activeProfile = idx;
    playSound("snd/load.mp3", false);
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

void initProfiles()
{
    char zero16[16];
    char zero20[20];
    for (int i = 0; i < 16; i++)
    {
        zero16[i] = 0;
    }
    for (int i = 0; i < 20; i++)
    {
        zero20[i] = 0;
    }
    for (int i = 0; i < 8; i++)
    {
        profiles[i] = (PlayerProfile){
            (ProfileHeader){
                "Free slot",
                "Start a new game",
                "And load here later"},
            (Progression){
                "menu",
                "",
                "",
                false,
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
                RED,
                "Unnamed"}};
    }
}