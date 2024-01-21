#ifndef FIGHT_H
#define FIGHT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//###########################################################################################
// Encounter
const int potionHeal = 10;
const int attackModifier = 2;

typedef struct inventory
{
    int potions;
    int arrows;
}inventory_t;

typedef struct character
{
    char* name;
    int health;
    int attack;
    int armor;
    inventory_t inv;
}character_t;

int clearScreen();
int meleAttack(character_t*, character_t*);
int rangedAttack(character_t*, character_t*);
int drinkPotion(character_t*);
int showStatus(character_t*);
int playerTurn(character_t*, character_t*);
int enemyTurn(character_t*, character_t*);
int enemyDeath(character_t*);
int figtLoop(character_t*, character_t*);
int checkDeath(character_t*, character_t*);
int findHeal(character_t*);
int findTresure(character_t*);
int findTrap(character_t*);

int clearScreen(){
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 11);
    return 0;
}

//All character init
int initZombies(character_t*);
int initPlayer(character_t*);
int initBoss(character_t*);

int meleAttack(character_t* attacker, character_t* defender){
    if(attacker->attack >= defender->armor){
        defender->health -= (attacker->attack - defender->armor);
        printf("The attack landed and dealt %d damage\n", (attacker->attack - defender->armor));
    }else{
        if(defender->attack >= attacker->armor){
            attacker->health -= (defender->attack - attacker->armor);
            printf("The attack bonced off the defender's armor. The defender conter attacked and dealt %d damage\n", (defender->attack - attacker->armor)*attackModifier);
        }else{
            printf("The attack bonced off the defender's armor. The defender conter attacked but it also bonced off\n");
        }
    }
    return 0;
}

int rangedAttack(character_t* attacker, character_t* defender){
    attacker -> inv.arrows--;
    if(attacker->attack*attackModifier >= defender->armor){
        defender->health -=  (attacker->attack - defender->armor)*attackModifier;
        printf("The arrow pierced the armor and dealt %d damage\n", (attacker->attack - defender->armor)*attackModifier);
    }else{
        printf("The arrow bonced off the defender's armor\n");
    }
    return 0;
}

int showStatus(character_t* charact){
    printf("---   %s :   ---\n", charact->name);
    printf("health: %d   |", charact->health);
    printf("armor: %d   |", charact->armor);
    printf("potions: %d\n", charact->inv.potions);
    printf("attack: %d   |", charact->attack);
    printf("arrows: %d\n", charact->inv.arrows);
    return 0;
}

int drinkPotion(character_t* drinker){
    drinker->health += potionHeal;
    drinker->inv.potions--;
    return 0;
}

int playerTurn(character_t* player, character_t* enemy){
    printf("You are faced with a %s. What will you do ?\n", enemy->name);
    printf("1: Mele attack   -   2: Skip Turn\n");
    if(player->inv.arrows > 0){
        printf("3: Ranged attack");
    }
    if(player->inv.arrows > 0 && player->inv.potions > 0){
        printf("   -   ");
    }
    if(player->inv.potions > 0){
        printf("4: Drink a potion\n");
    }
    while (true)
    {
        char var;
        scanf(" %c", &var);
        if(var == '1'){
            printf("You swing your sword at the enemy\n");
            meleAttack(player, enemy);
            return 0;
        }
        if(var == '2'){
            printf("The enemy looks at you confused as to why you would do that.\n");
            return 0;
        }
        if(var == '3'){
            printf("You fire an arrow at the enemy\n");
            rangedAttack(player, enemy);
            return 0;
        }
        if(var == '4'){
            printf("You drink a potion.\nhealth +%d\n", potionHeal);
            drinkPotion(player);
            return 0;
        }
    }

    return 0;
};

int enemyTurn(character_t* player, character_t* enemy){
    if(enemy->health < 15 && enemy->inv.potions > 0){
        printf("%s drinks a potion\n", enemy->name);
        drinkPotion(enemy);
    }else if (enemy->inv.arrows > 0)
    {
        printf("%s fire an arrow at you\n", enemy->name);
        rangedAttack(enemy, player);
    }else{
        printf("%s attack you\n", enemy->name);
        meleAttack(enemy, player);
    }

    return 0;
}

//this is only a function to make sure I don't forget to free their names
int enemyDeath(character_t* enemy){
    free(enemy->name);

    return 0;
};

// init character
int initZombies(character_t* zomby){
    zomby->armor = 10;
    zomby->health = 20;
    zomby->attack = 10;

    char* p = (char*)malloc(sizeof(char)*6);
    memcpy(p, "Zomby", sizeof(char)*6);
    zomby->name = p;

    zomby->inv.arrows = 0;
    zomby->inv.potions = 1;

    return 0;
};

int initPlayer(character_t* player){
    player->armor = 15;
    player->health = 50;
    player->attack = 17;

    char* p = (char*)malloc(sizeof(char)*7);
    memcpy(p, "Player", sizeof(char)*7);
    player->name = p;

    player->inv.arrows = 4;
    player->inv.potions = 2;

    return 0;
}

int initBoss(character_t* boss){
    boss->armor = 17;
    boss->health = 50;
    boss->attack = 20;

    char* p = (char*)malloc(sizeof(char)*15);
    memcpy(p, "Evilus Maximus", sizeof(char)*15);
    boss->name = p;

    boss->inv.arrows = 10;
    boss->inv.potions = 5;

    return 0;
}

int figtLoop(character_t* player, character_t* enemy){
    clearScreen();
    printf("The enemy sees you. You'll have to fight them\n");

    while (true){
        showStatus(player);
        printf("\n");
        showStatus(enemy);
        printf("\n");
        playerTurn(player, enemy);
        if(checkDeath(player, enemy) == 1){
            break;
        }
        if(checkDeath(player, enemy) == 2){
            return 2;
        }

        printf("\n");
        enemyTurn(player, enemy);
        if(checkDeath(player, enemy) == 1){
            break;
        }
        if(checkDeath(player, enemy) == 2){
            return 2;
        }

        printf("enter any character to continue :\n");
        char var;
        scanf(" %c", &var);
        if(var){
            clearScreen();
        }
    }

    return 0;
}

int checkDeath(character_t* player, character_t* enemy){
    if(enemy->health <= 0){
        printf("You have falen %s enemy\n", enemy->name);
        enemyDeath(enemy);

        printf("enter any character to continue :\n");
        char var;
        scanf(" %c", &var);
        if(var){
            clearScreen();
        }
        return 1;
    }
    if(player->health <= 0){
        return 2;
    }

    return 0;
}

int findHeal(character_t* player){
    player->inv.potions++;
    printf("You found a potion of healing in a chest\n");

    return 0;
}

int findTresure(character_t* player){
    int radomInt = rand() % (20 - 0);

    if(radomInt <= 7 ){
        printf("You found a new shiny armor in a chest\n");
        player -> armor += 2;
    }else if(radomInt <= 14 ){
        printf("You found a new shiny sword in a chest\n");
        player -> attack += 2;
    }else{
        printf("You found demi dozen arrows in a chest\n");
        player -> inv.arrows += 6;
    }

    return 0;
}

int findTrap(character_t* player){
    player->health -= 8;
    printf("You fell into a trap\n-8 health\n");

    if(player->health <= 0){
        return 2;
    }

    return 0;
}

#endif