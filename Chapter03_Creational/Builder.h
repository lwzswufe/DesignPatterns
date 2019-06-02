#include "MapSite.h"

class MazeBuilder
{
public: 
    MazeBuilder();

    virtual void BuildMaze();
    virtual void BuildRoom(int roomNo);
    virtual void BuildDoor(Room* r1, Room* r2);
    virtual void BuildWall(Room* r1, Room* r2);
    virtual Maze* GetMaze();
protected: 
    Maze* _maze;
};

class MazeGame
{
public:
    MazeGame();
    Maze* CreateMaze(MazeBuilder& builder);
};