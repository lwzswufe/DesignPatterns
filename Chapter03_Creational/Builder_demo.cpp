#include "Builder.h"

int main()
{
    MazeBuilder builder;
    MazeGame mazegame;
    Maze* maze = mazegame.CreateMaze(builder);     // 在这里定义迷宫内容
}
