#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GR_LOGON				1									//��¼��Ϣ

//��¼ģʽ
#define SUB_GR_LOGON_USERID			1									//I D ��¼
#define SUB_GR_LOGON_MOBILE			2									//�ֻ���¼
#define SUB_GR_LOGON_ACCOUNTS		3									//�ʻ���¼

//��¼���
#define SUB_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GR_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GR_LOGON_FINISH			102									//��¼���

//������ʾ
#define SUB_GR_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_GR_LogonUserID
{
	//�汾��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwProcessVersion;					//���̰汾

	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szIPAdrr[512];					//���IP
	
	//�㳡��Ϣ
	BYTE                            cbPlazaType;                        //�㳡���� 
};

//�ֻ���¼
struct CMD_GR_LogonMobile
{
	//�汾��Ϣ
	WORD							wGameID;							//��Ϸ��ʶ
	DWORD							dwProcessVersion;					//���̰汾

	//��������
	WORD							wVisibleMode;						//��ͼģʽ
	WORD							wVisibleTable;						//������Ŀ

	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�ʺŵ�¼
struct CMD_GR_LogonAccounts
{
	//�汾��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwProcessVersion;					//���̰汾

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//�㳡��Ϣ
	BYTE                            cbPlazaType;                        //�㳡����
};

//��¼�ɹ�
struct CMD_GR_LogonSuccess
{
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��
};

//��¼ʧ��
struct CMD_GR_LogonFailure
{
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������ʾ
struct CMD_GR_UpdateNotify
{
	//������־
	BYTE							cbMustUpdatePlaza;					//ǿ������
	BYTE							cbMustUpdateClient;					//ǿ������
	BYTE							cbAdviceUpdateClient;				//��������

	//��ǰ�汾
	DWORD							dwCurrentPlazaVersion;				//��ǰ�汾
	DWORD							dwCurrentFrameVersion;				//��ǰ�汾
	DWORD							dwCurrentClientVersion;				//��ǰ�汾
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_CONFIG				2									//������Ϣ

#define SUB_GR_CONFIG_COLUMN		100									//�б�����
#define SUB_GR_CONFIG_SERVER		101									//��������
#define SUB_GR_CONFIG_FINISH		102									//�������

//////////////////////////////////////////////////////////////////////////////////

//�б�����
struct CMD_GR_ConfigColumn
{
	BYTE							cbColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItem[MAX_COLUMN];				//�б�����
};

//��������
struct CMD_GR_ConfigServer
{
	//��������
	WORD							wTableCount;						//������Ŀ
	WORD							wChairCount;						//������Ŀ

	//��������
	BOOL                            bAllowDistribute;                   //������� 
	WORD							wServerType;						//��������	
	DWORD							dwServerRule;						//�������
};

//////////////////////////////////////////////////////////////////////////////////
//�û�����

#define MDM_GR_USER					3									//�û���Ϣ

//�û�����
#define SUB_GR_USER_LOOKON			1									//�Թ�����
#define SUB_GR_USER_SITDOWN			2									//��������
#define SUB_GR_USER_STANDUP			3									//��������
// add by hxh 20160708
#if 1
#define SUB_GR_USER_RULE			4									//�û�����
#define SUB_GR_USER_INVITE			5									//�û�����
#define SUB_GR_USER_INVITE_REQ		6									//��������
#define SUB_GR_USER_REPULSE_SIT  	7									//�ܾ��������
#define SUB_GR_USER_KICK_USER       8                                   //�߳��û�
#define SUB_GR_USER_INFO_REQ        9                                   //�����û���Ϣ
#define SUB_GR_USER_CHAIR_REQ       10                                  //�������λ��
#define SUB_GR_USER_CHAIR_INFO_REQ  11                                  //���������û���Ϣ
#define SUB_GR_USER_WAIT_DISTRIBUTE 12									//�ȴ�����
#endif

//�û�״̬
#define SUB_GR_USER_ENTER			100									//�û�����
#define SUB_GR_USER_SCORE			101									//�û�����
#define SUB_GR_USER_STATUS			102									//�û�״̬
#define SUB_GR_USER_SPREADINFO		103									//�û���Ϣ
#define SUB_GR_REQUEST_FAILURE		104									//����ʧ��

//��������
#define SUB_GR_USER_CHAT			201									//������Ϣ
#define SUB_GR_USER_EXPRESSION		202									//������Ϣ
#define SUB_GR_WISPER_CHAT			203									//˽����Ϣ
#define SUB_GR_WISPER_EXPRESSION	204									//˽�ı���
#define SUB_GR_COLLOQUY_CHAT		205									//�Ự��Ϣ
#define SUB_GR_COLLOQUY_EXPRESSION	206									//�Ự����

//////////////////////////////////////////////////////////////////////////////////

//�Թ�����
struct CMD_GR_UserLookon
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
};

//��������
struct CMD_GR_UserSitDown
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
};

//��������
struct CMD_GR_UserStandUp
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	BYTE							cbForceLeave;						//ǿ���뿪
};

