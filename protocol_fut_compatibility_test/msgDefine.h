#pragma pack(push, 1)

#include <assert.h>
#include <string.h>

#define EES_PROTOCOL_VERSION		15

typedef unsigned char			UInt1 ;
typedef unsigned short			UInt2 ;
typedef unsigned int			UInt4 ;
typedef unsigned long long		UInt8 ;	

/// 4λ���ȵļ۸�
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

/// 6λ���ȵļ۸�
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


typedef char	AccountID[16]	;		///< �ʻ�ID
typedef char	Symbol[8]	;			///< ��Լ
typedef	char	SymbolName[21];			///< ��Լ����
typedef char	ProductID[32] ;			///< product ID
typedef char	UserName[33];			///< �û���¼��
typedef char	UserPwd[33] ;			///< �û���¼����
typedef char	ExchangeOrderId[24];	///< ����������
typedef char	ExchangeExecId[24];		///< �������ɽ���

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

/// ����������浱ǰ�İ汾�� ������8��
struct req_report_version
{
	req_report_version()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_report_version_msg_id ; 
		m_nVersion = EES_PROTOCOL_VERSION ;
	}
	UInt1	m_nMsgID ;		///< 80=���浱ǰ�İ汾�� 
	UInt4	m_nVersion ;	///< �ͻ���Э��汾�ţ��������յ��������ƥ���������Ͽ�����
	UInt1	m_nReserved[3]; ///< Ԥ����
};

/// ���׵�¼��Ϣ
struct req_logon
{
	req_logon()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_logon_msg_id ;
		m_nVersion = EES_PROTOCOL_VERSION ;		
		m_nReserved[0] = 1; 
	}

	UInt1		m_nMsgID ;			///<	70=��¼����
	UInt4		m_nVersion ;		///<	Э��汾�ţ����������ƥ��Ļ��������¼
	UserName	m_sLogonName ;		///<	��¼������¼�ɹ���᷵�������͵�UserId��֮����Ҫ����UserId���н���
	UserPwd		m_sUserPwd ;		///<	MD5���ܺ������
	UInt1		m_nReserved[1];		///< Ԥ����
	char		m_sClientMac[18];	///�ͻ���MAC��ַ,��52:54:00:8F:FE:D6�����ĸ�ʽ�ϴ����м��ð�Ÿĳ�-��Ҳ��
	char		m_sClientProdInfo[30];	//�ͻ��˱�ʶ����"REMClient"
	short		m_udpPort;
	char		m_nReserved2[134];		//����
};

/// ��ѯ��¼��Ϣ
struct req_query_logon
{
	req_query_logon()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_logon_msg_id ; 
	}

	UInt1		m_nMsgID ;		///<	68 = ��ѯͨ����¼����
	char		m_UUID[33];		///<	����ͨ����¼������Ϣ
	UInt4		m_userID;
	char		m_nReserved[218];

};

/// ��ѯ�û�������ʻ� ������8��
struct req_query_user_link_account
{
	req_query_user_link_account()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_user_link_account_msg_id ;		
	}

	UInt1	m_nMsgID ;			///< 71=�˻��б��ѯ
	UInt4	m_nUserId ;			///< ��¼����Ӧ���û���
	UInt1	m_nReserved[3];		///< Ԥ����
} ;

/// ��ѯ��Լ�б� ������8��
struct req_query_symbol
{
	req_query_symbol()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_symbol_msg_id ;
	}
	UInt1	m_nMsgID ;			///<72=��Լ�б��ѯ	
	UInt4	m_nUserId;			///<�û�Id
	UInt1	m_nReserved[3];		///<Ԥ����
};

/// �ʻ���Լ��֤���ѯ ������24��
struct req_query_account_margin
{
	req_query_account_margin()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_account_margin_msg_id ;		
	}

	UInt1		m_nMsgID ;			///< 77=��֤���ѯ
	UInt4		m_nUserId ;			///< ��¼����Ӧ���û���
	AccountID	m_sAccount ;		///< �˻�ID
	UInt1		m_nReserved[3];		///< Ԥ����
};

/// �ʻ���Լ���ʲ�ѯ ������24��
struct req_query_account_fee
{
	req_query_account_fee()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_account_fee_msg_id ;		
	}

	UInt1		m_nMsgID ;		///< 78=���ʲ�ѯ
	UInt4		m_nUserId ;		///< ��¼����Ӧ���û���
	AccountID	m_sAccount ;	///< �˻�ID
	UInt1		m_nReserved[3]; ///< Ԥ����
};

/// ��ѯ�ʻ���λ ������32��
struct req_query_account_position
{
	req_query_account_position()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_account_position_msg_id ;		
	}
	UInt1		m_nMsgID ;		///<	73=��λ��ѯ
	UInt4		m_nUserID ;		///< 	��¼�û���ID
	AccountID	m_sAccount ;	///< 	�˻�ID
	UInt4		m_nRequestId;	///<	��ѯ����ID�����1��ʼ��ţ���Ӧ�з���ͬ����ֵ��
	UInt1		m_nReserved[7]; ///<	Ԥ����
};

