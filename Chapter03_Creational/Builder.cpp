#include "Builder.h"

void MazeBuilder::BuildMaze()
{
    _maze = new Maze;
}

void MazeBuilder::BuildWall(Room* r1, Room* r2)
{
    Wall* wall = new Wall(r1, r2);
    // r1->SetSide(East, wall)
}

void MazeBuilder::BuildRoom(int roomNo)
{
    _maze->AddRoom(new Room(roomNo));
}

void MazeBuilder::BuildDoor(Room* r1, Room* r2)
{
    Door* door = new Door(r1, r2);
    // r1->SetSide(East, door)
}

Maze* MazeGame::CreateMaze (MazeBuilder& builder)
{
    builder.BuildMaze();
    builder.BuildRoom(1);
    builder.BuildRoom(2);

    return builder.GetMaze();
}