//�û�����
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserScore					UserScore;							//������Ϣ
};

//�û�״̬
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserStatus					UserStatus;							//�û�״̬
};

//�û���Ϣ
struct CMD_GR_UserSpreadInfo
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserSpreadInfo               UserSpreadInfo;                     //��չ��Ϣ 
};

//����ʧ��
struct CMD_GR_RequestFailure
{
	BYTE							cbFailureCode;						//�������
	BYTE                            cbCommandCode;                      //�������
	TCHAR							szDescribeString[256];				//������Ϣ
};

//�û�����
struct CMD_GR_C_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GR_S_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GR_C_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�����
struct CMD_GR_S_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�˽��
struct CMD_GR_C_WisperChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�˽��
struct CMD_GR_S_WisperChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//˽�ı���
struct CMD_GR_C_WisperExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//˽�ı���
struct CMD_GR_S_WisperExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û��Ự
struct CMD_GR_ColloquyChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwConversationID;					//�Ự��ʶ
	DWORD							dwTargetUserID[16];					//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define MDM_GR_STATUS				4									//״̬��Ϣ

#define SUB_GR_TABLE_INFO			100									//������Ϣ
#define SUB_GR_TABLE_STATUS			101									//����״̬

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//������Ŀ
	tagTableStatus					TableStatusArray[512];				//����״̬
};

//����״̬
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//���Ӻ���
	tagTableStatus					TableStatus;						//����״̬
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_INSURE				5									//�û���Ϣ

//��������
#define SUB_GR_QUERY_INSURE_INFO	1									//��ѯ����
#define SUB_GR_SAVE_SCORE_REQUEST	2									//������
#define SUB_GR_TAKE_SCORE_REQUEST	3									//ȡ�����
#define SUB_GR_TRANSFER_SCORE_REQUEST  4                                //ת�˲��� 

#define SUB_GR_USER_INSURE_INFO		100									//��������
#define SUB_GR_USER_INSURE_SUCCESS	101									//���гɹ�
#define SUB_GR_USER_INSURE_FAILURE	102									//����ʧ��

//////////////////////////////////////////////////////////////////////////////////

//�������
struct CMD_GR_C_SaveScoreRequest
{
	SCORE							lSaveScore;							//�����Ŀ
};

//ȡ������
struct CMD_GR_C_TakeScoreRequest
{
	SCORE							lTakeScore;							//ȡ����Ŀ
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
};

//ת�˽��
struct CMD_GP_C_TransferScoreRequest
{
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
	TCHAR                           szLogonPass[LEN_PASSWORD];          //��¼����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
};

//��������
struct CMD_GR_S_UserInsureInfo
{
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//���гɹ�
struct CMD_GR_S_UserInsureSuccess
{
	SCORE							lUserScore;							//���Ͻ��
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GR_S_UserInsureFailure
{
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_MANAGE				6									//��������

#define SUB_GR_SEND_WARNING			1									//���;���
#define SUB_GR_SEND_MESSAGE			2									//������Ϣ
#define SUB_GR_LOOK_USER_IP			3									//�鿴��ַ
#define SUB_GR_KICK_USER			4									//�߳��û�
#define SUB_GR_LIMIT_ACCOUNS		5									//�����ʻ�
#define SUB_GR_SET_USER_RIGHT		6									//Ȩ������

//��������
#define SUB_GR_QUERY_OPTION			7									//��ѯ����
#define SUB_GR_OPTION_SERVER		8									//��������
#define SUB_GR_OPTION_CURRENT		9									//��ǰ����

//////////////////////////////////////////////////////////////////////////////////

//���;���
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szWarningMessage[LEN_USER_CHAT];	//������Ϣ
};

//ϵͳ��Ϣ
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//��Ϸ��Ϣ
	BYTE							cbRoom;								//��Ϸ��Ϣ
	WORD							wChatLength;						//��Ϣ����
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
};

//�鿴��ַ
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�߳��û�
struct CMD_GR_KickUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�����ʻ�
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//Ȩ������
struct CMD_GR_SetUserRight
{
	//Ŀ���û�
	DWORD							dwTargetUserID;						//Ŀ���û�

	//�󶨱���
	BYTE							cbGameRight;						//�ʺ�Ȩ��
	BYTE							cbAccountsRight;					//�ʺ�Ȩ��

	//Ȩ�ޱ仯
	BYTE							cbLimitRoomChat;					//��������
	BYTE							cbLimitGameChat;					//��Ϸ����
	BYTE							cbLimitPlayGame;					//��ϷȨ��
	BYTE							cbLimitSendWisper;					//������Ϣ
	BYTE							cbLimitLookonGame;					//�Թ�Ȩ��
};

//��������
struct CMD_GR_OptionCurrent
{
	DWORD							dwRuleMask;							//��������
	tagServerOptionInfo				ServerOptionInfo;					//��������
};

//��������
struct CMD_GR_ServerOption
{
	tagServerOptionInfo				ServerOptionInfo;					//��������
};

//////////////////////////////////////////////////////////////////////////////////

//���ñ�־
#define OSF_ROOM_CHAT				1									//��������
#define OSF_GAME_CHAT				2									//��Ϸ����
#define OSF_ROOM_WISPER				3									//����˽��
#define OSF_ENTER_TABLE				4									//������Ϸ
#define OSF_ENTER_SERVER			5									//���뷿��

//��������
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//���ñ�־
	BYTE							cbOptionValue;						//���ñ�־
};

//////////////////////////////////////////////////////////////////////////////////
//�������

#define MDM_GF_FRAME				100									//�������

//////////////////////////////////////////////////////////////////////////////////
//�������

//�û�����
#define SUB_GF_GAME_OPTION			1									//��Ϸ����
#define SUB_GF_USER_READY			2									//�û�׼��
#define SUB_GF_LOOKON_CONFIG		3									//�Թ�����

//��������
#define SUB_GF_USER_CHAT			10									//�û�����
#define SUB_GF_USER_EXPRESSION		11									//�û�����

//��Ϸ��Ϣ
#define SUB_GF_GAME_STATUS			100									//��Ϸ״̬
#define SUB_GF_GAME_SCENE			101									//��Ϸ����
#define SUB_GF_LOOKON_STATUS		102									//�Թ�״̬

//ϵͳ��Ϣ
#define SUB_GF_SYSTEM_MESSAGE		200									//ϵͳ��Ϣ
#define SUB_GF_ACTION_MESSAGE		201									//������Ϣ

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
struct CMD_GF_GameOption
{
	BYTE							cbAllowLookon;						//�Թ۱�־
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwClientVersion;					//��Ϸ�汾
};

//�Թ�����
struct CMD_GF_LookonConfig
{
	DWORD							dwUserID;							//�û���ʶ
	BYTE							cbAllowLookon;						//�����Թ�
};

//�Թ�״̬
struct CMD_GF_LookonStatus
{
	BYTE							cbAllowLookon;						//�����Թ�
};

//��Ϸ����
struct CMD_GF_GameStatus
{
	BYTE							cbGameStatus;						//��Ϸ״̬
	BYTE							cbAllowLookon;						//�Թ۱�־
};

//�û�����
struct CMD_GF_C_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GF_S_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GF_C_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�����
struct CMD_GF_S_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//////////////////////////////////////////////////////////////////////////////////
//��Ϸ����

#define MDM_GF_GAME					200									//��Ϸ����

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ

//�û�����
#define DTP_GR_NICK_NAME			1									//�û��ǳ�
#define DTP_GR_USER_NOTE			2									//�û���ע
#define DTP_GR_ADDR_DESCRIBE        3                                   //��ַ���� 

//////////////////////////////////////////////////////////////////////////////////

//�������
#define REQUEST_FAILURE_NORMAL		0									//����ԭ��
#define REQUEST_FAILURE_NOGOLD		1									//��Ҳ���
#define REQUEST_FAILURE_NOSCORE		2									//���ֲ���
#define REQUEST_FAILURE_PASSWORD	3									//�������

//�������
#define REQUEST_FAILURE_CMD_NONE     0                                  //ԭʼ����   
#define REQUEST_FAILURE_CMD_CONCLUDE 1                                  //�ر�����

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif