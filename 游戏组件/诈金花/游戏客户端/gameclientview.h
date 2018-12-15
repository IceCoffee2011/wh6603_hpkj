#pragma once

#include "Stdafx.h"

#include "CardControl.h"
#include "JettonControl.h"

#include "CJettonView.h"
#include "AdminControlDlg.h"
//////////////////////////////////////////////////////////////////////////
//��Ϣ����

//��Ϣ����
#define IDM_START					WM_USER+100							//��ʼ��Ϣ
#define IDM_ADD_SCORE				WM_USER+101							//��ע��Ϣ
#define	IDM_CONFIRM					WM_USER+109							//ȷ����Ϣ	
#define	IDM_CANCEL_ADD				WM_USER+119							//ȡ����Ϣ	

#define IDM_LOOK_CARD				WM_USER+104							//������Ϣ
#define IDM_COMPARE_CARD			WM_USER+105							//������Ϣ
#define IDM_OPEN_CARD				WM_USER+106							//������Ϣ
#define IDM_GIVE_UP					WM_USER+107							//������Ϣ
#define IDM_SEND_CARD_FINISH		WM_USER+108							//�������
#define IDM_COMPARE_USER			WM_USER+110							//��ѡ�û�
#define IDM_FALSH_CARD_FINISH		WM_USER+114							//�������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagSendCardItem
{
	WORD							wMoveIndex;
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
	WORD							m_wBankerUser;						//ׯ���û�
	LONGLONG						m_lMaxCellScore;					//�����ע
	LONGLONG						m_lCellScore;						//������ע
	LONGLONG						m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	LONGLONG						m_lMaxScore;						//�ⶥ����
	bool							m_bInvest;							//�ȴ���ע
	bool							m_bCompareCard;						//ѡ�ȱ�־
	WORD							m_wWaitUserChoice;					//�ȴ���־
	bool							m_bSystemOpen;						//ϵͳ����
	LONGLONG						m_wCurrentTimes;					//��ǰ����
	//��������
public:
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//��Ϸ״̬
	bool							m_bShow;							//��ʾ����
	TCHAR							m_tcBuffer[GAME_PLAYER][64];		//�Ƶ�����

	//���Ʊ���
protected:
	WORD							m_wConmareIndex;					//��������
	WORD							m_wConmareCount;					//������Ŀ
	WORD							m_wCompareChairID[2];				//�����û�

	WORD							m_wButtonDownUser;

	//���Ʊ���
protected:
	bool							m_bFalsh;							//���Ʋ���
	WORD							m_wFlashUser[GAME_PLAYER];			//�����û�
	WORD							m_wFalshCount;						//���ƴ���

	//������־
protected:
	bool							m_bCartoon;							//������־
	WORD							m_wLoserUser;						//���ưܻ�
	bool							m_bStopDraw;						//���Ʊ�־
	CSendCardItemArray				m_SendCardItemArray;				//��������

	//λ�ñ���
protected:
	CPoint							m_ptUserCompare;					//����λ��
	CPoint							m_SendCardPos;						//����λ��
	CPoint							m_ptUserCard[GAME_PLAYER];			//����λ��
	CPoint							m_KeepPos;							//����λ��

	CPoint							m_ptUserInfo[GAME_PLAYER];
	CPoint							m_ptLevel[GAME_PLAYER];
	CPoint							m_ptScore[GAME_PLAYER];

	INT								m_nXFace;
	INT								m_nYFace;
	INT								m_nXTimer;
	INT								m_nYTimer;
	//�û�״̬
protected:
	BOOL							m_bCompareUser[GAME_PLAYER];		//��ѡ�����û�

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btCancel;							//����ť
	CSkinButton						m_btConfirm;						//ȷ�ϰ�ť
	CSkinButton						m_btAddScore;						//��ע��ť
	CSkinButton						m_btFollow;							//��ע��ť
	CSkinButton						m_btGiveUp;							//������ť
	CSkinButton						m_btLookCard;						//���ư�ť
	CSkinButton						m_btCompareCard;					//���ư�ť
	CSkinButton						m_btOpenCard;						//���ư�ť

	//�ؼ�����
public:
	CCardControl					m_CardControl[GAME_PLAYER];			//�û��˿�
	CJettonControl					m_JettonControl;					//����ؼ�
public:
	CAdminControlDlg				m_AdminDlg;							//����Ա����
	CButton							m_btOpenAdmin;

	//λ����Ϣ
protected:
	CPoint							m_PointBanker[GAME_PLAYER];			//ׯ��λ��
	CPoint							m_ptJettons[GAME_PLAYER];			//����λ��

	//�������
protected:
	//CBitImage						m_ImageBanker;						//ׯ����Դ
	//CBitImage						m_ImageCard;						//�˿���Դ
	CBitImage						m_ImageTitle;						//������Դ
	CBitImage						m_ImageViewBack;					//������Դ
	CBitImage						m_ImageViewCenter;					//������Դ
	CBitImage						m_ImageJeton;						//������Դ
	CBitImage						m_ImageArrowhead;					//��ͷ��Դ
	CBitImage						m_ImageReady;						//׼����Դ
	CBitImage						m_ImagePocket;						//�����Դ
	CBitImage						m_ImageBalance;						//������Դ


	CPngImage						m_ImageBanker;
	CPngImage						m_ImageCard;
	CPngImage						m_ImageUserInfo;
	CPngImage						m_ImageTime;
	
	CPngImage						m_ImgaeScoreBackR;					// ��������
	CPngImage						m_ImgaeScoreBackL;					// ��������
	CPngImage						m_ImgaeScoreBackM;					// ��������
	CPngImage						m_ImgaeScoreWin;					// ��������
	CPngImage						m_ImgaeScoreLose;					// ��������
	CPngImage						m_ImgaeScoreAll;

	LONGLONG						m_lEndScore[GAME_PLAYER];
	CPoint							m_ptGameEndScore[GAME_PLAYER];

	//12-7-31
	CPngImage						m_PngOffLine;
	CPoint							m_ptOffLine[GAME_PLAYER];

public:
	CJettonView						m_JettonView;
	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
	//WIN7֧��
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
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);

