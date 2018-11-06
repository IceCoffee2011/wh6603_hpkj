#include "StdAfx.h"
#include "TableFrame.h"
#include "DataBasePacket.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//���߶���
#define IDI_OFF_LINE				(TIME_TABLE_SINK_RANGE+1)			//���߱�ʶ
#define TIME_OFF_LINE				30000L								//����ʱ��

//��Ϸ��¼
CGameScoreRecordArray				CTableFrame::m_GameScoreRecordBuffer;

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrame::CTableFrame()
{
	//��������
	m_wTableID=0;
	m_wChairCount=0;
	m_cbStartMode=START_MODE_ALL_READY;
    m_wUserCount=0;//<by hxh>

	//��־����
	m_bGameStarted=false;
	m_bDrawStarted=false;
	m_bTableStarted=false;
	ZeroMemory(m_bAllowLookon,sizeof(m_bAllowLookon));
	ZeroMemory(m_lFrozenedScore,sizeof(m_lFrozenedScore));

	//��Ϸ����
	m_lCellScore=0;
	m_cbGameStatus=GAME_STATUS_FREE;

	//��̬����
	m_dwTableOwnerID=0L;
	ZeroMemory(m_szEnterPassword,sizeof(m_szEnterPassword));

	//ʱ�����
	m_dwDrawStartTime=0L;
	ZeroMemory(&m_SystemTimeStart,sizeof(m_SystemTimeStart));

	//���߱���
	ZeroMemory(m_wOffLineCount,sizeof(m_wOffLineCount));
	ZeroMemory(m_dwOffLineTime,sizeof(m_dwOffLineTime));

	//������Ϣ
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//����ӿ�
	m_pITimerEngine=NULL;
	m_pITableFrameSink=NULL;
	m_pIMainServiceFrame=NULL;
	m_pIAndroidUserManager=NULL;

	//���Žӿ�
	m_pITableUserAction=NULL;
	m_pITableUserRequest=NULL;

	//���ݽӿ�
	m_pIKernelDataBaseEngine=NULL;
	m_pIRecordDataBaseEngine=NULL;

	//�û�����
	ZeroMemory(m_TableUserItemArray,sizeof(m_TableUserItemArray));

	return;
}

//��������
CTableFrame::~CTableFrame()
{
	//�ͷŶ���
	SafeRelease(m_pITableFrameSink);

	return;
}

//�ӿڲ�ѯ
VOID * CTableFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrame,Guid,dwQueryVer);
	return NULL;
}


//����֪ͨ��Ϸ��ʼ
//��ʼ��Ϸ
bool CTableFrame::StartGame()
{
	//��Ϸ״̬
	ASSERT(m_bDrawStarted==false);
	if (m_bDrawStarted==true) return false;

	//�������
	bool bGameStarted=m_bGameStarted;
	bool bTableStarted=m_bTableStarted;

	//����״̬
	m_bGameStarted=true;
	m_bDrawStarted=true;
	m_bTableStarted=true;
	m_dwDrawStartTime=(DWORD)time(NULL);

	//��ʼʱ��
	GetLocalTime(&m_SystemTimeStart);
	m_dwDrawStartTime=(DWORD)time(NULL);

	//��ʼ����
	if (bGameStarted==false)
	{
		//״̬����
		ZeroMemory(m_wOffLineCount,sizeof(m_wOffLineCount));
		ZeroMemory(m_dwOffLineTime,sizeof(m_dwOffLineTime));

		//�����û�
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);

			//�����û�
			if (pIServerUserItem!=NULL)
			{
				//����״̬
				BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
				if ((cbUserStatus!=US_OFFLINE)&&(cbUserStatus!=US_PLAYING)) pIServerUserItem->SetUserStatus(US_PLAYING,m_wTableID,i);
			}
		}

		//����״̬
		if (bTableStarted!=m_bTableStarted) SendTableStatus();             
	}

	//֪ͨ�¼�
	ASSERT(m_pITableFrameSink!=NULL);
	if (m_pITableFrameSink!=NULL) m_pITableFrameSink->OnEventGameStart();   //֪ͨ����Ϸ��ʼ��Ϸ

	return true;
}

