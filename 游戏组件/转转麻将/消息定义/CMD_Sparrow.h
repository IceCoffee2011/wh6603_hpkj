#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#define GAME_PLAYER					4									//��Ϸ����
#ifdef USE_GB
#define KIND_ID						512									//��Ϸ I D
#define GAME_NAME					TEXT("�����齫")					//��Ϸ����
#else
#define KIND_ID						303									//��Ϸ I D
#define GAME_NAME					TEXT("תת�齫")					//��Ϸ����
#endif
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾///////////
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾//////////
//#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH)	//��Ϸ����

//��Ϸ״̬
#define GS_MJ_FREE					GAME_STATUS_FREE					//����״̬
#define GS_MJ_PLAY					(GAME_STATUS_PLAY+1)				//��Ϸ״̬
//#define GS_MJ_FREE					GS_FREE								//����״̬
//#define GS_MJ_PLAY					(GS_PLAYING+1)						//��Ϸ״̬
#define GS_MJ_HAI_DI				(GAME_STATUS_PLAY+2)						//����״̬
//#define GS_MJ_BEFOREPLAY			(GS_PLAYING+3)						//��Ϸǰ״̬

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�

	CMD_WeaveItem()
	{
		ZeroMemory(this, sizeof(*this));
		wProvideUser = -1;
	}
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_OPERATE_NOTIFY		103									//������ʾ
#define SUB_S_OPERATE_RESULT		104									//��������
#define SUB_S_OPERATE_HAI_DI		105									//���ײ���
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�
#define SUB_S_TING_CARD             108                                 //��������
#define SUB_S_USER_TINGCARD_RESULT	109									//���ƵĽ��

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG						lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�

	CMD_S_StatusFree()
	{
		ZeroMemory(this, sizeof(*this));
		wBankerUser = -1;
	}
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONGLONG						lCellScore;							//��Ԫ����
	WORD							wSiceCount;							//���ӵ���
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�

	//״̬����
	BYTE							cbHearStatus;						//����״̬
	BYTE							cbActionCard;						//�����˿�
	BYTE							cbActionMask;						//��������
	BYTE							cbLeftCardCount;					//ʣ����Ŀ

	//������Ϣ
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
	BYTE							cbDiscardCount[4];					//������Ŀ
	BYTE							cbDiscardCard[4][55];				//������¼

	//�˿�����
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[14];						//�˿��б�

	//����˿�
	BYTE							cbWeaveCount[4];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[4][4];				//����˿�

	//������Ϣ
	bool                            m_bTingCard;                         //�ܷ�����
	
	//�Ƿ��Ѿ�����
	bool							bIsAlreadyOper;						//�Ƿ��Ѿ�������


	//�ܲ���Ϣ
	BYTE							byGangCard[4];						//����
	BYTE							byGangCardNums;						//����
	BYTE							byBuCard[4];						//����
	BYTE							byBuCardNums;						//����


	//����
	BYTE							byKindCard;							//����

	//�Ƿ������
	bool                            bIsPlayDice;

	CMD_S_StatusPlay()
	{
		ZeroMemory(this, sizeof(*this));
		wBankerUser = -1;
		wCurrentUser = -1;
		wOutCardUser = -1;
	}
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wSiceCount;							//���ӵ���
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbUserAction;						//�û�����
	BYTE							cbCardData[14];						//�˿��б�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
	//BYTE							cbKingCard;					        //����
	WORD                            wMPIndex;                           //���Ƶ���һ�ŵ�����

	BYTE							byGangCard[4];						//����
	BYTE							byGangCardNums;						//����
	BYTE							byBuCard[4];						//����
	BYTE							byBuCardNums;						//����

	CMD_S_GameStart()
	{
		ZeroMemory(this, sizeof(*this));
		wBankerUser = -1;
		wCurrentUser = -1;
	}
};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
	
	CMD_S_OutCard()
	{
		wOutCardUser = -1;
		cbOutCardData = -1;
	}
};

//�����˿�
struct CMD_S_SendCard
{
	bool							bSupplyCard;							//�油�ƣ��ٷ��� 
	BYTE							cbCardData;							//�˿�����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�

	BYTE							byGangCard[4];						//����
	BYTE							byGangCardNums;						//����
	BYTE							byBuCard[4];						//����
	BYTE							byBuCardNums;						//����

	CMD_S_SendCard()
	{
		ZeroMemory(this, sizeof(*this));
		wCurrentUser = -1;
	}
};

