void early(t_base base, t_entity *entity, t_hero *hero, int bas, int time)
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

    i = base.entity_count;
    if (time < 5)
    {
        printf("MOVE 3500 8000\n");
        printf("MOVE 6600 8800\n");
        printf("MOVE 7400 800\n");
    }
    if (time > 5)
    {
        j = closestenemytohero(base, entity, hero, 0);
        if (closestenemy(base, entity, hero) == j && entity[j].targetted != 1)
        {
            entity[j].targetted = 1;
            printf("MOVE %d %d\n", entity[j].x, entity[j].y);
        }
        else if (j > 0 && entity[j].targetted != 1)
        {
            entity[j].targetted = 1;
            printf("MOVE %d %d\n", entity[j].x, entity[j].y);
        }
        {
            printf("MOVE 3500 8000\n");
        }
        j = closestenemytohero(base, entity, hero, 1);
        if (closestenemy(base, entity, hero) == j && entity[j].targetted != 1)
        {
            entity[j].targetted = 1;
            printf("MOVE %d %d\n", entity[j].x, entity[j].y);
        }
        else if (j > 0 && entity[j].targetted != 1)
        {
            entity[j].targetted = 1;
            printf("MOVE %d %d\n", entity[j].x, entity[j].y);
        }
        {
            printf("MOVE 6600 8800\n");
        }
        j = closestenemytohero(base, entity, hero, 2);
        if (closestenemy(base, entity, hero) == j && entity[j].targetted != 1)
        {
            entity[j].targetted = 1;
            printf("MOVE %d %d\n", entity[j].x, entity[j].y);
        }
        else if (j > 0 && entity[j].targetted != 1)
        {
            entity[j].targetted = 1;
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


    while (time < 50)
    {
        early(base, entity, hero, base.x, base.time);
    }
    printf("MOVE 650 650\n");
    printf("MOVE 650 650\n");
    printf("MOVE 650 650\n");
}
        