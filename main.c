#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

#define WIDTH 17630
#define HEIGHT 9000
// The corner of the map representing your base
int base_x;
int base_y;
int enemy_base_x;
int enemy_base_y;

typedef struct s_base	t_base;
typedef struct s_hero	t_hero;
typedef struct s_entity	t_entity;

struct s_base
{
	int		entity_count;
	int		x; //x of base
	int		y; //y of base
	int		heroes_count; //number of heroes
	int		health; //health of base
	int		e_x; //x of enemy base
	int		e_y; //y of enemy base
	int		corner; //0 if ally base is top left, 1 if ally base is bottom right
	int		time; //time since start
};

struct s_hero
{
	int	id; //id of hero
	int	x; //x of hero
	int	y; //y of hero
	int	mana; //mana of hero
	int	shield_life; //shield_life of hero
    int target;
};

struct s_entity
{
	int	id; //Unique identifier for each entity
	int	type; //0 = monster, 1 = ally hero, 2 = enemy hero
	int	x; //x of entity
	int	y; //y of entity
	int	shield_life; //count down until shield spell fades
	int	is_controlled; //1 if controlled by a spell, 0 otherwise
	int	health; //remaining health
	int	vx; //direction x of entity
	int	vy; //direction y of entity
	int	near_base; //0 if no target, 1 if targeting a base
	int	threat_for; //0 if not a threat, 1 if threat for ally base, 2 if threat for enemy base
    int targetted;
    int distance;
};

//checkt of ie in range is
// int	is_in_range(int target_x, int target_y, int mark_x, int mark_y, int radius)
// {
// 	float	distances;
// 	float	distance_sqrt;

// 	distance_sqrt = distance(target_x, target_y, mark_x, mark_y);
// 	distances = distance_sqrt - radius;
// 	if (distances <= 0)
// 		return (1);
// 	return (0);
// }

// int	count_entity_in_range(t_base *base, t_entity *entity, int mark_x, int mark_y, int radius)
// {
// 	int	in_range = 0;

// 	for (int i = 0; i < base->entity_count; i++)
// 	{
// 		if (entity[i].type == 0 || entity[i].type == 2)
// 		{
// 			if (is_in_range(entity[i].x, entity[i].y, mark_x, mark_y, radius))
// 				in_range++;
// 		}
// 	}
// 	return (in_range);
// }


int distance(int x1, int y1, int x, int y)
{  
    int afstand;
    int afstandx;
    int afstandy;

    afstand = 0;
    afstandx = x1 - x;
    afstandy = y1 - y;
    afstandx *= afstandx;
    afstandy *= afstandy;
    return (sqrt(afstandy+afstandx));
}

int distancetobase(int id_monster, int id_hero, t_base base, t_entity *entity, t_hero *hero, int mijn)
{
    int distances;

    distances = 0;
    if (id_hero == -1)
    {
        if (mijn == 1)
        {
            distances = distance(entity[id_monster].x, entity[id_monster].vy, base_x, base_y);
        }
        else
        {
            distances = distance(entity[id_monster].x, entity[id_monster].vy, base.e_x, base.e_y);
        }
    }
    if (id_monster == -1)
    {
        if (mijn == 1)
            distances = distance(hero[id_hero].x, hero[id_hero].y, base_x, base_y);
        else
            distances = distance(hero[id_hero].x, hero[id_hero].y, base.e_x, base.e_y);
    }
    return (distances);
}

int hero_to_enemy(int id_monster, t_hero *hero, t_entity *entity)
{
    int i;
    int j;
    int k;

    i = 0;
    j = 20000;
    while (i < 3 && hero[i].target == 0)
    {
        if (j > distance(hero[i].x, hero[i].y, entity[id_monster].x, entity[id_monster].y))
            j = i;
    }
    return (j);
}

void spellbasenw()
{
    printf("SPELL WIND 9000 9000\n");
}

