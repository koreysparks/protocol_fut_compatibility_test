#pragma pack(push, 1)

#include <assert.h>
#include <string.h>

#define EES_PROTOCOL_VERSION		15

typedef unsigned char			UInt1 ;
typedef unsigned short			UInt2 ;
typedef unsigned int			UInt4 ;
typedef unsigned long long		UInt8 ;	

/// 4位长度的价格
struct	Price4
{
protected:
	UInt4 m_val ;

public:
	Price4()
	{
		m_val = 0 ; 
	}

	Price4& operator = (double val)
	{
		m_val = (val + 0.00001) * 10000 ;
		return *this ;
	}

	operator double ()
	{
		return m_val * 1.0/ 10000 ;
	}
};

/// 6位长度的价格
struct	Price6
{
protected:
	UInt1 m_val[6] ; 

public:
	Price6()
	{
		m_val[0] = 
		m_val[1] = 
		m_val[2] = 
		m_val[3] = 
		m_val[4] = 
		m_val[5] = 0 ;
	}

	Price6& operator = (double val)
	{
		long long l_val = (val + 0.00001) * 10000 ;
		memcpy(m_val, &l_val, 6) ;
		return *this ;
	}

	operator double ()
	{
		unsigned long long val = 0 ; 
		memcpy( &val, m_val, 6) ;
		return val * 1.0/ 10000 ;
	}
};


typedef char	AccountID[16]	;		///< 帐户ID
typedef char	Symbol[8]	;			///< 合约
typedef	char	SymbolName[21];			///< 合约名称
typedef char	ProductID[32] ;			///< product ID
typedef char	UserName[33];			///< 用户登录名
typedef char	UserPwd[33] ;			///< 用户登录密码
typedef char	ExchangeOrderId[24];	///< 交易所单号
typedef char	ExchangeExecId[24];		///< 交易所成交号

enum req_msg_id 
{
	req_new_order_msg_id					= 10 ,
	req_cxl_order_msg_id					= 11 ,
	req_query_logon_msg_id					= 68,
	req_logon_msg_id						= 70 ,
	req_query_user_link_account_msg_id		= 71 ,
	req_query_symbol_msg_id					= 72 ,
	req_query_account_position_msg_id		= 73 ,
	req_rewind_account_order_msg_id			= 74 ,
	req_rewind_account_exeution_msg_id		= 75 ,
	req_query_account_margin_msg_id			= 77 ,
	req_query_account_fee_msg_id			= 78 ,
	req_report_version_msg_id				= 80 ,
	req_query_account_buying_power_msg_id	= 81 ,
	req_exechage_connect_session_msg_id		= 84 ,
	req_instrument_status_msg_id			= 87 ,

};

enum response_msg_id
{
	resp_report_version_msg_id							= 80  ,
	resp_logon_msg_id									= 70  ,
	resp_query_logon_msg_id								= 68  ,
	resp_query_user_link_account_msg_id					= 71  ,
	resp_query_symbol_msg_id							= 72  ,
	resp_query_account_margin_msg_id					= 77  ,
	resp_query_account_fee_msg_id						= 78  ,
	resp_query_account_position_msg_id					= 73  ,
	resp_rewind_accoutn_order_msg_id					= 74  ,
	resp_rewind_account_order_finish_msg_id				= 76  ,
	resp_rewind_order_execution_msg_id					= 75  ,
	resp_rewind_account_exe_finish_msg_id				= 79  ,
	resp_query_account_buying_power_msg_id				= 81  ,
	resp_order_accept_msg_id							= 10  ,
	resp_order_reject_msg_id							= 11  ,
	resp_order_execution_msg_id							= 12  ,
	resp_order_cxled_msg_id								= 15  ,
	resp_cxl_reject_msg_id								= 174 ,
	resp_order_exchange_accept_id						= 180 ,
	resp_order_exchange_reject_id						= 181 ,
	recv_post_order_received_msg_id						= 17  ,
	recv_post_order_execution_received_msg_id			= 18  , 
	recv_query_exechange_connect_session_msg_id			= 84  ,
	recv_exechane_connect_session_status_change_msg_id  = 85  ,
	recv_query_instrument_status_msg_id					= 87  ,
	recv_instrument_status_change_msg_id				= 86  ,
};

/// 向服务器报告当前的版本号 （长度8）
struct req_report_version
{
	req_report_version()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_report_version_msg_id ; 
		m_nVersion = EES_PROTOCOL_VERSION ;
	}
	UInt1	m_nMsgID ;		///< 80=报告当前的版本号 
	UInt4	m_nVersion ;	///< 客户端协议版本号，服务器收到后如果不匹配则立即断开连接
	UInt1	m_nReserved[3]; ///< 预留域
};

