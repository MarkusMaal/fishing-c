#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// for generating spawn/luck tables
int main()
{
    FILE *outfile;
    srand(time(NULL));
    int r = rand();
    int *gen = malloc(1600);
    int *luck = malloc(1600);
    for (int i = 0; i < 400; i++)
    {
        r = rand() % 10;
        if (r < 3)
            gen[i] = 3;
        else if (r >= 3 && r < 5)
            gen[i] = 1;
        else if (r == 5)
            gen[i] = 4;
        else if (r == 6 || r == 7)
            gen[i] = 5;
        else
            gen[i] = rand() % 100 < 50 ? (r == 8 ? (rand() % 50 > 24 ? 2 : 6) : 255) : (r == 8 ? 6 : 0);
        if (gen[i] == 0 || gen[i] == 255)
        {
            luck[i] = 0;
        }
        else
        {
            luck[i] = rand() % 128;
        }
    }
    outfile = fopen("gen.bin", "wb");
    for (int i = 0; i < 400; i++)
    {
        fwrite(&gen[i], 1, 1, outfile);
    }
    for (int i = 0; i < 400; i++)
    {
        fwrite(&luck[i], 1, 1, outfile);
    }
    printf("Saved as gen.bin\n");
    free(gen);
    free(luck);
    fclose(outfile);
    return 0;
}