#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//��Ϸ����
protected:
	WORD							m_wCurrentUser;							 //��ǰ�û�
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
	bool							m_bFirstAdd;							 //��ʼ��һ�������������Ա���
	//�û�״̬
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	LONGLONG						m_lTableScore[GAME_PLAYER];				//��ע��Ŀ

	//��ע��Ϣ
protected:
	LONGLONG						m_lMaxScore;							//�ⶥ��Ŀ
	LONGLONG						m_lMaxCellScore;						//��Ԫ����
	LONGLONG						m_lUserAddScore;						//�û���ע(��)��Ŀ
	WORD							m_bCompareState;						//����״̬
	LONGLONG						m_lCellScore;							//��Ԫ��ע
	LONGLONG						m_lCurrentTimes;						//��ǰ����
	LONGLONG						m_lUserMaxScore;						//�û�������


	//�γ�����
	bool							m_bMingZhu[GAME_PLAYER];				//���ƶ���
	bool							m_bBiggest;								//�������
	BYTE                            m_cbCardV;								//��ǰ�Ƶ�����
	bool							m_bIsYz;								//���Ƿ��Ż�����������
	int								m_bMinZhuIndex;							//���� //���ƻ���
	//�ؼ�����
public:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	IAndroidUserItem *				m_pIAndroidUserItem;					//�û��ӿ�
	

	//////////////////////////////�������������
	//���в���
	LONGLONG						m_lRobotBankGetScore;					//ȡ����С����(ȡ�����Ǹ�ֵ���ϵ�һ�������)
	LONGLONG						m_lRobotBankGetScoreBanker;				//ȡ���������(����ֵһ��Ҫ����RobotBankGet)
	int								m_nRobotBankStorageMul;					//����
	LONGLONG						m_lBankerLostScore;						//ׯ����� (��������Ϊׯ)
	LONGLONG						m_lRobotScoreRange[2];					//���Χ
	LONGLONG						m_MinLostScore;							//������� �������ֵ,���ƻ��ʱȽ���
	LONGLONG						m_MaxLostScore;							//������� �������ֵ,���ƻ��ʷǳ���
	int								m_MinLostIndex;							//���ƻ��ʱȽ���
	int								m_MaxLostIndex;							//���ƻ��ʷǳ���
	TCHAR							m_szRoomName[32];						//���÷���
	/////////////////////////////�������������
	
	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool  RepositionSink();

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool  OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool  OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool  OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool  OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual void  OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û��뿪
	virtual void  OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�����
	virtual void  OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�״̬
	virtual void  OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);


	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubLookCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubCompareCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//�û�ǿ��
	bool OnSubPlayerExit(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//ȫ������
	bool Alloperations();
	//����
	bool OnAllCompare();
	//����
	bool GaiLv(BYTE bNum);
	
	bool IsCanYa();
	//ɾ������ʱ��
	void KillAllTimer();
	//��ȡ����
	void ReadConfigInformation();

};

//////////////////////////////////////////////////////////////////////////

#endif
