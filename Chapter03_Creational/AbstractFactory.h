#include "MapSite.h"

class MazeFactory
{
public: 
    MazeFactory();
    virtual Maze* MakeMaze() const;
    virtual Wall* MakeWall(Room* r1, Room* r2) const;
    virtual Room* MakeRoom(int roomNo) const;
    virtual Door* MakeDoor(Room* r1, Room* r2) const;
};

class BombedMazeFactory: public MazeFactory
{
public:
    BombedMazeFactory();
    virtual Room* MakeBombedRoom(int roomNo) const;
    virtual Door* MakeBombedDoor(Room* r1, Room* r2) const;
};

class MazeGame
{
public:
    MazeGame();
    Maze* CreateMaze(MazeFactory& factory);
};
