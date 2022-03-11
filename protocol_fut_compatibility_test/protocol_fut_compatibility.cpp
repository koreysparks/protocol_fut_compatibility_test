#include "protocol_fut_compatibility.hpp"

	void proc(int msgLen, char* data)
	{
		data += msgLen; 
	}

	CProtocolCient::CProtocolCient()
	{}

	CProtocolCient::CProtocolCient(
		ProtocolType type,
		ConnectParam& connParam,
		LogonParam& logon,
		CLog* log)
		:m_protocolType(type)
		,m_connParam(connParam)
		,m_logonID(logon.LogonID)
		,m_logonPWD(logon.PWD)
		,m_appid(logon.Appid)
		,m_authCode(logon.Authcode)
		,m_account(logon.Account)
		,m_orderMs(logon.EnterOrderMs)
		,m_orderCount(logon.OrderCount)
		,m_udpPort(logon.LocalTradeUDPPort)
		,m_cancelCount(0)
		,m_userID(0)
		,m_isTradeLogon(false)
		,m_isQueryLogon(false)
		,m_symbolCount(0)
		,m_maxToken(0)
		,m_log(log)
		,m_finishFlag(false)
	{
		int tradeResult = -1;
		int queryResult = -1;
		m_trade = boost::shared_ptr<CTCPClient>(new CTCPClient(connParam.TradeServerAddr.c_str(), connParam.TradeServerPort, tradeResult));
		m_query = boost::shared_ptr<CTCPClient>(new CTCPClient(connParam.QueryServerAddr.c_str(), connParam.QueryServerPort, queryResult));
		assert(0 == tradeResult && 0 == queryResult);

		if(UDP == type && 0 == logon.LocalTradeUDPPort)
		{
			log->log("error, user:%s local udp port is 0 ",	m_logonID.c_str());
			return;
		}

		if(UDP == type)
		{
			m_tradeUDP = boost::shared_ptr<CUDPClient>(new CUDPClient(connParam.TradeServerAddr.c_str(), connParam.TradeServerPortUDP, m_udpPort));
		}

		memset(m_uuid, 0, sizeof(m_uuid));
	}

	CProtocolCient::~CProtocolCient()
	{
		//m_trade->close();
		//m_query->close();	
	}

	
	int CProtocolCient::judgParam()
	{
		if(m_connParam.TradeServerAddr.empty() || 0 == m_connParam.TradeServerPort)
		{
			m_log->log("error user:%s trade param invalid %s:%d " , m_logonID.c_str(), m_connParam.TradeServerAddr.c_str(), m_connParam.TradeServerPort);
			return 1;
		}

		if(m_connParam.QueryServerAddr.empty() || 0 == m_connParam.QueryServerPort)
		{
			m_log->log("warn  user:%s query param invalid %s:%d " , m_logonID.c_str(), m_connParam.QueryServerAddr.c_str(), m_connParam.QueryServerPort);			
		}

		return 0;
	}


	void CProtocolCient::work(int& finishOrderCount)
	{
		req_new_order order;
		char* p = (char*)&order;
		for(size_t j = 0; j < (sizeof(req_new_order) - 1); ++j)
		{
			*(++p) = rand();
		}

		order.m_nUserID = m_userID;
		enterOrder(order);
		++finishOrderCount;
		_sleep_(m_orderMs);

		if(0 == finishOrderCount % 100)
		{
			m_log->log("debug user:%s enter order count %d",m_logonID.c_str(), finishOrderCount);
			m_log->log("debug user:%s cancel order count %d", m_logonID.c_str(), finishOrderCount);
		}

		req_cxl_order cxl;
		p = (char*)&cxl;
		for(size_t j = 0; j < (sizeof(req_cxl_order) - 1); ++j)
		{
			*(++p) = rand();
		}

		canclOrder(cxl, cxl.m_nOrderReferenceNumber);
		_sleep_(m_orderMs);
	}

	void CProtocolCient::run()
	{
		if(0 != init())
		{
			m_log->log("error user:%s init failed", m_logonID.c_str());
			return;
		}

		m_log->log("info  user:%s trade logon status: %d  query logon status: %d ", m_logonID.c_str(), m_isTradeLogon, m_isQueryLogon);
		
		while(1)
		{
			if(m_finishFlag)
			{
				_sleep_(2000);
				return;
			}
		}
	}

	int CProtocolCient::init()
	{
		if(judgParam())
		{
			return -1;
		}

		m_threadTrade.init(threadFunTradeRecv, (void*)this);
		m_threadQuery.init(threadFunQueryRecv, (void*)this);

		for (size_t i = 0; i < 10000000; ++i)
		{
			_sleep_(_TIME_MS_);
			if(m_isTradeLogon && m_isQueryLogon)
			{
				return 0;
			}
		}

		return -1;
	}
	
