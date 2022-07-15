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
    int controlled;
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
    int potentie;
    int controlled;
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
    if (mijn == 1 && base.x == 0)
        distances = distance(entity[counter].x, entity[counter].y, base_x, base_y);
    else if (mijn == 1 && base.x != 0)
        distances = distance(WIDTH - entity[counter].x, HEIGHT - entity[counter].y, 0, 0);
    else if (mijn == 0 && base.x != 0)
        distances = distance(entity[counter].x, entity[counter].y, base.e_x, base.e_y);
    else
        distances = distance(WIDTH - entity[counter].x, HEIGHT - entity[counter].y, 0, 0);
    return (distances);
}

int distanceto(int id, int id_2, t_base base, t_entity *entity, t_hero *hero)
{
    int distances;

    distances = 0;
    distances = distance(entity[id_2].x, entity[id_2].y, entity[id].x, entity[id].y);
    return (distances);
}

int in_range(int her, int id, int id2, t_base base, t_entity *entity, t_hero *hero, int range)
{
    if (her == 0)
    {
        if (distanceto(id, id2, base, entity, hero) <= range)
            return (1);
        else
            return (0);
    }
    else
    {
        if (distance(hero[id].x, hero[id].y, entity[id2].x, entity[id2].y) <= range)
            return (1);
        else
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
        if (entity[i].type == 0 )
        {
            if (in_range(1, id, i, base, entity, hero, range))
                count++;
        }
        if (entity[i].type == 2)
        {
            if (in_range(2, id, i, base, entity, hero, range))
                count++;
        }
        i++;
    }
    return (count);
}

