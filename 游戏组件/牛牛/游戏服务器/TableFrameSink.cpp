#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����

SCORE			CTableFrameSink::m_lStockScore = 0.0;
LONGLONG		CTableFrameSink::m_lStorageOff = 0L;
BYTE			CTableFrameSink::StorageStyle=1;
SCORE			CTableFrameSink::m_lAndroidWinAndLose = 0.0;
//////////////////////////////////////////////////////////////////////////

#define	IDI_SO_OPERATE							12							//����ʱ��
#define	TIME_SO_OPERATE							80000						//����ʱ��

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ��¼
	m_GameRecord.Empty();
	ZeroMemory(m_szNickName,sizeof(m_szNickName));

	//��Ϸ����	
	m_lExitScore=0;	
	m_wBankerUser=INVALID_CHAIR;
	m_wFisrtCallUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//�û�״̬
	ZeroMemory(m_cbDynamicJoin,sizeof(m_cbDynamicJoin));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_bCallStatus,sizeof(m_bCallStatus));
	for(BYTE i=0;i<m_wPlayerCount;i++)m_bOxCard[i]=0xff;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

		//������Ϣ
	m_dwCheatCount=0;
	m_dwCheatGameID=0;
	m_cbCheatType=CHEAT_TYPE_WIN;
	
	m_lStockScore = 0.00;
	m_lAndroidWinAndLose=0.0;
	m_lStorageOff = 0L;
	m_bWinnerCtrl=false;
	m_bAndroidWinningCtrl = false;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ--��������Ϣ�汾
VOID *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
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

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//��ȡ����
	_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\OxServer.ini"),szPath);
	//��ʼ�洢��Ŀ
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);

		//��ȡ���
		///////////////////��ȡС��ת�� �γ���������
		TCHAR OutBuf[255];
		memset(OutBuf,0,255);
		if(m_lStockScore > -0.0000001 && m_lStockScore < 0.000000001)
		{
			GetPrivateProfileString(szServerName,TEXT("StorageStart"),TEXT("0.0"),OutBuf,255,m_szFileName);

			CString TempValue;
			myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
			m_lStockScore=Mywtof(TempValue);
			m_lAndroidWinAndLose = m_lStockScore*2;
		}
		///////////////////////////////

	//��ȡ˥��ֵ
	m_lStorageDeduct=GetPrivateProfileInt(szServerName,TEXT("StorageDeduct"),10,m_szFileName);

	//�������˥��ֵ
	if( m_lStorageDeduct < 0 || m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 0;
	if ( m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 1000;

	//�γ��������
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
		if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
	m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);
	//�γ��������
	m_bWinnerCtrl=false;
	m_bAndroidWinningCtrl = false;

	ReadConfigInformation(true);
	return true;
}

//��λ����
VOID  CTableFrameSink::RepositionSink()
{
		//��Ϸ��¼
	m_GameRecord.Empty();

	//��Ϸ����
	m_lExitScore=0;	
	m_wCurrentUser=INVALID_CHAIR;

	//�û�״̬
	ZeroMemory(m_cbDynamicJoin,sizeof(m_cbDynamicJoin));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_bCallStatus,sizeof(m_bCallStatus));
	for(BYTE i=0;i<m_wPlayerCount;i++)m_bOxCard[i]=0xff;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

	return;
}


//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserEnter(pIServerUserItem->GetChairID());

	if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
		m_cbDynamicJoin[pIServerUserItem->GetChairID()]=true;

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) 
	{
		m_HistoryScore.OnEventUserLeave(pIServerUserItem->GetChairID());
		m_cbDynamicJoin[pIServerUserItem->GetChairID()]=false;
		ZeroMemory(m_szNickName[pIServerUserItem->GetChairID()],sizeof(m_szNickName[pIServerUserItem->GetChairID()]));
	}

	return true;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{

	//��ȡ����
	ReadConfigInformation(true);
	//����״̬
	m_pITableFrame->SetGameStatus(GS_TK_CALL);

	//�û�״̬
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ�û�
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);

		if(pIServerUser==NULL)
		{
			m_cbPlayStatus[i]=FALSE;
		}
		else
		{
			m_cbPlayStatus[i]=TRUE;
			//�����û��ǳ�
			_sntprintf(m_szNickName[i],CountArray(m_szNickName[i]),TEXT("%s"),pIServerUser->GetNickName());
		}
	}

	//�׾����ʼ��
	if(m_wFisrtCallUser==INVALID_CHAIR)
	{
		m_wFisrtCallUser=rand()%m_wPlayerCount;  //����û�
	}
	else
	{
		m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
	}

	//ʼ���û�
	while(m_cbPlayStatus[m_wFisrtCallUser]!=TRUE)
	{
		m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
	}

	////��ׯ�û�
	//��ǰ�û�
	m_wCurrentUser=m_wFisrtCallUser;

	//���ñ���
	CMD_S_CallBanker CallBanker;
	CallBanker.wCallBanker=m_wCurrentUser;
	CallBanker.bFirstTimes=true;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]!=TRUE) continue;
		m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));



	m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);


	BYTE bTempArray[GAME_PLAYER*MAX_COUNT];
	m_GameLogic.RandCardList(bTempArray,sizeof(bTempArray));

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);	
		if(pIServerUser==NULL)continue;
		//�ɷ��˿�
		CopyMemory(m_cbHandCardData[i],&bTempArray[i*MAX_COUNT],MAX_COUNT);
	}


			
	//�·����ǿ�����ص�
	////////////////////////////////////////////////////////
	//�ж��Ƿ����ָ�����
	//WORD wChairIDCtrl = INVALID_CHAIR;

	//if(m_dwCheatCount>0)  //���ƴ���
	//{
	//	m_bWinnerCtrl = true;  //������ƴ�������0��ָ�������Ӯ���ƿ���

	//	if(m_dwCheatGameID==0)  //������ID����0 ָ�������Ӯ���ƹر�
	//	{
	//		m_bWinnerCtrl=false;  
	//	}
	//	else   //���ID����0
	//	{
	//		//����ID
	//		wChairIDCtrl = GetChairIdByGameId(m_dwCheatGameID);  //��ȡ������Ӻ�

	//		//ָ����Ҳ���
	//		if(wChairIDCtrl==INVALID_CHAIR)
	//		{
	//			m_bWinnerCtrl=false;
	//		}
	//	}
	//}else{
	//	//���ƴ�������0
	//	m_bWinnerCtrl=false;
	//}





//////////////////////////////////////////����ָ����ҿ���

	////�Ƿ�������
	//if (m_bWinnerCtrl)  //ָ���������ȵ�һ
	//{
	//	OutputDebugStringA("hhh 111");
	//	//m_cbCheatTypeʤ������ 1Ϊʤ��0Ϊ��
	//	ProbeJudge(wChairIDCtrl,m_cbCheatType);

	//}
	//else if (m_bAndroidWinningCtrl)//������Ӯ�ʿ��Ƶڶ�
	//{
	//	OutputDebugStringA("hhh 222");
	//	//m_bCtrlThisTime = true;
	//	AndroidWinningControl();  //�����˻��ƻ���
	//	

	//}
	//else  //���������ȼ����ŵ��� Ҳ�����
	//{
	//	OutputDebugStringA("hhh 333");
	//	//m_bCtrlThisTime = false;
	//	
	//	//����������
	//	AndroidCheck();
	//	
	//}



	//////////////��������
	return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			WORD m_wWinTimes[GAME_PLAYER],m_wWinCount[GAME_PLAYER];
			ZeroMemory(m_wWinCount,sizeof(m_wWinCount));
			ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

			//�����˿�
			BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
			CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

			//ׯ�ұ���
			ASSERT(m_bOxCard[m_wBankerUser]<2);
			if(m_bOxCard[m_wBankerUser]==TRUE)
				m_wWinTimes[m_wBankerUser]=m_GameLogic.GetTimes(cbUserCardData[m_wBankerUser],MAX_COUNT);
			else m_wWinTimes[m_wBankerUser]=1;

			//�Ա����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;

				ASSERT(m_bOxCard[i]<2);

				//�Ա��˿�
				if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,m_bOxCard[i],m_bOxCard[m_wBankerUser])) 
				{
					m_wWinCount[i]++;
					//��ȡ����
					if(m_bOxCard[i]==TRUE)
						m_wWinTimes[i]=m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT);
					else m_wWinTimes[i]=1;
				}
				else
				{
					m_wWinCount[m_wBankerUser]++;
				}
			}

			//ͳ�Ƶ÷�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;

				WORD j=i;
				if(m_wWinCount[j]>0)	//�м�ʤ��
				{
					GameEnd.lGameScore[j]=Double_Round(m_lTableScore[j]*m_wWinTimes[j],2,1);
					GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[j];
					m_lTableScore[j]=0.;
				}
				else					//ׯ��ʤ��
				{
					GameEnd.lGameScore[j]=Double_Round(-m_lTableScore[j]*m_wWinTimes[m_wBankerUser],2,1);
					GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[j];
					m_lTableScore[j]=0.;
				}
			}

			//�м�ǿ�˷���	
			GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

			//�뿪�û�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_lTableScore[i]>COMPARE_ZERO)GameEnd.lGameScore[i]=Double_Round(-m_lTableScore[i],2,1);
			}

			//�޸Ļ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;
				//˰��
				if(( GAME_GENRE_SCORE & m_pGameServiceOption->wServerType )==0)
				{
					if (GameEnd.lGameScore[i]>COMPARE_ZERO)
					{
						GameEnd.lGameTax[i] =  m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
						GameEnd.lGameScore[i] -=GameEnd.lGameTax[i] ;
					}
				}
				ScoreInfoArray[i].lRevenue = GameEnd.lGameTax[i] ;
				ScoreInfoArray[i].lScore=GameEnd.lGameScore[i]; 
				ScoreInfoArray[i].cbType = (GameEnd.lGameScore[i]>COMPARE_ZERO)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				
				//��Ϸ��¼
				m_GameRecord.AppendFormat(TEXT("%s�÷�:%0.2f,%0.2f#"),m_szNickName[i],ScoreInfoArray[i].lScore, ScoreInfoArray[i].lRevenue);

				m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);
			}

			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE) continue;
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			//ͳ�ƽ���д�ֲ�������
			/////////////////////////////////////////////////////////
			////ͳ�ƿ����Ϣ ֻ�д��ڻ����˲��Ҵ�����ʵ�û�������²Ż����
			bool  bUserAI = false;  //�Ƿ���ڻ�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (pIServerUserItemTemp->IsAndroidUser()){
					//���ڻ�����
					bUserAI = true;
					//����FOR
					break;

				}
			}
			//ͳ����Ϣ�������
			SCORE lSystemScore=0.0;    //��ҵ÷�
			SCORE lCurrentStorgeNum = m_lStockScore; //��ǰ���ֵ
			SCORE RevenueScore = 0.0;	 //˰��
			SCORE lStorageDeduct=0.0;	//���˥��
			//���ͳ�� 
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				//����û�������
				if (pIServerUserIte==NULL)continue;					
				//���ڻ����˲�ִ��
				if(bUserAI){
					//����û����ǻ�����
					if(!pIServerUserIte->IsAndroidUser())
					{
						//����˰��
						if(ScoreInfoArray[i].lScore>0) RevenueScore = Double_Round(ScoreInfoArray[i].lRevenue,2,1);
						//������˥��
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//ϵͳ���ֵ
						m_lStockScore-=(Double_Round(ScoreInfoArray[i].lScore,2,1)+lStorageDeduct); 
						//ϵͳ�÷�
						lSystemScore-=ScoreInfoArray[i].lScore;
						m_lAndroidWinAndLose += abs(ScoreInfoArray[i].lScore);
					}
				}
			}

			////////////////////////////////////////////////////////////

			//�ж��Ƿ������ʵ���
			bool bUser = false;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (!pIServerUserItemTemp->IsAndroidUser()){
					//������ʵ�û�
					bUser = true;
					//����FOR
					break;

				}
			}

			//�ж��Ƿ������ʵ���
			if (bUser)
			{
				
				CString strLog;
				strLog.Format(TEXT("���ţ�%d,��ǰ��棺%0.2f,ϵͳ�÷֣�%0.2f,˰�գ�%0.2f,ʣ���棺%0.2f,���˥����%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ


				//�γ��������
				m_lStockLimit+=lSystemScore;
				
				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
				//��¼����Ӯ��¼
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//������ڿ����û��Ļ�//������Ϸ��Ϣ�����ƺ�
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //��ȡ�û��ʺ�
					if(pUserItem==NULL) continue;	//����û�������
					if(pUserItem->IsAndroidUser()) continue;  //����ǻ�����
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //�ж��û�����Ȩ��
					m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //������Ϣ����
				}
			}

			//////////////////////////////////////////////////////
			//�۳����
			m_lStorageOff=GetPrivateProfileInt(m_pGameServiceOption->szServerName,_T("StorageOff"),0,m_szFileName);
			//����ۼ���������0
			if (m_lStorageOff != 0)
			{
				m_lStockScore -= m_lStorageOff;
				TCHAR szStorageMin[1024]=TEXT("");
				_sntprintf(szStorageMin,CountArray(szStorageMin),TEXT("��Ϊ����������%I64d��ʣ���棺%0.2f"),m_lStorageOff, m_lStockScore);
				//�����Ϣ
				CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
				//������֮��Ѹ�ֵ���0
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageOff"),TEXT("0"),m_szFileName);

				//���Ϳ��ƺŴ�����ʾ
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //��ȡ�û��ʺ�
					if(pUserItem==NULL) continue; //����û�������
					if(pUserItem->IsAndroidUser()) continue; //����ǻ�����
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue; //�ж��û�����Ȩ��
					m_pITableFrame->SendGameMessage(pUserItem,szStorageMin,SMT_EJECT);  //������Ϣ����

				}
			}

			//������ݴ������

			/////////////////////////////////////////////////
			

			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);
			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case  GER_NETWORK_ERROR:
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount && (m_cbPlayStatus[wChairID]==TRUE||m_cbDynamicJoin[wChairID]==FALSE));

			if(m_cbPlayStatus[wChairID]==FALSE||m_cbDynamicJoin[wChairID]==TRUE) return true;
			//����״̬
			m_cbPlayStatus[wChairID]=FALSE;
			m_cbDynamicJoin[wChairID]=FALSE;

			m_GameRecord.AppendFormat(TEXT("%s�뿪#"),m_szNickName[wChairID]);

			//�������
			CMD_S_PlayerExit PlayerExit;
			PlayerExit.wPlayerID=wChairID;

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==wChairID || (m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)) continue;
				m_pITableFrame->SendTableData(i,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));

			WORD m_wWinTimes[GAME_PLAYER];
			if (m_pITableFrame->GetGameStatus()>GS_TK_CALL)   //��Ϸ�Ѿ���ʼ
			{
				//���屣���ұ���
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

				if (wChairID==m_wBankerUser)	//ׯ��ǿ��
				{
					//�������
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));
					ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

					BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
					CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

					//�÷ֱ���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;
						m_wWinTimes[i]=(m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING)?(1):(m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT));
					}

					//ͳ�Ƶ÷� ���»�û��
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;
						GameEnd.lGameScore[i]=Double_Round(m_lTableScore[i]*m_wWinTimes[i],2,1);
						GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[i];
						m_lTableScore[i]=SCORE_ZERO;
					}

					//�м�ǿ�˷��� 
					GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

					//�뿪�û�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_lTableScore[i]>0.001)GameEnd.lGameScore[i]=Double_Round(-m_lTableScore[i],2,1);
					}


					//����˰��
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(( GAME_GENRE_SCORE & m_pGameServiceOption->wServerType )==0)
						{
							if (GameEnd.lGameScore[i]>0.001)
							{
								GameEnd.lGameTax[i] = m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]); 
								GameEnd.lGameScore[i] -=GameEnd.lGameTax[i] ;
							}
						}
					}

				

					//������Ϣ
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || (m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE))continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					
					//�޸Ļ���
					tagScoreInfo ScoreInfo[GAME_PLAYER];
					ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_cbPlayStatus[i]!=TRUE && i!=m_wBankerUser) continue;
						ScoreInfo[i].lRevenue=GameEnd.lGameTax[i];
						ScoreInfo[i].lScore=GameEnd.lGameScore[i];
		
						if(GameEnd.lGameScore[i]>0.001)
							ScoreInfo[i].cbType=SCORE_TYPE_WIN;
						else
							ScoreInfo[i].cbType=SCORE_TYPE_LOSE;
		
						//��Ϸ��¼
						m_GameRecord.AppendFormat(TEXT("%s�÷�:%0.2f,%0.2f#"),m_szNickName[i],ScoreInfo[i].lScore, ScoreInfo[i].lRevenue);
					}
					
					m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

					
					//������Ϸ
					m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);
				}
				else						//�м�ǿ��
				{

					//�Ѿ���ע
					if (m_lTableScore[wChairID]>COMPARE_ZERO)
					{
						ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

						//�û��˿�
						BYTE cbUserCardData[MAX_COUNT];
						CopyMemory(cbUserCardData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);

						//�û�����
						m_wWinTimes[m_wBankerUser]=(m_pITableFrame->GetGameStatus()==GS_TK_SCORE)?(1):(m_GameLogic.GetTimes(cbUserCardData,MAX_COUNT));
					
						//�޸Ļ���
						SCORE lScore=Double_Round(-m_lTableScore[wChairID]*m_wWinTimes[m_wBankerUser],2,1);
						m_lExitScore+=(-lScore);
						m_lTableScore[wChairID]=(-lScore);

						ScoreInfoArray[wChairID].lScore = lScore;
						ScoreInfoArray[wChairID].cbType = SCORE_TYPE_LOSE;
						//��Ϸ��¼
						m_GameRecord.AppendFormat(TEXT("%s�÷�:%0.2f,%0.2f#"),m_szNickName[wChairID],ScoreInfoArray[wChairID].lScore, ScoreInfoArray[wChairID].lRevenue);

					}

					//�������
					WORD wUserCount=0;
					for (WORD i=0;i<m_wPlayerCount;i++)if(m_cbPlayStatus[i]==TRUE)wUserCount++;

					//////////////////////////////////////////////////////////////////
					//������Ϸ
					if(wUserCount==1)
					{
						//�������
						CMD_S_GameEnd GameEnd;
						ZeroMemory(&GameEnd,sizeof(GameEnd));
						ASSERT(m_lExitScore>-COMPARE_ZERO); 

						//ͳ�Ƶ÷�
						GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;
						if (GameEnd.lGameScore[m_wBankerUser]>COMPARE_ZERO && (GAME_GENRE_GOLD &m_pGameServiceOption->wServerType)!=0 )
						{
							GameEnd.lGameTax[m_wBankerUser] = m_pITableFrame->CalculateRevenue(m_wBankerUser,GameEnd.lGameScore[m_wBankerUser]); 
							GameEnd.lGameScore[m_wBankerUser] -=GameEnd.lGameTax[m_wBankerUser] ;
						}

						//�뿪�û�
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							if(m_lTableScore[i]>0.001)GameEnd.lGameScore[i]=Double_Round(-m_lTableScore[i],2,1);
						}

						//������Ϣ
						m_pITableFrame->SendTableData(m_wBankerUser,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
						m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

						WORD Zero=0;
						for ( Zero=0;Zero<m_wPlayerCount;Zero++)
						{
							if(m_lTableScore[Zero]>COMPARE_ZERO||m_lTableScore[Zero]<-COMPARE_ZERO)
								break;
						}
						if(Zero!=m_wPlayerCount)
						{
							if(m_wBankerUser!=INVALID_CHAIR)
							{
								ScoreInfoArray[m_wBankerUser].lScore =GameEnd.lGameScore[m_wBankerUser];
								ScoreInfoArray[m_wBankerUser].lRevenue = GameEnd.lGameTax[m_wBankerUser];
								ScoreInfoArray[m_wBankerUser].cbType =SCORE_TYPE_WIN;
								m_GameRecord.AppendFormat(TEXT("�÷�:%0.2f,%0.2f#"),m_szNickName[m_wBankerUser],ScoreInfoArray[m_wBankerUser].lScore, ScoreInfoArray[m_wBankerUser].lRevenue);
							
							}

						}
						m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

						//������Ϸ
						m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);	
					}
					else if	(m_pITableFrame->GetGameStatus()==GS_TK_SCORE )
					{
						if( m_lTableScore[wChairID]<COMPARE_ZERO &&m_lTableScore[wChairID]>-COMPARE_ZERO)
							OnUserAddScore(wChairID,SCORE_ZERO);
						else
							m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
					}
					else if (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING && m_bOxCard[wChairID]==0xff)
					{
						OnUserOpenCard(wChairID,0);
						m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
					}
					//////////////////////////////////////////////////////////////////

				}

			

			//ͳ�ƽ���д�ֲ�������
			/////////////////////////////////////////////////////////
			////ͳ�ƿ����Ϣ ֻ�д��ڻ����˲��Ҵ�����ʵ�û�������²Ż����
			bool  bUserAI = false;  //�Ƿ���ڻ�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (pIServerUserItemTemp->IsAndroidUser()){
					//���ڻ�����
					bUserAI = true;
					//����FOR
					break;

				}
			}
			//ͳ����Ϣ�������
			SCORE lSystemScore=0.0;    //��ҵ÷�
			SCORE lCurrentStorgeNum = m_lStockScore; //��ǰ���ֵ
			SCORE RevenueScore = 0.0;	 //˰��
			SCORE lStorageDeduct=0.0;	//���˥��
			//���ͳ�� 
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				//����û�������
				if (pIServerUserIte==NULL)continue;					
				//���ڻ����˲�ִ��
				if(bUserAI){
					//����û����ǻ�����
					if(!pIServerUserIte->IsAndroidUser())
					{
						//����˰��
						if(ScoreInfoArray[i].lScore>0) RevenueScore = Double_Round(ScoreInfoArray[i].lRevenue,2,1);
						//������˥��
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//ϵͳ���ֵ
						m_lStockScore-=(Double_Round(ScoreInfoArray[i].lScore,2,1)+lStorageDeduct); 
						//ϵͳ�÷�
						lSystemScore-=ScoreInfoArray[i].lScore;
						m_lAndroidWinAndLose += abs(ScoreInfoArray[i].lScore);
					}
				}
			}

			////////////////////////////////////////////////////////////

			//�ж��Ƿ������ʵ���
			bool bUser = false;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (!pIServerUserItemTemp->IsAndroidUser()){
					//������ʵ�û�
					bUser = true;
					//����FOR
					break;

				}
			}

			//�ж��Ƿ������ʵ���
			if (bUser)
			{
				
				CString strLog;
				strLog.Format(TEXT("���ţ�%d,��ǰ��棺%0.2f,ϵͳ�÷֣�%0.2f,˰�գ�%0.2f,ʣ���棺%0.2f,���˥����%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ


				//�γ��������
				m_lStockLimit+=lSystemScore;

				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
				//��¼����Ӯ��¼
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//������ڿ����û��Ļ�//������Ϸ��Ϣ�����ƺ�
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //��ȡ�û��ʺ�
					if(pUserItem==NULL) continue;	//����û�������
					if(pUserItem->IsAndroidUser()) continue;  //����ǻ�����
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //�ж��û�����Ȩ��
					m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //������Ϣ����
				}
			}

			//////////////////////////////////////////////////////
			//�۳����
			m_lStorageOff=GetPrivateProfileInt(m_pGameServiceOption->szServerName,_T("StorageOff"),0,m_szFileName);
			//����ۼ���������0
			if (m_lStorageOff != 0)
			{
				m_lStockScore -= m_lStorageOff;
				TCHAR szStorageMin[1024]=TEXT("");
				_sntprintf(szStorageMin,CountArray(szStorageMin),TEXT("��Ϊ����������%I64d��ʣ���棺%0.2f"),m_lStorageOff, m_lStockScore);
				//�����Ϣ
				CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
				//������֮��Ѹ�ֵ���0
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageOff"),TEXT("0"),m_szFileName);

				//���Ϳ��ƺŴ�����ʾ
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //��ȡ�û��ʺ�
					if(pUserItem==NULL) continue; //����û�������
					if(pUserItem->IsAndroidUser()) continue; //����ǻ�����
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue; //�ж��û�����Ȩ��
					m_pITableFrame->SendGameMessage(pUserItem,szStorageMin,SMT_EJECT);  //������Ϣ����

				}
			}

			//������ݴ������

			/////////////////////////////////////////////////
		




			}
			else    //��Ϸ��û�п�ʼ
			{
				//�������
				WORD wUserCount=0;
				for (WORD i=0;i<m_wPlayerCount;i++)if(m_cbPlayStatus[i]==TRUE) wUserCount++;

				//������Ϸ
				if(wUserCount==1)
				{
					//�������
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));

					//������Ϣ
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE) continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					//������Ϸ

					m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);		
				}
				else if(m_wCurrentUser==wChairID) OnUserCallBanker(wChairID,0);
			}

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
	case GAME_STATUS_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//��������
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_TK_CALL:	//��ׯ״̬
		{
			//��������
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			StatusCall.lCellScore=m_pGameServiceOption->lCellScore;
			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusCall.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusCall.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ñ���
			StatusCall.wCallBanker=m_wCurrentUser;
			StatusCall.cbDynamicJoin=m_cbDynamicJoin[wChiarID];
			CopyMemory(StatusCall.cbPlayStatus,m_cbPlayStatus,sizeof(StatusCall.cbPlayStatus));

			//��������
			CopyMemory(StatusCall.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusCall.szGameRoomName));


			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));
		}
	case GS_TK_SCORE:	//��ע״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			memset(&StatusScore,0,sizeof(StatusScore));

			StatusScore.lCellScore=m_pGameServiceOption->lCellScore;
			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusScore.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusScore.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//��ע��Ϣ
			StatusScore.lTurnMaxScore=m_lTurnMaxScore[wChiarID];
			StatusScore.wBankerUser=m_wBankerUser;
			StatusScore.cbDynamicJoin=m_cbDynamicJoin[wChiarID];
			CopyMemory(StatusScore.cbPlayStatus,m_cbPlayStatus,sizeof(StatusScore.cbPlayStatus));

			//���û���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;
				StatusScore.lTableScore[i]=m_lTableScore[i];
			}

				//��������
			CopyMemory(StatusScore.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusScore.szGameRoomName));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_TK_PLAYING:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//������Ϣ
			StatusPlay.lTurnMaxScore=m_lTurnMaxScore[wChiarID];
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.cbDynamicJoin=m_cbDynamicJoin[wChiarID];
			CopyMemory(StatusPlay.bOxCard,m_bOxCard,sizeof(StatusPlay.bOxCard));
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(StatusPlay.cbPlayStatus));

			//��������
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));


			//�����˿�
			for (WORD i=0;i< m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;
				WORD j= i;
				StatusPlay.lTableScore[j]=m_lTableScore[j];
				CopyMemory(StatusPlay.cbHandCardData[j],m_cbHandCardData[j],MAX_COUNT);
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}
	//Ч�����
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{
	switch(dwTimerID)
	{
	case IDI_SO_OPERATE:
		{
			//ɾ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

			//��Ϸ״̬
			switch( m_pITableFrame->GetGameStatus())
			{
			case GS_TK_CALL:			//�û���ׯ
				{
					OnUserCallBanker(m_wCurrentUser, 0);
					break;
				}
			case GS_TK_SCORE:			//��ע����
				{
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_lTableScore[i]>COMPARE_ZERO || m_cbPlayStatus[i]==FALSE || i==m_wBankerUser)continue;
						OnUserAddScore(i,m_lTurnMaxScore[i]/8);
					}

					break;
				}
			case GS_TK_PLAYING:			//�û�����
				{
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_bOxCard[i]<2 || m_cbPlayStatus[i]==FALSE)continue;
						OnUserOpenCard(i, 0);
					}

					break;
				}
			default:
				{
					break;
				}
			}

			if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
				m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ���� 
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	bool bResult=false;
	switch (wSubCmdID)
	{
	case SUB_C_CALL_BANKER:			//�û���ׯ
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CallBanker));
			if (wDataSize!=sizeof(CMD_C_CallBanker)) return false;

			//��������
			CMD_C_CallBanker * pCallBanker=(CMD_C_CallBanker *)pDataBuffer;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;


			//��Ϣ����
			bResult=OnUserCallBanker(pUserData->wChairID,pCallBanker->bBanker);
			break;
		}
	case SUB_C_ADD_SCORE:			//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pDataBuffer;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			bResult=OnUserAddScore(pUserData->wChairID,pAddScore->lScore);
			break;
		}
	case SUB_C_OPEN_CARD:			//�û�̯��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_OxCard));
			if (wDataSize!=sizeof(CMD_C_OxCard)) return false;

			//��������
			CMD_C_OxCard * pOxCard=(CMD_C_OxCard *)pDataBuffer;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			bResult=OnUserOpenCard(pUserData->wChairID,pOxCard->bOX);
			break;
		}
	case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;
			//if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
			return OnSubAmdinCommand(pIServerUserItem,pDataBuffer);
		}
	}

	//������ʱ��
	if(bResult)
	{
		m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
		return true;
	}

	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}


