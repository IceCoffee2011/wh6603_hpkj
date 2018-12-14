#pragma  once
#include "Stdafx.h"


#define IDC_ONE_SCORE					121								//��ע��ť
#define IDC_TWO_SCORE					122								//��ע��ť
#define IDC_THREE_SCORE					123								//��ע��ť
#define IDC_FOUR_SCORE					124								//��ע��ť
#define IDC_FIVE_SCORE					125								//��ע��ť
#define IDC_ACCEPT_SCORE				126								//��ע��ť
#define IDC_CLEAN_SCORE					127								//�����ť
#define IDM_ADD_SCORE				(WM_USER+110)						//��ע��Ϣ
class CJettonView
{
protected:
	BOOL			m_bShow;
	CPoint			m_ptBasePoint;
protected:
	CSkinButton		m_btJetton[5];
	CSkinButton		m_btAccept;
	CSkinButton		m_btClean;
	CBitImage		m_ImageScoreBack;
	
protected:
	SCORE	m_sBetScore;
	SCORE	m_sCellScore;
	SCORE	m_sMaxCellScore;
public:
	CJettonView();
	~CJettonView();

public :
	//��������
	VOID	InitView(CWnd	*pWnd);
	//�����ؼ�
	VOID	RectifyControl(int nWidth, int nHeight);
	//��ť��Ϣ
	BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	//������ʾ
	VOID	ShowWindows(BOOL bShow);
	//����
	VOID	DrawJettonView(CDC *pDC,CDFontEx * pViewFont);
	//��ע����
	VOID	SetCellScore(SCORE score);
	//�������
	VOID	SetMaxScore(SCORE score);
	//��ȡ��ע
	SCORE	GetBetScore();
	//��������
	VOID	RestData();
	//��ע
	VOID	AddScore(SCORE score);
	//ˢ������
	CRect	GetRect();
	//��ʾ״̬
	BOOL	IsWindowVisible(){return m_bShow;}

	VOID UpDateButton();

	VOID OnCleanScore();

};