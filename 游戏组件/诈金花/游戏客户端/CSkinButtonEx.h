#pragma once
#include "Stdafx.h"

#define IDI_LONG_DOWN   300
#define IDM_LONG_DOWN	WM_USER+300
class CSkinButtonEx:public CSkinButton
{
protected:
	 WORD		m_wID;
public:
	CSkinButtonEx();
	~CSkinButtonEx();
	VOID SetButtonID(WORD wID){m_wID = wID;}
	 //��Ϣ����
protected:
	//����
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//����
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};