void spellse()
{
    printf("SPELL WIND 14300 5400\n");
}


// void offensive(t_base base, t_entity *entity, t_hero *hero, int bas, int time)
// {
//     int flag;
//     int hero_i;
//     int spell;
//     int id_monster;

//     id_monster = -1;
//     flag = 0;
//     hero_i = 0;
//     spell = 0;

//     id_monster = closest(base, entity, hero, base.x, base.time, 1);
//     fprintf(stderr, "id offensive monster = %d", id_monster);
//     if (distance(hero[2].x, hero[2].y, entity[id_monster].vx, entity[id_monster].vy) < 1280 && entity[id_monster].type == 0 && id_monster > 0)
//         printf("wind %d %d\n", WIDTH, HEIGHT);//entity[id_monster].id);
//     else if (distance(hero[2].x, hero[2].y, base.e_x, base.e_y) > 5000)
//         printf("MOVE 14600 6000\n");
//     // else if (distance(hero[2].x, hero[2].y, entity[id_monster].vx, entity[id_monster].vy) > 2200 && id_monster > 0)
//     //     printf("MOVE %d %d\n", entity[id_monster].vx, entity[id_monster].vy);
//     else if (distance(entity[id_monster].vx, entity[id_monster].vy, base.e_x, base.e_y) < 7200)
//         printf("MOVE %d, %d", entity[id_monster].vx, entity[id_monster].vy);
//     else
//         printf("MOVE 14600 6000\n");
//     //bas 2 closest
// }

int closestenemy(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int distance;
    int p;

    i = 0;
    p = 0;
    distance = 100000;
    while (i < base.entity_count)
    {
        if (distance > distancetobase(i, -1, base, entity, hero, 0))
        {
            distance = distancetobase(i, -1, base, entity, hero, 0);
            p = i;
        }
        i++;
    }
    return (p);
}

void attack(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int bal;
    int p;

    p = -1;
    i = 0;
    bal = 0;
    //fprintf(stderr, "i = %d count = %d bal = %d", i, base.entity_count, bal);
    while (i < base.entity_count && bal == 0)
    {
        p = closestenemy(base, entity, hero);
        fprintf(stderr, "p = %d", p);
        if (distancetobase(i, -1, base, entity, hero, 0) < 7200 && entity[i].type == 0)
        {
            if (distancetobase(-1 , 2, base, entity, hero, 0) < 1280)
            {
                bal = 1;
            }
            else if ((entity[i].threat_for == 2 || entity[i].near_base == 1) && distancetobase(-1 , 2, base, entity, hero, 0) < 2200)
            {
                bal = 2;
            }
        }
        else if (distancetobase(i, -1, base, entity, hero, 0) < 7200 && entity[i].type == 0)
        {
            if (distance(entity[i].x + entity[i].vx, entity[i].y + entity[i].vy, base.e_x, base.e_y))
            {
                bal = 4;
            }
        }
        else if (distance(hero[2].x, hero[2].y, entity[p].x, entity[p].y) < 4000 && p >= 0 && entity[p].type == 0)
        {
            bal = 5;
        }
        else 
        {
            bal = 0;   
        }
        i++;
    }
    fprintf(stderr, "bal = %d", bal);
    // if (bal == 1)
    //     printf("WIND %d %d\n", WIDTH, HEIGHT);
    // else if (bal == 2)
    //     printf("shield %d\n", entity[i].id);
    // else if ( bal == 0)
    //     printf("MOVE %d %d\n", 15500 , 4000);
    // else if (bal == 4)
    //     printf("MOVE %d %d\n", entity[i].x, entity[i].y);
    // else if (bal == 5)
    //      printf("MOVE %d %d\n", entity[i].x, entity[i].y);
}

