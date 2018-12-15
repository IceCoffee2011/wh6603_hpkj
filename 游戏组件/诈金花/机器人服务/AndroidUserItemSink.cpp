#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					2									//����ʱ��

//��Ϸʱ��
#define IDI_ANDROID_ITEM_SINK		100
#define IDI_START_GAME				(IDI_ANDROID_ITEM_SINK+0)			//��ʼ��ʱ��
//��Ϸʱ��
#define IDI_ADD						(IDI_ANDROID_ITEM_SINK+3)			//��ע
#define IDI_SEE						(IDI_ANDROID_ITEM_SINK+4)			//����
#define IDI_FALLOW					(IDI_ANDROID_ITEM_SINK+5)			//��ע
#define IDI_COMPARE					(IDI_ANDROID_ITEM_SINK+6)			//����
#define IDI_GIVEUP					(IDI_ANDROID_ITEM_SINK+7)			//����
#define IDI_OPEN					(IDI_ANDROID_ITEM_SINK+8)			//����
#define IDI_COMPARE_OVER			(IDI_ANDROID_ITEM_SINK+9)			//����
//���ж�ʱ
#define IDI_BANK_OPERATE			(IDI_ANDROID_ITEM_SINK+10)			//���ж�ʱ

//ʱ���ʶ
#define TIME_START_GAME				3									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			4									//��ע��ʱ��
#define	TIME_USER_COMPARE_CARD		5									//���ƶ�ʱ��
//////////////////////////////////////////////////////////////////////////
//����ʱ��
#define TIME_LESS_TIME				2									//����ʱ�� 
#define TIME_RAND_TIME				3									//���ʱ�� 

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//�����˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	for(WORD i=0;i<GAME_PLAYER;i++)m_bMingZhu[i]=false;
	m_lMaxCellScore=0L;
	
	//�ӿڱ���
	m_pIAndroidUserItem=NULL;
	m_cbCardV = 0;
	m_bIsYz = false;
	m_bMinZhuIndex = 0;
	//�������
	srand( (unsigned)time(NULL));

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void *  CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool  CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem)!=NULL);
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);

	//�����ж�
	if (m_pIAndroidUserItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}
	return true;
}

//���ýӿ�
bool  CAndroidUserItemSink::RepositionSink()
{
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	m_lMaxCellScore=0L;
	for(WORD i=0;i<GAME_PLAYER;i++)m_bMingZhu[i]=false;
	return true;
}

