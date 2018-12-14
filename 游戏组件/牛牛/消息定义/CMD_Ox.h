#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE

#pragma pack(1)

#define COMPARE_ZERO	0.001
static int s_nmultiple[5]={1,2,5,10,20};


#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#define myatoi      atoi
#define myatoi64    _atoi64
//�γ��������
#define	Mywtof		atof

#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#define myatoi      _wtoi
#define myatoi64	_wtoi64
//�γ��������
#define	Mywtof		_wtof

#endif

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							101									//��Ϸ I D
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("ţţ")				        //��Ϸ����
#define MAXCOUNT						5									//�˿���Ŀ

#ifdef DEBUG
	#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
	#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾
#else
	#define VERSION_SERVER				PROCESS_VERSION(7,0,3)				//����汾
	#define VERSION_CLIENT				PROCESS_VERSION(7,0,3)				//����汾
#endif
//����ԭ��
#define GER_NO_PLAYER					0x10								//û�����

//��Ϸ״̬
#define GS_TK_FREE					GAME_STATUS_FREE						//�ȴ���ʼ
#define GS_TK_CALL					GAME_STATUS_PLAY						//��ׯ״̬
#define GS_TK_SCORE					GAME_STATUS_PLAY+1						//��ע״̬
#define GS_TK_PLAYING				GAME_STATUS_PLAY+2						//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_SCORE					101									//��ע���
#define SUB_S_PLAYER_EXIT				102									//�û�ǿ��
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����
#define SUB_S_OPEN_CARD					105									//�û�̯��
#define SUB_S_CALL_BANKER				106									//�û���ׯ
#define SUB_S_AMDIN_COMMAND				108									//ϵͳ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	SCORE							lCellScore;							//��������

	//��ʷ����
	SCORE								lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE								lCollectScore[GAME_PLAYER];			//������Ϣ
	TCHAR								szGameRoomName[64];					//��������
};

//��Ϸ״̬
struct CMD_S_StatusCall
{
	SCORE								lCellScore;							//��������

	WORD								wCallBanker;						//��ׯ�û�
	BYTE                                cbDynamicJoin;                      //��̬���� 
	BYTE                                cbPlayStatus[GAME_PLAYER];          //�û�״̬

	//��ʷ����
	SCORE								lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE								lCollectScore[GAME_PLAYER];			//������Ϣ
	TCHAR								szGameRoomName[64];					//��������
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	SCORE								lCellScore;							//��������
	//��ע��Ϣ
	BYTE                                cbPlayStatus[GAME_PLAYER];          //�û�״̬
	BYTE                                cbDynamicJoin;                      //��̬����
	SCORE								lTurnMaxScore;						//�����ע
	SCORE								lTableScore[GAME_PLAYER];			//��ע��Ŀ
	WORD								wBankerUser;						//ׯ���û�

	//��ʷ����
	SCORE								lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE								lCollectScore[GAME_PLAYER];			//������Ϣ
	TCHAR								szGameRoomName[64];					//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	SCORE								lCellScore;							//��������
	//״̬��Ϣ
	BYTE                                cbPlayStatus[GAME_PLAYER];          //�û�״̬
	BYTE                                cbDynamicJoin;                      //��̬����
	SCORE								lTurnMaxScore;						//�����ע
	SCORE								lTableScore[GAME_PLAYER];			//��ע��Ŀ
	WORD								wBankerUser;						//ׯ���û�

	//�˿���Ϣ
	BYTE								cbHandCardData[GAME_PLAYER][MAXCOUNT];//�����˿�
	BYTE								bOxCard[GAME_PLAYER];				//ţţ����

	//��ʷ����
	SCORE								lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE								lCollectScore[GAME_PLAYER];			//������Ϣ
	TCHAR								szGameRoomName[64];					//��������
};

//�û���ׯ
struct CMD_S_CallBanker
{
	WORD								wCallBanker;						//��ׯ�û�
	bool								bFirstTimes;						//�״ν�ׯ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	//��ע��Ϣ
	SCORE								lTurnMaxScore;						//�����ע
	WORD								wBankerUser;						//ׯ���û�
};

//�û���ע
struct CMD_S_AddScore
{
	WORD								wAddScoreUser;						//��ע�û�
	SCORE								lAddScoreCount;						//��ע��Ŀ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	SCORE								lGameTax[GAME_PLAYER];				//��Ϸ˰��
	SCORE								lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	BYTE								cbCardData[GAME_PLAYER];			//�û��˿�
};

//�������ݰ�
struct CMD_S_SendCard
{
	BYTE								cbCardData[GAME_PLAYER][MAXCOUNT];	//�û��˿�
};

//�û��˳�
struct CMD_S_PlayerExit
{
	WORD								wPlayerID;							//�˳��û�
};

//�û�̯��
struct CMD_S_Open_Card
{
	WORD								wPlayerID;							//̯���û�
	BYTE								bOpen;								//̯�Ʊ�־
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_CALL_BANKER				1									//�û���ׯ
#define SUB_C_ADD_SCORE					2									//�û���ע
#define SUB_C_OPEN_CARD					3									//�û�̯��

//�û���ׯ
struct CMD_C_CallBanker
{
	BYTE								bBanker;							//��ׯ��־
};


//�û���ע
struct CMD_C_AddScore
{
	SCORE								lScore;								//��ע��Ŀ
};

//�û�̯��
struct CMD_C_OxCard
{
	BYTE								bOX;								//ţţ��־
};

//////////////////////////////////////////////////////////////////////////
//���ƶ���
#define SUB_C_AMDIN_COMMAND			5									//����Ա����

#define RQ_OPTION_CANCLE		1		//ȡ��
#define RQ_OPTION_QUERYING		2		//��ѯ
#define RQ_OPTION_SETING		3		//����

#define CHEAT_TYPE_LOST		0		//��
#define CHEAT_TYPE_WIN		1		//Ӯ

struct CMD_C_AdminReq
{
	BYTE cbReqType;							//��������
	BYTE cbCheatCount;						//���ƴ���
	BYTE cbCheatType;						//��������
	DWORD dwGameID;							//��ұ�ʶ
};

#pragma pack()

#endif