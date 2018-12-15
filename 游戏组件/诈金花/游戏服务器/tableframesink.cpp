#include "StdAfx.h"
#include "TableFrameSink.h"


//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const BYTE			CTableFrameSink::m_GameStartMode=START_MODE_ALL_READY;	//��ʼģʽ


SCORE				CTableFrameSink::m_lStockScore = 0.0;
LONGLONG			CTableFrameSink::m_lStorageOff = 0L;
BYTE				CTableFrameSink::StorageStyle=1;
//��ʱ�� 0~30
#define IDI_GAME_COMPAREEND					(1)									//������ʱ��
#define IDI_GAME_OPENEND					(2)									//������ʱ��

#define TIME_GAME_COMPAREEND				6000								//������ʱ��
#define TIME_GAME_OPENEND					6000								//������ʱ��

//////////////////////////////////////////////////////////////////////////
void TraceMessage(CString szinfo,WORD id);
//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ��¼
	m_GameRecord.Empty(); //�������
	ZeroMemory(m_szNickName,sizeof(m_szNickName));  //�������
	ZeroMemory(m_lAddScoreTimes,sizeof(m_lAddScoreTimes));  //�������
	//��Ϸ����
	m_bOperaCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_lCompareCount=0L;
	m_bGameEnd=false;  //�Ƿ����
	//������Ϣ
	m_dwCheatCount=0;
	m_dwCheatGameID=0;
	m_cbCheatType=CHEAT_TYPE_WIN;
	m_bWinnerCtrl=false;
	m_bAndroidWinningCtrl = false;
	ZeroMemory(m_wFlashUser,sizeof(m_wFlashUser));

	//�û�״̬
	ZeroMemory(&m_StGameEnd,sizeof(m_StGameEnd));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	for(int i=0;i<m_wPlayerCount;i++)
	{
		m_bMingZhu[i]=false;
		m_wCompardUser[i].RemoveAll();
	}

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	m_lMaxCellScore=0L;
	m_lCellScore=SCORE_ZERO;
	m_lCurrentTimes=0L;		
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));


	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameCustomRule=NULL;
	srand( (unsigned)time(NULL));
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{

	if(!m_GameRecord.IsEmpty()) TraceMessage(m_GameRecord,m_wTableID);

}

//�ӿڲ�ѯ--��������Ϣ�汾
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
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

	//�Զ�����
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

		//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

		//��ע����
	m_lCellScore=Double_Round(m_pGameServiceOption->lCellScore,2,1);

	m_lCellScore_Long = (ceil( m_lCellScore* 100.00) / 100.00)*100;  //��С��ת������100����ʾ������ʱ�ٳ�100

	m_wTableID=m_pITableFrame->GetTableID();


	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//��ȡ����
	//TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\ZaJinHuaConfig.ini"),szPath);
	//��ʼ�洢��Ŀ
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);
	//��ȡ���
	m_lStockScore=GetPrivateProfileInt(szServerName,TEXT("StorageStart"),0,m_szFileName);
	//��ȡ˥��ֵ
	m_lStorageDeduct=GetPrivateProfileInt(szServerName,TEXT("StorageDeduct"),10,m_szFileName);


	//�������˥��ֵ
	if( m_lStorageDeduct < 0)
		m_lStorageDeduct = 0;
	if ( m_lStorageDeduct > 1000)
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
	
	
	//��ȡ����
	ReadConfigInformation(true);


	return true;
}

//��λ����
void  CTableFrameSink::RepositionSink()
{	
	//��Ϸ��¼
	m_GameRecord.Empty();
	ZeroMemory(m_lAddScoreTimes,sizeof(m_lAddScoreTimes));

	//��Ϸ����
	m_bOperaCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	m_lCompareCount=0L;
	ZeroMemory(m_wFlashUser,sizeof(m_wFlashUser));

	//�û�״̬
	ZeroMemory(&m_StGameEnd,sizeof(m_StGameEnd));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	for(int i=0;i<m_wPlayerCount;i++)
	{
		m_bMingZhu[i]=false;	
		m_wCompardUser[i].RemoveAll();
	}

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	m_lMaxCellScore=0L;										
	m_lCurrentTimes=0L;		
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));
	m_bWinnerCtrl=false;
	m_bAndroidWinningCtrl=false;

	return;
}

