#include "utils.h"
#include "profile.h"

MapsDef *mapsList = NULL;
int mapsCount = 0;
int dummy = 0;
t_sound *bgm = NULL;
pthread_t bgmThread;
pthread_t kbdThread;
volatile int thread_exit = 1;
volatile int kthread_exit = -1;
int enableDevMenu = false;

char *readKey(int timeout)
{
    rand();
    char c;
    struct termios old, t;
    char special;
    tcgetattr(STDIN_FILENO, &old);
    t = old;
    cfmakeraw(&t);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    if (timeout != 0)
    {
        kthread_exit = 1;
        pthread_create(&kbdThread, NULL, waitKey, NULL);
        usleep(1);
        pthread_cancel(kbdThread);
        c = kthread_exit;
        kthread_exit = 0;
    }
    else
    {
        c = getc(stdin);
    }
    special = 0;
    if (c == 13)
        special = 1;
    if (c == 32)
        special = 1;
    if (c == 127)
        special = 1;
    if (c == 27)
    {
        getc(stdin);
        c = getc(stdin);
        special = 1;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    char *str = malloc(128);

    if (!special)
    {
        snprintf(str, 128, "%d", c);
        return str;
    }
    else
    {
        switch (c)
        {
        case 13:
            str = "return";
            break;
        case 32:
            str = "space";
            break;
        case 65:
            str = "up";
            break;
        case 66:
            str = "down";
            break;
        case 67:
            str = "right";
            break;
        case 68:
            str = "left";
            break;
        case 80:
            str = "f1";
            break;
        case 81:
            str = "f2";
            break;
        case 82:
            str = "f3";
            break;
        case 83:
            str = "f4";
            break;
        case 127:
            str = "backspace";
            break;
        default:
            snprintf(str, 128, "%c", c);
            break;
        }
    }
    return str;
}

void chcolor(Color *colors)
{
    Color bg = *colors;
    Color fg = colors[1];
    int bgI = 40 + (int)bg;
    int fgI = 30 + (int)fg;
    if (bgI > 47)
        bgI += 52;
    if (fgI > 37)
        fgI += 52;
    printf("\x1b[%d;%dm", bgI, fgI);
}

void cprintf(char *string, Color *colors)
{
    Color bg = *colors;
    Color fg = colors[1];
    Color rbg = colors[2];
    Color rfg = colors[3];
    int bgI = 40 + (int)bg;
    int fgI = 30 + (int)fg;
    int rbgI = 40 + (int)rbg;
    int rfgI = 30 + (int)rfg;
    if (bgI > 47)
        bgI += 52;
    if (fgI > 37)
        fgI += 52;
    if (rbgI > 47)
        rbgI += 52;
    if (rfgI > 37)
        rfgI += 52;
    char *ansiSeq = malloc(30 + strlen(string));
    snprintf(ansiSeq, 27, "\x1b[%d;%dm", bgI, fgI);
    char *ansiEnd = malloc(10);
    snprintf(ansiEnd, 27, "\x1b[%d;%dm", rbgI, rfgI);
    strcat(ansiSeq, string);
    strcat(ansiSeq, ansiEnd);
    printf("%s", ansiSeq);
    free(ansiEnd);
    free(ansiSeq);
}

void clearScreen()
{
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 11);
}

