#pragma once
#include "protocol_fut_compatibility.hpp"
#include "../base/xml.hpp"
#include "../base/log.hpp"
#include "../base/db.hpp"

#include <boost/shared_ptr.hpp>

#ifdef _WIN32
	#ifdef _DEBUG
	#pragma comment(lib, "base_jys_d.lib")
	#else
	#pragma comment(lib, "base_jys.lib")
	#endif
#endif

using namespace pp;

CLog													g_log("protocol_fut_compatibility", true);
vector<boost::shared_ptr<pp::CThread> >					g_threadRunV;
vector<boost::shared_ptr<CProtocolCient> >				g_clientV;
int														g_finishClientN = 0;
boost::shared_mutex										g_lock;

#ifdef _WIN32
DWORD WINAPI clientRun(void*clientAddr)
#else
void* clientRun(void* clientAddr)
#endif
{
	CProtocolCient* client = (CProtocolCient*) clientAddr;
	client->run();
	writeLock wtlock( g_lock );
	++g_finishClientN;
	return NULL;
}

int main()
{
	ConnectParam connParam;
	string ip;
	int port = 0;
	string dbName;
	string user;
	string dbPwd;
	int orderCount = 0;
	int orderMs = 0;
	int	userCount;

	string paramName = "protocol_fut_compatibility.xml";
	ptree pt;
	read_xml(paramName, pt, boost::property_tree::xml_parser::trim_whitespace);
	BOOST_AUTO(child, pt.get_child("config.param"));//获取根节点的子节点
	BOOST_AUTO(pos, child.begin());
	for (; pos != child.end(); ++pos) 
	{ 
		if( "server_param" == pos->first)
		{
			connParam.TradeServerAddr = pos->second.get<string>("<xmlattr>.trade_ip");
			connParam.TradeServerPort = pos->second.get<int>("<xmlattr>.trade_port");
			connParam.TradeServerPortUDP = pos->second.get<int>("<xmlattr>.trade_udp_port");
			connParam.QueryServerAddr = pos->second.get<string>("<xmlattr>.query_ip");
			connParam.QueryServerPort = pos->second.get<int>("<xmlattr>.query_port");
		}

		if("db_param" == pos->first)
		{
			ip = pos->second.get<string>("<xmlattr>.host");
			port = pos->second.get<int>("<xmlattr>.port");
			dbName = pos->second.get<string>("<xmlattr>.db_name");
			user = pos->second.get<string>("<xmlattr>.user");
			dbPwd = pos->second.get<string>("<xmlattr>.pwd");
		}

		if("user_info" == pos->first)
		{
			userCount = pos->second.get<int>("<xmlattr>.test_user_count");
			orderCount = pos->second.get<int>("<xmlattr>.order_count");
			orderMs = pos->second.get<int>("<xmlattr>.order_wait_ms");
		}
	}

	CDB mysqlDB(ip, port, dbName, user, dbPwd, boost::shared_ptr<CLog>(&g_log));
	mysqlDB.query("select login_id, md5_pwd from t_user_info order by login_id ");
	DataType data = mysqlDB.getData();

	CDB mysqlDBUser(ip, port, dbName, user, dbPwd, boost::shared_ptr<CLog>(&g_log));
	mysqlDBUser.query("select login_id, account_id from t_user_info order by login_id ");
	DataType accountData = mysqlDBUser.getData();

	map<int, int> userUDPPort;
	// 最多测试用户数量 按xml配置，读取数据库前n个用户
	if(userCount > data.size())
	{
		userCount = data.size();
	}

	for(size_t i = 0; i < userCount; ++i)
	{
		LogonParam p;
		p.LogonID = data[i]["login_id"];
		p.PWD = data[i]["md5_pwd"];
		p.Appid = "SKT";
		p.Authcode = "skt_auth_code";
		p.OrderCount = orderCount;
		p.EnterOrderMs = orderMs;

		p.Account = accountData[i]["account_id"];

		if(connParam.TradeServerPortUDP)
		{
			int udpProt = 0;
			while(udpProt < 10000 || userUDPPort.find(udpProt) != userUDPPort.end())
			{
				// 随机udp端口
				udpProt = rand() % 60000;
			}

			userUDPPort[udpProt] = i;
			p.LocalTradeUDPPort = udpProt;
		}

		g_clientV.push_back(boost::shared_ptr<CProtocolCient>(new CProtocolCient(
			(0 == connParam.TradeServerPortUDP)? TCP:UDP, connParam, p, &g_log))); 
		g_threadRunV.push_back(boost::shared_ptr<CThread>(new CThread(
			clientRun, (void*)&(*g_clientV[i]))));	

		_sleep_(5000);
	}

	string quit;
	while(1)
	{
		cin >> quit;
		if("quit" == quit)
		{
			for(size_t i = 0; i < g_clientV.size(); ++i)
			{
				g_clientV[i]->setFinishFlag();
			}

			size_t time = 1000;
			while(time > 0)
			{
				_sleep_(1000);
				--time;
				if(g_finishClientN == g_clientV.size())
				{
					return 0;
				}
			}
		}

		if(g_finishClientN == g_clientV.size())
		{
			return 0;
		}
	}

	return 0;
}

