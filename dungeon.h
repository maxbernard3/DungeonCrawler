#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "encounter.h"

//###########################################################################################
// Dungeon
character_t PLAYER;

const int dungeonSize = 14;

enum tileType {start, empty, boss, zomby, heal, treasure, trap};
enum wall {no, top, right, botom, left, cornerTR, cornerBR, cornerBL, cornerTL};
enum cardinal {north, east, west, south};

typedef struct tile{
    enum tileType type;
    bool cleared;
    enum wall walls;
    bool player;
}tile_t;

int initMaze(tile_t*, int*);
int describeEnv(tile_t*, int);
int lookAround(tile_t*, int, enum cardinal);
int roomAction(tile_t*, int, character_t*);
int moveCardinal(tile_t*, int*, enum cardinal);
int mainLoop(tile_t*, int*, bool*, character_t*);
int showMaze(tile_t*);
int initFight(tile_t*, int, character_t*);

int initMaze(tile_t* maze, int* player){
    for (int i = 0; i < dungeonSize*dungeonSize; i++)
    {
        maze[i].cleared = false;
        maze[i].player = false;

        if(i%dungeonSize == 0 && i != 0){
            maze[i].walls = right;
        }else if (i%dungeonSize == dungeonSize-1 || i == 0)
        {
            maze[i].walls = left;
        }else if (i <= dungeonSize)
        {
            maze[i].walls = top;
        }else if (i >= ((dungeonSize*dungeonSize)-dungeonSize))
        {
            maze[i].walls = botom;
        }else{
            maze[i].walls = no;
        }

        int radomInt = rand() % (100 - 0);
        if (radomInt < 5){

            maze[i].type = trap;
        }else if (radomInt < 20){

            maze[i].type = zomby;
        }else if (radomInt < 40){

            maze[i].type = heal;
        }else if(treasure < 60){

            maze[i].type = treasure;
        }else{

            maze[i].type = empty;
        }
    }

    const int TR = dungeonSize-1;
    const int BL = dungeonSize*dungeonSize-dungeonSize;
    const int BR = dungeonSize*dungeonSize-1;
    
    maze[0].walls = cornerTL;
    maze[TR].walls = cornerTR;
    maze[BL].walls = cornerBL;
    maze[BR].walls = cornerBR;

    int radomInt = rand() % (195 - 0);
    maze[radomInt].type = boss;

    radomInt = rand() % (195 - 0);
    maze[radomInt].type = start;
    maze[radomInt].player = true;
    maze[radomInt].cleared = true;
    *player = radomInt;

    return 0;
}


int describeEnv(tile_t* maze, int i){
    switch (maze[i].type)
    {
    case (boss):
        printf("There is an ominous looking monster in this room\n");
        return 0;
    case (zomby):
        printf("There is a zoomby in this room. he doesn't look very menacing\n");
        return 0;
    case (heal):
        int roll = rand() % (20 - 0);
        if(roll >= 14){
            printf("There is a potion of healing in the room\n");
        }else{
            printf("The room looks empty\n");
        }
        return 0;
    case (treasure):
        roll = rand() % (20 - 0);
        if(roll >= 12){
            printf("There is an ancient relic in the room\n");
        }else{
            printf("The room looks empty\n");
        }
        return 0;
    case(trap):
        roll = rand() % (20 - 0);
        if(roll >= 10){
            printf("The room floor seems to be traped\n");
        }else{
            printf("The room looks empty\n");
        }
        return 0;

    default:
        printf("The room looks empty\n");
        return 0;
    }
    return 1;
}

int lookAround(tile_t* maze, int i, enum cardinal direction){
    clearScreen();
    showMaze(maze);
    int room = maze[i].walls;
    switch (direction)
    {
    case north:
        if (room == top || room == cornerTL || room == cornerTR )
        {
            printf("there is a wall to the north\n");
        }else
        {
            printf("there is a door to the north. You peek behind the door.\n");
            describeEnv(maze, i-dungeonSize);
        }   
        break;
    
    case south:
        if (room == botom || room == cornerBL || room == cornerBR )
        {
            printf("there is a wall to the south\n");
        }else
        {
            printf("there is a door to the south. You peek behind the door.\n");
            describeEnv(maze, i+dungeonSize);
        }   
        break;
    
    case west:
        if (room == right || room == cornerBR || room == cornerTR )
        {
            printf("there is a wall to the west\n");
        }else
        {
            printf("there is a door to the west. You peek behind the door.\n");
            describeEnv(maze, i-1);
        }   
        break;
    
    case east:
        if (room == left || room == cornerBL || room == cornerTL )
        {
            printf("there is a wall to the east\n");
        }else
        {
            printf("there is a door to the east. You peek behind the door.\n");
            describeEnv(maze, i+1);
        }
        break;
    
    default:
        break;
    }
    return 1;
}

int roomAction(tile_t* maze, int i, character_t* player){
    switch (maze[i].type)
    {
    case heal:
        findHeal(player);
        break;
    case trap:
        int go = findTrap(player);
        if(go == 2){
            return 2;
        }
        break;
    case treasure:
        findTresure(player);
        break;
    case empty:
        break;
    default:
        go = initFight(maze, i, player);
        if(go == 2){
            return 2;
        }
        if(go == 3){
            return 3;
        }
        break;
    }
    return 0;
}