/// 交易登录消息
struct req_logon
{
	req_logon()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_logon_msg_id ;
		m_nVersion = EES_PROTOCOL_VERSION ;		
		m_nReserved[0] = 1; 
	}

	UInt1		m_nMsgID ;			///<	70=登录请求
	UInt4		m_nVersion ;		///<	协议版本号，与服务器不匹配的话不允许登录
	UserName	m_sLogonName ;		///<	登录名，登录成功后会返回数字型的UserId，之后需要都用UserId进行交互
	UserPwd		m_sUserPwd ;		///<	MD5加密后的密码
	UInt1		m_nReserved[1];		///< 预留域
	char		m_sClientMac[18];	///客户端MAC地址,用52:54:00:8F:FE:D6这样的格式上传，中间的冒号改成-号也行
	char		m_sClientProdInfo[30];	//客户端标识，如"REMClient"
	short		m_udpPort;
	char		m_nReserved2[134];		//保留
};

/// 查询登录消息
struct req_query_logon
{
	req_query_logon()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_logon_msg_id ; 
	}

	UInt1		m_nMsgID ;		///<	68 = 查询通道登录请求
	char		m_UUID[33];		///<	交易通道登录返回信息
	UInt4		m_userID;
	char		m_nReserved[218];

};

/// 查询用户下面的帐户 （长度8）
struct req_query_user_link_account
{
	req_query_user_link_account()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_user_link_account_msg_id ;		
	}

	UInt1	m_nMsgID ;			///< 71=账户列表查询
	UInt4	m_nUserId ;			///< 登录名对应的用户名
	UInt1	m_nReserved[3];		///< 预留域
} ;

/// 查询合约列表 （长度8）
struct req_query_symbol
{
	req_query_symbol()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_symbol_msg_id ;
	}
	UInt1	m_nMsgID ;			///<72=合约列表查询	
	UInt4	m_nUserId;			///<用户Id
	UInt1	m_nReserved[3];		///<预留域
};

/// 帐户合约保证金查询 （长度24）
struct req_query_account_margin
{
	req_query_account_margin()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_account_margin_msg_id ;		
	}

	UInt1		m_nMsgID ;			///< 77=保证金查询
	UInt4		m_nUserId ;			///< 登录名对应的用户名
	AccountID	m_sAccount ;		///< 账户ID
	UInt1		m_nReserved[3];		///< 预留域
};

/// 帐户合约费率查询 （长度24）
struct req_query_account_fee
{
	req_query_account_fee()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_account_fee_msg_id ;		
	}

	UInt1		m_nMsgID ;		///< 78=费率查询
	UInt4		m_nUserId ;		///< 登录名对应的用户名
	AccountID	m_sAccount ;	///< 账户ID
	UInt1		m_nReserved[3]; ///< 预留域
};

/// 查询帐户仓位 （长度32）
struct req_query_account_position
{
	req_query_account_position()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_account_position_msg_id ;		
	}
	UInt1		m_nMsgID ;		///<	73=仓位查询
	UInt4		m_nUserID ;		///< 	登录用户的ID
	AccountID	m_sAccount ;	///< 	账户ID
	UInt4		m_nRequestId;	///<	查询请求ID，请从1开始编号，响应中返回同样的值。
	UInt1		m_nReserved[7]; ///<	预留域
};

/// 回滚帐户的订单 （长度24）
struct req_rewind_account_order
{
	req_rewind_account_order()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_rewind_account_order_msg_id ;
	}
	UInt1		m_nMsgID ;	///<	74=订单查询
	UInt4		m_nUserID ;	///< 	登录用户的ID
	AccountID	m_sAccount ;		///< 	账户ID
	UInt1		m_nReserved[3]; ///< 预留域
};

// 请求查询客户成交 （长度24）
struct req_rewind_account_exeution
{
	req_rewind_account_exeution()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_rewind_account_exeution_msg_id ;
	}
	UInt1		m_nMsgID ;			///<	75=成交查询
	UInt4		m_nUserID ;			///<	登录用户的ID
	AccountID	m_sAccount ;		///<	账户ID
	UInt1		m_nReserved[3];		///<	预留域
};

// 帐户资金情况查询 （长度32）
struct req_query_account_buying_power
{
	req_query_account_buying_power()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_account_buying_power_msg_id ;
	}
	UInt1		m_nMsgID ;			///<	81=BP查询
	UInt4		m_nUserID ;			///<	登录用户的ID
	AccountID	m_sAccount ;		///<	账户ID
	UInt4		m_nRequestId;		///<	查询请求ID，请从1开始编号，响应中返回同样的值。
	UInt1		m_nReserved[7];		///<	预留域
};

//  交易所连接席位查询请求 （长度16）
struct	req_exechage_connect_session
{
	req_exechage_connect_session()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_exechage_connect_session_msg_id ;
	}
	UInt1		m_nMsgID;					//< 消息类型	     84
	UInt4		m_nUserID;					///< 用户编号
	UInt4		m_nRequestId;				///< 请求ID
	UInt1		m_nReserved[7];				///< 预留域
};

