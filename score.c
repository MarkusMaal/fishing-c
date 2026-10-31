#include "score.h"

ScoreEntry scoreboard[5];

void getScores()
{
    FILE *scoreFile = fopen("save/scores.dat", "rb");
    if (scoreFile == NULL)
    {
        initScores();
        trySaveScores();
        return;
    }
    unsigned int storedsum;
    fread(&storedsum, sizeof(unsigned int), 1, scoreFile);
    fseek(scoreFile, 4, SEEK_SET);
    ScoreEntry storedScores[5];
    fread(&storedScores, sizeof(ScoreEntry) * 5, 1, scoreFile);
    if (crc32((char *)storedScores, sizeof(ScoreEntry) * 5) != storedsum) // score table is corrupt, disregard it
    {
        initScores();
        remove("save/scores.dat");
        return;
    }
    memcpy(scoreboard, storedScores, sizeof(ScoreEntry) * 5);
}

void initScores()
{
    for (size_t i = 0; i < sizeof(ScoreEntry) * 5; i++)
        ((char *)scoreboard)[i] = '\0';
    *scoreboard = (ScoreEntry){"Jason", 20520};
    scoreboard[1] = (ScoreEntry){"John", 16110};
    scoreboard[2] = (ScoreEntry){"Peter", 16020};
    scoreboard[3] = (ScoreEntry){"Jack", 12010};
    scoreboard[4] = (ScoreEntry){"Rose", 10010};
    return;
}

void trySaveScores()
{
    opendir("save");
    if (ENOENT == errno)
    {
        mkdir("save", 0777);
    }
    unsigned int csum = crc32((char *)scoreboard, sizeof(ScoreEntry) * 5);
    FILE *scoreFile = fopen("save/scores.dat", "wb");
    fwrite(&csum, sizeof(unsigned int), 1, scoreFile);
    fwrite(scoreboard, sizeof(ScoreEntry) * 5, 1, scoreFile);
    fclose(scoreFile);
}

void showScores()
{
    getScores();
    clearScreen();
    printf("\nFishing C > High scores\n\n");
    for (int i = 0; i < 5; i++)
        printf(" %d. %s\n    %.2f kg\n", i + 1, scoreboard[i].name, (float)scoreboard[i].score / 1000.0f);
}

void checkScorePush(unsigned int newScore)
{
    getScores();
    for (int i = 0; i < 5; i++)
    {
        if ((scoreboard[i]).score < newScore)
        {
            ScoreEntry newScoreEntry;
            clearScreen();
            printf("\nFishing C > High scores\n\nYou've go a high score!\n\nEnter your name: ");
            int nI = 0;
            char c;
            char *name = malloc(0x20);
            do
            {
                scanf("%c", &c);
                if (c == 8 && nI > 0)
                {
                    name[nI] = '\0';
                    name[nI - 1] = '\0';
                    nI--;
                }
                if (nI < 0x1F && c != 8)
                {
                    name[nI] = c != '\n' ? c : '\0';
                    nI++;
                }
            } while (c != '\n');
            strcpy(newScoreEntry.name, name);
            free(name);
            newScoreEntry.score = newScore;
            memcpy(&scoreboard[i + 1], &scoreboard[i], sizeof(ScoreEntry) * (4 - i)); // shift each scoreboard entry down by 1 dep on the score
            scoreboard[i] = newScoreEntry;
            trySaveScores();
            break;
        }
    }
    showScores();
}