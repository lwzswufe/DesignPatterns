#include <deque>
using std::deque;
#include <vector>
using std::vector;
#include "Data_Struct.h"


// 接口
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

// public方式继承接口 private方式继承接口实现
class buy_strategy :public strategy, private user_strategy
{
public: 
    buy_strategy();
    void update_quote(Quote quotation);
    void send_order(deque<Order>& order_deque);
};


