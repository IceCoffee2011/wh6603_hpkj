#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientEngine.h"
#include "JettonView.h"
#include ".\GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_NULLITY					200									//��Ч��ʱ��
#define IDI_START_GAME				201									//��ʼ��ʱ��
#define IDI_CALL_BANKER				202									//��ʼ��ʱ��

//��ʱ����ʶ
#define IDI_TIME_USER_ADD_SCORE		1									//��ע��ʱ��
#define IDI_TIME_OPEN_CARD			2									//̯�ƶ�ʱ��
#define IDI_TIME_NULLITY			3									//̯�ƶ�ʱ��

//ʱ���ʶ
#ifdef _DEBUG
#define TIME_USER_CALL_BANKER		10									//��ׯ��ʱ��
#define TIME_USER_START_GAME		10									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			10									//������ʱ��
#define TIME_USER_OPEN_CARD			10									//̯�ƶ�ʱ��
#else
#define TIME_USER_CALL_BANKER		30									//��ׯ��ʱ��
#define TIME_USER_START_GAME		30									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			30									//������ʱ��
#define TIME_USER_OPEN_CARD			30									//̯�ƶ�ʱ��

#endif

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)

	ON_MESSAGE(IDM_HINT_OX,OnHintOx)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_ADD_SCORE,OnAddScore)
	ON_MESSAGE(IDM_OX,OnOx)
	ON_MESSAGE(IDM_SHORTCUT_KEY,OnShortcut)
	ON_MESSAGE(IDM_READY,OnOpenCard)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
	ON_MESSAGE(IDM_SHOW_VALUE,OnSetUserOxValue)
	ON_MESSAGE(IDM_SHOW_INFO,OnShowInfo)
	ON_MESSAGE(IDM_OX_ENABLE,OnOxEnable)
	ON_MESSAGE(IDM_SORT,OnSortCard)
	ON_MESSAGE(IDM_RESORT,OnReSortCard)
	ON_MESSAGE(IDM_BANKER,OnBanker)
	ON_MESSAGE(IDM_YU_YIN,OnMessageYuYin)
	//ON_MESSAGE(IDM_CLICK_CARD,OnClickCard)
	ON_MESSAGE(IDM_ADMIN_COMMDN,OnAdminCommand)
	ON_MESSAGE(IDM_ADMIN_QUERYUSER,OnAdminQueryUser)
	ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//���ñ���
	m_dwCardHSpace=DEFAULT_PELS;

	//��ע��Ϣ
	m_lTurnMaxScore=SCORE_ZERO;
	m_bDynamicJoin=FALSE;
	//m_lTurnLessScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_bUserOxCard,sizeof(m_bUserOxCard));

	//״̬����
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	for (WORD i=0;i<GAME_PLAYER;i++)m_wViewChairID[i]=INVALID_CHAIR;

	//MessageBox(L"1111");
	//��ʷ����
	m_HistoryScore.ResetData();
	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool   CGameClientEngine::OnInitGameEngine()
{
	//ȫ�ֶ���
	CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
	ASSERT(m_pGlobalUnits!=NULL);


	//��������
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	if (m_pIClientKernel!=NULL) m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	////����ṹ
	//tagPhraseVoiceInfo pvi = {};
	//pvi.pszImageFrame = MAKEINTRESOURCE(IDB_PHRASE_FRAME);
	//pvi.pszImageBtUp = MAKEINTRESOURCE(IDB_PHRASE_BT_UP);
	//pvi.pszImageBtDown = MAKEINTRESOURCE(IDB_PHRASE_BT_DOWN);
	//pvi.pszPngFocus = TEXT("LINE");
	//pvi.cxBorderWidth = 5;
	//pvi.cyBorderHeight = 7;
	//pvi.cyBottomHeight = 30;
	//pvi.nPageCount = 10;
	//pvi.nTransPercent = 10;
	//pvi.crTextColor = RGB(245,187,128);

	////���������ؼ�
	//if( m_PhraseVoiceHelper.CreateInstance() )
	//{
	//	m_PhraseVoiceHelper->InitPhraseVoice(pvi,this);

	//	//���ûص�
	//	m_PhraseVoiceHelper->SetPhraseVoiceSink(this);
	//}

	//���Դ���
#ifdef _DEBUG
	//long lll=122877+76523;
	//m_GameClientView.m_btShortcut.ShowWindow(SW_SHOW);
	//BYTE bTemp[]={1,2,3,4,5};
	//for (BYTE i=0;i<GAME_PLAYER;i++)
	//{
	//	m_GameClientView.m_CardControl[i].SetDisplayFlag(true);
	//	m_GameClientView.m_CardControl[i].SetCardData(bTemp,5);
	//	//m_GameClientView.m_CardControlOx[i].SetDisplayFlag(true);
	//	//m_GameClientView.m_CardControlOx[i].SetCardData(bTemp,2);
	//}
	//m_GameClientView.m_CardControl[2].SetPositively(true);

#endif
	return true;
}

//���ÿ��
bool   CGameClientEngine::OnResetGameEngine()
{

	//ɾ��ʱ��
	KillTimer(IDI_TIME_USER_ADD_SCORE);
	KillTimer(IDI_TIME_OPEN_CARD);
	KillTimer(IDI_TIME_NULLITY);

	//��ע��Ϣ
	m_lTurnMaxScore=SCORE_ZERO;
	m_bDynamicJoin=FALSE;
	//m_lTurnLessScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_bUserOxCard,sizeof(m_bUserOxCard));

	//״̬����
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	for (WORD i=0;i<GAME_PLAYER;i++)m_wViewChairID[i]=INVALID_CHAIR;

	return true;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//ʱ����Ϣ
bool   CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_START_GAME:		//��ʼ��ʱ��
		{
			//��ֹ�ж�
			if ((nElapse==0)&&(wChairID==GetMeChairID())&&(!IsLookonMode()))
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return true;
			}
#ifndef DEBUG
			//����֪ͨ
			if ((nElapse<=5)&&(IsLookonMode()==false)&&(wChairID==GetMeChairID()))
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
#endif
			return true;
		}
	case IDI_NULLITY:			//����ʱ��
		{
			//��ֹ�ж�
			if ((nElapse==0)&&(wChairID==GetMeChairID())&&(!IsLookonMode()))
			{
				KillTimer(IDI_TIME_NULLITY);
				KillGameClock(IDI_NULLITY);
				return true;
			}
#ifndef DEBUG
			//����֪ͨ
			if ((nElapse<=5)&&(IsLookonMode()==false)&&(wChairID==GetMeChairID()))
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
#endif
			return true;
		}
	case IDI_CALL_BANKER:		//��ׯ��ʱ
		{
			//��ֹ�ж�
			if ((nElapse==0)&&(wChairID==GetMeChairID())&&(!IsLookonMode()))
			{
				//������ׯ
				OnBanker(0,0);
				return true;
			}
#ifndef DEBUG
			//����֪ͨ
			if ((nElapse<=5)&&(IsLookonMode()==false)&&(wChairID==GetMeChairID()))
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
#endif
			return true;			
		}
	}

	return false;
}

