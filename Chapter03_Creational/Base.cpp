#include "Base.h"

Maze* MazeGame::CreateMaze()
{
    Maze* maze= new Maze;
    Room* r1 = new Room(1);
    Room* r2 = new Room(2);
    Door* theDoor = new Door(r1, r2);

    maze->AddRoom(r1);
    maze->AddRoom(r2);

    r1->SetSide(North, new Wall(r1, NULL));
    r1->SetSide(East, theDoor);
    r1->SetSide(South, new Wall(r1, NULL));
    r1->SetSide(West, new Wall(r1, NULL));

    r2->SetSide(North, new Wall(r2, NULL));
    r2->SetSide(East, new Wall(r2, NULL));
    r2->SetSide(South, new Wall(r2, NULL));
    r2->SetSide(West, theDoor);
    return maze;
}
