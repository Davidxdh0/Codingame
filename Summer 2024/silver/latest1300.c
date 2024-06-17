#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct s_CostOfMove {
    int right[4];
    int down[4];
    int left[4];
    int up[4];
	int compete[4];
    int bestMove;
} t_CostOfMove;

typedef struct s_Game {
    int     id;
	char    gpu[65];
	int     count;
	int     reg_0;
	int     reg_1;
	int     reg_2;
	int     reg_3;
	int     reg_4;
	int     reg_5;
	int     reg_6;
} t_Game;

int distObst(t_Game *game, t_CostOfMove *ledger);

void printRound(t_CostOfMove *ledger, t_Game *game)
{
	fprintf(stderr, "[%d]:right %d, ", game->id, ledger->right[game->id]);
	fprintf(stderr, "down %d, ", ledger->down[game->id]);
	fprintf(stderr, "left %d, ", ledger->left[game->id]);
	fprintf(stderr, "up %d\n", ledger->up[game->id]);
}

void debug(t_Game *game, t_CostOfMove *ledger)
{
	int i = 1;
    fprintf(stderr, "gpu[%i]: %s\n", game->id, game->gpu);
    fprintf(stderr, "distobst: %i\n", distObst(game, ledger));
    fprintf(stderr, "r0: %i, ", game->reg_0);
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

void debugValues(t_CostOfMove *ledger, t_Game *game)
{
    fprintf(stderr, "[%d]: up: %d, right: %d, down: %d, left: %d\n", game->id, ledger->up[game->id], ledger->right[game->id], ledger->down[game->id], ledger->left[game->id]);
}

void dropPlayer(t_Game *game, t_CostOfMove *ledger)
{
	int cost = 0;
	int drop = 5;

	int loser = 0;
	if (game->reg_0 + drop < game->reg_1 || game->reg_0 + drop < game->reg_2 || game->reg_3 > 0)
	{
		if (game->reg_4 < 2 && game->reg_5 < 2)
		{
			ledger->right[game->id] = cost;
			ledger->up[game->id] = cost;
			ledger->down[game->id] = cost;
			ledger->left[game->id] = cost;
		}
	}
}

int distObst(t_Game *game, t_CostOfMove *ledger)
{
    int result = 0;
    int i = game->reg_0;
	if (game->gpu[i] == '#'){
		i++;
		result++;
	}
    while(i < 30)
    {
        if (game->gpu[i] == '#')
            return result;
        i++;
        result++;
    }
    if (i == 30)
        return 10;
    return result;
}

void right(t_Game *game, t_CostOfMove *ledger)
{
	int cost = 0;
	if (distObst(game, ledger) > 3)
        cost = 10;
	else if (distObst(game, ledger) <= 3)
		cost = -3;
	if (game->reg_3 > 0)
		cost = 0;
	ledger->right[game->id] = cost;
}

void down(t_Game *game, t_CostOfMove *ledger)
{
	int cost = 0;
	if (distObst(game, ledger) > 2)
        cost = 6;
	else if (distObst(game, ledger) <= 2)
		cost = -3;
	if (game->reg_3 > 0)
		cost = 0;
	ledger->down[game->id] = cost;
}

void left(t_Game *game, t_CostOfMove *ledger)
{
	int cost = 0;
	if (distObst(game, ledger) > 1)
        cost = 3;
	else if (distObst(game, ledger) <= 1)
		cost = -4;
	if (game->reg_3 > 0)
		cost = 0;
	ledger->left[game->id] = cost;
}

void up(t_Game *game, t_CostOfMove *ledger)
{
	int cost = 0;
	if (distObst(game, ledger) > 2)
		cost = 4;
	if (distObst(game, ledger) == 1)
        cost = 8; 
	else if (distObst(game, ledger) == 2)
		cost = -3;
	if (game->reg_3 > 0)
		cost = 0;
	ledger->up[game->id] = cost;
}

void dropNonCompetete(t_CostOfMove *ledger, t_Game *game)
{
	int cost = 0;
	if (ledger->compete[game->id] == 0)
	{
		ledger->right[game->id] = cost;
		ledger->up[game->id] = cost;
		ledger->down[game->id] = cost;
		ledger->left[game->id] = cost;
	}
}

void gpuSwimToBestmove(t_Game *game, t_CostOfMove *ledger, char c)
{
	if (c == 'U')
		ledger->bestMove = 0;
	else if (c == 'R')
		ledger->bestMove = 1;
	else if (c == 'D')
		ledger->bestMove = 2;
	else if (c == 'L')
		ledger->bestMove = 3;
}

void removePlayer(t_CostOfMove *ledger, int i)
{
    ledger->right[i] = 0;
    ledger->up[i] = 0;
    ledger->down[i] = 0;
    ledger->left[i] = 0;
}

void removeNonCompete(t_Game *game, t_CostOfMove *ledger)
{
    ledger->right[0] *= 10;
    ledger->up[0] *= 10;
    ledger->down[0] *= 10;
    ledger->left[0] *= 10;
    if (ledger->compete[0] == 0)
        removePlayer(ledger, 0);
    if (ledger->compete[1] == 0)
        removePlayer(ledger, 1);
    if (ledger->compete[2] == 0)
        removePlayer(ledger, 2);
    if (ledger->compete[3] == 0)
        removePlayer(ledger, 3);
}

void findCost(t_CostOfMove *ledger, t_Game *game)
{
	removeNonCompete(game, ledger);
	int up = ledger->up[0] + ledger->up[1] + ledger->up[2] + ledger->up[3];
	int right = ledger->right[0] + ledger->right[1] + ledger->right[2] + ledger->right[3];
	int down = ledger->down[0] + ledger->down[1] + ledger->down[2] + ledger->down[3];
	int left = ledger->left[0] + ledger->left[1] + ledger->left[2] + ledger->left[3];

	fprintf(stderr, "\nup: %i, right: %i, down: %i, left: %i\n", up, right, down, left);
	fprintf(stderr, "compete: %i, compete: %i, compete: %i, compete: %i\n", ledger->compete[0], ledger->compete[1], ledger->compete[2], ledger->compete[3]);
	int len = strlen(game->gpu);
	while (game->gpu[0] != 'G' && ledger->compete[3] == 1 && (game->count + len) < 100 )
	{
		gpuSwimToBestmove(game, ledger, game->gpu[0]);
		return;
	}
	if (up < 1622 && up >= right && up >= down && up > left)
		ledger->bestMove = 0;
	else if (right > down && right > left)
		ledger->bestMove = 1;
	else if(down > left)
		ledger->bestMove = 2;
	else
		ledger->bestMove = 3;
	fprintf(stderr, "bestmove: %i\n", ledger->bestMove);
	
}

void findOptimal(t_Game *game, t_CostOfMove *ledger)
{
	int pos = game->reg_0;
	right(game, ledger);
	down(game, ledger);
	left(game, ledger);
	up(game, ledger);
	if (game->reg_0 == 27 &&( game->reg_2 >= 25 || game->reg_3 >= 25) && game->reg_3 == 0)
		ledger->right[game->id] = 10;
	if (game->reg_0 == 28 &&( game->reg_2 >= 25 || game->reg_3 >= 25 )&& game->reg_3 == 0)
	{
		ledger->right[game->id] = 10;
		ledger->up[game->id] = 15;
	}
	dropPlayer(game, ledger);
}

void dropHurdle(t_Game *game, t_CostOfMove *ledger)
{
	int pos = 0;
	pos = game->reg_0 - game->reg_1;
	pos += game->reg_0 - game->reg_2;
	if (pos < -11)
		ledger->compete[game->id] = 0;
	else if (pos > 11)
		ledger->compete[game->id] = 0;
	else 
        ledger->compete[game->id] = 1;
	if (game->reg_3 > 0)
        ledger->compete[game->id] = 0;
	if(game->reg_0 < 20 && game->count > 95 )
        ledger->compete[game->id] = 0;
}

int valurArcher(t_Game *game)
{
	int i = 0;
	int count = 0;
	while (game->gpu[i])
	{
		count += game->gpu[i] - '0';
		i++;
	}
	return count;
}

void dropArcher(t_Game *game, t_CostOfMove *ledger)
{
	int round = strlen(game->gpu);
	int totalValue = valurArcher(game);
	int yNull = 0;
	int xNull = 0;
	int Null = 0;
	if (round < 5 || game->count >= 95)
	{
		if (game->reg_0 < 0)
			xNull = -1 * game->reg_0;
		if (game->reg_0 >= 0)
			xNull = game->reg_0;
		if (game->reg_1 < 0)
			yNull = -1 * game->reg_1;
		if (game->reg_1 >= 0)
			yNull = game->reg_1;
		Null = yNull + xNull;
		// if (totalValue + 8 < Null)
		// 	ledger->compete[game->id] = 0;
		// else
		// 	ledger->compete[game->id] = 1;
	}
	// if (game->reg_0 > -3 && game->reg_0 < 3 && game->reg_1 > -3 && game->reg_1 < 4)
	// 	ledger->compete[game->id] = 0;
	if (round + game->count > 100)
		ledger->compete[game->id] = 0;
}

void dropSkating(t_Game *game, t_CostOfMove *ledger)
{
	int pos = 0;
	pos = game->reg_0 - game->reg_1;
	pos += game->reg_0 - game->reg_2;
	if (pos < -10)
		ledger->compete[game->id] = 0;
	if (pos < -7 && game->reg_0 <= -1)
		ledger->compete[game->id] = 0;
	if (pos > 10)
		ledger->compete[game->id] = 0;
	if (pos > 7 && game->reg_0 > -1)
		ledger->compete[game->id] = 0;
	if (game->gpu[0] == 'G')
        ledger->compete[game->id] = 0;
    if (game->reg_3 < 0)
        ledger->compete[game->id] = 0;
	if (game->reg_6 + game->count > 100)
        ledger->compete[game->id] = 0;
}

int totalValue(t_Game *game, t_CostOfMove *ledger, int i)
{
	int player = 0;
	int combo = 0;
	int myscore = 0;
	int rounds = 0;
	if (i == 0)
	{
		player = game->reg_0;
		combo = game->reg_3;
	}
	else if (i == 1){
		player = game->reg_1;
		combo = game->reg_4;
	}
	else
	{
		player = game->reg_2;
		combo = game->reg_5;
	}
	rounds = strlen(game->gpu);
	myscore = player;
	while(rounds > 0)
	{
		myscore += combo;
		combo++;
		rounds--;
	}
	return myscore;
}

void dropSwim(t_Game *game, t_CostOfMove *ledger)
{
	int round = strlen(game->gpu);
	int myValue = totalValue(game, ledger, 0);
	int opp1Value = totalValue(game, ledger, 1) + game->reg_4;
	int opp2Value = totalValue(game, ledger, 2) + game->reg_5;
	if (myValue >= opp1Value || myValue >= opp2Value)
		ledger->compete[game->id] = 0;
	if (game->gpu[0] == 'G')
        ledger->compete[game->id] = 0;
    fprintf(stderr, "swim stat: my %i - %i - %i \n", myValue, opp1Value, opp2Value);
}

void dropWonLostGame(t_Game *game, t_CostOfMove *ledger)
{
	if (game->id == 0)
		dropHurdle(game, ledger);
	if (game->id == 1)
		dropArcher(game, ledger);
	if (game->id == 2)
		dropSkating(game, ledger);
	if (game->id == 3)
		dropSwim(game, ledger);
}

void bestHurdle(t_Game *game, t_CostOfMove *ledger)
{
	findOptimal(game, ledger);
}

void bestArcher(t_Game *game, t_CostOfMove *ledger)
{
	int x = 0;
	int y = 0;
	int round = strlen(game->gpu);
	int totalValue = valurArcher(game);
	if (game->reg_0 <= -5)
		ledger->right[game->id] = 10;
	if (game->reg_0 >= 5)
		ledger->left[game->id] = 10;
	if (game->reg_1 <= -5)
		ledger->down[game->id] = 10;
	if (game->reg_1 >= 5)
		ledger->up[game->id] = 10;
    if (game->reg_0 <= -8)
		ledger->right[game->id] = 15;
	if (game->reg_0 >= 8)
		ledger->left[game->id] = 15;
	if (game->reg_1 <= -8)
		ledger->down[game->id] = 15;
	if (game->reg_1 >= 8)
		ledger->up[game->id] = 15;
    if (round < 2)
    {
        ledger->right[game->id] *= 2;
        ledger->up[game->id] *= 2;
        ledger->down[game->id] *= 2;
        ledger->left[game->id] *= 2;
    }
}

void gpuToLedger(t_Game *game, t_CostOfMove *ledger, char c, int i)
{
	if (c == 'U')
		ledger->up[game->id] = i;
	else if (c == 'D')
		ledger->down[game->id] = i;
	else if (c == 'L')
		ledger->left[game->id] = i;
	else if (c == 'R')
		ledger->right[game->id] = i;
}

int beatable(int toBeat, int me)
{
    int temp = 0;
    if ((toBeat - me) < 3 && (toBeat - me) > -2 )
        return 1;
}

int distanceAB(int a, int b)
{
    int result = 0;
    result = a - b;
    return result;
}

void bestSkating(t_Game *game, t_CostOfMove *ledger)
{
	int move = 0;
	if (game->reg_3 > 3)
		gpuToLedger(game, ledger, game->gpu[0], 15);
	if (game->reg_3 > 0 && game->reg_3 <= 3)
	{
		gpuToLedger(game, ledger, game->gpu[1], 10);
		gpuToLedger(game, ledger, game->gpu[2], 20);
	}
	if (game->reg_3 >= 0 && game->reg_3 < 2)
		gpuToLedger(game, ledger, game->gpu[3], 15);
    if (game->reg_6 <= 3 && game->reg_3 < 3 && (distanceAB(game->reg_1, game->reg_0) < 3 || distanceAB(game->reg_2, game->reg_0) < 3))
        gpuToLedger(game, ledger, game->gpu[3], 40);
    if (game->reg_6 <= 3 && game->reg_3 < 4 && (distanceAB(game->reg_1, game->reg_0) < 2 || distanceAB(game->reg_2, game->reg_0) < 2))
        gpuToLedger(game, ledger, game->gpu[2], 20);
    if (game->reg_6 <= 3 && game->reg_3 < 5 && (distanceAB(game->reg_1, game->reg_0) < 1 || distanceAB(game->reg_2, game->reg_0) < 1))
        gpuToLedger(game, ledger, game->gpu[2], 15);
     if (game->reg_6 <= 3 && game->reg_3 < 5 && (distanceAB(game->reg_1, game->reg_0) < 0 || distanceAB(game->reg_2, game->reg_0) < 0))
        gpuToLedger(game, ledger, game->gpu[1], 15);
	if (game->reg_6 <= 2 && game->reg_3 < 3 && (beatable(game->reg_1, game->reg_0) || beatable(game->reg_2, game->reg_0)))
		gpuToLedger(game, ledger, game->gpu[3], 60);
	if (game->reg_6 <= 1 && (beatable(game->reg_1, game->reg_0) || beatable(game->reg_2, game->reg_0)))
		gpuToLedger(game, ledger, game->gpu[3], 60);
    if (game->reg_6 <= 1 && (distanceAB(game->reg_1, game->reg_0) < 3 || distanceAB(game->reg_2, game->reg_0) < 3))
        gpuToLedger(game, ledger, game->gpu[3], 60);
    if (game->reg_6 <= 1 && (distanceAB(game->reg_1, game->reg_0) < 2 || distanceAB(game->reg_2, game->reg_0) < 2))
        gpuToLedger(game, ledger, game->gpu[2], 30);
    if (game->reg_6 <= 1 && (distanceAB(game->reg_1, game->reg_0) < 1 || distanceAB(game->reg_2, game->reg_0) < 1))
        gpuToLedger(game, ledger, game->gpu[2], 15);
}

void gpuSwimToLedger(t_Game *game, t_CostOfMove *ledger, char c)
{
	if (c == 'U')
		ledger->up[game->id] = 2 * game->reg_3;
	else if (c == 'D')
		ledger->down[game->id] = 2 * game->reg_3;
	else if (c == 'L')
		ledger->left[game->id] = 2 * game->reg_3;
	else if (c == 'R')
		ledger->right[game->id] = 2 * game->reg_3;
}

void bestSwim(t_Game *game, t_CostOfMove *ledger)
{
	gpuSwimToLedger(game, ledger, game->gpu[0]);
}

void BestMoveGame(t_Game *game, t_CostOfMove *ledger)
{
	if (game->id == 0)
		bestHurdle(game, ledger);
	if (game->id == 1)
		bestArcher(game, ledger);
	if (game->id == 2)
		bestSkating(game, ledger);
	if (game->id == 3)
		bestSwim(game, ledger);
}

int main()
{
    int player_idx;
    scanf("%d", &player_idx);
    int nb_games;
    scanf("%d", &nb_games); fgetc(stdin);
    
	t_Game			game;
	t_CostOfMove	ledger;
	
	memset(&ledger, 0, sizeof(t_CostOfMove));
	game.count = 0;
    while (1) {
		int p = 0;
		initStr(&ledger);
        for (int i = 0; i < 3; i++) {
            char score_info[65];
            scanf("%[^\n]", score_info); fgetc(stdin);
            // fprintf(stderr, "score_info: %s\n", score_info);
        }
        for (int i = 0; i < nb_games; i++) {
			game.id = i;
            scanf("%s%d%d%d%d%d%d%d", game.gpu, &game.reg_0, &game.reg_1, &game.reg_2, &game.reg_3, &game.reg_4, &game.reg_5, &game.reg_6); fgetc(stdin);
debug(&game, &ledger);
            dropWonLostGame(&game, &ledger);
            BestMoveGame(&game, &ledger);
			dropNonCompetete(&ledger, &game);
debugValues(&ledger, &game);
		}
        findCost(&ledger, &game);
        if (ledger.bestMove == 0)
            printf("UP\n");
        else if (ledger.bestMove == 1)
            printf("RIGHT\n");
        else if (ledger.bestMove == 2)
            printf("DOWN\n");
        else if (ledger.bestMove == 3)
            printf("LEFT\n");
		game.count++;
    }
    return 0;
}