//��ׯ�¼�
bool CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE bBanker)
{
	//״̬Ч��
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_CALL) return true;
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return true;

	//���ñ���
	m_bCallStatus[wChairID]=TRUE;

	//��ׯ����
	WORD wCallUserCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==TRUE && m_bCallStatus[i]==TRUE) wCallUserCount++;
		else if(m_cbPlayStatus[i]!=TRUE) wCallUserCount++;
	}

	//��ע��ʼ
	if(bBanker==TRUE || wCallUserCount==m_wPlayerCount)
	{
		//ʼ���û�
		m_wBankerUser=wChairID;
		m_wCurrentUser=INVALID_CHAIR;

		//�������һ����ׯ�û�ǿ�����
		while(m_cbPlayStatus[m_wBankerUser]==FALSE)
		{
			m_wBankerUser=(m_wBankerUser+1)%GAME_PLAYER;
		}

		//����״̬
		m_pITableFrame->SetGameStatus(GS_TK_SCORE);

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wBankerUser);
		SCORE lBankerScore=pIServerUserItem->GetUserScore();

		//�������
		WORD wUserCount=0;
		for (WORD i=0;i<m_wPlayerCount;i++)if(m_cbPlayStatus[i]==TRUE )wUserCount++;

		//�����ע
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]!=TRUE || i==m_wBankerUser)continue;

			//��ȡ�û�
			pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

			//��ȡ����
			SCORE lScore=pIServerUserItem->GetUserScore();

			//��Ҽ���
			ASSERT(lScore>=m_pGameServiceOption->lCellScore);

			//��ע���� �ͻ�Ҫ��
			m_lTurnMaxScore[i]=__min(lBankerScore/(wUserCount-1)/5,lScore/5);
		}

		//���ñ���
		//m_wOperaCount=0;
		CMD_S_GameStart GameStart;
		GameStart.wBankerUser=m_wBankerUser;
		GameStart.lTurnMaxScore=0;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			GameStart.lTurnMaxScore=m_lTurnMaxScore[i];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	else		 //�û���ׯ
	{
		//�����¸����
		do{
			m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		}while(m_cbPlayStatus[m_wCurrentUser]==FALSE);
		
			
		

		//���ñ���
		CMD_S_CallBanker CallBanker;
		CallBanker.wCallBanker=m_wCurrentUser;
		CallBanker.bFirstTimes=false;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}

	//��Ϊȷ��ׯ��֮���ٿ�����Ӯ

	WORD wChairIDCtrl = INVALID_CHAIR;

	if(m_dwCheatCount>0)  //���ƴ���
	{
		m_bWinnerCtrl = true;  //������ƴ�������0��ָ�������Ӯ���ƿ���

		if(m_dwCheatGameID==0)  //������ID����0 ָ�������Ӯ���ƹر�
		{
			m_bWinnerCtrl=false;  
		}
		else   //���ID����0
		{
			//����ID
			wChairIDCtrl = GetChairIdByGameId(m_dwCheatGameID);  //��ȡ������Ӻ�

			//ָ����Ҳ���
			if(wChairIDCtrl==INVALID_CHAIR)
			{
				m_bWinnerCtrl=false;
			}
		}
	}else{
		//���ƴ�������0
		m_bWinnerCtrl=false;
	}




	//�Ƿ�������
	if (m_bWinnerCtrl)  //ָ���������ȵ�һ
	{
		OutputDebugStringA("hhh 111");
		//m_cbCheatTypeʤ������ 1Ϊʤ��0Ϊ��
		ProbeJudge(wChairIDCtrl,m_cbCheatType);

	}
	else if (m_bAndroidWinningCtrl)//������Ӯ�ʿ��Ƶڶ�
	{
		OutputDebugStringA("hhh 222");
		//m_bCtrlThisTime = true;
		AndroidWinningControl();  //�����˻��ƻ���


	}
	else  //���������ȼ����ŵ��� Ҳ�����
	{
		OutputDebugStringA("hhh 333");
		//m_bCtrlThisTime = false;

		//����������
		AndroidCheck();

	}




	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, SCORE lScore)
{
	//״̬Ч��
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_SCORE);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_SCORE) return true;

	SCORE lAddScore = lScore;
	//���Ч��
	if(m_cbPlayStatus[wChairID]==TRUE)
	{
		ASSERT(lAddScore>COMPARE_ZERO && lAddScore<=m_lTurnMaxScore[wChairID]);
		if (lAddScore<COMPARE_ZERO)
			lAddScore=m_pGameServiceOption->lCellScore;
		if(lAddScore-m_lTurnMaxScore[wChairID]>0.001)
			lAddScore=m_lTurnMaxScore[wChairID];
	}
	else //û��ע���ǿ��
	{
		ASSERT(lAddScore<COMPARE_ZERO&&lAddScore>-COMPARE_ZERO);
		if (lAddScore>COMPARE_ZERO||lAddScore<-COMPARE_ZERO) return false;
	}

	if(lAddScore>COMPARE_ZERO)
	{
		//��ע���
		m_lTableScore[wChairID]=lAddScore;

		//��������
		CMD_S_AddScore AddScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.lAddScoreCount=m_lTableScore[wChairID];

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		
		//��Ϸ��¼
		m_GameRecord.AppendFormat(TEXT("%s,��,%0.2f#"),m_szNickName[wChairID],lAddScore);
	}

	//��ע����
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_lTableScore[i]>COMPARE_ZERO && m_cbPlayStatus[i]==TRUE)bUserCount++;
		else if(m_cbPlayStatus[i]==FALSE || i==m_wBankerUser)bUserCount++;
	}

	//�м�ȫ��
	if(bUserCount==m_wPlayerCount)
	{
		//����״̬
		m_pITableFrame->SetGameStatus(GS_TK_PLAYING);

		//��������
		CMD_S_SendCard SendCard;
		ZeroMemory(SendCard.cbCardData,sizeof(SendCard.cbCardData));

		//�����˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE)continue;
			
			// ��Ϸ��¼
			m_GameRecord.AppendFormat( TEXT("%s-Card:"),m_szNickName[i] );

			//�ɷ��˿�
			CopyMemory(SendCard.cbCardData[i],m_cbHandCardData[i],MAX_COUNT);

			for(BYTE j = 0;j<CountArray(m_cbHandCardData[i]);j++)
			{
				m_GameRecord.AppendFormat( TEXT("%s,"),	TransformCardInfo(m_cbHandCardData[i][j]) );
			}
			m_GameRecord+=TEXT("#");
		}
		

		//��������
		for (WORD i=0;i< m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	}

	return true;
}



