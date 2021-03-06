#include "stdafx.h"
#include "GamePlaza.h"
#include "NumerButton.h"
#include "gdipng/MemDC.h"

IMPLEMENT_DYNAMIC(CNumerButton, CButton)

CNumerButton::CNumerButton()
: m_pIamge(NULL)
, m_bHaveBitmaps(false)
// , m_pCurBtn(NULL)
, m_width(0)
, m_height(0)
, m_bIsHovering(false)
, m_bIsTracking(false)
, m_bIsDisabled(false)
, b_select(false)
{

}

CNumerButton::~CNumerButton()
{
// 	if(m_pIamge != NULL) 
// 	{
// 		delete m_pIamge;
// 		m_pIamge = NULL;
// 	}

// 	if (m_dcStd.m_hDC != NULL)
// 	{
// 		m_dcStd.DeleteDC();
// 	}
// 	if (m_dcBk.m_hDC != NULL)
// 	{
// 		m_dcBk.DeleteDC();
// 	}
// 	if (m_dcStdH.m_hDC != NULL)
// 	{
// 		m_dcStdH.DeleteDC();
// 	}
// 	if (m_dcGS.m_hDC != NULL)
// 	{
// 		m_dcGS.DeleteDC();
// 	}
// 	if (m_dcStdSel.m_hDC != NULL)
// 	{
// 		m_dcStdSel.DeleteDC();
// 	}
}

void CNumerButton::SetImage(Bitmap* imageList, int index, int size)
{
	m_pIamge = imageList;
	m_index = index;
	m_width = m_pIamge->GetWidth() / size;
	m_height = m_pIamge->GetHeight() / 4;
}

void CNumerButton::SetSelect(bool bSel)
{
	if(b_select != bSel)
	{
		b_select = bSel;
		Invalidate();
	}
}

BEGIN_MESSAGE_MAP(CNumerButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
//	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE( WM_MOUSEHOVER, OnMouseHover )
	ON_MESSAGE( WM_MOUSELEAVE, OnMouseLeave ) 

END_MESSAGE_MAP()

BOOL CNumerButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CNumerButton::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CButton::OnShowWindow(bShow, nStatus);

	return;
}

void CNumerButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	TCHAR wav_path[MAX_PATH] = _T("");
	CBmpUtil::GetExePath(_T("skin\\game\\MouseDown.wav"), wav_path);
	::PlaySound(wav_path, NULL, SND_ASYNC);

	b_select = !b_select;
	Invalidate();
	return;
}

void CNumerButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);
	CButton::PreSubclassWindow();
}

void CNumerButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	if(m_pIamge == NULL)
		return;

	Gdiplus::Graphics graphics(pDC->m_hDC);

	graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_index*m_width, 0, m_width, m_height, UnitPixel);

	if (b_select) 
	{
		graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height),m_index*m_width, m_height*3, m_width, m_height, UnitPixel);
	}
	else if(m_bIsHovering)
	{
		graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_index*m_width, m_height, m_width, m_height, UnitPixel);
	}
	else
	{
		if (IsWindowEnabled()) {
			graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_index*m_width, 0, m_width, m_height, UnitPixel);
		} else {
			graphics.DrawImage(m_pIamge, Rect(0, 0, m_width, m_height), m_index*m_width, m_height*2, m_width, m_height, UnitPixel);
		}

	}

	//PaintBtn(pDC);
}

// void CNumerButton::PaintBk(CDC *pDC)
// {
// 	CRect rect;
// 	GetClientRect(rect);
// 	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
// }
// 
// void CNumerButton::PaintBtn(CDC *pDC)
// {
// 	CRect rect;
// 	GetClientRect(rect);
// 	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), m_pCurBtn, 0, 0, SRCCOPY);
// }

LRESULT CNumerButton::OnMouseHover(WPARAM wParam,LPARAM lParam)
{

	m_bIsHovering = true;
	Invalidate();
	return 1;
}

LRESULT CNumerButton::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	Invalidate();
	return 1;
}

void CNumerButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bIsTracking = !!_TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}