//ʱ����Ϣ
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��ʱ��
		{
			KillAllTimer();
			//����׼��
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_ADD:				//��ע����
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_ADD);
			m_bFirstAdd=false;
			LONGLONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lMaxCellScore*2):m_lMaxCellScore;
			//������Ϣ
			CMD_C_AddScore AddScore;
			ZeroMemory(&AddScore,sizeof(AddScore));
			AddScore.wState=0;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
			return true;
		}
	case IDI_SEE:				//���ƶ���
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_SEE);
			if(!m_bMingZhu[wMeChairID])
			{
				m_bMingZhu[wMeChairID]=true;
				m_pIAndroidUserItem->SendSocketData(SUB_C_LOOK_CARD,NULL,0);
			}
			return true;
		}
	case IDI_FALLOW:			//��ע����
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_FALLOW);
			m_bFirstAdd=false;
			LONGLONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lCurrentTimes*m_lCellScore*2):(m_lCurrentTimes*m_lCellScore);
			//������Ϣ
			CMD_C_AddScore AddScore;
			ZeroMemory(&AddScore,sizeof(AddScore));
			AddScore.wState=0;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
			return true;
		}
	case IDI_COMPARE:			//���ƶ���
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_COMPARE);
			LONGLONG lCurrentScore = m_lCurrentTimes*m_lCellScore*2;
			//������Ϣ
			CMD_C_AddScore AddScore;
			ZeroMemory(&AddScore,sizeof(AddScore));
			AddScore.wState=TRUE;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
			return true;
		}
	case IDI_COMPARE_OVER:		//������Ϣ
		{
			LONGLONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lMaxCellScore*2):m_lMaxCellScore;
			//������Ϣ
			CMD_C_AddScore AddScore;
			ZeroMemory(&AddScore,sizeof(AddScore));
			AddScore.wState=TRUE;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			m_pIAndroidUserItem->KillGameTimer(IDI_COMPARE_OVER);
			CMD_C_CompareCard CompareCard;
			ZeroMemory(&CompareCard,sizeof(CompareCard));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(i==wMeChairID)continue;
				if(m_cbPlayStatus[i]==TRUE)
				{
					CompareCard.wCompareUser=i;
					break;
				}
			}
			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD,&CompareCard,sizeof(CompareCard));
			return true;
		}
	case IDI_GIVEUP:			//��������
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_GIVEUP);
			m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP,NULL,0);
			return true;
		}
	case IDI_OPEN:				//���ƶ���
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_OPEN);
			LONGLONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lCurrentTimes*m_lCellScore*4):(m_lCurrentTimes*m_lCellScore*2);
			//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.wState=2;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
			m_pIAndroidUserItem->SendSocketData(SUB_C_OPEN_CARD,NULL,0);
			return true;
		}
	case IDI_BANK_OPERATE:		//���в���
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_BANK_OPERATE);  //ɾ��ʱ��

			//��������
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();  //��ȡ����ָ��
			LONGLONG lRobotScore = pUserItem->GetUserScore();					//��ȡ���
			{

				//�жϴ�ȡ
				if (lRobotScore > m_lRobotScoreRange[1])   //��Ҵ������ֵ  ���
				{
					LONGLONG lSaveScore=0L;

					lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);  //����ٷֱ�
					if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

					if (lSaveScore > 0)
						m_pIAndroidUserItem->PerformSaveScore(lSaveScore);			//���

				}
				else if (lRobotScore < m_lRobotScoreRange[0])     //ȡ��
				{

					//�γ������޸�
					//�������
					srand( (unsigned)time(NULL));
					
					SCORE lScore=m_lRobotBankGetScore+(rand()%(m_lRobotBankGetScoreBanker-m_lRobotBankGetScore));
					if (lScore > 0)
						m_pIAndroidUserItem->PerformTakeScore(lScore);
				}
			}
			return true;
		}
	

	}

	return false;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_ANDROID_GET_CARD:// ��ȡ�˿�
		{
			if(wDataSize != sizeof(CMD_S_SendCard)) return false;
			CMD_S_SendCard * p = (CMD_S_SendCard*)pData;
			//�����˿�����
			CopyMemory(m_cbHandCardData,p->cbCardData,sizeof(p->cbCardData));
			//�����û�״̬
			CopyMemory(m_cbPlayStatus,p->cbPlayStatus,sizeof(p->cbPlayStatus));

			//��ȡ����
			WORD MeChairID=m_pIAndroidUserItem->GetChairID();
			//���ƶ���
			m_bMingZhu[MeChairID]=false;
			//////////////////////////////////////////////
			//�������
			m_bBiggest=p->IsWinUser;

			m_bFirstAdd=true;


			//��ȡ����
			ReadConfigInformation();

			if(m_wCurrentUser==MeChairID)
			{
				m_bFirstAdd=true;
				//���������в���
				Alloperations();
			}


			return true;
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			//��Ϣ����
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:		//�û���ע
		{
			//��Ϣ����
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_LOOK_CARD:		//������Ϣ
		{
			//��Ϣ����
			return OnSubLookCard(pData,wDataSize);
		}
	case SUB_S_COMPARE_CARD:	//������Ϣ
		{
			//��Ϣ����
			return OnSubCompareCard(pData,wDataSize);
		}
	case SUB_S_OPEN_CARD:		//������Ϣ
		{
			//��Ϣ����
			return OnSubOpenCard(pData,wDataSize);
		}
	case SUB_S_GIVE_UP:			//�û�����
		{
			//��Ϣ����
			return OnSubGiveUp(pData,wDataSize);
		}
	case SUB_S_PLAYER_EXIT:		//�û�ǿ��
		{
			//��Ϣ����
			return OnSubPlayerExit(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			//�޸ģ�2014-3-28
			m_bMingZhu[m_pIAndroidUserItem->GetChairID()]=false;

			//��Ϣ����
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_WAIT_COMPARE:	//�ȴ�����
		{

			//��Ϣ����
			if (wDataSize!=sizeof(CMD_S_WaitCompare)) return false;
			CMD_S_WaitCompare * pWaitCompare=(CMD_S_WaitCompare *)pData;	
			ASSERT(pWaitCompare->wCompareUser==m_wCurrentUser);

			//���¶�ʱ
			if(m_pIAndroidUserItem->GetChairID()==m_wCurrentUser)
			{
				//����ʱ��
				UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
				m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse);
			}

			return true;
		}
	}

	//�������
	//ASSERT(FALSE);

	return true;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lCellScore = pStatusFree->lCellScore;

			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));
			//��Ϣ����
			ReadConfigInformation();
			//��ʼʱ��
			UINT nElapse=rand()%(3)+TIME_LESS;
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

			return true;
		}
	case GAME_STATUS_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			m_lCellScore = pStatusPlay->lCellScore;
			m_lMaxCellScore = pStatusPlay->lMaxCellScore;

			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));
			
			ReadConfigInformation();
			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//�û�����
void  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}



//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{


	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	//ɾ������ʱ��
	KillAllTimer();

	m_bIsYz = false;
	//��ʼ��
	m_bCompareState=0;
	m_lMaxCellScore=0;
	m_lUserAddScore=0;
	m_bMinZhuIndex=0;
	//������
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	//������Ϣ
	for(WORD i=0;i<GAME_PLAYER;i++)m_bMingZhu[i]=false;

	//������Ϣ
	m_lCellScore=pGameStart->lCellScore;		//��Ԫ��ע	
	m_lCurrentTimes=pGameStart->lCurrentTimes;  //��ǰ����
	m_lMaxCellScore=pGameStart->lMaxScore;		//��Ԫ����
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_lUserMaxScore=pGameStart->lUserMaxScore;

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;
	KillAllTimer();
	//���ñ���
	m_cbPlayStatus[pGiveUp->wGiveUpUser]=FALSE;

	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	IServerUserItem *pIServerUserItem=NULL;

	//�������
	BYTE bCount = 0;
	BYTE bUserCount = 0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]==TRUE)
		{
			bCount++;
			IServerUserItem* pIUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
			if(pIUserItem == NULL) continue;
			if(!pIUserItem->IsAndroidUser())
			{
				bUserCount++;
			}
		}
	}

	if(bUserCount<=0 && bCount >= 2)
	{
		if(GaiLv(100)<40)
		{
			/*OnAllCompare();*/
			UINT nElapse=TIME_LESS_TIME;
			m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse+1);

		}
		else
			Alloperations();
// 		UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
// 		m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse);
		return true;
	}
	//�жϽ���
	if(bCount>1)
	{
		WORD MeChairID=m_pIAndroidUserItem->GetChairID();
		if(m_wCurrentUser==MeChairID)
		{
			Alloperations();
		}
	}
	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{

	//��������
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	ASSERT(pAddScore->wCurrentUser < GAME_PLAYER);
	ASSERT(pAddScore->lCurrentTimes<=m_lMaxCellScore/m_lCellScore);
	
	KillAllTimer();
	//��ǰ�û�
	m_lCurrentTimes = pAddScore->lCurrentTimes;
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTableScore[pAddScore->wAddScoreUser]+=pAddScore->lAddScoreCount;
	m_lUserAddScore+=pAddScore->lAddScoreCount;
	m_bCompareState=pAddScore->wCompareState;

	//����ʱ��
	if(pAddScore->wCompareState==FALSE && m_wCurrentUser==wMeChairID)
	{
		Alloperations();
	}
	else if(pAddScore->wCompareState==TRUE && m_wCurrentUser==wMeChairID)
	{
		//����
		OnAllCompare();
	}
	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubLookCard(const void * pBuffer, WORD wDataSize)
{

	//Ч������
	if (wDataSize!=sizeof(CMD_S_LookCard)) return false;
	KillAllTimer();
	CMD_S_LookCard * pLookCard=(CMD_S_LookCard *)pBuffer;
	m_bMingZhu[pLookCard->wLookCardUser] = true;   //���Ʊ���

	//�ж��˿��Ƿ���
	IsCanYa();

	//���¶�ʱ
	if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		//��עʱ��
		Alloperations();
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubCompareCard(const void * pBuffer, WORD wDataSize)
{
	
	//Ч������
	if (wDataSize!=sizeof(CMD_S_CompareCard)) return false;
	CMD_S_CompareCard * pCompareCard=(CMD_S_CompareCard *)pBuffer;
	KillAllTimer();
	//���ñ���
	m_cbPlayStatus[pCompareCard->wLostUser]=FALSE;
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();

	//��ǰ�û�
	m_wCurrentUser=pCompareCard->wCurrentUser;

	//�������
	BYTE bCount=0;
	BYTE bUserCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]==TRUE)
		{
			IServerUserItem* pUserItem = m_pIAndroidUserItem->GetTableUserItem(i);

			bCount++;
			if(pUserItem == NULL) continue;

			if(!pUserItem->IsAndroidUser())
				bUserCount++;
		}
	}

	//�жϽ���
	if(bCount>1)
	{
		//�ؼ���Ϣ
		if(wMeChairID==m_wCurrentUser)
		{
			//���в���
			if((bUserCount < 1)&&(GaiLv(100)<40))
			{
				//OnAllCompare();
				UINT nElapse=TIME_LESS_TIME;
				m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse+1);
			}
			else
				Alloperations();
				
		}
	}

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_OpenCard)) return false;
	//Ч������
	KillAllTimer();
	return true;
}

//�û�ǿ��
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	////Ч������
	//if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	//CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	////��Ϸ��Ϣ
	//ASSERT(m_cbPlayStatus[pPlayerExit->wPlayerID]==TRUE);
	//m_cbPlayStatus[pPlayerExit->wPlayerID]=FALSE;


	//Ч������
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	//��Ϸ��Ϣ
	ASSERT(m_cbPlayStatus[pPlayerExit->wPlayerID]==TRUE);
	m_cbPlayStatus[pPlayerExit->wPlayerID]=FALSE;
	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	////Ч�����
	//if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	////��ʼ��ť

	////��עʱ��
	//UINT nElapse=rand()%(TIME_START_GAME+3)+TIME_LESS*3;
	//m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
	//��עʱ��
	KillAllTimer();
	//������ȡ���ж�

	m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE, (rand()%2+1));
	UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	return true;
}
//////////////////////////////////////////////////////////////////////////
//ȫ������
bool CAndroidUserItemSink::Alloperations()
{
	srand((unsigned)time(NULL));//�������������

	//��ȡ����
	WORD MeChairID=m_pIAndroidUserItem->GetChairID();
	
	//////////////Ĭ��
	m_bMinZhuIndex=0;	
	if(m_lUserAddScore<m_MinLostScore) m_bMinZhuIndex=m_MinLostIndex;   ////������עû�дﵽ��������
	if(m_lUserAddScore>m_MinLostScore && m_lUserAddScore<m_MaxLostScore) m_bMinZhuIndex=(rand()%m_MaxLostIndex)+m_MinLostIndex+15;   //��֮��
	if(m_lUserAddScore>m_MaxLostScore) m_bMinZhuIndex=m_MaxLostIndex;   ////������ע�ﵽ�������

	CString strLog;
	strLog.Format(TEXT("���Ʊ���:%d,��С���Ʊ���:%d,����Ʊ���:%d,�������:%d"),m_bMinZhuIndex,m_MinLostIndex,m_MaxLostIndex,m_MinLostScore);
	CTraceService::TraceString(strLog,TraceLevel_Info);
	strLog.Format(TEXT("�������:%d,�û���ע:%d"),m_MaxLostScore,m_lUserAddScore);
	CTraceService::TraceString(strLog,TraceLevel_Info);	

	if(!m_bMingZhu[MeChairID]) //û�п����Ƶ�
	{
	
		if(GaiLv(m_bMinZhuIndex)&&m_bBiggest==true)  //�ٷ�֮10���Ʋ�������������
		{
			//����
			UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
			m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse);
			return true;
		}

	}

	//�𶹲��� �޵�ģʽ
	//////////////////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	//�������
	LONG lTemp=(m_bMingZhu[MeChairID])?6:5;   

	//m_lUserMaxScore�û����-����������ע ��������ʣ���ٽ�� ���� ������޵�5~6�� 
	//�����Ϸ�Ҳ���
	if((m_lUserMaxScore-m_lTableScore[MeChairID]) < (m_lMaxCellScore*lTemp))   
	{
		//��ǰ�������Ƿ������������
		if(m_bBiggest)
		{
			//����  ���ڽ�Ҳ�����,���л�����ѡ����
			UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
			m_pIAndroidUserItem->SetGameTimer(IDI_OPEN,nElapse);
			return true;
		}
		else  //�������
		{
			////^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			if(m_bIsYz)   //���ǵ�������
			{
				///////////////////////////////////////////////////////////////
				if(m_bMingZhu[MeChairID])  //�Ѿ�����
				{
					if (GaiLv(40))  //�ٷ�֮40
					{
						//����
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
					}
					else
					{

						if(m_cbCardV>=CT_SHUN_ZI)
						{
							//����
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);

						}
						else
						{
							//����
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
							
						}
					}
				
				}
				else   ///////////////////////////////////������ִ��?m_bIsYz�����ǿ���֮���ΪTRUE?!!ʲô�߼�
				{
					if (GaiLv(60))
					{
						//����
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
					}
					else
					{
							//����
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);

						CString szStorageMin=TEXT("������ִ��?m_bIsYz�����ǿ���֮���ΪTRUE?!!ʲô�߼�");
						//�����Ϣ
						CTraceService::TraceString(szStorageMin,TraceLevel_Warning);


						return true;
					}
				} ////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////
			}  //////û�п����ƻ���,���ǵ��ŵ����
			else
			{
				//����
				if(m_bMingZhu[MeChairID])    //�Ѿ�����  ��Ϊ���ǵ����������зǳ����������
				{
					if (GaiLv(90))  //�ٷ�֮��ʮ����
					{
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
						CString szStorageMin=TEXT("�����Ϸ�Ҳ���,���Ҳ������,�������ǵ��ŵ��Ʒ���������");
						//�����Ϣ
						CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
						return true;
					}
					else
					{				
						//����
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
					}
				}
				else			//û�п����Ƶ�
				{
					if (GaiLv(m_bMinZhuIndex))	//�ٷ�֮��ʮ����  û�п��ƾ�����? ɵB��! ����֮ǰ����Ӧ���ȿ�������?������!��2B Ҫ��
					{
						//����
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse);
					}
					else
					{
						//����		û�п�����,ֱ�ӿ���   ���ﻹ����! 
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
					}
				}

			}
			////^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			return true;
		}
	}
	//////////////////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	//����������
	if(m_bFirstAdd)//��һ��
	{
		if(m_bBiggest)//Ӯ ���ѵ������
		{
			
			if(m_bMingZhu[MeChairID])  //�����Ƶ�
			{
				if(GaiLv(80))
				{
					//��ע
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
				}
				else
				{
					//��ע
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_ADD,nElapse);
					return true;
				}
			}
			else   //û�п����Ƶ�
			{
				if(GaiLv(m_bMinZhuIndex)) 
				{
					//����
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse+2);
					return true;
				}
				else
				{
					//��ע
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
				}
			}
		}
		else//��
		{			
			if(m_bMingZhu[MeChairID])  //������
			{
			
				if(m_bIsYz)            //���ǵ�������
				{
					switch(m_cbCardV)
					{
					case CT_SINGLE:
					case CT_SPECIAL:
						{
								//����
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);	
								return true;
						}
					case CT_DOUBLE:
					case CT_SHUN_ZI:
					case CT_JIN_HUA:
					case CT_SHUN_JIN:
					case CT_BAO_ZI:
						{
							//��ע
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
							return true;
						}
					}

					////////////////////////////�����û�д���͸�ע:
					//��ע
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;

				}
				else
				{
					//�ƺ�С
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);	
					return true;
				}

			}
			else   //������û�п����Ƶ�
			{				
				if(GaiLv(m_bMinZhuIndex))   //���ƻ����е�С!
				{
					//����
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse+2);
					return true;
				}
				else
				{
					//��ע
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
				}			
			}
		}
	}
	else   //���ǵ�һ����ע 2 3 4 5  ����ע��
	{

		///////////////////�γ������ص�
		if(m_bBiggest)//Ӯ  
		{			
			if(m_bMingZhu[MeChairID])  //�ѿ���
			{
				BYTE bUserCount = 0;
				for(BYTE i = 0;i < GAME_PLAYER;i++)
				{
					if(m_cbPlayStatus[i] != true) continue;
					IServerUserItem* pUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
					if(pUserItem == NULL) continue;
					if(!pUserItem->IsAndroidUser())
						bUserCount++;
				}

				if((bUserCount < 1)&&GaiLv(100) < 30)
				{
					//OnAllCompare();
					UINT nElapse=TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse+1);

					return true;
				}
				if(m_bIsYz)        //������ǵ���
				{
					//////////////////////�ж�����
					switch(m_cbCardV)
					{
					case CT_DOUBLE:
						{
							if(m_lUserAddScore>m_MaxLostScore && GaiLv(55))
							{
								//����
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
								return true;

							}
							else
							{
								if(GaiLv(55))   //��ע
								{
									//��ע
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
									return true;

								}
								else
								{
									//����
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
									return true;
								}

							}

						}
					case CT_SHUN_ZI:
						{
							if(m_lUserAddScore>m_MaxLostScore && GaiLv(60))
							{
								//����
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
								return true;

							}
							else
							{
								if(GaiLv(20))
								{
									//��ע
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_ADD,nElapse);
									return true;
									
								}
								else
								{
									//��ע
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
									return true;
				
								}		
							
							}
						
						}
					case CT_JIN_HUA:
					case CT_SHUN_JIN:
					case CT_BAO_ZI:
						{
						
							if(GaiLv(60))
							{
								//��ע
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_ADD,nElapse);
								return true;
							
							}
							else
							{
								//��ע
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
								return true;

							}
	
						}
					}	


					////////////////////////////�����û�д���͸�ע:
					//��ע
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
					
				}
				else   //������,�ƺ�С 
				{
					BYTE cbFirstValue=m_GameLogic.GetCardLogicValue(m_cbHandCardData[MeChairID][0]);
					if(cbFirstValue<11 && GaiLv(80)) 
					{
						//����,�����ǵ����Ƶ�,����û�дﵽ���������������,������ɱ��ģʽ��
						//����
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
						return true;					
					}

					///////////////////////////////////////////////�����ж�,��ǰ��ע����,�����ע�����Ƚ��پͷ���,�����ע���Ƚϴ��ľͱ���
					if(m_lUserAddScore>m_MaxLostScore)
					{     ////�����������

						//����
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
						return true;

					}
					//�����ٷ�֮80
					if(m_lUserAddScore<m_MinLostScore && GaiLv(60))
					{    
						//����,�����ǵ����Ƶ�,����û�дﵽ���������������,������ɱ��ģʽ��
						//����
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
						return true;
					}
					else
					{
						//���ƻ��ע
						if(m_lUserAddScore>m_MinLostScore && m_lUserAddScore<m_MaxLostScore && GaiLv(55))
						{    
							//��ע
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
							return true;
						}
						else
						{
							//����
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
							return true;
						}
					}				
				}

			}
			else   //û�п��� Ӯ
			{
				BYTE bUserCount = 0;
				for(BYTE i = 0;i < GAME_PLAYER;i++)
				{
					if(m_cbPlayStatus[i] != true) continue;
					IServerUserItem* pUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
					if(pUserItem == NULL) continue;
					if(!pUserItem->IsAndroidUser())
						bUserCount++;
				}

				if((bUserCount < 1)&&GaiLv(100) < 30)
				{
					//OnAllCompare();
					UINT nElapse=TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse+1);

					return true;
				}


				if(GaiLv(m_bMinZhuIndex))   //���ƻ����е�С!
				{
					//����
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse+2);
					return true;
				}
				else
				{
					//��ע
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
				}
			}

		}
		else//�Ʋ�������,��
		{
			BYTE bUserCount = 0;
			for(BYTE i = 0;i < GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i] != true) continue;
				IServerUserItem* pUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
				if(pUserItem == NULL) continue;
				if(!pUserItem->IsAndroidUser())
					bUserCount++;
			}

			if((bUserCount < 1)&&GaiLv(100) < 60)
			{
				UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
				m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
				//OnAllCompare();
				return true;
			}

			if(m_bMingZhu[MeChairID]) //������
			{			
				if(m_bIsYz)  //���ǵ��ŵ����� ���αȽϴ�
				{

					//////////////////////�ж�����
					switch(m_cbCardV)
					{
					case CT_DOUBLE:
					case CT_SHUN_ZI:
						{
							///δ�ﵽ��С���޸�ע
							if(m_lUserAddScore<m_MinLostScore && GaiLv(50))
							{    
								//��ע
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
								return true;
							}
							else
							{
								if(m_lUserAddScore > m_MaxLostScore && GaiLv(30))//���������֣�30%�ļ��ʷ���
								{  
									//����
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
									return true;
								}
								else 
								{
									//���������� ���� ��������С֮����55%�ļ��ʱ���
									if((m_lUserAddScore > m_MinLostScore && m_lUserAddScore < m_MaxLostScore && GaiLv(55)) || m_lUserAddScore > m_MaxLostScore)
									{
										//����
										UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
										m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
										return true;
									}
									else
									{
										//��ע
										UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
										m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
										return true;
									}
								}

							}

						}
					case CT_JIN_HUA:
					case CT_SHUN_JIN:
					case CT_BAO_ZI:
						{
							//���û�дﵽ���޸�ע
							if(m_lUserAddScore<m_MaxLostScore)
							{    

								//��ע
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
								return true;

							}
							else
							{
								///���ƻ��ע
								if(m_lUserAddScore>m_MaxLostScore && GaiLv(80))
								{  

									//����
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
									return true;
								}
								else
								{
									//��ע
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
									return true;
								}
							}

						}
					}	

				}
				else   //�����Ƶ�,ֱ�ӷ���
				{
					BYTE cbFirstValue=m_GameLogic.GetCardLogicValue(m_cbHandCardData[MeChairID][0]);
					if(cbFirstValue<12) 
					{

						//����,�����ǵ����Ƶ�,����û�дﵽ���������������,������ɱ��ģʽ��
						//����
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
						return true;		
					}


					///////////////////////////////////////////////�����ж�,��ǰ��ע����,�����ע�����Ƚ��پͷ���,�����ע���Ƚϴ��ľͱ���
					if(m_lUserAddScore>m_MaxLostScore)
					{    
						////�����������
						//����
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
						return true;
					}
					//�����ٷ�֮80
					if(m_lUserAddScore<m_MinLostScore && GaiLv(80))
					{    

						//����,�����ǵ����Ƶ�,����û�дﵽ���������������,������ɱ��ģʽ��
						//����
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
						return true;
					}
					else
					{
						//�����ٷ�֮50
						if(m_lUserAddScore>m_MinLostScore && m_lUserAddScore<m_MaxLostScore && GaiLv(25))
						{    
							//��ע
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
							return true;
						}
						else
						{
							////////�жϵ���������ʲô��,Q���¾�����

							//����
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
							return true;
						}
					}
				}
			}
			else   //û�п����Ƶ�
			{
				if(GaiLv(m_bMinZhuIndex))   //���ƻ����е�С!
				{
					//����
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse+2);
					return true;
				}
				else
				{
					//��ע
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
				}

			}
		}
	}
	return true;
}
//����
bool CAndroidUserItemSink::OnAllCompare()
{
	m_bCompareState=0;
	//����
	UINT nElapse=TIME_LESS_TIME;
	m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse+1);
	return true;
}

