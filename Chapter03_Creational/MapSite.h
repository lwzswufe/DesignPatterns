#include <stdio.h>
#define SIDES 4
/*
定义构造迷宫所需要的各种构件
*/
// 方向
enum Direction { East  = 0, 
                 South = 1,
                 West  = 2, 
                 North = 4};
// 迷宫构件
class MapSite
{
public:
    // 进入房间/门/墙壁
    virtual void Enter() = 0;
};
// 当前位置
extern MapSite* POSITION;
// 房间类
class Room: public MapSite
{
public:
    Room(int roomNo);
    MapSite* GetSide(Direction) const;
    void SetSide(Direction, MapSite*);
    virtual void Enter();
private:
    MapSite* _sides[4];
    int _roomNumber;
};
// 墙类
class Wall: public MapSite
{
public: 
    Wall();
    virtual void Enter();
};
// 门类
class Door: public MapSite
{
public: 
    Door(Room* = NULL, Room* = NULL);
    virtual void Enter();
    Room* OtherSideFrom(Room*);
private:
    Room* _room1;
    Room* _room2;
    bool _isOpen;
};
// 迷宫类
class Maze
{
public:
    Maze();
    void AddRoom(Room*);
    // 根据房间编号查找房间
    Room* RoomNo(int roomNo) const;
private: 

};