// 合约状态查询情况 （长度16）
struct req_instrument_status
{
	req_instrument_status()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_instrument_status_msg_id ;
	}
	UInt1		m_nMsgID;					//< 消息类型	     87
	UInt4		m_nUserID;					///< 用户编号
	UInt4		m_nRequestId;				///< 请求ID
	UInt1		m_nReserved[7];				///< 预留域

};
/// 下单指令 （长度96）
struct req_new_order
{
	req_new_order()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_new_order_msg_id;	
		m_nSecType = 3;		
		m_nHedgeFlag = 2;
		m_nTif = 99998;
		m_nExpireMonthAndCallPut = 9;
		m_nExpireDay = 16;
		m_nExpireYear = 13;
	}

	UInt1		m_nMsgID;						///<消息类型	10
	UInt4		m_nUserID;						///<送单子的用户编号
	Symbol		m_sSymbol;						///<合约代码	
	AccountID	m_sAccount;						///<资金帐号
	UInt1		m_nOrderType;					///<单子类型 1=Limit 限价单
	UInt1		m_nSide;						///<买卖方向：1=买单（开今）  2=卖单（平今）  3=买单（平今）  4=卖单（开今）
	UInt4		m_nQuantity;					///<数量
	Price6		m_dPrice;						///<价格
	UInt1		m_nHedgeFlag;					///<投机套利标志，1=套利；2=投机；3=套保
	UInt1		m_mkt_session_id;				///<指定席位号，可以填0
	UInt1		m_session_select_mode;			///<如要指定席位号，必须填1，否则填0
	UInt4		m_nOrderToken;					///<客户编制的订单编号	
	UInt1		m_nCrossType;					///<保留	
	UInt1		m_nSecType;						///<交易品种类型，3=期货 2=期权
	UInt1		m_nExpireMonthAndCallPut;		///<期权相关，先填9	
	UInt1		m_nExpireDay;					///<期权相关，先填16	
	UInt1		m_nExpireYear;					///<期权相关，先填13	
	Price6		m_dStrikePrice;					///<保留	，期权支持后有用处	
	Price6		m_nReserved1;					///<保留
	UInt4		m_nTif;							///<单子存活时间，99998为当日有效，0为FOK/FAK订单
	UInt1		m_nReserved2[2];				///<保留		
	UInt1		m_nExchangeId;					///<102=中金所,103=上期所,104=大商所,105=郑商所
	UInt1		m_nReserved3;					///<保留
	UInt4		m_nMinQuantity;					///<最小成交数量  0：任何数量
	UInt1		m_nReserved4[6];				///<保留	
	UInt8		m_custom_field;					///<保留
	UInt1		m_nReserved5[5];				///<保留
};

/// 请求撤单指令
struct req_cxl_order
{
	req_cxl_order()
	{
		memset(this, 0, sizeof(*this)) ;
		m_nMsgID = req_cxl_order_msg_id ; 
	}
	UInt1		m_nMsgID;						///<消息类型	11
	UInt4		m_nUserID;						///<送单子的用户编号
	UInt8		m_nOrderReferenceNumber;		///<盛立系统产生的单子号
	UInt4		m_Reserved1;					///<保留
	AccountID	m_sAccount ;					///<资金账号
	UInt1		m_nReserved2[7];				///<保留
};



/// 回复服务端版本号 （长度8）
struct resp_report_version
{
	UInt1	m_nMsgID ;		///< 80
	UInt4	m_nVersion ;	///< 客户端协议版本号， 
	UInt1	m_nReserved[3]; ///< 预留域
};

/// 登录回应 （长度56）
struct resp_logon
{
	UInt1		m_nMsgID ;			///<	70
	UInt4		m_nVersion ;		///<	协议版本号，与服务器不匹配的话不允许登录
	UInt4		m_nResult	;		///<	返回0表示登录成功，非0表示登录失败，失败时，不会返回UserId		0-	登录成功		1-	用户名/密码错误		2-	用户存在配置问题，如账户列表为空等
	UserName	m_sLogonID;			///<	登录名，登录成功后会返回数字型的UserId，之后需要都用UserId进行交互
	UInt4		m_nUserId ;			///<	登录名对应的用户ID
	UInt4		m_nTradingDate;		///<	交易日，格式为yyyyMMdd的int型值
	UInt4		m_nMaxOrderToken ;	///<	该用户下单目前最大的Token
	UInt1		m_nReserved[2];		///<	预留域
};

/// 查询通道登录回应 （长度256）
struct resp_query_logon
{
	UInt1		m_nMsgID;				///<	68
	UInt4		m_result;				///<	登录结果 0 = 成功
	UInt4		m_orderFCParamCount;	///<    下单流控参数
	UInt4		m_orderFCParamInterval; ///<	微妙数
	UInt4		m_cancelFCParamCount;	///<    撤单流控参数
	UInt4		m_cancelFCParamInterval; ///<	微妙数
	UInt1		m_nReserved[235];
};

