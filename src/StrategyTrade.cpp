#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "StrategyTrade.h"
#include "CustomTradeSpi.h"

extern std::unordered_map<std::string, TickToKlineHelper> g_KlineHash;
extern CThostFtdcInvestorPositionField *g_InvestorPosition;	  // 标的持仓情况

// 线程互斥量
std::mutex marketDataMutex;
using namespace std;

void DoubleAverageStrategy::StrategyCheckAndTrade(TThostFtdcInstrumentIDType instrumentID, CustomTradeSpi *customTradeSpi)
{
	// 加锁
	std::lock_guard<std::mutex> lk(marketDataMutex);
	TickToKlineHelper tickToKlineObject = g_KlineHash.at(std::string(instrumentID));
	// 策略
	std::vector<KLineDataType> KLineVec = tickToKlineObject.m_KLineDataArray;

	int shortPeriod = 5;
	int longPeriod = 20;
	if (KLineVec.size() >= (longPeriod + 1))									// 如果当天采集的均线不足20min，则不进行判断。后期如有数据库维护，链接历史数据。
	{
		int len = KLineVec.size();
		double last_5min_averageprice = 0;										// 用于存放上时刻点5min均线
		double now_5min_averageprice =	0;										// 用于存放本时刻点的5min均线
		int i = 0;
		while (i < shortPeriod)
		{
			last_5min_averageprice += KLineVec[len-i-2].close_price/shortPeriod;
			now_5min_averageprice += KLineVec[len-i-1].close_price/shortPeriod;
			i++;
		};

		double last_20min_averageprice = 0;						// 用于存放20min均线数据
		double now_20min_averageprice = 0;
		int j = 0;
		while (j < longPeriod)
		{
			last_20min_averageprice += KLineVec[len-i-2].close_price/longPeriod;
			now_20min_averageprice += KLineVec[len-i-1].close_price/longPeriod;
			j++;
		};

		
		bool crossover = (last_5min_averageprice < last_20min_averageprice) && (now_5min_averageprice > now_20min_averageprice);	// 金叉：5min均线上传20min均线														
		bool crossbelow = (last_5min_averageprice > last_20min_averageprice) && (now_5min_averageprice < now_20min_averageprice);	// 死叉：5min均线下传20min均线

		cout<<"当前信号触发情况"<<'\t'<<"->"<<'\t'<<"crossover:"<<crossover<<'\t'<<"crossbelow:"<<crossbelow<<endl;
		if (crossover)
		{
			if (NULL == g_InvestorPosition)											// 如果是今仓为空，则直接做多
			{
				cout<<"出现金叉，空仓直接做多"<<endl;
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price + 20, 1, THOST_FTDC_D_Buy);
			}
			else if ((*g_InvestorPosition).PosiDirection == THOST_FTDC_PD_Short)				// 如果今仓存在空头，则先平今仓，然后做多。
			{
				cout<<"出现金叉，存在空头，先平仓再做多"<<endl;
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price + 20, 1, THOST_FTDC_D_Sell);
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price + 20, 1, THOST_FTDC_D_Buy);
			}
		}
		else if (crossbelow)
		{
			
			if (NULL == g_InvestorPosition)											// 如果是今仓为空，则直接做空
			{
				cout<<"出现死叉，空仓直接做空"<<endl;
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price - 20, 1, THOST_FTDC_D_Buy);
			}
			else if ((*g_InvestorPosition).PosiDirection == THOST_FTDC_PD_Long)				// 如果今仓存在多头，则先平今仓，然后做多。
			{
				cout<<"出现金叉，存在多头，先平仓再做空"<<endl;
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price - 20, 1, THOST_FTDC_D_Sell);
				customTradeSpi->reqOrderInsert(instrumentID, KLineVec[len - 1].close_price - 20, 1, THOST_FTDC_D_Buy);
			}
		}
	}
}