// closest monster to hero in 3000
int hero_closest_counter(int id_hero, t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int j;
    int k;

    i = 0;
    j = 3000;
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

    k = -1;
    i = 0;
    j = 20000;
    id_hero = 0;
    while (id_hero < 2 && base.entity_count > 2 && entity[i].targetted != 1)
    {
        if (j > distance(hero[id_hero].x, hero[id_hero].y, entity[counter].x, entity[counter].y))
        {
            j = distance(hero[id_hero].x, hero[id_hero].y, entity[counter].x, entity[counter].y);
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

int closestenemyid(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int dist;
    int p;

    i = 0;
    p = 0;
    dist = 20000;
    while (i < base.entity_count)
    {   
        if (dist > distancetobase(i, base, entity, hero, 1) && entity[i].type == 0)
        {
            dist = distancetobase(i, base, entity, hero, 1);
            p = i;
        }
        i++;
    }
    return (p);
}

int closestenemyidbenemy(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int dist;
    int p;

    i = 0;
    p = 0;
    dist = 20000;
    while (i < base.entity_count)
    {   
        if (dist > distancetobase(i, base, entity, hero, 0) && entity[i].type == 0)
        {
            dist = distancetobase(i, base, entity, hero, 0);
            p = i;
        }
        i++;
    }
    return (p);
}

int secondclosest(t_base base, t_entity *entity, t_hero *hero)
{
    int m = closestenemyid(base, entity, hero);
    int i;
    int p;
    int dist;

    dist = 10000;
    p = 0;
    i = 0;
    while (i < base.entity_count)
    {   
        if (dist > distancetobase(i, base, entity, hero, 0) && entity[i].type == 0 && i != m)
        {
            dist = distancetobase(i, base, entity, hero, 0);
            p = i;
        }
        i++;
    }
    return (p);
}

int closestenemydist(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int dist;
    int p;

    i = 0;
    p = 0;
    dist = 20000;
    while (i < base.entity_count)
    {   
        if (dist > distancetobase(i, base, entity, hero, 1) && entity[i].type == 0 )
        {
            dist = distancetobase(i, base, entity, hero, 1);
            p = i;
            
        }
        i++;
    }
    return (dist);
}

int closestenemytohero(t_base base, t_entity *entity, t_hero *hero, int hero_id)
{
    int i;
    int dist;
    int p;

    i = 0;
    p = 0;
    dist = 10000;
    while (i < base.entity_count)
    {
        if (dist > distance(hero[hero_id].x, hero[hero_id].y, entity[i].x, entity[i].y) && entity[i].type == 0)
        {
            dist = distance(hero[hero_id].x, hero[hero_id].y, entity[i].x ,entity[i].y);
            p = i;
        }
        i++;
    }
    fprintf(stderr, "closest enemy to hero i = %d id = %d hero_id = %d\n", p, entity[p].id, hero_id);
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
        if ((6000 > distancetobase(i, base, entity, hero, 1) || entity[i].threat_for == 1) && entity[i].type == 0 && entity[i].targetted != 1)
        {
            p = i;
            entity[i].threat = 1;
        }
        //hp erbij?
        if (entity[i].near_base == 1 && entity[i].targetted != 1)
        {
            p = i;
            entity[i].threat = 2;
        }
        if (entity[i].near_base == 1 && entity[i].targetted != 1) //&& enemyhero() == 1)
        {
            p = i;
            entity[i].threat = 3;
        }
        // if (i > 2)
        //     fprintf(stderr, "threat = %d", entity[i].threat);
        i++;
    }
}

int countthreats(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int p;

    i = 0;
    p = 0;
    while (i < base.entity_count)
    {
        if (entity[i].threat > 0)
        {
            p++;
        }
        i++;
    }
    return (p);
}

int smartwindwall(int defender, t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int j;
    int closest_enemyid;
    int closest_enemydist;
    int enemywindwall;
    static int maxenemywindwall;
    int windwall;
    int monstershield;

    monstershield = 0;
    windwall = 0;
    i = 0;
    j = 0;

    if (defender >= 0)
    {
        closest_enemyid = closestenemyid(base, entity, hero);
        fprintf(stderr, "wind closest id = %d\n", entity[closest_enemyid].id);
        closest_enemydist = closestenemydist(base, entity, hero);
        fprintf(stderr, "wind dist = %d\n", closest_enemydist);
        //dichtbijbase
        if (closest_enemydist < 1000 && base.time < 80 )
            windwall = 1;
        else if (closest_enemydist < 1700 && base.time > 79)
            windwall = 1;
        else if (closest_enemydist < 2500 && base.time > 107)
            windwall = 1;
        //fprintf(stderr, "windwall = %d\n", windwall);
        //hoge vx vy en als dubbele push defensiever spelen
        while (i < base.entity_count)
        {
            //fprintf(stderr, "i = %d\n", i);
            if (entity[i].shield_life > 0 && entity[i].type == 0)
            {
                //fprintf(stderr, "windshield = %d\n", i);
                monstershield = 1;
            }
            i++;
        }
        //fprintf(stderr, "windcounttreats = %d\n", i);
        if (countthreats(base, entity, hero) > 1 && monstershield == 1 && entity[closest_enemyid].shield_life == 0)
        {
            windwall = 1;
           // fprintf(stderr, "windwalltreats = %d\n", windwall);
        }
    }
    if (defender >= 0 && windwall == 1 && in_range(1, defender, closest_enemyid, base, entity, hero, 1700) && (entity[closest_enemyid].shield_life == 0 ))
    {
        return (1);
        //fprintf(stderr, "wind return 1\n");
    }
    //fprintf(stderr, "geen wind\n");
    return (0);
}

int hero_distance_enemyhero(int id_hero, t_base base, t_hero *hero, t_entity *entity)
{
    int i;
    int j;
    int k;

    i = 0;
    j = 10000;
    while (i < base.entity_count)
    {
        if (j > distance(hero[id_hero].x, hero[id_hero].y, entity[i].x, entity[i].y) && entity[i].type == 2)
            j = distance(hero[id_hero].x, hero[id_hero].y, entity[i].x, entity[i].y);
        i++;
    }
    return (j);
}

int attackthreat1(t_base base, t_hero *hero, t_entity *entity, int hero_id)
{
    int i;
    int j;
    int closest;

    j = closestenemyid(base, entity, hero);
    //targetted == 0;
    closest = closestenemytohero(base, entity, hero, hero_id);
    if (entity[closest].threat_for == 1)
    {
        //entity[closest].targetted = 1;
        printf("MOVE %d %d\n", entity[closest].x, entity[closest].x);
        return (1);
    }
    return (0);
}

int controlspell(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int hero_id;
    int control_spell;
    int count;

    count = 0;
    hero_id = 0;
    control_spell = 0;
    while(hero_id < 3)
    {
        
        if (hero[hero_id].controlled == 1 )
        {   
            hero[hero_id].controlled = 1;
            count += hero_id + 1;
        }
        hero_id++;
    }
    fprintf(stderr, "controlcount = %d\n", count);
    return (count);
}

int spellshield(t_base base, t_entity *entity, t_hero *hero, int hero_id)
{
    int i;
    int j;
    int distances;
    int p;

    p = 0;
    i = 0;
    distances = 20000;
    while (i < base.entity_count)
    {
        if (distances > distanceto(i, hero_id, base, entity, hero) && entity[i].type == 2)
        {
            distances = distanceto(i, hero_id, base, entity, hero);
            p = i;
        }
        i++;
    }
    //fprintf(stderr, "distance spellshield = %d\n", distance);
    if ((hero[hero_id].shield_life <= 1))
    {
        //fprintf(stderr, "shield = 1\n");
        return (1);
    }
    //fprintf(stderr, "shield = 0\n");
    return (0);
}

void defenseone(int defenders, t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int j;
    int threatcount;
    int potential;
    int k;
    int n;
    k = 0;
    i = 0;
    j = 0;
    n = 1;
    if (base.x == 0)
        n = -n;
    while (i < defenders)
    {
        int c = closestenemyid(base, entity, hero); //4000 - 2000
        int c2 = secondclosest(base, entity, hero); //1000 - 2000
        int bestx = 0;
        int besty = 0;
        if (in_range(0, c2, c, base, entity, hero, 800))
        {
            bestx = (entity[c].x + entity[c2].x) /2;
            besty = (entity[c].y + entity[c2].y) /2;
        }
        //fprintf(stderr, "shieldstatus = %d\n", entity[i].shield_life);
        j = closestenemyid(base, entity, hero);
        k = closestenemytohero(base, entity, hero, i);
        //fprintf(stderr, "one closestid = %d\none hero = %d\n", entity[j].id, i);
        //fprintf(stderr, "one smarwind = %d + i = %d\n", smartwindwall(i, base, entity, hero), i);
        if (smartwindwall(i, base, entity, hero) == 1 && in_range(1, i, j, base, entity, hero, 1280) && entity[j].shield_life == 0)
            printf("SPELL WIND %d %d\n", base.e_x, base.e_y);
        else if (spellshield(base, entity, hero, i) == 1 && hero[i].controlled == 1 && base.time > 20 && hero_distance_enemyhero(i, base, hero, entity) < 3400)
            printf("SPELL SHIELD %d\n", hero[i].id);
        else if (j > 2 && distancetobase(j, base, entity, hero, 1) < 8000)
        {
            //fprintf(stderr, "one 2e = %d\n", entity[j].id);
            if (in_range(0, c2, c, base, entity, hero, 1600))
            {
                bestx = (entity[c].x + entity[c2].x) /2;
                besty = (entity[c].y + entity[c2].y) /2;
                printf("MOVE %d %d\n", bestx, besty);
            }
            else
            {
                entity[j].targetted = 1;
                printf("MOVE %d %d\n", entity[j].x-300*n, entity[j].y-300*n);
            }
            }
        else if (base.x == 0 && entity[k].x > 0 && entity[k].x < 8500 && entity[k].y > 0 && entity[k].y < 2500 && k > 0 && distancetobase(k, base, entity, hero, 1) < 8000)
        {
           // fprintf(stderr, "one 3e = %d\n", entity[j].id);
            entity[k].targetted = 1;
            printf("MOVE %d %d\n", entity[k].x-300*n, entity[k].y-300*n);
        }
        else if (base.x != 0 && entity[k].x > 8000 && entity[k].x < 17600 && entity[k].y > HEIGHT - 3000 && entity[k].y < 9000 && k > 0 && distancetobase(k, base, entity, hero, 1) < 8000)
        {
            //fprintf(stderr, "one 4e = %d\n", entity[j].id);
            entity[k].targetted = 1;
            printf("MOVE %d %d\n", entity[k].x, entity[k].y);
        }
        else if (base.x == 0)
        {
            if (base.time % 2 == 0)
                printf("MOVE %d %d\n", base.x + 8500 , base.y + 1500);
            else
                printf("MOVE %d %d\n", base.x + 6600, base.y + 1500);
        }
        else
        {
            if (base.time % 2 == 0)
                printf("MOVE %d %d\n", base.x - 8500 , base.y - 1500);
            else
                printf("MOVE %d %d\n", base.x - 6600, base.y - 1500);
        }
        i++;
    }           
}
int closestattackersdist(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int dist;
    int p;

    dist = 10000;
    i = 0;
    while (i < base.entity_count)
    {
        if (entity[i].type == 2 && dist > distance(base.x, base.y, entity[i].x, entity[i].y))
        {
            dist = distance(base.x, base.y, entity[i].x, entity[i].y);
        }
        i++;
    }
    return (dist);
}

int defendersi(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int count = 0;
    i = 0;
    int p;

    p = -1;
    while (i < base.entity_count)
    {
        if (entity[i].type == 2 && distancetobase(i, base, entity, hero, 0) < 6000)
        {
            p = i;
        }
        i++;
    }
    fprintf(stderr, "defi2 = %d + %d\n", i, entity[i].id);
    return (p);
}
int defenderscount(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int count = 0;
    while (i < base.entity_count)
    {
        if (entity[i].type == 2 && distancetobase(i, base, entity, hero, 0) < 5000)
            count++;
        i++;
    }
    return (count);
}

int potential(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < base.entity_count)
    {
        if (distancetobase(i, base, entity, hero, 0) < 7200 && entity[i].threat_for == 2)
        {
            return (i);
        }
        if (entity[i].near_base == 1 && distancetobase(i, base, entity, hero, 0) < 5000 || (distancetobase(i, base, entity, hero, 0) < 7000))
        {
            fprintf(stderr, "potentie id = %d", entity[i].id);
            entity[i].potentie = 1;
        }
        i++;
    }
    return (0);
}

void    attacktwo(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int j;
    int k;
    int aantaldefenders;
    int potentiaal;
    int n;
    int controlled;
    int b;
    int hero_i;
    int closestenen;

    if (base.x == 0)
        hero_i = 2;
    else
        hero_i = 2;
    /*
    na 50 seconden loop rondom basis en 
    if 1 defender, binnen bereik, niet geshield en monster geshield binnen 2500, control defender
    push monsters, binnen bereik en verder dan 5000 enemy base
    als monster binnen 3500 shield
    loop de cirkel af op zoek naar monsters 
    */
    k = 0;
    i = 0;
    j = 0;
    n = 1;
    controlled = 0;
    potentiaal = 0;
    closestenen = 0;
    
    b = potential(base, entity, hero);
    k = closestenemytohero(base, entity, hero, hero_i);
    n = closestenemytohero(base, entity, hero, hero_i);
    closestenen = closestenemyidbenemy(base, entity, hero);
    if (base.time < 80 && base.x == 0)
    {
        fprintf(stderr, "id %d target = %d", entity[k].id, entity[k].targetted);
        if (k > 0 && entity[k].targetted != 1 && distancetobase(k, base, entity, hero, 1) > 7000)
            printf("MOVE %d %d\n", entity[k].x, entity[k].y);
        else
            printf("MOVE %d %d\n", 6000, 7700);
    }
    else if (base.time < 80 && base.x != 0)
    {
        fprintf(stderr, "id %d target = %d", entity[k].id, entity[k].targetted);
        if (k >= 0 && entity[k].targetted == 0 && distancetobase(k, base, entity, hero, 1) > 6000)
            printf("MOVE %d %d\n", entity[k].x, entity[k].y);
        else
            printf("MOVE %d %d\n", 17600 - 6000, 9000 - 7700);
    }
    else
    {
        fprintf(stderr, "%d , %d , %d , %d , %d\n", hero_i, entity[hero_i].id, hero[hero_i].id, hero[hero_i].shield_life, entity[k].id);
        fprintf(stderr, "else hero_i = %d heroid = %d dist = %d + id = %d\n", hero_i, entity[hero_i].id, distanceto(hero_i, closestenen, base, entity, hero), entity[closestenen].id);
        k = defendersi(base, entity, hero);
        fprintf(stderr, "controlled %d\n", hero[2].controlled);
        if (hero[2].controlled == 1 && hero[hero_i].shield_life <= 1)
             printf("SPELL SHIELD %d\n", hero[hero_i].id);
        else if (distancetobase(closestenen, base, entity, hero, 0) < 4000 && entity[closestenen].type == 0 && entity[closestenen].shield_life == 0 && base.time > 70 && hero[0].mana > 30)
            printf("SPELL SHIELD %d\n", entity[closestenen].id);
        else if (defenderscount(base, entity, hero) < 1 && entity[k].shield_life <= 1 && in_range(1, hero_i, k, base, entity, hero, 2200) && k >= 0 && hero[0].mana > 50 && distancetobase(closestenen, base, entity, hero, 0) < 5000)
            printf("SPELL CONTROL %d %d %d\n", entity[k].id, WIDTH, 0);
        else if (closestenen > 0 && in_range(2, hero_i, closestenen, base, entity, hero, 1280) == 1 && entity[closestenen].type == 0 && (distancetobase(closestenen, base, entity, hero, 0) < 7000 || entity[closestenen].threat_for == 1)) //&& distancetobase(k, base, entity, hero, 0) > 4000)
            printf("SPELL WIND %d %d\n", base.e_x, base.e_y);
        else if (distancetobase(closestenen, base, entity, hero, 0) < 3000 && entity[closestenen].type == 0 && entity[closestenen].shield_life == 0)
            printf("SPELL SHIELD %d\n", entity[closestenen].id);
        else if (distancetobase(n, base, entity, hero, 0) < 3500 && entity[n].type == 0 && entity[n].shield_life == 0)
            printf("SPELL SHIELD %d\n", entity[n].id);
        else if (base.x == 0)
            printf("MOVE %d %d\n", 17600 - 2000, 9000 - 2000);
        else
            printf("MOVE %d %d\n", 2300, 2300);
    }

}

void    defense(t_base base, t_entity *entity, t_hero *hero)
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

    //how many defenders
    defenseone(2, base, entity, hero);
    //attackone(1, base, entity, hero);
    attacktwo(base, entity, hero);
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
    int cspell;
    int wt;

    wt = 0;
    cspell = 0;
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
                hero[b].controlled = entity[i].is_controlled;
				b++;
			}
		}
        counter = 0;
        while (counter < base.entity_count)
        {
            if (counter > 0)
            {
                entity[i].targetted = 0;
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
                //fprintf(stderr, "i = %d vx = %d vy = %d\n", entity[counter].id, entity[counter].vx, entity[counter].vy);
            }
            counter++;
        }
        threats(base, entity, hero);
        if (cspell < hero[2].controlled)
        {
            cspell = 1;
        }
        hero[2].controlled = cspell;
        if (wt < hero[0].controlled || wt < hero[1].controlled)
            wt = 1;
        hero[0].controlled = wt;
        hero[1].controlled = wt;

        fprintf(stderr, "laatse cspell = %d\n", cspell);
        //fprintf(stderr, "threatscount = %d\n", countthreats(base, entity, hero));
        // fprintf(stderr, "smartwindwall = %d\n", smartwindwall(1, base, entity, hero));

        //fprintf(stderr, "cspellmain = %d",  entity[0].controlled);
        //defenseone(1, base, entity, hero);
        defense(base, entity, hero);
		free (entity);
		base.time++;
	}
	free (hero);
	return (0);
}
