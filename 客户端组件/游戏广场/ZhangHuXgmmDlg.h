#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#include "MessageDlg.h"
//�޸�����
class CZhangHuXgmmDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhangHuXgmmDlg)

public:
	CZhangHuXgmmDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZhangHuXgmmDlg();

// �Ի�������
	enum { IDD = IDD_DLG_ZHANGHU_XGMM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnOk2();
	afx_msg void OnBnClickedBtnLockMachine();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

private:
	void AdjustCtrls();

private:
	Bitmap* m_bmpBk;
	DWORD m_dwTickCount;
public:
	CString m_old_pwd;
	CString m_new_pwd;
	CString m_confirm_pwd;

	CSkinEditEx m_editOldPwd;
	CSkinEditEx m_editNewPwd;
	CSkinEditEx m_editConfirmPwd;

	CImageButton m_btnDlOK;

	CString m_old_pwd_qk;
	CString m_new_pwd_qk;
	CString m_confirm_pwd_qk;
	CSkinEditEx m_editOldPwdQk;
	CSkinEditEx m_editNewPwdQk;
	CSkinEditEx m_editConfirmPwdQk;
	CImageButton m_btnQkOK;

	CImageButton m_btnLockMachine;
	CFont m_font;

protected:
	virtual void OnCancel();
	virtual void OnOK();
	//���غ���
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bAlterLogonPass;						//��ȡ��־����
	bool							m_bAlterQukuanPass;							//��ȡ��־
protected:
	VOID SendToServer(int nSendType);

};