//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�

	BYTE							byGangCard[4];						//����
	BYTE							byGangCardNums;						//����
	BYTE							byBuCard[4];						//����
	BYTE							byBuCardNums;						//����

	CMD_S_OperateNotify()
	{
		ZeroMemory(this, sizeof(*this));
		wResumeUser = -1;
	}
};

//��������
struct CMD_S_OperateResult
{
	bool							bBringMuscleCanHu;				//�����ܺ���
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
	BYTE								bMuscle;							// �ʽ, 0 ��Чֵ, 1 ���2 ���3 �Ž�
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	LONGLONG					llMuscleGold[GAME_PLAYER]; //��Ҹ���ֵ

	CMD_S_OperateResult()
	{
		bBringMuscleCanHu=false;
		wOperateUser = 0;
		wProvideUser = 0;
		cbOperateCode = 0x00;
		cbOperateCard = 0;
		bMuscle = 1;
		
		for (BYTE i=0; i<GAME_PLAYER; i++)
		{
			llMuscleGold[i] = 0l;
		}
	}
};

//���ײ���
struct CMD_S_OperateHaiDi
{
	WORD							wCurrentUser;						//��ǰ�û�

	CMD_S_OperateHaiDi()
	{
		ZeroMemory(this, sizeof(*this));
		wCurrentUser = -1;
	}
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbChiHuCard;						//�Ժ��˿�
	BYTE							cbHaiDiCard;						//�����˿�
	BYTE							cbCardCount[4];						//�˿���Ŀ
	BYTE							cbCardData[4][14];					//�˿�����	
    BYTE                            bKingCard;                          //��������
	
	bool							bIsHuangzhuang;						//�Ƿ��ׯ
	
	WORD							wProvideUser;						//�����û�	
	WORD							wChiHuKind[4];						//��������
	WORD                            wChiHuRight[4];                     //����Ȩλ
	
	int                             iChiHuType;                         //��������

	bool                            IsUserLeft[GAME_PLAYER];            //�û��Ƿ�ǿ��
	BYTE                            cbKingCount[GAME_PLAYER];           //������Ŀ
	
	int                             iChiHuKind[GAME_PLAYER];           //�������ͣ�0��û����1��������2�����ƣ�3�����ڣ� 
	
	LONGLONG						cbTimes[GAME_PLAYER];               //��ҷ���
	LONGLONG						iMuscle[GAME_PLAYER];					// ��	
	LONGLONG						iCellScore[GAME_PLAYER];            //���䱶�ʣ���Ԫ���֣�
	LONGLONG						lGameScore[4];					    //��Ϸ����

	CMD_S_GameEnd()
	{
		ZeroMemory(this, sizeof(*this));
		wProvideUser = -1;
	}
};
//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�

	CMD_S_Trustee()
	{
		ZeroMemory(this, sizeof(*this));
		wChairID = -1;
	}
};
//�û�����
struct CMD_S_GameTingCard
{
	WORD							wTingCardUser;						//�����û�

	CMD_S_GameTingCard()
	{
		ZeroMemory(this, sizeof(*this));
		wTingCardUser = -1;
	}
};

//�û�����
struct CMD_S_Listen
{
	WORD							wChairId;							//�����û�
};

///��������
struct CMD_S_ReplaceCard
{
	WORD							wReplaceUser;						//�����û�
	BYTE							cbReplaceCard;						//�����˿�
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			2									//�����˿�
#define SUB_C_OPERATE_HAI_DI		3									//���ײ���
#define SUB_C_TRUSTEE				4									//�û��й�
#define SUB_C_USERTINGCARD			5									//��������
#define SUB_C_SZSTART               6                                   //����ɸ��
#define SUB_C_LISTEN				7									///�û�����

//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����

	CMD_C_OutCard()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//��������
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�

	CMD_C_OperateCard()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//���ײ���
struct CMD_C_OperateHaiDi
{
	BYTE							cbTakeHaiDi;						//��ȡ����

	CMD_C_OperateHaiDi()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

///�û�����
struct CMD_C_Listen
{
	BYTE							cbListen;							//�����û�
};

//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	

	CMD_C_Trustee()
	{
		ZeroMemory(this, sizeof(*this));
	}
};
//�û����Ʋ���
struct CMD_UserTingOperate
{
	bool							bTingCard;							//�Ƿ�����

	CMD_UserTingOperate()
	{
		ZeroMemory(this, sizeof(*this));
	}
};


//////////////////////////////////////////////////////////////////////////

#endif