//̯���¼�
bool CTableFrameSink::OnUserOpenCard(WORD wChairID, BYTE bOx)
{
	//״̬Ч��
	ASSERT (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING) return true;
	if (m_bOxCard[wChairID]!=0xff) return true;

	//Ч������
	ASSERT(bOx<=TRUE);
	if(bOx>TRUE)return false;

	//Ч������
	if(bOx)
	{
		ASSERT(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0);
		if(!(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0))return false;
	}
	else if(m_cbPlayStatus[wChairID]==TRUE)
	{
		if(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>=OX_THREE_SAME)bOx=TRUE;
	}

	//ţţ����
	m_bOxCard[wChairID] = bOx;

	//̯������
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bOxCard[i]<2 && m_cbPlayStatus[i]==TRUE)bUserCount++;
		else if(m_cbPlayStatus[i]==FALSE)bUserCount++;
	}

	 //�������
	CMD_S_Open_Card OpenCard;
	ZeroMemory(&OpenCard,sizeof(OpenCard));

	//���ñ���
	OpenCard.bOpen=bOx;
	OpenCard.wPlayerID=wChairID;

	//��������
	for (WORD i=0;i< m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
		
		m_pITableFrame->SendTableData(i,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));	

	//������Ϸ
	if(bUserCount == m_wPlayerCount)
	{
		return OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
	}

	return true;
}