#ifdef _WIN32
	DWORD CProtocolCient::threadFunQueryRecv(void* lParam)
#else
	void* CProtocolCient::threadFunQueryRecv(void* lParam)
#endif
	{
		CProtocolCient* myself = (CProtocolCient*)lParam;
		const char* data = NULL;
		int dataLen = 0;
		unsigned char msgID;

		while(1)
		{
			if(myself->m_isTradeLogon)
			{
				readLock rlock(myself->m_queryLock);
				myself->logonQueryServer();
				break;
			}
		}

		while(1)
		{
			if(!myself->m_isQueryLogon)
			{
				continue;
			}

			readLock rlock(myself->m_queryLock);
			if(myself->m_finishFlag)
			{
				return NULL;
			}

			myself->querySymbol();
			data = myself->m_query->recv(dataLen);
			if (data)
			{
				while(data && *data)
				{
					msgID = (response_msg_id) (*data) ;
					switch(msgID)
					{
						case	resp_query_logon_msg_id				:	myself->recvQueryLogon((resp_query_logon*)data);	data += sizeof(resp_query_logon); 		break;
						case	resp_query_symbol_msg_id			:	myself->recvSymbol((resp_query_symbol*)data);		data += sizeof(resp_query_symbol); 		break;

						default										:	myself->m_log->log("warn  user:%s query unknow msg id: %d", myself->m_logonID.c_str(), msgID);	data = NULL;					break;
					}
				}
			}
		}

		return 0;
	}


#ifdef _WIN32
	DWORD CProtocolCient::threadFunTradeRecv(void* lParam)
#else
	void* CProtocolCient::threadFunTradeRecv(void* lParam)
#endif
	{
		CProtocolCient* myself = (CProtocolCient*)lParam;
		myself->sendProtocolVersion();

		int finishOrderCount = 0;
		unsigned char msgID;
		int dataLen = 0;
		const char* data = NULL;
		while(1)
		{
			if(myself->m_finishFlag || finishOrderCount >= myself->m_orderCount)
			{
				return NULL;
			}

			readLock rlock(myself->m_tradeLock);
			if(myself->m_isTradeLogon)
			{
				myself->work(finishOrderCount);
			}

			data = myself->m_trade->recv(dataLen);
			if (data)
			{
				while(data && *data)
				{
					msgID = (response_msg_id) (*data) ;
					switch(msgID)
					{
					case	resp_report_version_msg_id		: myself->recvVersion((resp_report_version*)data); 						data += sizeof(resp_report_version);		break;
					case	resp_logon_msg_id				: myself->recvLogon((resp_logon*)data);									data += sizeof(resp_logon);					break;
					case	resp_order_accept_msg_id		: myself->recvOrderAccept((resp_order_accept*)data); 					data += sizeof(resp_order_accept);			break;
					case    resp_order_reject_msg_id		: myself->recvOrderRejet((resp_order_reject*)data); 					data += sizeof(resp_order_reject);			break;
					case	resp_order_execution_msg_id		: myself->recvOrderExed((resp_order_execution*)data);					data += sizeof(resp_order_execution);		break;
					case    resp_order_exchange_accept_id	: myself->recvOrederMarketAccept((resp_exchange_order_accept*)data); 	data += sizeof(resp_exchange_order_accept);	break; 
					case	resp_order_exchange_reject_id	: myself->recvOrderMarketRejet((resp_exchange_order_reject*)data);		data += sizeof(resp_exchange_order_reject);	break;
					case	resp_cxl_reject_msg_id			: myself->recvOrderCxlRej((resp_cxl_reject*)data);						data += sizeof(resp_cxl_reject);			break;
					case	resp_order_cxled_msg_id			: myself->recvOrderCxld((resp_order_cxled*)data);						data += sizeof(resp_order_cxled);			break;

					default									: myself->m_log->log("warn  user:%s trade unknow msg id: %d",  myself->m_logonID.c_str(), msgID);			data = NULL;								break;
					}
				}
			}
		}

		return NULL;
	}


