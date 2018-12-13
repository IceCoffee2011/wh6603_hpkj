#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "SendCard.h"
//#include "..\..\������\Include\cnd_ILogMgr.h"
//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//Ч������
enum enEstimatKind
{
	EstimatKind_OutCard,			//����Ч��
	EstimatKind_GangCard,			//����Ч��
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	WORD							m_wSiceCount;							//���ӵ���
	WORD							m_wBankerUser;							//ׯ���û�
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//�û��˿�
	bool							m_bTrustee[GAME_PLAYER];				//�Ƿ��й�
	//WORD                            m_wMPIndex;                             //��������
	

	//������Ϣ
protected:
	WORD							m_wOutCardUser;							//�����û�
	BYTE							m_cbOutCardData;						//�����˿�
	BYTE							m_cbOutCardCount;						//������Ŀ
	BYTE							m_cbDiscardCount[GAME_PLAYER];			//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];		//������¼

	//������Ϣ
protected:
	BYTE							m_cbSendCardData;						//�����˿�
	BYTE							m_cbSendCardCount;						//������Ŀ
	BYTE							m_cbLeftCardCount;						//ʣ����Ŀ
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//����˿�
	BYTE							m_cbKingCard[4];						//����
	BYTE							m_cbKingCount[GAME_PLAYER];				//���Ƹ���
	//BYTE                            m_cbMoCard;                             //������


	//
protected:
	//tagGSKUResult					m_tagUserGSKUResult[GAME_PLAYER];

	//���б���
protected:
	WORD							m_wResumeUser;							//��ԭ�û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wProvideUser;							//��Ӧ�û�
	BYTE							m_cbProvideCard;						//��Ӧ�˿�
	LONGLONG					m_llMuscle[GAME_PLAYER];					// ����Ŀ					

	//״̬����
protected:
	bool							m_bSendStatus;							//����״̬
	bool							m_bGangStatus;							//����״̬
	//bool							m_bEnjoinChiHu[GAME_PLAYER];			//��ֹ�Ժ�
	//bool							m_bEnjoinChiPeng[GAME_PLAYER];			//��ֹ����

	//�û�״̬
public:
	bool							m_bResponse[GAME_PLAYER];				//��Ӧ��־
	WORD							m_cbUserAction[GAME_PLAYER];			//�û�����
	BYTE							m_cbOperateCard[GAME_PLAYER];			//�����˿�
	WORD							m_cbPerformAction[GAME_PLAYER];			//ִ�ж���

	//����˿�
protected:
	BYTE							m_cbWeaveItemCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][4];		//����˿�
	BYTE							m_byGangCard[GAME_PLAYER][4];			//����
	BYTE							m_byGangCardNums[GAME_PLAYER];			//����
	BYTE							m_byBuCard[GAME_PLAYER][4];				//����
	BYTE							m_byBuCardNums[GAME_PLAYER];			//����



//2011-8-8	osc update
private:
	//bool							m_bUserFirstIsTingPai[GAME_PLAYER];		//�Ƿ��һ������
	//bool							m_bHearStatus[GAME_PLAYER];				//����״̬
	//bool							m_bIsUserFirstOutCard[GAME_PLAYER];		//�Ƿ��һ�γ���
	WORD							m_byUserHuCardType[GAME_PLAYER];		//��Һ�������
	//int								m_iSaveGameOutCardCount;				//������Ϸ������(���ڸ�����)
	//int								m_iSaveGameSendCardCount;				//������Ϸ������(���ڸ�����)	
	BYTE							m_byGameEndType;						//��Ϸ��������, 0:��ׯ, 1:����; 2:����; 3:��������; 4:���׷���; 5�����ף���ׯ

	bool                            m_bIsPlayDice;                         //�Ƿ������


	//������Ϣ
protected:
	WORD							m_wHaiDiUser;							//�����û�
	BYTE							m_cbChiHuCard;							//�Ժ��˿�
	tagChiHuResult					m_ChiHuResult[GAME_PLAYER];				//�Ժ����
	CMD_S_OperateResult		m_OperateResult;						///< ����ɺ��������ݴ�

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

private:
	CSendCard						m_SendCardTool;							//v

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	//static const enStartMode		m_GameStartMode;						//��ʼģʽ
private:
	//cnd_ILogMgr*	m_pRoomLog;	// ��־�����ָ��
	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void  Release() { if (IsValid()) delete this; return ; }
	//�Ƿ���Ч
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	// ͨ����Ϸ����˵�ͨ�������Դ����κμ������ݻ��Աָ��
	virtual bool channel_to_game_service(int iMainCode, int iAssistantCode, void* pData, int iDataLen);

	//��ʼ��
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void  RepositionSink();

	//��Ϣ�ӿ�
public:
	////��ʼģʽ
	//virtual enStartMode  GetGameStartMode();
	//��Ϸ״̬
	virtual bool  IsUserPlaying(WORD wChairID);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool  OnEventGameStart();
	//��Ϸ����
	virtual bool  OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool  OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool  OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool  OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool  OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�����¼�
public:
	//�û�ͬ��
	virtual bool  OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	//�û�����
	virtual bool  OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool  OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool  OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool  OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);


public:
	//��ʼ������
	void InitData();
	// ������Ϣ
	bool OnUserOpenDoor();

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//�û�����
	bool OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);
	/// ��ǰ��Ҳ���
	bool OnCurUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);
	/// ������Ҳ���
	bool OnOtherUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard);
	////���ײ���
	//bool OnUserOperateHaiDi(WORD wChairID, BYTE cbTakeHaiDi);
	//�û����Ʋ���
	bool OnUserTingCard(WORD wChairID, bool bIsTing=false);

	//��������
protected:
	//���Ͳ���
	bool SendOperateNotify();
	//�ɷ��˿�
	/// [@param bSupplyCard in] �油�ƣ�����������
	bool DispatchCardData(WORD wCurrentUser, bool bSupplyCard);
	//��Ӧ�ж�
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);
	//bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);
	//�û�����
	void UserStartOperation();
	/// д���
	/// [@param wWinChairID in] Ӯ ���ID
	/// [@param wLostChairID in] �� Ǯ���ID
	/// [@param iMuscle in] ����
	/// [@param llResultGold out] ÿ����ҵı䶯���
	bool OnWriteMuscleScore(WORD wWinChairID, WORD wLostChairID, int iMuscle, LONGLONG llResultGold[]);
	bool OnWriteMuscleScore(WORD wWinChairID, int iMuscle, LONGLONG llResultGold[]);


//	//�����ж�  CP add 4.12
//protected:
//	void OnUserTaskJudgement(WORD wChairID, BYTE btTag);
	/// ���Խ���
	void TestGameEndFun(BYTE bSelect);
	///////////////////////////
public:
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	virtual bool QueryBuckleServiceCharge(WORD wChairID);
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	virtual void SetGameBaseScore(LONG lBaseScore);
	virtual bool OnActionUserOnReady(WORD,IServerUserItem *,void *,WORD);
};



//////////////////////////////////////////////////////////////////////////

#endif