/// �ع��ʻ��Ķ��� ������24��
struct req_rewind_account_order
{
	req_rewind_account_order()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_rewind_account_order_msg_id ;
	}
	UInt1		m_nMsgID ;	///<	74=������ѯ
	UInt4		m_nUserID ;	///< 	��¼�û���ID
	AccountID	m_sAccount ;		///< 	�˻�ID
	UInt1		m_nReserved[3]; ///< Ԥ����
};

// �����ѯ�ͻ��ɽ� ������24��
struct req_rewind_account_exeution
{
	req_rewind_account_exeution()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_rewind_account_exeution_msg_id ;
	}
	UInt1		m_nMsgID ;			///<	75=�ɽ���ѯ
	UInt4		m_nUserID ;			///<	��¼�û���ID
	AccountID	m_sAccount ;		///<	�˻�ID
	UInt1		m_nReserved[3];		///<	Ԥ����
};

// �ʻ��ʽ������ѯ ������32��
struct req_query_account_buying_power
{
	req_query_account_buying_power()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_query_account_buying_power_msg_id ;
	}
	UInt1		m_nMsgID ;			///<	81=BP��ѯ
	UInt4		m_nUserID ;			///<	��¼�û���ID
	AccountID	m_sAccount ;		///<	�˻�ID
	UInt4		m_nRequestId;		///<	��ѯ����ID�����1��ʼ��ţ���Ӧ�з���ͬ����ֵ��
	UInt1		m_nReserved[7];		///<	Ԥ����
};

//  ����������ϯλ��ѯ���� ������16��
struct	req_exechage_connect_session
{
	req_exechage_connect_session()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_exechage_connect_session_msg_id ;
	}
	UInt1		m_nMsgID;					//< ��Ϣ����	     84
	UInt4		m_nUserID;					///< �û����
	UInt4		m_nRequestId;				///< ����ID
	UInt1		m_nReserved[7];				///< Ԥ����
};

// ��Լ״̬��ѯ��� ������16��
struct req_instrument_status
{
	req_instrument_status()
	{
		memset(this, 0, sizeof(*this));
		m_nMsgID = req_instrument_status_msg_id ;
	}
	UInt1		m_nMsgID;					//< ��Ϣ����	     87
	UInt4		m_nUserID;					///< �û����
	UInt4		m_nRequestId;				///< ����ID
	UInt1		m_nReserved[7];				///< Ԥ����

};
/// �µ�ָ�� ������96��
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

	UInt1		m_nMsgID;						///<��Ϣ����	10
	UInt4		m_nUserID;						///<�͵��ӵ��û����
	Symbol		m_sSymbol;						///<��Լ����	
	AccountID	m_sAccount;						///<�ʽ��ʺ�
	UInt1		m_nOrderType;					///<�������� 1=Limit �޼۵�
	UInt1		m_nSide;						///<��������1=�򵥣�����  2=������ƽ��  3=�򵥣�ƽ��  4=����������
	UInt4		m_nQuantity;					///<����
	Price6		m_dPrice;						///<�۸�
	UInt1		m_nHedgeFlag;					///<Ͷ��������־��1=������2=Ͷ����3=�ױ�
	UInt1		m_mkt_session_id;				///<ָ��ϯλ�ţ�������0
	UInt1		m_session_select_mode;			///<��Ҫָ��ϯλ�ţ�������1��������0
	UInt4		m_nOrderToken;					///<�ͻ����ƵĶ������	
	UInt1		m_nCrossType;					///<����	
	UInt1		m_nSecType;						///<����Ʒ�����ͣ�3=�ڻ� 2=��Ȩ
	UInt1		m_nExpireMonthAndCallPut;		///<��Ȩ��أ�����9	
	UInt1		m_nExpireDay;					///<��Ȩ��أ�����16	
	UInt1		m_nExpireYear;					///<��Ȩ��أ�����13	
	Price6		m_dStrikePrice;					///<����	����Ȩ֧�ֺ����ô�	
	Price6		m_nReserved1;					///<����
	UInt4		m_nTif;							///<���Ӵ��ʱ�䣬99998Ϊ������Ч��0ΪFOK/FAK����
	UInt1		m_nReserved2[2];				///<����		
	UInt1		m_nExchangeId;					///<102=�н���,103=������,104=������,105=֣����
	UInt1		m_nReserved3;					///<����
	UInt4		m_nMinQuantity;					///<��С�ɽ�����  0���κ�����
	UInt1		m_nReserved4[6];				///<����	
	UInt8		m_custom_field;					///<����
	UInt1		m_nReserved5[5];				///<����
};

/// ���󳷵�ָ��
struct req_cxl_order
{
	req_cxl_order()
	{
		memset(this, 0, sizeof(*this)) ;
		m_nMsgID = req_cxl_order_msg_id ; 
	}
	UInt1		m_nMsgID;						///<��Ϣ����	11
	UInt4		m_nUserID;						///<�͵��ӵ��û����
	UInt8		m_nOrderReferenceNumber;		///<ʢ��ϵͳ�����ĵ��Ӻ�
	UInt4		m_Reserved1;					///<����
	AccountID	m_sAccount ;					///<�ʽ��˺�
	UInt1		m_nReserved2[7];				///<����
};



/// �ظ�����˰汾�� ������8��
struct resp_report_version
{
	UInt1	m_nMsgID ;		///< 80
	UInt4	m_nVersion ;	///< �ͻ���Э��汾�ţ� 
	UInt1	m_nReserved[3]; ///< Ԥ����
};

/// ��¼��Ӧ ������56��
struct resp_logon
{
	UInt1		m_nMsgID ;			///<	70
	UInt4		m_nVersion ;		///<	Э��汾�ţ����������ƥ��Ļ��������¼
	UInt4		m_nResult	;		///<	����0��ʾ��¼�ɹ�����0��ʾ��¼ʧ�ܣ�ʧ��ʱ�����᷵��UserId		0-	��¼�ɹ�		1-	�û���/�������		2-	�û������������⣬���˻��б�Ϊ�յ�
	UserName	m_sLogonID;			///<	��¼������¼�ɹ���᷵�������͵�UserId��֮����Ҫ����UserId���н���
	UInt4		m_nUserId ;			///<	��¼����Ӧ���û�ID
	UInt4		m_nTradingDate;		///<	�����գ���ʽΪyyyyMMdd��int��ֵ
	UInt4		m_nMaxOrderToken ;	///<	���û��µ�Ŀǰ����Token
	UInt1		m_nReserved[2];		///<	Ԥ����
};

/// ��ѯͨ����¼��Ӧ ������256��
struct resp_query_logon
{
	UInt1		m_nMsgID;				///<	68
	UInt4		m_result;				///<	��¼��� 0 = �ɹ�
	UInt4		m_orderFCParamCount;	///<    �µ����ز���
	UInt4		m_orderFCParamInterval; ///<	΢����
	UInt4		m_cancelFCParamCount;	///<    �������ز���
	UInt4		m_cancelFCParamInterval; ///<	΢����
	UInt1		m_nReserved[235];
};

/// ��ѯ�û������ʻ���Ӧ ������80��
struct resp_query_user_link_account
{
	UInt1		m_nMsgID ;				///<	71
	UInt1		m_nLastRecordFlag;		///<	0����ʾ�����¼	1����ʾ������־��������¼��ʵ������û������
	UInt4		m_nUserId;				///< 	�û�Id
	AccountID	m_sAccount	;			///<	
	UInt4		m_nPrevilege ;			///< 	����Ȩ�ޣ�ĿǰӲ���ݲ�֧�֣�Ҳ����˵������ȫ����Ȩ��	99����ȫ����	1��ֻ��	2��ֻƽ��
	double		m_dInitialBp ;			///<	��ʼȨ��
	double		m_dAvailableBp	;		///< 	�ܿ����ʽ�
	double		m_dMargin ; 			///< 	���в�λռ�õı�֤��
	double		m_dFrozenMargin ;		///< 	���йҵ�����ı�֤��
	double		m_dCommissionFee  ;		///< 	�ѿ۳����������ܽ��
	double		m_dFrozenCommission ;	///<	�ҵ�������������ѽ��
	UInt1		m_nReserved[6];			 ///< Ԥ����
} ;

///��ѯ��Լ�б��Ӧ ������120��
struct resp_query_symbol 
{
	UInt1		m_nMsgID ;					///<	72
	UInt1		m_nLastRecordFlag;			///<	0-	��ʾ�����¼		1-	��ʾ������־��������¼��ʵ������û������
	UInt1		m_nSecType	;				///<	3=Future��Ŀǰֻ�����
	Symbol		m_sSymbol	;				///<	��Լ����/��Ʊ����
	SymbolName  m_sSymbolName;				///<	��Լ����
	UInt1		m_nExchangeID ;				///<	102=�н���		103=������		104=������		105=֣����
	ProductID	m_sProdID ;					///<	��Ʒ����
	UInt1		m_nProductClass ;			///<	Ŀǰֻ����1
	UInt2		m_nDeliveryYear ;			///<	������
	UInt1		m_nDeliveryMonth ;			///<	������
	UInt4		m_nMaxMarketOrderVolume ;	///<	�м۵�����µ���
	UInt4		m_nMinMarketOrderVolume	;	///<	�м۵���С�µ���
	UInt4		m_nMaxLimitOrderVolume ;	///<	�޼۵�����µ���
	UInt4		m_nMinLimitOrderVolume ;	///<	�޼۵���С�µ���
	UInt4		m_nVolumeMultiple;			///<	��Լ����
	double		m_dPriceTick;				///<	��С�䶯��λ*1000000
	UInt4		m_nCreateDate;				///<	��������
	UInt4		m_nOpenDate ;				///<	��������
	UInt4		m_nExpireDate ;				///<	������
	UInt4		m_nStartDelivDate ;			///<	��ʼ������
	UInt4		m_nEndDelivDate;			///<	����������
	UInt1		m_nInstLifePhase ;			///<	��Լ��������״̬		0=δ����		1=����		2=ͣ��		3=����
	UInt1		m_nIsTrading;				///<	��ǰ�Ƿ���		0=δ����		1=����
	UInt1		m_nReserved[1];				///<	����
} ;