/// 查询用户关联帐户回应 （长度80）
struct resp_query_user_link_account
{
	UInt1		m_nMsgID ;				///<	71
	UInt1		m_nLastRecordFlag;		///<	0：表示常规记录	1：表示结束标志，本条记录的实际内容没有意义
	UInt4		m_nUserId;				///< 	用户Id
	AccountID	m_sAccount	;			///<	
	UInt4		m_nPrevilege ;			///< 	操作权限，目前硬件暂不支持，也就是说都是完全操作权限	99：完全操作	1：只读	2：只平仓
	double		m_dInitialBp ;			///<	初始权益
	double		m_dAvailableBp	;		///< 	总可用资金
	double		m_dMargin ; 			///< 	所有仓位占用的保证金
	double		m_dFrozenMargin ;		///< 	所有挂单冻结的保证金
	double		m_dCommissionFee  ;		///< 	已扣除的手续费总金额
	double		m_dFrozenCommission ;	///<	挂单冻结的总手续费金额
	UInt1		m_nReserved[6];			 ///< 预留域
} ;

///查询合约列表回应 （长度120）
struct resp_query_symbol 
{
	UInt1		m_nMsgID ;					///<	72
	UInt1		m_nLastRecordFlag;			///<	0-	表示常规记录		1-	表示结束标志，本条记录的实际内容没有意义
	UInt1		m_nSecType	;				///<	3=Future，目前只有这个
	Symbol		m_sSymbol	;				///<	合约名称/股票代码
	SymbolName  m_sSymbolName;				///<	合约名称
	UInt1		m_nExchangeID ;				///<	102=中金所		103=上期所		104=大商所		105=郑商所
	ProductID	m_sProdID ;					///<	产品代码
	UInt1		m_nProductClass ;			///<	目前只会是1
	UInt2		m_nDeliveryYear ;			///<	交割年
	UInt1		m_nDeliveryMonth ;			///<	交割月
	UInt4		m_nMaxMarketOrderVolume ;	///<	市价单最大下单量
	UInt4		m_nMinMarketOrderVolume	;	///<	市价单最小下单量
	UInt4		m_nMaxLimitOrderVolume ;	///<	限价单最大下单量
	UInt4		m_nMinLimitOrderVolume ;	///<	限价单最小下单量
	UInt4		m_nVolumeMultiple;			///<	合约乘数
	double		m_dPriceTick;				///<	最小变动价位*1000000
	UInt4		m_nCreateDate;				///<	创建日期
	UInt4		m_nOpenDate ;				///<	上市日期
	UInt4		m_nExpireDate ;				///<	到期日
	UInt4		m_nStartDelivDate ;			///<	开始交割日
	UInt4		m_nEndDelivDate;			///<	结束交割日
	UInt1		m_nInstLifePhase ;			///<	合约生命周期状态		0=未上市		1=上市		2=停牌		3=到齐
	UInt1		m_nIsTrading;				///<	当前是否交易		0=未交易		1=交易
	UInt1		m_nReserved[1];				///<	保留
} ;

/// 查询帐户保证金回应 （长度80）
struct resp_query_account_margin
{
	UInt1		m_nMsgID ;				///<	77
	UInt1		m_nLastRecordFlag;		///<	0-	表示常规记录		1-	表示结束标志，本条记录的实际内容没有意义
	AccountID	m_sAccount;				///<	账户ID
	UInt1		m_nHedgeFlag ;			///<	投机套利标志，1=套利；2=投机；3=套保
	Symbol		m_sSymbol ;				///<	合约名称
	UInt1		m_nExchangeID ; 		///<	102=中金所		103=上期所		104=大商所		105=郑商所
	ProductID	m_sProdID;				///< 	4	Alpha	产品代码
	double		m_dLongMarginRatio ;	///<		
	double		m_dShortMarginRatio ;	///<	目前用不上
	char        m_Reserved[4];

};

/// 查询帐户费率回应 （长度112）
struct resp_query_account_fee
{
	UInt1		m_nMsgID ;							///<	Message Type	0	1	Int1	78
	UInt1		m_nLastRecordFlag;					///<	0-	表示常规记录		1-	表示结束标志，本条记录的实际内容没有意义
	AccountID	m_sAccount;							///<	账户ID
	UInt1		m_nHedgeFlag ;						///<	投机套利标志，1=套利；2=投机；3=套保
	Symbol		m_sSymbol ;							///<	合约名称/股票代码
	UInt1		m_nExchangeID	;					///<	102=中金所		103=上期所		104=大商所		105=郑商所
	ProductID	m_sProdID ;							///<	产品代码
	double		m_dOpenRatioByMoney ;				///<	开仓手续费率，按照金额
	double		m_dOpenRatioByVolume ;				///<	开仓手续费率，按照手数
	double		m_dCloseYesterdayRatioByMoney;		///<	平昨手续费率，按照金额
	double		m_dCloseYesterdayRatioByVolume;		///<	平昨手续费率，按照手数
	double		m_dCloseTodayRatioByMoney;			///<	平今手续费率，按照金额
	double		m_dCloseTodayRatioByVolume	;		///<	平今手续费率，按照手数
	UInt1		m_nPosDir;							///<	1=多头；5=空头
	UInt1		m_nReserved[3];						///<	预留域
} ;

