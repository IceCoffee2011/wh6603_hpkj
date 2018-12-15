#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "DlgCustomRule.h"
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink :public ITableFrameSink, public ITableUserAction
{
			
	//��Ϸ��¼
protected:
	CString							m_GameRecord;
	TCHAR							m_szNickName[GAME_PLAYER][LEN_NICKNAME];
	LONGLONG						m_lAddScoreTimes[GAME_PLAYER];
	WORD							m_wTableID;

	//��Ϸ����
protected:
	BYTE							m_bOperaCount;							//��������
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wBankerUser;							//ׯ���û�
	WORD							m_wFlashUser[GAME_PLAYER];				//�����û�
	SCORE							m_lCellScore;							//��Ԫ��ע

	//�û�״̬
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	LONGLONG						m_lCompareCount;						//����״̬
	bool							m_bGameEnd;								//����״̬
	CMD_S_GameEnd					m_StGameEnd;							//��������

	//�˿˱���
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
	CWHArray<WORD>					m_wCompardUser[GAME_PLAYER];			 //�˿�����

	//��ע��Ϣ
protected:
	LONGLONG						m_lCellScore_Long;						//��Ԫ��ע
	LONGLONG						m_lTableScore[GAME_PLAYER];				//��ע��Ŀ
	LONGLONG						m_lUserMaxScore[GAME_PLAYER];			//�����ע
	LONGLONG						m_lMaxCellScore;						//��Ԫ����
	LONGLONG						m_lCurrentTimes;						//��ǰ����
	bool							m_bMingZhu[GAME_PLAYER];				//������ע

	//AI����
protected:
	//�γ��������
	///////
	bool							m_bRefreshCfg;							//ÿ��ˢ��
	bool							m_bRefreshStorageCfg;					//�Ƿ�ˢ�¿��
	//////

	static SCORE					m_lStockScore;							//����Ӯ��
	static LONGLONG					m_lStorageOff;							//������ֵ
	static BYTE						StorageStyle;							//��ǰϵͳ�Է�ģʽ,1,����,2ɱ��,3�ͷ�	
	LONGLONG						m_lStorageDeduct;						//�ؿ۱��� ǧ�ֱ�
	SCORE							m_lStockLimit;							//����Ӯ��


	LONGLONG						m_lStorageMin;							//�����Сֵ
	LONGLONG						m_lStorageMax;							//������ֵ
	BYTE							StorageRatio;							//����ֵ��Ӯ�����ٷ�֮N		
	BYTE							StorageMinRatio;						//�����Сֵ��Ӯ�����ٷ�֮N		ɱ��ģʽ
	BYTE							StorageMaxRatio;						//������ֵ��Ӯ�����ٷ�֮N		�ͷ�ģʽ
	LONGLONG						StorageEating;							//�Էְٷֱ�,ϵͳ�������ֵ��һ���ԳԵ����ֵ
	
	
	
	TCHAR							m_szFileName[MAX_PATH];					//�����ļ�·��
protected:
	//���������Ӯ
	bool							m_bWinnerCtrl;							//�Ƿ�������Ӯ
	BYTE							m_cbCheatType;							//��������
	DWORD							m_dwCheatCount;							//���״���
	DWORD							m_dwCheatGameID;						//�����ʺ�
	DWORD							m_dwCheatGameID1;						//�����ʺ�

	//////�����Ӯ����
	bool							m_bAndroidWinningCtrl;					//�Ƿ���ƻ�������Ӯ����
	LONGLONG				        m_AndroidWinningRatio;					//������Ӯ�ı���
//�γ��������
	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	tagGameServiceOption			* m_pGameServiceOption;					//���ò���
	tagCustomRule					* m_pGameCustomRule;					//�Զ�����
	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	static const BYTE				m_GameStartMode;						//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() {delete this; }
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void  RepositionSink();
	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
		//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual BYTE  GetGameStartMode();
	//��Ϸ״̬
	virtual bool  IsUserPlaying(WORD wChairID);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool  OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool  OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool  OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool  OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool  OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);


	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
protected:
	//�����¼�
	bool OnUserGiveUp(WORD wChairID,bool bExit=false);
	//�����¼�
	bool OnUserLookCard(WORD wChairID);
	//�����¼�
	bool OnUserCompareCard(WORD wFirstChairID,WORD wNextChairID);
	//�����¼�
	bool OnUserOpenCard(WORD wUserID,bool bSystemOpen=false);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, LONGLONG lScore, bool bGiveUp, bool bCompareUser);
protected:
	//��ת����
	CString TransformCardInfo( BYTE cbCardData );
protected:
	//���͹���Ա,�������˿�����
	void AndroidSendUserCard(); 
private:
	//��ȡ����
	void ReadConfigInformation(bool bReadFresh);
	//////��Ӯ����ʵ�ֺ���
	bool AndroidWinningControl();
	//�����û�Id��ȡ�������µ�����
	WORD GetChairIdByGameId(DWORD dwGameId);
	//����ָ�����
	void ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType);
		//����������
	bool AndroidCheck();
	
		//����Ա����
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer);

};

//////////////////////////////////////////////////////////////////////////

#endif