bool CAndroidUserItemSink::GaiLv(BYTE bNum)
{
	if (rand()%100<bNum)
	{
		return true;
	}
	return false;
}
bool CAndroidUserItemSink::IsCanYa()
{
	//��ȡ����
	m_cbCardV =m_GameLogic.GetCardType(m_cbHandCardData[m_pIAndroidUserItem->GetChairID()],3);
	//�ж��Ƿ���
	if(m_cbCardV>1&&m_cbCardV!=7) m_bIsYz = true;
	return m_bIsYz;
}

//���ȫ����ʱ��
void CAndroidUserItemSink::KillAllTimer()
{	
	m_pIAndroidUserItem->KillGameTimer(IDI_COMPARE_OVER);
	m_pIAndroidUserItem->KillGameTimer(IDI_FALLOW);
	m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
	m_pIAndroidUserItem->KillGameTimer(IDI_ADD);
	m_pIAndroidUserItem->KillGameTimer(IDI_SEE);
	m_pIAndroidUserItem->KillGameTimer(IDI_FALLOW);
	m_pIAndroidUserItem->KillGameTimer(IDI_COMPARE);
	m_pIAndroidUserItem->KillGameTimer(IDI_GIVEUP);
	m_pIAndroidUserItem->KillGameTimer(IDI_OPEN);
}


