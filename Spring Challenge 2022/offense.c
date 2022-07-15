void attack(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int bal;
    int p;

    p = 1;
    i = 0;
    bal = 0;
    //fprintf(stderr, "i = %d count = %d bal = %d", i, base.entity_count, bal);
   // p = closestenemy(base, entity, hero);
    while (i < base.entity_count && bal == 0)
    {
        p = closestenemy(base, entity, hero);
        //fprintf(stderr, "p = %d , base enemy %d %d", p, base.e_x, base.e_y);
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
            if (distance(entity[i].x + entity[i].vx, entity[i].y + entity[i].y, base.e_x, base.e_y))
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
    if (bal == 1)
        printf("WIND %d %d\n", WIDTH, HEIGHT);
    else if (bal == 2)
        printf("shield %d\n", entity[i].id);
    else if ( bal == 0)
        printf("MOVE %d %d\n", 15500 , 4000);
    else if (bal == 4)
        printf("MOVE %d %d\n", entity[i].x, entity[i].y);
    else if (bal == 5)
         printf("MOVE %d %d\n", entity[i].x, entity[i].y);
}