//��ת����
CString CTableFrameSink::TransformCardInfo( BYTE cbCardData ) 
{
	CString str = TEXT("");

	BYTE cbCardValue=cbCardData&LOGIC_MASK_VALUE;
	switch(cbCardValue)
	{
		case 0x01:
		{
			str += TEXT("A");
			break;
		}
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
			{
				str.Format( TEXT("%d"),cbCardValue);
				break;
			}
		case 0x0A:
			{
				str += TEXT("10");
				break;
			}
		case 0x0B:
			{
				str += TEXT("J");
				break;
			}
		case 0x0C:
			{
				str += TEXT("Q");
				break;
			}
		case 0x0D:
			{
				str += TEXT("K");
				break;
			}
		default:
			ASSERT(FALSE);
	}
	
	BYTE cbCardColor = (cbCardData&LOGIC_MASK_COLOR);
	switch( cbCardColor )
	{
	case 0x00:
		str += TEXT("����");
		break;
	case 0x10:
		str += TEXT("÷��");
		break;
	case 0x20:
		str += TEXT("����");
		break;
	case 0x30:
		str += TEXT("����");
		break;
	default:
		ASSERT(FALSE);
	}

	return str;
}

//�����û�Id��ȡ������������Id
WORD CTableFrameSink::GetChairIdByGameId(DWORD dwGameId)
{
	WORD wChairID;
	for (wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue; //�û�������

		if( pIServerUserItem->GetUserID() == dwGameId)
		{
			return wChairID;
		}
	}

	return INVALID_CHAIR;
}