int moveCardinal(tile_t* maze, int* pi, enum cardinal direction){
    int i = *pi;
    int room = maze[i].walls;
    switch (direction)
    {
    case north:
        if (!(room == top || room == cornerTL || room == cornerTR ))
        {
            maze[i].cleared = true;
            maze[i].player = false;
            maze[i].type = empty;

            maze[i-dungeonSize].player = true;
            *pi = i-dungeonSize;
        }   
        break;
    
    case south:
        if (!(room == botom || room == cornerBL || room == cornerBR ))
        {
            maze[i].cleared = true;
            maze[i].player = false;
            maze[i].type = empty;

            maze[i+dungeonSize].player = true;
            *pi = i+dungeonSize;
        }
        break;
    
    case west:
        if (!(room == right || room == cornerBR || room == cornerTR))
        {
            maze[i].cleared = true;
            maze[i].player = false;
            maze[i].type = empty;

            maze[i-1].player = true;
            *pi = i-1;
        }   
        break;
    
    case east:
        if (!(room == left || room == cornerBL || room == cornerTL))
        {
            maze[i].cleared = true;
            maze[i].player = false;
            maze[i].type = empty;

            maze[i+1].player = true;
            *pi = i+1;
        }   
        break;
    
    default:
        return 1;
    }

    clearScreen();
    showMaze(maze);

    return 0;
}

int showMaze(tile_t* maze){
    showStatus(&PLAYER);
    printf("\n--- Maze ---\n");

    for (int i = 0; i < dungeonSize*dungeonSize; i++)
    {
        if(i%dungeonSize == 0 && i != 0){
            printf("\n");
        }

        if(maze[i].player == true){
            printf("0 ");
        }else if (maze[i].cleared == true){
            printf(". ");
        }else{
            printf("# ");
        }
    }
    printf("\n");

    return 0;
};

int initFight(tile_t* maze, int i, character_t* player){
    character_t enemy;

    switch (maze[i].type)
    {
    case zomby:
        initZombies(&enemy);
        int go = figtLoop(player, &enemy);
        if(go == 2){
            return 2;
        }

        clearScreen();
        showMaze(maze);
        printf("the room is now empty\n");
        break;

    case boss:
        initBoss(&enemy);
        go = figtLoop(player, &enemy);
        if(go == 2){
            return 2;
        }
        else{
            return 3;
        }
        break;
    
    default:
        return 1;
        break;
    }

    return 0;
}

int mainLoop(tile_t* maze, int* pi, bool* movement, character_t* player){
    while (true)
    {
        if(player->inv.potions > 0){
            printf("switch between movment [1] and observation [2] drink potion [3]\n");
        }else{
            printf("switch between movment [1] and observation [2]\n");
        }

        if(*movement){
            printf("move with Z-Q-S-D :\n");

            char var;
            scanf(" %c", &var);
            if(var == '1'){
                *movement = true;
                clearScreen();
                showMaze(maze);
            }else if(var == '2'){
                *movement = false;
                clearScreen();
                showMaze(maze);
            }
            else if(var == '3' && player->inv.potions > 0){
                drinkPotion(player);
                clearScreen();
                showMaze(maze);
                printf("You drink a potion.\nhealth +%d\n", potionHeal);
            }
            else if(var == 'Z' || var == 'z')
            {
                moveCardinal(maze, pi, north);
                int go =roomAction(maze, *pi, player);
                if(go == 2){
                    return 2;
                }
                if(go == 3){
                    return 3;
                }
                break;
            }else if (var == 'Q' || var == 'q')
            {
                moveCardinal(maze, pi, west);
                int go = roomAction(maze, *pi, player);
                if(go == 2){
                    return 2;
                }
                if(go == 3){
                    return 3;
                }
                break;
            }else if (var == 'S' || var == 's')
            {
                moveCardinal(maze, pi, south);
                int go = roomAction(maze, *pi, player);
                if(go == 2){
                    return 2;
                }
                if(go == 3){
                    return 3;
                }
                break;
            }else if (var == 'D' || var == 'd')
            {
                moveCardinal(maze, pi, east);
                int go = roomAction(maze, *pi, player);
                if(go == 2){
                    return 2;
                }
                if(go == 3){
                    return 3;
                }
                break;
            }else{
                clearScreen();
                showMaze(maze);
                printf("invalid input\n");
            }

        }else{
            printf("look around with I-J-K-L :\n");

            char var;
            scanf(" %c", &var);
            if(var == '1'){
                *movement = true;
                clearScreen();
                showMaze(maze);
            }else if(var == '2'){
                *movement = false;
                clearScreen();
                showMaze(maze);
            }
            else if(var == '3' && player->inv.potions > 0){
                drinkPotion(player);
                clearScreen();
                showMaze(maze);
                printf("You drink a potion.\nhealth +%d\n", potionHeal);
            }
            else if(var == 'I' || var == 'i')
            {
                lookAround(maze, *pi, north);
                break;
            }else if (var == 'J' || var == 'j')
            {
                lookAround(maze, *pi, west);
                break;
            }else if (var == 'K' || var == 'k')
            {
                lookAround(maze, *pi, south);
                break;
            }else if (var == 'L' || var == 'l')
            {
                lookAround(maze, *pi, east);
                break;
            }else{
                clearScreen();
                showMaze(maze);
                printf("invalid input\n");
            }
        }
    }
    return 0;
};

#endif