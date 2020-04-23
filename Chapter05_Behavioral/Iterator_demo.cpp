#include <stdio.h>
#include "List.h"


void print1(List<int> &list)
{   
    printf("size: %llu\n", list.size());
    for(int i=0; i<list.size(); ++i)
    {
        printf("%d, ", list[i]);
    }
    printf("\n");
}


void print2(List<int> &list)
{   
    printf("size: %llu\n", list.size());
    for(auto iter=list.begin(); iter!=list.end(); ++iter)
    {
        printf("%d, ", *iter);
    }
    printf("\n");
}

int main()
{
    List<int> list;
    for(int i=0; i<5; ++i)
    {
        list.push_back(i + 1);
    }
    print1(list);
    print2(list);
}