//����ָ�������Ӯ
bool CTableFrameSink::ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType)
{	
	//��ȡ�û�
	IServerUserItem * pIServerCtrlUserItem=m_pITableFrame->GetTableUserItem(wChairIDCtrl);
	if(pIServerCtrlUserItem==NULL) return false; //�û�������, ����Ҫ���Ʒ���
	if(pIServerCtrlUserItem->GetUserID()!=m_dwCheatGameID ) return false; //����ID��һ�·���

		//���ƿ��ƿ��ƿ��ƿ��ƿ��ƿ���
		//�γ��������
		//��������
		//////////////////////////////////////////////////////////////////////////
		BOOL bOxCard[GAME_PLAYER]={0};
		BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT]={0};
		//�����˿�����
		CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));
		for(int i=0; i<m_wPlayerCount; i++)
		{
			//�ж���û����ţţ
			bOxCard[i]=FALSE;
			if(m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>=0) bOxCard[i]=TRUE;
		}

		//�����˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
		}



		//�����ֵ�������
		int iMaxIndex=0;
		int iLostIndex=0;

		for(int i=0; i<GAME_PLAYER;i++)
		{
			//�û�����
			if (m_cbPlayStatus[i]==FALSE) continue;  //����û���ǰ������Ϸ��̬��������FOR
			//��ȡ����������
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iMaxIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iMaxIndex],bOxCard[i]);
			if(!cbCompareResult) iMaxIndex=i;
		}

			//�Ա����
		for (int i=0;i<GAME_PLAYER;i++)
		{
			//�û�����
			if (m_cbPlayStatus[i]==FALSE) continue; //����û���ǰ������Ϸ��̬��������FOR
			//�Ա��˿�
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iLostIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iLostIndex],bOxCard[i]);
			if(cbCompareResult) iLostIndex=i;
		}

			//////////////////////////////////////////
			
			WORD wWinerUser=iMaxIndex;  //���������
			if(cbCheatType==CHEAT_TYPE_LOST) wWinerUser=iLostIndex; //��С������

				//����
			if (wWinerUser!=wChairIDCtrl)    //�����ǰ�û����ǿ��ƺ�,�򽻻��˿����� ������Ҫ����
			{
					
					BYTE cbTemp[MAX_COUNT]={0};
					CopyMemory(cbTemp,m_cbHandCardData[wWinerUser],MAX_COUNT);   //����ǰ���������ƴ�ŵ���ʱ����
					CopyMemory(m_cbHandCardData[wWinerUser],m_cbHandCardData[wChairIDCtrl],MAX_COUNT);  //�����ƺŵ����뵱ǰ���ƽ���
					CopyMemory(m_cbHandCardData[wChairIDCtrl],cbTemp,MAX_COUNT);  //����ʱ���˿���������ƺŽ���
			}


		    ////////////////////////////
			////������Ϣ��ʼ��
			if(m_dwCheatCount>0) m_dwCheatCount--;
			if(m_dwCheatCount==0) {m_dwCheatGameID=0;}
			m_bWinnerCtrl = false;
			///////////////////////////

			//�γ��������
			//д�������ļ�
			CString TempValue;
			
			//д���������
			TempValue.Format(TEXT("%d"),m_dwCheatCount);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatCount"),TempValue,m_szFileName);
		
			//�γ��������
	
			return true;
}



//////��������Ӯ���ʣ�����ָ����ҿ��ƣ��Ϳ�滥��
bool CTableFrameSink::AndroidWinningControl()
{
	//�����˸���
	int iAndroidCount=0;
	//���˸���
	int iPlayerCount=0;

	//�����˻���Ŀ������
	int iPostionAndroid[GAME_PLAYER]={0};
	//���˻���Ŀ������
	int iPostionPlayer[GAME_PLAYER]={0};
	
	//��ʼ��ָ��
	IServerUserItem *pIServerUserItem=NULL;

	//��ȡ���������˸�������Ҹ���
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			if (pIServerUserItem->IsAndroidUser())
			{
				//��¼����
				iPostionAndroid[iAndroidCount]=i;
				iAndroidCount++;
			}
			else
			{
				//��¼����
				iPostionPlayer[iPlayerCount]=i;
				iPlayerCount++;
			}
		}
	}

	//û�л����˷���
	if(iAndroidCount==0) return false;
	//û�����˷���
	if (iPlayerCount==0) return false;

	//////////////////////////////////
//��������
		//////////////////////////////////////////////////////////////////////////
		BOOL bOxCard[GAME_PLAYER]={0};
		BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT]={0};
		//�����˿�����
		CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));
		for(int i=0; i<m_wPlayerCount; i++)
		{
			//�ж���û����ţţ
			bOxCard[i]=FALSE;
			if(m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>=0) bOxCard[i]=TRUE;
		}

		//�����˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
		}



		//�����ֵ�������
		int iMaxIndex=0;
		int iLostIndex=0;

		for(int i=0; i<GAME_PLAYER;i++)
		{
			//�û�����
			if (m_cbPlayStatus[i]==FALSE) continue;  //����û���ǰ������Ϸ��̬��������FOR
			//��ȡ����������
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iMaxIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iMaxIndex],bOxCard[i]);
			if(!cbCompareResult) iMaxIndex=i;
		}

			//�Ա����
		for (int i=0;i<GAME_PLAYER;i++)
		{
			//�û�����
			if (m_cbPlayStatus[i]==FALSE) continue; //����û���ǰ������Ϸ��̬��������FOR
			//�Ա��˿�
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iLostIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iLostIndex],bOxCard[i]);
			if(cbCompareResult) iLostIndex=i;
		}

		bool bAndroidBanker = false;
		IServerUserItem *pBankerUser = m_pITableFrame->GetTableUserItem(m_wBankerUser);
		if(pBankerUser == NULL) return false;
		bAndroidBanker = pBankerUser->IsAndroidUser();


	//���Ʊ���
	int iTarget=0;//Ŀ������
	bool bChange = false;//�Ƿ�Ҫ����

	//�������
	srand( (unsigned)time(NULL));
	//��ȡ�����
	DOUBLE fAndroidWinPer = (float)(m_lStockScore/m_lAndroidWinAndLose) ;
	//��ȡ�����
	WORD Ratio =fAndroidWinPer*100; //����0~100����


	CString str;
	str.Format(L"nnn Ratio��%d",Ratio);
	OutputDebugString(str);

	//iMaxindexitem��ȡ
	pIServerUserItem=m_pITableFrame->GetTableUserItem(iMaxIndex);  //��ȡ����Ƶ��û�
	if(pIServerUserItem == NULL) return false;   //����û�����������
	bool ReturnValue=(m_AndroidWinningRatio>Ratio)?true:false; //������ñ����ɵ�����
	//������Ӯ
	if(ReturnValue)  //������ñ����ɵ�����
	{
//		if(pIServerUserItem->IsAndroidUser()==FALSE)//���������������
		{
			if(bAndroidBanker)
			{
				iTarget=m_wBankerUser;//iPostionAndroid[rand()%iAndroidCount];  //���������Ŀ�껻��
				bChange = true;									//���û��Ʊ���ΪTRUE
			}
			else// if(iMaxIndex == m_wBankerUser)
			{
				//��ʱ����
				BYTE cbTemp[MAX_COUNT]={0};

				CopyMemory(cbTemp,m_cbHandCardData[m_wBankerUser],MAX_COUNT);  //����Ƶ�����˿�
				CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[iLostIndex],MAX_COUNT);  //Ŀ���˿��û�
				CopyMemory(m_cbHandCardData[iLostIndex],cbTemp,MAX_COUNT);    //Ŀ���������û��˿�

			}
		}
