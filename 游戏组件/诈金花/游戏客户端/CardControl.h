#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��
#define MAX_PELS					25									//��С����
#define LESS_PELS					10									//��С����
#define DEFAULT_PELS				20									//Ĭ������

//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////

//X ���з�ʽ
enum enXCollocateMode 
{ 
	enXLeft,						//�����
	enXCenter,						//�ж���
	enXRight,						//�Ҷ���
};

//Y ���з�ʽ
enum enYCollocateMode 
{ 
	enYTop,							//�϶���
	enYCenter,						//�ж���
	enYBottom,						//�¶���
};

//////////////////////////////////////////////////////////////////////////

//�˿˿ؼ�
class CCardControl
{
	//���ñ���
public:
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bDisplayHead;						//��ʾ�˿� 
	bool							m_bLookCard;						//���Ʊ�־
	bool							m_bCompareCard;						//���Ʊ�־
	bool							m_bCompareBack;						//���Ʊ���
	DWORD							m_dwCardHSpace;						//������
	WORD							m_wCardColor;						//�˿���ɫ
	WORD							m_wControlID;						//�ؼ�ID
	WORD							m_wMessageID;						//������Ϣ

	//״̬����
protected:
	bool							m_bCaptureMouse;					//�����־
	CWHArray<BYTE>					m_CardDataArray;					//�˿�����

	//λ����Ϣ
protected:
	CRgn							m_CardRegion;						//�˿�����
	CPoint							m_BenchmarkPos;						//��׼λ��
	enXCollocateMode				m_XCollocateMode;					//��ʾģʽ
	enYCollocateMode				m_YCollocateMode;					//��ʾģʽ

	//��̬����
protected:
	static bool						m_bLoad;							//��ʼ��־
	static CSize					m_CardSize;							//�˿˴�С
	static CPngImage				m_ImageCard;						//ͼƬ��Դ

	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//���ƺ���
public:
	//���ü��
	void SetCardSpace(DWORD dwCardSpace);
	//������Ӧ
	void SetPositively(bool bPositively);
	//��ʾ�˿�
	void SetDisplayHead(bool bDisplayHead);
	//��׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��ȡλ��
	CPoint GetCardPos();

	int GetBenchmarkPosX(){return m_BenchmarkPos.x;}
	int GetBenchmarkPosY(){return m_BenchmarkPos.y;}

	//���ܺ���
public:
	//��ȡ��Ŀ
	DWORD GetCardCount();
	//�����˿�
	DWORD GetCardData(BYTE cbCardData[], DWORD dwMaxCount);
	//�����˿�
	DWORD SetCardData(const BYTE bCardData[], DWORD dwCardCount);
	//�����˿�
	void SetCardColor(WORD wColor);
	//�����˿�
	void SetLookCard(bool bLook);
	//��ȡ����
	bool GetLookCard(){return m_bLookCard;}
	//�����˿�
	void SetCompareCard(bool bCompare);
	//�����˿�
	void SetCommandID(WORD ControlID,WORD wMessageID);
	//�����˿�
	void SetCompareBack(bool bShow);

	BOOL OnClickJudge(CPoint Point);

	//��Ϣ����
public:
	//�ػ�����
	void DrawCardControl(CDC *pDC);



};

//////////////////////////////////////////////////////////////////////////

#endif
