// AutoUpdateDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SkinProgressCtrl.h"
#include "SkinButton.h"
#include "SkinStatic.h"

// CAutoUpdateDlg dialog
class CAutoUpdateDlg : public CDialog, public IDownloadSink
{
// Implementation
protected:
	HICON					m_hIcon;				//ͼ����
	CString					m_strCurrentFile;		//��ǰ�ļ�
	CSkinProgressCtrl		m_Progress1;			//���ȿؼ�
	CSkinProgressCtrl		m_Progress2;			//���ȿؼ�
	CSkinStatic				m_PercentText;			//�ٷֱ��ı�
	CSkinStatic				m_StatusText;			//״̬�ı�
	CSkinStatic				m_CountText;            //��С 
	CSkinStatic				m_CountText2;           //������
	CSkinButton				m_btCancel;				//������ť
	CSkinButton				m_btRetry;				//���԰�ť
	CSkinButton				m_btBySelf;				//�ֶ�����

public:
	CDownloadMgr			m_DownloadMgr;			//���ع���
	CString					m_strCurrentDir;		//��ǰĿ¼
	ULONG					m_ulCurentLenght;		//��ǰ����
	ULONG					m_ulTotalLength;		//�������

	// add by hxh 20160607
	int                     m_status;//����״̬ 0:׼������;1:��������;2:�������

public:
	CDC*					m_pMemDC;
	CBitmap*				m_pOldBitmap;
	CBitmap*				m_pBitmapMem;
	CBitmap					m_bmBack;
    CPngImage               m_BackImage;
    CPngImage               m_StatusImage[3];
    CFont                   m_font;
    CFont                   m_font2;
	//��׼����
public:
	CAutoUpdateDlg(CWnd* pParent = NULL);
	virtual ~CAutoUpdateDlg();
	// ����PNGͼƬ
	VOID  LoadPngImage();
	//���غ���
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	//�ӿں���
public:
	virtual void OnStatusCallback(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCTSTR pszStatusText);

protected:
	BOOL CheckVersionUpdate(CString& strUpdateURL);
	void BuildBkDC();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRetry();
	afx_msg void OnBnClickedSelf();
	afx_msg void OnClose();
	afx_msg LRESULT OnTransaprentBk( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
