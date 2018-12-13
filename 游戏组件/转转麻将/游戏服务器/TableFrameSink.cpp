#include "StdAfx.h"
#include "TableFrameSink.h"
//#include "..\..\������\Include\GlobalChannel.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
//const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//�˿�����
BYTE const n_cbCardDataArray[27]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
};



//Open sendcard tool
#define			TOOL_SEND_CARD		

//////////////////////////////////////////////////////////////////////////

// ���Խ���
void CTableFrameSink::TestGameEndFun(BYTE bSelect)//Test
{
	//static int i=0;
	//bSelect = i;
	//i++;
	//if (5==i)
	//	i=0;

	switch(bSelect)
	{
	case 0:	// ����������Լ��н�,����û��
		{			
			m_byGameEndType = 1;
			ZeroMemory(m_llMuscle, sizeof(m_llMuscle));
			m_llMuscle[m_wProvideUser]=1;
			m_ChiHuResult[m_wProvideUser].wChiHuKind=CHK_PING_HU;
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			break;
		}
	case 1:	// ����������Լ��޽�,�����н�
		{
			m_byGameEndType = 1;
			ZeroMemory(m_llMuscle, sizeof(m_llMuscle));
			m_llMuscle[(m_wProvideUser+1)%GAME_PLAYER]=10;
			m_ChiHuResult[m_wProvideUser].wChiHuKind=CHK_PING_HU;
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			break;
		}
	case 2:	// ��ҵ��ݣ��������޽�,�����н�
		{
			m_byGameEndType = 2;
			ZeroMemory(m_llMuscle, sizeof(m_llMuscle));
			m_llMuscle[(m_wProvideUser+1)%GAME_PLAYER]=10;
			m_llMuscle[(m_wProvideUser+2)%GAME_PLAYER]=10;
			m_ChiHuResult[(m_wProvideUser+1)%GAME_PLAYER].wChiHuKind=CHK_PING_HU;
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			break;
		}
	case 3:	// ��ҵ��ݣ��������н�,�����޽�
		{
			m_byGameEndType = 2;
			ZeroMemory(m_llMuscle, sizeof(m_llMuscle));
			m_llMuscle[(m_wProvideUser)%GAME_PLAYER]=10;
			m_ChiHuResult[(m_wProvideUser+1)%GAME_PLAYER].wChiHuKind=CHK_PING_HU;
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			break;
		}
	case 4: // ͨ��
		{
			m_byGameEndType = 2;
			ZeroMemory(m_llMuscle, sizeof(m_llMuscle));

			for (BYTE i=0; i<GAME_PLAYER; i++)
			{
				if (i==m_wProvideUser)
					continue;

				m_ChiHuResult[i].wChiHuKind=CHK_PING_HU;
			}
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			break;
		}
	}
}

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//m_pRoomLog = NULL;

	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	//m_wMPIndex=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_llMuscle,sizeof(m_llMuscle));

	CString str;
	str.Format(_T("server: CTableFrameSink 37---%+I64d,%+I64d,%+I64d,%+I64d"), m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
	OutputDebugString(str);


	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	//ZeroMemory(m_bHearStatus,sizeof(m_bHearStatus));
	//ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	m_wHaiDiUser=INVALID_CHAIR;
	ZeroMemory(m_ChiHuResult,sizeof(m_ChiHuResult));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//���Ͽ���
	//memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));



	//osc update
	//memset(m_bIsUserFirstOutCard, true, sizeof(m_bIsUserFirstOutCard));
	//memset(m_bUserFirstIsTingPai, 0, sizeof(m_bUserFirstIsTingPai));
	memset(m_byUserHuCardType, 0, sizeof(m_byUserHuCardType));
	//m_iSaveGameOutCardCount = 0;
	//m_iSaveGameSendCardCount = 0;
	m_byGameEndType = 0;

	m_bIsPlayDice=false;

	//::srand((unsigned int)time(NULL));
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

// ͨ����Ϸ����˵�ͨ�������Դ����κμ������ݻ��Աָ��
bool CTableFrameSink::channel_to_game_service(int iMainCode, int iAssistantCode, void* pData, int iDataLen)
{
	//switch(iMainCode)
	//{
	//case CTGS_MAIN_LOG:	// ��־
	//	{
	//		if(CTGS_ASS_LOG_POINT==iAssistantCode)	// ��־�����ָ��
	//		{
	//			if(pData)
	//			{
	//				//m_pRoomLog = (cnd_ILogMgr*)pData;

	//				// ����ÿ�����Ӷ��ᵽ�ˣ���ֻ��дһ��
	//				static bool bArrow = true;
	//				if(bArrow/* && m_pRoomLog*/)
	//				{
	//					bArrow = !bArrow;

	//					//m_pRoomLog->AddMsg(0, 0, TRUE, "***** ���ǡ�תת�齫����Ϸ�������־");
	//					//m_pRoomLog->AddMsg(0, 0, TRUE, "***** ���ļ�����������ʱ��ʼ��¼");
	//					//m_pRoomLog->AddMsg(0, 0, TRUE, "***** ��ֹͣ������߹ҵ�ʱ��ֹ");
	//					//m_pRoomLog->AddMsg(0, 0, TRUE, "***** ÿ����¼ǰ���Ǽ�¼ʱ��");
	//					//m_pRoomLog->AddMsg(0, 0, TRUE, "***** ���濪ʼ����������ļ�¼");
	//				}
	//			}
	//		}

	//		break;
	//	}
	//default:
	//	{
	//		break;
	//	}
	//}

	return true;
}

//��ʼ��
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ʼģʽ//////////////////
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);//add by hxh 20170321
	return true;
}

//��λ����
void  CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	//m_wMPIndex=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_llMuscle,sizeof(m_llMuscle));

	CString str;
	str.Format(_T("server: RepositTableFrameSink 140---%+I64d,%+I64d,%+I64d,%+I64d"), m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
	OutputDebugString(str);

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	//ZeroMemory(m_bHearStatus,sizeof(m_bHearStatus));
	//ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	m_wHaiDiUser=INVALID_CHAIR;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	//���Ͽ���
	//memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));


	//osc update
	//memset(m_bIsUserFirstOutCard, true, sizeof(m_bIsUserFirstOutCard));
	//memset(m_bUserFirstIsTingPai, 0, sizeof(m_bUserFirstIsTingPai));
	memset(m_byUserHuCardType, 0, sizeof(m_byUserHuCardType));
	//m_iSaveGameOutCardCount = 0;
	//m_iSaveGameSendCardCount = 0;
	m_byGameEndType = 0;
	return;
}

////��ʼģʽ
//enStartMode  CTableFrameSink::GetGameStartMode()
//{
//	return m_GameStartMode;
//}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}


//��ʼ������
void CTableFrameSink::InitData()
{
	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	//m_wMPIndex=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_llMuscle,sizeof(m_llMuscle));

	CString str;
	str.Format(_T("server: InitData 216---%+I64d,%+I64d,%+I64d,%+I64d"), m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
	OutputDebugString(str);

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	//ZeroMemory(m_bHearStatus,sizeof(m_bHearStatus));
	//ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	m_wHaiDiUser=INVALID_CHAIR;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	//���Ͽ���
	//memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));

	//osc update
	//memset(m_bIsUserFirstOutCard, true, sizeof(m_bIsUserFirstOutCard));
	//memset(m_bUserFirstIsTingPai, 0, sizeof(m_bUserFirstIsTingPai));
	memset(m_byUserHuCardType, 0, sizeof(m_byUserHuCardType));
	//m_iSaveGameOutCardCount = 0;
	//m_iSaveGameSendCardCount = 0;
	m_byGameEndType = 0;

	//memset(m_cbKingCard, 0, sizeof(m_cbKingCard));
	memset(m_cbKingCount, 0, sizeof(m_cbKingCount));
	//m_cbMoCard = 0;
	m_wCurrentUser = INVALID_CHAIR;


	memset(m_byGangCard, 0, sizeof(m_byGangCard));
	memset(m_byGangCardNums, 0, sizeof(m_byGangCardNums));
	memset(m_byBuCard, 0, sizeof(m_byBuCard));
	memset(m_byBuCardNums, 0, sizeof(m_byBuCardNums));

	m_bIsPlayDice=true;

}
//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	InitData();

#ifdef TOOL_SEND_CARD
	TCHAR chDirWork[1024]={0};
	//���ù���Ŀ¼
	GetModuleFileName(GetModuleHandle(NULL),chDirWork,sizeof(chDirWork));
	CString strDirWorkPath=_T("");
	strDirWorkPath.ReleaseBuffer();
	strDirWorkPath.Append(chDirWork);
	int nPos = 0; 
	nPos=strDirWorkPath.ReverseFind('\\'); 
	strDirWorkPath=strDirWorkPath.Left(nPos);
	strDirWorkPath.Append(_T("\\MJCard.ini"));
	m_SendCardTool.SetFilePath(strDirWorkPath);
#endif

	if (m_wBankerUser<0 || m_wBankerUser>=GAME_PLAYER)
	{
		m_wBankerUser = rand()%GAME_PLAYER;
	}

	//�����˿�
	int iNum1=0,iNum2=0;
#ifdef TOOL_SEND_CARD
	m_SendCardTool.GetSZNums(iNum1, iNum2);
#endif
	if (0==iNum1 || 0==iNum2)
	{
		iNum1 = rand()%6+1;
		iNum2 = rand()%6+1;
	}
	m_wSiceCount=MAKEWORD(iNum1,iNum2);//���������
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

#ifdef TOOL_SEND_CARD
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		BYTE byCardData[MAX_COUNT-1]={0};
		int iCardNums = m_SendCardTool.GetCardData(i, byCardData, MAX_COUNT-1);
		if (iCardNums==MAX_COUNT-1)
		{
			m_GameLogic.SwitchToCardIndex(byCardData,MAX_COUNT-1,m_cbCardIndex[i]);
		}
		else 
		{
			m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
		}
	}
#else
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}
#endif


#ifdef TOOL_SEND_CARD
	//�ı���ǽ����
	BYTE byWallCard[MAX_REPERTORY]={0};
	int iCardNums = m_SendCardTool.GetCardWallData(byWallCard, MAX_REPERTORY);
	if (iCardNums>0)
	{
		::memcpy(&m_cbRepertoryCard[m_cbLeftCardCount-1], byWallCard, iCardNums);
	}
#endif

	//�����˿�
	BYTE byNextCard=0;
#ifdef TOOL_SEND_CARD
	byNextCard = m_SendCardTool.GetNextCard();
