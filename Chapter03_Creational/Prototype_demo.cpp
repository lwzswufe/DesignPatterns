#include "Prototype.h"
#include "AbstractFactory.h"

int main()
{   
    Maze* maze = new Maze;
    Wall* wall = new Wall(NULL, NULL); 
    Room* room = new Room(0);
    Door* door = new Door(NULL, NULL);
    MazsPrototypeFactory factory;
    factory.set_prototype(maze, wall, room, door); //设置原型
    MazeGame mazegame;
    Maze* maze = mazegame.CreateMaze(factory);     // 在这里定义迷宫内容
}