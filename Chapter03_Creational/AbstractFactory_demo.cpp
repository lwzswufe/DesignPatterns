#include "AbstractFactory.h"

int main()
{
    MazeFactory factory;
    MazeGame mazegame;
    Maze* maze = mazegame.CreateMaze(factory);     // 在这里定义迷宫内容
}