/// 查询帐户仓位回应 （长度128）
struct resp_query_account_position
{
	UInt1		m_nMsgID ;			///< 73
	UInt1		m_nLastRecordFlag;	///< 0-	表示常规记录		1-	表示结束标志，本条记录的实际内容没有意义
	UInt4		m_nUserId ;			///< 用户Id
	AccountID	m_sAccount;			///< 资金账号
	UInt4		m_nRequestId;		///< 查询请求ID，返回同样的值，0表示主动报告
	Symbol		m_sSymbol ;			///< 合约名称/股票代码
	UInt1		m_nPosiDirection;	///< 多空方向	1：多头	5：空头
	UInt4		m_nInitOvnQty;		///< 隔夜仓初始数量，这个值不会变化，除非通过HelpDesk手工修改
	double		m_dInitOvnMargin;	///< 初始隔夜仓保证金
	UInt4		m_nOvnQty;			///< 当前隔夜仓数量，可以为0
	UInt4		m_nFrozenOvnQty;	///< 冻结的昨仓数量
	UInt4		m_nTodayQty;		///< 当前今仓数量，可能为0
	UInt4		m_nFrozenTodayQty;	///< 冻结的今仓数量
	double		m_dOvnMargin;		///< 隔夜仓占用保证金
	double		m_dTodayMargin;		///< 今仓占用的保证金
	double		m_dTotalFee;		///< 总手续费
	double		m_dOvnLiquidPL;		///< 昨仓平仓盈亏
	double		m_dTodayLiquidPL;	///< 今仓平仓盈亏
	double		m_dOvnMarketPL;		///< 昨仓持仓盈亏
	double		m_dTodayMarketPL;	///< 今仓持仓盈亏
	double		m_dPosCost;			///< 持仓成本
	UInt1		m_nHedgeFlag ;		///< 投机套利标志，1=套利；2=投机；3=套保
};

/// 订单回滚回应 （长度160）
struct resp_rewind_account_order
{
	resp_rewind_account_order()
	{
	}

	UInt1			m_nMsgID ;						///<	74
	UInt4			m_nUserId ;						///<	原来单子的用户，对应着LoginID。
	UInt1			m_OrigMsgID	;					///<	74- 单子接受信息
	UInt8			m_nTimeStamp ;					///<	时间，从晚上零时算起的纳秒数
	UInt4			m_nOrderToken ;					///<	原来单子的token 
	UInt1			m_nOrderType ;					///<	1=limit 限价单；	
	UInt1			m_nSide ;						///<	1 = 买单（开今）	2 = 卖单（平今）	3= 买单（平今）	4 = 卖单（开今）	5= 买单（平昨）	6= 卖单（平昨）
	UInt1			m_Reserved1;					///<	保留
	UInt4			m_nQuantity ;					///<	数量
	UInt1			m_InstrumentType ;				///<	3＝期货
	Symbol			m_sSymbol;						///<	合约代码
	UInt1			m_Reserved2[11];				///<	保留	
	double			m_dPrice;						///<	价格
	UInt1			m_Reserved3[8];					///<	保留	
	UInt4			m_nTif ;						///<	单子的存活时间特殊的值为	0=IOC（immediate or cancel,不成交马上取消）	99998= market hour (市场时段)
	UInt1			m_Reserved4[2];					///<	保留	
	AccountID		m_sAccount;						///<	资金帐号.
	UInt1			m_nHedgeFlag ;					///<	投机套利标志，1=套利；2=投机；3=套保
	UInt1			m_nExchengeID ;					///<	102=中金所	103=上期所	104=大商所	105=郑商所
	UInt1			m_Reserved5;					///<	保留	
	UInt4			m_nMinimumQuantity ;			///<	0：任何数量；	全部成交；	最小成交数量， 如果输入值等于大于单子数量，则为AON（All-or-none)单子。
	UInt1			m_Reserved6[8];					///<	保留	
	UInt8			m_custom_field;					///<	
	UInt8			m_nOrderReferenceNumber ;		///<	盛立系统产生的单子号，和盛立交流时可用该号。
	UInt1			m_nOrderState ;					///<	
	UInt1			m_nCxlRjtFlag	;				///<	0：挂单或者全成交	1：已经撤单	2：被拒绝	3：已过期，类似已经撤单
	UInt8			m_nCloseTime ;					///<	如果订单已经撤单、或者拒绝，则该值有效
	UInt4			m_nFilledQty ;					///<	成交数量
	ExchangeOrderId	m_chExechangeOrdID;				///<    交易所订单ID
	UInt8			m_nExchangeTranTimeStamp;		///<    市场接受/拒绝的时间戳

};

