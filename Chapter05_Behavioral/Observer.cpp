#include "Observer.h"

int GetTime()
{
    time_t timer;
    struct tm * timeinfo;
    double seconds;
    time(&timer);
    timeinfo = localtime(&timer);  
    int now = timeinfo->tm_hour * 10000 + 
              timeinfo->tm_min * 100 + 
              timeinfo->tm_sec;
    return now;
};

void Subject::Attach(Observer* ob)
{
    this->_observers.push_back(ob);
}

void Subject::Detach(Observer* ob)
{   
    for (auto iter=this->_observers.begin(); iter<this->_observers.end(); iter++)
    {
        if (*iter == ob)
            this->_observers.erase(iter);
    }
}

void Subject::Notify()
{
    for (Observer* ob: this->_observers)
    {
        ob->update(this);
    }
}

int Subject::GetData()
{
    return this->_data;
}

bool Subject::IsUpdate()
{   
    int NewData = GetTime();
    if (NewData > this->_data)
    {
        this->_data = NewData;
        return true;
    }
    else 
        return false;
}

void Observer::update(Subject* theChangeSubject)
{
    cout << "GetTime: " << theChangeSubject->GetData() << endl;
}