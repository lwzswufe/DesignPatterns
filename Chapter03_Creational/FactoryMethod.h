#include "MapSite.h"

class MazeGame
{
public:
    MazeGame();
    Maze* CreateMaze();
    virtual Maze* MakeMaze() const;
    virtual Room* MakeRoom(int roomNo) const;
    virtual Wall* MakeWall(Room* r1, Room* r2) const;
    virtual Door* MakeDoor(Room* r1, Room* r2) const;
};
