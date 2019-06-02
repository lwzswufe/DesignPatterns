#include "FactoryMethod.h"

int main()
{
    MazeGame mazegame;
    Maze* maze = mazegame.CreateMaze();     // 在这里定义迷宫内容
}