//��ɢ��Ϸ
bool CTableFrame::DismissGame()
{
	//״̬�ж�
	ASSERT(m_bTableStarted==true);
	if (m_bTableStarted==false) return false;

	//������Ϸ
	if ((m_bGameStarted==true)&&(m_pITableFrameSink->OnEventGameConclude(INVALID_CHAIR,NULL,GER_DISMISS)==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//����״̬
	if ((m_bGameStarted==false)&&(m_bTableStarted==true))
	{
		//���ñ���
		m_bTableStarted=false;

		//����״̬
		SendTableStatus();
	}

	return false;
}

//������Ϸ
bool CTableFrame::ConcludeGame(BYTE cbGameStatus,LPCTSTR szDrawInfo,WORD cbMcYes)
{
	//Ч��״̬
	ASSERT(m_bGameStarted==true);
	if (m_bGameStarted==false) return false;

	//EWIN�������� �°汾��ɰ汾����
	if (cbMcYes!=608){
	
	
			//����ӿ�
			m_pITimerEngine=NULL;
			m_pITableFrameSink=NULL;
			m_pIMainServiceFrame=NULL;
			m_pIAndroidUserManager=NULL;

			//���Žӿ�
			m_pITableUserAction=NULL;
			m_pITableUserRequest=NULL;

			//���ݽӿ�
			m_pIKernelDataBaseEngine=NULL;
			m_pIRecordDataBaseEngine=NULL;
	
			return false;
	
	}  
	//�������
	bool bDrawStarted=m_bDrawStarted;

	//����״̬
	m_bDrawStarted=false;
	m_cbGameStatus=cbGameStatus;
	m_bGameStarted=(cbGameStatus>=GAME_STATUS_PLAY)?true:false;

	//��Ϸ��¼
	if (bDrawStarted==true)
	{
		if(CServerRule::IsRecordGameScore(m_pGameServiceOption->dwServerRule)==true)
		{
			//��������
			DBR_GR_GameScoreRecord GameScoreRecord;
			ZeroMemory(&GameScoreRecord,sizeof(GameScoreRecord));

			//���ñ���
			GameScoreRecord.wTableID=m_wTableID;
			GameScoreRecord.dwPlayTimeCount=(bDrawStarted==true)?(DWORD)time(NULL)-m_dwDrawStartTime:0;

			//��Ϸʱ��
			GameScoreRecord.SystemTimeStart=m_SystemTimeStart;
			GetLocalTime(&GameScoreRecord.SystemTimeConclude);

			//��Ϸ��¼
			if(szDrawInfo==NULL)
				GameScoreRecord.szDrawInfo[0] = 0;
			else
				lstrcpyn(GameScoreRecord.szDrawInfo,szDrawInfo,sizeof(GameScoreRecord.szDrawInfo));

			//�û�����
			for (INT_PTR i=0;i<m_GameScoreRecordActive.GetCount();i++)
			{
				//��ȡ����
				ASSERT(m_GameScoreRecordActive[i]!=NULL);
				tagGameScoreRecord * pGameScoreRecord=m_GameScoreRecordActive[i];

				//�û���Ŀ
				if (pGameScoreRecord->cbAndroid==FALSE)
				{
					GameScoreRecord.wUserCount++;
				}
				else
				{
					GameScoreRecord.wAndroidCount++;
				}

				//����ͳ��
				GameScoreRecord.lUserMedal+=pGameScoreRecord->lUserMedal;

				//ͳ����Ϣ
				if (pGameScoreRecord->cbAndroid==FALSE)
				{
					GameScoreRecord.lWasteCount-=pGameScoreRecord->lScore-pGameScoreRecord->lRevenue;
					GameScoreRecord.lRevenueCount+=pGameScoreRecord->lRevenue;

					//�ɼ���Ϣ
					if (GameScoreRecord.wRecordCount<CountArray(GameScoreRecord.GameScoreRecord))
					{
						WORD wIndex=GameScoreRecord.wRecordCount++;
						CopyMemory(&GameScoreRecord.GameScoreRecord[wIndex],pGameScoreRecord,sizeof(tagGameScoreRecord));
					}
				}				
			}

			//Ͷ������
			WORD wHeadSize=sizeof(GameScoreRecord)-sizeof(GameScoreRecord.GameScoreRecord);
			WORD wDataSize=sizeof(GameScoreRecord.GameScoreRecord[0])*GameScoreRecord.wRecordCount;

			//EWIN���磬ע������鿴���ݰ���û�г���16376�ĳ���
					//�������ݰ����ȣ����������ʾ����
					if((wHeadSize+wDataSize)>16375){

						//�������
						CTraceService::TraceString(TEXT("��Ϸ���������ݰ����ȳ������ֵ16376"),TraceLevel_Exception);

					}

			m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GR_GAME_SCORE_RECORD,0,&GameScoreRecord,wHeadSize+wDataSize);
		}

		//�����¼
		if (m_GameScoreRecordActive.GetCount()>0L)
		{
			m_GameScoreRecordBuffer.Append(m_GameScoreRecordActive);
			m_GameScoreRecordActive.RemoveAll();
		}
	}

	//��������
	if (m_bGameStarted==false)
	{
		//��������
		bool bOffLineWait=false;

		//�����û�
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);

			//�û�����
			if (pIServerUserItem!=NULL)
			{
				//�������
				if (m_lFrozenedScore[i]!=0L)
				{
					pIServerUserItem->UnFrozenedUserScore(m_lFrozenedScore[i]);
					m_lFrozenedScore[i]=0L;
				}

				//����״̬
				if (pIServerUserItem->GetUserStatus()==US_OFFLINE)
				{
					//���ߴ���
					bOffLineWait=true;
					PerformStandUpAction(pIServerUserItem);
				}
				else
				{
					//����״̬
					pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,i);

					//��������
					if (pIServerUserItem->IsAndroidUser()==true)
					{
						//���һ���
						CAttemperEngineSink * pAttemperEngineSink=(CAttemperEngineSink *)m_pIMainServiceFrame;
						tagBindParameter * pBindParameter=pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());
						IAndroidUserItem * pIAndroidUserItem=m_pIAndroidUserManager->SearchAndroidUserItem(pIServerUserItem->GetUserID(),
							pBindParameter->dwSocketID);

						//��������
						if (pIAndroidUserItem!=NULL)
						{
							//��ȡʱ��
							SYSTEMTIME SystemTime;
							GetLocalTime(&SystemTime);
							DWORD dwTimeMask=(1L<<SystemTime.wHour);

							//��ȡ����
							tagAndroidService * pAndroidService=pIAndroidUserItem->GetAndroidService();
							tagAndroidParameter * pAndroidParameter=pIAndroidUserItem->GetAndroidParameter();

							//������Ϣ
							if(pAndroidService->dwResidualPlayDraw>0)
								--pAndroidService->dwResidualPlayDraw;

							//����ʱ��
							if (((pIServerUserItem->GetTableID()==GetTableID())&&(pAndroidParameter->dwServiceTime&dwTimeMask)==0L))
							{
								PerformStandUpAction(pIServerUserItem);
								continue;
							}

							//��Ϸʱ��
							DWORD dwCurrentTime=(DWORD)time(NULL);
							DWORD dwLogonTime=pIServerUserItem->GetLogonTime();
							DWORD dwReposeTime=pAndroidService->dwReposeTime;
							if ((dwLogonTime+dwReposeTime)<dwCurrentTime)
							{
								PerformStandUpAction(pIServerUserItem);
								continue;
							}

							//�����ж�
							if ((pIServerUserItem->GetTableID()==GetTableID())&&(pAndroidService->dwResidualPlayDraw==0))
							{
								PerformStandUpAction(pIServerUserItem);
								continue;
							}
						}
					}
				}
			}
		}

		//ɾ��ʱ��
		if (bOffLineWait==true) KillGameTimer(IDI_OFF_LINE);
	}

	//��������
	ASSERT(m_pITableFrameSink!=NULL);
	if (m_pITableFrameSink!=NULL) m_pITableFrameSink->RepositionSink();

	//�߳����
	if (m_bGameStarted==false)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			if (m_TableUserItemArray[i]==NULL) continue;
			IServerUserItem * pIServerUserItem=m_TableUserItemArray[i];

			//��������
			if ((SCORE_EQUAL(m_pGameServiceOption->lMinTableScore,0L)==false)&&(SCORE_LESS(pIServerUserItem->GetUserScore(),m_pGameServiceOption->lMinTableScore)))
			{
				//������ʾ
				TCHAR szDescribe[128]=TEXT("");
				if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
				{
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("������Ϸ������ ") SCORE_STRING TEXT("�����ܼ�����Ϸ��"),m_pGameServiceOption->lMinTableScore);
				}
				else
				{
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("������Ϸ������ ") SCORE_STRING TEXT("�����ܼ�����Ϸ��"),m_pGameServiceOption->lMinTableScore);
				}

				//������Ϣ
				SendGameMessage(pIServerUserItem,szDescribe,SMT_CHAT|SMT_CLOSE_GAME|SMT_EJECT);

				//�û�����
				PerformStandUpAction(pIServerUserItem);

				continue;
			}

			//�ر��ж�
			if ((CServerRule::IsForfendGameEnter(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0))
			{
				//������Ϣ
				LPCTSTR pszMessage=TEXT("����ϵͳά������ǰ��Ϸ���ӽ�ֹ�û�������Ϸ��");
				SendGameMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_CHAT|SMT_CLOSE_GAME);

				//�û�����
				PerformStandUpAction(pIServerUserItem);

				continue;
			}
		}
	}

	return true;
}

