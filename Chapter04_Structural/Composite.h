
struct Position                             // 坐标
{
    int x;
    int y;
};

class Unit                                  // 军事单位
{
public:

    virtual void Attack(Unit* enemy);       // 攻击
    virtual void Defence(Unit* enemy);      // 防御
    virtual void Move(Position* dest);      // 移动
    virtual void Consume();                 // 消耗补给
    Unit* next;                             // 同组织的下一个单位
    int quantity;                           // 人数
    int attack;                             // 攻击力
    int defense;                            // 防御力
    int movement;                           // 机动性
    int range;                              // 射程
    int provisions;                         // 给养
    int consume;                            // 消耗
};

class Infantry: public Unit                 // 步兵
{
    void Attack(Unit* enemy);               // 攻击
    void Defence(Unit* enemy);              // 防御
    void Move(Position* dest);              // 移动
    void Consume();                         // 消耗补给
};

class Cavalry: public Unit                  // 骑兵
{
public:
    void Attack(Unit* enemy);               // 攻击
    void Defence(Unit* enemy);              // 防御
    void Move(Position* dest);              // 移动
    void Consume();                         // 消耗补给
};

class Division                              // 师
{
public:
    void add_unit(Unit* unit);              // 增加单位
    void remove_unit(Unit* unit);           // 去掉单位
    Unit* Get_Unit();                       // 获取单位
    Unit* unit;                             // 第一个单位
};