//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation()
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//��ȡ����
	TCHAR szConfigFileName[MAX_PATH]=TEXT("");
	_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\ZaJinHuaConfig.ini"),szPath);
	//��������
	m_lRobotScoreRange[0] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMin"), 50, szConfigFileName);
	m_lRobotScoreRange[1] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMax"), 1000, szConfigFileName);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//�������
	m_lRobotBankGetScore = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGet"), 100, szConfigFileName);

	//������� 
	m_lRobotBankGetScoreBanker = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGetBanker"), 500, szConfigFileName);

	//����
	m_nRobotBankStorageMul = GetPrivateProfileInt(m_szRoomName, _T("RobotBankStoMul"), 50, szConfigFileName);

	///���ƻ��ʿ���
	m_MinLostScore= GetPrivateProfileInt(m_szRoomName, _T("MinLostScore"), 0, szConfigFileName);							//������� �������ֵ,���ƻ��ʱȽ���
	m_MaxLostScore= GetPrivateProfileInt(m_szRoomName, _T("MaxLostScore"), 0, szConfigFileName);							//������� �������ֵ,���ƻ��ʷǳ���
	m_MinLostIndex= GetPrivateProfileInt(m_szRoomName, _T("MinLostIndex"), 0, szConfigFileName);							//������� �������ֵ,���ƻ��ʱȽ���
	m_MaxLostIndex= GetPrivateProfileInt(m_szRoomName, _T("MaxLostIndex"), 0, szConfigFileName);							//������� �������ֵ,���ƻ��ʷǳ���
}

//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