//д�����
bool CTableFrame::WriteUserScore(WORD wChairID, tagScoreInfo & ScoreInfo)
{
	//Ч�����
	ASSERT((wChairID<m_wChairCount)&&(ScoreInfo.cbType!=SCORE_TYPE_NULL));
	if ((wChairID>=m_wChairCount)&&(ScoreInfo.cbType==SCORE_TYPE_NULL)) return false;

	//��ȡ�û�
	ASSERT(GetTableUserItem(wChairID)!=NULL);
	IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);

	//д�����
	if (pIServerUserItem!=NULL)
	{
		//��Ϸʱ��
		DWORD dwCurrentTime=(DWORD)time(NULL);
		DWORD dwPlayTimeCount=(m_bDrawStarted==true)?dwCurrentTime-m_dwDrawStartTime:0L;

		//���Ƽ���
		SCORE lUserMemal=SCORE_LESS(ScoreInfo.lScore,0L)?-ScoreInfo.lScore:0;

		//д�����
		pIServerUserItem->WriteUserScore(ScoreInfo.lScore,ScoreInfo.lRevenue,lUserMemal,ScoreInfo.cbType,dwPlayTimeCount);

		//��Ϸ��¼
		if (CServerRule::IsRecordGameScore(m_pGameServiceOption->dwServerRule)==true)
		{
			//��������
			tagGameScoreRecord * pGameScoreRecord=NULL;

			//��ѯ���
			if (m_GameScoreRecordBuffer.GetCount()>0L)
			{
				//��ȡ����
				INT_PTR nCount=m_GameScoreRecordBuffer.GetCount();
				pGameScoreRecord=m_GameScoreRecordBuffer[nCount-1];

				//ɾ������
				m_GameScoreRecordBuffer.RemoveAt(nCount-1);
			}

			//��������
			if (pGameScoreRecord==NULL)
			{
				try
				{
					//��������
					pGameScoreRecord=new tagGameScoreRecord;
					if (pGameScoreRecord==NULL) throw TEXT("��Ϸ��¼���󴴽�ʧ��");
				}
				catch (...)
				{
					ASSERT(FALSE);
				}
			}

			//��¼����
			if (pGameScoreRecord!=NULL)
			{
				//�û���Ϣ
				pGameScoreRecord->wChairID=wChairID;
				pGameScoreRecord->dwUserID=pIServerUserItem->GetUserID();
				pGameScoreRecord->cbAndroid=(pIServerUserItem->IsAndroidUser()?TRUE:FALSE);

				//�ɼ���Ϣ
				pGameScoreRecord->lScore=ScoreInfo.lScore;
				pGameScoreRecord->lGrade=ScoreInfo.lGrade;
				pGameScoreRecord->lRevenue=ScoreInfo.lRevenue;
				pGameScoreRecord->lUserMedal=lUserMemal;

				//������Ϣ
				pGameScoreRecord->dwPlayTimeCount=dwPlayTimeCount;

				//��������
				m_GameScoreRecordActive.Add(pGameScoreRecord);
			}
		}
	}
	else
	{
		//�뿪�û�
		CTraceService::TraceString(TEXT("ϵͳ��ʱδ֧���뿪�û��Ĳ��ֲ�������"),TraceLevel_Exception);

		return false;
	}

	return true;
}

//д�����
bool CTableFrame::WriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount)
{
	//Ч�����
	ASSERT(wScoreCount==m_wChairCount);
	if (wScoreCount!=m_wChairCount) return false;

	//д�����
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (ScoreInfoArray[i].cbType!=SCORE_TYPE_NULL)
		{
			WriteUserScore(i,ScoreInfoArray[i]);
		}
	}

	return true;
}

//����˰��
SCORE CTableFrame::CalculateRevenue(WORD wChairID, SCORE lScore)
{
	//Ч�����
	ASSERT(wChairID<m_wChairCount);
	if (wChairID>=m_wChairCount) return 0L;

	//����˰��
	if (lScore>=REVENUE_BENCHMARK)
	{
		//��ȡ�û�
		ASSERT(GetTableUserItem(wChairID)!=NULL);
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);

		//����˰��
		SCORE lRevenue=lScore*m_pGameServiceOption->cbRevenueRatio/REVENUE_DENOMINATOR;

		//���봦��
		lRevenue = Double_Round(lRevenue,2,1);

		return lRevenue;
	}

	return 0L;
}

//�����޶�
SCORE CTableFrame::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	//�û�Ч��
	ASSERT(pIServerUserItem->GetTableID()==m_wTableID);
	if (pIServerUserItem->GetTableID()!=m_wTableID) return 0L;

	//��ѯ���
	SCORE lTrusteeScore=pIServerUserItem->GetTrusteeScore();
	SCORE lMinEnterScore=m_pGameServiceOption->lMinTableScore;
	SCORE lUserConsumeQuota=m_pITableFrameSink->QueryConsumeQuota(pIServerUserItem);

	//Ч����
	ASSERT(SCORE_GREATER_EQUAL(lUserConsumeQuota,0L)&&SCORE_LESS_EQUAL(lUserConsumeQuota,pIServerUserItem->GetUserScore()-lMinEnterScore));
	if (SCORE_LESS(lUserConsumeQuota,0L)||SCORE_GREATER(lUserConsumeQuota,pIServerUserItem->GetUserScore()-lMinEnterScore)) return 0L;

	return lUserConsumeQuota+lTrusteeScore;
}

//Ѱ���û�
IServerUserItem * CTableFrame::SearchUserItem(DWORD dwUserID)
{
	//��������
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//�����û�
	for (WORD i=0;i<m_wChairCount;i++)
	{
		pIServerUserItem=GetTableUserItem(i);
		if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserID()==dwUserID)) return pIServerUserItem;
	}

	//�Թ��û�
	do
	{
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserID()==dwUserID)) return pIServerUserItem;
	} while (pIServerUserItem!=NULL);

	return NULL;
}

//��Ϸ�û�
IServerUserItem * CTableFrame::GetTableUserItem(WORD wChairID)
{
	//Ч�����
	ASSERT(wChairID<m_wChairCount);
	if (wChairID>=m_wChairCount) return NULL;

	//��ȡ�û�
	return m_TableUserItemArray[wChairID];
}

//�Թ��û�
IServerUserItem * CTableFrame::EnumLookonUserItem(WORD wEnumIndex)
{
	if (wEnumIndex>=m_LookonUserItemArray.GetCount()) return NULL;
	return m_LookonUserItemArray[wEnumIndex];
}

//����ʱ��
bool CTableFrame::SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)
{
	//Ч�����
	ASSERT((dwTimerID>0)&&(dwTimerID<TIME_TABLE_MODULE_RANGE));
	if ((dwTimerID<=0)||(dwTimerID>=TIME_TABLE_MODULE_RANGE)) return false;

	//����ʱ��
	DWORD dwEngineTimerID=IDI_TABLE_MODULE_START+m_wTableID*TIME_TABLE_MODULE_RANGE;
	if (m_pITimerEngine!=NULL) m_pITimerEngine->SetTimer(dwEngineTimerID+dwTimerID,dwElapse,dwRepeat,dwBindParameter);

	return true;
}

//ɾ��ʱ��
bool CTableFrame::KillGameTimer(DWORD dwTimerID)
{
	//Ч�����
	ASSERT((dwTimerID>0)&&(dwTimerID<=TIME_TABLE_MODULE_RANGE));
	if ((dwTimerID<=0)||(dwTimerID>TIME_TABLE_MODULE_RANGE)) return false;

	//ɾ��ʱ��
	DWORD dwEngineTimerID=IDI_TABLE_MODULE_START+m_wTableID*TIME_TABLE_MODULE_RANGE;
	if (m_pITimerEngine!=NULL) m_pITimerEngine->KillTimer(dwEngineTimerID+dwTimerID);

	return true;
}

//��������
bool CTableFrame::SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)
{
	//״̬Ч��
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)&&(pIServerUserItem->IsClientReady()==false)) return false;

	//��������
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);

	return true;
}

//��������
bool CTableFrame::SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//״̬Ч��
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)&&(pIServerUserItem->IsClientReady()==false)) return false;

	//��������
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);

	return true;
}

//��������
bool CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID)
{
	//�û�Ⱥ��
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;

			//Ч��״̬
			ASSERT(pIServerUserItem->IsClientReady()==true);
			if (pIServerUserItem->IsClientReady()==false) continue;

			//��������
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);
		}

		return true;
	}
	else
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) return false;

		//Ч��״̬
		ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//��������
		m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);

		return true;
	}

	return false;
}

//��������
bool CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//�û�Ⱥ��
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);
			if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

			//��������
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
		}

		return true;
	}
	else
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);
		if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) return false;

		CString strLog;
	    strLog.Format(L"SendTableData wChairID=%d",wChairID);
		OutputDebugStringW(strLog);

		//��������
		m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);

		return true;
	}

	return false;
}

//��������
bool CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID)
{
	//��������
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//ö���û�
	do
	{
		//��ȡ�û�
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//Ч��״̬
		ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//��������
		if ((wChairID==INVALID_CHAIR)||(pIServerUserItem->GetChairID()==wChairID))
		{
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);
		}

	} while (true);

	return true;
}

