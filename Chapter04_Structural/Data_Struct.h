#define CODESIZE 7

struct Quote
{   
    int trade_pr;
    int settlement;
    char code[CODESIZE];
};


struct Order
{
    int price;
    int vol;
    char code[CODESIZE];
};
