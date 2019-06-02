#include "MapSite.h"
#include "AbstractFactory.h"


class MazsPrototypeFactory: public MazeFactory 
{
public: 
    MazsPrototypeFactory();
    
    virtual void set_prototype(Maze*, Wall*, Room*, Door*);
    virtual Maze* MakeMaze() const;
    virtual Wall* MakeWall(Room* r1, Room* r2) const;
    virtual Room* MakeRoom(int roomNo) const;
    virtual Door* MakeDoor(Room* r1, Room* r2) const;
private: 
    Maze* _prototypeMaze;
    Wall* _prototypeWall;
    Room* _prototypeRoom;
    Door* _prototypeDoor;
};