public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//���ܺ���
public:
	//������ע
	void SetUserTableScore(WORD wChairID, LONGLONG lTableScore,LONGLONG lCurrentScore);
	//��ʼ���붯��
	void BeginMoveJettons();
	//ֹͣ���붯��
	void StopMoveJettons();
	//ʤ�����
	void SetGameEndInfo( WORD wWinner );
	//�ȴ�ѡ��
	void SetWaitUserChoice(WORD wChoice);
	//ׯ�ұ�־
	void SetBankerUser(WORD wBankerUse);
	//������Ϣ
	void SetScoreInfo(LONGLONG lMaxCellScore,LONGLONG lCellScore);
	//ѡ�ȱ�־
	void SetCompareCard(bool bCompareCard,BOOL bCompareUser[]);
	//ֹͣ����
	void StopUpdataScore(bool bStop);
	//�����ж�
	bool IsDispatchCard(){return m_bCartoon;}
	//��ȡ��Ϣ
	bool GetCompareInfo(){return m_bCompareCard;};
	// ��������
	void SetGameEndScore(LONGLONG score[GAME_PLAYER]);
	// ϵͳ����
	void SetSystemOpen(bool bSystem);
	// ���÷ⶥ
	void SetMaxScore(LONGLONG lScore){m_lMaxScore = lScore;}
	// ��ǰ����
	void SetCurrentTimes(LONGLONG wTimes){m_wCurrentTimes = wTimes;}
	
	//����Ա����
	VOID OpenAdminWnd();

	//��������
public:
	//���ƶ���
	void PerformCompareCard(WORD wCompareUser[2], WORD wLoserUser);
	//���ƶ���
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);
	//���ƶ���
	bool bFalshCard(WORD wFalshUser[]);
	//���ƴ���
	bool SendCard();
	//���ƴ���
	bool FlashCard();
	//���ƴ���
	bool CompareCard();
	//ֹͣ����
	void StopFlashCard();
	//ֹͣ����
	void StopCompareCard();
	//��ɷ���
	void FinishDispatchCard();
	//������ͼ
	void RefreshGameView();
	// ʱ�ӻ���
	void DrawUserClockEx(CDC *pDC,INT nXPos, INT nYPos,WORD wChairID);
	//�滭�˿�
	void DrawCompareCard(CDC * pDC, INT nXPos, INT nYPos, bool bChapped);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,int nWidth, int nWeight,UINT nFormat=DT_LEFT);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect,int nWidth, int nWeight,UINT nFormat=DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	// �滭��������
	void DrawOverScore(CDC * pDC, SCORE lOutNum, INT nXPos, INT nYPos );
	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
