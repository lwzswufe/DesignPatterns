#include <stdio.h>
#include "MapSite.h"

class MazeFactory
{
public: 
    MazeFactory();
    virtual Maze* MakeMaze() const;
    virtual Wall* MakeWall() const;
    virtual Room* MakeRoom(int roomNo) const;
    virtual Door* MakeDoor(Room* r1, Room* r2) const;
};

class BombedRoom: public Room
{
public: 
    void bomb();
    bool GetIsBombed() const;
private:
    bool is_bombed;
};

class BombedDoor: public Room
{
public:
    void bomb();
    bool GetIsBombed() const;
private:
    bool is_bombed;
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