// 		else
// 		{
// 			if(bAndroidBanker)
// 			{
// 
// 			}
// 				//��ʱ����
// 			BYTE cbTemp[MAX_COUNT]={0};
// 
// 			CopyMemory(cbTemp,m_cbHandCardData[m_wBankerUser],MAX_COUNT);  //����Ƶ�����˿�
// 			CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[iLostIndex],MAX_COUNT);  //Ŀ���˿��û�
// 			CopyMemory(m_cbHandCardData[iLostIndex],cbTemp,MAX_COUNT);    //Ŀ���������û��˿�
// 
// 		}
	}
	//���Ӯ
	else
	{
		if(pIServerUserItem->IsAndroidUser()==TRUE)//������ڻ���������
		{
			iTarget=iPostionPlayer[rand()%iPlayerCount];
			bChange = true;
		}
	}

	//����
	if (bChange)
	{
		//��ʱ����
		BYTE cbTemp[MAX_COUNT]={0};
		//������
		CopyMemory(cbTemp,m_cbHandCardData[iMaxIndex],MAX_COUNT);  //����Ƶ�����˿�
		CopyMemory(m_cbHandCardData[iMaxIndex],m_cbHandCardData[iTarget],MAX_COUNT);  //Ŀ���˿��û�
		CopyMemory(m_cbHandCardData[iTarget],cbTemp,MAX_COUNT);    //Ŀ���������û��˿�

	}

	return true;
}





//������ �γ���������
bool CTableFrameSink::AndroidCheck()
{
	//�����˸���
	int iAndroidCount=0;
	//���˸���
	int iPlayerCount=0;

	//�����˻���Ŀ������
	int iPostionAndroid[GAME_PLAYER]={0};
	//���˻���Ŀ������
	int iPostionPlayer[GAME_PLAYER]={0};

	//��ʼ��ָ��
	IServerUserItem *pIServerUserItem=NULL;

	//��ȡ���������˸�������Ҹ���
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			if (pIServerUserItem->IsAndroidUser())
			{
				//��¼����
				iPostionAndroid[iAndroidCount]=i;
				iAndroidCount++;
			}
			else
			{
				//��¼����
				iPostionPlayer[iPlayerCount]=i;
				iPlayerCount++;
			}
		}
	}
	OutputDebugStringA("hhh 56");
	//û�л����˷���
	if(iAndroidCount==0) return false;
	//û�����˷���
	if (iPlayerCount==0) return false;

		OutputDebugStringA("hhh 61");
	bool bAndroidBanker = false;
	IServerUserItem *pBankerUser = m_pITableFrame->GetTableUserItem(m_wBankerUser);
	if(pBankerUser == NULL) return false;
	bAndroidBanker = pBankerUser->IsAndroidUser();
		//////////////////////////////////
		OutputDebugStringA("hhh 62");
	//��������
		//////////////////////////////////////////////////////////////////////////
		BOOL bOxCard[GAME_PLAYER]={0};
		BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT]={0};
		//�����˿�����
		CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));
		for(int i=0; i<m_wPlayerCount; i++)
		{
			//�ж���û����ţţ
			bOxCard[i]=FALSE;
			if(m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>=0) bOxCard[i]=TRUE;
		}

		//�����˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
		}

	OutputDebugStringA("hhh 63");

		//�����ֵ�������
		int iMaxIndex=0;
		int iLostIndex=0;

		for(int i=0; i<GAME_PLAYER;i++)
		{
			//�û�����
			if (m_cbPlayStatus[i]==FALSE) continue;  //����û���ǰ������Ϸ��̬��������FOR
			//��ȡ����������
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iMaxIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iMaxIndex],bOxCard[i]);
			if(!cbCompareResult) iMaxIndex=i;
		}

			//�Ա����
		for (int i=0;i<GAME_PLAYER;i++)
		{
			//�û�����
			if (m_cbPlayStatus[i]==FALSE) continue; //����û���ǰ������Ϸ��̬��������FOR
			//�Ա��˿�
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iLostIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iLostIndex],bOxCard[i]);
			if(cbCompareResult) iLostIndex=i;
		}



	//���Ʊ���
	int iTarget=0;//Ŀ������
	bool bChange = false;//�Ƿ�Ҫ����

	//�������
	srand( (unsigned)time(NULL));
	DOUBLE fAndroidWinPer = (float)(m_lStockScore/m_lAndroidWinAndLose) ;
	//��ȡ�����
	WORD Ratio =fAndroidWinPer*100; //����0~100����


	//iMaxindexitem��ȡ
	pIServerUserItem=m_pITableFrame->GetTableUserItem(iMaxIndex);  //��ȡ����Ƶ��û�
	if(pIServerUserItem == NULL) return false;   //����û�����������
	

	//�γ��������
	WORD WinRatio=100;
	if(StorageStyle==1) WinRatio = StorageRatio;            //���嵱ǰ��Ӯ���� //Ĭ�ϱ���
	if(StorageStyle==2) WinRatio = StorageMinRatio;         //���嵱ǰ��Ӯ���� //ɱ��
	if(StorageStyle==3) WinRatio = StorageMaxRatio;         //���嵱ǰ��Ӯ���� //�ͷ�
	//������ɱ�ֻ����ͷ�
	//�Ե����
	if(m_lStockScore>=m_lStorageMax && StorageStyle!=3)
	{
		
			SCORE tmp_lStockScore = m_lStockScore;							 //ԭ�����
			SCORE m_StorageEating=m_lStockScore*(StorageEating/100.00);		 //ϵͳ�Է�
			
			//��ȥ�Ե��ķ�
			m_lStockScore-=m_StorageEating;
			WinRatio=StorageMaxRatio;  //�ͷ�ģʽ
						//�ͷ�ģʽ
			StorageStyle=3;

	}
	
	//���������Сֵ,ɱ��ģʽ
	if(m_lStockScore<=m_lStorageMin /*&& StorageStyle!=2*/)
	{
		WinRatio=StorageMinRatio;
		StorageStyle=2;//ɱ��ģʽ
	}
	
				 //�����ǰģʽ
				CString strLog;
				strLog.Format(TEXT("���ţ�%d,��Ϸģʽ��%d,��ǰ��Ӯ���ʣ�%d (��Ϸģʽ1,����,2,ɱ��,3,�ͷ�)"),m_pITableFrame->GetTableID()+1,StorageStyle,WinRatio);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ



				CString str;
				str.Format(L"hhh ���ֵRatio��%d ϵͳ�Ŀ��m_lStockScoreΪ��%lf ������Ӯ�ĸ���ΪWinRatio:%d",Ratio,m_lStockScore,WinRatio);
				OutputDebugString(str);

	bool ReturnValue=(WinRatio>Ratio)?true:false; //�������
	//�γ��������

	CString str1;
	str1.Format(L"hhh ���������ʤ��ΪReturnValue��%d",ReturnValue);
	OutputDebugString(str1);

	//������Ӯ
	if(ReturnValue)  //������ñ����ɵ�����
	{
//		if(pIServerUserItem->IsAndroidUser()==FALSE)//���������������
		{
			if(bAndroidBanker)
			{
				iTarget=m_wBankerUser;//iPostionAndroid[rand()%iAndroidCount];  //���������Ŀ�껻��
				bChange = true;									//���û��Ʊ���ΪTRUE
			}
			else// if(iMaxIndex == m_wBankerUser)
			{
				//��ʱ����
				BYTE cbTemp[MAX_COUNT]={0};

				CopyMemory(cbTemp,m_cbHandCardData[m_wBankerUser],MAX_COUNT);  //����Ƶ�����˿�
				CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[iLostIndex],MAX_COUNT);  //Ŀ���˿��û�
				CopyMemory(m_cbHandCardData[iLostIndex],cbTemp,MAX_COUNT);    //Ŀ���������û��˿�

			}
		}
