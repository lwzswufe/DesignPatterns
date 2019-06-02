#include "AbstractFactory.h"

Maze* MazeFactory::MakeMaze() const
{
    return new Maze;
}

Wall* MazeFactory::MakeWall(Room* r1, Room* r2) const
{
    return new Wall(r1, r2);
}

Room* MazeFactory::MakeRoom(int roomNo) const
{
    return new Room(roomNo);
}

Door* MazeFactory::MakeDoor(Room* r1, Room* r2) const
{
    return new Door(r1, r2);
}

Maze* MazeGame::CreateMaze (MazeFactory& factory)
{
    Maze* maze = factory.MakeMaze();
    Room* r1 = factory.MakeRoom(1);
    Room* r2 = factory.MakeRoom(2);
    Door* door = factory.MakeDoor(r1, r2);

    maze->AddRoom(r1);
    maze->AddRoom(r2);

    r1->SetSide(East, door);
    r1->SetSide(South, factory.MakeWall(r1, NULL));
    r1->SetSide(West, factory.MakeWall(r1, NULL));
    r1->SetSide(North, factory.MakeWall(r1, NULL));

    r2->SetSide(West, factory.MakeWall(r2, NULL));
    r2->SetSide(South, factory.MakeWall(r2, NULL));
    r2->SetSide(East, door);
    r2->SetSide(North, factory.MakeWall(r2, NULL));
}