#endif
	--m_cbLeftCardCount;
	if (0==byNextCard || 255==byNextCard)
	{
		byNextCard = m_cbRepertoryCard[m_cbLeftCardCount];
	}
	m_cbSendCardCount++;
	m_cbSendCardData=byNextCard;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//���ñ���
	m_cbProvideCard=0;
	m_wProvideUser=INVALID_CHAIR;
	m_wCurrentUser=m_wBankerUser;

	CString str;
	str.Format(_T("server: OnEventGameStart() 538-- m_wCurrentUser %d"), m_wCurrentUser);
	OutputDebugString(str);

	//��������
	tagGangCardResult GangCardResult;
	BYTE byGangCard[MAX_INDEX]={0};
	BYTE byGangCardNums = 0;
	//bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbUserAction[i] = WIK_NULL;
		m_byUserHuCardType[i] = WIK_NULL;
		//m_bUserFirstIsTingPai[i] = false;
		//m_bIsUserFirstOutCard[i] = true; 

		//ׯ���ж�
		if (i==m_wBankerUser)
		{
			memset(byGangCard, 0, sizeof(byGangCard));
			byGangCardNums = 0;
			//�����ж�
			memset(&GangCardResult, 0, sizeof(tagGangCardResult));
			m_GameLogic.AnalyseGangCard(m_cbCardIndex[i],NULL,0,GangCardResult);
			//if (GangCardResult.cbCardCount>=1)
			//{
			//	m_GameLogic.EstimateTingCard(m_cbCardIndex[i], NULL, 0, byGangCard, byGangCardNums);
			//}

			if (byGangCardNums>0)
			{
				m_cbUserAction[i] |= WIK_TING;
			}
			if (GangCardResult.cbCardCount>=1)
			{
				m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
			}

			//�����ж�
			tagChiHuResult ChiHuResult;
			//BYTE cbReCardIndex[MAX_INDEX];
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],NULL,0,0,0,ChiHuResult);
			//m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],NULL,0,0,0,ChiHuResult,m_cbKingCard,1,cbReCardIndex);
		}

		////״̬����
		//if ((bAroseAction==false)&&(i!=m_wBankerUser)&&(m_cbUserAction[i]!=WIK_NULL))
		//{
		//	bAroseAction=true;
		//	m_wResumeUser=m_wCurrentUser;
		//	m_wCurrentUser=INVALID_CHAIR;
		//}
	}

	//osc update
	m_wCurrentUser=m_wBankerUser;
	m_wProvideUser=m_wBankerUser;
	m_wResumeUser=m_wBankerUser;

	//��������
	CMD_S_GameStart GameStart;
	memset(&GameStart, 0, sizeof(CMD_S_GameStart));
	GameStart.wSiceCount=m_wSiceCount;
	//GameStart.wMPIndex=m_wMPIndex;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;     
	CopyMemory(GameStart.bTrustee,m_bTrustee,sizeof(m_bTrustee));
	//GameStart.cbKingCard = m_cbKingCard[0];

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//���ñ���
		memset(GameStart.byGangCard, 0, sizeof(GameStart.byGangCard));
		memset(GameStart.byBuCard, 0, sizeof(GameStart.byBuCard));
		GameStart.byGangCardNums = 0;
		GameStart.byBuCardNums = 0;
		GameStart.cbUserAction=m_cbUserAction[i];

		//��ʼ������ֵ
		memset(m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
		m_byGangCardNums[i] = 0;
		memset(m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
		m_byBuCardNums[i] = 0;

		if (i == m_wBankerUser)
		{
			if (byGangCardNums>0 && byGangCardNums<=4)
			{
				memcpy(GameStart.byGangCard, byGangCard, sizeof(BYTE)*byGangCardNums);
				GameStart.byGangCardNums = byGangCardNums;

				memcpy(m_byGangCard[i], byGangCard, sizeof(BYTE)*byGangCardNums);
				m_byGangCardNums[i] = byGangCardNums;
			}
			if (GangCardResult.cbCardCount>=1)
			{
				memcpy(GameStart.byBuCard, GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
				GameStart.byBuCardNums = GangCardResult.cbCardCount;

				memcpy(m_byBuCard[i], GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
				m_byBuCardNums[i] = GangCardResult.cbCardCount;
			}
		}

		//����
		memset(GameStart.cbCardData, 0, sizeof(GameStart.cbCardData));
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(CMD_S_GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(CMD_S_GameStart));
	}

	return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	CString str;
	str.Format(_T("server: OnEventGameConclude 637---cbReason %d---%+I64d,%+I64d,%+I64d,%+I64d"), cbReason, m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
	OutputDebugString(str);

	m_bIsPlayDice=false;
	::memset(m_bTrustee, 0, sizeof(m_bTrustee));
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			LONGLONG lCellScore=m_pGameServiceOption->lCellScore;
			GameEnd.bIsHuangzhuang = false;

			//������Ϣ
			//GameEnd.bKingCard=m_cbKingCard[0];
			GameEnd.wProvideUser=wChairID;
			GameEnd.cbChiHuCard=m_cbChiHuCard;
			GameEnd.cbHaiDiCard=m_cbRepertoryCard[0];
			GameEnd.iChiHuType=m_byGameEndType;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				GameEnd.wChiHuKind[i]=m_ChiHuResult[i].wChiHuKind;  //��������
				GameEnd.wChiHuRight[i]=m_ChiHuResult[i].wChiHuRight; //����Ȩλ
				GameEnd.cbKingCount[i]=m_ChiHuResult[i].m_bKingCount;  //���Ƹ���
			}

			CString str2;
			str2.Format(_T("server: OnEventGameConclude 678--m_byGameEndType %d,(%d,%d,%d,%d)"), m_byGameEndType,m_ChiHuResult[0].wChiHuKind,m_ChiHuResult[1].wChiHuKind,
				m_ChiHuResult[2].wChiHuKind,m_ChiHuResult[3].wChiHuKind);
			OutputDebugString(str2);

			//��������
			WORD wChiHuUserCount=0;

			//cbChiHuOrder Ϊ����������еķ���

			//ͳ�ƻ���  ������ץ�ڣ��������ͣ������ƣ�
			if (m_wProvideUser!=INVALID_CHAIR  &&  0!=m_byGameEndType &&  5!=m_byGameEndType)
			{
				OutputDebugString(_T("server: OnEventGameConclude : ���˺���"));
				//���˺���
				//��������
				if ((1==m_byGameEndType || 3==m_byGameEndType)   &&   (m_ChiHuResult[m_wProvideUser].wChiHuKind!=CHK_NULL))
				{
					//����,����������
					OutputDebugString(_T("server: OnEventGameConclude : ���˺��� ����"));
					////��������
					//BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[m_wProvideUser]);
					//if(0==GameEnd.cbKingCount[m_wProvideUser] || 3==GameEnd.cbKingCount[m_wProvideUser])
					//{//������
					//	cbChiHuOrder+=1;
					//}
					//else if(4==GameEnd.cbKingCount[m_wProvideUser])
					//{//�ĸ���
					//	cbChiHuOrder+=2;
					//}
					//if (m_bUserFirstIsTingPai[m_wProvideUser])
					//{//��һ�ֱ���
					//	cbChiHuOrder+=1;
					//}
					//if (0==cbChiHuOrder) cbChiHuOrder=1;
					//cbChiHuOrder++;//������

					ZeroMemory(GameEnd.iMuscle, sizeof(GameEnd.iMuscle));
					GameEnd.iMuscle[m_wProvideUser] = m_llMuscle[m_wProvideUser];
					// ����(�������� ���� + ������ҵĽ���Ŀ) * �׷�
					GameEnd.cbTimes[m_wProvideUser] = (m_llMuscle[m_wProvideUser] + 2) * (GAME_PLAYER-1);
					GameEnd.lGameScore[m_wProvideUser] = lCellScore * GameEnd.cbTimes[m_wProvideUser];
					for (BYTE i=0; i<GAME_PLAYER; i++)
					{
						//Ӯ�ҹ���
						if (i == m_wProvideUser)
						{
							continue;
						}

						// ����3�Ҹ������׷�*2����Ǯ����ע������������ң�֮ǰ�õĽ����֣�
						GameEnd.cbTimes[i] = - (m_llMuscle[m_wProvideUser]+2);
						GameEnd.lGameScore[i] = lCellScore*GameEnd.cbTimes[i];
					}

					//for (WORD i=0;i<m_wPlayerCount;i++)
					//{
					////Ӯ�ҹ���
					//if (i==m_wProvideUser) continue;

					//GameEnd.lGameScore[i]-=((i==m_wBankerUser) || (m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
					//GameEnd.lGameScore[m_wProvideUser]+=(i==m_wBankerUser || (m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
					////����ͳ��
					////GameEnd.cbTimes[i]=((i==m_wBankerUser) || (m_wProvideUser==m_wBankerUser))?(-2*cbChiHuOrder) : (-cbChiHuOrder);
					////GameEnd.cbTimes[m_wProvideUser]+=(i==m_wBankerUser || (m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder) : (cbChiHuOrder);
					//GameEnd.cbTimes[i]=GameEnd.lGameScore[i]/lCellScore;
					//GameEnd.cbTimes[m_wProvideUser]=(GameEnd.lGameScore[m_wProvideUser]/lCellScore);
					//}
					GameEnd.iChiHuKind[m_wProvideUser]=1;

					//������Ŀ
					wChiHuUserCount=1;

					//ׯ������
					m_wBankerUser=m_wProvideUser;

				}
				else if (2==m_byGameEndType || 4==m_byGameEndType) //׽������
				{
					OutputDebugString(_T("server: OnEventGameConclude 763: ���˺��� ����"));
					//����, m_wProvideUserΪ���ڵ����
					//��������
					WORD wNextBankerUser=INVALID_CHAIR;
					LONGLONG llAllMuscle = 0; //����ӮǮ��ҵĽ�
					GameEnd.iChiHuKind[m_wProvideUser]=3;

					ZeroMemory(GameEnd.iMuscle, sizeof(GameEnd.iMuscle));

					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						// ��ҹ���
						if (m_ChiHuResult[i].wChiHuKind==CHK_NULL)
						{
							continue;
						}

						GameEnd.iMuscle[i] = m_llMuscle[i];
						// ��һ��, ��Ϊ���˷��� (ʵ�ʷ���=����Ŀ+����һ��)
						GameEnd.cbTimes[i] = GameEnd.iMuscle[i] + 1;
						GameEnd.lGameScore[i] = lCellScore*GameEnd.cbTimes[i];
						GameEnd.iChiHuKind[i] = 2;

						// ��Ǯ������ϵĽ���Ŀ + ����һ��
						llAllMuscle += GameEnd.iMuscle[i]+1;
					}

					// ��������Ҫ��� �� ��Ŀ
					GameEnd.cbTimes[m_wProvideUser] = -llAllMuscle; //(������ �� ����)
					GameEnd.lGameScore[m_wProvideUser] = lCellScore*GameEnd.cbTimes[m_wProvideUser];

					//ѭ���ۼ�
					//for (WORD i=0;i<m_wPlayerCount;i++)
					//{
					//	//��ҹ���
					//	if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) continue;

					//	//��������
					//	BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[i]);
					//	if(0==GameEnd.cbKingCount[i] || 3==GameEnd.cbKingCount[i])
					//	{//������
					//		cbChiHuOrder+=1;
					//	}
					//	else if(4==GameEnd.cbKingCount[i])
					//	{//�ĸ���
					//		cbChiHuOrder+=2;
					//	}
					//	if (m_bUserFirstIsTingPai[i])
					//	{//��һ�ֱ���
					//		cbChiHuOrder+=1;
					//	}
					//	if (0==cbChiHuOrder) cbChiHuOrder=1;

					//	GameEnd.lGameScore[i]+=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))? (2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
					//	GameEnd.lGameScore[m_wProvideUser]-=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
					//	//GameEnd.cbTimes[m_wProvideUser]=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))? (-2*cbChiHuOrder) : (-cbChiHuOrder);
					//	//GameEnd.cbTimes[i]=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder) : (cbChiHuOrder);
					//	GameEnd.cbTimes[m_wProvideUser]=GameEnd.lGameScore[m_wProvideUser]/lCellScore;
					//	GameEnd.cbTimes[i]=(GameEnd.lGameScore[i]/lCellScore);
					//	//GameEnd.iChiHuKind[i]=2;
					//	//���ñ���
					//	wNextBankerUser=i;
					//	wChiHuUserCount++;
					//}


					//ׯ������
					m_wBankerUser=(wChiHuUserCount>=2)?m_wProvideUser:wNextBankerUser;
					if ((m_wHaiDiUser!=INVALID_CHAIR)&&(wChiHuUserCount>1)) m_wBankerUser=m_wHaiDiUser;
				}

				//�����˿�
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
				}
			}
			else
			{
				OutputDebugString(_T("server: OnEventGameConclude : ��ׯ����"));
				//��ׯ����
				GameEnd.bIsHuangzhuang = true;
				GameEnd.cbChiHuCard=0;
				GameEnd.cbHaiDiCard=m_cbRepertoryCard[0];

				//����ׯ��
				m_wBankerUser=(m_wHaiDiUser!=INVALID_CHAIR)?m_wHaiDiUser:m_wResumeUser;

				//�����˿�
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					GameEnd.cbCardCount[i]=0;
				}
			}

			//��������
			LONGLONG lRevenue=0L;  //�����
			LONGLONG lUserScore[GAME_PLAYER]={0};
			ZeroMemory(lUserScore,sizeof(lUserScore));
			CopyMemory(lUserScore,GameEnd.lGameScore,sizeof(lUserScore));

			CString str1;
			str1.Format(_T("server: OnEventGameConclude 865--(%I64d,%I64d,%I64d,%I64d)"), GameEnd.lGameScore[0],GameEnd.lGameScore[1],
				GameEnd.lGameScore[2],GameEnd.lGameScore[3]);
			OutputDebugString(str1);


			//д��
			for(int i=0;i<GAME_PLAYER;i++)
			{
				if(lUserScore[i]>0)
				{
					lRevenue=lUserScore[i]*(m_pGameServiceOption->cbRevenueRatio/100.0f);
					GameEnd.lGameScore[i]=lUserScore[i]-lRevenue;
				}
				else
				{
					lRevenue = 0;
				}

				GameEnd.lGameScore[i]=lUserScore[i]-lRevenue;
				GameEnd.iCellScore[i]=0;

				//д�����
				LONGLONG lScore=GameEnd.lGameScore[i];
				BYTE ScoreKind;
				if(lScore>0L)
				{
					ScoreKind=SCORE_TYPE_WIN;
					GameEnd.iCellScore[i] = lScore*(0.01*m_pGameServiceOption->cbRevenueRatio);
				}

				else if(lScore==0L) 	
				{
					ScoreKind=SCORE_TYPE_DRAW;
				}
				else				
				{
					ScoreKind=SCORE_TYPE_LOSE;
				}

				CString str;
				str.Format(_T("server: OnEventGameConclude 899---%d,(%I64d,%I64d),%d"), i,GameEnd.lGameScore[i],lRevenue,ScoreKind);
				OutputDebugString(str);

				///m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lRevenue,ScoreKind);

				////��������ж�
				//if (enScoreKind_Win==ScoreKind)
				//{
				//	OnUserTaskJudgement(i, 1);
				//}

			}

			CString str;
			str.Format(_T("server: OnEventGameConclude 909---%d,%d"), m_pGameServiceOption->cbRevenueRatio,m_wProvideUser);
			OutputDebugString(str);
			str.Format(_T("server: OnEventGameConclude 889---lGameScore ��Ϸ����,%+I64d,%+I64d,%+I64d,%+I64d"), GameEnd.lGameScore[0],GameEnd.lGameScore[1],GameEnd.lGameScore[2],GameEnd.lGameScore[3]);
			OutputDebugString(str);
			str.Format(_T("server: OnEventGameConclude 890---iCellScore �����,%+I64d,%+I64d,%+I64d,%+I64d"), GameEnd.iCellScore[0],GameEnd.iCellScore[1],GameEnd.iCellScore[2],GameEnd.iCellScore[3]);
			OutputDebugString(str);
			str.Format(_T("server: OnEventGameConclude 891---iChiHuKind ��������,%d,%d,%d,%d"), GameEnd.iChiHuKind[0],GameEnd.iChiHuKind[1],GameEnd.iChiHuKind[2],GameEnd.iChiHuKind[3]);
			OutputDebugString(str);
			str.Format(_T("server: OnEventGameConclude 892---cbTimes ����,%d,%d,%d,%d"), GameEnd.cbTimes[0],GameEnd.cbTimes[1],GameEnd.cbTimes[2],GameEnd.cbTimes[3]);
			OutputDebugString(str);
			str.Format(_T("server: OnEventGameConclude 893---iMuscle ��,%d,%d,%d,%d"), GameEnd.iMuscle[0],GameEnd.iMuscle[1],GameEnd.iMuscle[2],GameEnd.iMuscle[3]);
			OutputDebugString(str);

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(CMD_S_GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(CMD_S_GameEnd));
			//������Ϸ
			///m_pITableFrame->ConcludeGame();
