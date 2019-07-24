#include "Quote_api.h"
#include "Quote_spi.h"
#include "Quote_generator.h"

class MyQuoteApi :public QuoteApi
{
public:
    MyQuoteApi(const char* inifile);

	void Release();

    int GetClock();

	ErrorInfo *GetApiLastError();

	void SetUDPBufferSize(uint32_t buff_size);

	void RegisterSpi(QuoteSpi *spi);

	void SetHeartBeatInterval(uint32_t interval);

	int Login();

	int Logout();

	int QueryAllTickers(EXCHANGE_TYPE exchange_id);

private:
	// 开启行情推送线程
	void start_push_quote_thread();
	// 关闭行情推送线程
	void close_push_quote_thread();

	QuoteSpi* _spi;

    int clock;

};

class Quote_Generator;
// 行情推送线程参数结构体
struct Quote_Push_CreateQuoteApiParam_Struct
{
    bool push_status;                       // 推送状态 true推送  false不推送
    int clock;                              // 
    QuoteSpi* spi;                          // 行情回调类
	Quote_Generator* generator;             // 行情生成类
};
// Api构造函数
extern QuoteApi* QuoteApi::CreateQuoteApi(const char* inifile);
// 推送行情线程
void *task_push_quote(void* args);