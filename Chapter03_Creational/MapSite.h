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
// 房间类
class Room: public MapSite
{
public:
    Room(int roomNo);
    Room* Clone(int roomNo);                    // 仅针对Prototype
    MapSite* GetSide(Direction) const;
    void SetSide(Direction, MapSite*);
    virtual void Enter();
private:
    MapSite* _sides[SIDES];
    int _roomNumber;
};
// 墙类
class Wall: public MapSite
{
public: 
    Wall(Room* = NULL, Room* = NULL);
    Wall* Clone(Room* r1, Room* r2);            // 仅针对Prototype
    virtual void Enter();
protected:
    // 房间1
    Room* _room1;
    // 房间2
    Room* _room2;
};
// 有门的墙类
class Door: public Wall
{
public: 
    Door(Room* = NULL, Room* = NULL);
    Door* Clone(Room* r1, Room* r2);            // 仅针对Prototype
    virtual void Enter();
    // 开门
    void open();
    // 关门
    void close();
private:
    // 门是否打开
    bool _isOpen;
};
// 迷宫类
class Maze
{
public:
    Maze();
    Maze* Clone();
    void AddRoom(Room*);
    // 根据房间编号查找房间
    Room* RoomNo(int roomNo) const;
private: 

};
// 可爆炸的房间
class BombedRoom: public Room
{
public: 
    void bomb();
    bool GetIsBombed() const;
private:
    bool is_bombed;
};
// 可爆炸房间的门
class BombedDoor: public Room
{
public:
    void bomb();
    bool GetIsBombed() const;
private:
    bool is_bombed;
};