//�Թ�״̬
bool   CGameClientEngine::OnEventLookonMode(void * pBuffer, WORD wDataSize)
{
	//�ؼ�����
	//if(IsAllowLookon() )
	//	m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetDisplayFlag(true);
	//else m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetDisplayFlag(false);
     return true;
}

//������Ϣ
bool   CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_CALL_BANKER:	//�û���ׯ
		{
			//��Ϣ����
			return OnSubCallBanker(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			//��Ϣ����
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//�û���ע
		{
			//��Ϣ����
			return OnSubAddScore(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:	//������Ϣ
		{
			//��Ϣ����
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_OPEN_CARD:	//�û�̯��
		{
			//��Ϣ����
			return OnSubOpenCard(pBuffer,wDataSize);
		}
	case SUB_S_PLAYER_EXIT:	//�û�ǿ��
		{
			//��Ϣ����
			return OnSubPlayerExit(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return false;
}

//��Ϸ����
bool   CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	if(IsLookonMode())m_GameClientView.m_bLookOnUser=true;
	switch (cbGameStation)
	{
	case GS_TK_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//���ÿؼ�
			if (IsLookonMode()==false && GetMeUserItem()->GetUserStatus()!=US_READY)
			{
				SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_USER_START_GAME);
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
			}
			
			m_lCellScore=pStatusFree->lCellScore;
			m_GameClientView.m_JettonView.SetCellScore(m_lCellScore);
			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusFree->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusFree->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}
			//if(IsDistributeMode()==false) m_GameClientView.m_btScore.ShowWindow(SW_SHOW);

				//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
			{

				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}

			
			return true;
		}
	case GS_TK_CALL:	// ��ׯ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;
			CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pBuffer;

			m_lCellScore=pStatusCall->lCellScore;
			m_GameClientView.m_JettonView.SetCellScore(m_lCellScore);

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusCall->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusCall->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}
			//if(IsDistributeMode()==false) m_GameClientView.m_btScore.ShowWindow(SW_SHOW);

			//��Ϸ��Ϣ
			CopyMemory(m_cbPlayStatus,pStatusCall->cbPlayStatus,sizeof(m_cbPlayStatus));
			m_bDynamicJoin=pStatusCall->cbDynamicJoin;
			m_GameClientView.SetUserDynamicJoin(m_bDynamicJoin);

			//�û���Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ͼλ��
				m_wViewChairID[i]=SwitchViewChairID(i);

				//��ȡ�û�
				IClientUserItem * pUserData=GetTableUserItem(i);
				if (pUserData==NULL) continue;

				//�û�����
				lstrcpyn(m_szAccounts[i],pUserData->GetNickName(),CountArray(m_szAccounts[i]));
			}
			
			//ʼ���û�
			if(!IsLookonMode() && pStatusCall->wCallBanker==GetMeChairID())
			{
				//�ؼ���ʾ
				//ActiveGameFrame();
				m_GameClientView.m_btBanker.ShowWindow(SW_SHOW);
				m_GameClientView.m_btIdler.ShowWindow(SW_SHOW);
			}

			//�ȴ���־
			WORD wViewID=m_wViewChairID[pStatusCall->wCallBanker];
			m_GameClientView.SetWaitCall((BYTE)wViewID);

			//ʵ�ʶ�ʱ��
			if(pStatusCall->wCallBanker==GetMeChairID())
			{
				SetGameClock(pStatusCall->wCallBanker,IDI_CALL_BANKER,TIME_USER_CALL_BANKER);
			}
			else 
			{
				SetGameClock(pStatusCall->wCallBanker,IDI_NULLITY,TIME_USER_CALL_BANKER);
				SetTimer(IDI_TIME_NULLITY,1000,NULL);
			}

				//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
			{

				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}
			
			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GS_TK_SCORE:	//��ע״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pBuffer;

			m_lCellScore=pStatusScore->lCellScore;
			m_GameClientView.m_JettonView.SetCellScore(m_lCellScore);

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusScore->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusScore->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}
			//if(IsDistributeMode()==false) m_GameClientView.m_btScore.ShowWindow(SW_SHOW);

			//���ñ���
			m_lTurnMaxScore=pStatusScore->lTurnMaxScore;
			m_wBankerUser=pStatusScore->wBankerUser;			
			CopyMemory(m_lTableScore,pStatusScore->lTableScore,sizeof(m_lTableScore));

			//��Ϸ��Ϣ
			CopyMemory(m_cbPlayStatus,pStatusScore->cbPlayStatus,sizeof(m_cbPlayStatus));
			m_bDynamicJoin=pStatusScore->cbDynamicJoin;
			m_GameClientView.SetUserDynamicJoin(m_bDynamicJoin);

			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ͼλ��
				m_wViewChairID[i]=SwitchViewChairID(i);

				//�������
				if(m_lTableScore[i]>COMPARE_ZERO)m_GameClientView.SetUserTableScore(m_wViewChairID[i],m_lTableScore[i]);
				
				//��ȡ�û�
				IClientUserItem * pUserData=GetTableUserItem(i);
				if (pUserData==NULL) continue;
				
				//�û�����
				lstrcpyn(m_szAccounts[i],pUserData->GetNickName(),CountArray(m_szAccounts[i]));
			}

			//���ó���
			if (!IsLookonMode() && pStatusScore->lTurnMaxScore>COMPARE_ZERO && m_lTableScore[GetMeChairID()]==0.)
			{
				m_GameClientView.m_JettonView.SetMaxScore(m_lTurnMaxScore);
				m_GameClientView.m_JettonView.ShowWindows(TRUE);

				//ʵ�ʶ�ʱ��
				SetTimer(IDI_TIME_USER_ADD_SCORE,(TIME_USER_ADD_SCORE)*1000,NULL);
			}

			//ׯ�ұ�־
			WORD wID=m_wViewChairID[m_wBankerUser];
			m_GameClientView.SetBankerUser(wID);

			//�ȴ���־
			m_GameClientView.SetWaitInvest(true);

			//������ʾ����ʱ��
			SetGameClock(GetMeChairID(),IDI_NULLITY,TIME_USER_ADD_SCORE);
			SetTimer(IDI_TIME_NULLITY,1000,NULL);

			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);

				//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
			{

				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}
			return true;
		}
	case GS_TK_PLAYING:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			m_lCellScore=pStatusPlay->lCellScore;
			m_GameClientView.m_JettonView.SetCellScore(m_lCellScore);

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusPlay->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusPlay->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}
			//if(IsDistributeMode()==false) m_GameClientView.m_btScore.ShowWindow(SW_SHOW);

			//���ñ���
			m_lTurnMaxScore=pStatusPlay->lTurnMaxScore;
			m_wBankerUser=pStatusPlay->wBankerUser;
			CopyMemory(m_lTableScore,pStatusPlay->lTableScore,sizeof(m_lTableScore));
			CopyMemory(m_bUserOxCard,pStatusPlay->bOxCard,sizeof(m_bUserOxCard));
			//��Ϸ��Ϣ
			CopyMemory(m_cbPlayStatus,pStatusPlay->cbPlayStatus,sizeof(m_cbPlayStatus));
			m_bDynamicJoin=pStatusPlay->cbDynamicJoin;
			m_GameClientView.SetUserDynamicJoin(m_bDynamicJoin);

			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ͼλ��
				m_wViewChairID[i]=SwitchViewChairID(i);

				//�������
				if(m_lTableScore[i]>COMPARE_ZERO)m_GameClientView.SetUserTableScore(m_wViewChairID[i],m_lTableScore[i]);
				
				//��ȡ�û�
				IClientUserItem * pUserData=GetTableUserItem(i);
				if (pUserData==NULL) continue;
				
				//�˿�����
				CopyMemory(m_cbHandCardData[i],pStatusPlay->cbHandCardData[i],MAX_COUNT);

				//�û�����
				lstrcpyn(m_szAccounts[i],pUserData->GetNickName(),CountArray(m_szAccounts[i]));
			}

			//ׯ�ұ�־
			WORD wID=m_wViewChairID[m_wBankerUser];
			m_GameClientView.SetBankerUser(wID);

			//������Ϣ
			//m_GameClientView.SetScoreInfo(m_lTurnMaxScore,0);

			//���ý���
			SCORE lTableScore=SCORE_ZERO;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//����λ��
				WORD wViewChairID=m_wViewChairID[i];

				//�����˿�
				if (m_cbPlayStatus[i]==TRUE) 
				{
					m_GameClientView.m_CardControl[wViewChairID].SetCardData(pStatusPlay->cbHandCardData[i],MAX_COUNT);
				}
			}

			WORD wMeChiarID=GetMeChairID();
			WORD wViewChairID=m_wViewChairID[wMeChiarID];
			if(!IsLookonMode())m_GameClientView.m_CardControl[wViewChairID].SetPositively(true);
			if(IsAllowLookon() || !IsLookonMode())
			{				
				m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(true);
			}

			//̯�Ʊ�־
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_bUserOxCard[i]!=0xff)
				{
					m_GameClientView.ShowOpenCard(m_wViewChairID[i]);
				}
			}
				//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
			{

				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}


			//����ʱ��
			SetGameClock(GetMeChairID(),IDI_NULLITY,TIME_USER_OPEN_CARD);
			SetTimer(IDI_TIME_NULLITY,1000,NULL);

			//�ȴ���־
			m_GameClientView.m_bOpenCard=true;

			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);

			//�ؼ�����
			if(!IsLookonMode())
			{
				//��ʾ�ؼ�
				if(m_bUserOxCard[wMeChiarID]==0xff)
				{
					OnSendCardFinish(0,0);
				}
				else 
				{
					m_GameClientView.m_CardControl[wViewChairID].SetPositively(false);
					if(m_bUserOxCard[wMeChiarID]==TRUE)//ţ�Ʒ���
					{
						//���ñ���
						BYTE bTemp[MAX_COUNT];
						CopyMemory(bTemp,m_cbHandCardData[wMeChiarID],sizeof(bTemp));

						//��ȡţ��
						m_GameLogic.GetOxCard(bTemp,MAX_COUNT);

						BYTE bCardValue=m_GameLogic.GetCardType(bTemp,MAX_COUNT);
						ASSERT(bCardValue>0);

						if(bCardValue<OX_THREE_SAME)
						{
							//���ÿؼ�
							m_GameClientView.m_CardControl[wViewChairID].SetCardData(bTemp,3);
							m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(&bTemp[3],2);	
						}

						//��ʾ����
						m_GameClientView.SetUserOxValue(wViewChairID,bCardValue);
					}
					else 
					{
						//��ţ
						m_GameClientView.SetUserOxValue(wViewChairID,0);
					}					
				}
			}
			return true;
		}
	}

	return false;
}

