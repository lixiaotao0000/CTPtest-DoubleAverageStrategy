#pragma once

#ifdef PYCTPTRADER_EXPORTS
#	define QUICKLIB_TD_API __declspec(dllexport)
#else
#	define CTP_TD_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	//xx	
	int CTP_TD_API QryQueryMaxOrderVolume(char *BrokerID, char * InvestorID, char * Instrument, char * Direction, char * OffsetFlag, char * HedgeFlag, int MaxVolume);

	int CTP_TD_API Login();

	int CTP_TD_API InsertOrderByRate(char *Instrument, char direction, char offsetFlag, char priceType, double price, double rate, bool BalanceType, int multiplier);
	int CTP_TD_API InsertOrder(char *Instrument, char direction, char offsetFlag, char priceType, double price, int num);
	int CTP_TD_API DeleteOrder(char *Instrument, int OrderRef);
	int CTP_TD_API QryTradedVol(int OrderRef);
	int CTP_TD_API QryPosition(char *Instrument, int positiontype);

	int CTP_TD_API ReqQueryMaxOrderVolume(char * BrokerID, char * InvestorID, char * InstrumentID, char Direction, char  OffsetFlag, char  HedgeFlag, int MaxVolume);

	int  CTP_TD_API ReqQryContractBank();
	double  CTP_TD_API QryExchangeMarginRate(char *Instrument, int positiontype);    //保证金率
	double  CTP_TD_API QryUnderlyingMultiple(char *Instrument);    //乘数

																		///期货发起银行资金转期货请求
																		//int  QUICKLIB_TD_API  ReqFromBankToFutureByFuture(int nRequestID);  //ADD

	int  CTP_TD_API  ReqFromBankToFutureByFuture(
		char * BankID,
		//char * BankBranchID,
		//char * BrokerID,
		char *  BrokerBranchID,
		char *BankAccount,
		char * BankPassWord,
		char * AccountID,
		//char *Password,
		//char * CurrencyID,
		double  TradeAmount,
		int nRequestID);

	///期货发起期货资金转银行请求
	int  CTP_TD_API  ReqFromFutureToBankByFuture(
		char * BankID,
		//char * BankBranchID,
		//char * BrokerID,
		char *  BrokerBranchID,
		char *BankAccount,
		char * BankPassWord,
		char * AccountID,
		//char *Password,
		//char * CurrencyID,
		double  TradeAmount,
		int nRequestID);  //ADD



	int CTP_TD_API ReqQryInstrument();



	void CTP_TD_API *QryPositionList(int i);
	double CTP_TD_API QryBalance(bool BalanceType);     //1静态权益 0动态权益 自己增加
	double CTP_TD_API QryAvailable();   //可用资金
	void CTP_TD_API SetShowPosition(bool showstate);   //可用资金

	int CTP_TD_API GetCmd();
	CTP_TD_API char * GetCmdContent2();

	void CTP_TD_API * GetCmdContent();

	void CTP_TD_API * GetCmdContent_Order();
	void CTP_TD_API * GetCmdContent_Trade();


	void CTP_TD_API * GetCmdContent_BankToFutureByFuture();
	void CTP_TD_API * GetCmdContent_FutureToBankByFuture();

	void CTP_TD_API * GetCmdContent_QueryMaxOrderVolume();


	void CTP_TD_API * GetCmdContent_Settlement();
	void CTP_TD_API * GetCmdContent_Error();

	void CTP_TD_API * GetCmdContent_LoginScuess();
	void CTP_TD_API * GetCmdContent_Connected();
	void CTP_TD_API * GetCmdContent_ProductGroupMargin();
	void CTP_TD_API * GetCmdContent_CommissionRate();


	void CTP_TD_API * GetCmdContent_AccountRegist();

	void CTP_TD_API * GetCmdContent_BankToFutureByFuture();
	void CTP_TD_API * GetCmdContent_FutureToBankByFuture();



	int CTP_TD_API OnCmd();
	int CTP_TD_API GetUnGetCmdSize();

	//double QUICKLIB_TD_API QryAvailable2();   //可用资金
#ifdef __cplusplus
}
#endif