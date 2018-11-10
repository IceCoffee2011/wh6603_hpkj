#pragma once

#include "afxwin.h"
#include "ImageButton.h"
#include "HuoDongHuanLeLingDlg.h"
#include "HuoDongZaJinDanDlg.h"
#include "HuoDongDaliBaoDlg.h"
#include "HuodongZhuanzhuan.h"
#include "HuoDongDaili.h"
#define  IDM_RETURN_GAME 11118
//�ר��
class CHuoDongDlg : public CDialog
{
	DECLARE_DYNAMIC(CHuoDongDlg)

public:
	CHuoDongDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHuoDongDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HUODONG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnClose();

	afx_msg void OnBnClickedBtnDalibao();
	afx_msg void OnBnClickedBtnZajindan();
	afx_msg void OnBnClickedBtnHuanlesong();
	afx_msg void OnBnClickedBtnZhuanzhuan();
	afx_msg void OnBnClickedBtnDaili();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	void AdjustCtrls();

private:
	Bitmap*					m_bmpBk;
	Bitmap*					m_bmpTip;
	CImageButton			m_btnDalibao;		//�������ť
	CImageButton			m_btnZaJinDan;		//�ҽ𵰰�ť
	CImageButton			m_btnHuanlesong;	//������
	CImageButton			m_btnZhuanZhuan;	//����תת
	CImageButton			m_btnDaili;			//�������
public:
	CHuoDongDaliBaoDlg		m_dalibao;		//�����
	CHuoDongZaJinDanDlg		m_zajindan;		//�ҽ�
	CHuoDongHuanLeSongDlg	m_huanlesong;	//������
	HuodongZhuanzhuan		m_zhuanzhuan;	//����תת
	CHuoDongDaili			m_daili;		//����
	CImageButton m_btnClose;
};