//��������
bool CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//��������
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//ö���û�
	do
	{
		//��ȡ�û�
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//Ч��״̬
		ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//��������
		if ((wChairID==INVALID_CHAIR)||(pIServerUserItem->GetChairID()==wChairID))
		{
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
		}

	} while (true);

	return true;
}

//������Ϣ
bool CTableFrame::SendGameMessage(LPCTSTR lpszMessage, WORD wType)
{
	//��������
	WORD wEnumIndex=0;

	//������Ϣ
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(i);
		if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

		//������Ϣ
		m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);
	}

	//ö���û�
	do
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//Ч��״̬
		ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//������Ϣ
		m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);

	} while (true);

	return true;
}

//������Ϣ
bool CTableFrame::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//������Ϣ
	m_pIMainServiceFrame->SendRoomMessage(pIServerUserItem,lpszMessage,wType);

	return true;
}

//��Ϸ��Ϣ
bool CTableFrame::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//�û�Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//������Ϣ
	return m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);
}

//���ͳ���
bool CTableFrame::SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	//�û�Ч��
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) return false;

	//���ͳ���
	ASSERT(m_pIMainServiceFrame!=NULL);
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_GAME_SCENE,pData,wDataSize);

	return true;
}

//�����¼�
bool CTableFrame::OnEventUserOffLine(IServerUserItem * pIServerUserItem)
{
	//����Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

 //   // add by hxh 20161102
 //   bool bRet=PerformStandUpAction(pIServerUserItem);
	//return bRet;

	//�û�����
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	IServerUserItem * pITableUserItem=m_TableUserItemArray[pUserInfo->wChairID];

	//�û�����
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

	//��Ϸ�û�
	if (cbUserStatus!=US_LOOKON)
	{
		//Ч���û�
		ASSERT(pIServerUserItem==GetTableUserItem(wChairID));
		if (pIServerUserItem!=GetTableUserItem(wChairID)) return false;

		//���ߴ���
		if (cbUserStatus==US_PLAYING)
		{
			//�ضϴ���
			if (m_dwOffLineTime[wChairID]!=0L)
			{
				//�û�����
				pIServerUserItem->SetClientReady(false);
				pIServerUserItem->SetUserStatus(US_OFFLINE,m_wTableID,wChairID);

				return true;
			}

			//�¼�֪ͨ
			if (m_pITableUserAction!=NULL)
			{
				m_pITableUserAction->OnActionUserOffLine(wChairID,pIServerUserItem);
			}

			//���ñ���
			m_wOffLineCount[wChairID]++;
			m_dwOffLineTime[wChairID]=(DWORD)time(NULL);

			//����֪ͨ
			pIServerUserItem->SetClientReady(false);
			pIServerUserItem->SetUserStatus(US_OFFLINE,m_wTableID,wChairID);

			//ʱ������
			WORD wOffLineCount=GetOffLineUserCount();
			if (wOffLineCount==1) SetGameTimer(IDI_OFF_LINE,TIME_OFF_LINE,1,wChairID);

			return true;
		}
	}

	//�û�����
	PerformStandUpAction(pIServerUserItem);

	//ɾ���û�
	ASSERT(pIServerUserItem->GetUserStatus()==US_FREE);
	pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);

	return true;
}

//�ؽ��¼�
bool CTableFrame::OnEventUserReConnect(IServerUserItem * pIServerUserItem)
{
	//����Ч��
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�û�����
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	IServerUserItem * pITableUserItem=m_TableUserItemArray[pUserInfo->wChairID];

	//�û�����
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

	//���ñ���
	m_dwOffLineTime[wChairID]=0L;

	return true;
}

