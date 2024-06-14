#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct s_CostOfMove {
    int right[4];
    int down[4];
    int left[4];
    int up[4];
    int bestMove;
} t_CostOfMove;

typedef struct s_Game {
	char gpu[65];
	int* obstacles;
	int obstacles_pos[3];
	int id;
	int reg_0;
	int reg_1;
	int reg_2;
	int reg_3;
	int reg_4;
	int reg_5;
	int reg_6;
} t_Game;

void printObstacles(int *obstacles)
{
    int i = 0;
    fprintf(stderr, "obstacles: ");
    while (obstacles[i] && obstacles[i] != -1)
    {
        fprintf(stderr, "%d, ", obstacles[i]);
        i++;
    }
    fprintf(stderr, "\n");
}

void printRound(t_CostOfMove *ledger, t_Game *game)
{
	fprintf(stderr, "[%d]:right %d, ", game->id, ledger->right[game->id]);
	fprintf(stderr, "down %d, ", ledger->down[game->id]);
	fprintf(stderr, "left %d, ", ledger->left[game->id]);
	fprintf(stderr, "up %d\n", ledger->up[game->id]);

}

void debug(t_Game *game)
{
	int i = 1;
    // fprintf(stderr, "gpu: %s\n", game.gpu);
	if (game->obstacles[0])
	{
		fprintf(stderr, "obst: %i", game->obstacles[0]);
		while(game->obstacles[i])
		{
			fprintf(stderr, ", %d", game->obstacles[i]);
			i++;
		}
	}
    fprintf(stderr, "\nr0: %i, ", game->reg_0);
    fprintf(stderr, "r1: %i, ", game->reg_1);
    fprintf(stderr, "r2: %i, ", game->reg_2);
    fprintf(stderr, "r3: %i, ", game->reg_3);
    fprintf(stderr, "r4: %i, ", game->reg_4);
    fprintf(stderr, "r5: %i, ", game->reg_5);
    fprintf(stderr, "r6: %i\n", game->reg_6);
}

void debugBestMove(t_CostOfMove *ledger)
{
	fprintf(stderr, "bestmove: %d - ", ledger->bestMove);
	if (ledger->bestMove == 0)
		fprintf(stderr, "UP\n");
	else if (ledger->bestMove == 1)
		fprintf(stderr, "right\n");
	else if (ledger->bestMove == 2)
		fprintf(stderr, "up\n");
	else if (ledger->bestMove == 3)
		fprintf(stderr, "left\n");
}

int *findObstacles(char *gpu)
{
    int i = 0;
    int count = 0;
    int *array;
    while(gpu[i])
    {
        if (gpu[i] == '#')
            count++;
        i++;
    }
    array = malloc(sizeof(int) * (count + 1));
    i = 0;
    int pos = 0;
    while(gpu[i])
    {
        if (gpu[i] == '#')
        {
            array[pos] = i;
            pos++;
        }
        i++;
    }
    array[pos] = -1;
    return array;
}

void dropPlayer(t_Game *game, t_CostOfMove *ledger)
{
	int cost = 0;
	int drop = 4;
	if (game->reg_0 + drop < game->reg_1 || game->reg_0 + drop < game->reg_2 || game->reg_3 > 0)
	{
		ledger->right[game->id] = cost;
		ledger->up[game->id] = cost;
		ledger->down[game->id] = cost;
		ledger->left[game->id] = cost;
	}
}

void right(t_Game *game, t_CostOfMove *ledger)
{
	int cost = 0;
	if (game->reg_0 + 3 < game->obstacles[game->obstacles_pos[game->id]])
        cost = 5;
	else if (game->reg_0 + 3 >= game->obstacles[game->obstacles_pos[game->id]])
		cost = -2;
	if (game->reg_3 > 0)
		cost = 0;
	ledger->right[game->id] = cost;
}

void down(t_Game *game, t_CostOfMove *ledger)
{
	int cost = 0;
	if (game->reg_0 + 2 < game->obstacles[game->obstacles_pos[game->id]])
        cost = 2;
	else if (game->reg_0 + 2 >= game->obstacles[game->obstacles_pos[game->id]])
		cost = -3;
	if (game->reg_3 > 0)
		cost = 0;
	ledger->down[game->id] = cost;
}