/// 成交回滚的回复 （长度88）
struct resp_rewind_order_execution
{
	UInt1			m_nMsgID ;					///<	75
	UInt4			m_nUserId ;					///<	原来单子的用户，对应着LoginID。
	AccountID		m_sAccount ;				///<	账户ID
	UInt1			m_nOrigID ;					///<	75=单子成交信息
	UInt8			m_nTimeStamp ;				///<	时间，从晚上零时算起的纳秒数
	UInt4			m_nOrderToken ;				///<	原来单子的你的token
	UInt8			m_nOrderReferenceNumber ;	///<	盛立系统产生的单子号，和盛立交流时可用该号。
	UInt4			m_nFillQty ;				///<	单子成交量
	double			m_dFillPrice	;			///<	单子成交价
	UInt1			m_nLiquidityFlag ;			///<	成交标志,目前不需要
	UInt8			m_nExecID ;					///<	单子成交号
	ExchangeExecId	m_sExchExecId;				///<	交易所成交号
	UInt1			m_nReserved[1];				///<	预留域
};

/// 当一个账户的所有订单和成交都rewind完成后的消息 （长度24）
struct resp_rewind_account_order_finish
{
	UInt1			m_nMsgID	;	///< 76
	AccountID		m_sAccount ;	///< 账户ID
	UInt1			m_nReserved[7]; ///< 预留域
};

/// 当一个账户的所有成交都rewind完成后的消息 （长度24）
struct resp_rewind_account_exe_finish
{
	UInt1			m_nMsgID	;	///< 79
	AccountID		m_sAccount ;	///< 账户ID
	UInt1			m_nReserved[7]; ///< 预留域
};

//一次只查一个账户，返回一条记录 （长度104）
struct resp_query_account_buying_power
{
	UInt1		m_nMsgID ;				///<	81	
	UInt4		m_nUserId;				///<	用户Id
	AccountID	m_sAccount	;			///<	帐户ID
	UInt4		m_nRequestId;			///<	查询请求ID，返回同样的值
	UInt4		m_Previlege ;			///<	操作权限，目前暂不支持
	double		m_InitialBp ;			///<	初始权益
	double		m_AvailableBp	;		///<	总可用资金
	double		m_dMargin ; 			///<	所有仓位占用的保证金
	double		m_dFrozenMargin ;		///<	所有挂单冻结的保证金
	double		m_dCommissionFee  ;		///<	已扣除的手续费总金额
	double		m_dFrozenCommission ;	///<	挂单冻结的总手续费金额
	double		m_dOvnInitMargin;		///<	初始昨仓保证金
	double		m_dTotalLiquidPL;		///<	总平仓盈亏
	double		m_dTotalMarketPL;		///<	总持仓盈亏
	UInt1		m_nReserved[3];			///<	预留域
};

// 交易所连接查询响应 （长度272）
struct	resp_query_exechange_connect_session
{
	UInt1		m_nMsgID;					//< 消息类型	     84
	UInt4		m_nUserID;					///< 用户编号
	UInt1		m_nExchangeId;				///< 交易所ID   102 中金  103 上期
	UInt4		m_nReqId;					///< 请求ID
	UInt1		m_nLastRecordFlag;			///< 0表示常规记录	1表示结束标志，本条记录的实际内容没有意义
	UInt1		m_nReserved[5];				///< 预留域
	UInt1		m_nSessionCount;			///< 交易所连接席位个数
	UInt1		m_nSessionId[255];			///< 交易所连接席位ID	
};


// 交易所状态变化通知 （长度8）
struct	resp_exechange_connect_session_status_change
{
	UInt1		m_nMsgID;					//< 消息类型	     85
	UInt1		m_nSessionStatus;			///< 状态：0：断开	1：连接，可以下单
	UInt1		m_nSessionId;				///< 交易所连接席位ID	
	UInt1		m_nReserved[5];				///< 预留域
};

// 合约状态查询响应 （长度64）
struct resp_query_instrument_status
{
	UInt1		m_nMsgID;					///< 消息类型	     87
	UInt4		m_nUserID;					///< 用户编号
	UInt4		m_nReqId;					///< 请求ID
	UInt1		m_nExechangeId;				///< 交易所ID	
	char		m_chSymbol[31];				///< 合约编码
	char		m_chSymbolStatus;			///< 合约状态 ‘0’: 开盘前‘1’: 非交易‘2’: 连续交易‘3’: 集合竞价报单‘4’: 集合竞价价格平衡‘5’: 集合竞价撮合‘6’: 收盘
	UInt4		m_nTradSegmenSN;			///< 交易时间段
	char		m_chEnterTimep[9];			///< 进入本状态时间
	char		m_chEnterReason;			///< ‘1’: 自动切换‘2’: 手动切换‘3’: 熔断‘4’: 熔断手动
	UInt1		m_nLastRecordFlag;			///< = 0表示是常规记录，记录体内容有效	=1 表示是最后一条记录，记录体内容无效
	UInt1		m_nReserved[7];				///< 预留域

};