//�û�����
VOID   CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//���óɼ�
	if (bLookonUser==false)
	{
		//���ñ���
		m_HistoryScore.OnEventUserEnter(pIClientUserItem->GetChairID());

		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return;
}

//�û��뿪
VOID   CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//���óɼ�
	if (bLookonUser==false)
	{
		//���ñ���
		m_HistoryScore.OnEventUserLeave(pIClientUserItem->GetChairID());

		//���½���
		m_GameClientView.InvalidGameView(0,0,0,0);
	}

	return;
}

//�û�״̬
VOID   CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}

//�û���ׯ
bool CGameClientEngine::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{
	//����״̬
	SetGameStatus(GS_TK_CALL);
	//Ч������
	if (wDataSize!=sizeof(CMD_S_CallBanker)) return false;
	CMD_S_CallBanker * pCallBanker=(CMD_S_CallBanker *)pBuffer;

	//�״ν�ׯ
	if(pCallBanker->bFirstTimes)
	{
		//�û���Ϣ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��ͼλ��
			m_wViewChairID[i]=SwitchViewChairID(i);

			//��ȡ�û�
			IClientUserItem * pUserData=GetTableUserItem(i);
			if (pUserData==NULL) continue;

			//��Ϸ��Ϣ
			if(!m_bDynamicJoin) m_cbPlayStatus[i]=TRUE;

			//�û�����
			lstrcpyn(m_szAccounts[i],pUserData->GetNickName(),CountArray(m_szAccounts[i]));
		}

		//�Թ�����������
		if (IsLookonMode()) OnStart(0,0);
	}

	//ɾ����ʱ��/��ť
	if(m_GameClientView.m_btIdler.IsWindowVisible()==TRUE)
	{
		KillGameClock(IDI_CALL_BANKER);
		m_GameClientView.m_btIdler.ShowWindow(SW_HIDE);
		m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
	}

	//�û��ؼ���ʾ
	if(IsCurrentUser(pCallBanker->wCallBanker) && m_cbPlayStatus[pCallBanker->wCallBanker])
	{
		m_GameClientView.m_btBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btIdler.ShowWindow(SW_SHOW);
	}

	//�ȴ���־
	WORD wViewID=m_wViewChairID[pCallBanker->wCallBanker];
	m_GameClientView.SetWaitCall((BYTE)wViewID);

	//ʵ�ʶ�ʱ��
	if(pCallBanker->wCallBanker==GetMeChairID())
	{
		SetGameClock(pCallBanker->wCallBanker,IDI_CALL_BANKER,TIME_USER_CALL_BANKER);
	}
	else 
	{
		SetGameClock(pCallBanker->wCallBanker,IDI_NULLITY,TIME_USER_CALL_BANKER);
		SetTimer(IDI_TIME_NULLITY,1000,NULL);
	}

	return true;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//����״̬
	SetGameStatus(GS_TK_PLAYING);
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//ɾ����ʱ��/��ť
	if(m_GameClientView.m_btIdler.IsWindowVisible()==TRUE)
	{
		KillGameClock(IDI_CALL_BANKER);
		m_GameClientView.m_btIdler.ShowWindow(SW_HIDE);
		m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
	}

	//���ñ���
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
	m_wBankerUser=pGameStart->wBankerUser;
	m_GameClientView.SetWaitCall(0xff);


	//���ó���
	if (!IsLookonMode() && pGameStart->lTurnMaxScore>COMPARE_ZERO)
	{
		m_GameClientView.m_JettonView.SetMaxScore(m_lTurnMaxScore);
		m_GameClientView.m_JettonView.ShowWindows(TRUE);
		//ʵ�ʶ�ʱ��
		SetTimer(IDI_TIME_USER_ADD_SCORE,(TIME_USER_ADD_SCORE)*1000,NULL);
	}

	//ׯ�ұ�־
	WORD wID=m_wViewChairID[m_wBankerUser];
	m_GameClientView.SetBankerUser(wID);

	//�ȴ���־
	m_GameClientView.SetWaitInvest(true);

	//������ʾ����ʱ��
	SetGameClock(GetMeChairID(),IDI_NULLITY,TIME_USER_ADD_SCORE);
	SetTimer(IDI_TIME_NULLITY,1000,NULL);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�û���ע
bool CGameClientEngine::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//ɾ����ʱ��/���ư�ť
	if(IsCurrentUser(pAddScore->wAddScoreUser) && m_GameClientView.m_JettonView.IsWindowVisible()==TRUE)
	{
		m_GameClientView.m_JettonView.ShowWindows(FALSE);
		KillTimer(IDI_TIME_USER_ADD_SCORE);
		//UpdateScoreControl(NULL,SW_HIDE);
	}

	//��������
	WORD wAddScoreUser=pAddScore->wAddScoreUser;
	WORD wViewChairID=m_wViewChairID[wAddScoreUser];

	//��ע����
	m_lTableScore[pAddScore->wAddScoreUser]=pAddScore->lAddScoreCount;
	m_GameClientView.SetUserTableScore(wViewChairID,pAddScore->lAddScoreCount);
	if (!IsCurrentUser(pAddScore->wAddScoreUser))m_GameClientView.SetUserTableScore(wViewChairID,pAddScore->lAddScoreCount,true);

	//��������
	if (!IsCurrentUser(pAddScore->wAddScoreUser) && m_cbPlayStatus[wAddScoreUser]==TRUE)
		PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	return true;
}

//������Ϣ
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//ɾ����ʱ��
	KillGameClock(IDI_NULLITY);
	KillTimer(IDI_TIME_NULLITY);

	m_GameClientView.SetWaitInvest(false);

	CopyMemory(m_cbHandCardData,pSendCard->cbCardData,sizeof(m_cbHandCardData));

	WORD wMeChiarID=GetMeChairID();
	WORD wViewChairID=m_wViewChairID[wMeChiarID];
	if((IsAllowLookon() || !IsLookonMode()) && !m_bDynamicJoin)
	{
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(true);
	}

	//�ɷ��˿�
	for(WORD i=0;i<MAX_COUNT;i++)
	{
		for (WORD j=m_wBankerUser;j<m_wBankerUser+GAME_PLAYER;j++)
		{
			WORD w=j%GAME_PLAYER;
			if (m_cbPlayStatus[w]==TRUE)
			{
				WORD wViewChairID=m_wViewChairID[w];
				if(pSendCard->cbCardData[w][i]!=0)
				{
					m_GameClientView.DispatchUserCard(wViewChairID,m_cbHandCardData[w][i]);
				}
			}
		}
	}

	return true;
}

//�û�̯��
bool CGameClientEngine::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
	CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;

	//���ñ���
	WORD wMeChairID=GetMeChairID();
	WORD wID=pOpenCard->wPlayerID;
	WORD wViewChairID=m_wViewChairID[wID];
	m_bUserOxCard[wID]=pOpenCard->bOpen;
	m_GameClientView.ShowOpenCard(wViewChairID);

	//̯�Ʊ�־
	if(!IsCurrentUser(wID))
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OPEN_CARD"));
	}
	else if(m_GameClientView.m_btOpenCard.IsWindowVisible()==TRUE)
	{
		//ɾ��ʱ��/�ؼ�
		KillTimer(IDI_TIME_OPEN_CARD);
		m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
		m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);
//		m_GameClientView.m_btReSort.ShowWindow(SW_HIDE);
//		m_GameClientView.m_btShortcut.ShowWindow(SW_HIDE);

		//��ʾ������ţţ
		if(m_bUserOxCard[wID]==FALSE)
		{
			m_GameClientView.m_CardControl[wViewChairID].ShootAllCard(false);
			m_GameClientView.SetUserOxValue(wViewChairID,0);
		}
		else
		{
			//��ʾ�ٷ���ţ��
			OnHintOx(0,0);
			m_GameClientView.m_CardControl[wViewChairID].SetOX(true);
			OnSortCard(0,0);
		}
		m_GameClientView.m_CardControl[wViewChairID].SetPositively(false);
	}

	return true;
}

//�û�ǿ��
bool CGameClientEngine::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	WORD wID=pPlayerExit->wPlayerID;

	//��Ϸ��Ϣ
	ASSERT(m_cbPlayStatus[wID]==TRUE);
	m_cbPlayStatus[wID]=FALSE;

	//�û�����
	for(WORD i=0;i<32;i++)m_szAccounts[wID][i]=0;
	m_szAccounts[wID][0] = '#';

	BYTE bCount=0;
	for(WORD i=0;i<GAME_PLAYER;i++)if(m_cbPlayStatus[i]!=FALSE)bCount++;
	if(bCount==1)m_GameClientView.SetBankerUser(INVALID_CHAIR);

	return true;

}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//ɾ����ʱ��
	KillGameClock(IDI_NULLITY);
	KillGameClock(IDI_CALL_BANKER);
	KillTimer(IDI_TIME_USER_ADD_SCORE);
	KillTimer(IDI_TIME_OPEN_CARD);
	KillTimer(IDI_TIME_NULLITY);

	//��������
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.ShowOpenCard(i,FALSE);
	}	
	m_GameClientView.SetWaitInvest(false);
	m_GameClientView.m_JettonView.ShowWindows(FALSE);
	m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btIdler.ShowWindow(SW_HIDE);
	m_GameClientView.SetWaitCall(0xff);
	m_GameClientView.m_bOpenCard=false;
	m_GameClientView.SetUserDynamicJoin(FALSE);
	m_bDynamicJoin=FALSE;

	//״̬����
	SetGameStatus(GS_TK_FREE);

	//��������
	if (IsLookonMode()==false)
	{
		if (pGameEnd->lGameScore[GetMeChairID()]>COMPARE_ZERO) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//��ʾ����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(GetTableUserItem(i)!=NULL)
		{
			if(!m_cbPlayStatus[i]) continue;
			m_GameClientView.ShowScore(m_wViewChairID[i],true);
			m_GameClientView.SetUserTableScore(m_wViewChairID[i],pGameEnd->lGameScore[i]);
		}
	}
//	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ϣ
		m_HistoryScore.OnEventUserScore(i,pGameEnd->lGameScore[i]);

		//��ȡ����
		tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

		//������
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
	}

	//��ʾ����
	bool bOxSound=false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=m_wViewChairID[i];
		if(i==GetMeChairID() && !IsLookonMode())continue;
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayFlag(true);

		//ţţ����
		if(m_cbHandCardData[i][0]>0)
		{
			if(m_bUserOxCard[i]==TRUE)
			{
				//�˿�����
				BYTE bCardData[MAX_COUNT];
				CopyMemory(bCardData,m_cbHandCardData[i],sizeof(bCardData));

				//��ȡţţ����
				m_GameLogic.GetOxCard(bCardData,MAX_COUNT);

				BYTE bCardValue=m_GameLogic.GetCardType(bCardData,MAX_COUNT);
				ASSERT(bCardValue>0);

				//��������
				if(bCardValue<OX_THREE_SAME)
				{
					m_GameClientView.m_CardControl[wViewChairID].SetCardData(bCardData,3);
					m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(&bCardData[3],2);
				}

				//��ʾ����
				if(bCardValue>=10)bOxSound=true;
				m_GameClientView.SetUserOxValue(wViewChairID,bCardValue);
			}
			else
			{
				//��ţ
				m_GameClientView.SetUserOxValue(wViewChairID,0);
			}
		}
	}
	m_GameClientView.DisplayTypeEx(true);

	//�ɼ���ʾ�ڼ�ʱ����Ի���
	//TCHAR szBuffer[256]=TEXT("");
	//_sntprintf(szBuffer,CountArray(szBuffer),TEXT("\n���ֽ���,�ɼ�ͳ��"));
	////m_pIStringMessage->InsertNormalString(szBuffer);
	//for (WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	if(pGameEnd->lGameScore[i]==0)continue;

	//	//�ɼ����
	//	//IClientUserItem * pUserData=GetTableUserItem(i);
	//	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s��%+I64d"),m_szAccounts[i],pGameEnd->lGameScore[i]);
	//	//m_pIStringMessage->InsertNormalString(szBuffer);
	//}

	//ţţ����
	if(bOxSound)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_OXOX"));
	}

	//��ʼ��ť
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_USER_START_GAME);
	}

	//״̬����
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	return true;
}

//���¿���
void CGameClientEngine::UpdateScoreControl(SCORE lScore[],BOOL bShow)
{


	return ;
}

//ת����Ϣ
void CGameClientEngine::ChangeUserInfo(BYTE bCardData[],BYTE bCardCount,CString &CardInfo)
{
	//ת����Ϣ
	for(BYTE i=0;i<bCardCount;i++)
	{
		//�˿˻�ɫ
		BYTE bCardColor = bCardData[i]&LOGIC_MASK_COLOR;
		switch(bCardColor)
		{
		case 0x00:
			{
				CardInfo+=TEXT("����");
			}
			break;
		case 0x10:
			{
				CardInfo+=TEXT("÷��");
			}
			break;
		case 0x20:
			{
				CardInfo+=TEXT("����");
			}
			break;
		case 0x30:
			{
				CardInfo+=TEXT("����");
			}
			break;
		}

		//�˿�����
		BYTE bTempCardData = bCardData[i]&LOGIC_MASK_VALUE;
		switch(bTempCardData)
		{
		case 0x01:CardInfo+=TEXT("��A��.");break;
		case 0x02:CardInfo+=TEXT("��2��.");break;
		case 0x03:CardInfo+=TEXT("��3��.");break;
		case 0x04:CardInfo+=TEXT("��4��.");break;
		case 0x05:CardInfo+=TEXT("��5��.");break;
		case 0x06:CardInfo+=TEXT("��6��.");break;
		case 0x07:CardInfo+=TEXT("��7��.");break;
		case 0x08:CardInfo+=TEXT("��8��.");break;
		case 0x09:CardInfo+=TEXT("��9��.");break;
		case 0x0a:CardInfo+=TEXT("��10��.");break;
		case 0x0b:CardInfo+=TEXT("��J��.");break;
		case 0x0c:CardInfo+=TEXT("��Q��.");break;
		case 0x0d:CardInfo+=TEXT("��K��.");break;
		}
	}

	return ;
}

//��ʾ��ť
LRESULT	CGameClientEngine::OnHintOx(WPARAM wParam, LPARAM lParam)
{
	if(m_GameLogic.GetOxCard(m_cbHandCardData[GetMeChairID()],MAX_COUNT))
	{
		m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetShootCard(m_cbHandCardData[GetMeChairID()],3);
		if(m_GameClientView.m_btOx.IsWindowVisible()==FALSE)OnShortcut(0,0);
		OnOxEnable(1,1);
	}
	else 
	{
		m_GameClientView.SetUserOxValue(MY_VIEW_CHAIRID,0);
	}

	return 0;
}

//��ʼ��ť
LRESULT	CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	if(!IsLookonMode())KillGameClock(IDI_START_GAME);

	//���ý���
	m_GameClientView.DisplayTypeEx(false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	//m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,SCORE_ZERO);
	m_GameClientView.SetUserDynamicJoin(FALSE);

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.ShowScore(i,false);
		m_GameClientView.m_CardControl[i].SetOX(false);
		m_GameClientView.SetUserOxValue(i,0xff);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		if(i!=m_wViewChairID[GetMeChairID()])m_GameClientView.m_CardControl[i].SetDisplayFlag(false);
		m_GameClientView.m_CardControlOx[i].SetCardData(NULL,0);
	}

	//��ע��Ϣ
	m_lTurnMaxScore=SCORE_ZERO;
	m_bDynamicJoin=FALSE;
	//m_lTurnLessScore=0L;m_cbHandCardData
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bUserOxCard,sizeof(m_bUserOxCard));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//������Ϣ
	if(!IsLookonMode()) SendUserReady(NULL,0);

	return 0;
}

//��ע��ť
LRESULT	CGameClientEngine::OnAddScore(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillTimer(IDI_TIME_USER_ADD_SCORE);

	//��ȡ����
	WORD wMeChairID=GetMeChairID();

	BYTE bTemp=(BYTE)wParam;
	SCORE lCurrentScore=m_GameClientView.m_JettonView.GetBetScore();

	//���ư�ť
	m_GameClientView.m_JettonView.ShowWindows(FALSE);
	//UpdateScoreControl(NULL,SW_HIDE);

	//��������
	m_lTableScore[wMeChairID]=lCurrentScore;
	m_GameClientView.SetUserTableScore(m_wViewChairID[wMeChairID],m_lTableScore[wMeChairID],true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentScore;
	SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	return 0;
}

//��ݼ���ť 
LRESULT CGameClientEngine::OnShortcut(WPARAM wParam, LPARAM lParam)
{
	//WORD wViewChairID=m_wViewChairID[GetMeChairID()];

	//if(m_GameClientView.m_btOx.IsWindowVisible()==FALSE)
	//{
	//	m_GameClientView.m_CardControl[wViewChairID].SetKey(false);
	//	m_GameClientView.m_btOx.ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	m_GameClientView.m_CardControl[wViewChairID].SetKey(true);
	//	m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
	//}
	m_GameClientView.m_btOx.EnableWindow(FALSE);

	return 0;
}

//ţţ��ť
LRESULT	CGameClientEngine::OnOx(WPARAM wParam, LPARAM lParam)
{
	//ţţ
	m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetOX(true);

	//ȡ����Ӧ
	m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetPositively(false);

	//����ţ��
	OnSortCard(0,0);

	//��ʾ����
	OnSetUserOxValue(0,0);

	m_GameClientView.m_btOx.EnableWindow(FALSE);

	//ֱ�ӿ���
	OnOpenCard(0,0);

	return 0;
}

//�������
LRESULT CGameClientEngine::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//��Ӧ�˿�
	WORD wMeChairID=GetMeChairID();
	if(!IsLookonMode())m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetPositively(true);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_NULLITY,TIME_USER_OPEN_CARD);
	SetTimer(IDI_TIME_NULLITY,1000,NULL);

	//�ȴ���־
	m_GameClientView.m_bOpenCard=true;

	//���ǹۿ�
	if (IsLookonMode())return 0;

	////��������
	//BYTE bCardType = m_GameLogic.GetCardType(m_cbHandCardData[wMeChairID],MAX_COUNT);
	//if(bCardType >= OX_THREE_SAME)
	//{
	//	//��ʾ��Ϣ
	//	m_GameClientView.SetUserAction(true);

	//	//�û���Ϣ
	//	WORD wViewChairID=m_wViewChairID[wMeChairID];
	//	m_GameClientView.m_CardControl[wViewChairID].ShootAllCard(false);

	//	//������Ϣ
	//	CMD_C_OxCard OxCard;
	//	OxCard.bOX=TRUE;
	//	SendSocketData(SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));

	//	//Ԥ�ȴ���
	//	m_GameClientView.ShowOpenCard(wViewChairID);
	//	m_GameClientView.m_CardControl[wViewChairID].SetPositively(false);

	//	//��ʾ����
	//	m_GameClientView.SetUserOxValue(wViewChairID,bCardType);

	//	//����ţ��Ϣ
	//	m_bUserOxCard[wMeChairID]=OxCard.bOX;

	//	return 0;
	//}

	//�ؼ�����
	if(!m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].GetKey())
	{
		m_GameClientView.m_btOx.EnableWindow(FALSE);
	}

	if(!m_bDynamicJoin)
	{
		m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btHintOx.ShowWindow(SW_SHOW);
	}

	//ʱ������
	SetTimer(IDI_TIME_OPEN_CARD,TIME_USER_OPEN_CARD*1000,NULL);

	return 0;
}

