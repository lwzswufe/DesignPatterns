class Base
{
public:
    Base();
    virtual void Do();
};

class Decorator: public Base
{
public: 
    Decorator(Base* base);
    virtual void Do();
private:
    Base* base;
};


class BorderDecorator: public Decorator
{
public: 
    BorderDecorator(Base* base);
    virtual void Do();
private:
    Base* base;
};


class ScrollDecorator: public Decorator
{
public: 
    ScrollDecorator(Base* base);
    virtual void Do();
private:
    Base* base;
};