void *printMenu(char *title, MenuItem *items, int max, int *sel, Color *defaultColors)
{
    void *actionPtr = NULL;
    char *strAlloc;
    char *bar = malloc(10);
    int lSel = *sel;
    int j;
    char *keysPressed;
    keysPressed = malloc(64);
    for (int i = 0; i < 64; i++)
        keysPressed[i] = '\0';
    while (1 < 2)
    {
        clearScreen();
        printf("\n%s\n\n", title);
        for (int i = 0; i < max; i++)
        {
            char *txtBuf = malloc(strlen(items[i].text) + 1);
            strcpy(txtBuf, items[i].text);
            if (profiles[activeProfile].progression.solmn == TRUE)
            {
                srand((size_t)items[i].text); // use memory address of the original string as the seed
                for (size_t i = 0; i < strlen(txtBuf); i++)
                {
                    if (rand() % 4 == 1)
                        txtBuf[i] += rand() % 128 - 64;
                }
            }
            switch (items[i].cl)
            {
            case BUTTON:
                printf("%s %s", lSel == i ? "->" : "  ", txtBuf);
                break;
            case DISABLED_BUTTON:
                strAlloc = malloc(strlen(txtBuf) + 3);
                snprintf(strAlloc, 128, "%s %s", lSel == i ? "->" : "  ", txtBuf);
                cprintf(strAlloc, (Color[]){*defaultColors, LBLACK, *defaultColors, defaultColors[1]});
                free(strAlloc);
                break;
            case CHOICE:
                if (*items[i].value > max - 1)
                {
                    *items[i].value = max - 1;
                }
                else if (*items[i].value < 0)
                {
                    *items[i].value = 0;
                }
                printf("%s %s: %s %s", lSel == i ? "->" : "  ", txtBuf, items[i].choices[*items[i].value], lSel == i ? "< >" : "");
                break;
            case SELECTOR:
                printf("%s %s: %d %s", lSel == i ? "->" : "  ", txtBuf, *items[i].value + 1, lSel == i ? "< >" : "");
                break;
            case CHECKBOX:
                printf("%s [%c] %s", lSel == i ? "->" : "  ", *items[i].value & 0x01 ? 'X' : ' ', txtBuf);
                break;
            case SLIDER:
                for (j = 0; j < 10; j++)
                    bar[j] = ' ';
                for (j = 0; j < *items[i].value * 10 / items[i].maxValue; j++)
                    bar[j] = '-';
                if (lSel == i)
                {
                    bar[*items[i].value] = '|';
                }
                printf("%s %s: [%s] %d", lSel == i ? "->" : "  ", txtBuf, bar, *items[i].value);
                break;
            default:
                break;
            }
            printf("\n");
            free(txtBuf);
        }
        char key[10];
        strcpy(key, readKey(false));
        strcat(keysPressed, key);
        if (strcmp(keysPressed, "upupdowndownleftrightleftright9897return") == 0)
        {
            playSound(enableDevMenu ? "snd/error.mp3" : "snd/positive.mp3", 0);
            enableDevMenu = !enableDevMenu;
        }
        else if (strcmp(keysPressed, "upupdowndownupupdowndown9897return") == 0)
        {
            switch (profiles[activeProfile].progression.solmn)
            {
            case FIND_ME_IF_YOU_CAN:
                profiles[activeProfile].progression.solmn = READY;
                playSound("snd/positive.mp3", 0);
                break;
            case READY:
                profiles[activeProfile].progression.solmn = TRUE;
                playSound("snd/error.mp3", 0);
                break;
            case TRUE:
                profiles[activeProfile].progression.solmn = FIND_ME_IF_YOU_CAN;
                playSound("snd/negative.mp3", 0);
                break;
            }
        }
        if (strlen(keysPressed) >= 55)
        {
            free(keysPressed);
            keysPressed = malloc(64);
            for (int i = 0; i < 64; i++)
                keysPressed[i] = '\0';
        }
        if ((strcmp(key, "return") == 0) && items[lSel].cl == BUTTON)
        {
            playSound("snd/submenu.mp3", 0);
            *sel = lSel;
            free(bar);
            return items[lSel].action;
        }
        else if ((strcmp(key, "return") == 0) && items[lSel].cl == DISABLED_BUTTON)
        {
            playSound("snd/error.mp3", 0);
        }
        else if (((strcmp(key, "return") == 0) || (strcmp(key, "space") == 0)) && items[lSel].cl == CHECKBOX)
        {
            playSound("snd/click.mp3", 0);
            *(items[lSel].value) = *(items[lSel].value) > 0 ? 0 : 1;
            ((void (*)(int *))items[lSel].action)(&lSel);
        }
        else if (strcmp(key, "down") == 0)
        {
            if (lSel < max - 1)
                lSel++;
            else
                lSel = 0;
            playSound("snd/selection.mp3", 0);
        }
        else if (strcmp(key, "up") == 0)
        {
            if (lSel > 0)
                lSel--;
            else
                lSel = max - 1;
            playSound("snd/selection.mp3", 0);
        }
        else if ((strcmp(key, "right") == 0) && (int)items[lSel].cl > 1 && items[lSel].cl != CHECKBOX)
        {
            (*items[lSel].value)++;
            playSound("snd/click.mp3", 0);
        }
        else if ((strcmp(key, "left") == 0) && (int)items[lSel].cl > 1 && items[lSel].cl != CHECKBOX)
        {
            (*items[lSel].value)--;
            playSound("snd/click.mp3", 0);
        }
        else
        {
            // playSound("snd/error.mp3", 0);
        }
        if (*items[lSel].value >= items[lSel].maxValue)
            *items[lSel].value = 0;
        if (*items[lSel].value < 0)
            *items[lSel].value = items[lSel].maxValue - 1;
    }
    *sel = lSel;
    free(bar);
    return actionPtr;
}