int CProtocolCient::tradeSendData(const char* pData, int len)
{
	return m_trade->send(pData, len);
}

int CProtocolCient::querySendData(const char* pData, int len)
{
	m_queryResp = false;
	return m_query->send(pData, len);
}

int CProtocolCient::sendProtocolVersion()
{
	req_report_version	ver; 
	return tradeSendData((const char*)&ver, sizeof(req_report_version));
}

int CProtocolCient::recvVersion(resp_report_version *msg)
{
	m_log->log("info  user:%s return protocol version:%d", m_logonID.c_str(), msg->m_nVersion);
	return logonServer();
}

int CProtocolCient::logonServer()
{
	req_logon logonMsg;
	logonMsg.m_nReserved[0] = 1;
	strncpy(logonMsg.m_sLogonName, m_logonID.c_str(), sizeof(logonMsg.m_sLogonName));
	strncpy(logonMsg.m_sUserPwd,  m_logonPWD.c_str(), sizeof(logonMsg.m_sUserPwd));
	strncpy(logonMsg.m_sClientMac, m_authCode.c_str(), sizeof(logonMsg.m_sClientMac));
	strncpy(logonMsg.m_sClientProdInfo, m_appid.c_str(), sizeof(logonMsg.m_sClientProdInfo));
	logonMsg.m_udpPort = m_udpPort;
	return tradeSendData((const char*)&logonMsg, sizeof(req_logon));
}

int CProtocolCient::recvLogon(resp_logon* msg)
{
	if (msg->m_nResult == 0)
	{
		m_userID = msg->m_nUserId;
		m_maxToken = msg->m_nMaxOrderToken;

		// µÇÂ¼²éÑ¯		
		strcpy(m_uuid, msg->m_sLogonID);
		m_log->log("info  user:%s trade logon succeed", m_logonID.c_str());	
		m_isTradeLogon = true;
	}
	else 
	{
		m_log->log("error user:%s trade logon failed  result:%d",m_logonID.c_str(), msg->m_nResult) ;
	}

	return 0;
}

int CProtocolCient::logonQueryServer()
{
	req_query_logon logonMsg;
	logonMsg.m_userID = m_userID;
	strcpy(logonMsg.m_UUID, m_uuid);

	return querySendData((const char*)&logonMsg, sizeof(req_query_logon));
}

void CProtocolCient::recvQueryLogon(resp_query_logon *msg)
{
	if(msg->m_result == 0)
	{
		m_isQueryLogon = true;
		m_log->log("info  user:%s query logon succeed ", m_logonID.c_str());
		return;
	}
	else
	{
		m_log->log("info  user:%s query logon failed  result:%d", m_logonID.c_str(),msg->m_result);
	}
}

int CProtocolCient::enterOrder(req_new_order& order)
{
	order.m_nUserID = m_userID;
	order.m_nOrderToken = ++m_maxToken;
	if(0 != m_udpPort)
	{
		int ret = m_tradeUDP->sendto((const char*)&order, sizeof(req_new_order));
		if(ret <= 0)
		{
			m_log->log("error udp enter order failed ret:%d", ret);
			return ret;
		}
	}

	int ret = tradeSendData((const char*)&order, sizeof(req_new_order));
	if(-1 == ret)
	{
		m_log->log("error user:%s enter order failed, ret:%d ", m_logonID.c_str(), ret);
	}

	return ret;
}

