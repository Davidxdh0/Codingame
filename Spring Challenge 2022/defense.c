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

