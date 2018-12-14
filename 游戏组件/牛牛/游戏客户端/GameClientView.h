#pragma once

#include "Stdafx.h"
#include "JettonView.h"
#include "CardControl.h"
#include "HistoryRecord.h"
#include "AdminControlDlg.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_HINT_OX					WM_USER+99							//��ʾ��Ϣ
#define IDM_START					WM_USER+100							//��ʼ��Ϣ
#define IDM_READY					WM_USER+101							//̯����Ϣ
#define IDM_OX						WM_USER+102							//ţţ��Ϣ
#define IDM_MAX_SCORE				WM_USER+103							//����ע
#define IDM_SEND_CARD_FINISH		WM_USER+104							//�������
#define IDM_SHOW_VALUE				WM_USER+105							//������Ϣ
#define IDM_SHOW_INFO				WM_USER+106							//��ʾ��Ϣ
#define IDM_OX_ENABLE				WM_USER+107							//ţţ��ť
#define IDM_SORT					WM_USER+108							//�˿˷���
#define IDM_RESORT					WM_USER+109							//������Ϣ

#define IDM_SHORTCUT_KEY			WM_USER+111							//��ݼ�
#define IDM_BANKER					WM_USER+112							//��ׯ��Ϣ
#define IDM_CLICK_CARD				WM_USER+114							//�����Ϣ
#define IDM_YU_YIN					(WM_USER+209)						//��������


#define MY_VIEW_CHAIRID				2									//����λ��

#define	DRAW_HEIGHT						34								//�滭�߶�
/////////////////////////////////////////////////////////////////////////
//�ṹ����

//����״̬
struct tagJettonStatus
{
	//������Ϣ
	WORD							wMoveCount;							//�ƶ�����
	WORD							wMoveIndex;							//�ƶ�����

	//������Ϣ
	CPoint							ptFrom;								//����λ��
	CPoint							ptDest;								//Ŀ��λ��
	CPoint							ptCourse;							//����λ��
	SCORE						lGold;								//������Ŀ

	//�ƶ���ʽ
	INT								iMoveType;							//�ƶ���ʽ
};

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//����˵��
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//���ݱ���
protected:
	BYTE							m_cbIndex;							//ţţ����
	WORD							m_wBankerUser;						//ׯ���û�
	SCORE							m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	BYTE							m_bOxValue[GAME_PLAYER];			//ţţ����
	BYTE							m_bOpenCardUser[GAME_PLAYER];		//̯���û�
	BYTE							m_bCallUser;						//�ȴ���ׯ
	BYTE                            m_cbDynamicJoin;                    //��̬����
	bool							m_bInvest;							//�ȴ���ע
	bool							m_bShowSocre[GAME_PLAYER];			//��ʾ����
	bool							m_bUserAction;						//�ȴ���־
	bool							m_bJettonAction;					//�ƶ���ʾ
	tagJettonStatus					m_JettonStatus[GAME_PLAYER];		//������Ϣ
	SCORE							m_lMoveGold[GAME_PLAYER];			//������Ϣ
    

	//��������
public:
	tagHistoryScore	*				m_pHistoryScore[GAME_PLAYER];		//������Ϣ
	bool							m_bShowScore;						//����״̬
	bool							m_bShow;							//��ʾ����
	bool							m_bLookOnUser;						//�Թ۱�־
	bool							m_bOpenCard;						//�ȴ�̯��

	//��������
protected:
	CPoint							m_SendCardPos;						//����λ��
	WORD							m_wSendCount;						//�ƶ�����
	WORD							m_wSendIndex;						//�ƶ�����
	CPoint							m_SendEndingPos[GAME_PLAYER];		//����λ��
	CPoint							m_SendStartPos;						//ʼ��λ��
	CSendCardItemArray				m_SendCardItemArray;				//��������

	//��ť�ؼ�
public:
	CSkinButton						m_btHintOx;							//��ʾ��ť
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOpenCard;						//̯�ư�ť
	CSkinButton						m_btOx;								//ţţ��ť
	CSkinButton						m_btBanker;							//ׯ�Ұ�ť
	CSkinButton						m_btIdler;							//�мҰ�ť
	CSkinButton						m_btScore;							//���ְ�ť
	CSkinButton						m_btYuYin;							//������ť

	//�ؼ�����
public:
	CCardControl					m_CardControl[GAME_PLAYER];			//�û��˿�
	CCardControl					m_CardControlOx[GAME_PLAYER];		//�û��˿�

	CAdminControlDlg				m_AdminDlg;							//����Ա����
	CButton							m_btOpenAdmin;
	//λ����Ϣ