void CProtocolCient::recvOrderAccept(resp_order_accept* msg)
{
	m_log->log("info  user:%s recv order accept clt order token:%d", m_logonID.c_str(), msg->m_nOrderToken);
}

void CProtocolCient::recvOrderRejet(resp_order_reject* msg)
{

}

void CProtocolCient::recvOrederMarketAccept(resp_exchange_order_accept* msg)
{
	m_log->log("info  user:%s recv order mkt accept clt order token:%d", m_logonID.c_str(), msg->m_nOrderToken);
}

void CProtocolCient::recvOrderMarketRejet(resp_exchange_order_reject* msg)
{
}

void CProtocolCient::recvOrderExed(resp_order_execution* msg)
{
}

int CProtocolCient::canclOrder(req_cxl_order& cxl, size_t token)
{
	cxl.m_nUserID = m_userID;
	if(0 != m_udpPort)
	{
		return m_tradeUDP->sendto((const char*)&cxl, sizeof(req_cxl_order));
	}

	int ret = tradeSendData((const char*)&cxl, sizeof(req_cxl_order));
	if(ret  <= 0)
	{
		m_log->log("error user:%s cancl order failed ret:%d", m_logonID.c_str(), ret);
	}

	return ret;
}

void CProtocolCient::recvOrderCxld(resp_order_cxled* msg)
{
	m_order[msg->m_nOrderToken].IsClosed = true;
	m_log->log("info user:%s cancl order succeed clt token:%d ", m_logonID.c_str(), msg->m_nOrderToken);
}

void CProtocolCient::recvOrderCxlRej(resp_cxl_reject* msg)
{
}

void CProtocolCient::convertFromTimestamp(unsigned long long int timeStamp, tm& tmresult, unsigned int& nanoSec)
{
	unsigned int sec;
	char* p = (char*)(&timeStamp);
	memcpy(&sec, p + 4, 4);
	memcpy(&nanoSec, p, 4);

#ifdef _WIN32
	time_t tt32 = (time_t)sec;
	localtime_s(&tmresult, &tt32);
#else
	time_t tt32 = (time_t)sec;
	localtime_r(&tt32, &tmresult);
#endif
	return;
}

int CProtocolCient::querySymbol()
{
	req_query_symbol queryS;
	queryS.m_nUserId = m_userID;
	m_queryResp = false;
	return querySendData((const char*)&queryS, sizeof(req_query_symbol));
}

void CProtocolCient::recvSymbol(resp_query_symbol* msg)
{
	if(NULL != msg)
	{
		if(msg->m_nLastRecordFlag == 1)
		{
			m_log->log("info  user:%s recv symbol finished", m_logonID.c_str());
			m_queryResp = true;
			return;
		}
		m_symbol[msg->m_sSymbol] = *msg;
		m_symbolCount++;
	}
}


bool CProtocolCient::bind_cpu(int cpu_id, pthread_t thd_id)
{
#ifndef _WIN32

	int	cpu = (int)sysconf(_SC_NPROCESSORS_ONLN);
	cpu_set_t	cpu_info;

	if(cpu < cpu_id)
	{
		return false;
	}

	CPU_ZERO(&cpu_info);
	CPU_SET(cpu_id, &cpu_info);

	if(0 == pthread_setaffinity_np( thd_id, sizeof(cpu_set_t), &cpu_info )  )
	{
		m_log->log("bind cpu failed  cpu id:%d thread id:%d", cpu_id, thd_id);
		return false;
	}

	m_log->log("bind cpu success  cpu id:%d thread id:%d", cpu_id, thd_id);
#endif

	return true;

}


void CProtocolCient::setFinishFlag()
{
	writeLock tlock(m_tradeLock);
	writeLock qlock(m_queryLock);
	m_finishFlag = true;
}