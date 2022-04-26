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

int distanceto(int id, int id_hero, int basex, int basey, t_base base, t_entity *entity, t_hero *hero)
{
    int distances;

    distances = 0;
    if (basex == 0)
    {
        distances = distance(entity[id_hero].x, entity[id_hero].y, entity[id].x, entity[id].y);
    }
    if (basex == 1)
    {
        distances = distance(entity[id].x, entity[id].y, base_x, base_y);
    }
    return (distances);
}

int distancetohero(int id_hero, int counter, t_base base, t_entity *entity, t_hero *hero, int mijn)
{
    int distances;

    distances = 0;

    distances = distance(entity[counter].x, entity[counter].y, entity[id_hero].x, entity[id_hero].y);
    return (distances);
}

int distancetobase(int counter, int basex, t_base base, t_entity *entity, t_hero *hero)
{
    int distances;

    distances = 0;
    if (base == )
    {
        distances = distance(entity[id_monster].x, entity[id_monster].y, entity[hero_id].y, hero_id);
    }
    else if ( base == WIDTH || BASE == 0)
    {
        distances = distance(entity[id_monster].x, entity[id_monster].y, base, base);
    }
    return (distances);
}

int hero_closest_idenemy(int counter, int id_hero, t_base base, t_hero *hero, t_entity *entity)
{
    int i;
    int j;
    int k;

    i = 0;
    j = 20000;
    while (i < base.entity_count && hero[i].target != 1)
    {
        if (j > distance(hero[id_hero].x, hero[id_hero].y, entity[counter].x, entity[counter].y))
            j = i;
        i++;
    }
    return (j);
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

    i = 0;
    p = 0;
    dist = 10000;
    while (i < base.entity_count)
    {
        if (dist > distancetobase(i, -1, base, entity, hero, 1) && entity[i].type == 0 && entity[i].targetted == 0)
        {
            dist = distancetobase(i, -1, base, entity, hero, 1);
            p = entity[i].id;
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
        if (entity[i].type == 0)
        {
            if (dist > distance(entity[hero_id].x, entity[hero_id].y, entity[i].x, entity[i].y) && entity[i].type == 0 && entity[i].targetted == 0)
            {
                dist = distance(entity[hero_id].x, entity[hero_id].y, entity[i].x ,entity[i].y);
                p = entity[i].id;
                // fprintf(stderr, "distance = %d, i = %d closest enemy id = %d\n",dist, i, p);
                // fprintf(stderr, "dist = %d\n", distance(entity[i].x, entity[i].y, base.x, base.y));
            }
            i++;
        }
    }
    // fprintf(stderr, "closest enemy id = %d\n", p);
    //fprintf(stderr, "id = %d threats = %d\n", entity[p].id, entity[p].threat);
    fprintf(stderr, "p=%d id=%d threats = %d\n", p, entity[p].id, entity[p].threat);
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
        if (6000 > distancetobase(i, -1, base, entity, hero, 1) && entity[i].type == 0)
        {
            
            p = entity[i].id;
            entity[p].threat = 1;
            
        }
        //hp erbij?
        if (3000 > distancetobase(i, -1, base, entity, hero, 1) && entity[i].type == 0)
        {
            
            p = entity[i].id;
            entity[p].threat = 2;
            
        }
        i++;
    }
}