//̯�ư�ť
LRESULT	CGameClientEngine::OnOpenCard(WPARAM wParam, LPARAM lParam)
{
	WORD wViewChairID=MY_VIEW_CHAIRID;

	if(m_GameClientView.m_CardControl[wViewChairID].GetOX()==FALSE)
	{
		OnHintOx(0,0);
	}
	if(m_GameClientView.m_btOx.IsWindowEnabled()==TRUE)
	{
		//ţţ
		m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetOX(true);

		//ȡ����Ӧ
		m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].SetPositively(false);

		//����ţ��
		OnSortCard(0,0);

		m_GameClientView.m_btOx.EnableWindow(FALSE);
	}

	//ɾ��ʱ��
	KillTimer(IDI_TIME_OPEN_CARD);

	//����ؼ�
	m_GameClientView.m_btOx.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);
//	m_GameClientView.m_btReSort.ShowWindow(SW_HIDE);
//	m_GameClientView.m_btShortcut.ShowWindow(SW_HIDE);



	//������Ϣ
	CMD_C_OxCard OxCard;
	OxCard.bOX=(m_GameClientView.m_CardControl[wViewChairID].GetOX())?TRUE:FALSE;
	SendSocketData(SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));

	//Ԥ�ȴ���
	if(OxCard.bOX==FALSE)
	{
		//��ʾ������ţţ
		m_GameClientView.m_CardControl[wViewChairID].ShootAllCard(false);
		m_GameClientView.SetUserOxValue(wViewChairID,0);
	}
	m_GameClientView.ShowOpenCard(wViewChairID);
	m_GameClientView.m_CardControl[wViewChairID].SetPositively(false);

	//����ţ��Ϣ
	m_bUserOxCard[GetMeChairID()]=OxCard.bOX;

	PlayGameSound(AfxGetInstanceHandle(),TEXT("OPEN_CARD"));

	return 0;
}

//��ʾ����
LRESULT CGameClientEngine::OnSetUserOxValue(WPARAM wParam, LPARAM lParam)
{
	WORD wViewChairID=m_wViewChairID[GetMeChairID()];

	//��ȡ����
	BYTE bCardData[MAX_COUNT],bCardCount=0;
	ZeroMemory(bCardData,sizeof(bCardData));
	bCardCount=(BYTE)m_GameClientView.m_CardControlOx[wViewChairID].GetCardCount();
	ASSERT(bCardCount==2);
	m_GameClientView.m_CardControlOx[wViewChairID].GetCardData(bCardData,bCardCount);

	//��ʾ����
	BYTE bSum=m_GameLogic.GetCardLogicValue(bCardData[0])+m_GameLogic.GetCardLogicValue(bCardData[1]);
	if(bSum>10)bSum-=10;
	m_GameClientView.SetUserOxValue(wViewChairID,bSum);

	return 0;
}

//��ʾ��Ϣ
LRESULT CGameClientEngine::OnShowInfo(WPARAM wParam, LPARAM lParam)
{
	//��������
	TCHAR szBuffer[64];
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("����ʾ��Ϣ��:���ܹ���ţ"));
	//m_pIStringMessage->InsertNormalString(szBuffer);

	return 0;
}

//ţBTΪ��
LRESULT CGameClientEngine::OnOxEnable(WPARAM wParam, LPARAM lParam)
{
	BYTE bEnable=BYTE(wParam);
	m_GameClientView.m_btOx.EnableWindow(bEnable);
	if(bEnable==TRUE)
	{
		bool bShoot = ((BYTE(lParam)==TRUE)?false:true);
		BYTE cbCardData[MAX_COUNT];
		m_GameClientView.m_CardControl[MY_VIEW_CHAIRID].GetShootCard(cbCardData,bShoot);
		BYTE cbValue=m_GameLogic.GetCardLogicValue(cbCardData[0]);
		cbValue+=m_GameLogic.GetCardLogicValue(cbCardData[1]);
		if(cbValue>10)cbValue-=10;
		m_GameClientView.SetUserOxValue(MY_VIEW_CHAIRID,cbValue);
	}
	else 
	{
		BYTE cbValue=m_GameClientView.GetValue(MY_VIEW_CHAIRID);
		if(cbValue!=0xff && cbValue!=0)
			m_GameClientView.SetUserOxValue(MY_VIEW_CHAIRID,0xff);
	}

	return 0;
}

