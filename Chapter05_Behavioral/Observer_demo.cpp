#include "Observer.h"

/*
g++ Observer.h Observer.cpp Observer_demo.cpp -o Observe.exe
*/

int main()
{
    Observer ob_1;
    Subject clocker;
    clocker.Attach(&ob_1);
    int update_time = 0;
    while (update_time < 10)
    {   
        if(clocker.IsUpdate())
        {   
            clocker.Notify();
            update_time++;
        }
    }
    return 0;
}