/// ��ѯ�ʻ���֤���Ӧ ������80��
struct resp_query_account_margin
{
	UInt1		m_nMsgID ;				///<	77
	UInt1		m_nLastRecordFlag;		///<	0-	��ʾ�����¼		1-	��ʾ������־��������¼��ʵ������û������
	AccountID	m_sAccount;				///<	�˻�ID
	UInt1		m_nHedgeFlag ;			///<	Ͷ��������־��1=������2=Ͷ����3=�ױ�
	Symbol		m_sSymbol ;				///<	��Լ����
	UInt1		m_nExchangeID ; 		///<	102=�н���		103=������		104=������		105=֣����
	ProductID	m_sProdID;				///< 	4	Alpha	��Ʒ����
	double		m_dLongMarginRatio ;	///<		
	double		m_dShortMarginRatio ;	///<	Ŀǰ�ò���
	char        m_Reserved[4];

};

/// ��ѯ�ʻ����ʻ�Ӧ ������112��
struct resp_query_account_fee
{
	UInt1		m_nMsgID ;							///<	Message Type	0	1	Int1	78
	UInt1		m_nLastRecordFlag;					///<	0-	��ʾ�����¼		1-	��ʾ������־��������¼��ʵ������û������
	AccountID	m_sAccount;							///<	�˻�ID
	UInt1		m_nHedgeFlag ;						///<	Ͷ��������־��1=������2=Ͷ����3=�ױ�
	Symbol		m_sSymbol ;							///<	��Լ����/��Ʊ����
	UInt1		m_nExchangeID	;					///<	102=�н���		103=������		104=������		105=֣����
	ProductID	m_sProdID ;							///<	��Ʒ����
	double		m_dOpenRatioByMoney ;				///<	�����������ʣ����ս��
	double		m_dOpenRatioByVolume ;				///<	�����������ʣ���������
	double		m_dCloseYesterdayRatioByMoney;		///<	ƽ���������ʣ����ս��
	double		m_dCloseYesterdayRatioByVolume;		///<	ƽ���������ʣ���������
	double		m_dCloseTodayRatioByMoney;			///<	ƽ���������ʣ����ս��
	double		m_dCloseTodayRatioByVolume	;		///<	ƽ���������ʣ���������
	UInt1		m_nPosDir;							///<	1=��ͷ��5=��ͷ
	UInt1		m_nReserved[3];						///<	Ԥ����
} ;

/// ��ѯ�ʻ���λ��Ӧ ������128��
struct resp_query_account_position
{
	UInt1		m_nMsgID ;			///< 73
	UInt1		m_nLastRecordFlag;	///< 0-	��ʾ�����¼		1-	��ʾ������־��������¼��ʵ������û������
	UInt4		m_nUserId ;			///< �û�Id
	AccountID	m_sAccount;			///< �ʽ��˺�
	UInt4		m_nRequestId;		///< ��ѯ����ID������ͬ����ֵ��0��ʾ��������
	Symbol		m_sSymbol ;			///< ��Լ����/��Ʊ����
	UInt1		m_nPosiDirection;	///< ��շ���	1����ͷ	5����ͷ
	UInt4		m_nInitOvnQty;		///< ��ҹ�ֳ�ʼ���������ֵ����仯������ͨ��HelpDesk�ֹ��޸�
	double		m_dInitOvnMargin;	///< ��ʼ��ҹ�ֱ�֤��
	UInt4		m_nOvnQty;			///< ��ǰ��ҹ������������Ϊ0
	UInt4		m_nFrozenOvnQty;	///< ������������
	UInt4		m_nTodayQty;		///< ��ǰ�������������Ϊ0
	UInt4		m_nFrozenTodayQty;	///< ����Ľ������
	double		m_dOvnMargin;		///< ��ҹ��ռ�ñ�֤��
	double		m_dTodayMargin;		///< ���ռ�õı�֤��
	double		m_dTotalFee;		///< ��������
	double		m_dOvnLiquidPL;		///< ���ƽ��ӯ��
	double		m_dTodayLiquidPL;	///< ���ƽ��ӯ��
	double		m_dOvnMarketPL;		///< ��ֲֳ�ӯ��
	double		m_dTodayMarketPL;	///< ��ֲֳ�ӯ��
	double		m_dPosCost;			///< �ֲֳɱ�
	UInt1		m_nHedgeFlag ;		///< Ͷ��������־��1=������2=Ͷ����3=�ױ�
};