protected:
	CPoint							m_PointBanker[GAME_PLAYER];			//ׯ��λ��
	CPoint							m_ptJeton[GAME_PLAYER];				//���λ��
	CPoint							m_ptValue[GAME_PLAYER];				//����λ��
	CPoint							m_ptOpenTag[GAME_PLAYER];			//̯��λ��
	CPoint							m_ptCard[GAME_PLAYER];				//���λ��
	CSize							m_sizeHistory;						//���ִ�С

	//�������
protected:
	CBitImageEx						m_ImageCard;						//�˿���Դ
	CBitImageEx						m_ImageViewBack;					//������Դ
	CBitImageEx						m_ImageViewCenter;					//������Դ
	CBitImageEx						m_ImageJeton;						//������Դ
	CBitImageEx						m_ImageOpenCard;					//̯����Դ
	CBitImageEx						m_ImageScore;						//������Դ

	CPngImageEx						m_ImageOxValueZero;					//ţֵ��Դ
	CPngImageEx						m_ImageDoulbeOx;					//ţţ��Դ
	CPngImageEx						m_ImageNumber;						//������Դ
	CPngImageEx						m_ImageOxValue;						//ţֵ��Դ
	CPngImageEx						m_ImageWaitCall;					//�ȴ���Դ
	CPngImageEx 					m_ImageTimeBack;					//ʱ�䱳��
	CPngImageEx 					m_ImageTimeNumber;					//ʱ������
	CPngImageEx						m_ImageBanker;						//ׯ����Դ
	CPngImageEx						m_PngUserBack;						//��ҵ׿�
	CPngImageEx						m_PngHistoryScore;					//��ʷ����

	CPngImageEx						m_PngAutoOpen;						//�Զ�����
	CPngImageEx						m_PngWaitBet;						//�ȴ���ע
	CPngImageEx						m_PngWaitOpen;						//�ȴ�����
	CPngImageEx						m_PngOpenTag;						//���Ʊ�ʾ
	CPngImageEx                     m_PngReady;                         //׼����ʶ

	//��ʾ����
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CSize                           m_SizeFace;
	CPoint                          SizeTimeBack;
	
	//12-7-31
	CPngImageEx						m_PngOffLine;
	CPoint							m_ptOffLine[GAME_PLAYER];

public:
	CJettonView						m_JettonView;
	CDFontEx                        m_ViewFontEx;                       //��ͼ����
	         

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//WIN7֧��
public:
	//�Ƿ��������
	virtual bool RealizeWIN7() { return true; }

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//�̳к���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//���ܺ���
public:	
	//�滭ʱ��
	void DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
	//��ȡ�ӿ�
	//IGameFrameEngine *GetGameFrameEngine(){return m_pIGameFrameEngine;}
	//��ʾ��Ϣ
	void SetUserAction(bool bWait);
	//�����ж�
	bool IsDispatchCard();
	//��ɷ���
	void FinishDispatchCard();
	//�����˿�
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);
	//������ע
	void SetUserTableScore(WORD wChairID, SCORE lTableScore,bool bMove=false);
	//ׯ�ұ�־
	void SetBankerUser(WORD wBankerUser);
	//�ȴ���ׯ
	void SetWaitCall(BYTE bCallUser);
	//�ȴ���־
	void SetWaitInvest(bool Invest);
	//���ö�̬�����־
	void SetUserDynamicJoin(BYTE cbDynamicJoin) { m_cbDynamicJoin=cbDynamicJoin; };
	//������Ϣ
	//void SetScoreInfo(LONGLONG lTurnMaxScore,LONGLONG lTurnLessScore);
	//��ʾ����
	void DisplayTypeEx(bool bShow);
	//���õ���
	void SetUserOxValue(WORD wChiarID,BYTE bValue);
	//��ȡ����
	BYTE GetValue(WORD wChiarID){return m_bOxValue[wChiarID];}
	//��ʾ̯��
	void ShowOpenCard(WORD wChiarID,BOOL bShow=TRUE);
	//��ʾ����
	void ShowScore(WORD wChiarID,bool bShow);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
	//���ְ�ť
	VOID OnBnClickedScore();
	//���û���
	bool SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore);
	//���ƶ���
	void DrawMoveAnte( WORD wViewChairID, SCORE lTableScore);
	//�ƶ�����
	bool MoveJetton();
	//�����û�ͼ��
	void DrawUserReadyEx(CDC *pDC,int nXPos,int nYPos);
	
		//����Ա����
	VOID OpenAdminWnd();

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