//ʱ���¼�
bool CTableFrame::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//�ص��¼�
	if ((dwTimerID>=0)&&(dwTimerID<TIME_TABLE_SINK_RANGE))
	{
		ASSERT(m_pITableFrameSink!=NULL);
		return m_pITableFrameSink->OnTimerMessage(dwTimerID,dwBindParameter);
	}

	//�¼�����
	switch (dwTimerID)
	{
	case IDI_OFF_LINE:	//�����¼�
		{
			//Ч��״̬
			ASSERT(m_bGameStarted==true);
			if (m_bGameStarted==false) return false;

			//��������
			DWORD dwOffLineTime=0L;
			WORD wOffLineChair=INVALID_CHAIR;

			//Ѱ���û�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				if ((m_dwOffLineTime[i]!=0L)&&((m_dwOffLineTime[i]<dwOffLineTime)||(wOffLineChair==INVALID_CHAIR)))
				{
					wOffLineChair=i;
					dwOffLineTime=m_dwOffLineTime[i];
				}
			}

			//λ���ж�
			ASSERT(wOffLineChair!=INVALID_CHAIR);
			if (wOffLineChair==INVALID_CHAIR) return false;

			//�û��ж�
			ASSERT(dwBindParameter<m_wChairCount);
			if (wOffLineChair!=(WORD)dwBindParameter)
			{
				//ʱ�����
				DWORD dwCurrentTime=(DWORD)time(NULL);
				DWORD dwLapseTime=dwCurrentTime-m_dwOffLineTime[wOffLineChair];

				//����ʱ��
				dwLapseTime=__min(dwLapseTime,TIME_OFF_LINE-2000L);
				SetGameTimer(IDI_OFF_LINE,TIME_OFF_LINE-dwLapseTime,1,wOffLineChair);

				return true;
			}

			//��ȡ�û�
			ASSERT(GetTableUserItem(wOffLineChair)!=NULL);
			IServerUserItem * pIServerUserItem=GetTableUserItem(wOffLineChair);

			//������Ϸ
			if (pIServerUserItem!=NULL)
			{
				//���ñ���
				m_dwOffLineTime[wOffLineChair]=0L;

				//�û�����
				PerformStandUpAction(pIServerUserItem);
			}

			//����ʱ��
			if (m_bGameStarted==true)
			{
				//��������
				DWORD dwOffLineTime=0L;
				WORD wOffLineChair=INVALID_CHAIR;

				//Ѱ���û�
				for (WORD i=0;i<m_wChairCount;i++)
				{
					if ((m_dwOffLineTime[i]!=0L)&&((m_dwOffLineTime[i]<dwOffLineTime)||(wOffLineChair==INVALID_CHAIR)))
					{
						wOffLineChair=i;
						dwOffLineTime=m_dwOffLineTime[i];
					}
				}

				//����ʱ��
				if (wOffLineChair!=INVALID_CHAIR)
				{
					//ʱ�����
					DWORD dwCurrentTime=(DWORD)time(NULL);
					DWORD dwLapseTime=dwCurrentTime-m_dwOffLineTime[wOffLineChair];

					//����ʱ��
					dwLapseTime=__min(dwLapseTime,TIME_OFF_LINE-2000L);
					SetGameTimer(IDI_OFF_LINE,TIME_OFF_LINE-dwLapseTime,1,wOffLineChair);
				}
			}

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��Ϸ�¼�
bool CTableFrame::OnEventSocketGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(m_pITableFrameSink!=NULL);

	//��Ϣ����
	return m_pITableFrameSink->OnGameMessage(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//����¼�
bool CTableFrame::OnEventSocketFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//��Ϸ����
	if (m_pITableFrameSink->OnFrameMessage(wSubCmdID,pData,wDataSize,pIServerUserItem)==true) return true;

	////��������
	//if(m_pIGameMatchSink!=NULL && m_pIGameMatchSink->OnFrameMessage(wSubCmdID,pData,wDataSize,pIServerUserItem)==true) return true;

	//Ĭ�ϴ���
	switch (wSubCmdID)
	{
	case SUB_GF_GAME_OPTION:	//��Ϸ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GF_GameOption));
			if (wDataSize!=sizeof(CMD_GF_GameOption)) return false;

			//��������
			CMD_GF_GameOption * pGameOption=(CMD_GF_GameOption *)pData;

			//��ȡ����
			WORD wChairID=pIServerUserItem->GetChairID();
			BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

			//��������
			if ((cbUserStatus!=US_LOOKON)&&((m_dwOffLineTime[wChairID]!=0L)))
			{
				//���ñ���
				m_dwOffLineTime[wChairID]=0L;

				//ɾ��ʱ��
				WORD wOffLineCount=GetOffLineUserCount();
				if (wOffLineCount==0) KillGameTimer(IDI_OFF_LINE);
			}

			//����״̬
			pIServerUserItem->SetClientReady(true);
			if (cbUserStatus!=US_LOOKON) m_bAllowLookon[wChairID]=pGameOption->cbAllowLookon?true:false;

			//����״̬
			CMD_GF_GameStatus GameStatus;
			GameStatus.cbGameStatus=m_cbGameStatus;
			GameStatus.cbAllowLookon=m_bAllowLookon[wChairID]?TRUE:FALSE;
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_GAME_STATUS,&GameStatus,sizeof(GameStatus));

			//�¼�֪ͨ
			if ((cbUserStatus==US_PLAYING)&&(m_pITableUserAction!=NULL))
			{
				m_pITableUserAction->OnActionUserConnect(wChairID,pIServerUserItem);
			}

			//���ͳ���
			bool bSendSecret=((cbUserStatus!=US_LOOKON)||(m_bAllowLookon[wChairID]==true));
			m_pITableFrameSink->OnEventSendGameScene(wChairID,pIServerUserItem,m_cbGameStatus,bSendSecret);

			//��ʼ�ж�
			if ((cbUserStatus==US_READY)&&(EfficacyStartGame(wChairID)==true))
			{
				StartGame(); 
			}

			return true;
		}
	case SUB_GF_USER_READY:		//�û�׼��  //EWIN����
		{
			//��ȡ����
			WORD wChairID=pIServerUserItem->GetChairID();
			BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

			//Ч��״̬
			ASSERT(GetTableUserItem(wChairID)==pIServerUserItem);
			if (GetTableUserItem(wChairID)!=pIServerUserItem) return false;

			//Ч��״̬
			ASSERT(cbUserStatus==US_SIT);
			if (cbUserStatus!=US_SIT) return true;

			//�����ж�
			if ((m_pGameServiceOption->cbDistributeRule&DISTRIBUTE_ALLOW)!=0)
			{
				m_pIMainServiceFrame->InsertDistribute(pIServerUserItem);
				return true;
			}

			//�¼�֪ͨ
			if (m_pITableUserAction!=NULL)
			{
				m_pITableUserAction->OnActionUserOnReady(wChairID,pIServerUserItem,pData,wDataSize);
			}

			// add by hxh 20161031
#if 1
			//��������
			CMD_GR_UserStatus UserStatus;
			ZeroMemory(&UserStatus,sizeof(UserStatus));

			//��������
			UserStatus.dwUserID=pIServerUserItem->GetUserID();
			UserStatus.UserStatus.wTableID=pIServerUserItem->GetTableID();
			UserStatus.UserStatus.wChairID=pIServerUserItem->GetChairID();
			UserStatus.UserStatus.cbUserStatus=US_READY;

			//��Ϸ�û�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)/*||(pIServerUserItem->IsClientReady()==false)*/) continue;

				//��������
				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
			}
#endif

			//��ʼ�ж�
            BYTE cbUserStatus1=US_READY;
			if (EfficacyStartGame(wChairID)==false)
			{
				pIServerUserItem->SetUserStatus(US_READY,m_wTableID,wChairID);
			}
			else
			{
				StartGame();
                cbUserStatus1=US_PLAYING;
			}

			// add by hxh 20161031
#if 0
            CMD_GR_UserStatus rsp;
			ZeroMemory(&rsp,sizeof(rsp));
			rsp.dwUserID=pIServerUserItem->GetUserID();
			rsp.UserStatus.wTableID=pIServerUserItem->GetTableID();
			rsp.UserStatus.wChairID=wChairID;
            rsp.UserStatus.cbUserStatus=cbUserStatus1;
            //m_pIMainServiceFrame->SendData(BG_ALL_CLIENT,MDM_GF_FRAME,SUB_GF_USER_READY,&rsp,sizeof(rsp));
			//��Ϸ�û�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)/*||(pIServerUserItem->IsClientReady()==false)*/) continue;

				//��������
				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_READY,&rsp,sizeof(rsp));
			}