/// �����ع���Ӧ ������160��
struct resp_rewind_account_order
{
	resp_rewind_account_order()
	{
	}

	UInt1			m_nMsgID ;						///<	74
	UInt4			m_nUserId ;						///<	ԭ�����ӵ��û�����Ӧ��LoginID��
	UInt1			m_OrigMsgID	;					///<	74- ���ӽ�����Ϣ
	UInt8			m_nTimeStamp ;					///<	ʱ�䣬��������ʱ�����������
	UInt4			m_nOrderToken ;					///<	ԭ�����ӵ�token 
	UInt1			m_nOrderType ;					///<	1=limit �޼۵���	
	UInt1			m_nSide ;						///<	1 = �򵥣�����	2 = ������ƽ��	3= �򵥣�ƽ��	4 = ����������	5= �򵥣�ƽ��	6= ������ƽ��
	UInt1			m_Reserved1;					///<	����
	UInt4			m_nQuantity ;					///<	����
	UInt1			m_InstrumentType ;				///<	3���ڻ�
	Symbol			m_sSymbol;						///<	��Լ����
	UInt1			m_Reserved2[11];				///<	����	
	double			m_dPrice;						///<	�۸�
	UInt1			m_Reserved3[8];					///<	����	
	UInt4			m_nTif ;						///<	���ӵĴ��ʱ�������ֵΪ	0=IOC��immediate or cancel,���ɽ�����ȡ����	99998= market hour (�г�ʱ��)
	UInt1			m_Reserved4[2];					///<	����	
	AccountID		m_sAccount;						///<	�ʽ��ʺ�.
	UInt1			m_nHedgeFlag ;					///<	Ͷ��������־��1=������2=Ͷ����3=�ױ�
	UInt1			m_nExchengeID ;					///<	102=�н���	103=������	104=������	105=֣����
	UInt1			m_Reserved5;					///<	����	
	UInt4			m_nMinimumQuantity ;			///<	0���κ�������	ȫ���ɽ���	��С�ɽ������� �������ֵ���ڴ��ڵ�����������ΪAON��All-or-none)���ӡ�
	UInt1			m_Reserved6[8];					///<	����	
	UInt8			m_custom_field;					///<	
	UInt8			m_nOrderReferenceNumber ;		///<	ʢ��ϵͳ�����ĵ��Ӻţ���ʢ������ʱ���øúš�
	UInt1			m_nOrderState ;					///<	
	UInt1			m_nCxlRjtFlag	;				///<	0���ҵ�����ȫ�ɽ�	1���Ѿ�����	2�����ܾ�	3���ѹ��ڣ������Ѿ�����
	UInt8			m_nCloseTime ;					///<	��������Ѿ����������߾ܾ������ֵ��Ч
	UInt4			m_nFilledQty ;					///<	�ɽ�����
	ExchangeOrderId	m_chExechangeOrdID;				///<    ����������ID
	UInt8			m_nExchangeTranTimeStamp;		///<    �г�����/�ܾ���ʱ���

};

/// �ɽ��ع��Ļظ� ������88��
struct resp_rewind_order_execution
{
	UInt1			m_nMsgID ;					///<	75
	UInt4			m_nUserId ;					///<	ԭ�����ӵ��û�����Ӧ��LoginID��
	AccountID		m_sAccount ;				///<	�˻�ID
	UInt1			m_nOrigID ;					///<	75=���ӳɽ���Ϣ
	UInt8			m_nTimeStamp ;				///<	ʱ�䣬��������ʱ�����������
	UInt4			m_nOrderToken ;				///<	ԭ�����ӵ����token
	UInt8			m_nOrderReferenceNumber ;	///<	ʢ��ϵͳ�����ĵ��Ӻţ���ʢ������ʱ���øúš�
	UInt4			m_nFillQty ;				///<	���ӳɽ���
	double			m_dFillPrice	;			///<	���ӳɽ���
	UInt1			m_nLiquidityFlag ;			///<	�ɽ���־,Ŀǰ����Ҫ
	UInt8			m_nExecID ;					///<	���ӳɽ���
	ExchangeExecId	m_sExchExecId;				///<	�������ɽ���
	UInt1			m_nReserved[1];				///<	Ԥ����
};

/// ��һ���˻������ж����ͳɽ���rewind��ɺ����Ϣ ������24��
struct resp_rewind_account_order_finish
{
	UInt1			m_nMsgID	;	///< 76
	AccountID		m_sAccount ;	///< �˻�ID
	UInt1			m_nReserved[7]; ///< Ԥ����
};