//��ʼģʽ
BYTE  CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	ASSERT(wChairID<m_wPlayerCount);
	return (m_cbPlayStatus[wChairID]==TRUE)?true:false;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{

	//����״̬
	m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);

	m_bGameEnd=false;  //�Ƿ����
	ZeroMemory(m_szNickName,sizeof(m_szNickName));
	//��ȡ����
	ReadConfigInformation(true);

	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	//�����ע
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ�û�
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUser==NULL) continue;

		SCORE lUserScore=pIServerUser->GetUserScore();
		ASSERT(lUserScore >=m_pGameServiceOption->lCellScore);

		//���ñ���
		m_cbPlayStatus[i]=TRUE;

		m_lUserMaxScore[i]=(ceil(lUserScore * 100.00) / 100.00)*100;

		_sntprintf(m_szNickName[i],CountArray(m_szNickName[i]),pIServerUser->GetNickName());

		m_GameRecord.AppendFormat(TEXT("P%ld,%s,%0.2f#"),i,m_szNickName[i],pIServerUser->GetUserScore());
	}

	//��Ԫ�������� 20��      //�γ�����
	m_lMaxCellScore=m_lCellScore_Long*20;
	
	//��ǰ����
	m_lCurrentTimes=1;

	//�����˿�
	m_GameLogic.RandCardList(m_cbHandCardData[0],sizeof(m_cbHandCardData)/sizeof(m_cbHandCardData[0][0]));

	
	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i],MAX_COUNT);
	}

	//�·����ǿ�����ص�
	////////////////////////////////////////////////////////
	//�ж��Ƿ����ָ�����
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
			if( wChairIDCtrl==INVALID_CHAIR)
			{
				m_bWinnerCtrl=false;
			}
		}
	}else{
		//���ƴ�������0
		m_bWinnerCtrl=false;
	}

	//////////////////////////////////////////����ָ����ҿ���

	//�Ƿ�������
	if (m_bWinnerCtrl)  //ָ���������ȵ�һ
	{
		//m_cbCheatTypeʤ������ 1Ϊʤ��0Ϊ��
		ProbeJudge(wChairIDCtrl,m_cbCheatType);

	}
	else if (m_bAndroidWinningCtrl)//������Ӯ�ʿ��Ƶڶ�
	{
		
		AndroidWinningControl();  //�����˻��ƻ���

	}
	else  //���������ȼ����ŵ��� Ҳ�����
	{
		
		
		//����������
		AndroidCheck();
	}
	//////////////��������

	
	//�û�����
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			m_lTableScore[i]=m_lCellScore_Long;
			m_lAddScoreTimes[i]++;
		}
	}

	// ��Ϸ��¼
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			m_GameRecord.AppendFormat( TEXT("P%ld,Card:"),i);
			for(WORD j=0;j<CountArray(m_cbHandCardData[i]);j++)
				m_GameRecord.AppendFormat( TEXT("%s,"),TransformCardInfo(m_cbHandCardData[i][j]));
			m_GameRecord+=TEXT("#");
		}

	}

	//����ׯ��
	if(m_wBankerUser==INVALID_CHAIR)m_wBankerUser=rand()%m_wPlayerCount;

	//ׯ���뿪
	if(m_wBankerUser<m_wPlayerCount && m_cbPlayStatus[m_wBankerUser]==FALSE)m_wBankerUser=rand()%m_wPlayerCount;

	//ȷ��ׯ��
	while(m_cbPlayStatus[m_wBankerUser]==FALSE)
	{
		m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;
	}

	//��ǰ�û�
	m_wCurrentUser=(m_wBankerUser+1)%m_wPlayerCount;

	while(m_cbPlayStatus[m_wCurrentUser]==FALSE)
	{
		m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	}




	//�·����ͻ�������Ϣ��Ҫ��ӣ��Ƿ������ʤ������ǰ��棬
	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	GameStart.lMaxScore=m_lMaxCellScore;
	GameStart.lCellScore=m_lCellScore_Long;
	GameStart.lCurrentTimes=m_lCurrentTimes;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wMaxTimes = m_pGameCustomRule->wMaxTimes;

	CopyMemory(GameStart.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));
	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			GameStart.lUserMaxScore=m_lUserMaxScore[i];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	//������˷����û���
	AndroidSendUserCard();

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_COMPARECARD:	//���ƽ���
	case GER_NO_PLAYER:		//û�����
		{
			//�γ���������

			if(m_bGameEnd) return true;  //����ǽ�����̬��ֱ�ӷ��ز���ִ��
			
			m_bGameEnd=true;		//���õ�ǰ�ǽ�����̬

			//��Ϸ��¼
			if(cbReason == GER_COMPARECARD)
				m_GameRecord+=TEXT("���ƽ���:#"); //���ƽ���
			else
				m_GameRecord+=TEXT("û�����:#");  //û�����
			
			 //����ͳ��
			m_GameRecord+=TEXT("������עͳ��:#");  //����ͳ��
			for(WORD i = 0;i<GAME_PLAYER;i++)
			{
				if(m_lAddScoreTimes[i]>0L)
				{
					//m_GameRecord.AppendFormat( TEXT("%s,��ע%I64d��,��%0.2f#"),m_szNickName[i],m_lAddScoreTimes[i],m_lTableScore[i]/100.00);
					m_GameRecord.AppendFormat( TEXT("P%d,��ע%I64d��,��%0.2f#"),i,m_lAddScoreTimes[i],m_lTableScore[i]/100.00);
				}
			}

			////��Ϸ��������
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd)); //�����Ϣ
		
			//���ñ����û�Ϊ��Ч
			for (WORD i=0;i<m_wPlayerCount;i++){
				for (WORD j=0;j<4;j++)
				{
					//�����û��������
					GameEnd.wCompareUser[i][j]=INVALID_CHAIR;
				}
			}

			//Ψһ���
			WORD wWinner=0;  //ʤ���û�
			WORD wUserCount=0; //�û�����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{	
				//������Ϸ�û�
				if(m_cbPlayStatus[i]==TRUE)
				{
					//�û�������һ
					wUserCount++;
					wWinner=i;
					if(GER_COMPARECARD==cbReason)ASSERT(m_wBankerUser==i);
					m_wBankerUser=i;
				}
			}

			//ʤ����ǿ��
			if(wUserCount==0 /*&& GER_COMPARECARD==cbReason*/)
			{
				//ʤ���û����ׯ��
				wWinner=m_wBankerUser;
			}

			//������ע
			LONGLONG lWinnerScore=0L;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if(i==wWinner)continue;
				//����û��÷�
				GameEnd.lGameScore[i]=-m_lTableScore[i];
				//ʤ���û��ܵ÷�
				lWinnerScore+=m_lTableScore[i];
			}

			//���ʤ���÷�����100 
			//�·�˰����Ҫ��100����0.01��˰�ս�����ʾ ����0.01��˰��ͳһ��ȡ0.01��˰��
			if(lWinnerScore>100){

				//����˰��
				GameEnd.lGameTax=m_pITableFrame->CalculateRevenue(wWinner,lWinnerScore);
			
			}
			else
			{	//����˰��
				GameEnd.lGameTax=m_pITableFrame->CalculateRevenue(wWinner,100.00);
			}

			//ʤ���û�����˰�յ÷�
			GameEnd.lGameScore[wWinner]=lWinnerScore-GameEnd.lGameTax;

			//�����˿�����
			CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

			//�˿�����
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				WORD wCount=0;
				while(m_wCompardUser[i].GetCount()>0)
				{
					GameEnd.wCompareUser[i][wCount++] = m_wCompardUser[i].GetAt(0);
					m_wCompardUser[i].RemoveAt(0);
				}
			}

			//���ý���
			GameEnd.wEndState=0;
			///////////////////////////////////////////////////////
			////����÷�
			//����û���������1,���������û��÷���˰��
			if(wUserCount==1)
			{
				//�޸Ļ���
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				//�����������
				ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if(m_cbPlayStatus[i]==FALSE) continue;  //����û�������Ϸ��,������ЩFOR ������һ��FOR
					ScoreInfoArray[i].cbType =(GameEnd.lGameScore[i]>0)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;  //����û��÷��Ǵ���O��Ϊʤ�����������
					ScoreInfoArray[i].lRevenue=Double_Round(GameEnd.lGameTax/100.00,2,1);                //����˰��   ��100����Ϊ ���������Ϸ��ȫ�����ǰ����������е�                        
					ScoreInfoArray[i].lScore=Double_Round(GameEnd.lGameScore[i]/100.00,2,1);			 //����÷�   ��100����Ϊ ���������Ϸ��ȫ�����ǰ����������е�
					//��Ϸ��¼
					m_GameRecord.AppendFormat(TEXT("P%ld-:%0.2f,%0.2f#"),i,ScoreInfoArray[i].lScore ,ScoreInfoArray[i].lRevenue); //д����Ϸ��¼ 
				
				}
				//д�ֲ���
				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
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
			for (WORD i=0;i<m_wPlayerCount;i++)
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
						if(GameEnd.lGameScore[i]>0) RevenueScore = Double_Round(GameEnd.lGameTax/100.00,2,1);
						//������˥��
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//ϵͳ���ֵ
						m_lStockScore-=(Double_Round(GameEnd.lGameScore[i]/100.00+RevenueScore,2,1)+lStorageDeduct); 
						//ϵͳ�÷�
						lSystemScore-=GameEnd.lGameScore[i]/100.00;

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
				strLog.Format(TEXT("A���ţ�%d,��ǰ��棺%0.2f,ϵͳ�÷֣�%0.2f,˰�գ�%0.2f,ʣ���棺%0.2f,���˥����%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ

				//�γ��������
				m_lStockLimit+=lSystemScore;

				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
				//��¼����Ӯ��¼
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//������ڿ����û��Ļ�//������Ϸ��Ϣ�����ƺ�
				for(int i=0; i<m_wPlayerCount; i++)
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
				for(int i=0; i<m_wPlayerCount; i++)
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


			if(wChairID==GAME_PLAYER)
			{
				//��������
				CopyMemory(&m_StGameEnd,&GameEnd,sizeof(m_StGameEnd));
				m_pITableFrame->SetGameTimer(IDI_GAME_COMPAREEND,TIME_GAME_COMPAREEND,1,0);
			}
			else
			{
				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));


				TraceMessage(m_GameRecord,m_wTableID);  //�����¼

				//������Ϸ
				m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);

			}

			//��ȡ����
			ReadConfigInformation(true);


			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:
		{
			if(m_bGameEnd)return true;

			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//ǿ�˴���
			if(m_cbPlayStatus[wChairID]==TRUE)
				return OnUserGiveUp(wChairID,true);
			return true;
		}
	case GER_OPENCARD:			//���ƽ��� //�Զ�����ִ�������  
		{
			if(m_bGameEnd)return true;
			m_bGameEnd = true;
			
			m_GameRecord+=TEXT("���ƽ���:#");
			
			 //����ͳ��
			m_GameRecord+=TEXT("������עͳ��:#");  //����ͳ��
			for(WORD i = 0;i<GAME_PLAYER;i++)
			{
				if(m_lAddScoreTimes[i]>0)
				{
					m_GameRecord.AppendFormat( TEXT("P%ld,��ע%I64d��,��%0.2f#"),i,m_lAddScoreTimes[i],m_lTableScore[i]/100.00);
				}
			}
			
			///////////////////////////////////////////////////////////////////
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				for (WORD j=0;j<4;j++)
				{
					GameEnd.wCompareUser[i][j]=INVALID_CHAIR;
				}
			}
			//ʤ�����
			WORD wWinner=m_wBankerUser;

			//�������
			LONGLONG lWinnerScore=0L;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if(i==wWinner)continue;
				lWinnerScore+=m_lTableScore[i];
				GameEnd.lGameScore[i]=-m_lTableScore[i];
			}

			//����˰��
			//GameEnd.lGameTax=m_pITableFrame->CalculateRevenue(wWinner,lWinnerScore);
			//���ʤ���÷�����100 
			//�·�˰����Ҫ��100����0.01��˰�ս�����ʾ ����0.01��˰��ͳһ��ȡ0.01��˰��
			if(lWinnerScore>100){

				//����˰��
				GameEnd.lGameTax=m_pITableFrame->CalculateRevenue(wWinner,lWinnerScore);
			
			}
			else
			{	//����˰��
				GameEnd.lGameTax=m_pITableFrame->CalculateRevenue(wWinner,100);
			}
			//ʤ���û�����˰�յ÷�
			GameEnd.lGameScore[wWinner]=lWinnerScore-GameEnd.lGameTax;

			//���ƽ���
			GameEnd.wEndState=1;
			//�����˿�����
			CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

			//�˿�����
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{				
				WORD wCount=0;
				while(m_wCompardUser[i].GetCount()>0)
				{
					GameEnd.wCompareUser[i][wCount++] = m_wCompardUser[i].GetAt(0);
					m_wCompardUser[i].RemoveAt(0);
				}
			}

			//�޸Ļ���
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{				
				if(i==wWinner || m_cbPlayStatus[i]==FALSE)continue;
				ScoreInfo[i].lScore=Double_Round(GameEnd.lGameScore[i]/100.00,2,1);
				ScoreInfo[i].cbType=SCORE_TYPE_LOSE;
				//��Ϸ��¼
				m_GameRecord.AppendFormat(TEXT("P%ld-:%0.2f,%0.2f#"),i,ScoreInfo[i].lScore, 0.00);
			
			}
			ScoreInfo[wWinner].lScore=Double_Round(GameEnd.lGameScore[wWinner]/100.00,2,1);
			ScoreInfo[wWinner].cbType=SCORE_TYPE_WIN;
			ScoreInfo[wWinner].lRevenue=Double_Round(GameEnd.lGameTax/100.00,2,1);
	
			//��Ϸ��¼
			m_GameRecord.AppendFormat(TEXT("P%ld-:%0.2f,%0.2f#"),wWinner,ScoreInfo[wWinner].lScore, ScoreInfo[wWinner].lRevenue);


			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));



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
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				//����û�������
				if (pIServerUserIte==NULL)continue;					
				//���ڻ����˲�ִ��
				if(bUserAI)
				{
					//����û����ǻ�����
					if(!pIServerUserIte->IsAndroidUser())
					{
						//����˰��
						if(GameEnd.lGameScore[i]>0) RevenueScore = Double_Round(GameEnd.lGameTax/100.00,2,1);
						//������˥��
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//ϵͳ���ֵ
						m_lStockScore-=(Double_Round(GameEnd.lGameScore[i]/100.00+RevenueScore,2,1)+lStorageDeduct); 
						//ϵͳ�÷�
						lSystemScore-=GameEnd.lGameScore[i]/100.00;
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
				strLog.Format(TEXT("B���ţ�%d,��ǰ��棺%0.2f,ϵͳ�÷֣�%0.2f,˰�գ�%0.2f,ʣ���棺%0.2f,���˥����%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ


				//�γ��������
				m_lStockLimit+=lSystemScore;

				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
				//��¼����Ӯ��¼
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//������ڿ����û��Ļ�//������Ϸ��Ϣ�����ƺ�
				for(int i=0; i<m_wPlayerCount; i++)
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
				for(int i=0; i<m_wPlayerCount; i++)
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

			if(wChairID==GAME_PLAYER)
			{
				//��������
				CopyMemory(&m_StGameEnd,&GameEnd,sizeof(m_StGameEnd));
				m_pITableFrame->SetGameTimer(IDI_GAME_OPENEND,TIME_GAME_OPENEND,1,0);
			}
			else
			{
				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				TraceMessage(m_GameRecord,m_wTableID);

				//������Ϸ
				m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);
			}

			ReadConfigInformation(true);
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
			StatusFree.lCellScore= m_lCellScore;
			StatusFree.wMaxTimes = m_pGameCustomRule->wMaxTimes;
				//��������
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));


			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_STATUS_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//��ע��Ϣ
			StatusPlay.lMaxCellScore=m_lMaxCellScore;
			StatusPlay.lCellScore=m_lCellScore_Long;
			StatusPlay.lCurrentTimes=m_lCurrentTimes;
			StatusPlay.lUserMaxScore=m_lUserMaxScore[wChiarID];
			StatusPlay.wMaxTimes = m_pGameCustomRule->wMaxTimes;

			//���ñ���
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			CopyMemory(StatusPlay.bMingZhu,m_bMingZhu,sizeof(m_bMingZhu));
			CopyMemory(StatusPlay.lTableScore,m_lTableScore,sizeof(m_lTableScore));
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));

			//��ǰ״̬
			StatusPlay.bCompareState=(m_lCompareCount<=0)?false:true;

			//�����˿�
			CopyMemory(&StatusPlay.cbHandCardData,&m_cbHandCardData[wChiarID],MAX_COUNT);

				//��������
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	CString strLog;
	strLog.Format(L"\ncbGameStatus = %d",cbGameStatus);
	OutputDebugString(strLog);

	//Ч�����
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼� 
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	//������ʱ
	if(wTimerID==IDI_GAME_COMPAREEND || IDI_GAME_OPENEND==wTimerID)
	{
		//ɾ��ʱ��
		if(wTimerID==IDI_GAME_COMPAREEND)m_pITableFrame->KillGameTimer(IDI_GAME_COMPAREEND);
		else m_pITableFrame->KillGameTimer(IDI_GAME_OPENEND);

		ASSERT(m_pITableFrame->GetGameStatus()==GAME_STATUS_PLAY);

		if(m_pITableFrame->GetGameStatus()==GAME_STATUS_PLAY)
		{
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));

			TraceMessage(m_GameRecord,m_wTableID);

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);
		}
	}

	return false;
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	try{
	switch (wSubCmdID)
	{
	case SUB_C_GIVE_UP:			//�û�����
		{
			//Ч����Ϣ
			ASSERT(wDataSize==0);
			if (wDataSize!=0) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pIServerUserItem->GetChairID()]==TRUE);
			if (m_cbPlayStatus[pIServerUserItem->GetChairID()]==FALSE) return false;

			//��Ϣ����
			return OnUserGiveUp(pIServerUserItem->GetChairID());
		}
	case SUB_C_LOOK_CARD:		//�û�����
	case SUB_C_OPEN_CARD:		//�û�����
		{
			//Ч����Ϣ
			ASSERT(wDataSize==0);
			if (wDataSize!=0) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			WORD wChairID = pIServerUserItem->GetChairID();
			//״̬�ж�
			ASSERT(m_cbPlayStatus[wChairID]==TRUE);
			if (m_cbPlayStatus[wChairID]==FALSE) return false;

			//��Ϣ����
			if(wSubCmdID==SUB_C_LOOK_CARD)return OnUserLookCard(wChairID);
			return OnUserOpenCard(wChairID);
		}
	case SUB_C_COMPARE_CARD:	//�û�����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CompareCard));
			if (wDataSize!=sizeof(CMD_C_CompareCard)) return false;

			//��������
			CMD_C_CompareCard * pCompareCard=(CMD_C_CompareCard *)pData;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			WORD wChairID = pIServerUserItem->GetChairID();

			//����Ч��
			ASSERT(wChairID==m_wCurrentUser);
			if(wChairID!=m_wCurrentUser)return false;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[wChairID]==TRUE && m_cbPlayStatus[pCompareCard->wCompareUser]==TRUE);
			if(m_cbPlayStatus[wChairID]==FALSE || m_cbPlayStatus[pCompareCard->wCompareUser]==FALSE)return false;

			//��Ϣ����
			return OnUserCompareCard(wChairID,pCompareCard->wCompareUser);
		}
	case SUB_C_ADD_SCORE:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pData;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			WORD wChairID = pIServerUserItem->GetChairID();

			//״̬�ж�
			ASSERT(m_cbPlayStatus[wChairID]==TRUE);
			if (m_cbPlayStatus[wChairID]==FALSE) return false;

			//��ǰ״̬
			if(pAddScore->wState>0)m_lCompareCount=pAddScore->lScore;

			//��Ϣ����
			return OnUserAddScore(wChairID,pAddScore->lScore,false,((pAddScore->wState>0)?true:false));
		}
	case SUB_C_WAIT_COMPARE:	//�ȴ�����
		{
			//Ч����Ϣ
			ASSERT(wDataSize==0);
			if (wDataSize!=0) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			WORD wChairID = pIServerUserItem->GetChairID();

			//״̬�ж�
			ASSERT(m_cbPlayStatus[wChairID]==TRUE);
			if (m_cbPlayStatus[wChairID]==FALSE) return false;

			//��Ϣ����
			CMD_S_WaitCompare WaitCompare;
			WaitCompare.wCompareUser = wChairID;
			if(m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_WAIT_COMPARE,&WaitCompare,sizeof(WaitCompare)) &&
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_WAIT_COMPARE,&WaitCompare,sizeof(WaitCompare)) )
				return true;
		}
	case SUB_C_FINISH_FLASH:	//��ɶ��� 
		{
												//Ч����Ϣ
			ASSERT(wDataSize==0);
			if (wDataSize!=0) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			WORD wChairID = pIServerUserItem->GetChairID();

			//״̬�ж�
			ASSERT(m_wFlashUser[wChairID]==TRUE);
			if (m_wFlashUser[wChairID]==FALSE) return true;

			//���ñ���
			m_wFlashUser[wChairID] = FALSE;

			//������Ϸ
			WORD i=0;
			for( i=0;i<GAME_PLAYER;i++)
			{
				if(m_wFlashUser[i] != FALSE)break;
			}	
			if(i==GAME_PLAYER)
			{
				//ɾ��ʱ��
				m_pITableFrame->KillGameTimer(IDI_GAME_COMPAREEND);
				m_pITableFrame->KillGameTimer(IDI_GAME_OPENEND);

				ASSERT(m_pITableFrame->GetGameStatus()==GAME_STATUS_PLAY);
				//������Ϣ
				if(m_pITableFrame->GetGameStatus()==GAME_STATUS_PLAY)
				{
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));

					TraceMessage(m_GameRecord,m_wTableID);

					//������Ϸ
					
					m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);
				}
			}
			return true;
		}
		
		case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
			return OnSubAmdinCommand(pIServerUserItem,pData);
		}

	}
	}catch(...){
		CString szerror ;
		szerror.Format(TEXT("%d��%d��%s"),wSubCmdID,wDataSize,TEXT("��Ϸ��Ϣ����"));
		CTraceService::TraceString(szerror,TraceLevel_Exception);
	}
	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}