// 合约状态变化通知 （长度48）
struct	resp_instrument_status_change
{
	UInt1		m_nMsgID;					///< 消息类型	     86
	UInt1		m_nExechangeId;				///< 交易所ID
	char		m_chSymbol[31];				///< 合约编码
	char		m_chSymbolStatus;			///< 合约状态 
	UInt4		m_nTradSegmenSN;			///< 交易时间段
	char		m_chEnterTimep[9];			///< 进入本状态时间
	char		m_chEnterReason;			///< ‘1’: 自动切换‘2’: 手动切换‘3’: 熔断‘4’: 熔断手动
};

/// 订单被REM系统接受（长度112）
struct resp_order_accept
{
	UInt1		m_nMsgID;							///<	消息类型	10
	UInt4		m_nUserID;							///<	用户编号
	UInt1		m_nSendToUserMsgType;				///<	发送给客户的消息 10
	UInt8		m_nTimeStamp;						///<	时间戳(晚上零时起的纳秒数)
	UInt4		m_nOrderToken;						///<	原来单子的编号
	UInt1		m_nOrderType;						///<	单子类型 1=Limit 限价单
	UInt1		m_nSide;							///<	买卖方向
	UInt1		m_Reserved1;						///<	保留
	UInt4		m_nQuantity;						///<	数量
	UInt1		m_nSecType;							///<	3=期货
	Symbol		m_sSymbol ;							///<	合约代码
	UInt1		m_Reserved2[9];						///<	保留
	Price6		m_dPrice;							///<	价格
	UInt1		m_Reserved3[6];						///<	保留
	UInt4		m_nTif;								///<	单子存活时间
	UInt1		m_Reserved4[2];						///<	保留
	AccountID	m_sAccount ;						///<	资金帐号
	UInt1		m_nHedgeFlag;						///<	投机套利标志，1=套利；2=投机；3=套保
	UInt1		m_nExchengeID;						///<	102=中金所	103=上期所	104=大商所	105=郑商所
	UInt1		m_Reserved5;						///<	保留
	UInt4		m_nMinQuantity;						///<	最小成交数量  0：任何数量
	UInt1		m_Reserved6[8];						///<	保留
	UInt8		m_custom_field;						///<	保留3
	UInt8		m_nOrderReferenceNumber;			///<	盛立系统产生的单子号
	UInt1		m_nState;							///<	单子状态
	UInt1		m_nReserved[3];						///<	预留域
};


/// 订单被拒绝（长度40）
struct resp_order_reject
{
	UInt1	m_nMsgID;							//<消息类型		11
	UInt4	m_nUserID;							///<用户编号
	UInt1	m_nSendToUserMsgType;				///<发送给客户的消息 11
	UInt8	m_nTimeStamp;						///<时间戳(晚上零时起的纳秒数)
	UInt4	m_nOrderToken;						///<原来单子的编号
	UInt1	m_nRejectBy;						///<被谁拒绝  1=盛立  (文档中定义了2但又删除了)
	UInt1	m_nRejectReason;					///<被拒绝原因
	UInt4	m_nGrammerResult;					///<语法检查结果
	UInt1	m_nRiskResult[12];					///<风控检查结果
	UInt1	m_nReserved[4];						 ///< 预留域
};

/// 收到成交消息 （长度88）
struct resp_order_execution
{
	UInt1			m_nMsgID;						///<	消息类型	
	UInt4			m_nUserID;						///<	用户编号
	UInt1			m_nSendToUserMsgType;			///<	发送给客户的消息 
	UInt8			m_nTimeStamp;					///<	时间戳(晚上零时起的纳秒数)
	UInt4			m_nOrderToken;					///<	原来单子的编号
	UInt8			m_nOrderReferenceNumber;		///<	盛立系统产生的单子号
	UInt4			m_nFillQty;						///<	单子成交量
	Price6			m_dExecutedPrice;				///<	单子的成交价
	UInt1			m_nLiquidityFlag;				///<	成交标志
	UInt8			m_nExecutionNumber;				///<	单子成交编号
	AccountID		m_sAccount ;					///<	资金帐号
	ExchangeExecId	m_sExchExecId;					///<	交易所成交号
	UInt1			m_nReserved[3];					///<	预留域

};

/// 订单撤销完成 （长度48）
struct resp_order_cxled
{
	UInt1		m_nMsgID;						//<消息类型		
	UInt4		m_nUserID;						///<用户编号
	UInt1		m_nSendToUserMsgType;			///<发送给客户的消息 
	UInt8		m_nTimeStamp;					///<时间戳(晚上零时起的纳秒数)
	UInt4		m_nOrderToken;					///<原来单子的编号
	UInt8		m_nOrderReferenceNumber;		///<盛立系统产生的单子号
	UInt4		m_nDecrement;					///<这次信息所取消息的单子量
	UInt1		m_nReason;						///<订单被撤消息的原因
	AccountID	m_sAccount ;					///<资金帐号
	UInt1		m_nReserved[1];					///< 预留域

};

