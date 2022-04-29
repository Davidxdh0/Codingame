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
        closest_enemydist = closestenemydist(base, entity, hero);
        //dichtbijbase
        if (closest_enemydist < 1000 && base.time < 80)
            windwall = 1;
        else if (closest_enemydist < 1700 && base.time > 79)
            windwall = 1;
        else if (closest_enemydist < 2500 && base.time > 107)
            windwall = 1;
        while (i < base.entity_count)
        {
            if (entity[i].shield_life > 0 && entity[i].type == 0)
                monstershield = 1;
            i++;
        }
        if (countthreats(base, entity, hero) > 1 && monstershield == 1 && entity[closest_enemyid].shield_life == 0)
            windwall = 1;
 
    }
    if (defender >= 0 && windwall == 1 && in_range(1, defender, closest_enemyid, base, entity, hero, 1700) && (entity[closest_enemyid].shield_life == 0 ))
    {
        return (1);
        //fprintf(stderr, "wind return 1\n");
    }
    //fprintf(stderr, "geen wind\n");
    return (0);
}