//�˿˷���
LRESULT CGameClientEngine::OnSortCard(WPARAM wParam, LPARAM lParam)
{
	//���ؿؼ�
	m_GameClientView.m_btHintOx.ShowWindow(SW_HIDE);

	//���ñ���
	WORD wViewChairID=m_wViewChairID[GetMeChairID()];

	//������Ŀ
	BYTE bCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCount();

	//���ñ���
	BYTE bCardOxData[MAX_COUNT];
	BYTE bCardValueData[MAX_COUNT];
	ZeroMemory(bCardOxData,sizeof(bCardOxData));
	ZeroMemory(bCardValueData,sizeof(bCardValueData));

	//��ȡţ��&������
	if(bCardCount==2)
	{
		BYTE bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardOxData,false);
		ASSERT(bCount==3);
		bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardValueData);
		ASSERT(bCount==2);
	}
	else
	{
		BYTE bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardValueData,false);
		ASSERT(bCount==2);
		bCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetShootCard(bCardOxData);
		ASSERT(bCount==3);
	}

	//�����˿�
	m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(bCardValueData,MAX_COUNT-3);
	m_GameClientView.m_CardControl[wViewChairID].SetCardData(bCardOxData,3);

	return 0;
}

//���Ű�ť
LRESULT CGameClientEngine::OnReSortCard(WPARAM wParam, LPARAM lParam)
{
	WORD wMeChiarID=GetMeChairID();
	WORD wViewChairID=m_wViewChairID[wMeChiarID];

	//ȡ��ţţ
	if(m_GameClientView.m_CardControl[wViewChairID].GetOX())
	{
		m_GameClientView.m_CardControlOx[wViewChairID].SetCardData(NULL,0);
		m_GameClientView.m_CardControl[wViewChairID].SetCardData(m_cbHandCardData[wMeChiarID],MAX_COUNT);
		m_GameClientView.m_CardControl[wViewChairID].SetPositively(true);
		m_GameClientView.m_CardControl[wViewChairID].SetOX(false);

		//ȡ����ʾ����
		m_GameClientView.SetUserOxValue(wViewChairID,0xff);
	}

	return 0;
}

//��ׯ��Ϣ
LRESULT CGameClientEngine::OnBanker(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameClock(IDI_CALL_BANKER);

	//�ؼ�����
	m_GameClientView.m_btBanker.ShowWindow(SW_HIDE);
	m_GameClientView.m_btIdler.ShowWindow(SW_HIDE);

	//���ñ���
	CMD_C_CallBanker CallBanker;
	CallBanker.bBanker = (BYTE)wParam;

	//������Ϣ
	SendSocketData(SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));

	return 0;
}

//�����Ϣ
LRESULT CGameClientEngine::OnClickCard(WPARAM wParam, LPARAM lParam)
{
	PlayGameSound(AfxGetInstanceHandle(),TEXT("CLICK_CARD"));
	
	return 0;
}

//��������
LRESULT CGameClientEngine::OnMessageYuYin(WPARAM wParam, LPARAM lParam)
{
	////Ч��
	//ASSERT( m_PhraseVoiceHelper.GetInterface() );
	//if( !m_PhraseVoiceHelper.GetInterface() ) return 0;

	//CRect rcControl;
	//m_GameClientView.m_btYuYin.GetWindowRect(rcControl);

	//m_PhraseVoiceHelper->SetBenchmarkPos(rcControl.left,rcControl.top-2);
	//m_PhraseVoiceHelper->ShowPhraseWindow();

	return 0;
}

//��ʱ����
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case IDI_TIME_USER_ADD_SCORE:
		{
			//���Դ���
			//m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);
			//ɾ����ʱ��
			KillTimer(IDI_TIME_USER_ADD_SCORE);

			//��ȡλ��
			WORD wMeChairID=GetMeChairID();
			WORD wViewChairID=m_wViewChairID[wMeChairID];

			//���ư�ť
			m_GameClientView.m_JettonView.ShowWindows(FALSE);
			//UpdateScoreControl(NULL,SW_HIDE);

			//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.lScore=__max(m_GameClientView.m_JettonView.GetBetScore(),m_lCellScore);
			SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			//��������
			m_lTableScore[wMeChairID]=AddScore.lScore;
			m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[wMeChairID],true);
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

			return;
		}
	case IDI_TIME_OPEN_CARD:
		{
			//ɾ����ʱ��
			KillTimer(IDI_TIME_OPEN_CARD);

			//̯��
			OnOpenCard(0,0);

			return;
		}
	case IDI_TIME_NULLITY:
		{
			//���½���
			m_GameClientView.InvalidGameView(0,0,0,0);
			
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////////
LRESULT CGameClientEngine::OnAdminCommand(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_AMDIN_COMMAND,(CMD_C_AdminReq*)wParam,sizeof(CMD_C_AdminReq));

	return true;
}
//////////////////////////////////////////////////////////////////////////
LRESULT CGameClientEngine::OnAdminQueryUser(WPARAM wParam,LPARAM lParam)
{
	DWORD	m_wGameID[GAME_PLAYER];
	TCHAR	m_szAccounts1[GAME_PLAYER][LEN_ACCOUNTS];	//�������

	memset(m_wGameID,0,sizeof(m_wGameID));
	ZeroMemory(m_szAccounts1,sizeof(m_szAccounts1));

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pClientUserItem=GetTableUserItem(i);
		IClientUserItem * pClientUserItem1=GetTableUserItem(GetMeChairID());

		if (pClientUserItem==NULL) continue;

		//�û���Ϣ
	//	lstrcpyn(m_szAccounts1[i],pClientUserItem->GetNickName(),CountArray(m_szAccounts1[i]));
		if(pClientUserItem->GetUserID()!=pClientUserItem1->GetUserID())
			_sntprintf(m_szAccounts1[i],CountArray(m_szAccounts1[i]),TEXT("%d"),pClientUserItem->GetUserID());
		else
			lstrcpyn(m_szAccounts1[i],pClientUserItem->GetNickName(),CountArray(m_szAccounts1[i]));

		m_wGameID[i] = pClientUserItem->GetUserID();
	}
	m_GameClientView.m_AdminDlg.SetUserInfo(m_szAccounts1,m_wGameID);

	return true;
}

