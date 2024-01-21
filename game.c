#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dungeon.h"

int restart(tile_t* maze, int* playerPos){
    enemyDeath(&PLAYER);
    printf("press A to restart :\n");
    char var;
    scanf(" %c", &var);
    if(var){
        initMaze(maze, playerPos);
        initPlayer(&PLAYER);
        clearScreen();
        showMaze(maze);
    }

    return 0;
};

int main(){
    //srand(time(NULL));

    tile_t maze[dungeonSize*dungeonSize];
    int playerPos;
    bool mov = false;
    initMaze(maze, &playerPos);
    initPlayer(&PLAYER);
    clearScreen();
    showMaze(maze);

    while (true)
    {
        int go = mainLoop(maze, &playerPos, &mov, &PLAYER);
        if(go == 2){
            printf("You have been falen\n");
            printf("\n--- GAME OVER ---\n");
            restart(maze, &playerPos);
        }
        if(go == 3){
            printf("You have slain Evilus Maximus\n");
            printf("The world is now free of this monster\n");
            printf("\n--- VICTORY ---\n");
            restart(maze, &playerPos);
        }
    }

    return 0;
}