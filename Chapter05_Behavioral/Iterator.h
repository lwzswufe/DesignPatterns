template<typename _Tp>
class Iterator
{
public:
    Iterator(_Tp* ptr) {_ptr = ptr;}
    // 使得迭代器指向下一元素
    _Tp Next() {_ptr++;}
    // 返回引用
    _Tp& operator*() {return *_ptr;}
    // 返回指针
    _Tp* operator->() {return _ptr;}
    // 迭代
    Iterator& operator++() {Next(); return *this;}
    // 逆向迭代
    Iterator& operator--() {Next(); return *this;}
    // 相等比较
    bool operator==(const Iterator<_Tp>& iter) {return _ptr == iter.base();} 
    // 不相等比较
    bool operator!=(const Iterator<_Tp>& iter) {return _ptr != iter.base();} 
    // 小于
    bool operator<(const Iterator<_Tp>& iter) {return _ptr > iter.base();} 
    // 大于
    bool operator>(const Iterator<_Tp>& iter) {return _ptr < iter.base();} 
    // 获取对象储存地址
    _Tp* base() const { return _ptr; }
private:
    // 对象储存地址
    _Tp* _ptr;    
};