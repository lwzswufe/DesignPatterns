#include <memory>
#include <stdio.h>
#include <string.h>
#include "Iterator.h"

template<typename _Tp>
class List
{
public:
    // 请为Queue分配2^N的大小
    List();
    // 请为Queue分配2^N的大小
    List(const size_t size);
    // 析构函数
    ~List();
    // 随机访问
    _Tp& operator[](const size_t size);
    // 写入数据
    void push_back(_Tp);
    // 从尾部删除数据
    void pop_back(); 
    // 是否为空
    bool empty();
    // 获取大小
    size_t size();
    // 起始元素
    Iterator<_Tp> begin();
    // 尾元素的后面一个位置
    Iterator<_Tp> end();
    // 返回一个元素
    _Tp& data();
    // 返回首元素
    _Tp& front();
    // 返回尾元素
    _Tp& back();
private:
    // 申请空间
    void _alloc(const size_t idx);
    // 回收空间
    void _dealloc(_Tp* arr);
    // 第一条数据位置
    _Tp* _M_start;
    // 尾元素位置
    _Tp* _M_end;
    // 储存空间尾部
    _Tp* _M_stroage_end;
    // 容量
    size_t capacity;
};

template<typename _Tp>
List<_Tp>::List()
{
    _M_start = nullptr;
    _M_end = nullptr;
    _M_stroage_end = nullptr;
    capacity = 0;
}

template<typename _Tp>
List<_Tp>::List(size_t _size)
{
    _alloc(_size);
}

template<typename _Tp>
List<_Tp>::~List()
{
    _dealloc(_M_start);
    _M_start = nullptr;
    _M_end = nullptr;
    _M_stroage_end = nullptr;
    capacity = 0;
}

// 申请空间方法
template<typename _Tp>
void List<_Tp>::_alloc(const size_t _size)
{
    _M_start = (_Tp*) malloc(sizeof(_Tp) * _size);
    _M_end = _M_start;
    _M_stroage_end = _M_start + _size;
    capacity = _size;
}

// 释放方法
template<typename _Tp>
void List<_Tp>::_dealloc(_Tp* arr)
{
    if (arr != nullptr)
        delete[] arr;
}

// 获取储存元素数目
template<typename _Tp>
size_t List<_Tp>::size()
{   
    return _M_end - _M_start;
}

template<typename _Tp>
bool List<_Tp>::empty()
{   
    return _M_start == _M_end;
}

// 写入数据value
template<typename _Tp>
void List<_Tp>::push_back(_Tp value)
{   
    if (size() >= capacity)
    {   
        if (capacity == 0)
            _alloc(1);
        else
        {   
            _Tp* arr = _M_start;
            size_t _size = size();
            _alloc(capacity * 2);
            memcpy(_M_start, arr, sizeof(_Tp) * _size);
            _M_end = _M_start + _size;
            _dealloc(arr);
        }
    }
    *(_M_end) = value;
    _M_end++;;
}

// 读取数据
template<typename _Tp>
void List<_Tp>::pop_back()
{
    if (empty())
    {
        return;
    }
    else
    {
        _M_end--;
    }
}

template<typename _Tp>
_Tp& List<_Tp>::operator[](const size_t idx)
{
    return *(_M_start + idx);
}

template<typename _Tp>
Iterator<_Tp> List<_Tp>::begin()
{   
    Iterator<_Tp> iter(_M_start);
    return iter;
}

template<typename _Tp>
Iterator<_Tp> List<_Tp>::end()
{   
    Iterator<_Tp> iter(_M_end);
    return iter;
}

template<typename _Tp>
_Tp& List<_Tp>::data()
{
    return *_M_start;
}

template<typename _Tp>
_Tp& List<_Tp>::front()
{
    return *_M_start;
}

template<typename _Tp>
_Tp& List<_Tp>::back()
{
    return *(_M_end - 1);
}