/// ��һ���˻������гɽ���rewind��ɺ����Ϣ ������24��
struct resp_rewind_account_exe_finish
{
	UInt1			m_nMsgID	;	///< 79
	AccountID		m_sAccount ;	///< �˻�ID
	UInt1			m_nReserved[7]; ///< Ԥ����
};

//һ��ֻ��һ���˻�������һ����¼ ������104��
struct resp_query_account_buying_power
{
	UInt1		m_nMsgID ;				///<	81	
	UInt4		m_nUserId;				///<	�û�Id
	AccountID	m_sAccount	;			///<	�ʻ�ID
	UInt4		m_nRequestId;			///<	��ѯ����ID������ͬ����ֵ
	UInt4		m_Previlege ;			///<	����Ȩ�ޣ�Ŀǰ�ݲ�֧��
	double		m_InitialBp ;			///<	��ʼȨ��
	double		m_AvailableBp	;		///<	�ܿ����ʽ�
	double		m_dMargin ; 			///<	���в�λռ�õı�֤��
	double		m_dFrozenMargin ;		///<	���йҵ�����ı�֤��
	double		m_dCommissionFee  ;		///<	�ѿ۳����������ܽ��
	double		m_dFrozenCommission ;	///<	�ҵ�������������ѽ��
	double		m_dOvnInitMargin;		///<	��ʼ��ֱ�֤��
	double		m_dTotalLiquidPL;		///<	��ƽ��ӯ��
	double		m_dTotalMarketPL;		///<	�ֲܳ�ӯ��
	UInt1		m_nReserved[3];			///<	Ԥ����
};

// ���������Ӳ�ѯ��Ӧ ������272��
struct	resp_query_exechange_connect_session
{
	UInt1		m_nMsgID;					//< ��Ϣ����	     84
	UInt4		m_nUserID;					///< �û����
	UInt1		m_nExchangeId;				///< ������ID   102 �н�  103 ����
	UInt4		m_nReqId;					///< ����ID
	UInt1		m_nLastRecordFlag;			///< 0��ʾ�����¼	1��ʾ������־��������¼��ʵ������û������
	UInt1		m_nReserved[5];				///< Ԥ����
	UInt1		m_nSessionCount;			///< ����������ϯλ����
	UInt1		m_nSessionId[255];			///< ����������ϯλID	
};


// ������״̬�仯֪ͨ ������8��
struct	resp_exechange_connect_session_status_change
{
	UInt1		m_nMsgID;					//< ��Ϣ����	     85
	UInt1		m_nSessionStatus;			///< ״̬��0���Ͽ�	1�����ӣ������µ�
	UInt1		m_nSessionId;				///< ����������ϯλID	
	UInt1		m_nReserved[5];				///< Ԥ����
};

// ��Լ״̬��ѯ��Ӧ ������64��
struct resp_query_instrument_status
{
	UInt1		m_nMsgID;					///< ��Ϣ����	     87
	UInt4		m_nUserID;					///< �û����
	UInt4		m_nReqId;					///< ����ID
	UInt1		m_nExechangeId;				///< ������ID	
	char		m_chSymbol[31];				///< ��Լ����
	char		m_chSymbolStatus;			///< ��Լ״̬ ��0��: ����ǰ��1��: �ǽ��ס�2��: �������ס�3��: ���Ͼ��۱�����4��: ���Ͼ��ۼ۸�ƽ�⡮5��: ���Ͼ��۴�ϡ�6��: ����
	UInt4		m_nTradSegmenSN;			///< ����ʱ���
	char		m_chEnterTimep[9];			///< ���뱾״̬ʱ��
	char		m_chEnterReason;			///< ��1��: �Զ��л���2��: �ֶ��л���3��: �۶ϡ�4��: �۶��ֶ�
	UInt1		m_nLastRecordFlag;			///< = 0��ʾ�ǳ����¼����¼��������Ч	=1 ��ʾ�����һ����¼����¼��������Ч
	UInt1		m_nReserved[7];				///< Ԥ����

};

// ��Լ״̬�仯֪ͨ ������48��
struct	resp_instrument_status_change
{
	UInt1		m_nMsgID;					///< ��Ϣ����	     86
	UInt1		m_nExechangeId;				///< ������ID
	char		m_chSymbol[31];				///< ��Լ����
	char		m_chSymbolStatus;			///< ��Լ״̬ 
	UInt4		m_nTradSegmenSN;			///< ����ʱ���
	char		m_chEnterTimep[9];			///< ���뱾״̬ʱ��
	char		m_chEnterReason;			///< ��1��: �Զ��л���2��: �ֶ��л���3��: �۶ϡ�4��: �۶��ֶ�
};

