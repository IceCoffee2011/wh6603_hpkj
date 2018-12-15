#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"
#include ".\cardcontrol.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
bool					CCardControl::m_bLoad=false;					//��ʼ��־
CSize					CCardControl::m_CardSize;						//�˿˴�С
CPngImage				CCardControl::m_ImageCard;						//�˿�ͼƬ

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//���ñ���
	m_bPositively=false;
	m_bDisplayHead=false;
	m_bLookCard=false;
	m_bCompareCard=false;
	m_bCompareBack=false;
	m_wCardColor=INVALID_CHAIR;
	m_dwCardHSpace=DEFAULT_PELS;
	m_wControlID=INVALID_CHAIR;
	m_wMessageID=INVALID_CHAIR;

	//״̬����
	m_bCaptureMouse=false;

	//λ�ñ���
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;

	//��Դ����
	if (m_bLoad==false)
	{
		//������Դ
		m_bLoad=true;
		m_ImageCard.LoadImage(AfxGetInstanceHandle(),TEXT("CARD"));

		//��ȡ��С
		m_CardSize.cx=m_ImageCard.GetWidth()/13;
		m_CardSize.cy=m_ImageCard.GetHeight()/5;
	}

	return;
}

//��������
CCardControl::~CCardControl()
{
}


//�ػ�����
void CCardControl::DrawCardControl(CDC *pDC)
{

	//�滭�˿�
	DWORD dwXImagePos,dwYImagePos;
	DWORD dwXScreenPos,dwYScreenPos;
	for (INT_PTR i=0;i<m_CardDataArray.GetCount();i++)
	{
		//��������
		BYTE cbCardData=m_CardDataArray[i];
		bool bDrawCard=(m_bDisplayHead==true);

		//ͼƬλ��
		if(m_wCardColor!=INVALID_CHAIR)
		{
			//��������
			if(m_wCardColor==1)
			{
				dwXImagePos=m_CardSize.cx*3;
			}
			//���Ʊ���
			else if(m_wCardColor==2)
			{
				dwXImagePos=m_CardSize.cx*4;
			}
			//���Ƹ���
			else if(m_wCardColor==0)
			{
				dwXImagePos=m_CardSize.cx*2;
			}
			else if(m_wCardColor==3)
			{
				dwXImagePos=m_CardSize.cx*5;
			}
			else if(m_wCardColor==4)
			{
				dwXImagePos=m_CardSize.cx*0;
			}
			else if(m_wCardColor==5)
			{
				dwXImagePos=m_CardSize.cx*1;
			}
			dwYImagePos=m_CardSize.cy*4;
		}
		else if ((bDrawCard==true)&&(cbCardData!=0))
		{
			dwXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
			dwYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
		}
		else
		{
			if(!m_bCompareBack)		//����&���Ʊ���
			{
				dwXImagePos=m_CardSize.cx*((m_bLookCard)?6:2);
			}
			else					//���Ʊ���
			{
				dwXImagePos=m_CardSize.cx*5;
			}
			dwYImagePos=m_CardSize.cy*4;
		}

		//��Ļλ��
		dwYScreenPos=0;
		dwXScreenPos=m_dwCardHSpace*i;

		//�滭�˿�
		m_ImageCard.DrawImage(pDC,m_BenchmarkPos.x+dwXScreenPos,m_BenchmarkPos.y+dwYScreenPos,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos);
	}


	return;
}

//��ȡ��Ŀ
DWORD CCardControl::GetCardCount()
{
	return (WORD)m_CardDataArray.GetCount();
}

//��ʾ�˿�
void CCardControl::SetDisplayHead(bool bDisplayHead)
{
	//״̬�ж�
	if (m_bDisplayHead==bDisplayHead) return;

	//���ñ���
	m_bDisplayHead=bDisplayHead;

	return;
}

//���ü��
void CCardControl::SetCardSpace(DWORD dwCardSpace)
{
	//���ñ���
	m_dwCardHSpace=dwCardSpace;

	return;
}

//������Ӧ
void CCardControl::SetPositively(bool bPositively)
{
	//���ñ���
	m_bPositively=bPositively;

	return;
}

//�����˿�
DWORD CCardControl::GetCardData(BYTE cbCardData[], DWORD dwMaxCount)
{
	//�����˿�
	DWORD dwCopyCount=__min((DWORD)m_CardDataArray.GetCount(),dwMaxCount);
	CopyMemory(cbCardData,m_CardDataArray.GetData(),sizeof(BYTE)*dwCopyCount);

	return dwCopyCount;
}

//�����˿�
DWORD CCardControl::SetCardData(const BYTE cbCardData[], DWORD dwCardCount)
{
	//�����˿�
	m_CardDataArray.SetSize(dwCardCount);
	CopyMemory(m_CardDataArray.GetData(),cbCardData,sizeof(BYTE)*dwCardCount);

	return dwCardCount;
}

//�����˿�
void CCardControl::SetCardColor(WORD wColor)
{
	m_wCardColor=wColor;
	return;
}

//�����˿�
void CCardControl::SetLookCard(bool bLook)
{
	m_bLookCard=bLook;

	return;
}

//�����˿�
void CCardControl::SetCompareCard(bool bCompare)
{
	m_bCompareCard=bCompare;

	return ;
}

//�����˿�
void CCardControl::SetCompareBack(bool bShow)
{
	m_bCompareBack=bShow;
	return ;
}

//�����˿�
void CCardControl::SetCommandID(WORD ControlID,WORD wMessageID)
{
	m_wMessageID=wMessageID;
	m_wControlID=ControlID;
	return ;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	return;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;
	return;
}

//��ȡλ��
CPoint CCardControl::GetCardPos()
{
	//��������
	DWORD dwCardCount=(DWORD)m_CardDataArray.GetCount();

	//�����С
	CSize ControlSize;
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;

	//����λ��
	int nXPos=0;
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ nXPos=m_BenchmarkPos.x; break; }
	case enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.cx/2; break; }
	case enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.cx; break; }
	}

	//����λ��
	int nYPos=0;
	switch (m_YCollocateMode)
	{
	case enYTop:	{ nYPos=m_BenchmarkPos.y; break; }
	case enYCenter: { nYPos=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { nYPos=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	CPoint cpTemp;
	cpTemp.x=nXPos;
	cpTemp.y=nYPos;

	return cpTemp;
}

BOOL CCardControl::OnClickJudge(CPoint Point)
{
	if(m_wControlID == INVALID_CHAIR || m_bCompareCard == false) 
		return FALSE;

	DWORD dwCardCount=(DWORD)m_CardDataArray.GetCount();

	//�����С
	CSize ControlSize;
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;

	CRect rc(m_BenchmarkPos.x,m_BenchmarkPos.y,m_BenchmarkPos.x+m_CardSize.cx,m_BenchmarkPos.y+m_CardSize.cy);
	return rc.PtInRect(Point);
}


//������
//void CCardControl::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	if(m_wControlID!=INVALID_CHAIR && m_bCompareCard)
//	{
//		//�û�ID
//		//TCHAR obj[32]=TEXT("");
//		//_sntprintf(obj,sizeof(obj),TEXT("%I64d:%I64d"),m_wMessageID,m_wControlID);
//		//MessageBox(obj);
//		CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
//		if(pGameFrameEngine != NULL)pGameFrameEngine->SendMessage(m_wMessageID,m_wControlID,m_wControlID);
//	}
//
//}
//////////////////////////////////////////////////////////////////////////