#endif

			return true;
		}
	case SUB_GF_USER_CHAT:		//�û�����
		{
			//��������
			CMD_GF_C_UserChat * pUserChat=(CMD_GF_C_UserChat *)pData;

			//Ч�����
			ASSERT(wDataSize<=sizeof(CMD_GF_C_UserChat));
			ASSERT(wDataSize>=(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)));
			ASSERT(wDataSize==(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

			//Ч�����
			if (wDataSize>sizeof(CMD_GF_C_UserChat)) return false;
			if (wDataSize<(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString))) return false;
			if (wDataSize!=(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

			//Ŀ���û�
			if ((pUserChat->dwTargetUserID!=0)&&(SearchUserItem(pUserChat->dwTargetUserID)==NULL))
			{
				ASSERT(FALSE);
				return true;
			}

			//״̬�ж�
			if ((CServerRule::IsForfendGameChat(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0L))
			{
				SendGameMessage(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ�����ֹ��Ϸ���죡"),SMT_CHAT);
				return true;
			}

			//Ȩ���ж�
			if (CUserRight::CanRoomChat(pIServerUserItem->GetUserRight())==false)
			{
				SendGameMessage(pIServerUserItem,TEXT("��Ǹ����û����Ϸ�����Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"),SMT_EJECT|SMT_CHAT);
				return true;
			}

			//������Ϣ
			CMD_GF_S_UserChat UserChat;
			ZeroMemory(&UserChat,sizeof(UserChat));

			//�ַ�����
			lstrcpyn(UserChat.szChatString,pUserChat->szChatString,CountArray(UserChat.szChatString));

			//��������
			UserChat.dwChatColor=pUserChat->dwChatColor;
			UserChat.wChatLength=pUserChat->wChatLength;
			UserChat.dwTargetUserID=pUserChat->dwTargetUserID;
			UserChat.dwSendUserID=pIServerUserItem->GetUserID();
			UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

			//��Ϸ�û�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

				//��������
				WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
				WORD wSendSize=wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]);
				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wSendSize);
			}

			//�Թ��û�
			WORD wEnumIndex=0;
			IServerUserItem * pIServerUserItem=NULL;

			//ö���û�
			do
			{
				//��ȡ�û�
				pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
				if (pIServerUserItem==NULL) break;

				//��������
				if (pIServerUserItem->IsClientReady()==true)
				{
					WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
					WORD wSendSize=wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]);
					m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wSendSize);
				}
			} while (true);

			return true;
		}
	case SUB_GF_USER_EXPRESSION:	//�û�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GF_C_UserExpression));
			if (wDataSize!=sizeof(CMD_GF_C_UserExpression)) return false;

			//��������
			CMD_GF_C_UserExpression * pUserExpression=(CMD_GF_C_UserExpression *)pData;

			//Ŀ���û�
			if ((pUserExpression->dwTargetUserID!=0)&&(SearchUserItem(pUserExpression->dwTargetUserID)==NULL))
			{
				ASSERT(FALSE);
				return true;
			}

			//״̬�ж�
			if ((CServerRule::IsForfendGameChat(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0L))
			{
				SendGameMessage(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ�����ֹ��Ϸ���죡"),SMT_CHAT);
				return true;
			}

			//Ȩ���ж�
			if (CUserRight::CanRoomChat(pIServerUserItem->GetUserRight())==false)
			{
				SendGameMessage(pIServerUserItem,TEXT("��Ǹ����û����Ϸ�����Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��"),SMT_EJECT|SMT_CHAT);
				return true;
			}

			//������Ϣ
			CMD_GR_S_UserExpression UserExpression;
			ZeroMemory(&UserExpression,sizeof(UserExpression));

			//��������
			UserExpression.wItemIndex=pUserExpression->wItemIndex;
			UserExpression.dwSendUserID=pIServerUserItem->GetUserID();
			UserExpression.dwTargetUserID=pUserExpression->dwTargetUserID;

			//��Ϸ�û�
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

				//��������
				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));
			}

			//�Թ��û�
			WORD wEnumIndex=0;
			IServerUserItem * pIServerUserItem=NULL;

			//ö���û�
			do
			{
				//��ȡ�û�
				pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
				if (pIServerUserItem==NULL) break;

				//��������
				if (pIServerUserItem->IsClientReady()==true)
				{
					m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserExpression,sizeof(UserExpression));
				}
			} while (true);

			return true;
		}
	case SUB_GF_LOOKON_CONFIG:		//�Թ�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GF_LookonConfig));
			if (wDataSize<sizeof(CMD_GF_LookonConfig)) return false;

			//��������
			CMD_GF_LookonConfig * pLookonConfig=(CMD_GF_LookonConfig *)pData;

			//Ŀ���û�
			if ((pLookonConfig->dwUserID!=0)&&(SearchUserItem(pLookonConfig->dwUserID)==NULL))
			{
				ASSERT(FALSE);
				return true;
			}

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()!=US_LOOKON);
			if (pIServerUserItem->GetUserStatus()==US_LOOKON) return false;

			//�Թ۴���
			if (pLookonConfig->dwUserID!=0L)
			{
				for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
				{
					//��ȡ�û�
					IServerUserItem * pILookonUserItem=m_LookonUserItemArray[i];
					if (pILookonUserItem->GetUserID()!=pLookonConfig->dwUserID) continue;
					if (pILookonUserItem->GetChairID()!=pIServerUserItem->GetChairID()) continue;

					//������Ϣ
					CMD_GF_LookonStatus LookonStatus;
					LookonStatus.cbAllowLookon=pLookonConfig->cbAllowLookon;

					//������Ϣ
					ASSERT(m_pIMainServiceFrame!=NULL);
					m_pIMainServiceFrame->SendData(pILookonUserItem,MDM_GF_FRAME,SUB_GF_LOOKON_STATUS,&LookonStatus,sizeof(LookonStatus));

					break;
				}
			}
			else
			{
				//�����ж�
				bool bAllowLookon=(pLookonConfig->cbAllowLookon==TRUE)?true:false;
				if (bAllowLookon==m_bAllowLookon[pIServerUserItem->GetChairID()]) return true;

				//���ñ���
				m_bAllowLookon[pIServerUserItem->GetChairID()]=bAllowLookon;

				//������Ϣ
				CMD_GF_LookonStatus LookonStatus;
				LookonStatus.cbAllowLookon=pLookonConfig->cbAllowLookon;

				//������Ϣ
				for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
				{
					//��ȡ�û�
					IServerUserItem * pILookonUserItem=m_LookonUserItemArray[i];
					if (pILookonUserItem->GetChairID()!=pIServerUserItem->GetChairID()) continue;

					//������Ϣ
					ASSERT(m_pIMainServiceFrame!=NULL);
					m_pIMainServiceFrame->SendData(pILookonUserItem,MDM_GF_FRAME,SUB_GF_LOOKON_STATUS,&LookonStatus,sizeof(LookonStatus));
				}
			}

			return true;
		}
	}

	return false;
}

//��ȡ��λ
WORD CTableFrame::GetNullChairID()
{
	//��������
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_TableUserItemArray[i]==NULL)
		{
			return i;
		}
	}

	return INVALID_CHAIR;
}

//�����λ
WORD CTableFrame::GetRandNullChairID()
{
	//��������
	WORD wIndex = rand()%m_wChairCount;
	for (WORD i=wIndex;i<m_wChairCount+wIndex;i++)
	{
		if (m_TableUserItemArray[i%m_wChairCount]==NULL)
		{
			return i%m_wChairCount;
		}
	}

	return INVALID_CHAIR;
}

//������Ŀ
WORD CTableFrame::GetAndroidCount()
{
	//��������
	WORD wAndroidCount=0;

	//��Ŀͳ��
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//�û��ӿ�
		IServerUserItem * pIServerUserItem = GetTableUserItem(i);
		if ((pIServerUserItem!=NULL) && (pIServerUserItem->IsAndroidUser()==true)) 
			++wAndroidCount;
	}

	return wAndroidCount;
}

//�û���Ŀ
WORD CTableFrame::GetSitUserCount()
{
	//��������
	WORD wUserCount=0;

	//��Ŀͳ��
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (GetTableUserItem(i)!=NULL) wUserCount++;
	}

	return wUserCount;
}

//�Թ���Ŀ
WORD CTableFrame::GetLookonUserCount()
{
	//��ȡ��Ŀ
	INT_PTR nLookonCount=m_LookonUserItemArray.GetCount();

	return (WORD)(nLookonCount);
}

//������Ŀ
WORD CTableFrame::GetOffLineUserCount()
{
	//��������
	WORD wOffLineCount=0;

	//��������
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_dwOffLineTime[i]!=0L) wOffLineCount++;
	}

	return wOffLineCount;
}


//����״��
WORD CTableFrame::GetTableUserInfo(tagTableUserInfo & TableUserInfo)
{
	//���ñ���
	ZeroMemory(&TableUserInfo,sizeof(TableUserInfo));

	//�û�����
	for (WORD i=0;i<m_pGameServiceAttrib->wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//�û�����
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			TableUserInfo.wTableUserCount++;
		}
		else
		{
			TableUserInfo.wTableAndroidCount++;
		}

		//׼���ж�
		if (pIServerUserItem->GetUserStatus()==US_READY)
		{
			TableUserInfo.wTableReadyCount++;
		}
	}

	//������Ŀ
	switch (m_cbStartMode)
	{
	case START_MODE_ALL_READY:		//����׼��
		{
			TableUserInfo.wMinUserCount=2;
			break;
		}
	case START_MODE_PAIR_READY:		//��Կ�ʼ
		{
			TableUserInfo.wMinUserCount=2;
			break;
		}
	case START_MODE_TIME_CONTROL:	//ʱ�����
		{
			TableUserInfo.wMinUserCount=1;
			break;
		}
	default:						//Ĭ��ģʽ
		{
			TableUserInfo.wMinUserCount=m_pGameServiceAttrib->wChairCount;
			break;
		}
	}

	return TableUserInfo.wTableAndroidCount+TableUserInfo.wTableUserCount;
}