// 		else
// 		{
// 				//��ʱ����
// 				BYTE cbTemp[MAX_COUNT]={0};
// 
// 				CopyMemory(cbTemp,m_cbHandCardData[m_wBankerUser],MAX_COUNT);  //����Ƶ�����˿�
// 				CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[iLostIndex],MAX_COUNT);  //Ŀ���˿��û�
// 				CopyMemory(m_cbHandCardData[iLostIndex],cbTemp,MAX_COUNT);    //Ŀ���������û��˿�
// 
// 		}
	}
	//���Ӯ
	else
	{
		if(pIServerUserItem->IsAndroidUser()==TRUE)//������ڻ���������
		{
			iTarget=iPostionPlayer[rand()%iPlayerCount];
			bChange = true;
		}
	}

	//����
	if (bChange)
	{
		//��ʱ����
		BYTE cbTemp[MAX_COUNT]={0};
		//������
		CopyMemory(cbTemp,m_cbHandCardData[iMaxIndex],MAX_COUNT);  //����Ƶ�����˿�
		CopyMemory(m_cbHandCardData[iMaxIndex],m_cbHandCardData[iTarget],MAX_COUNT);  //Ŀ���˿��û�
		CopyMemory(m_cbHandCardData[iTarget],cbTemp,MAX_COUNT);    //Ŀ���������û��˿�
	}



return true;


}



bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{
	//��������й���ԱȨ�� �򷵻ش���
	//if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
	const CMD_C_AdminReq* AdminReq=static_cast<const CMD_C_AdminReq*>(pDataBuffer);
	switch(AdminReq->cbReqType)
	{
	case RQ_OPTION_CANCLE:	//ȡ��
		{
			if (m_dwCheatGameID!=0)
			{
				//������ʾ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("�Ѿ��ɹ���GameIDΪ:%d������Ȩ��ȡ��"),m_dwCheatGameID);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
				m_dwCheatGameID=0;
			}
			else
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("��ǰû�����������û�!"),SMT_CHAT);
			}
			break;
		}
	case RQ_OPTION_QUERYING:			//��ѯ
		{
			if (m_dwCheatGameID!=0)
			{
				TCHAR szType[][32]={TEXT("��"),TEXT("Ӯ")};

				if(m_cbCheatType!=0 && m_cbCheatType!=1) break;

				//������ʾ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��ǰ����GameID��%d��ʣ����ƴ�����%d�֣��������ͣ�%s\n"),
					m_dwCheatGameID,m_dwCheatCount,szType[m_cbCheatType]);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
			}
			else
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("��ǰû�����������û�!"),SMT_CHAT);
			}
			break;
		}
	case RQ_OPTION_SETING:		//����
		{
			m_dwCheatGameID=AdminReq->dwGameID;
			m_dwCheatCount=AdminReq->cbCheatCount;
			m_cbCheatType=AdminReq->cbCheatType;




			//�γ��������
			//д�������ļ�
			CString TempValue;
			//д���������
			TempValue.Format(TEXT("%d"),m_cbCheatType);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatType"),TempValue,m_szFileName);
			//д���������
			TempValue.Format(TEXT("%d"),m_dwCheatCount);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatCount"),TempValue,m_szFileName);
			//д������û�ID
			TempValue.Format(TEXT("%d"),m_dwCheatGameID);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("WinnerUserID"),TempValue,m_szFileName);
			//�γ��������


			TCHAR szType[][32]={TEXT("��"),TEXT("Ӯ")};

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("�ɹ�ΪGameID:%d��������Ȩ��,���ƣ�%d�Σ����ͣ�%s"),m_dwCheatGameID,m_dwCheatCount,szType[m_cbCheatType]);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
			break;
		}
	}
	return true;
}






//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//��ȡ����
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	TCHAR OutBuf[255];
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);
	//��ȡ����
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\OxServer.ini"),szPath);
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);

	//�Ƿ��ȡÿ��ˢ�����ñ���
	if (bReadFresh)
	{
		//ÿ��ˢ��
		BYTE cbRefreshCfg = GetPrivateProfileInt(szServerName, TEXT("Refresh"), 0, szFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;

		//�Ƿ�ˢ�¿��
		BYTE cbRefreshStorageCfg = GetPrivateProfileInt(szServerName, TEXT("RefreshStorage"), 0, szFileName);
		m_bRefreshStorageCfg = cbRefreshStorageCfg?true:false;

	}

	//�������
	if(m_bRefreshStorageCfg)
	{
		//���ÿ��
		
		memset(OutBuf,0,255);
		GetPrivateProfileString(szServerName,TEXT("StorageStart"),TEXT("0.0"),OutBuf,255,szFileName);
			
		CString TempValue;
		myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
		m_lStockScore=Mywtof(TempValue);
	}
	else
	{
		if(m_lStockScore <= 0.00001 && m_lStockScore >= -0.00001)
		{
			memset(OutBuf,0,255);
			GetPrivateProfileString(szServerName,TEXT("StorageStart"),TEXT("0.0"),OutBuf,255,szFileName);

			CString TempValue;
			myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
			m_lStockScore=Mywtof(TempValue);

		}
	}


	//����˥��ֵ
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageDeduct"),TEXT("1"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageDeduct);

// 	memset(OutBuf,0,255);
// 	GetPrivateProfileString(szServerName,TEXT("StorageMin"),TEXT("10"),OutBuf,255,szFileName);
// 	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageMin);
// 	if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
// 
// 	memset(OutBuf,0,255);
// 	GetPrivateProfileString(szServerName,TEXT("StorageMax"),TEXT("100"),OutBuf,255,szFileName);
// 	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageMax);
		//�γ��������
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
	m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
//	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageEating"),TEXT("50"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&StorageEating);


	//ϵͳ����Ӯ��
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageLimit"),TEXT("0.0"),OutBuf,255,szFileName);
	
	//////////////////////////////////
	//�γ������ȡС������ֵ
	CString TempValue;
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
	m_lStockLimit=Mywtof(TempValue);
	/////////////////////////////////

	CString str;
	str.Format(L"hhh ��ȡ����ϵͳ��ӮΪ:%lf",m_lStockLimit);
	OutputDebugString(str);

	//�γ��������

	////2012-08-28
	//���������Ӯ
	BYTE cbWinnerCtrl = GetPrivateProfileInt(szServerName, TEXT("WinnerCtrl"), 0, szFileName);
	m_bWinnerCtrl = cbWinnerCtrl?true:false;
	//��Ҫʤ�������UserID
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("WinnerUserID"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%lu"),&m_dwCheatGameID);
	//��ȡʤ������
	m_cbCheatType = GetPrivateProfileInt(szServerName, TEXT("CheatType"), 1, szFileName);
	//��ȡ���ƴ���
	m_dwCheatCount = GetPrivateProfileInt(szServerName, TEXT("CheatCount"), 0, szFileName);
	/////////////////



	//////��ȡ��������Ӯ
	BYTE cbAndroidWinningCtrl = GetPrivateProfileInt(szServerName, TEXT("AndroidWinningCtrl"), 0, szFileName);
	m_bAndroidWinningCtrl = cbAndroidWinningCtrl?true:false;

	//////��ȡ��������Ӯ����
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("AndroidWinningRatio"),TEXT("0"),OutBuf,60,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_AndroidWinningRatio);
	if (m_AndroidWinningRatio<0 || m_AndroidWinningRatio >100)
	{
		m_AndroidWinningRatio = 60;
	}

	CString strLog;
	strLog.Format(TEXT("���ţ�%d,��ʼ��棺%0.2f,������Сֵ:%I64d  %I64d,������Ӯ����:%I64d"),m_pITableFrame->GetTableID()+1,m_lStockScore,m_lStorageMax,m_lStorageMin,m_AndroidWinningRatio);
	CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ


	

}