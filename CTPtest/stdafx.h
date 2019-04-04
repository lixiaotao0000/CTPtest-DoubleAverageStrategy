/*
1.本文件为Quicklib 期货行情库底层代码
2.本文件及调用的库文件Quicklib CTP期货行情库和交易库遵循 开源协议GPL v3
简单的说：对基于GPL v3协议的源代码，若个人或机构仅仅是自己使用，则可以闭源。
若基于该开源代码，开发出程序或衍生产品用于商业行为则也必须开源。

Quciklib Python框架和工具遵循GPL v3协议包括：
（1）Quicklib CTP   期货行情库
（2）Quicklib CTP   期货交易库
（3）Quicklib CTP2  A股行情库
（4）Quicklib MOM模式  博易资管交易库
（用于接入资管投顾系统，MOM模式可实现私募进行投顾的选拔考核，并通过自己的风控系统接入实盘）

Quciklib Python框架和工具暂不遵循开源协议包括：
（5）Quicklib 监控器库（预警、监控、交易信号数据复制、跟单）（可免费试用）

对Quciklib开源库做出贡献的，并得到原始作者肯定的，将公布在http://www.quciklib.cn网站上，
并添加在《开源说明和感谢.txt》，并将该文件不断更新放入每一个新版本的Quicklib库里。

原始作者：QQ 147423661 林(王登高)
官方网站：http://www.quicklib.cn
官方QQ群：5172183(1群)、25884087(2群)
*/
#pragma once

#undef UNICODE
#undef _UNICODE

#define WM_STRATEGY WM_USER+1 

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//#pragma comment(lib,"ThostTraderApi32位\\thostmduserapi.lib")
//#pragma comment(lib,"ThostTraderApi64位\\thostmduserapi.lib")
//#ifndef _WIN64  
//  #include "ThostTraderApi64位/ThostFtdcUserApiDataType.h"
//  #include "ThostTraderApi64位/ThostFtdcUserApiStruct.h"
//  #include "ThostTraderApi64位/ThostFtdcMdApi.h"
//#else  
//  #include "ThostTraderApi32位/ThostFtdcUserApiDataType.h"
//  #include "ThostTraderApi32位/ThostFtdcUserApiStruct.h"
//  #include "ThostTraderApi32位/ThostFtdcMdApi.h"
//#endif  



#include <string>
#include <fstream>
#include <iostream>
#include <map>
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <hash_map>
#define PYCTPMARKET_EXPORTS

#include <list>
struct cmdcontent
{
	int cmd;
	char content[31];
};
#define MAX_EVENTNUM  64

#define  EID_OnFrontDisconnected   0
#define  EID_OnFrontConnected      1
#define  EID_OnRspUserLogin_Failer 2
#define  EID_OnRspUserLogin_Scuess 3
#define  EID_OnRtnDepthMarketData  4
#define  EID_IsErrorRspInfo        5
#define  EID_OnRspSubMarketData    6
#define  EID_OnRspUnSubMarketData  7
#define  EID_OnRspUserLogout       8
#define  EID_OnRspForQuote         9
//#define  EID_OnLog                 10

//回调类型
#define SYSTEM_EMPTY                  8000 //无
#define SYSTEM_LOGIN_SCUESS           8001 //登录成功
#define SYSTEM_LOGIN_DENIED           8002 //登录被拒绝
//#define SYSTEM_LOGIN_ERRORPASSWORD  8003 //密码错误 ??
#define SYSTEM_LOGINOUT_SCUESS        8004 //登出成功
#define SYSTEM_NETCONNECT_SCUESS      8005 //连接成功
#define SYSTEM_NETCONNECT_BREAK       8006 //断开连接
#define SYSTEM_NETCONNECT_FAILER      8007 //连接失败 ??
#define SYSTEM_SUBCRIBE_SCUESS        8008 //订阅成功
#define SYSTEM_UNSUBCRIBE_SCUESS      8009 //取消订阅成功
#define SYSTEM_NEWTICK                8010 //新Tick到来
#define SYSTEM_SYSTEM_ERROR           8011 //错误应答
#define SYSTEM_QRY_FORQUOTE           8012 //
//#define SYSTEM_LOG                    8013 //日志
struct TThostFtdcInstrumentIDTypeStruct
{
	//TThostFtdcInstrumentIDType Instrument;
};