//��������
bool CTableFrame::InitializationFrame(WORD wTableID, tagTableFrameParameter & TableFrameParameter)
{
	//���ñ���
	m_wTableID=wTableID;
	m_wChairCount=TableFrameParameter.pGameServiceAttrib->wChairCount;

	CString strLog;
	strLog.Format(L"CTableFrame::InitializationFrame m_wTableID=[%d],m_wChairCount=[%d]",m_wTableID,m_wChairCount);
	CTraceService::TraceString(strLog,TraceLevel_Normal);

	//���ò���
	m_pGameParameter=TableFrameParameter.pGameParameter;
	m_pGameServiceAttrib=TableFrameParameter.pGameServiceAttrib;
	m_pGameServiceOption=TableFrameParameter.pGameServiceOption;

	//����ӿ�
	m_pITimerEngine=TableFrameParameter.pITimerEngine;
	m_pIMainServiceFrame=TableFrameParameter.pIMainServiceFrame;
	m_pIAndroidUserManager=TableFrameParameter.pIAndroidUserManager;
	m_pIKernelDataBaseEngine=TableFrameParameter.pIKernelDataBaseEngine;
	m_pIRecordDataBaseEngine=TableFrameParameter.pIRecordDataBaseEngine;

	//��������
	IGameServiceManager * pIGameServiceManager=TableFrameParameter.pIGameServiceManager;
	m_pITableFrameSink=(ITableFrameSink *)pIGameServiceManager->CreateTableFrameSink(IID_ITableFrameSink,VER_ITableFrameSink);

	//�����ж�
	if (m_pITableFrameSink==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	IUnknownEx * pITableFrame=QUERY_ME_INTERFACE(IUnknownEx);
	if (m_pITableFrameSink->Initialization(pITableFrame)==false) return false;

	//���ñ���
	m_lCellScore=m_pGameServiceOption->lCellScore;

	//��չ�ӿ�
	m_pITableUserAction=QUERY_OBJECT_PTR_INTERFACE(m_pITableFrameSink,ITableUserAction);
	m_pITableUserRequest=QUERY_OBJECT_PTR_INTERFACE(m_pITableFrameSink,ITableUserRequest);

	return true;
}

//��������
bool CTableFrame::PerformStandUpAction(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->GetTableID()==m_wTableID);
	ASSERT(pIServerUserItem->GetChairID()<=m_wChairCount);

	//�û�����
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	//��Ϸ�û�
	if ((m_bGameStarted==true)&&((cbUserStatus==US_PLAYING)||(cbUserStatus==US_OFFLINE)))
	{
		//������Ϸ
		BYTE cbConcludeReason=(cbUserStatus==US_OFFLINE)?GER_NETWORK_ERROR:GER_USER_LEAVE;
		m_pITableFrameSink->OnEventGameConclude(wChairID,pIServerUserItem,cbConcludeReason);

		//�뿪�ж�
		if (m_TableUserItemArray[wChairID]!=pIServerUserItem) return true;
	}

	//���ñ���
	if (pIServerUserItem==pITableUserItem)
	{
		//���ñ���
		m_TableUserItemArray[wChairID]=NULL;

		//�¼�֪ͨ
		if (m_pITableUserAction!=NULL)
		{
			m_pITableUserAction->OnActionUserStandUp(wChairID,pIServerUserItem,false);
		}

		//�û�״̬
		pIServerUserItem->SetClientReady(false);
		pIServerUserItem->SetUserStatus((cbUserStatus==US_OFFLINE)?US_NULL:US_FREE,INVALID_TABLE,INVALID_CHAIR);

		//��������
		bool bTableLocked=IsTableLocked();
		bool bTableStarted=IsTableStarted();
		WORD wTableUserCount=GetSitUserCount();

		//���ñ���<by hxh>
		m_wUserCount=wTableUserCount;

		//������Ϣ
		if (wTableUserCount==0)
		{
			m_dwTableOwnerID=0L;
			m_szEnterPassword[0]=0;
		}

		//��������
		if ((m_bGameStarted==false)&&(m_bTableStarted==true))
		{
			if (m_cbStartMode==START_MODE_FULL_READY) m_bTableStarted=false;
			if (m_cbStartMode==START_MODE_PAIR_READY) m_bTableStarted=false;
			if ((m_cbStartMode==START_MODE_ALL_READY)&&(wTableUserCount<=1)) m_bTableStarted=false;
		}

		//��ʼ�ж�
		if (EfficacyStartGame(INVALID_CHAIR)==true)
		{
			StartGame();
		}

		//����״̬
		if ((bTableLocked!=IsTableLocked())||(bTableStarted!=IsTableStarted()))
		{
			SendTableStatus();
		}

		return true;
	}
	else
	{
		//��������
		for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
		{
			if (pIServerUserItem==m_LookonUserItemArray[i])
			{
				//ɾ������
				m_LookonUserItemArray.RemoveAt(i);

				//�¼�֪ͨ
				if (m_pITableUserAction!=NULL)
				{
					m_pITableUserAction->OnActionUserStandUp(wChairID,pIServerUserItem,true);
				}

				//�û�״̬
				pIServerUserItem->SetClientReady(false);
				pIServerUserItem->SetUserStatus(US_FREE,INVALID_TABLE,INVALID_CHAIR);

				return true;
			}
		}

		//�������
		ASSERT(FALSE);
	}

	return true;
}

