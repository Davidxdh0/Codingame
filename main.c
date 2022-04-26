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
    int threat;
    int targetted;
    int distance;
};
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

int distancetobase(int counter, t_base base, t_entity *entity, t_hero *hero, int mijn)
{
    int distances;

    distances = 0;
    if (mijn == 1)
    {
        distances = distance(entity[counter].x, entity[counter].y, base_x, base_y);
    }
    else
    {
        distances = distance(entity[counter].x, entity[counter].y, base.e_x, base.e_y);
    }
    return (distances);
}

int distanceto(int id, int id_2, t_base base, t_entity *entity, t_hero *hero)
{
    int distances;

    distances = 0;
    distances = distance(entity[id_2].x, entity[id_2].y, entity[id].x, entity[id].y);
    return (distances);
}

int in_range(int id, int id_2, t_base base, t_entity *entity, t_hero *hero, int range)
{
    if (distanceto(id, id_2, base, entity, hero) <= range)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int count_in_range(int id, t_base base, t_entity *entity, t_hero *hero, int range)
{
    int i;
    int count;

    count = 0;
    i = 0;
    while (i < base.entity_count)
    {
        if (entity[i].type == 0 || entity[i].type == 2)
        {
            if (in_range(id, i, base, entity, hero, range))
                count++;
        }
        i++;
    }
    return (count);
}


int hero_closest_counter(int id_hero, t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int j;
    int k;

    i = 0;
    j = 20000;
    k = 0;
    while (i < base.entity_count )
    {
        if (j > distance(entity[id_hero].x, entity[id_hero].y, entity[i].x, entity[i].y) && entity[i].type == 0)
        {
            j = distance(entity[id_hero].x, entity[id_hero].y, entity[i].x, entity[i].y);
            k = i;
        }
        i++;
    }
    return (k);
}

int closest_hero(int counter, t_base base, t_hero *hero, t_entity *entity)
{
    int i;
    int j;
    int k;
    int id_hero;

    i = 0;
    j = 20000;
    id_hero = 0;
    while (id_hero < 3 && hero[i].target != 1 && base.entity_count > 2)
    {
        if (j > distance(entity[id_hero].x, entity[id_hero].y, entity[counter].x, entity[counter].y))
        {
            j = distance(entity[id_hero].x, entity[id_hero].y, entity[counter].x, entity[counter].y);
            k = id_hero;
        }
        id_hero++;
    }
    return (k);
}

int hero_distance_enemy(int counter, int id_hero, t_base base, t_hero *hero, t_entity *entity)
{
    int i;
    int j;
    int k;

    i = 0;
    j = 20000;
    while (i < base.entity_count && hero[i].target != 1)
    {
        if (j > distance(hero[id_hero].x, hero[id_hero].y, entity[counter].x, entity[counter].y))
            j = distance(hero[id_hero].x, hero[id_hero].y, entity[counter].x, entity[counter].y);
        i++;
    }
    return (j);
}

int closestenemy(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int dist;
    int p;

    i = 2;
    p = 0;
    dist = 10000;
    while (i < base.entity_count)
    {   
        if (dist > distancetobase(i, base, entity, hero, 1) && entity[i].type == 0 && entity[i].targetted == 0)
        {
            dist = distancetobase(i, base, entity, hero, 1);
            p = i;
        }
        i++;
    }
    return (p);
}

int closestenemytohero(t_base base, t_entity *entity, t_hero *hero, int hero_id)
{
    int i;
    int dist;
    int p;

    i = 0;
    p = 0;
    dist = 2000;
    while (i < base.entity_count)
    {
        if (dist > distance(entity[hero_id].x, entity[hero_id].y, entity[i].x, entity[i].y) && entity[i].type == 0 && entity[i].targetted == 0)
        {
            dist = distance(entity[hero_id].x, entity[hero_id].y, entity[i].x ,entity[i].y);
            p = i;
        }
        i++;
    }
    return (p);
}

void threats(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int dist;
    int p;

    i = 0;
    p = 0;
    dist = 10000;
    while (i < base.entity_count)
    {
        if (6000 > distancetobase(i, base, entity, hero, 1) && entity[i].type == 0)
        {
            p = entity[i].id;
            entity[i].threat = 1;
        }
        //hp erbij?
        if (3000 > distancetobase(i, base, entity, hero, 1) && entity[i].type == 0)
        {
            p = entity[i].id;
            entity[i].threat = 2;
        }
        i++;
    }
}

// void    baseposition(t_base base, t_entity *entity, t_hero *hero, int bas, int time)
// {
//     int flag;
//     int hero_i;
//     int spell;

//     flag = 0;
//     hero_i = 0;
//     spell = 0;
//     while(hero_i < 2)
//     {    
//         if (time > 150)
//             spell = 0;
//         if (flag == 0 && bas == 0 && spell < 2)
//             if (time > 40)
//             {
//                 printf("SPELL WIND 2000 2000\n");
//                 spell = 1;
//             }
//             else
//                 printf("MOVE %d %d\n", 650, 650);
//         else if (flag == 1 & bas == 0 && spell < 2)
//         {
//             if (time > 40)
//             {
//                 printf("SPELL WIND 2000 2000\n");
//                  spell = 1;
//             }
//             else
//                 printf("MOVE %d %d\n", 650, 650);
//         }
//         else if (flag == 0 && bas != 0 && spell < 2)
//             if (time > 50)
//             {
//                 spell = 1;
//                 printf("SPELL WIND 14211 5600\n");
//             }
//             else
//                 printf("MOVE %d %d\n", WIDTH - 650, HEIGHT - 650);
//         else if (flag == 1 & bas != 0 && spell < 2)
//         {
//             if (time > 50)
//             {
//                 spell = 1;
//                 printf("SPELL WIND 14211 5600\n");
//             }
//             else
//                 printf("MOVE %d %d\n", WIDTH - 650, HEIGHT - 650);
//         }
//         hero_i++;
//     }
//         //cutoff target
// }

void early(t_base base, t_entity *entity, t_hero *hero, int bas, int time)
{
    int i;
    int j;
    int flag;
    int hero_i;
    int k;
    int spell;
    int ce;
    flag = 0;
    int id_monster;
    int closest_hero;

    ce = 0;
    j = 0;
    fprintf(stderr, "time = %d\n", time);
    i = base.entity_count;
    fprintf(stderr, "i = %d\n", i);
    if (time < 7 && i < 4)
    {
        fprintf(stderr, "onder 7\n");
        printf("MOVE 3500 8000\n");
        printf("MOVE 6600 8800\n");
        printf("MOVE 7400 800\n");
    }
    
    else if (time > 6)
    {
        fprintf(stderr, "na 6\n");
        j = closestenemytohero(base, entity, hero, 0);
        ce = closestenemy(base, entity, hero);
        fprintf(stderr, "ce = %d\n",ce);
        fprintf(stderr, "j0 = %d\n", j);
        if (ce == j && ce != 0)
        {
            //entity[j].targetted = 1;
            printf("MOVE %d %d\n", entity[j].x, entity[j].y);
        }
        else if (j > 0)
        {
            //entity[j].targetted = 1;
            printf("MOVE %d %d\n", entity[j].x, entity[j].y);
        }
        else
        {
            printf("MOVE 3500 8000\n");
        }
        j = closestenemytohero(base, entity, hero, 1);
        fprintf(stderr, "j 1 = %d\n", j);
        if (ce == j && ce != 0)
        {
            //entity[j].targetted = 1;
            printf("MOVE %d %d\n", entity[j].x, entity[j].y);
        }
        else if (j > 0 )
        {
            //entity[j].targetted = 1;
            printf("MOVE %d %d\n", entity[j].x, entity[j].y);
        }
        else
        {
            printf("MOVE 6600 8800\n");
        }
        j = closestenemytohero(base, entity, hero, 2);
        fprintf(stderr, "j 2 = %d\n", j);
        if (ce == j && ce != 0)
        {
            //entity[j].targetted = 1;
            printf("MOVE %d %d\n", entity[j].x, entity[j].y);
        }
        else if (j > 0 )
        {
            //entity[j].targetted = 1;
            printf("MOVE %d %d\n", entity[j].x, entity[j].y);
        }
        else
        {
            printf("MOVE 7400 800");
        }
    }
}

void    defense(t_base base, t_entity *entity, t_hero *hero, int time)
{
    int i;
    int j;
    int flag;
    int hero_i;
    int k;
    int spell;
    flag = 0;
    int id_monster;
    int closest_hero;


    if (time < 50)
    {
        early(base, entity, hero, base.x, base.time);
    }
    else
    {
        printf("MOVE 650 650\n");
        printf("MOVE 650 650\n");
        printf("MOVE 650 650\n");
    }
}
        
int main()
{
    t_base		base;
	t_entity	*entity;
	t_hero		*hero;

    int p;
    int i;
    int b;
    int counter;

    counter = 0;

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
        counter = 0;
        while (counter < base.entity_count)
        {
            if (counter > 2)
            {
                //fprintf(stderr, "distanceto() = %d id=%d\n", distanceto(counter, 1, base, entity, hero), entity[counter].id);
                // fprintf(stderr, "closesthero = %d\n", closest_hero(counter, 0, base, hero, entity));
                //fprintf(stderr, "coun=%d countent= %d x=%d y=%d\n", counter, base.entity_count, entity[counter].x, entity[counter].y);
                //printf(stderr, "dist=%d id monster = %d\n", distancetobase(counter, base, entity, hero, 1), entity[counter].id);
                //fprintf(stderr, "range=%d", in_range(counter, 0, base, entity, hero, 3000));
                //count_in_range(int id, t_base base, t_entity *entity, t_hero *hero, int range)
                //fprintf(stderr, "range=%d", count_in_range(0, base, entity, hero, 3000));
                //hero_closest_idenemy(int id_hero, t_base base, t_hero *hero, t_entity *entity)
                //fprintf(stderr, "hci=%d", hero_closest_counter(0, base, entity, hero));
                //closest_hero(t_base base, t_hero *hero, t_entity *entity)
                //fprintf(stderr, "closesthero=%d", closest_hero(counter, base, hero, entity));
                //hero_distance_enemy(int counter, int id_hero, t_base base, t_hero *hero, t_entity *entity)
                //fprintf(stderr, "herodistanceenemy=%d\n", hero_distance_enemy(counter, 2, base, hero, entity));
                // int closestenemy(t_base base, t_entity *entity, t_hero *hero)
                //fprintf(stderr, "closestenemy=%d\n", closestenemy(base, entity, hero));
            }
            counter++;
        }
        
        threats(base, entity, hero);
        defense(base, entity, hero, base.time);
        //baseposition(base, entity, hero, base.x, base.time);
        //attack(base, entity, hero);

        //printf("MOVE %d %d\n", 2000, 2000);
		free (entity);
		base.time++;
	}
	free (hero);
	return (0);
}
