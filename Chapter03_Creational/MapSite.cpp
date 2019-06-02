 #include "MapSite.h"
 
MapSite* POSITION = NULL;

Room::Room(int roomNo)
{
    this->_roomNumber = roomNo;
    for(int i=0; i<SIDES; i++)
    {
        this->_sides[i] = NULL;
    }
}

MapSite* Room::GetSide(Direction d) const
{
    return this->_sides[d];
}

void Room::SetSide(Direction d, MapSite* m)
{
    this->_sides[d] = m;
}

void Room::Enter()
{
    POSITION = this;
}

Wall::Wall(Room* room1, Room* room2)
{
    this->_room1 = room1;
    this->_room2 = room2;
}

void Wall::Enter()
{
    return;
}

Door::Door(Room* room1, Room* room2)
{   
    this->_room1 = room1;
    this->_room2 = room2;
    this->_isOpen = false;
}

void Door::open()
{
    this->_isOpen = true;
}

void Door::close()
{
    this->_isOpen = false;
}

void Door::Enter()
{
    // if (this->_isOpen)
    //     POSITION = this;
}