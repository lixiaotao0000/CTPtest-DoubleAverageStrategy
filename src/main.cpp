#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "CustomMdSpi.h"
#include "CustomTradeSpi.h"
#include "TickToKlineHelper.h"
#include "StrategyTrade.h"
#include <map>
#include <thread>

using namespace std;

// 链接库
#pragma comment (lib, "thostmduserapi.lib")
#pragma comment (lib, "thosttraderapi.lib")


// ---- 全局变量 ---- //
// 公共参数
TThostFtdcBrokerIDType gBrokerID = "9999";                         // 模拟经纪商代码
TThostFtdcInvestorIDType gInvesterID = "135267";                         // 投资者账户名
TThostFtdcPasswordType gInvesterPassword = "simnow314";                     // 投资者密码

// 行情参数
CThostFtdcMdApi *g_pMdUserApi = nullptr;                           // 行情指针
char gMdFrontAddr[] = "tcp://180.168.146.187:10031";               // 模拟行情前置地址
char *g_pInstrumentID[] = { "rb1910" };							   // 行情合约代码列表，中、上、大、郑交易所各选一种
int instrumentNum = 1;                                             // 行情合约订阅数量
unordered_map<string, TickToKlineHelper> g_KlineHash;              // 不同合约的k线存储表

// 交易参数
CThostFtdcTraderApi *g_pTradeUserApi = nullptr;                    // 交易指针
char gTradeFrontAddr[] = "tcp://180.168.146.187:10030";            // 模拟交易前置地址
TThostFtdcInstrumentIDType g_pTradeInstrumentID = "rb1910";        // 所交易的合约代码
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Sell;       // 买卖方向
TThostFtdcPriceType gLimitPrice = 22735;                           // 交易价格
CThostFtdcInvestorPositionField *g_InvestorPosition;				   // 标的持仓情况

CThostFtdcDepthMarketDataField *depthdata[200] = { 0 };			   //深度行情
CustomMdSpi *gMDSpi;											   // 行情接口实例	
CustomTradeSpi *pTradeSpi;										   // 交易实例
CThostFtdcMdSpi *pMdUserSpi;									   // 行情接口

// 登录
int OnFrontConnected() {
	CThostFtdcReqUserLoginField *reqUserLogin = new CThostFtdcReqUserLoginField();
	strcpy(reqUserLogin->BrokerID, gBrokerID);
	strcpy(reqUserLogin->UserID, gInvesterID);
	strcpy(reqUserLogin->Password, gInvesterPassword);

	return g_pTradeUserApi->ReqUserLogin(reqUserLogin, 0); // 登录
}


void  SetRejectdataTime(double  begintime1, double endtime1, double begintime2, double endtime2, double begintime3, double endtime3, double begintime4, double endtime4)
{
	if (begintime1 < 0 || endtime1 < 0 || begintime2 < 0 || endtime2 < 0 || begintime3 < 0 || endtime3 < 0 || begintime4 < 0 || endtime4 < 0)
	{
		printf("设置拒收行情的时间段必须大于等于0\n");
	}

	if (begintime1 != 100 && endtime1 != 100)
	{
		printf("[%0.06f ~ %0.06f]拒收数据的时间段\n", begintime1, endtime1);
		gMDSpi->begintime1 = begintime1;
		gMDSpi->endtime1 = endtime1;
	}

	if (begintime2 != 100 && endtime2 != 100)
	{
		printf("[%0.06f ~ %0.06f]拒收数据的时间段\n", begintime2, endtime2);
		gMDSpi->begintime2 = begintime2;
		gMDSpi->endtime2 = endtime2;
	}

	if (begintime3 != 100 && endtime3 != 100)
	{
		printf("[%0.06f ~ %0.06f]拒收数据的时间段\n", begintime3, endtime3);
		gMDSpi->begintime3 = begintime3;
		gMDSpi->endtime3 = endtime3;

	}

	if (begintime3 != 100 && endtime3 != 100)
	{
		printf("[%0.06f ~ %0.06f]拒收数据的时间段\n", begintime4, endtime4);
		gMDSpi->begintime4 = begintime4;
		gMDSpi->endtime4 = endtime4;
	}

}

double GetTimeSec(SYSTEMTIME sys_time)
{
	double system_times;
	system_times = (double)((sys_time.wHour) / 10e1) + (double)((sys_time.wMinute) / 10e3) + (double)((sys_time.wSecond) / 10e5);	//格式时间0.145100
	return system_times;
}

