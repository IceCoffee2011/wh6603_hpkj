#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "HistoryScore.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
		//��Ϸ��¼
protected:
	CString							m_GameRecord;
	TCHAR							m_szNickName[GAME_PLAYER][LEN_NICKNAME];
	//��Ϸ����
protected:
	WORD							m_wBankerUser;							//ׯ���û�
	WORD							m_wFisrtCallUser;						//ʼ���û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	SCORE							m_lExitScore;							//ǿ�˷��� 

	//�û�����
protected:
	BYTE                            m_cbDynamicJoin[GAME_PLAYER];           //��̬����
	BYTE							m_cbPlayStatus[GAME_PLAYER];				//��Ϸ״̬
	BYTE							m_bCallStatus[GAME_PLAYER];				//��ׯ״̬
	BYTE							m_bOxCard[GAME_PLAYER];					//ţţ����
	SCORE							m_lTableScore[GAME_PLAYER];				//��ע��Ŀ

	//�˿˱���
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�

	//��ע��Ϣ
protected:
	SCORE							m_lTurnMaxScore[GAME_PLAYER];			//�����ע


//////////////////////////////�γ��������
	//AI����
protected:
	
	bool							m_bRefreshCfg;							//ÿ��ˢ��
	bool							m_bRefreshStorageCfg;					//�Ƿ�ˢ�¿��
	//////ȫ�־�̬����
	static SCORE					m_lStockScore;							//����Ӯ��
	static LONGLONG					m_lStorageOff;							//������ֵ
	static BYTE						StorageStyle;							//��ǰϵͳ�Է�ģʽ,1,����,2ɱ��,3�ͷ�	
	LONGLONG						m_lStorageDeduct;						//�ؿ۱��� ǧ�ֱ�
	SCORE							m_lStockLimit;							//����Ӯ��
	static SCORE							m_lAndroidWinAndLose;					//��������Ӯ��ֵ	
	//�γ��������
	LONGLONG						m_lStorageMin;							//�����Сֵ
	LONGLONG						m_lStorageMax;							//������ֵ
	BYTE							StorageRatio;							//����ֵ��Ӯ�����ٷ�֮N		
	BYTE							StorageMinRatio;						//�����Сֵ��Ӯ�����ٷ�֮N		ɱ��ģʽ
	BYTE							StorageMaxRatio;						//������ֵ��Ӯ�����ٷ�֮N		�ͷ�ģʽ
	LONGLONG						StorageEating;							//�Էְٷֱ�,ϵͳ�������ֵ��һ���ԳԵ����ֵ
	
				
	TCHAR							m_szFileName[MAX_PATH];					//����·��
	//�γ��������
protected:
	//���������Ӯ
	bool							m_bWinnerCtrl;							//�Ƿ�������Ӯ
	DWORD							m_dwCheatGameID;						//�����ʺ�
	DWORD							m_dwCheatCount;							//���״���
	BYTE							m_cbCheatType;							//��������
	DWORD							m_dwCheatGameID1;						//�����ʺ�
	//////�����Ӯ����
	bool							m_bAndroidWinningCtrl;					//�Ƿ���ƻ�������Ӯ����
	LONGLONG				        m_AndroidWinningRatio;					//������Ӯ�ı���

//////////////////////////////�γ��������
	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���
	CHistoryScore					m_HistoryScore;							//��ʷ�ɼ�

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

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

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool  OnEventGameStart();
	//��Ϸ����
	virtual bool  OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool  OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool  OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool  OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool  OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}

	//��Ϸ�¼�
protected:
	//��ׯ�¼�
	bool OnUserCallBanker(WORD wChairID, BYTE bBanker);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, SCORE);
	//̯���¼�
	bool OnUserOpenCard(WORD wChairID, BYTE bOx);
protected:
	//��ת����
	CString TransformCardInfo( BYTE cbCardData );
	//���ܺ���
private:
	//////��Ӯ����ʵ�ֺ���
	bool AndroidWinningControl();
	//�����˿�����
	bool AndroidCheck();
	//����ָ�����
	bool ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType);
	//����Ա����
	bool OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer);
	//��ȡ���Ӻ�
	WORD GetChairIdByGameId(DWORD dwGameID);
	//��ȡ����
	void ReadConfigInformation(bool bReadFresh);
};

//////////////////////////////////////////////////////////////////////////

#endif
