
{
    int i;
    int j;

    hero_i = 0;
    id_monster = 0;

    k = 0;
    i = 0;
    j = 0;
    hero_i = 0;
    spell = 0;  
    //defense
    if (life > 2, monsters niet te dichtbij en enemy hero niet binnen 7000)
    {    
        1 man verdedigen rond de 5000 + control monsters als 
        na 80 als er meer dan 3 enemies in base shield + windwall
        //aanval 2 man - na 53 control monsters als enemy niet naar hun base gaat + 1 goal scorer
        // 
        // binnen 5000 en hp is meer dan x, dubbele windpush
    }
    if (life < 2, meer hp dan enemy of meer monsters of enemy hero binnen 7000 gespot)
    {
       2 defend dichterbij, shielden, windwall binnen 4000 of wanneer vx, vy te hoog is.
       //aanval 1 man
       // control monsters + control andere heros, loop met hero mee en windpush hero weg
    }

    aanval 2 man: 

    aanvaller ziet verdedigers geen spellshield, schiet ze tegenovergestelde kant, weg van doel, van dichtste monster
    
    als dat niet kan spellshield de monster en zorg dat ie op afstand blijft


void target(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int j;
    int threatcount;
    int potential;
    int defenders;

    
    i = 0;
    j = 0;
    threatcount = 0;
    potential = 0;
    defenders = 0;
    if (base.time < 54)
        defense(1)
        early()
    if (time > 53 && base.time < 90)
        defense(1);
        attacktwo(2);
    if (time >= 90 && defenders == 2)
        defense(2);
        attackone(1);
    if (time >= 90 && defenders == 1)
        defense 2 met shields
        aanval controllen
}

int spellshield(t_base base, t_entity *entity, t_hero *hero)
{
    int i;
    int j;
    int distance;

    distance = 10000;
    while (i < entity)
    {
        if entity = enemy hero
        {
            distance = enemy hero en defender hero
        }
    }
    if (enemycontrolcast == 1 && distance < 2300 && distanceclosestenemybase < 5500 && (hero geen spellshield || laatste tick))
        return 1
    return 0
}

int controlspell()
{
    static int controlspell;
    static int i;

    controlspell = 0;
    if (defender.controlled == 1)
    {
        i = 1;
        controlspell = i;
    }
    return (controlspell)
}

int hero_distance_enemyhero(int id_hero, t_base base, t_hero *hero, t_entity *entity)
{
    int i;
    int j;
    int k;

    i = 0;
    j = 20000;
    while (i < base.entity_count)
    {
        if (j > distance(hero[id_hero].x, hero[id_hero].y, entity[i].x, entity[i].y) && entity.type == 2)
            j = distance(hero[id_hero].x, hero[id_hero].y, entity[i].x, entity[i].y);
        i++;
    }
    return (j);
}

int attackthreat1(t_base base, t_hero *hero, t_entity *entity)
{
    int i;
    int j;
    int closest;

    j = closestenemybase();
    //targetted == 0;
    closest = closestenemytohero(t_base base, t_entity *entity, t_hero *hero, int hero_id)
    if entity[closest].threat_for == 1;
    {
        printf("MOVE % %", entity[closest].x entity[closest].x);
        return 1
    }
    return 0
}


defense(int defenders)
{
    int i;
    int j;
    int threatcount;
    int potential;
    int defenders;

    k = 0;
    i = 0;
    j = 0;

    if (defenders == 1)
    {
        j = closestenemybase()
        k = closestenemytohero(base, entity, hero, hero_id)
        if (smartwindwall(1))
            printf("wind base.e_x base.e _y")
        else if (spellshield() == 1
            printf("spellshield defender");
        else if (entity[0].shieldLife > 0 || hero_distance_enemyhero(0) < 2300 || !controlspell()
            printf("MOVE % %", entity[j].x entity[j].x);
        else if (entity[k].x > x && entity[k].x < x && entity[k].y > y && entity[k].y < y)
            printf("MOVE % %", entity[k].x entity[k].y);
        else
        {
            if (base.time % 4 == 0)
                printf("MOVE % %", base.x + 900, base.y + 900)
            else
                printf("MOVE % %", base.x + 2000, base.y + 2000)
        }
    }           
}