void initSoundEngine()
{
    init_ao();
}

void *waitKey(void *arg)
{
    if (arg != NULL)
        return NULL;
    kthread_exit = getc(stdin);
    return NULL;
}
void *loopSound(void *arg)
{
    bgm = play_async(arg, 100.0);
    while (get_state(bgm) == PLAYING)
    {
        sleep(1);
    }
    if (thread_exit == 0)
        return NULL;
    return loopSound(arg);
}

void playSound(char *fileName, char loop)
{
    if (loop)
    {
        if (profiles[activeProfile].progression.solmn == TRUE && rand() % 8 == 1)
            return;
        if (!profiles[activeProfile].options.music)
            return;
        thread_exit = 1;
        pthread_create(&bgmThread, NULL, loopSound, fileName);
    }
    else
    {
        if (!profiles[activeProfile].options.sfx)
            return;
        play_async(fileName, 100.0);
    }
}

void stopSound()
{
    thread_exit = 0;
    /*if (profiles[activeProfile].options.music)
    {*/
    running_sounds(STOP, bgm);
    //}
}

void unloadSoundEngine()
{
    destroy_ao();
}

void getMaps()
{
    FILE *infile;
    infile = fopen("maps/maps.def", "rb");
    if (infile == NULL)
    {
        mapsCount = 0;
        return;
    }
    Record rStruct;
    if (fread(&rStruct, sizeof(Record), 1, infile) != 1)
    {
        fclose(infile);
        printf("Error reading maps data\n\n");
        return;
    }
    if (strcmp(rStruct.label, "MAPDEF"))
    {
        fclose(infile);
        printf("Read \"%s\", expected \"MAPDEF\"\n", rStruct.label);
        return;
    }
    mapsCount = rStruct.chunkCount;
    printf("%d\n", mapsCount);
    mapsList = malloc(rStruct.chunkCount * rStruct.chunkSize);
    for (int i = 0; i < rStruct.chunkCount; i++)
    {
        if (fread(&mapsList[i], sizeof(MapsDef), 1, infile) != 1)
        {
            printf("Error parsing chunk data");
            break;
        }
    }
    fclose(infile);
}

void locate(int row, int col)
{
    printf("\033[%d;%dH", row, col);
}

char *strrpl(char *str, char *find, char *replace)
{
    int i;
    char *pt = strstr(str, find);
    char *firstStr;
    if (pt == NULL)
    {
        printf("cannot find string \n");
        return NULL;
    }

    firstStr = (char *)malloc(100 * sizeof(char));

    // copy just until i find what i need to replace
    // i tried to specify the length of firstStr just with pt - str

    strncpy(firstStr, str, strlen(str) - strlen(pt));
    strcat(firstStr, replace);
    strcat(firstStr, pt + strlen(find));

    for (i = 0; i < (int)strlen(firstStr); i++)
        str[i] = firstStr[i];
    return str;
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