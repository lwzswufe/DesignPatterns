#include <deque>
using std::deque;
#include <vector>
using std::vector;
#include "Data_Struct.h"


// 定义接口
class strategy
{
public:
    void virtual update_quote(Quote quotation) = 0;
    void virtual send_order(deque<Order>& order_deque) = 0;
};

// 策略实现
class user_strategy
{
public:     // 在此处定义 其子类才可以访问
    void strategy_1(Quote quotation);
    vector<Order> order_vec;
private:   //  在此处定义 其子类不可以访问
};

// public方式继承接口 维护一个指向 策略实现的指针
class buy_strategy :public strategy
{
public: 
    buy_strategy(user_strategy* _strategy);
    void update_quote(Quote quotation);
    void send_order(deque<Order>& order_deque);
private:
    user_strategy* _strategy;
};