#if 1//def USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif

			if (m_wBankerUser>=GAME_PLAYER)
			{
				m_wBankerUser = INVALID_CHAIR;
			}
			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.bIsHuangzhuang = false;

			//��������
			LONGLONG lCellScore=m_pGameServiceOption->lCellScore;	//��Ԫ����
			DOUBLE lRestrictScore=m_pGameServiceOption->lRestrictScore;	//�������
			LONGLONG Revenue = m_pGameServiceOption->cbRevenueRatio;	//˰�ձ���
			LONGLONG lRevenue=0L;
			//lRevenue = (lRestrictScore/3)*(Revenue/100.0f);
			lRevenue = lCellScore;

			//���ñ���
			GameEnd.wProvideUser=INVALID_CHAIR;

			//GameEnd.bKingCard=m_cbKingCard[0];

			//ͳ�ƻ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (i==wChairID) 
				{
					GameEnd.lGameScore[i]=-lRestrictScore - lRevenue;
					GameEnd.IsUserLeft[i]=true;
				}
				else 
				{
					GameEnd.lGameScore[i]=lRestrictScore/3 - lRevenue;
					GameEnd.IsUserLeft[i]=false;
				}
			}


			CString str;
			str.Format(_T("server: OnEventGameConclude 949---%+I64d,%+I64d,%+I64d,%+I64d"), GameEnd.lGameScore[0],GameEnd.lGameScore[1],GameEnd.lGameScore[2],GameEnd.lGameScore[3]);
			OutputDebugString(str);

			////֪ͨ��Ϣ
			//TCHAR szMessage[128]=TEXT("");
			//_snprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetAccounts());
			//for (WORD i=0;i<m_wPlayerCount;i++)
			//{
			//	IServerUserItem * pISendUserItem=m_pITableFrame->GetServerUserItem(i);
			//	if (pISendUserItem!=NULL) m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			//}
			//WORD wIndex=0;
			//do
			//{
			//	IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			//	if (pISendUserItem==NULL) break;
			//	m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			//} while (true);


			//�޸Ļ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.iCellScore[i]=0;
				if (wChairID==i)
				{
					//m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],0L,enScoreKind_Flee);
				}
				else
				{
					//m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lRevenue,enScoreKind_Win);
				}
				if (GameEnd.lGameScore[i]>0)
				{
					GameEnd.iCellScore[i] = GameEnd.lGameScore[i]*(0.01*m_pGameServiceOption->cbRevenueRatio);
				}
			}

			CString str1;
			str1.Format(_T("server: OnEventGameConclude 989-- ������Ϣ SUB_S_GAME_END ǿ��"));
			OutputDebugString(str1);

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			///m_pITableFrame->ConcludeGame();
#if 1//def USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif

			m_wBankerUser = INVALID_CHAIR;
			return true;
		}
	}

	return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
	case GS_MJ_HAI_DI:	//����״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			bool bIsMeTingPai = false;
			//if (m_bIsUserFirstOutCard[wChiarID] && m_wOutCardUser==wChiarID)
			//{
			//	bool bIsTing = m_GameLogic.IsCanTingCard(m_cbCardIndex[wChiarID], m_WeaveItemArray[wChiarID], m_cbWeaveItemCount[wChiarID], m_cbKingCard);
			//	if (bIsTing)
			//	{
			//		bIsMeTingPai = true;
			//	}
			//}
			StatusPlay.m_bTingCard=bIsMeTingPai;				

			//����
			//StatusPlay.byKindCard = m_cbKingCard[0];

			StatusPlay.bIsPlayDice=m_bIsPlayDice;

			//��Ϸ����
			StatusPlay.wSiceCount=m_wSiceCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			StatusPlay.bIsAlreadyOper = m_bResponse[wChiarID];

			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			//StatusPlay.cbHearStatus=(m_bHearStatus[wChiarID]==true)?TRUE:FALSE;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//����˿�
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//�˿�����
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);

			//�ܲ���Ϣ
			memcpy(StatusPlay.byGangCard, m_byGangCard[wChiarID], sizeof(BYTE)*m_byGangCardNums[wChiarID]);
			StatusPlay.byGangCardNums = m_byGangCardNums[wChiarID];
			memcpy(StatusPlay.byBuCard, m_byBuCard[wChiarID], sizeof(BYTE)*m_byBuCardNums[wChiarID]);
			StatusPlay.byBuCardNums = m_byBuCardNums[wChiarID];

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	CString strBuffer;
	tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
	strBuffer.Format(_T("server: CTableFrameSink::OnGameMessage() wSubCmdID=%d, wDataSize=%d, wChairID=%d"), wSubCmdID, wDataSize, pUserData->wChairID); 
	OutputDebugString(strBuffer);

	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	//case SUB_C_OPERATE_HAI_DI:	//���ײ���
	//	{
	//		//Ч����Ϣ
	//		ASSERT(wDataSize==sizeof(CMD_C_OperateHaiDi));
	//		if (wDataSize!=sizeof(CMD_C_OperateHaiDi)) return false;

	//		//�û�Ч��
	//		tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	//		if (pUserData->cbUserStatus!=US_PLAY) return true;

	//		//��Ϣ����
	//		CMD_C_OperateHaiDi * pOperateHaiDi=(CMD_C_OperateHaiDi *)pDataBuffer;
	//		return OnUserOperateHaiDi(pUserData->wChairID,pOperateHaiDi->cbTakeHaiDi);
	//	}
	case SUB_C_TRUSTEE:			//�йܲ���
		{
			//TestGameEndFun(0);return true;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo/*GetUserData*/();

			m_bTrustee[pUserData->wChairID]=pTrustee->bTrustee;
			CMD_S_Trustee Trustee;
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pUserData->wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			return true;
		}
	case SUB_C_USERTINGCARD:	//���Ʋ���
		{
			if(wDataSize != sizeof(CMD_UserTingOperate)) return false;
			CMD_UserTingOperate *TingOperate =(CMD_UserTingOperate *)pDataBuffer;

			//�û�Ч��
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			return OnUserTingCard(pIServerUserItem->GetChairID(), TingOperate->bTingCard);
		}
	case SUB_C_SZSTART:
		{
			m_bIsPlayDice=false;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_C_SZSTART);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_C_SZSTART);
		}
		break;
	default:
		break;
	}

	return false;
}