//�Թ۶���
bool CTableFrame::PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT((pIServerUserItem!=NULL)&&(wChairID<m_wChairCount));
	ASSERT((pIServerUserItem->GetTableID()==INVALID_TABLE)&&(pIServerUserItem->GetChairID()==INVALID_CHAIR));

	//��������
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	//״̬�ж�
	if ((CServerRule::IsForfendGameLookon(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ�����ֹ�û��Թۣ�"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);
		return false;
	}

	//��Ϸ״̬
	if ((m_bTableStarted==false)&&(pIServerUserItem->GetMasterOrder()==0L))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ϸ��û�п�ʼ�������Թ۴���Ϸ����"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);
		return false;
	}

	//�����ж�
	if ((pITableUserItem==NULL)&&(pIServerUserItem->GetMasterOrder()==0L))
	{
		SendRequestFailure(pIServerUserItem,TEXT("���������λ��û����Ϸ��ң��޷��Թ۴���Ϸ��"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);
		return false;
	}

	//��չЧ��
	if (m_pITableUserRequest!=NULL)
	{
		//��������
		tagRequestResult RequestResult;
		ZeroMemory(&RequestResult,sizeof(RequestResult));

		//����Ч��
		if (m_pITableUserRequest->OnUserRequestLookon(wChairID,pIServerUserItem,RequestResult)==false)
		{
			//������Ϣ
			SendRequestFailure(pIServerUserItem,RequestResult.szFailureReason,RequestResult.cbFailureCode,REQUEST_FAILURE_CMD_CONCLUDE);

			return false;
		}
	}

	//�����û�
	m_LookonUserItemArray.Add(pIServerUserItem);

	//�û�״̬
	pIServerUserItem->SetClientReady(false);
	pIServerUserItem->SetUserStatus(US_LOOKON,m_wTableID,wChairID);

	//�¼�֪ͨ
	if (m_pITableUserAction!=NULL)
	{
		m_pITableUserAction->OnActionUserSitDown(wChairID,pIServerUserItem,true);
	}

	return true;
}


//EWIN����
//���¶���
bool CTableFrame::PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT((pIServerUserItem!=NULL)&&(wChairID<m_wChairCount));
	ASSERT((pIServerUserItem->GetTableID()==INVALID_TABLE)&&(pIServerUserItem->GetChairID()==INVALID_CHAIR));

	//��������
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	//���ֱ���
	SCORE lUserScore=pIServerUserItem->GetUserScore();
	SCORE lMinTableScore=m_pGameServiceOption->lMinTableScore;
	SCORE lLessEnterScore=m_pITableFrameSink->QueryLessEnterScore(wChairID,pIServerUserItem);

	//״̬�ж�
	if ((CServerRule::IsForfendGameEnter(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ǹ����ǰ��Ϸ���ӽ�ֹ�û����룡"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);
		return false;
	}

	//��̬����
	bool bDynamicJoin=true;
	if (m_pGameServiceAttrib->cbDynamicJoin==FALSE) bDynamicJoin=false;
	if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule)==false) bDynamicJoin=false;

	//��Ϸ״̬
	if ((m_bTableStarted==true)&&(bDynamicJoin=false))
	{
		SendRequestFailure(pIServerUserItem,TEXT("��Ϸ�Ѿ���ʼ�ˣ����ڲ��ܽ�����Ϸ����"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);
		return false;
	}

	//�����ж�
	if (pITableUserItem!=NULL)
	{
		//������Ϣ
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�����Ѿ��� [ %s ] �����ȵ��ˣ��´ζ���Ҫ����ˣ�"),pITableUserItem->GetNickName());
		CString str;
		str.Format(L"ttt ���ӱ�[%s]",pITableUserItem->GetNickName());
		OutputDebugString(str);

		//������Ϣ
		SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);

		return false;
	}

	//��������
	if (((SCORE_EQUAL(lMinTableScore,0L)==false)&&SCORE_LESS(lUserScore,lMinTableScore))||((SCORE_EQUAL(lLessEnterScore,0L)==false)&&SCORE_LESS(lUserScore,lLessEnterScore)))
	{
		//������Ϣ
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("������Ϸ������Ҫ ") SCORE_STRING TEXT(" ��Ϸ�ң�������Ϸ�Ҳ��������ܼ��룡"),__max(lMinTableScore,lLessEnterScore));

		//������Ϣ
		SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NOSCORE,REQUEST_FAILURE_CMD_CONCLUDE);

		return false;
	}

	//��չЧ��
	if (m_pITableUserRequest!=NULL)
	{
		//��������
		tagRequestResult RequestResult;
		ZeroMemory(&RequestResult,sizeof(RequestResult));

		//����Ч��
		if (m_pITableUserRequest->OnUserRequestSitDown(wChairID,pIServerUserItem,RequestResult)==false)
		{
			//������Ϣ
			SendRequestFailure(pIServerUserItem,RequestResult.szFailureReason,RequestResult.cbFailureCode,REQUEST_FAILURE_CMD_CONCLUDE);

			return false;
		}
	}

	//���ñ���
	m_TableUserItemArray[wChairID]=pIServerUserItem;

	//�û�״̬
	if (IsGameStarted()==false||(m_cbStartMode!=START_MODE_TIME_CONTROL))//by hxh 20161031
	{
		//if (CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)==false)
        if (CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)==false && (m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0)
		{
			pIServerUserItem->SetClientReady(false);
			pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,wChairID);
		}
		else
		{
			pIServerUserItem->SetClientReady(false);
			pIServerUserItem->SetUserStatus(US_READY,m_wTableID,wChairID);
		}
	}
	else
	{
		pIServerUserItem->SetClientReady(false);
		pIServerUserItem->SetUserStatus(US_PLAYING,m_wTableID,wChairID);
	}

	//���ñ���<by hxh>
	m_wUserCount=GetSitUserCount();

	//������Ϣ<by hxh 20161101>
	if (GetSitUserCount()==1)
	{
		//״̬����
		bool bTableLocked=IsTableLocked();

		//���ñ���
		m_dwTableOwnerID=pIServerUserItem->GetUserID();
		//lstrcpyn(m_szEnterPassword,pUserRule->szPassword,CountArray(m_szEnterPassword));

		//����״̬
		if (bTableLocked!=IsTableLocked()) SendTableStatus();
	}

	//�¼�֪ͨ
	if (m_pITableUserAction!=NULL)
	{
		m_pITableUserAction->OnActionUserSitDown(wChairID,pIServerUserItem,false);
	}

	return true;
}

//����״̬
bool CTableFrame::SendTableStatus()
{
	//��������
	CMD_GR_TableStatus TableStatus;
	ZeroMemory(&TableStatus,sizeof(TableStatus));

	//��������
	TableStatus.wTableID=m_wTableID;
	TableStatus.TableStatus.cbTableLock=IsTableLocked()?TRUE:FALSE;
	TableStatus.TableStatus.cbPlayStatus=IsTableStarted()?TRUE:FALSE;

	//��������
	//m_pIMainServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_STATUS,SUB_GR_TABLE_STATUS,&TableStatus,sizeof(TableStatus));//by hxh 20160622
    m_pIMainServiceFrame->SendData(BG_COMPUTER,MDM_GR_STATUS,SUB_GR_TABLE_STATUS,&TableStatus,sizeof(TableStatus));

	//�ֻ�����

	return true;
}

//����ʧ��
bool CTableFrame::SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, BYTE cbFailureCode,BYTE cbCommandCode)
{
	//��������
	CMD_GR_RequestFailure RequestFailure;
	ZeroMemory(&RequestFailure,sizeof(RequestFailure));

	//��������
	RequestFailure.cbFailureCode=cbFailureCode;
	RequestFailure.cbCommandCode=cbCommandCode;
	lstrcpyn(RequestFailure.szDescribeString,pszDescribe,CountArray(RequestFailure.szDescribeString));

	//��������
	WORD wDataSize=CountStringBuffer(RequestFailure.szDescribeString);
	WORD wHeadSize=sizeof(RequestFailure)-sizeof(RequestFailure.szDescribeString);
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_REQUEST_FAILURE,&RequestFailure,wHeadSize+wDataSize);

	return true;
}

//��ʼЧ��
bool CTableFrame::EfficacyStartGame(WORD wReadyChairID)
{
	//״̬�ж�
	if (m_bGameStarted==true) return false;

	//ģʽ����
	if (m_cbStartMode==START_MODE_TIME_CONTROL) return false;
	if (m_cbStartMode==START_MODE_MASTER_CONTROL) return false;

	//׼������
	WORD wReadyUserCount=0;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pITableUserItem=GetTableUserItem(i);
		if (pITableUserItem==NULL) continue;

		//�û�ͳ��
		if (pITableUserItem!=NULL)
		{
			//״̬�ж�
			if (pITableUserItem->IsClientReady()==false) return false;
			if ((wReadyChairID!=i)&&(pITableUserItem->GetUserStatus()!=US_READY)) return false;

			//�û�����
			wReadyUserCount++;
		}
	}

	//��ʼ����
	switch (m_cbStartMode)
	{
	case START_MODE_ALL_READY:			//����׼��
		{
			//��Ŀ�ж�
			if (wReadyUserCount>=2L) return true;

			return false;
		}
	case START_MODE_FULL_READY:			//���˿�ʼ
		{
			//�����ж�
			if (wReadyUserCount==m_wChairCount) return true;

			return false;
		}
	case START_MODE_PAIR_READY:			//��Կ�ʼ
		{
			//��Ŀ�ж�
			if (wReadyUserCount==m_wChairCount) return true;
			if ((wReadyUserCount<2L)||(wReadyUserCount%2)!=0) return false;

			//λ���ж�
			for (WORD i=0;i<m_wChairCount/2;i++)
			{
				//��ȡ�û�
				IServerUserItem * pICurrentUserItem=GetTableUserItem(i);
				IServerUserItem * pITowardsUserItem=GetTableUserItem(i+m_wChairCount/2);

				//λ�ù���
				if ((pICurrentUserItem==NULL)&&(pITowardsUserItem!=NULL)) return false;
				if ((pICurrentUserItem!=NULL)&&(pITowardsUserItem==NULL)) return false;
			}

			return true;
		}
	default:
		{
			ASSERT(FALSE);
			return false;
		}
	}

	return false;
}


//��ַЧ��
bool CTableFrame::EfficacyIPAddress(IServerUserItem * pIServerUserItem)
{
	return true;
}

//����Ч��
bool CTableFrame::EfficacyScoreRule(IServerUserItem * pIServerUserItem)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