//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}
//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID,bool bExit)
{
	//��������
	m_cbPlayStatus[wChairID] = FALSE;


	//�޸Ļ���
	tagScoreInfo ScoreInfoArray;
	ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
	ScoreInfoArray.cbType =(bExit)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
	ScoreInfoArray.lRevenue=0;  
	ScoreInfoArray.lScore=-Double_Round(m_lTableScore[wChairID]/100.00,2,1);
	m_pITableFrame->WriteUserScore(wChairID,ScoreInfoArray);

	//��Ϸ��¼
	if(bExit)
		m_GameRecord.AppendFormat( TEXT("P%ld:��,%0.2f#"),wChairID,ScoreInfoArray.lScore);
	else
		m_GameRecord.AppendFormat( TEXT("P%ld:��,%0.2f#"),wChairID,ScoreInfoArray.lScore);

	///////////////////////////////////////////////////
	//ʣ�µ������û���û�û�н�ֱ�ӿ���  4-8�޸�..
	//��ʼ��ָ��
	//IServerUserItem *pIServerUserItem=NULL;

	//WORD wPlayer = 0;
	//WORD wChair = INVALID_CHAIR;
	////��ȡ���������˸�������Ҹ���
	//for(WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
	//	if(pIServerUserItem == NULL) continue;

	//	if (wChair == INVALID_CHAIR && m_cbPlayStatus[i] == true)
	//	{
	//		wChair = pIServerUserItem->GetChairID();
	//	}

	//	if ((!pIServerUserItem->IsAndroidUser())&&m_cbPlayStatus[i] == true)
	//	{
	//		wPlayer++;
	//	}
	//}

	//if(0 == wPlayer)
	//{
	//	OnUserOpenCard(wChair);
	//	return true;
	//}

	//����ͳ��
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//�жϽ���
	if (wPlayerCount>=2)
	{
		if (m_wCurrentUser==wChairID) OnUserAddScore(wChairID,0L,true,false);
		//������Ϣ
		CMD_S_GiveUp GiveUp;
		GiveUp.wGiveUpUser=wChairID;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	}
	else OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserLookCard(WORD wChairID)
{
	//״̬Ч��
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return false;

	//����Ч��
	ASSERT(!m_bMingZhu[wChairID]);
	if (m_bMingZhu[wChairID]) return true;

	//���ò���
	m_bMingZhu[wChairID]=true;

	//��������
	CMD_S_LookCard LookCard;
	CopyMemory(LookCard.cbCardData,m_cbHandCardData[wChairID],sizeof(m_cbHandCardData[0]));
	LookCard.wLookCardUser=wChairID;

	m_GameRecord.AppendFormat( TEXT("P%ld:����#"),wChairID);

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LOOK_CARD,&LookCard,sizeof(LookCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LOOK_CARD,&LookCard,sizeof(LookCard));

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserCompareCard(WORD wFirstChairID,WORD wNextChairID)
{
	//Ч�����
	ASSERT(m_lCompareCount>0);
	if(!(m_lCompareCount>0))return false;
	ASSERT(m_wCurrentUser==m_wBankerUser || m_lTableScore[m_wCurrentUser]+m_lCompareCount>=m_lCellScore_Long);
	if(!(m_wCurrentUser==m_wBankerUser || m_lTableScore[m_wCurrentUser]+m_lCompareCount>=m_lCellScore_Long))return false;

	//��Ϸ��¼
	m_GameRecord.AppendFormat(TEXT("P%ld-P%ld����#"),wFirstChairID,wNextChairID);

	//�Ƚϴ�С
	WORD wChairID=m_GameLogic.CompareCard(m_cbHandCardData[wFirstChairID],m_cbHandCardData[wNextChairID],MAX_COUNT);

	//״̬����
	m_lCompareCount=0L;

	//ʤ���û�
	WORD wLostUser,wWinUser;
	if(wChairID==TRUE) 
	{
		wWinUser=wFirstChairID;
		wLostUser=wNextChairID;
	}
	else
	{
		wWinUser=wNextChairID;
		wLostUser=wFirstChairID;
	}

	//��������
	m_wCompardUser[wLostUser].Add(wWinUser);
	m_wCompardUser[wWinUser].Add(wLostUser);
	m_cbPlayStatus[wLostUser]=FALSE;

	//����ͳ��
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	///////////////////////////////////////////////////
	//ʣ�µ������û���û�û�н�ֱ�ӿ���  4-8�޸�..
	//��ʼ��ָ��
	IServerUserItem *pIServerUserItem=NULL;

	WORD wPlayer = 0;
	WORD wChair = INVALID_CHAIR;
	//��ȡ���������˸�������Ҹ���
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		if (wChair == INVALID_CHAIR && US_PLAYING == pIServerUserItem->GetUserStatus())
		{
			wChair = pIServerUserItem->GetChairID();
		}

		if (pIServerUserItem!=NULL)
		{
			if (!pIServerUserItem->IsAndroidUser())
			{
				wPlayer++;
			}
		}
	}

	if(0 == wPlayer)
	{
		OnUserOpenCard(wChair);
		return true;
	}

	//������Ϸ
	if(wPlayerCount>=2)
	{
		if(m_cbPlayStatus[wFirstChairID]==TRUE)
		{
			m_wCurrentUser=wFirstChairID;
		}
		else
		{
			//�û��л�
			WORD wNextPlayer=INVALID_CHAIR;
			for (WORD i=1;i<m_wPlayerCount;i++)
			{
				//���ñ���
				wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;

				//�����ж�
				if (m_cbPlayStatus[wNextPlayer]==TRUE) break;
			}
			//�����û�
			m_wCurrentUser=wNextPlayer;
		}
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_S_CompareCard CompareCard;
	CompareCard.wCurrentUser = m_wCurrentUser;
	CompareCard.wLostUser = wLostUser;
	CompareCard.wCompareUser[0] = wFirstChairID;
	CompareCard.wCompareUser[1] = wNextChairID;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_COMPARE_CARD,&CompareCard,sizeof(CompareCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_COMPARE_CARD,&CompareCard,sizeof(CompareCard));

	//�޸Ļ���
	tagScoreInfo ScoreInfoArray;
	ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
	ScoreInfoArray.cbType =SCORE_TYPE_LOSE;                                
	ScoreInfoArray.lScore=-(m_lTableScore[wLostUser]/100.00);

	m_pITableFrame->WriteUserScore(wLostUser,ScoreInfoArray);

	//��Ϸ��¼
	m_GameRecord.AppendFormat( TEXT("P%ld:%0.2f#"),wLostUser,ScoreInfoArray.lScore);

	CString strLog;
	strLog.Format(L"\nwPlayerCount = %d",wPlayerCount);
	OutputDebugString(strLog);

	//������Ϸ
	if (wPlayerCount<2)
	{
		m_wBankerUser=wWinUser;
		m_wFlashUser[wNextChairID] = TRUE;
		m_wFlashUser[wFirstChairID] = TRUE;
		OnEventGameConclude(GAME_PLAYER,NULL,GER_COMPARECARD);
	}

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserOpenCard(WORD wUserID,bool bSystemOpen)
{
	//Ч�����
	ASSERT(m_lCompareCount>0);
	if(!(m_lCompareCount>0))return false;
	ASSERT(m_wCurrentUser==wUserID);
	if(m_wCurrentUser!=wUserID)return false;

	//��¼��Ϣ
	if(bSystemOpen)
		m_GameRecord+= TEXT("ϵͳ����#");
	else
	{
		m_GameRecord.AppendFormat( TEXT("P%ld,����#"),wUserID);
	}

	//��������
	m_wCurrentUser=INVALID_CHAIR;
	m_lCompareCount = 0L;

	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//�������
	WORD wWinner=wUserID;

	//����������
	for (WORD i=1;i<m_wPlayerCount;i++)
	{
		WORD w=(wUserID+i)%m_wPlayerCount;

		//�û�����
		if (m_cbPlayStatus[w]==FALSE) continue;

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[w],cbUserCardData[wWinner],MAX_COUNT)>=TRUE) 
		{
			wWinner=w;
		}
	}
	ASSERT(m_cbPlayStatus[wWinner]==TRUE);
	if(m_cbPlayStatus[wWinner]==FALSE)return false;

	//ʤ�����
	m_wBankerUser = wWinner;

	//��������
	CMD_S_OpenCard OpenCard;
	OpenCard.wWinner=wWinner;
	OpenCard.bSystem=bSystemOpen;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));

	//������Ϸ
	for(WORD i=0;i<GAME_PLAYER;i++)if(m_cbPlayStatus[i]==TRUE)m_wFlashUser[i] = TRUE;
	OnEventGameConclude(GAME_PLAYER,NULL,GER_OPENCARD);

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore, bool bGiveUp, bool bCompareCard)
{
	if (bGiveUp==false)				//��������
	{
		//״̬Ч��
		ASSERT(m_wCurrentUser==wChairID);
		if (m_wCurrentUser!=wChairID) return false;
		if(lScore<0) return false;
		//���Ч��
		ASSERT((lScore+m_lTableScore[wChairID])<=m_lUserMaxScore[wChairID]);
		if ((lScore+m_lTableScore[wChairID])>m_lUserMaxScore[wChairID]) return false;

		//��ǰ����					bGiveUp������lScoreΪ0
		LONGLONG lTimes=(m_bMingZhu[wChairID] || bCompareCard)?2:1;

		LONGLONG lTemp=lScore/m_lCellScore_Long/lTimes;
		ASSERT(m_lCurrentTimes<=lTemp && m_lCurrentTimes<=m_lMaxCellScore/m_lCellScore_Long);
		if(!(m_lCurrentTimes<=lTemp && m_lCurrentTimes<=m_lMaxCellScore/m_lCellScore_Long))return false;
		m_lCurrentTimes = lTemp;

		//�û�ע��
		m_lTableScore[wChairID]+=lScore;
		m_lAddScoreTimes[wChairID]++;

		//��Ϸ��¼
		//m_GameRecord.AppendFormat( TEXT("P%ld:��,%0.2f,��%0.2f#"),wChairID,lScore/100.00,m_lTableScore[wChairID]/100.00);
	}

	bool bSystemOpen = false;
	//�����û�
	if(!bCompareCard)
	{
		//�û��л�
		WORD wNextPlayer=INVALID_CHAIR;
		for (WORD i=1;i<m_wPlayerCount;i++)
		{
			//���ñ���
			wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;
			
			//�����ж�
			if (m_cbPlayStatus[wNextPlayer]==TRUE)
			{
				m_wCurrentUser=wNextPlayer;
				break;
			}
		}

		CString str;
		str.Format(TEXT("wMaxTimes = %d\n"),m_pGameCustomRule->wMaxTimes);
		if(m_pGameCustomRule->wMaxTimes>0)
		{
			if(wNextPlayer!=INVALID_CHAIR && m_lTableScore[wChairID]>=(m_lCellScore_Long*m_pGameCustomRule->wMaxTimes))
			{
				bSystemOpen= true;
			}
		}
	}	

	//��������
	CMD_S_AddScore AddScore;
	AddScore.lCurrentTimes=m_lCurrentTimes;
	AddScore.wCurrentUser=bSystemOpen?INVALID_CHAIR:m_wCurrentUser;
	AddScore.wAddScoreUser=wChairID;
	AddScore.lAddScoreCount=lScore;

	//�ж�״̬
	AddScore.wCompareState=( bCompareCard )?TRUE:FALSE;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));

	if(bSystemOpen)
	{
		m_lCompareCount = lScore;

		OnUserOpenCard(m_wCurrentUser,true);
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

//�����Ϣ
void TraceMessage(CString szinfo,WORD id)
{
	if(!szinfo.IsEmpty())
		szinfo.Replace(TEXT("#"),TEXT("\r\n"));
	
	COleDateTime timeNow, dateNow;
	timeNow = COleDateTime::GetCurrentTime();     // ��ȡ��ǰ����ʱ��
	dateNow = COleDateTime::GetCurrentTime();     // ͬ����ȡ��ǰ����ʱ��
	CString sTime = timeNow.Format(VAR_TIMEVALUEONLY);     // ��ȡ��ǰʱ��
	CString sDate = dateNow.Format(VAR_DATEVALUEONLY);     // ��ȡ��ǰ����

	TCHAR tchBuffer[MAX_PATH ]; 
	LPTSTR lpszCurDir;
	lpszCurDir = tchBuffer; 
	GetCurrentDirectory(MAX_PATH , lpszCurDir);//�õ���ǰ����·��

	CString Filep = lpszCurDir;
	Filep.AppendFormat(TEXT("\\%s"),GAME_NAME);

	//�����ļ���	��Ϸ����
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ�����ļ���ʧ�ܣ�")+Filep,TraceLevel_Exception);
		 }
	}
	Filep.AppendFormat(TEXT("\\TableID_%d"),id);

	//�����ļ���	���ӷ���
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ�����ļ���ʧ�ܣ�")+Filep,TraceLevel_Exception);
		 }
	}
	Filep.AppendFormat(TEXT("\\%s"),sDate);
	
	//�����ļ���	���ڷ���
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ�����ļ���ʧ�ܣ�")+Filep,TraceLevel_Exception);
		 }
	}

	
	CString FileName=TEXT("");     // �ļ��� = ����_ʱ��.txt
	FileName.AppendFormat(TEXT("%s\\%s.txt"),Filep,sTime);

	TCHAR pFilePath[MAX_PATH];
	LPCTSTR filepath = FileName;
	int FilePathLength = FileName.GetLength();     // ����ļ������ȣ�CFile������CString��ֻ����string��������Ҫ��ת��
	int i=0;
	for( i=0; i < FilePathLength ; i++)
	{
		pFilePath[i] = FileName.GetAt(i);     // CString -> string
		if( (pFilePath[i] == TEXT(':')) && (i != 1))     // �޳�':'�Ȳ�����Ϊ�ļ����ķ���,��������Ŀ¼���ð��
			pFilePath[i] = '-';
	}
	pFilePath[i] = '\0';     // �ļ�������

	CFile File;
	if ((File.Open(pFilePath,CFile::modeWrite)==FALSE)&&
		(File.Open(pFilePath,CFile::modeWrite|CFile::modeCreate)==FALSE))
	{
		ASSERT(FALSE);
		return;
	}

	File.SeekToEnd();
	File.Write(szinfo.GetBuffer(),szinfo.GetLength()*2);

	szinfo.ReleaseBuffer();
	File.Flush();
	File.Close();

	return;
}
//////////////////////////////////////////////////////////////////////////
//������ר�ã�������
void CTableFrameSink::AndroidSendUserCard()
{

	// �����˿�
	CMD_S_SendCard pSendCard;
	ZeroMemory(&pSendCard,sizeof(pSendCard));
	CopyMemory(pSendCard.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));
	//�����û�״̬
	CopyMemory(pSendCard.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//��ȡ��������
	//�����ֵ�������
	int iMaxIndex=0;
	

	for(int i=0; i<GAME_PLAYER;i++)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue;  //����û���ǰ������Ϸ��̬��������FOR
		//��ȡ����������
		BYTE cbCompareResult=m_GameLogic.CompareCard(m_cbHandCardData[iMaxIndex],m_cbHandCardData[i],MAX_COUNT);
		if(cbCompareResult==0) iMaxIndex=i;
	}


	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		if(pIServerUserItem==NULL) continue;
		// �жϻ����˷� 
		if(pIServerUserItem->IsAndroidUser()||(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))==true)
		{

			pSendCard.IsWinUser=false;
			//��������Ƹ�������;
			if(i==iMaxIndex) //Ӯ��
				pSendCard.IsWinUser=true;
	
			m_pITableFrame->SendTableData(i,SUB_S_ANDROID_GET_CARD,&pSendCard,sizeof(pSendCard));
		}
	}
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
void CTableFrameSink::ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType)
{	
	//��ȡ�û�
	IServerUserItem * pIServerCtrlUserItem=m_pITableFrame->GetTableUserItem(wChairIDCtrl);
	if(pIServerCtrlUserItem==NULL) return; //�û�������, ����Ҫ���Ʒ���
	if(pIServerCtrlUserItem->GetUserID()!=m_dwCheatGameID ) return; //����ID��һ�·���
	//�����ֵ�������
	int iMaxIndex=0;
	int iLostIndex=0;

	for(int i=0; i<GAME_PLAYER;i++)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue;  //����û���ǰ������Ϸ״̬��������FOR
		//��ȡ����������
		BYTE cbCompareResult=m_GameLogic.CompareCard(m_cbHandCardData[iMaxIndex],m_cbHandCardData[i],MAX_COUNT);
		if(cbCompareResult==0) iMaxIndex=i;
	}

	//�γ������޸�8.9
	//////////////////////����˿� ���ҵڶ�����˿˸����ƺ�
	if(iMaxIndex==0) iLostIndex=1;
	//�Ա����
	for (int i=0;i<GAME_PLAYER;i++)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue; //����û���ǰ������Ϸ״̬��������FOR
		if(iMaxIndex==i) continue; //����
		//�Ա��˿�
		BYTE cbCompareResult=m_GameLogic.CompareCard(m_cbHandCardData[iLostIndex],m_cbHandCardData[i],MAX_COUNT);
		if(cbCompareResult==0) iLostIndex=i;
	}
	

	//��ʱ����
	BYTE cbTemp[MAX_COUNT]={0};
	//������
	WORD wWinerUser=iMaxIndex;  //���������
	if(cbCheatType==CHEAT_TYPE_LOST) wWinerUser=iLostIndex; //��С������

	if (wWinerUser!=wChairIDCtrl)    //�����ǰ�û����ǿ��ƺ�,�򽻻��˿����� ������Ҫ����
	{

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
	
	return;
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

	//�����ֵ����
	WORD iMaxIndex=INVALID_CHAIR;	

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

	//��ȡ����������
	for(int i=0; i<GAME_PLAYER;++i)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue;  //������û������Ϸ������

		if (iMaxIndex==INVALID_CHAIR)
		{
			iMaxIndex=i;  //��ʼ����һλ�û�
			continue;
		}

		//��һ���ȵڶ�����Ϊ��
		bool bFirstIsMax=m_GameLogic.CompareCard(m_cbHandCardData[iMaxIndex],m_cbHandCardData[i],MAX_COUNT);
		//�ڶ����ȵ�һ����Ļ�����������¼�����ٱȽϣ�ֱ���ҳ�����Ƶ����
		if(!bFirstIsMax) iMaxIndex=i;
	}

	//���Ʊ���
	int iTarget=0;//Ŀ������
	bool bChange = false;//�Ƿ�Ҫ����

	//�������
	srand( (unsigned)time(NULL));
	//��ȡ�����
	WORD Ratio = rand()%(100); //����0~100����

	//iMaxindexitem��ȡ
	pIServerUserItem=m_pITableFrame->GetTableUserItem(iMaxIndex);  //��ȡ����Ƶ��û�
	if(pIServerUserItem == NULL) return false;   //����û�����������
	bool ReturnValue=(m_AndroidWinningRatio>Ratio)?true:false; //������ñ����ɵ�����
	//������Ӯ
	if(ReturnValue)  //������ñ����ɵ�����
	{
		if(pIServerUserItem->IsAndroidUser()==FALSE)//���������������
		{
			iTarget=iPostionAndroid[rand()%iAndroidCount];  //���������Ŀ�껻��
			bChange = true;									//���û��Ʊ���ΪTRUE
		}
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
		CopyMemory(m_cbHandCardData[iTarget],cbTemp,MAX_COUNT);    //Ŀ���������û�����
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

	//�����ֵ����
	WORD iMaxIndex=INVALID_CHAIR;	

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

	//��ȡ����������
	for(int i=0; i<GAME_PLAYER;++i)
	{
		//�û�����
		if (m_cbPlayStatus[i]==FALSE) continue;  //������û������Ϸ������

		if (iMaxIndex==INVALID_CHAIR)
		{
			iMaxIndex=i;  //��ʼ����һλ�û�
			continue;
		}

		//��һ���ȵڶ�����Ϊ��
		bool bFirstIsMax=m_GameLogic.CompareCard(m_cbHandCardData[iMaxIndex],m_cbHandCardData[i],MAX_COUNT);
		//�ڶ����ȵ�һ����Ļ�����������¼�����ٱȽϣ�ֱ���ҳ�����Ƶ����
		if(!bFirstIsMax) iMaxIndex=i;
	}

	//���Ʊ���
	int iTarget=0;//Ŀ������
	bool bChange = false;//�Ƿ�Ҫ����

	//�������
	srand( (unsigned)time(NULL));
	//��ȡ�����
	WORD Ratio = rand()%(100); //����0~99����
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
	if(m_lStockScore>=m_lStorageMax)
	{	
		SCORE tmp_lStockScore = m_lStockScore;							 //ԭ�����
		SCORE m_StorageEating=m_lStockScore*(StorageEating/100.00);		 //ϵͳ�Է�

		//��ȥ�Ե��ķ�
		m_lStockScore-=m_StorageEating;
		WinRatio=StorageMaxRatio;  //�ͷ�ģʽ
		//�ͷ�ģʽ
		StorageStyle=3;
		//������ֵ
		CString strLog;
		strLog.Format(TEXT("���ţ�%d,��ǰ��棺%0.2f,ϵͳ�Է֣�%0.2f,ʣ���棺%0.2f"),m_pITableFrame->GetTableID()+1,tmp_lStockScore,m_StorageEating,m_lStockScore);
		CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ
	}
	
	//���������Сֵ,ɱ��ģʽ
	if(m_lStockScore<=m_lStorageMin)
	{
		WinRatio=StorageMinRatio;
		StorageStyle=2;//ɱ��ģʽ
	}

	//����������С֮��,Ĭ��ģʽ
	if(m_lStockScore > m_lStorageMin && m_lStockScore < m_lStorageMax)
	{
		WinRatio=StorageRatio;
		StorageStyle = 1;//Ĭ��ģʽ
	}
	
	bool ReturnValue=(WinRatio>Ratio)?true:false; //�������
	//�γ��������
	
			    //�����ǰģʽ
				CString strLog;
				strLog.Format(TEXT("���ţ�%d,��Ϸģʽ��%d,��ǰ��Ӯ���ʣ�%d (��Ϸģʽ1,����,2,ɱ��,3,�ͷ�)"),m_pITableFrame->GetTableID()+1,StorageStyle,WinRatio);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //�����Ϣ

	//������Ӯ
	if(ReturnValue)  //������ñ����ɵ�����
	{
		if(pIServerUserItem->IsAndroidUser()==FALSE)//���������������
		{
			iTarget=iPostionAndroid[rand()%iAndroidCount];  //���������Ŀ�껻��
			bChange = true;									//���û��Ʊ���ΪTRUE
		}
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
	if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
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
			m_cbCheatType=AdminReq->cbCheatType;
			m_dwCheatCount=AdminReq->cbCheatCount;
			m_dwCheatGameID=AdminReq->dwGameID;
			
			

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

			m_dwCheatGameID1 = pIServerUserItem->GetUserID();

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



////////
//��ȡ����
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	TCHAR OutBuf[255];
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);
	//��ȡ����
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\ZaJinHuaConfig.ini"),szPath);
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


	//����˥��ֵ
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageDeduct"),TEXT("1"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageDeduct);

	//������ֵ
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageMax"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageMax);

		//�γ��������
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
	if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
	m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);


	//ϵͳ����Ӯ��
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageLimit"),TEXT("0.0"),OutBuf,255,szFileName);
	
	//////////////////////////////////
	//�γ������ȡС������ֵ
	CString TempValue;
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
	m_lStockLimit=Mywtof(TempValue);
	/////////////////////////////////

	//�γ��������   //����ǰ̨����
	//���������Ӯ
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

}

