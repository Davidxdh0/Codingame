#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
typedef struct s_mountain	t_mountain;

struct s_mountain
{
	int	height; //Unique identifier for each entity
};

/**
 * The while loop represents the game.
 * Each iteration represents a turn of the game
 * where you are given inputs (the heights of the mountains)
 * and where you have to print an output (the index of the mountain to fire on)
 * The inputs you are given are automatically updated according to your last actions.
 **/

int main()
{
    t_mountain	*mountain;

    int j;
    int max;
    j = 0;
    max = 0;
    int k;
    k = 0;
    // game loop
    while (j < 8) 
    {
        j = 0;
        for (int i = 0; i < 8; i++) 
        {
            // represents the height of one mountain.
            int mountain_h;
            scanf("%d", &mountain[i].height);
        }
        while(j < 8)
        {
            
            if (mountain[j].height > max)
            {
                max = mountain[j].height;
                k = j;
            }
            j++;
        }   
        // Write an action using printf(). DON'T FORGET THE TRAILING \n
        // To debug: fprintf(stderr, "Debug messages...\n");
        // fprintf(stderr, "height = %d", mountain[k].height);
        // fprintf(stderr, "sukkel %d\n", k); // The index of the mountain to fire on.
        printf("%d\n", k);
    }
    return 0;
}