//�û����Ʋ���
bool CTableFrameSink::OnUserTingCard(WORD wChairID, bool bIsTing)
{
	//if (wChairID>=GAME_PLAYER || !m_bIsUserFirstOutCard[wChairID])
	{
		return false;
	}

	//bool bRetIsTing = m_GameLogic.IsCanTingCard(m_cbCardIndex[wChairID], m_WeaveItemArray[wChairID], m_cbWeaveItemCount[wChairID], m_cbKingCard);
	//if (bRetIsTing)
	//{
	//	m_bIsUserFirstOutCard[wChairID] = false;
	//	m_bUserFirstIsTingPai[wChairID] = bIsTing; //�ǲ��ǵ�һ������.
	//	m_bHearStatus[wChairID] = bIsTing;

	//	if (bIsTing)
	//	{
	//		//���߿ͻ����û�ĳ����������ˡ�
	//		CMD_S_GameTingCard userting;
	//		userting.wTingCardUser = wChairID;
	//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_TINGCARD_RESULT,&userting,sizeof(CMD_S_GameTingCard));
	//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_TINGCARD_RESULT,&userting,sizeof(CMD_S_GameTingCard));
	//	}
	//	//�û���ʼ����
	//	UserStartOperation();
	//}

	return true;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	////ׯ������
	//if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	//{
	//	m_wBankerUser=pIServerUserItem->GetChairID();
	//}

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	////ׯ������
	//if ((bLookonUser==false)&&(wChairID==m_wBankerUser))
	//{
	//	m_wBankerUser=INVALID_CHAIR;

	//	for (WORD i=0;i<m_wPlayerCount;i++)
	//	{
	//		if ((i!=wChairID)&&(m_pITableFrame->GetServerUserItem(i)!=NULL))
	//		{
	//			m_wBankerUser=i;
	//			m_bTrustee[wChairID]=false;
	//			CMD_S_Trustee Trustee;
	//			Trustee.bTrustee=false;
	//			Trustee.wChairID = wChairID;
	//			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	//			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));


	//			break;
	//		}
	//	}
	//}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) 
	{
		return true;
	}

	//Ч�����
	if (wChairID!=m_wCurrentUser)
	{
		return false;
	}
	if (m_GameLogic.IsValidCard(cbCardData)==false)
	{
		return false;
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_bIsPlayDice=false;
	m_bSendStatus=true;
	m_bGangStatus=false;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//���Ƽ�¼
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//�û��л�
	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	//if (m_bIsUserFirstOutCard[wChairID])
	//{//�ǲ��ǵ�һȦ
	//bool bIsTing = m_GameLogic.IsCanTingCard(m_cbCardIndex[wChairID], m_WeaveItemArray[wChairID], m_cbWeaveItemCount[wChairID], m_cbKingCard);
	//if (bIsTing)
	//{
	//	CMD_S_GameTingCard tingcard;
	//	tingcard.wTingCardUser = wChairID;
	//	m_pITableFrame->SendTableData(wChairID,SUB_S_TING_CARD,&tingcard,sizeof(CMD_S_GameTingCard));
	//	return true;
	//}
	//else
	//{
	//m_bIsUserFirstOutCard[wChairID] = false;
	//}
	//}

	//�û���ʼ����
	UserStartOperation();

	return true;
}

//�û�����
void CTableFrameSink::UserStartOperation()
{
	//��Ӧ�ж�
	bool bAroseAction=EstimateUserRespond(m_wProvideUser,m_cbProvideCard,EstimatKind_OutCard);

	//��������
	//if (m_bGangStatus)
	//{
	//	for (WORD i=0;i<m_wPlayerCount;i++)
	//	{
	//		if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) break;
	//	}
	//	
	//	if (i==m_wPlayerCount) 
	//	{
	//		m_bGangStatus=false;
	//	}
	//}

	//�ɷ��˿ˣ���һ���ƣ�������Ҳ��ܲ�������������������ʱ����
	if (bAroseAction==false) 
	{
		m_bSendStatus = true;
		DispatchCardData(m_wCurrentUser, false);
	}
}

bool CTableFrameSink::OnWriteMuscleScore(WORD wWinChairID, WORD wLostChairID, int iMuscle, LONGLONG llResultGold[])
{
	ASSERT(0<=wWinChairID  &&  GAME_PLAYER>wWinChairID);
	ASSERT(0<=wLostChairID  &&  GAME_PLAYER>wLostChairID);
	if (0>wWinChairID || GAME_PLAYER<=wWinChairID || 0>wLostChairID || GAME_PLAYER<=wLostChairID)
	{
		CString strBuffer;
		strBuffer.Format(_T("Error: CTableFrameSink::OnWriteMuscleScore(WORD,WORD,int) wWinChairID=%d, wLostChairID=%d, iMuscle=%d"), 
			wWinChairID, wLostChairID, iMuscle);
		OutputDebugString(strBuffer);
		return false;
	}

	if (0 > iMuscle)
	{
		iMuscle *=  -1;
	}

	++m_llMuscle[wWinChairID];
	ZeroMemory(llResultGold, sizeof(LONGLONG)*4);
	llResultGold[wWinChairID] = iMuscle*m_pGameServiceOption->lCellScore*(1-0.01*m_pGameServiceOption->cbRevenueRatio);//��˰
	llResultGold[wLostChairID] = -(iMuscle*m_pGameServiceOption->lCellScore);

/*	m_pITableFrame->WriteUserScore(wWinChairID, llResultGold[wWinChairID], 0, enScoreKind_Present);
	m_pITableFrame->WriteUserScore(wLostChairID, llResultGold[wLostChairID], 0, enScoreKind_Present);*/		
	return true;
}

bool CTableFrameSink::OnWriteMuscleScore(WORD wWinChairID, int iMuscle, LONGLONG llResultGold[])
{
	ASSERT(0<=wWinChairID  &&  GAME_PLAYER>wWinChairID);
	if (0>wWinChairID || GAME_PLAYER<=wWinChairID)
	{
		CString strBuffer;
		strBuffer.Format(_T("Error: CTableFrameSink::OnWriteMuscleScore(WORD,int) wWinChairID=%d, iMuscle=%d"), wWinChairID, iMuscle);
		OutputDebugString(strBuffer);
		return false;
	}

	if (0 > iMuscle)
	{
		iMuscle *=  -1;
	}

	++m_llMuscle[wWinChairID];
	ZeroMemory(llResultGold, sizeof(LONGLONG)*4);
	llResultGold[wWinChairID] = iMuscle*m_pGameServiceOption->lCellScore*(1-0.01*m_pGameServiceOption->cbRevenueRatio);//��˰

	//m_pITableFrame->WriteUserScore(wWinChairID, iMuscle*m_pGameServiceOption->lCellScore, 0, enScoreKind_Present);

	for (WORD wID=0; wID<GAME_PLAYER; wID++)
	{
		if (wID == wWinChairID)
		{
			continue;
		}

		llResultGold[wID] = -(iMuscle/(GAME_PLAYER-1)*m_pGameServiceOption->lCellScore);
		//m_pITableFrame->WriteUserScore(wID, llResultGold[wID], 0, enScoreKind_Present);
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//Ч��״̬�����еĲ���ֻ������Ϸ״̬��
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//Ч���û���wChairID==m_wCurrentUser��ʾ�Լ�������m_wCurrentUser==INVALID_CHAIR��ʾ������Ҳ�����
	// ��������ܲ�������������EstimateUserRespond������ m_wCurrentUser����INVALID_CHAIR
	// ������ͬʱ�����Լ��ܲ�������������ܲ����������������ġ�
	ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) return true;

	// û���ƣ���Ϸ����
	// ��Ϊû�к��׸������ץ���һ���Ƶģ��������ܺ���ȴ������������Ϸ����
	// ��������ڷ��ƺ����Ѿ�����
	if (WIK_NULL==cbOperateCode  &&  0>=m_cbLeftCardCount)
	{
		m_byGameEndType = 0; //����
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
		return true;
	}

	//ȡ����
	//if (WIK_NULL==cbOperateCode && 0==cbOperateCard)
	//{
	//	m_tagUserGSKUResult[wChairID].bIsGSKU = false;
	//}

	if (INVALID_CHAIR == m_wCurrentUser) //����������������һ��Ѳ�������ǰ��Ҳ����ң�
	{
		return OnOtherUserOperateCard(wChairID, cbOperateCode, cbOperateCard);
	}
	else if (wChairID == m_wCurrentUser) //�����������Լ���������ǰ������ң�
	{
		return OnCurUserOperateCard(wChairID, cbOperateCode, cbOperateCard);
	}

	return false;
}

/// ��ǰ��Ҳ���
bool CTableFrameSink::OnCurUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	// ��䶯���
	LONGLONG llReturnMuscleGold[GAME_PLAYER]={0};
	bool bBrightMuscleCanHu = false; //����ʱ�Ƿ�������ܺ���

	//Ч�����
	if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0)) return true;

	//�˿�Ч�飬��������Ҵ���ƵĺϷ���
	if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard)==false))
	{
		return false;  
	}

	//���ñ���
	m_bSendStatus=true;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//ִ�ж���
	switch (cbOperateCode)
	{
	case WIK_FILL:			//���Ʋ���
	case WIK_GANG:			//���Ʋ���
	//case WIK_TING:			//���Ʋ���
		{
			//��������
			//bool bAnGang = false;		//�ǲ��ǰ���
			BYTE cbWeaveIndex=0xFF;
			BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

			//���ƴ����û����������齫ֻ��һ����������ƣ����������ȥ����ƶѲ���
			if (m_cbCardIndex[wChairID][cbCardIndex]==1)
			{
				// ����������ƶ���Ѱ�ҿ����ܷ�
				for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
					if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
					{
						cbWeaveIndex=i;
						break;
					}
				}

				//Ч�鶯��
				if (cbWeaveIndex==0xFF)
				{
					return false;
				}

				//����˿ˣ���������������ƶѱ���
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;

				// ����ʱ��������к�����Ҫ��֪��������Ƿ���Ҫ�������û��Ը�������ʱ�������Ч
				for (BYTE i=0; i<GAME_PLAYER; i++)
				{
					if (i == wChairID)
						continue;

					m_cbUserAction[i] = NULL;
					//�Ժ��ж�
					tagChiHuResult ChiHuResult;
					BYTE cbWeaveCount=m_cbWeaveItemCount[i];
					m_cbUserAction[i]=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbOperateCard,0,ChiHuResult);
					if (WIK_NULL != m_cbUserAction[i])
					{
						bBrightMuscleCanHu = true;
					}
				}

				// ����ʱû������ܺ�����������ɹ�
				if ( !bBrightMuscleCanHu )
				{
					// ����
					OnWriteMuscleScore(wChairID, 3, llReturnMuscleGold);

					CString str;
					str.Format(_T("server: OnUserOperateCard 1641-%d-----%+I64d,%+I64d,%+I64d,%+I64d"), wChairID, m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
					OutputDebugString(str);
				}
			}
			else // �Լ��������Լ����齫�����Դ�������Ƽ���Ƿ��ܲ�
			{//�ܵ����
				//�˿�Ч��
				ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
				if (m_cbCardIndex[wChairID][cbCardIndex]!=4)
				{
					return false;
				}

				//���ñ���������˿ˣ���������������ƶѱ���
				cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
				m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				//bAnGang = true;

				// ����
				OnWriteMuscleScore(wChairID, 6, llReturnMuscleGold);

				CString str;
				str.Format(_T("server: OnUserOperateCard 1670-%d-----%+I64d,%+I64d,%+I64d,%+I64d"), wChairID, m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
				OutputDebugString(str);
			}

			//����״̬
			m_bGangStatus=true;
			//if (WIK_TING == cbOperateCode)
			//{//����
				//����״̬
				//m_bHearStatus[wChairID]=true;
				//m_bEnjoinChiPeng[wChairID]=true;
				//m_tagUserGSKUResult[wChairID].bIsGang = true;
				//if (m_bIsUserFirstOutCard[wChairID])
				//{
				//	m_bUserFirstIsTingPai[wChairID] = true; //���Է���
				//}
				//m_bIsUserFirstOutCard[wChairID] = false;
			//}

			//������
			CMD_S_OperateResult OperateResult;
			OperateResult.wOperateUser=wChairID;
			OperateResult.wProvideUser=wChairID;
			OperateResult.cbOperateCode=cbOperateCode;
			OperateResult.cbOperateCard=cbOperateCard;
			OperateResult.bBringMuscleCanHu=bBrightMuscleCanHu;
			OperateResult.bMuscle = ((1==m_cbCardIndex[wChairID][cbCardIndex]) ? 2 : 1);
			CopyMemory(OperateResult.llMuscleGold, llReturnMuscleGold, sizeof(llReturnMuscleGold));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

			//ɾ���˿ˣ��ղŲ��������齫��Ŀ��0���ﵽɾ���齫����
			m_cbCardIndex[wChairID][cbCardIndex]=0;

			// ����������ܺ��������ܺ��ĺ����Ƿ���Ҫ����
			// ��ǰ���������ݲ����������� m_OperateResult ��
			if ( bBrightMuscleCanHu )
			{
				m_wCurrentUser=INVALID_CHAIR;
				m_wResumeUser = wChairID;
				m_cbProvideCard = cbOperateCard;
				CopyMemory(&m_OperateResult, &OperateResult, sizeof(OperateResult));
				SendOperateNotify();
			}
			else
			{
				//�����˿�
				m_bSendStatus = true;
				DispatchCardData(wChairID, true);
			}

			return true;
		}
	case WIK_CHI_HU:		//�Ժ�����
		{
			//����Ȩλ
			//if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) 
			//	m_byUserHuCardType[wChairID]|=CHR_TIAN;

			//if ((wChairID == m_wProvideUser) && m_tagUserGSKUResult[wChairID].bIsGang
			//	/*&& (m_iSaveGameOutCardCount==m_cbOutCardCount) 
			//	&& (m_iSaveGameSendCardCount==m_cbSendCardCount)*/)
			//{//���Ͽ���
			//	m_tagUserGSKUResult[wChairID].bIsGSKU = true;
			//	m_byUserHuCardType[wChairID] |= CHR_QIANG_GANG;
			//}

			//��ͨ����
			//BYTE cbReCardIndex[MAX_INDEX];
			BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
			tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
			m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,
				cbWeaveItemCount,0,m_byUserHuCardType[wChairID],m_ChiHuResult[wChairID]);
			//m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,m_byUserHuCardType[wChairID],m_ChiHuResult[wChairID],m_cbKingCard,4,cbReCardIndex);

			//������Ϣ
			m_cbChiHuCard=m_cbProvideCard;

			//������Ϸ
			m_byGameEndType = 1; //����
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}
	default: break;
	}

	return true;
}

/// ������Ҳ���
bool CTableFrameSink::OnOtherUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//Ч��״̬
	//ASSERT(m_bResponse[wChairID]==false);
	//ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

	//Ч��״̬
	// ÿ�����ֻ�ܲ���һ�Σ�������������Ƿ����������ȷ���ܲ�����������Ҷ���������Ӧ
	if (m_bResponse[wChairID]==true)
		return true;

	// ��Ҳ���ȡ������������������¼���Ĳ����ǡ�û�в�������������ҷ��������ǷǷ�����
	if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) 
		return true;

	//��������
	WORD wTargetUser=wChairID;
	BYTE cbTargetAction=cbOperateCode;

	// ��䶯���
	LONGLONG llReturnMuscleGold[GAME_PLAYER]={0};
	bool bBrightMuscleCanHu = false; //����ʱ�Ƿ�������ܺ���

	//���ñ���
	m_bResponse[wChairID]=true;
	m_cbPerformAction[wChairID]=cbOperateCode;
	// ��ҿ��Բ��������Ǵ����������ݷǷ������øմ�������滻����Ϊ�������ƴ����Ķ���
	m_cbOperateCard[wChairID]=(0==cbOperateCard)?m_cbProvideCard:cbOperateCard;

	//bool bIsOtherGangAction = false;
	//ִ���жϣ�ѭ���ж϶���Ȩ�ޣ�Ȩ��������ң�ID��Ȩ�޷ֱ𱣴��� wTargetUser��cbTargetAction��
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ��Ҳ����Ķ������� �� �����Ķ���
		BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

		//if ((cbUserAction&WIK_TING) > 0)
		//{
		//	bIsOtherGangAction = true;
		//}

		//���ȼ���
		BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
		BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

		//�����ж�
		if (cbUserActionRank>cbTargetActionRank)
		{
			wTargetUser=i;//����Ȩ���������
			cbTargetAction=cbUserAction;
		}
	}

	// Ȩ��������һ�û����
	if (m_bResponse[wTargetUser]==false) 
	{
		return true;
	}

	// ������Ȩ���Ǻ������ж��ܺ���������Ҷ�������û, ����û�в����ģ������ȴ�
	if (WIK_CHI_HU == cbTargetAction)//�Ժ��ȴ�
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU)) 
				return true;
		}
	}
	else if (WIK_NULL == cbTargetAction) //��Ȼ����в��������Ƕ��Ƿ���
	{
		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//��ʼ��
		for(int i=0; i<m_wPlayerCount; i++)
		{
			m_cbUserAction[i] = WIK_NULL;
			::memset(&m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
			m_byGangCardNums[i] = 0;
			::memset(&m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
			m_byBuCardNums[i] = 0;
		}

		// ����ʱ������ܺ�������������û�к�������֮ǰ��������Ȼ��Ч
		if (m_OperateResult.bBringMuscleCanHu)
		{
			OnWriteMuscleScore(m_OperateResult.wOperateUser, 3, llReturnMuscleGold);

			CString str;
			str.Format(_T("server: OnUserOperateCard 1641-�����ܺ�-���ڿ۽����-%d-----%+I64d,%+I64d,%+I64d,%+I64d"), m_OperateResult.wOperateUser, m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
			OutputDebugString(str);

			m_OperateResult.bBringMuscleCanHu = false;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&m_OperateResult,sizeof(m_OperateResult));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&m_OperateResult,sizeof(m_OperateResult));
			ZeroMemory(&m_OperateResult, sizeof(m_OperateResult));
		}

		//�����˿�
		m_bSendStatus = true;
		DispatchCardData(m_wResumeUser, false);//by han 20120104
		return true;
	}

	//��������
	BYTE cbTargetCard=m_cbOperateCard[wTargetUser];
	//���Ʋ���
	if (cbTargetAction==WIK_CHI_HU)
	{
		//������Ϣ
		m_cbChiHuCard=cbTargetCard;

		//�����жϣ�ѭ�������������Ƿ��ܺ��ƣ����������ܺ��ƣ�����Ϸ���������ڣ�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//BYTE bKingCount=0;
			//bKingCount=m_GameLogic.GetKingCardCount(m_cbCardIndex[i],NULL,0,m_cbKingCard,4);
			//if (0 != bKingCount)
			//{
			//	continue;
			//}
			//�����ж�
			if ((i==m_wProvideUser)||((m_cbPerformAction[i]&WIK_CHI_HU)==0))
				continue;

			//����Ȩλ
			//WORD wChiHuRight=0;
			//if ((1==m_cbSendCardCount)&&(1==m_cbOutCardCount))
			//{
			//	m_byUserHuCardType[i]|=CHR_DI;
			//}
			if (m_OperateResult.bBringMuscleCanHu
				/*m_tagUserGSKUResult[m_wOutCardUser].bIsGang*/ 
				/*&& (m_iSaveGameOutCardCount==m_cbOutCardCount-1)
				&& (m_iSaveGameSendCardCount==m_cbSendCardCount)*/)
			{//������
				m_byUserHuCardType[i]|=CHR_QIANG_GANG;
			}
			ZeroMemory(&m_OperateResult, sizeof(m_OperateResult));
			//if (bIsOtherGangAction)
			//{//���ܺ�
			//	m_byUserHuCardType[i]|=CHR_QIANG_GANG;
			//}

			//��ͨ����
			if (0 != m_cbChiHuCard)
			{
				//�����ж�
				//BYTE cbReCardIndex[MAX_INDEX];
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_byUserHuCardType[i],m_ChiHuResult[i]);
				//m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_byUserHuCardType[i],m_ChiHuResult[i],m_cbKingCard,4,cbReCardIndex);

				//�����˿�
				if (CHK_NULL != m_ChiHuResult[i].wChiHuKind)
				{
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
				}
			}
		}

		//������Ϸ
		ASSERT(CHK_NULL != m_ChiHuResult[wTargetUser].wChiHuKind);
		m_byGameEndType = 2; //����--���ܺ�
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
		return true;
	}

	//���Ʊ�����������Ѿ���������ֻ�����������Ȳ�����
	m_cbOutCardData=0;
	m_bSendStatus=true;
	m_wOutCardUser=INVALID_CHAIR;

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction)); 
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ASSERT(4 >= m_cbWeaveItemCount[wTargetUser]);
	if (4 < m_cbWeaveItemCount[wTargetUser])
		return true;

	WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
	m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
	m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
	m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
	m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

	//ɾ���˿�
	switch (cbTargetAction)
	{
		//case WIK_LEFT:		//���Ʋ���
		//	{
		//		//ɾ���˿�
		//		BYTE cbRemoveCard[]={cbTargetCard+1,cbTargetCard+2};
		//		m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

		//		break;
		//	}
		//case WIK_RIGHT:		//���Ʋ���
		//	{
		//		//ɾ���˿�
		//		BYTE cbRemoveCard[]={cbTargetCard-2,cbTargetCard-1};
		//		m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

		//		break;
		//	}
		//case WIK_CENTER:	//���Ʋ���
		//	{
		//		//ɾ���˿�
		//		BYTE cbRemoveCard[]={cbTargetCard-1,cbTargetCard+1};
		//		m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

		//		break;
		//	}
	case WIK_PENG:		//���Ʋ���
		{
			//ɾ���˿�
			BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
			m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

			break;
		}
	case WIK_FILL:		//���Ʋ���
	case WIK_GANG:		//���Ʋ���
		//case WIK_TING:		//���Ʋ���
		{
			//ɾ���˿ˣ�ׯ����Ϸ�տ�ʼץ��14���齫�����ʱ�����Ʋ���Ҫɾ��4���齫����OnEventGameStart()������ʵ��
			if (1==m_cbSendCardCount  &&  0==m_cbOutCardData)
			{
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard,cbTargetCard};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
			}
			else //���������������ֻ������
			{
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
			}

			// �Ž�
			OnWriteMuscleScore(wTargetUser, m_WeaveItemArray[wTargetUser][wIndex].wProvideUser, 3, llReturnMuscleGold);

			CString str;
			str.Format(_T("server: OnUserOperateCard 1512-%d,%d-----%+I64d,%+I64d,%+I64d,%+I64d"), wTargetUser, m_WeaveItemArray[wTargetUser][wIndex].wProvideUser,
				m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
			OutputDebugString(str);

			str.Format(_T("server: OnUserOperateCard 1749------%+I64d,%+I64d,%+I64d,%+I64d"), llReturnMuscleGold[0],llReturnMuscleGold[1],llReturnMuscleGold[2],llReturnMuscleGold[3]);
			OutputDebugString(str);
			break;
		}
	}

	//������
	CMD_S_OperateResult OperateResult;
	OperateResult.wOperateUser=wTargetUser;
	OperateResult.cbOperateCard=cbTargetCard;
	OperateResult.cbOperateCode=cbTargetAction;
	OperateResult.bMuscle = ((WIK_FILL==cbTargetAction || WIK_GANG==cbTargetAction) ? 3 : 0);
	OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
	CopyMemory(OperateResult.llMuscleGold, llReturnMuscleGold, sizeof(llReturnMuscleGold));

	//����״̬
	if (WIK_GANG==cbTargetAction  ||  WIK_FILL==cbTargetAction/*  ||  WIK_TING==cbTargetAction*/)
	{
		m_bGangStatus=true;
	}
	//if (cbTargetAction==WIK_TING)
	//{
	//m_bHearStatus[wTargetUser]=true;
	//m_bEnjoinChiPeng[wTargetUser]=true;
	//m_tagUserGSKUResult[wTargetUser].bIsGang = true;

	//if (m_bIsUserFirstOutCard[wChairID])
	//{
	//	m_bUserFirstIsTingPai[wChairID] = true; //���Է���
	//}
	//m_bIsUserFirstOutCard[wChairID] = false;
	//}

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

	//BYTE byStartID = m_wProvideUser;
	//while (byStartID<m_wPlayerCount)
	//{
	//	//m_bIsUserFirstOutCard[byStartID] = false;
	//	if (byStartID==wTargetUser)
	//	{
	//		break;
	//	}
	//	byStartID = (byStartID+m_wPlayerCount-1)%m_wPlayerCount;
	//}

	//�����û�
	m_wCurrentUser=wTargetUser;
	//���ƴ���
	if (WIK_GANG==cbTargetAction  ||  WIK_FILL==cbTargetAction/*  ||  WIK_TING==cbTargetAction*/)
	{
		//�������ܣ������Ƶ���ҷ�һ����
		m_bSendStatus = true;
		DispatchCardData(wTargetUser, true);
	}
	return true;
}