void    baseposition(t_base base, t_entity *entity, t_hero *hero, int bas, int time)
{
    int flag;
    int hero_i;
    int spell;

    flag = 0;
    hero_i = 0;
    spell = 0;
    while(hero_i < 3)
    {    
        if (time > 150)
            spell = 0;
        if (flag == 0 && bas == 0 && spell < 2)
            if (time > 40)
            {
                printf("SPELL WIND 2000 2000\n");
                spell = 1;
            }
            else
                printf("MOVE %d %d\n", 650, 650);
        else if (flag == 1 & bas == 0 && spell < 2)
        {
            if (time > 40)
            {
                printf("SPELL WIND 2000 2000\n");
                 spell = 1;
            }
            else
                printf("MOVE %d %d\n", 650, 650);
        }
        else if (flag == 0 && bas != 0 && spell < 2)
            if (time > 50)
            {
                spell = 1;
                printf("SPELL WIND 14211 5600\n");
            }
            else
                printf("MOVE %d %d\n", WIDTH - 650, HEIGHT - 650);
        else if (flag == 1 & bas != 0 && spell < 2)
        {
            if (time > 50)
            {
                spell = 1;
                printf("SPELL WIND 14211 5600\n");
            }
            else
                printf("MOVE %d %d\n", WIDTH - 650, HEIGHT - 650);
        }
        hero_i++;
    }
        //cutoff target
}

// void target(t_base base, t_entity *entity, t_hero *hero, int bas, int time)
// {
//     int i;
//     int j;
//     int flag;
//     int hero_i;
//     int k;
//     int spell;
//     flag = 0;
//     int id_monster;
//     int closest_hero;


//     hero_i = 0;
//     id_monster = 0;

//     k = 0;
//     i = 0;
//     j = 0;
//     hero_i = 0;
//     spell = 0;
//     while(life > 2 en monsters niet te dichtbij)
//     {    
//         doe om de 12 ish rondes spellshield
//         2 man verdedigen rond de 5000 + wind
//     }
//     while (life < 2 of veel monsters komende)
//     {
//        2 defend dichterbij en windwall
//     }
//     while 
//     aanvaller ziet verdedigers geen spellshield, schiet ze tegenovergestelde kant, weg van doel, van dichtste monster
    
//     als dat niet kan spellshield de monster en zorg dat ie op afstand blijft

// }

int main()
{
    t_base		base;
	t_entity	*entity;
	t_hero		*hero;

    int p;
    int i;
    int b;

    scanf("%d%d%d", &base.x, &base.y, &base.heroes_count);
	if (base.x == 0 && base.y == 0)
	{
		base.e_x = 17630;
		base.e_y = 9000;
	}
	else
	{
		base.e_x = 0;
		base.e_y = 0;
	}
    hero = malloc(sizeof(t_hero) * base.heroes_count);
    base.time = 0;
	while (1)
	{
		for (int i = 0; i < 2; i++)
		{
			hero[i].id = i;
			scanf("%d%d", &base.health, &hero[i].mana);
		}
		scanf("%d", &base.entity_count);
		entity = malloc(sizeof(t_entity) * base.entity_count);
		b = 0;
		for (int i = 0; i < base.entity_count; i++)
		{
			scanf("%d%d", &entity[i].id, &entity[i].type);
			scanf("%d%d", &entity[i].x, &entity[i].y);
			scanf("%d%d%d", &entity[i].shield_life, &entity[i].is_controlled, &entity[i].health);
			scanf("%d%d", &entity[i].vx, &entity[i].vy);
			scanf("%d%d", &entity[i].near_base, &entity[i].threat_for);
			if (entity[i].type == 1 && b < base.heroes_count)
			{
				hero[b].id = entity[i].id;
				hero[b].x = entity[i].x;
				hero[b].y = entity[i].y;
				hero[b].shield_life = entity[i].shield_life;
                hero[b].target = 0;
				b++;
			}
		}
        baseposition(base, entity, hero, base.x, base.time);
        attack(base, entity, hero);
		free (entity);
		base.time++;
	}
	free (hero);
	return (0);
}