/// ������REMϵͳ���ܣ�����112��
struct resp_order_accept
{
	UInt1		m_nMsgID;							///<	��Ϣ����	10
	UInt4		m_nUserID;							///<	�û����
	UInt1		m_nSendToUserMsgType;				///<	���͸��ͻ�����Ϣ 10
	UInt8		m_nTimeStamp;						///<	ʱ���(������ʱ���������)
	UInt4		m_nOrderToken;						///<	ԭ�����ӵı��
	UInt1		m_nOrderType;						///<	�������� 1=Limit �޼۵�
	UInt1		m_nSide;							///<	��������
	UInt1		m_Reserved1;						///<	����
	UInt4		m_nQuantity;						///<	����
	UInt1		m_nSecType;							///<	3=�ڻ�
	Symbol		m_sSymbol ;							///<	��Լ����
	UInt1		m_Reserved2[9];						///<	����
	Price6		m_dPrice;							///<	�۸�
	UInt1		m_Reserved3[6];						///<	����
	UInt4		m_nTif;								///<	���Ӵ��ʱ��
	UInt1		m_Reserved4[2];						///<	����
	AccountID	m_sAccount ;						///<	�ʽ��ʺ�
	UInt1		m_nHedgeFlag;						///<	Ͷ��������־��1=������2=Ͷ����3=�ױ�
	UInt1		m_nExchengeID;						///<	102=�н���	103=������	104=������	105=֣����
	UInt1		m_Reserved5;						///<	����
	UInt4		m_nMinQuantity;						///<	��С�ɽ�����  0���κ�����
	UInt1		m_Reserved6[8];						///<	����
	UInt8		m_custom_field;						///<	����3
	UInt8		m_nOrderReferenceNumber;			///<	ʢ��ϵͳ�����ĵ��Ӻ�
	UInt1		m_nState;							///<	����״̬
	UInt1		m_nReserved[3];						///<	Ԥ����
};


/// �������ܾ�������40��
struct resp_order_reject
{
	UInt1	m_nMsgID;							//<��Ϣ����		11
	UInt4	m_nUserID;							///<�û����
	UInt1	m_nSendToUserMsgType;				///<���͸��ͻ�����Ϣ 11
	UInt8	m_nTimeStamp;						///<ʱ���(������ʱ���������)
	UInt4	m_nOrderToken;						///<ԭ�����ӵı��
	UInt1	m_nRejectBy;						///<��˭�ܾ�  1=ʢ��  (�ĵ��ж�����2����ɾ����)
	UInt1	m_nRejectReason;					///<���ܾ�ԭ��
	UInt4	m_nGrammerResult;					///<�﷨�����
	UInt1	m_nRiskResult[12];					///<��ؼ����
	UInt1	m_nReserved[4];						 ///< Ԥ����
};

/// �յ��ɽ���Ϣ ������88��
struct resp_order_execution
{
	UInt1			m_nMsgID;						///<	��Ϣ����	
	UInt4			m_nUserID;						///<	�û����
	UInt1			m_nSendToUserMsgType;			///<	���͸��ͻ�����Ϣ 
	UInt8			m_nTimeStamp;					///<	ʱ���(������ʱ���������)
	UInt4			m_nOrderToken;					///<	ԭ�����ӵı��
	UInt8			m_nOrderReferenceNumber;		///<	ʢ��ϵͳ�����ĵ��Ӻ�
	UInt4			m_nFillQty;						///<	���ӳɽ���
	Price6			m_dExecutedPrice;				///<	���ӵĳɽ���
	UInt1			m_nLiquidityFlag;				///<	�ɽ���־
	UInt8			m_nExecutionNumber;				///<	���ӳɽ����
	AccountID		m_sAccount ;					///<	�ʽ��ʺ�
	ExchangeExecId	m_sExchExecId;					///<	�������ɽ���
	UInt1			m_nReserved[3];					///<	Ԥ����

};

/// ����������� ������48��
struct resp_order_cxled
{
	UInt1		m_nMsgID;						//<��Ϣ����		
	UInt4		m_nUserID;						///<�û����
	UInt1		m_nSendToUserMsgType;			///<���͸��ͻ�����Ϣ 
	UInt8		m_nTimeStamp;					///<ʱ���(������ʱ���������)
	UInt4		m_nOrderToken;					///<ԭ�����ӵı��
	UInt8		m_nOrderReferenceNumber;		///<ʢ��ϵͳ�����ĵ��Ӻ�
	UInt4		m_nDecrement;					///<�����Ϣ��ȡ��Ϣ�ĵ�����
	UInt1		m_nReason;						///<����������Ϣ��ԭ��
	AccountID	m_sAccount ;					///<�ʽ��ʺ�
	UInt1		m_nReserved[1];					///< Ԥ����

};