////���ײ���
//bool CTableFrameSink::OnUserOperateHaiDi(WORD wChairID, BYTE cbTakeHaiDi)
//{
//	//Ч��״̬
//	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_HAI_DI);
//	if (m_pITableFrame->GetGameStatus()!=GS_MJ_HAI_DI) return true;
//
//	//Ч���û�
//	ASSERT(wChairID==m_wCurrentUser);
//	if (wChairID!=m_wCurrentUser) return false;
//
//	//���״���
//	if (cbTakeHaiDi==1)
//	{
//		//�����ж�
//		//BYTE cbReCardIndex[MAX_INDEX];
//		BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
//		tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
//		m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,0,m_ChiHuResult[wChairID]);
//		//m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,0,m_ChiHuResult[wChairID],m_cbKingCard,4,cbReCardIndex);
//
//
//		//����ж�
//		if (m_ChiHuResult[wChairID].wChiHuKind==CHK_NULL)
//		{
//			//�����ж�
//			bool bOtherChiHu=false;
//			for (WORD i=0;i<m_wPlayerCount;i++)
//			{
//				//�����ж�
//				if (i==wChairID) continue;
//
//				//�����ж�
//				pWeaveItem=m_WeaveItemArray[i];
//				cbWeaveItemCount=m_cbWeaveItemCount[i];
//				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,0,m_ChiHuResult[i]);
//				//m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,0,m_ChiHuResult[i],m_cbKingCard,4,cbReCardIndex);	
//
//				//�������
//				if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL)
//				{
//					bOtherChiHu=true;
//					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
//					m_ChiHuResult[i].wChiHuRight |= CHR_HAI_DI;
//					m_ChiHuResult[i].wChiHuRight |= m_byUserHuCardType[i];
//				}
//			}
//
//			//������Ϸ
//			if (bOtherChiHu==false)
//			{
//				//��ׯ����
//				m_cbChiHuCard=0;
//				m_cbLeftCardCount=0;
//				m_wHaiDiUser=wChairID;
//				m_wProvideUser=INVALID_CHAIR;
//				m_byGameEndType = 5; //���׻�ׯ
//				OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
//			}
//			else
//			{
//				//���ڽ���
//				m_cbLeftCardCount=0;
//				m_wHaiDiUser=wChairID;
//				m_wProvideUser=wChairID;
//				m_cbChiHuCard=m_cbProvideCard;
//				m_byGameEndType = 4; //���׷���
//				OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
//			}
//
//		}
//		else
//		{//��Һ���
//
//			m_ChiHuResult[wChairID].wChiHuRight |= CHR_HAI_DI;
//			m_ChiHuResult[wChairID].wChiHuRight |= m_byUserHuCardType[wChairID];
//
//			//�����˿�
//			m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
//
//			//��������
//			m_cbLeftCardCount=0;
//			m_wHaiDiUser=wChairID;
//			m_wProvideUser=wChairID;
//			m_cbChiHuCard=m_cbProvideCard;
//			m_byGameEndType = 3; //��������
//			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
//		}
//	}
//	else
//	{
//		//�����û�
//		m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;
//
//		//��ׯ�ж�
//		if (m_wCurrentUser==m_wResumeUser)
//		{
//			m_cbChiHuCard=0;
//			m_wCurrentUser=INVALID_CHAIR;
//			m_wProvideUser=INVALID_CHAIR;
//			m_byGameEndType = 0; //���׻�ׯ
//			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
//
//			return true;
//		}
//
//		//��������
//		CMD_S_OperateHaiDi OperateHaiDi;
//		OperateHaiDi.wCurrentUser=m_wCurrentUser;
//
//		//������Ϣ
//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
//
//		return true;
//	}
//
//
//	return true;
//}

//���Ͳ���
bool CTableFrameSink::SendOperateNotify()
{
	//���Ϳ��Բ�������ʾ��������������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//��������
			CMD_S_OperateNotify OperateNotify;
			memset(&OperateNotify, 0, sizeof(CMD_S_OperateNotify));
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//if (m_bHearStatus[i])
			//{//������
			//	if ((OperateNotify.cbActionMask&WIK_CHI_HU)>0)
			//	{
			//		m_byGangCardNums[i] = 0;
			//		m_byBuCardNums[i] = 0;
			//		memset(m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
			//		memset(m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
			//		m_cbUserAction[i] = OperateNotify.cbActionMask = WIK_CHI_HU;
			//	}
			//}

			if (m_byGangCardNums[i]>0 && m_byGangCardNums[i]<=4)
			{//�ܡ���
				memcpy(OperateNotify.byGangCard, m_byGangCard[i], sizeof(BYTE)*m_byGangCardNums[i]);
				OperateNotify.byGangCardNums = m_byGangCardNums[i];
			}
			if (m_byBuCardNums[i]>=1)
			{//��
				memcpy(OperateNotify.byBuCard, m_byBuCard[i], sizeof(BYTE)*m_byBuCardNums[i]);
				OperateNotify.byBuCardNums = m_byBuCardNums[i];
			}

			//��������
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser, bool bSupplyCard)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) 
	{
		return false;
	}

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	CString strBuffer;
	strBuffer.Format(_T("Server: CTableFrameSink::DispatchCardData ʣ������Ŀ:%d"),m_cbLeftCardCount);
	OutputDebugString(strBuffer);