// 判断是否是交易时间
bool IsTradeTime() {
	SYSTEMTIME sys_time;
	//将变量值设置为本地时间
	GetLocalTime(&sys_time);

	SYSTEMTIME beginTime1 = { sys_time.wYear,sys_time.wMonth,sys_time.wDayOfWeek ,sys_time.wDay ,9 ,30 ,0,0 };
	SYSTEMTIME endTime1 = { sys_time.wYear,sys_time.wMonth,sys_time.wDayOfWeek ,sys_time.wDay ,11 ,30 ,0,0 };
	SYSTEMTIME beginTime2 = { sys_time.wYear,sys_time.wMonth,sys_time.wDayOfWeek ,sys_time.wDay ,13 ,0 ,0,0 };
	SYSTEMTIME endTime2 = { sys_time.wYear,sys_time.wMonth,sys_time.wDayOfWeek ,sys_time.wDay ,15 ,0 ,0,0 };

	double STime1 = GetTimeSec(beginTime1);
	double ETime1 = GetTimeSec(endTime1);
	double STime2 = GetTimeSec(beginTime2);
	double ETime2 = GetTimeSec(endTime2);

	double NowTime = GetTimeSec(sys_time);

	if (NowTime >= STime1&&NowTime <= ETime1) {
		return true;
	}

	if (NowTime >= STime2&&NowTime <= ETime2) {
		return true;
	}

	return false;
}

// 成交回调平仓
void CallBackCloseOrder(bool flag, int iOrderRef) 
{
	// 平仓
	int orderRef = pTradeSpi->InsertOrder(g_pInstrumentID[0], '0', '1', '2', depthdata[gMDSpi->gMarket[g_pInstrumentID[0]]]->LastPrice + 20, 1);
}

// 查看订单是否成交
bool isOrderSuccess(int orderRef) {
	bool flag = false;
	char *vorderRef = new char[13];
	_snprintf_s(vorderRef, sizeof(TThostFtdcOrderRefType), sizeof(TThostFtdcOrderRefType) - 1, "%012d", orderRef);
	if (pTradeSpi->tradeOrderRefs.find(vorderRef) != pTradeSpi->tradeOrderRefs.end()) {
		// 交易成交为1，失败0
		if (pTradeSpi->tradeOrderRefs[vorderRef] == 1) 
		{
			flag = true;
		}
	}

	return flag;
}


int main()
{
	// 账号密码
	//cout << "请输入账号： ";
	//scanf("%s", gInvesterID);
	//cout << "请输入密码： ";
	//scanf("%s", gInvesterPassword);

	// 初始化行情线程
	cout << "初始化行情..." << endl;
	g_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();   // 创建行情实例
	pMdUserSpi = new CustomMdSpi;       // 创建行情回调实例
	gMDSpi = new CustomMdSpi();			// 行情实现实例
	g_pMdUserApi->RegisterSpi(pMdUserSpi);               // 注册事件类
	g_pMdUserApi->RegisterFront(gMdFrontAddr);           // 设置行情前置地址
	g_pMdUserApi->Init();                                // 连接运行


	// 初始化交易线程
	cout << "初始化交易..." << endl;
	g_pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(); // 创建交易实例
	//CThostFtdcTraderSpi *pTradeSpi = new CustomTradeSpi;
	pTradeSpi = new CustomTradeSpi;               // 创建交易回调实例
	g_pTradeUserApi->RegisterSpi(pTradeSpi);                      // 注册事件类
	g_pTradeUserApi->SubscribePublicTopic(THOST_TERT_RESTART);    // 订阅公共流
	g_pTradeUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // 订阅私有流
	g_pTradeUserApi->RegisterFront(gTradeFrontAddr);              // 设置交易前置地址
	g_pTradeUserApi->Init();                                      // 连接运行
	pTradeSpi->OnFrontConnected();								  // 交易登录 


	// ******* 处理业务员 *********
	// 设置拒收时间
	SetRejectdataTime(0.0400, 0.0840, 0.1530, 0.2030, NULL, NULL, NULL, NULL);

	// 策略运行主体
	if (!IsTradeTime()) 
	{
		// 订阅行情
		g_pMdUserApi->SubscribeMarketData(&g_pInstrumentID[0], 1);	// rb1910

		// 等待行情预定
		Sleep(2000);
		DoubleAverageStrategy DAS;									// 生成策略实例
		while(1)
		{
			// 策略交易
			std::cout << "=====开始进入策略交易=====" << std::endl;
			cout<<"此时K线存储的数据个数为："<<g_KlineHash[g_pInstrumentID[0]].m_KLineDataArray.size()<<endl;
			//cout<<"此时K线存储是否为空:"<<g_KlineHash[g_pInstrumentID[0]].m_KLineDataArray.empty()<<endl;
		
			DAS.StrategyCheckAndTrade(&g_pTradeInstrumentID[0], pTradeSpi);
			Sleep(500);
		}
	}

	// 等到线程退出
	g_pMdUserApi->Join();
	delete pMdUserSpi;
	g_pMdUserApi->Release();

	g_pTradeUserApi->Join();
	delete pTradeSpi;
	g_pTradeUserApi->Release();

	getchar();
	return 0;
}