/// 被动订单, （长度128）
struct recv_post_order_received
{
	UInt1			m_nMsgID;						///<	消息类型		17
	UInt4			m_nUserID;						///<	用户编号
	UInt1			m_nSendToUserMsgType;			///<	发送给客户的消息 17
	UInt8			m_nTimeStamp;					///<	时间戳(晚上零时起的纳秒数)
	UInt8			m_nOrderReferenceNumber;		///<	盛立系统产生的单子号
	UInt4			m_nOrderToken;					///<	原来单子的编号
	UInt1			m_nOrderType;					///<	单子类型 1=Limit 限价单
	UInt1			m_nSide;						///<	买卖方向
	UInt1			m_Reserved1;					///<	保留
	UInt4			m_nQuantity;					///<	数量
	UInt1			m_nSecType;						///<	3=期货
	Symbol			m_sSymbol ;						///<	合约代码
	Price6			m_dPrice;						///<	价格
	UInt1			m_Reserved2[6];					///<	保留
	UInt4			m_nTif;							///<	单子存活时间
	UInt1			m_Reserved3[2];					///<	保留
	AccountID		m_sAccount ;					///<	资金帐号
	UInt1			m_nHedgeFlag;					///<	投机套利标志，1=套利；2=投机；3=套保
	UInt1			m_nExchengeID;					///<	102=中金所	103=上期所	104=大商所	105=郑商所
	UInt1			m_Reserved4;					///<	保留
	UInt4			m_nMinQuantity;					///<	最小成交数量
	UInt1			m_Reserved5[16];				///<	保留	
	UInt1			m_nState;						///<	单子状态
	ExchangeOrderId	m_chExechangeOrdID;				///<    交易所单号
	UInt1			m_nReserved6[4];				///<	保留	
};

/// 被动成交, （长度88）
struct recv_post_order_execution_received
{
	UInt1			m_nMsgID;					///<消息类型		18
	UInt4			m_nUserID;					///<用户编号
	UInt1			m_nSendToUserMsgType;		///<发送给客户的消息 18
	UInt8			m_nTimeStamp;				///<时间戳(晚上零时起的纳秒数)
	UInt8			m_nOrderReferenceNumber;	///<盛立系统产生的单子号
	UInt4			m_nFillQty;					///<单子成交量
	Price6			m_dFillPrice;				///<单子的成交价
	UInt1			m_nLiquidityFlag;			///<成交标志
	UInt8			m_nExecID;					///<单子成交编号
	AccountID		m_sAccount ;				///<资金帐号
	ExchangeExecId	m_chExechangeOrdID;			///<交易所成交号
	UInt1			m_nReserved[7];				///<保留

};


///撤单拒绝 （长度48）
struct resp_cxl_reject
{
	UInt1		m_nMsgID;						///< 消息类型	174
	UInt8		m_nOrderReferenceNumber;		///< 盛立系统产生的单子号，和盛立交流时可用该号
	UInt1		m_Reserved1[4];					///< 保留
	UInt4		m_nRejectReason;				///< cancel拒绝的理由，客户端不必理会改值
	AccountID	m_sAccount ;					///< 资金帐号
	UInt4		m_nCxlUserID;					///< 发起撤单请求的用户编号
	UInt1		m_nReserved2[3];				///< 预留域
	UInt4		m_nUserID;						///< 原来单子的用户ID
	UInt4		m_nOrderToken;					///< 原来单子的客户端编号
};


// 交易所接受通知 （长度72）
struct	resp_exchange_order_accept
{
	UInt1			m_nMsgID;						///< 消息类型	180
	UInt1			m_nReserved[7];					///< 预留域
	UInt8			m_nMarketTimeStamp;				///< 市场时间信息
	UInt8			m_nOrderReferenceNumber;		///< 盛立系统的订单编号
	ExchangeOrderId	m_chExechangeOrdID;				///< 市场订单号
	AccountID		m_sAccount ;					///< 资金帐号
	UInt4			m_nUserID;						///< 单子的用户ID
	UInt4			m_nOrderToken;					///< 单子的客户端编号
};


//  交易所拒绝通知 （长度136）
struct	resp_exchange_order_reject
{
	UInt1		m_nMsgID;						///< 消息类型	181
	UInt1		m_nReserved[7];					///< 预留域
	UInt8		m_nMarketTimeStamp;				///< 市场时间信息
	UInt8		m_nOrderReferenceNumber;		///< 盛立系统的订单编号
	char		m_chReasonText[88];				///< 交易所接口中获取的错误字符串
	AccountID	m_sAccount ;					///< 资金帐号	
	UInt4		m_nUserID;						///< 单子的用户ID
	UInt4		m_nOrderToken;					///< 单子的客户端编号
};

#pragma pack(pop)