//	//�����ж�
//	if (m_cbLeftCardCount==1)
//	{
//		//���ñ���
//		m_wResumeUser=wCurrentUser;
//		m_wCurrentUser=wCurrentUser;
//		m_wProvideUser=INVALID_CHAIR;
//
//#ifdef TOOL_SEND_CARD
//		m_cbProvideCard = m_SendCardTool.GetNextCard();
//#else
//		m_cbProvideCard=m_cbRepertoryCard[0];
//#endif
//
//		//����״̬
//		m_pITableFrame->SetGameStatus(GS_MJ_HAI_DI);
//
//		//��������
//		CMD_S_OperateHaiDi OperateHaiDi;
//		OperateHaiDi.wCurrentUser=wCurrentUser;
//
//		//������Ϣ
//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
//
//		return true;
//	}

	//��ʼ��
	for(int i=0; i<m_wPlayerCount; i++)
	{
		m_cbUserAction[i] = WIK_NULL;
		::memset(&m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
		m_byGangCardNums[i] = 0;
		::memset(&m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
		m_byBuCardNums[i] = 0;
	}

	//���ƴ���
	m_cbUserAction[wCurrentUser] = WIK_NULL;
	tagGangCardResult GangCardResult;
	BYTE byGangCard[MAX_INDEX]={0};
	BYTE byGangCardNums = 0;
	memset(byGangCard, 0, sizeof(byGangCard));
	memset(&GangCardResult, 0, sizeof(tagGangCardResult));
	if (m_bSendStatus==true)
	{
		//�����˿�
		BYTE byNextCard=0;
#ifdef TOOL_SEND_CARD
		byNextCard = m_SendCardTool.GetNextCard();
#endif
		--m_cbLeftCardCount;
		if (0==byNextCard || 255==byNextCard)
		{
			byNextCard = m_cbRepertoryCard[m_cbLeftCardCount];
		}
		m_cbSendCardCount++;
		m_cbSendCardData=byNextCard;
		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

		//���ñ���
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;

		//if (m_tagUserGSKUResult[wCurrentUser].bIsGang)
		//{//���ڸ����ڣ������������������
		//	m_iSaveGameOutCardCount = m_cbOutCardCount;
		//	m_iSaveGameSendCardCount = m_cbSendCardCount;
		//}

		//�����ж�
		if (/*!m_bHearStatus[wCurrentUser] && */m_cbLeftCardCount>1)
		{
			//m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
			//	m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);

			m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);
			//if (GangCardResult.cbCardCount>=1)
			//{
			//	m_GameLogic.EstimateTingCard(m_cbCardIndex[wCurrentUser], m_WeaveItemArray[wCurrentUser], m_cbWeaveItemCount[wCurrentUser], byGangCard, byGangCardNums, m_cbKingCard);
			//}

			//if (byGangCardNums>0)
			//{
			//	m_cbUserAction[wCurrentUser] |= WIK_TING;
			//}
			if (GangCardResult.cbCardCount>=1)
			{
				m_cbUserAction[wCurrentUser] |= WIK_FILL|WIK_GANG;
			}
		}

		//����Ȩλ
		WORD wChiHuRight=0;
		//if (m_bGangStatus) 
		//{
		//	wChiHuRight|=CHR_QIANG_GANG;
		//}

		//�����ж�
		//BYTE cbReCardIndex[MAX_INDEX];;
		tagChiHuResult ChiHuResult;
		m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],
			0,wChiHuRight,ChiHuResult);
		//m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],
		//	                         0,wChiHuRight,ChiHuResult,m_cbKingCard,4,cbReCardIndex);
		//if (m_bGangStatus && (WIK_CHI_HU&m_cbUserAction[wCurrentUser]))
		//{
		//	m_tagUserGSKUResult[wCurrentUser].bIsGang = true;
		//}

		// �Ѿ������һ���ƣ���ץ�Ƶ������Ҳ�������Ϊ�����Ƹı�ս�֣���Ϸֻ�����ֿ���
		if (0>=m_cbLeftCardCount && !(WIK_CHI_HU&m_cbUserAction[wCurrentUser]))
		{
			m_byGameEndType = 0; //����
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			return true;
		}
	}

	//���ñ���
	m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;

	//��������
	CMD_S_SendCard SendCard;
	memset(&SendCard, 0, sizeof(CMD_S_SendCard));
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
	SendCard.cbCardData=(m_bSendStatus==true)?m_cbSendCardData:0x00;
	SendCard.bSupplyCard = bSupplyCard;

	if (byGangCardNums>0 && byGangCardNums<=4)
	{
		memcpy(SendCard.byGangCard, byGangCard, sizeof(BYTE)*byGangCardNums);
		SendCard.byGangCardNums = byGangCardNums;

		memcpy(m_byGangCard[wCurrentUser], byGangCard, sizeof(BYTE)*byGangCardNums);
		m_byGangCardNums[wCurrentUser] = byGangCardNums;
	}
	if (GangCardResult.cbCardCount>=1)
	{
		memcpy(SendCard.byBuCard, GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
		SendCard.byBuCardNums = GangCardResult.cbCardCount;

		memcpy(m_byBuCard[wCurrentUser], GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
		m_byBuCardNums[wCurrentUser] = GangCardResult.cbCardCount;
	}

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	 //��һ���ƣ��ж���������ǲ��ǿ��Բ����������������ȣ�
	//��������
	bool bAroseAction=false;

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//�����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ʼ��
		::memset(&m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
		m_byGangCardNums[i] = 0;
		::memset(&m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
		m_byBuCardNums[i] = 0;

		//�û�����
		if (wCenterUser==i) continue;

		//��������
		if (/*!m_bHearStatus[i] && */EstimatKind==EstimatKind_OutCard )
		{
			//�����ж�
			//if (m_bEnjoinChiPeng[i]==false)
			{
				//�����ж�
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				//�����ж�
				//WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				//if (wEatUser==i) m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);
			}

			//�����ж�
			if (m_cbLeftCardCount>1) 
			{		
				BYTE byCardType = WIK_NULL;
				byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				if ((byCardType&(WIK_GANG|WIK_FILL))>0)
				{
					m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
					m_byBuCard[i][0] = cbCenterCard;
					m_byBuCardNums[i] = 1;
					//bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], NULL, 0, cbCenterCard);
					//if (bIsTing)
					//{
					//	m_byGangCard[i][0] = cbCenterCard;
					//	m_byGangCardNums[i] = 1;
					//	m_cbUserAction[i] |= WIK_TING;
					//}
				}

				//m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
			}
		}

		//�����ж�
		//if (m_bEnjoinChiHu[i]==false)
		{
			//����Ȩλ
			WORD wChiHuRight=0;
			//if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
			//if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
			//if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

			//�Ժ��ж�
			tagChiHuResult ChiHuResult;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			m_cbUserAction[i] |= m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult);

			//�Ժ�����
			//if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) m_bEnjoinChiHu[i]=true;
		}

		//����ж�
		if (m_cbUserAction[i]!=WIK_NULL) bAroseAction=true;
	}

	//�������
	if (bAroseAction==true) 
	{
		//���ñ���
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//������ʾ
		SendOperateNotify();

		return true;
	}

	return false;
}

//��Ӧ�ж�
//bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
//{
//	//��������
//	bool bAroseAction=false;
//	
//
//	//�û�״̬
//	ZeroMemory(m_bResponse,sizeof(m_bResponse));
//	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
//	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
//
//
//
//	//�����ж�
//	for (WORD i=0;i<m_wPlayerCount;i++)
//	{
//
//		//��ʼ��
//		::memset(&m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
//		m_byGangCardNums[i] = 0;
//		::memset(&m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
//		m_byBuCardNums[i] = 0;
//
//		//�û�����
//		if (wCenterUser==i) continue;
//
//		//������͵Ļ�ɫ����Ŀ
//		BYTE cbWeaveCount=m_cbWeaveItemCount[i];
//		//for (WORD j=0;j<cbWeaveCount;j++)
//		//{
//		//	BYTE cbCardColor[i][j]=m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR;
//		//}
//
//		//tagGangCardResult GangCardResult;
//		//BYTE bySaveUserShouPai[MAX_INDEX] = {0};
//		//BYTE byGangCard[MAX_INDEX]={0};
//		//BYTE byGangCardNums = 0;
//		////�����ж�
//		//memset(&GangCardResult, 0, sizeof(tagGangCardResult));
//		//memcpy(bySaveUserShouPai, m_cbCardIndex[i], sizeof(BYTE)*MAX_INDEX);
//		//BYTE byCenterCardIndex = m_GameLogic.SwitchToCardIndex(cbCenterCard);
//		//if (byCenterCardIndex<MAX_INDEX)
//		//{
//		//	++bySaveUserShouPai[byCenterCardIndex];
//		//}
//		//m_GameLogic.AnalyseGangCard(bySaveUserShouPai,NULL,0,GangCardResult);
//		//if (GangCardResult.cbCardCount>=1)
//		//{
//		//	m_GameLogic.EstimateTingCard(bySaveUserShouPai, byGangCard, byGangCardNums, m_cbKingCard);
//		//}
//        		
//		//��������
//		if (!m_bHearStatus[i] && EstimatKind==EstimatKind_OutCard )
//		{
//			if (cbWeaveCount == 0)
//			{//û���¼�
//				//�����ж�
//				if (m_bEnjoinChiPeng[i]==false)
//				{//�ܳ�����
//					//�����ж�
//					m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//					//�����ж�
//					WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//					if (wEatUser==i)
//					{//�����¼��ܳ�
//						m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//					}
//				}
//				//�����ж�
//				if (m_cbLeftCardCount>1) 
//				{
//					BYTE byCardType = WIK_NULL;
//					byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//					if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//					{
//						m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//						m_byBuCard[i][0] = cbCenterCard;
//						m_byBuCardNums[i] = 1;
//						bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], NULL, 0, cbCenterCard, m_cbKingCard);
//						if (bIsTing)
//						{
//							m_byGangCard[i][0] = cbCenterCard;
//							m_byGangCardNums[i] = 1;
//							m_cbUserAction[i] |= WIK_TING;
//						}
//					}
//				}
//			}
//			else if (cbWeaveCount == 1)
//			{//����¼��˲�����һ����.
//				BYTE cbCardColor=m_WeaveItemArray[i][0].cbCenterCard&MASK_COLOR;
//				if (((m_WeaveItemArray[i][0].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_RIGHT) ))
//				{//�������
//					//�����ж�
//					if (m_bEnjoinChiPeng[i]==false && (cbCenterCard&MASK_COLOR) == cbCardColor)
//					{
//						//�����ж�
//						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						//�����ж�
//						WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//						if (wEatUser==i)
//						{
//							m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//					}
//					//�����ж�
//					if (m_cbLeftCardCount>1 && (cbCenterCard&MASK_COLOR) == cbCardColor) 
//					{
//						BYTE byCardType = WIK_NULL;
//						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//						{
//							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//							m_byBuCard[i][0] = cbCenterCard;
//							m_byBuCardNums[i] = 1;
//							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//							if (bIsTing)
//							{
//								m_byGangCard[i][0] = cbCenterCard;
//								m_byGangCardNums[i] = 1;
//								m_cbUserAction[i] |= WIK_TING;
//							}
//						}
//					}
//				}
//				else if (m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//				{//�������
//					//�����ж�
//					if (m_bEnjoinChiPeng[i]==false)
//					{
//						//�����ж�
//						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						if ((cbCenterCard&MASK_COLOR) == cbCardColor)
//						{
//							//�����ж�
//							WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//							if (wEatUser==i)
//							{
//								m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							}
//						}
//
//					}
//					//�����ж�
//					if (m_cbLeftCardCount>1) 
//					{
//						BYTE byCardType = WIK_NULL;
//						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//						{
//							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//							m_byBuCard[i][0] = cbCenterCard;
//							m_byBuCardNums[i] = 1;
//							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//							if (bIsTing)
//							{
//								m_byGangCard[i][0] = cbCenterCard;
//								m_byGangCardNums[i] = 1;
//								m_cbUserAction[i] |= WIK_TING;
//							}
//						}
//					}
//				}
//
//			}
//			else if (cbWeaveCount == 2)
//			{//�¼�����
//				BYTE cbCardColor=m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR;
//                if(((m_WeaveItemArray[i][1].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][1].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][1].cbWeaveKind == WIK_RIGHT) ))
//                {//�Ե������
//					//�����ж�
//					if (m_bEnjoinChiPeng[i]==false && ((cbCenterCard&MASK_COLOR) == cbCardColor))
//					{
//						//�����ж�
//						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						//�����ж�
//						WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//						if (wEatUser==i)
//						{
//							m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//					}
//
//					//�����ж�
//					if (m_cbLeftCardCount>1 && ((cbCenterCard&MASK_COLOR) == cbCardColor)) 
//					{
//						BYTE byCardType = WIK_NULL;
//						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//						{
//							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//							m_byBuCard[i][0] = cbCenterCard;
//							m_byBuCardNums[i] = 1;
//							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//							if (bIsTing)
//							{
//								m_byGangCard[i][0] = cbCenterCard;
//								m_byGangCardNums[i] = 1;
//								m_cbUserAction[i] |= WIK_TING;
//							}
//						}
//					}
//                }
//				else if (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//				{
//					//�����ж�
//					if (m_bEnjoinChiPeng[i]==false)
//					{
//						if (m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							//�����ж�
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//						else if(((cbCenterCard&MASK_COLOR) == cbCardColor) && ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_RIGHT) )) 
//						{
//							//�����ж�
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//
//						if ((m_WeaveItemArray[i][0].cbCenterCard&MASK_COLOR) == (m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR))
//						{
//							//�����ж�
//							if ((cbCenterCard&MASK_COLOR) == cbCardColor)
//							{
//								WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//								if (wEatUser==i)
//								{
//									m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//								}
//							}
//						}
//					}
//					//�����ж�
//					if (m_cbLeftCardCount>1) 
//					{
//						if (m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							BYTE byCardType = WIK_NULL;
//							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//							{
//								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//								m_byBuCard[i][0] = cbCenterCard;
//								m_byBuCardNums[i] = 1;
//								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//								if (bIsTing)
//								{
//									m_byGangCard[i][0] = cbCenterCard;
//									m_byGangCardNums[i] = 1;
//									m_cbUserAction[i] |= WIK_TING;
//								}
//							}
//						}
//						else if(((cbCenterCard&MASK_COLOR) == cbCardColor) && ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_RIGHT) )) 
//						{
//							BYTE byCardType = WIK_NULL;
//							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//							{
//								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//								m_byBuCard[i][0] = cbCenterCard;
//								m_byBuCardNums[i] = 1;
//								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//								if (bIsTing)
//								{
//									m_byGangCard[i][0] = cbCenterCard;
//									m_byGangCardNums[i] = 1;
//									m_cbUserAction[i] |= WIK_TING;
//								}
//							}
//						}
//					}
//				}
//			}
//
//			else if (cbWeaveCount == 3)
//			{//����
//				BYTE cbCardColor=m_WeaveItemArray[i][2].cbCenterCard&MASK_COLOR;
//				if(((m_WeaveItemArray[i][2].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][2].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][2].cbWeaveKind == WIK_RIGHT) ))
//				{
//					//�����ж�
//					if (m_bEnjoinChiPeng[i]==false && ((cbCenterCard&MASK_COLOR) == cbCardColor))
//					{
//						//�����ж�
//						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						//�����ж�
//						WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//						if (wEatUser==i)
//						{
//							m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//					}
//
//					//�����ж�
//					if (m_cbLeftCardCount>1 && ((cbCenterCard&MASK_COLOR) == cbCardColor)) 
//					{
//						BYTE byCardType = WIK_NULL;
//						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//						{
//							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//							m_byBuCard[i][0] = cbCenterCard;
//							m_byBuCardNums[i] = 1;
//							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//							if (bIsTing)
//							{
//								m_byGangCard[i][0] = cbCenterCard;
//								m_byGangCardNums[i] = 1;
//								m_cbUserAction[i] |= WIK_TING;
//							}
//						}
//					}
//				}
//				else if (m_WeaveItemArray[i][2].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][2].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//				{
//					//�����ж�
//					if (m_bEnjoinChiPeng[i]==false)
//					{
//						bool bIsTongSe = false;
//						if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
//						{
//							//�����ж�
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							bIsTongSe = true;
//						}
//						else if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							//�����ж�
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							bIsTongSe = true;
//						}
//						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
//						{
//							//�����ж�
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							bIsTongSe = true;
//						}
//						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							//�����ж�
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							bIsTongSe = true;
//						}
//						else if((cbCenterCard&MASK_COLOR) == cbCardColor)
//						{
//							//�����ж�
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//
//						if (bIsTongSe)
//						{
//							//�����ж�
//							if ((((m_WeaveItemArray[i][0].cbCenterCard&MASK_COLOR)==(m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR) ) && 
//								((m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR)==(m_WeaveItemArray[i][2].cbCenterCard&MASK_COLOR) )) && 
//								(cbCenterCard&MASK_COLOR) == cbCardColor)
//							{//ͬ��ɫ
//								WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//								if (wEatUser==i)
//								{
//									m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//								}
//							}
//						}
//					}
//					//�����ж�
//					if (m_cbLeftCardCount>1) 
//					{
//						bool bIsTongSe = false;
//						if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
//						{
//							bIsTongSe = true;
//						}
//						else if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							bIsTongSe = true;
//						}
//						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
//						{
//							bIsTongSe = true;
//						}
//						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							bIsTongSe = true;
//						}
//
//						if (bIsTongSe)
//						{
//							BYTE byCardType = WIK_NULL;
//							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//							{
//								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//								m_byBuCard[i][0] = cbCenterCard;
//								m_byBuCardNums[i] = 1;
//								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//								if (bIsTing)
//								{
//									m_byGangCard[i][0] = cbCenterCard;
//									m_byGangCardNums[i] = 1;
//									m_cbUserAction[i] |= WIK_TING;
//								}
//							}
//						}
//						else if((cbCenterCard&MASK_COLOR) == cbCardColor)
//						{
//							BYTE byCardType = WIK_NULL;
//							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//							{
//								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//								m_byBuCard[i][0] = cbCenterCard;
//								m_byBuCardNums[i] = 1;
//								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//								if (bIsTing)
//								{
//									m_byGangCard[i][0] = cbCenterCard;
//									m_byGangCardNums[i] = 1;
//									m_cbUserAction[i] |= WIK_TING;
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	
//        //�����ж�
//		BYTE bKingCount=0;
//		bKingCount=m_GameLogic.GetKingCardCount(m_cbCardIndex[i],NULL,0,m_cbKingCard,4);
//		if (0==bKingCount)
//		{//�����ˣ�����û�����Ʋ����жϺ���.
//			//����Ȩλ
//			WORD wChiHuRight=0;
//			//�Ժ��ж�
//			BYTE cbReCardIndex[MAX_INDEX]={0};
//			tagChiHuResult ChiHuResult;
//			//BYTE cbWeaveCount=m_cbWeaveItemCount[i];
//			//m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult,m_cbKingCard,4,cbReCardIndex);
//			//������ֻ��������ֻ�����������Ʋ��ܽ���
//			//ͳ��������Ŀ
//			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult);
//			//m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult,m_cbKingCard,4,cbReCardIndex, true);
//		}
//
//		//����ж�
//		if (m_cbUserAction[i]!=WIK_NULL) bAroseAction=true;
//	}
//
//	////����¼����еĸ���
//	//WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//	//for (int k=0; k<m_wPlayerCount; k++)
//	//{
//	//	if (wEatUser==k && m_cbUserAction[k]!=WIK_NULL)
//	//	{
//	//		//��������
//	//		tagGangCardResult GangCardResult;
//	//		BYTE byGangCard[MAX_INDEX]={0};
//	//		BYTE byGangCardNums = 0;
//	//		//�����ж�
//	//		memset(&GangCardResult, 0, sizeof(tagGangCardResult));
//	//		m_GameLogic.AnalyseGangCard(m_cbCardIndex[k],m_WeaveItemArray[k], m_cbWeaveItemCount[k],GangCardResult, m_cbKingCard);
//	//		if (GangCardResult.cbCardCount>=1)
//	//		{
//	//			m_GameLogic.EstimateTingCard(m_cbCardIndex[k], m_WeaveItemArray[k], m_cbWeaveItemCount[k], byGangCard, byGangCardNums, m_cbKingCard);
//	//		}
//	//		if (byGangCardNums>0)
//	//		{//��Ӹ�
//	//			m_cbUserAction[k] |= WIK_TING;
//	//			memcpy(&m_byGangCard[k][m_byGangCardNums[k]], byGangCard, sizeof(BYTE)*byGangCardNums);
//	//			m_byGangCardNums[k] += byGangCardNums;
//	//		}
//	//		if (GangCardResult.cbCardCount>=1)
//	//		{//��Ӳ�
//	//			m_cbUserAction[k] |= WIK_FILL|WIK_GANG;
//	//			memcpy(&m_byBuCard[k][m_byBuCardNums[k]], GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
//	//			m_byBuCardNums[k] += GangCardResult.cbCardCount;
//	//		}
//
//	//		//����ж�
//	//		if (m_cbUserAction[k]!=WIK_NULL) bAroseAction=true;
//	//		break;
//	//	}
//	//}
//
//	//�������
//	if (bAroseAction==true) 
//	{
//		//���ñ���
//		m_wProvideUser=wCenterUser;
//		m_cbProvideCard=cbCenterCard;
//		m_wResumeUser=m_wCurrentUser;
//		m_wCurrentUser=INVALID_CHAIR;
//
//		//������ʾ
//		SendOperateNotify();
//
//		return true;
//	}
//
//	return false;
//}


//cp add 4.11
//==============================================================================
//����ID
#define					Sparrow_TASK_ID_1			31001
#define					Sparrow_TASK_ID_2			31002
#define					Sparrow_TASK_ID_3			31003
#define					Sparrow_TASK_ID_4			31004
#define					Sparrow_TASK_ID_5			31005
#define					Sparrow_TASK_ID_6			31006
#define					Sparrow_TASK_ID_7			31007
#define					Sparrow_TASK_ID_8			31008
#define					Sparrow_TASK_ID_9			31009
#define					Sparrow_TASK_ID_10			31010
#define					Sparrow_TASK_ID_11			31011

//==============================================================================




//
////��������ж�
//void CTableFrameSink::OnUserTaskJudgement(WORD wChairID, BYTE btTag)
//{
//	//wChairID:���ID;  btTag:1:Ӯ,0:��.
//	if (wChairID>=GAME_PLAYER || btTag>1)
//	{
//		return;
//	}
//	//���Ȼ�ȡwChairID����������.
//	IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem( wChairID );
//	if (NULL == pUserItem)
//	{
//		return;
//	}
//	tagServerUserData *pUserData = pUserItem->GetUserData();
//	if (NULL == pUserData)
//	{
//		return;
//	}
//	//pUserData->task.dwID:����ID
//	if (1==btTag)
//	{//wChairIDӮ��
//		bool bIsWin = false;
//		switch(pUserData->task.dwID)
//		{
//		case Sparrow_TASK_ID_1:	//����1: С��
//			{
//				if ( (CHK_JI_HU == m_ChiHuResult[wChairID].wChiHuKind) || (CHK_PING_HU == m_ChiHuResult[wChairID].wChiHuKind) )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_2:	//����2: ��ϲ
//			{   
//				if (( CHK_SIXI_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsSiXi(m_cbCardIndex[wChairID]) )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_3:	//����3: ����
//			{   
//                if (( CHK_BANBAN_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsBanBanHu(m_cbCardIndex[wChairID]) )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_4:	//����4: ȱһɫ
//			{   
//				if (( CHK_QUEYISE_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsQueYiSe(m_cbCardIndex[wChairID]) )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_5:	//����5: ����˳
//			{   
//				if (( CHK_LIULIU_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsLiuLiuShun(m_cbCardIndex[wChairID]) )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_6:	//����6: ������
//			{   
//				if (( CHK_PENG_PENG & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( CHK_PENG_PENG == m_ChiHuResult[wChairID].wChiHuKind )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_7:	//����7: ������
//			{  		
//				if (( CHK_JIANG_JIANG & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_8:	//����8: ��С��
//			{   
//				if (( CHK_QI_XIAO_DUI & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( CHK_QI_XIAO_DUI == m_ChiHuResult[wChairID].wChiHuKind )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_9:	//����9: ���Ͽ���
//			{   
//				if ( true == m_tagUserGSKUResult[wChairID].bIsGang && true == m_tagUserGSKUResult[wChairID].bIsGSKU)
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_10:	//����10: �غ�
//			{   
//				if (( CHR_DI & m_ChiHuResult[wChairID].wChiHuRight ) > 0)
//				//if ( CHR_DI == m_ChiHuResult[wChairID].wChiHuRight )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_11:	//����11: ���
//			{   
//				if (( CHR_TIAN & m_ChiHuResult[wChairID].wChiHuRight ) > 0)
//				//if ( CHR_TIAN == m_ChiHuResult[wChairID].wChiHuRight)
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		default:
//			break;
//		}
//		if (true == bIsWin)
//		{//�������.
//			//�ύ�������
//			m_pITableFrame->PefermTaskFinish(wChairID);
//		}
//	}
//}
//////////////////////////////////////////////////////////////////////////
////////////////
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0;
}
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0;
}
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	return false;
}
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return false;
}
void CTableFrameSink::SetGameBaseScore(LONG lBaseScore)
{
	return;
}
bool CTableFrameSink::OnActionUserOnReady(WORD,IServerUserItem *,void *,WORD)
{
	return false;
}
//////////////////////////////////////////////////////////////////////////




