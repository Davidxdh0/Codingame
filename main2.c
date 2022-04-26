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