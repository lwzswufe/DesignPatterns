#include <iostream>
using std::cout; using std::endl;
#include <vector>
using std::vector;
#include <time.h> 

class Observer;

class Subject
{
public:
    // 增加观察者
    virtual void Attach(Observer*);  
    // 删除观察者   
    virtual void Detach(Observer*);
    // 通知     
    virtual void Notify();     
    // 获取数据
    virtual int GetData();     
    // 是否更新
    virtual bool IsUpdate();    
private:
    vector<Observer*> _observers;
    int _data;
};

class Observer
{
public: 
    // 更新
    virtual void update(Subject* theChangeSubject); 
};