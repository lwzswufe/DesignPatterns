#include "Quote_api.h"
#include "Quote_spi.h"
#include "Quote_generator.h"

class Quote_Generator;
// 行情推送线程参数结构体
struct Quote_Push_Param_Struct
{
    bool  push_status;                      // 推送状态 true推送  false不推送
    double  clock;                          // HHMMSS.sss
	const char* ini_file;					// 配置文件
    QuoteSpi* spi;                          // 行情回调类
};
// Api构造函数
// extern QuoteApi* QuoteApi::CreateQuoteApi(const char* inifile);
// 推送行情线程
void *task_push_quote(void* args);
int GetTimenum();
extern int ApiTimenum;

class MyQuoteApi :public QuoteApi
{
public:
    MyQuoteApi(const char* ini_file);

    double GetClock();

	void RegisterSpi(QuoteSpi *spi);

	int Login();

	int Logout();

	int QueryAllTickers(EXCHANGE_TYPE exchange_id);

private:
	// 开启行情推送线程
	void start_push_quote_thread();
	// 关闭行情推送线程
	void close_push_quote_thread();
	// 行情回调类指针
	QuoteSpi* _spi;
	// 行情线程参数
	Quote_Push_Param_Struct quote_param;
	// 配置文件
	const char* ini_file;
	// 子线程启动状态
	bool quote_pthread_status;
};