#pragma once
#include "Stdafx.h"
#include "Resource.h"

class CJettonView
{
	//��������
protected:
	LONGLONG			m_sCellScore;				//��Ԫ����
	LONGLONG			m_sLimitMax;				//��ע����
	LONGLONG			m_sLimitMin;				//��ע����
	LONGLONG			m_sCheck;					//��ע���
	BOOL				m_bMingZhu;
	//ͼƬ��Դ
protected:
	CPngImage			m_PngBackView;				//����ͼƬ
	CPngImage			m_ImageDrag;				// �϶�ͼƬ
	CFont				m_font;
	COLORREF			m_Color;

	//��ʶ����
protected:
	BOOL				m_bShowView;				//��ʾ����

	//λ�ñ���
protected:
	CPoint				m_ptBasePoint;				//��ʼ����
	CRect				m_rcCheckScore;
	
	//�϶�����
protected:
	bool				m_bMouseDown;			// �����
	int					m_lDragSite;			// �϶�λ��
	CRect				m_rectDrag;				// �϶�����
	CSize				m_sizeDrag;				// �϶�ͼƬ��С


	//��ť�ؼ�
protected:
	CSkinButton			m_JettonButton[6];		//���밴ť

public:
	//���캯��
	CJettonView();
	//��������
	~CJettonView();

	//----------------------------------------------------------------------------------

	//��ʼ��
	BOOL	InitView(CWnd *pParent);
	//�����
	BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	// ��Ϣ����
	BOOL	PreTranslateMessage(MSG * pMsg);
	//���ƽ���
	VOID	DrawView(CWnd * pWnd,CDC * pDC, INT nWidth, INT nHeight);
	//�����ؼ�
	VOID	RectifyControl(INT nWidth, INT nHeight);
	//��ʾ����
	VOID	ShowWindows(BOOL bshow);
	//----------------------------------------------------------------------------------

	//�������
	VOID	EmptyData();
	//������Ϣ
	VOID	SetSocreInfo(LONGLONG slimitmax,LONGLONG slimitmin,BOOL	bMingZhu);
	//��ע����
	VOID	AddScore(LONGLONG saddscore);
	//��ע����
	VOID	MinusScore(LONGLONG sminusscore);
	//��ԭ��С
	VOID	CleanScore();
	//��ʾ����
	CRect	GetRect();
	//���»���
	VOID	UpdataDrag();
	//----------------------------------------------------------------------------------

	//�϶�����
	VOID    SetDragScore();
	//������ע
	VOID	SetCheckScore(LONGLONG scheck);
	//��������
	VOID	SetLimitMaxScore(LONGLONG slimitmax);
	//��������
	VOID	SetLimitMinScore(LONGLONG slimitmin);
	//��Ԫ����
	VOID	SetCellScore(LONGLONG	sCellScore);
	//��ȡ��ע
	LONGLONG	GetCheckScore();
	//��ȡ����
	LONGLONG	GetLimitMaxScore(){return m_sLimitMax;}
	//��ȡ����
	LONGLONG	GetLimitMinScore(){return m_sLimitMin;}

	BOOL	IsWindowVisible(){return m_bShowView;}
	
	VOID UpdateButton(BOOL bShow[4]);

};