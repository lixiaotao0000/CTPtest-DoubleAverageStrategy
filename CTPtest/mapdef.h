
#pragma once

#include <iostream>
using  namespace std;
#include <string.h>
#define NULL 0
#include <math.h>
#include "def.h"

#include "CRC32.h"

struct strategys
{
	char instructment[20];
	int type;
	int strategy[100];
};
extern strategys * its;

struct Instructment
{

	char instructment[20];
	int type;
	int MD_begin1;
	int MD_end1;
	int MD_begin2;
	int MD_end2;
	int MD_begin3;
	int MD_end3;
	int MD_begin4;
	int MD_end4;
	int MD_begin5;
	int MD_end5;

	int TD_begin1;
	int TD_end1;
	int TD_begin2;
	int TD_end2;
	int TD_begin3;
	int TD_end3;
	int TD_begin4;
	int TD_end4;
	int TD_begin5;
	int TD_end5;

};

extern Instructment * it;
//Instructment * it;
//bool crcstate = false; //true采用crc32,fasle不采用
/*
//CRC32
GetCrc32Value(keystr, keystr, 10);
printf("CRC32:[%s]\n", keystr);
*/

//不采用false  41 36 50 54 48 62  (47/47/16/31/47/31毫秒)
//crc32 true   41 42 44 45 51 62  (47/47/47/47/47/47毫秒)
struct hashinfo
{
	double usedrate;
	double max_len;
	double ave_len;
	int maxlen_num;
};

//排序用保存HASH的结构体
struct HashData
{
	int cycle;
	int key;
	int M;
	int N;
	int sort;
	int emaspace;
};


//Instructment * it2=NULL;

/*
int cmp2(const void *a, const void *b)
{
return (*(HashData *)a)->sort > (*(HashData *)b)->sort ? 1 : -1;
}


struct Sort_Struct2
{
int id;
int value;
}SortPoint2[TYPE_NUM], SortSpace2[TYPE_NUM];
*/



//char Crcresult[9] = { 0 };
//sprintf_s(Crcresult, sizeof(Crcresult), "%.8X", GetCRC32BFromBuffer((unsigned char *)(LPCTSTR)(datastr), strlen(datastr)));



//CRC32B校验码


//#define GUEST_ONLINE_MaxSize 5000//      10000              //哈希表的长度   应该与M相等， 2倍，使得装载因子小于0.5 
//#define M_GUEST_ONLINE  4997//9997  //素数



//unsigned long GUEST_ONLINE_MaxSize[CYCLE_NUM] = { 9899,6400,3600,2500,900,400 };
//int M_GUEST_ONLINE[CYCLE_NUM] = { 9898,5533,3597,2497,897,397 };
#include <time.h>

//#include "comm.h"

/*
typedef struct hterm1
{
	DWORD dwLastActiveTime;			 // 记录此用户的活动时间（服务器使用）
	DWORD LoginErrorTime;
	int LoginErrorNum;
	char cpsname[MAX_USERNAME];	 // 用户名
	char checkcode[4];
	char ip[30];  //6个字符+1位\0
	//int k;      //拼音所对应的整数
	//int si;     //查找长度
				//PEER_RECORD_INFO deallist;
				//ADDR_RADER_INFO addr[MAX_ADDR_NUMBER]; // 由节点的私有终端和公共终端组成的数组
				//u_char AddrNum;					 // addr数组元素数量
				//ADDR_RADER_INFO p2pAddr;				 // P2P通信时应使用的地址（客户方使用）
				//char oncehash[7];

	//enum  entry_status states;

	PEER_INFO   OrderUserPeerList;  // 订购该操盘手的用户列表;

}GuestOnlineHash;

GuestOnlineHash *CpsOnlineList = new GuestOnlineHash[GUEST_ONLINE_MaxSize]();
*/

// 一个终端信息

/*
struct ADDR_INFO2
{
	DWORD dwIp;
	u_short nPort;
};
// 一个节点信息
struct PEER_INFO2
{
	DWORD dwLastActiveTime;			 // 记录此用户的活动时间（服务器使用）
	char szUserName[MAX_USERNAME];	 // 用户名
	u_char AddrNum;					 // addr数组元素数量
	ADDR_INFO2 addr[MAX_ADDR_NUMBER]; // 由节点的私有终端和公共终端组成的数组

	ADDR_INFO2 p2pAddr;				 // P2P通信时应使用的地址（客户方使用）

};
*/
/*
struct MaxSpaceCX
{
	int M=0;
	int N=0;
	double EMA_SPACE_MN=0;
};
MaxSpaceCX Mcx;
*/
 
#define PERIOD_NUM   8

struct PERIODTYPE
{
  int periodtype[PERIOD_NUM];
 
};