/// ��������, ������128��
struct recv_post_order_received
{
	UInt1			m_nMsgID;						///<	��Ϣ����		17
	UInt4			m_nUserID;						///<	�û����
	UInt1			m_nSendToUserMsgType;			///<	���͸��ͻ�����Ϣ 17
	UInt8			m_nTimeStamp;					///<	ʱ���(������ʱ���������)
	UInt8			m_nOrderReferenceNumber;		///<	ʢ��ϵͳ�����ĵ��Ӻ�
	UInt4			m_nOrderToken;					///<	ԭ�����ӵı��
	UInt1			m_nOrderType;					///<	�������� 1=Limit �޼۵�
	UInt1			m_nSide;						///<	��������
	UInt1			m_Reserved1;					///<	����
	UInt4			m_nQuantity;					///<	����
	UInt1			m_nSecType;						///<	3=�ڻ�
	Symbol			m_sSymbol ;						///<	��Լ����
	Price6			m_dPrice;						///<	�۸�
	UInt1			m_Reserved2[6];					///<	����
	UInt4			m_nTif;							///<	���Ӵ��ʱ��
	UInt1			m_Reserved3[2];					///<	����
	AccountID		m_sAccount ;					///<	�ʽ��ʺ�
	UInt1			m_nHedgeFlag;					///<	Ͷ��������־��1=������2=Ͷ����3=�ױ�
	UInt1			m_nExchengeID;					///<	102=�н���	103=������	104=������	105=֣����
	UInt1			m_Reserved4;					///<	����
	UInt4			m_nMinQuantity;					///<	��С�ɽ�����
	UInt1			m_Reserved5[16];				///<	����	
	UInt1			m_nState;						///<	����״̬
	ExchangeOrderId	m_chExechangeOrdID;				///<    ����������
	UInt1			m_nReserved6[4];				///<	����	
};

/// �����ɽ�, ������88��
struct recv_post_order_execution_received
{
	UInt1			m_nMsgID;					///<��Ϣ����		18
	UInt4			m_nUserID;					///<�û����
	UInt1			m_nSendToUserMsgType;		///<���͸��ͻ�����Ϣ 18
	UInt8			m_nTimeStamp;				///<ʱ���(������ʱ���������)
	UInt8			m_nOrderReferenceNumber;	///<ʢ��ϵͳ�����ĵ��Ӻ�
	UInt4			m_nFillQty;					///<���ӳɽ���
	Price6			m_dFillPrice;				///<���ӵĳɽ���
	UInt1			m_nLiquidityFlag;			///<�ɽ���־
	UInt8			m_nExecID;					///<���ӳɽ����
	AccountID		m_sAccount ;				///<�ʽ��ʺ�
	ExchangeExecId	m_chExechangeOrdID;			///<�������ɽ���
	UInt1			m_nReserved[7];				///<����

};


///�����ܾ� ������48��
struct resp_cxl_reject
{
	UInt1		m_nMsgID;						///< ��Ϣ����	174
	UInt8		m_nOrderReferenceNumber;		///< ʢ��ϵͳ�����ĵ��Ӻţ���ʢ������ʱ���øú�
	UInt1		m_Reserved1[4];					///< ����
	UInt4		m_nRejectReason;				///< cancel�ܾ������ɣ��ͻ��˲�������ֵ
	AccountID	m_sAccount ;					///< �ʽ��ʺ�
	UInt4		m_nCxlUserID;					///< ���𳷵�������û����
	UInt1		m_nReserved2[3];				///< Ԥ����
	UInt4		m_nUserID;						///< ԭ�����ӵ��û�ID
	UInt4		m_nOrderToken;					///< ԭ�����ӵĿͻ��˱��
};


// ����������֪ͨ ������72��
struct	resp_exchange_order_accept
{
	UInt1			m_nMsgID;						///< ��Ϣ����	180
	UInt1			m_nReserved[7];					///< Ԥ����
	UInt8			m_nMarketTimeStamp;				///< �г�ʱ����Ϣ
	UInt8			m_nOrderReferenceNumber;		///< ʢ��ϵͳ�Ķ������
	ExchangeOrderId	m_chExechangeOrdID;				///< �г�������
	AccountID		m_sAccount ;					///< �ʽ��ʺ�
	UInt4			m_nUserID;						///< ���ӵ��û�ID
	UInt4			m_nOrderToken;					///< ���ӵĿͻ��˱��
};


//  �������ܾ�֪ͨ ������136��
struct	resp_exchange_order_reject
{
	UInt1		m_nMsgID;						///< ��Ϣ����	181
	UInt1		m_nReserved[7];					///< Ԥ����
	UInt8		m_nMarketTimeStamp;				///< �г�ʱ����Ϣ
	UInt8		m_nOrderReferenceNumber;		///< ʢ��ϵͳ�Ķ������
	char		m_chReasonText[88];				///< �������ӿ��л�ȡ�Ĵ����ַ���
	AccountID	m_sAccount ;					///< �ʽ��ʺ�	
	UInt4		m_nUserID;						///< ���ӵ��û�ID
	UInt4		m_nOrderToken;					///< ���ӵĿͻ��˱��
};

#pragma pack(pop)

