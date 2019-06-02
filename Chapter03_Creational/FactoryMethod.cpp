#include "FactoryMethod.h"

Maze* MazeGame::CreateMaze()
{
    Maze* maze = MakeMaze();
    Room* r1 = MakeRoom(1);
    Room* r2 = MakeRoom(2);
    Door* door = MakeDoor(r1, r2);

    maze->AddRoom(r1);
    maze->AddRoom(r2);

    r1->SetSide(East, door);
    r1->SetSide(South, MakeWall(r1, NULL));
    r1->SetSide(West, MakeWall(r1, NULL));
    r1->SetSide(North, MakeWall(r1, NULL));

    r2->SetSide(West, MakeWall(r2, NULL));
    r2->SetSide(South, MakeWall(r2, NULL));
    r2->SetSide(East, door);
    r2->SetSide(North, MakeWall(r2, NULL));
}

Maze* MazeGame::MakeMaze() const
{
    return new Maze;
}

Room* MazeGame::MakeRoom(int roomNo) const
{
    return new Room(roomNo);
}

Wall* MazeGame::MakeWall(Room* r1, Room* r2) const
{
    return new Wall(r1, r2);
}

Door* MazeGame::MakeDoor(Room* r1, Room* r2) const
{
    return new Door(r1, r2);
}

