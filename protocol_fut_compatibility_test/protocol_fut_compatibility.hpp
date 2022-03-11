///////////////////////////
// 兼容性测试工具
#pragma once
#include <iostream>
#include <ctime>
#include <string>
#include <map>


#include "msgDefine.h"
#include "../base/log.hpp"
#include "../base/tcpClient.hpp"
#include "../base/udpClient.hpp"
#include "../base/thread.hpp"

using namespace std;
using namespace pp;

enum ProtocolType
{
	TCP = 0,
	UDP = 1
};

struct LogonParam
{
	string					LogonID;
	string					PWD;
	string					Appid;
	string					Authcode;	
	string					LocalIP;
	int						LocalTradeUDPPort;
	string					Account;
	int						OrderCount;
	int						EnterOrderMs;

	LogonParam()
		:OrderCount(0)
		,EnterOrderMs(500)
		,LocalTradeUDPPort(0)
	{}
};

struct ConnectParam
{
	ConnectParam()
		:TradeServerPort(0)
		,TradeServerPortUDP(0)
		,QueryServerPort(0)
	{}
	
	string					 TradeServerAddr;
	int						 TradeServerPort;
	int						 TradeServerPortUDP;
	string					 QueryServerAddr;
	int						 QueryServerPort;
	string					 QuoteServerAddr;
	int						 QuoteServerPort;

	
};

struct Order
{
	Order()
		:ClientOrderToken(0)
		,RemOrderID(0)
		,RemAcceptStatus(0)
		,MarketAcceptStatus(0)
		,Qty(0)
		,Price(0.0)
		,FillQty(0)
		,SendFailed(false)
		,SendCancled(false)
		,IsClosed(false)
	{}

	size_t					ClientOrderToken;
	unsigned long long		RemOrderID;
	ExchangeOrderId			MarketOrderToken;
	int						RemAcceptStatus;			// 1 accept -1 rej  
	int						MarketAcceptStatus;			// 1 accept -1 rej
	int						Qty;
	string					Symbol;
	double					Price;
	int						FillQty;
	bool					SendFailed;
	bool					SendCancled;
	bool					IsClosed;
};


class CProtocolCient
{
public:
	CProtocolCient();
	CProtocolCient(ProtocolType type,
				   ConnectParam& connParam,
				   LogonParam& logon,
				   CLog* log);

	~CProtocolCient();

public:

#ifdef _WIN32
	static DWORD WINAPI threadFunTradeRecv(void*);
	static DWORD WINAPI threadFunQueryRecv(void*);

#else
	static void* threadFunTradeRecv(void*);
	static void* threadFunQueryRecv(void*);

#endif



public:
	int init();
	void run();
	bool bind_cpu(int cpu_id, pthread_t thd_id);

	void setFinishFlag();

	void calcOrderTime();
	void waitQueryResp();
	void convertFromTimestamp(unsigned long long int timeStamp, tm& tmResult, unsigned int& nanoSec);

private:
	int tradeSendData(const char* pData, int len);
	int querySendData(const char* pData, int len);

	// 协议版本校验
	int sendProtocolVersion();
	int recvVersion(resp_report_version *msg);

	// 交易登录
	int logonServer();
	int recvLogon(resp_logon* msg);

	// 查询登录
	int logonQueryServer();
	void recvQueryLogon(resp_query_logon* msg);

	// 下单
	int enterOrder(req_new_order& order);
	void recvOrderAccept(resp_order_accept* msg);
	void recvOrderRejet(resp_order_reject* msg);	

	void recvOrederMarketAccept(resp_exchange_order_accept* msg);
	void recvOrderMarketRejet(resp_exchange_order_reject* msg);
	void recvOrderExed(resp_order_execution* msg);

	// 撤单
	int canclOrder(req_cxl_order& cxl, size_t clientOrderToken);
	void recvOrderCxld(resp_order_cxled* msg);
	void recvOrderCxlRej(resp_cxl_reject* msg);	

	// 查询合约
	int querySymbol();
	void recvSymbol(resp_query_symbol* msg);
 
	// 地址端口校验
	int judgParam();

	void work(int& finishOrderCount);


	

private:
	ProtocolType					m_protocolType;

	

	ConnectParam					m_connParam;
	string							m_logonID;
	string							m_logonPWD;
	short							m_udpPort;
	string							m_appid;
	string							m_authCode;
	string							m_account;
	unsigned int					m_userID;
	int								m_maxToken;
	bool							m_isTradeLogon;
	bool							m_isQueryLogon;
	bool							m_queryResp;

	char							m_uuid[33];
	map<string, resp_query_symbol>	m_symbol;
	int								m_symbolCount;

	CLog*							m_log;
	map<size_t, Order>			    m_order;
	int								m_orderCount;
	int								m_cancelCount;
	int								m_orderMs;

	pp::CThread						m_threadTrade;
	pp::CThread						m_threadQuery;
	boost::shared_ptr<CTCPClient>	m_trade;
	boost::shared_ptr<CUDPClient>	m_tradeUDP;
	boost::shared_ptr<CTCPClient>	m_query;
	bool							m_finishFlag;
	boost::shared_mutex				m_tradeLock;
	boost::shared_mutex				m_queryLock;
};