void left(t_Game *game, t_CostOfMove *ledger)
{
	int cost = 0;
	if (game->reg_0 + 1 < game->obstacles[game->obstacles_pos[game->id]])
        cost = 1;
	else if (game->reg_0 + 1 >= game->obstacles[game->obstacles_pos[game->id]])
		cost = -4;
	if (game->reg_3 > 0)
		cost = 0;
	ledger->left[game->id] = cost;
}

void up(t_Game *game, t_CostOfMove *ledger)
{
	int cost = 0;
	if (game->reg_0 + 2 <= game->obstacles[game->obstacles_pos[game->id]])
		cost = 2;
	if (game->gpu[game->reg_0 + 1] == '#')
    {
        game->obstacles_pos[game->id]++;
        cost = 4;
    }    
	else if (game->reg_0 + 3 >= game->obstacles[game->obstacles_pos[game->id]])
		cost = -4;
	if (game->reg_3 > 0)
		cost = 0;
	ledger->up[game->id] = cost;
}

void findCost(t_CostOfMove *ledger)
{
	int up = ledger->up[0] + ledger->up[1] + ledger->up[2] + ledger->up[3];
	int right = ledger->right[0] + ledger->right[1] + ledger->right[2] + ledger->right[3];
	int down = ledger->down[0] + ledger->down[1] + ledger->down[2] + ledger->down[3];
	int left = ledger->left[0] + ledger->left[1] + ledger->left[2] + ledger->left[3];
	fprintf(stderr, "\nup: %i, right: %i, down: %i, left: %i\n", up, right, up, left);
	if (up < 16 && up >= right && up >= down && up > left)
		ledger->bestMove = 0;
	else if (right > down && right > left)
		ledger->bestMove = 1;
	else if(down > left)
		ledger->bestMove = 2;
	else
		ledger->bestMove = 3;
}

void findOptimal(t_Game *game, t_CostOfMove *ledger)
{
	int pos = game->reg_0;
	if (pos == 0)
		game->obstacles_pos[game->id] = 0;
	game->obstacles = findObstacles(game->gpu);
	right(game, ledger);
	down(game, ledger);
	left(game, ledger);
	up(game, ledger);
	if (game->reg_0 == 27 &&( game->reg_2 >= 25 || game->reg_3 >= 25) && game->reg_3 == 0)
		ledger->right[game->id] = 10;
	if (game->reg_0 == 28 &&( game->reg_2 >= 25 || game->reg_3 >= 25 )&& game->reg_3 == 0)
	{
		ledger->right[game->id] = 10;
		ledger->up[game->id] = 10;
	}
	dropPlayer(game, ledger);
}

int main()
{
    int player_idx;
    scanf("%d", &player_idx);
    int nb_games;
    scanf("%d", &nb_games); fgetc(stdin);
    
	t_Game			game;
	t_CostOfMove	ledger;

	// game loop
    while (1) {
        for (int i = 0; i < 3; i++) {
            char score_info[65];
            scanf("%[^\n]", score_info); fgetc(stdin);
            // fprintf(stderr, "score_info: %s\n", score_info);
        }
        for (int i = 0; i < nb_games; i++) {
			game.id = i;
            scanf("%s%d%d%d%d%d%d%d", game.gpu, &game.reg_0, &game.reg_1, &game.reg_2, &game.reg_3, &game.reg_4, &game.reg_5, &game.reg_6); fgetc(stdin);
            findOptimal(&game, &ledger);
            // debug(&game);
			// printRound(&ledger, &game);
        }
        findCost(&ledger);
        // debugBestMove(&ledger);
        if (ledger.bestMove == 0)
            printf("UP\n");
        else if (ledger.bestMove == 1)
            printf("RIGHT\n");
        else if (ledger.bestMove == 2)
            printf("UP\n");
        else if (ledger.bestMove == 3)
            printf("LEFT\n");
    }
    return 0;
}