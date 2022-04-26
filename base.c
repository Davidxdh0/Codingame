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