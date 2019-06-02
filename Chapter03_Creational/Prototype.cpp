#include "Prototype.h"

MazsPrototypeFactory::MazsPrototypeFactory()
{
}

void MazsPrototypeFactory::set_prototype(Maze* maze, Wall* wall, Room* room, Door* door)
{
    _prototypeMaze = maze;
    _prototypeWall = wall;
    _prototypeRoom = room;
    _prototypeDoor = door;
}

Maze* MazsPrototypeFactory::MakeMaze() const
{
    return this->_prototypeMaze->Clone();
}

Wall* MazsPrototypeFactory::MakeWall(Room* r1, Room* r2) const
{
    return this->_prototypeWall->Clone(r1, r2);
}

Room* MazsPrototypeFactory::MakeRoom(int roomNo) const
{
    return this->_prototypeRoom->Clone(roomNo);
}

Door* MazsPrototypeFactory::MakeDoor(Room* r1, Room* r2) const
{
    return this->_prototypeDoor->Clone(r1, r2);
}