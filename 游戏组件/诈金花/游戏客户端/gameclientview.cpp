#include "StdAfx.h"
#include "Resource.h"
#include "GameClientEngine.h"
#include "GameClientView.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ť��ʶ
#define IDC_START						100								//��ʼ��ť
#define IDC_MAX_SCORE					104								//���ť
#define IDC_MIN_SCORE					105								//���ٰ�ť
#define IDC_CONFIRM						106								//ȷ����ť
#define	IDC_CANCEL_ADD					119								//ȡ����ť
#define IDC_ADD_SCORE					110								//��ע��ť
#define IDC_LOOK_CARD					111								//������Ϣ
#define IDC_COMPARE_CARD				112								//������Ϣ
#define IDC_FOLLOW						113								//������Ϣ
#define IDC_GIVE_UP						114								//������Ϣ
#define	IDC_OPEN_CARD					115								//������Ϣ

//��ʱ����ʶ
#define IDI_CARTOON						99								//������ʱ��
#define	TIME_CARTOON					100								//����ʱ��
#define IDI_MOVE_JETTON					101								//�ƶ����붨ʱ��

//�����ٶ�  //(SPEED)10 (TIME_CARTOON)70 /CPU UP
#define	SPEED							4								//�����ٶȲ���
#define	ROCK							10								//�񶯸���
#define TIME_MOVE_JETTON				20								//�ƶ�����ʱ��
#define TIME_MOVE_NUMBER				50								//��������ʱ��

//λ�ñ���
#define	MY_SELF_CHAIRID					2								//�Լ�λ��

//���Ʊ���
#define VSCARD_TIME						3								//���ƴ���
#define VSCARD_COUNT					6								//������Ŀ
#define COMPARE_RADII					130								//���ƿ��

//���Ʊ���
#define SEND_PELS						30								//��������
#define LESS_SEND_COUNT					2								//���ʹ���
//��ť��ʶ
#define IDC_BT_ADMIN					221
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)							  
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	
		//��ť��Ϣ
	ON_BN_CLICKED(IDC_BT_ADMIN,OpenAdminWnd)
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_wFlashUser[i]=INVALID_CHAIR;	
	}
	m_lMaxScore=0;
	m_bCartoon =false;
	m_SendCardPos.SetPoint(0,0);
	m_SendCardItemArray.RemoveAll();
	m_bSystemOpen=false;
	//���Ʊ���
	m_wConmareIndex=0;
	m_wConmareCount=0;
	m_wLoserUser=INVALID_CHAIR;
	ZeroMemory(m_wCompareChairID,sizeof(m_wCompareChairID));
	m_wCurrentTimes = 1;

	//���ݱ���
	ZeroMemory(m_tcBuffer,sizeof(m_tcBuffer));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bCompareUser,sizeof(m_bCompareUser));
	m_wBankerUser=INVALID_CHAIR;
	m_lMaxCellScore=0;
	m_lCellScore=0;	
	m_wFalshCount=0;
	m_bStopDraw=false;
	m_bFalsh=false;
	m_bInvest=false;		
	m_bShow=false;
	m_bCompareCard=false;
	m_wWaitUserChoice=INVALID_CHAIR;
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewBack.LoadFromResource(hInstance,IDB_VIEW_BACK);
	m_ImageViewCenter.LoadFromResource(hInstance,IDB_VIEW_FILL);
	m_ImageArrowhead.LoadFromResource(hInstance,IDB_ARROWHEAD);
	m_ImageBanker.LoadImage(hInstance,TEXT("BANKER_FLAG"));
	m_ImageCard.LoadImage(hInstance,TEXT("CARD"));
	m_ImageUserInfo.LoadImage(hInstance,TEXT("USERINFO"));
	m_ImageTime.LoadImage(hInstance,TEXT("TIME"));

	m_ImgaeScoreBackR.LoadImage(hInstance,TEXT("SCORE_BACK_R"));
	m_ImgaeScoreBackL.LoadImage(hInstance,TEXT("SCORE_BACK_L"));
	m_ImgaeScoreBackM.LoadImage(hInstance,TEXT("SCORE_BACK_M"));
	m_ImgaeScoreWin.LoadImage(hInstance,TEXT("SCORE_NUM_WIN"));
	m_ImgaeScoreLose.LoadImage(hInstance,TEXT("SCORE_NUM_LOSE"));
	m_ImgaeScoreAll.LoadImage(hInstance,TEXT("ALLSCORE_NUM"));

	m_PngOffLine.LoadImage(hInstance,TEXT("OFF_LINE"));

	m_nXFace=48;
	m_nYFace=48;
	m_nXTimer=65;
	m_nYTimer=69;



	m_wButtonDownUser = INVALID_CHAIR;

	ZeroMemory(m_lEndScore,sizeof(m_lEndScore));

	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	
	AfxSetResourceHandle(AfxGetInstanceHandle());

	//�����ؼ�
	CRect rcCreate(0,0,0,0);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_START);
	m_btConfirm.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_CONFIRM);
	m_btCancel.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_CANCEL_ADD);
	m_btOpenCard.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OPEN_CARD);

	m_btAddScore.Create(TEXT(""),WS_CHILD|WS_DISABLED,rcCreate,this,IDC_ADD_SCORE);
	m_btCompareCard.Create(TEXT(""),WS_CHILD|WS_DISABLED,rcCreate,this,IDC_COMPARE_CARD);
	m_btGiveUp.Create(TEXT(""),WS_CHILD|WS_DISABLED,rcCreate,this,IDC_GIVE_UP);
	m_btLookCard.Create(TEXT(""),WS_CHILD|WS_DISABLED,rcCreate,this,IDC_LOOK_CARD);
	m_btFollow.Create(TEXT(""),WS_CHILD|WS_DISABLED,rcCreate,this,IDC_FOLLOW);

	//m_GoldControl.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,CRect(50,50,66,66),this,8);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
	m_btAddScore.SetButtonImage(IDB_ADD_SCORE,hInstance,false,false);
	m_btConfirm.SetButtonImage(IDB_CONFIRM,hInstance,false,false);
	m_btCancel.SetButtonImage(IDB_CANCLE,hInstance,false,false);
	m_btCompareCard.SetButtonImage(IDB_COMPARE_CARD,hInstance,false,false);
	m_btOpenCard.SetButtonImage(IDB_OPEN_CARD,hInstance,false,false);
	m_btGiveUp.SetButtonImage(IDB_GIVE_UP,hInstance,false,false);
	m_btLookCard.SetButtonImage(IDB_LO0K_CARD,hInstance,false,false);
	m_btFollow.SetButtonImage(IDB_FOLLOW,hInstance,false,false);

	//�����ؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_CardControl[i].SetCommandID(i,IDM_COMPARE_USER);
	}

	m_JettonView.InitView(this);
	
	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_BT_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);
	
	return 0;
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_START:				//��ʼ��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_START,0,0);
			return TRUE;
		}
	case IDC_ADD_SCORE:			//��ע��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_ADD_SCORE,0,0);
			return TRUE;
		}
	case IDC_CONFIRM:			//ȷ����ť
		{
			//������Ϣ
			SendEngineMessage(IDM_CONFIRM,0,0);
			return TRUE;
		}
	case IDC_CANCEL_ADD:		//ȡ����ť
		{
			//������Ϣ
			SendEngineMessage(IDM_CANCEL_ADD,0,0);
			return TRUE;
		}
	case IDC_FOLLOW:			//��ע��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_CONFIRM,1,1);
			return TRUE;
		}
	case IDC_MIN_SCORE:			//���ٰ�ť
		{
			//������Ϣ
			//SendEngineMessage(IDM_MIN_SCORE,0,0);
			return TRUE;
		}
	case IDC_MAX_SCORE:			//���ť
		{
			//������Ϣ
			//SendEngineMessage(IDM_MAX_SCORE,0,0);
			return TRUE;
		}
	case IDC_COMPARE_CARD:		//���ư�ť
		{
			//������Ϣ
			SendEngineMessage(IDM_COMPARE_CARD,0,0);
			return TRUE;
		}
	case IDC_LOOK_CARD:			//���ư�ť
		{
			//������Ϣ
			SendEngineMessage(IDM_LOOK_CARD,0,0);
			return TRUE;
		}
	case IDC_OPEN_CARD:			//������Ϣ
		{
			//������Ϣ
			SendEngineMessage(IDM_OPEN_CARD,0,0);
			return TRUE;
		}
	case IDC_GIVE_UP:			//������ť
		{
			//������Ϣ
			SendEngineMessage(IDM_GIVE_UP,0,0);
			return TRUE;
		}
	}

	if(m_JettonView.OnCommand(wParam,lParam)) 
	{
		InvalidGameView(0,0,0,0);
		return TRUE;
	}
	return __super::OnCommand(wParam, lParam);
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	//ɾ��ʱ��
	KillTimer(IDI_CARTOON);
	KillTimer(IDI_MOVE_JETTON);
	m_lMaxScore=0;
	m_bSystemOpen=false;
	m_wCurrentTimes = 1;

	//��������
	m_bCartoon =false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_wFlashUser[i]=INVALID_CHAIR;
	}
	m_SendCardPos.SetPoint(m_KeepPos.x,m_KeepPos.y);
	m_SendCardItemArray.RemoveAll();

	//���Ʊ���
	m_wConmareIndex = 0;
	m_wConmareCount = 0;
	m_wLoserUser=INVALID_CHAIR;
	ZeroMemory(m_wCompareChairID,sizeof(m_wCompareChairID));

	//���ݱ���
	ZeroMemory(m_tcBuffer,sizeof(m_tcBuffer));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	m_wBankerUser=INVALID_CHAIR;			
	ZeroMemory(m_bCompareUser,sizeof(m_bCompareUser));
	m_lMaxCellScore=0;					
	m_lCellScore=0;		
	m_wFalshCount=0;
	m_bStopDraw=false;
	m_bFalsh=false;
	m_bInvest=false;
	m_bShow=false;
	m_bCompareCard=false;
	m_wWaitUserChoice=INVALID_CHAIR;
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//����ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btCancel.ShowWindow(SW_HIDE);
	m_btConfirm.ShowWindow(SW_HIDE);
	//m_btMaxScore.ShowWindow(SW_HIDE);
	//m_btMinScore.ShowWindow(SW_HIDE);
	m_btOpenCard.ShowWindow(SW_HIDE);
	m_btAddScore.EnableWindow(FALSE);
	m_btCompareCard.EnableWindow(FALSE);
	m_btGiveUp.EnableWindow(FALSE);
	m_btLookCard.EnableWindow(FALSE);
	m_btFollow.EnableWindow(FALSE);

	//���ؿؼ�
	m_JettonControl.ResetControl();

	for (WORD i=0;i<GAME_PLAYER;i++) m_CardControl[i].SetCardData(NULL,0);

	m_wButtonDownUser = INVALID_CHAIR;

	ZeroMemory(m_lEndScore,sizeof(m_lEndScore));

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	
	m_KeepPos.x=nWidth/2;
	m_KeepPos.y=5;

	m_ptUserInfo[0].SetPoint(nWidth/2+225 ,nHeight/2-325);
	m_ptUserInfo[1].SetPoint(nWidth/2+325 ,nHeight/2+115);
	m_ptUserInfo[2].SetPoint(nWidth/2-75,nHeight/2+160);
	m_ptUserInfo[3].SetPoint(nWidth/2-480,nHeight/2+115);
	m_ptUserInfo[4].SetPoint(nWidth/2-395,nHeight/2-325);

	m_ptOffLine[0].SetPoint(m_ptUserInfo[0].x ,m_ptUserInfo[0].y+90);
	m_ptOffLine[1].SetPoint(m_ptUserInfo[1].x ,m_ptUserInfo[1].y+90);
	m_ptOffLine[2].SetPoint(m_ptUserInfo[2].x ,m_ptUserInfo[2].y+90);
	m_ptOffLine[3].SetPoint(m_ptUserInfo[3].x ,m_ptUserInfo[3].y+90);
	m_ptOffLine[4].SetPoint(m_ptUserInfo[4].x ,m_ptUserInfo[4].y+90);

	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_ptAvatar[i].SetPoint(m_ptUserInfo[i].x+14,m_ptUserInfo[i].y+13);
		m_ptNickName[i].SetPoint(m_ptUserInfo[i].x+72,m_ptUserInfo[i].y+10);
		m_ptClock[i].SetPoint(m_ptUserInfo[i].x+13,m_ptUserInfo[i].y+70);
		m_ptLevel[i].SetPoint(m_ptUserInfo[i].x+72,m_ptUserInfo[i].y+32);
		m_ptScore[i].SetPoint(m_ptUserInfo[i].x+72,m_ptUserInfo[i].y+30);
	}

	//׼��λ��
	m_ptReady[0].x=nWidth/2+205;
	m_ptReady[0].y=nHeight/2-180;
	m_ptReady[1].x=nWidth/2+205;
	m_ptReady[1].y=nHeight/2+35;
	m_ptReady[2].x=nWidth/2+10;
	m_ptReady[2].y=nHeight/2+140;
	m_ptReady[3].x=nWidth/2-205;
	m_ptReady[3].y=nHeight/2+35;
	m_ptReady[4].x=nWidth/2-205;
	m_ptReady[4].y=nHeight/2-180;

	//�û��˿�
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(i==0||i==4)
		{
			m_CardControl[i].SetBenchmarkPos(m_ptUserInfo[i].x,m_ptUserInfo[i].y+m_ImageUserInfo.GetHeight()+10,enXCenter,enYCenter);
			m_ptUserCard[i].SetPoint(m_ptUserInfo[i].x,m_ptUserInfo[i].y+95);
		}
		else
		{
			m_CardControl[i].SetBenchmarkPos(m_ptUserInfo[i].x,m_ptUserInfo[i].y-m_ImageCard.GetHeight()/5-10,enXCenter,enYCenter);
			m_ptUserCard[i].SetPoint(m_ptUserInfo[i].x,m_ptUserInfo[i].y-125);
		}
	}


	//���ֿؼ�λ��
	m_ptGameEndScore[0].SetPoint(nWidth/2+185,nHeight/2-170);
	m_ptGameEndScore[1].SetPoint(nWidth/2+310,nHeight/2-50);
	m_ptGameEndScore[2].SetPoint(nWidth/2-90,nHeight/2+100);
	m_ptGameEndScore[3].SetPoint(nWidth/2-330,nHeight/2-50);
	m_ptGameEndScore[4].SetPoint(nWidth/2-185,nHeight/2-170);


	//ׯ�ұ�־
	m_PointBanker[0].x=m_ptAvatar[0].x;
	m_PointBanker[0].y=m_ptAvatar[0].y+135+m_ImageUserInfo.GetHeight();
	m_PointBanker[1].x=m_ptAvatar[1].x-60;
	m_PointBanker[1].y=m_ptAvatar[1].y-45;
	m_PointBanker[2].x=m_ptUserInfo[2].x-45;
	m_PointBanker[2].y=m_ptUserInfo[2].y-25;
	m_PointBanker[3].x=m_ptAvatar[3].x+m_ImageUserInfo.GetWidth()+10;
	m_PointBanker[3].y=m_ptAvatar[3].y-45;
	m_PointBanker[4].x=m_ptAvatar[4].x+m_ImageUserInfo.GetWidth()-45;
	m_PointBanker[4].y=m_ptAvatar[4].y+135+m_ImageUserInfo.GetHeight();

	//����λ��
	m_ptJettons[0].x=m_ptUserInfo[0].x-80;
	m_ptJettons[0].y=m_ptUserInfo[0].y+120;
	m_ptJettons[1].x=m_ptUserInfo[1].x-160;
	m_ptJettons[1].y=m_ptUserInfo[1].y-150;
	m_ptJettons[2].x=m_ptUserInfo[2].x-70;
	m_ptJettons[2].y=m_ptUserInfo[2].y-60;
	m_ptJettons[3].x=m_ptUserInfo[3].x+200;
	m_ptJettons[3].y=m_ptUserInfo[3].y-100;
	m_ptJettons[4].x=m_ptUserInfo[4].x+180;
	m_ptJettons[4].y=m_ptUserInfo[4].y+140;

	//m_ptUserCard[0].x=m_CardControl[0].GetCardPos().x;
	//m_ptUserCard[0].y=m_CardControl[0].GetCardPos().y;
	//m_ptUserCard[1].x=m_CardControl[1].GetCardPos().x;
	//m_ptUserCard[1].y=m_CardControl[1].GetCardPos().y;
	//m_ptUserCard[2].x=m_CardControl[2].GetCardPos().x;
	//m_ptUserCard[2].y=m_CardControl[2].GetCardPos().y-20;
	//m_ptUserCard[3].x=m_CardControl[3].GetCardPos().x+40;
	//m_ptUserCard[3].y=m_CardControl[3].GetCardPos().y+39;
	//m_ptUserCard[4].x=m_CardControl[4].GetCardPos().x+80;
	//m_ptUserCard[4].y=m_CardControl[4].GetCardPos().y+20;

	//��ť�ؼ�
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��ʼ��ť
	int nx=nWidth/2-(142*7/2);
	int ny=nHeight/2+265;
	int nw=142;
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,nx+nw*3,ny,0,0,uFlags);

	//���ư�ť
	DeferWindowPos(hDwp,m_btAddScore,NULL,nx+nw,ny,0,0,uFlags);
	DeferWindowPos(hDwp,m_btFollow,NULL,nx+nw*2,ny,0,0,uFlags);
	DeferWindowPos(hDwp,m_btLookCard,NULL,nx+nw*3,ny,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nx+nw*4,ny,0,0,uFlags);
	DeferWindowPos(hDwp,m_btCompareCard,NULL,nx+nw*5,ny,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOpenCard,NULL,nx+nw*6,ny,0,0,uFlags);

	DeferWindowPos(hDwp,m_btConfirm,NULL,nx+nw*3,ny,0,0,uFlags);
	DeferWindowPos(hDwp,m_btCancel,NULL,nx+nw*4,ny,0,0,uFlags);

	//����λ��
	m_ptUserCompare.SetPoint(nWidth/2,nHeight*2/5);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	//����ؼ�
	m_JettonControl.SetBenchmarkPos(nWidth/2,nHeight/2-35);

	m_JettonView.RectifyControl(nWidth,nHeight);

}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	////����Ч��

	DrawViewImage(pDC,m_ImageViewCenter,DRAW_MODE_SPREAD);
	if(m_bFalsh)
	{
		int xValue=rand()%ROCK;
		int yValue=rand()%ROCK;
		if(xValue%2==0)xValue*=(-1);
		if(yValue%2==0)yValue*=(-1);
		m_ImageViewBack.TransDrawImage(pDC,(nWidth/2-512)+xValue,(nHeight/2-397)+yValue,
			m_ImageViewBack.GetWidth(),m_ImageViewBack.GetHeight(),0,0,RGB(255,0,255));

	}
	else 
	{
		//�滭����
		DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);
	}
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_CardControl[i].DrawCardControl(pDC);
	}
	if(!m_bStopDraw)
	{
		//�滭�û�

		for (WORD i=0;i<GAME_PLAYER;i++)
		{

			//��������
			m_ImageUserInfo.DrawImage(pDC,m_ptUserInfo[i].x,m_ptUserInfo[i].y);
			WORD wUserTimer=GetUserClock(i);
			IClientUserItem * pClientUserItem=GetClientUserItem(i);
//����ͼƬ
//#ifdef DEBUG
//			m_PngOffLine.DrawImage(pDC,m_ptOffLine[i].x,m_ptOffLine[i].y);
//#endif
			//�滭�û�
			if (pClientUserItem!=NULL)
			{
				
				//�û�����
				CRect rc(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+68,m_ptNickName[i].y+16);
				CDFontEx::DrawText(this, pDC, 12, 500,pClientUserItem->GetNickName(),&rc, RGB(250,250,0), DT_LEFT|DT_END_ELLIPSIS);

				//�û����
				TCHAR szBuffer[64]=TEXT("");

				_sntprintf(szBuffer,CountArray(szBuffer),SCORE_STRING,pClientUserItem->GetUserScore());
				rc.SetRect(m_ptScore[i].x,m_ptScore[i].y,m_ptScore[i].x+68,m_ptScore[i].y+16);
				CDFontEx::DrawText(this, pDC, 12, 500,szBuffer,&rc, RGB(250,250,0), DT_LEFT|DT_END_ELLIPSIS);
				
				//��ַ����
				rc.SetRect(m_ptScore[i].x,m_ptScore[i].y+20,m_ptScore[i].x+76,m_ptScore[i].y+32);
				CDFontEx::DrawText(this, pDC, 12, 500,pClientUserItem->GetAddrDescribe(),&rc, RGB(250,250,0), DT_LEFT|DT_END_ELLIPSIS);

				CGameClientEngine *pGameClientDlg=(CGameClientEngine*)CGameFrameEngine::GetInstance();
				IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->m_pIClientKernel;

				if(CUserRight::IsGameCheatUser(pIClientKernel->GetUserAttribute()->dwUserRight))
				{
					_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%d"),pClientUserItem->GetUserID());
					rc.SetRect(m_ptScore[i].x,m_ptScore[i].y+60,m_ptScore[i].x+68,m_ptScore[i].y+72);
					CDFontEx::DrawText(this,pDC,12,500,szBuffer,&rc,RGB(250,250,0),DT_LEFT|DT_END_ELLIPSIS);
				}

				//������Ϣ
				if (wUserTimer!=0) 
				{
					DrawUserClockEx(pDC,m_ptClock[i].x,m_ptClock[i].y,i);
				}

				//׼����־
				if (pClientUserItem->GetUserStatus()==US_READY) 
				{
					DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
				}
				if (pClientUserItem->GetUserStatus()==US_OFFLINE)
				{
					m_PngOffLine.DrawImage(pDC,m_ptOffLine[i].x,m_ptOffLine[i].y);
				}
				DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pClientUserItem);
			}
		}

		//ׯ����Ϣ
		if (m_wBankerUser<5)
		{
			//ׯ�ұ�־
			m_ImageBanker.DrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y);
		}

		//�滭����
		m_JettonControl.DrawJettonControl(pDC);

		//������ע
		LONGLONG lTableScore=0;
		for (WORD i=0;i<GAME_PLAYER;i++) lTableScore+=m_lTableScore[i];

		if(lTableScore>0)
		{
			TCHAR szAllScore[128]=TEXT("");
			_sntprintf(szAllScore,CountArray(szAllScore),TEXT("")SCORE_STRING,lTableScore/100.);
			DrawNumber( pDC, &m_ImgaeScoreAll, TEXT(".0123456789"), szAllScore, nWidth/2-35, nHeight/2-153, DT_CENTER);
		}

		//�滭����
		for (BYTE i=0;i<GAME_PLAYER;i++)
		{
			if (m_lTableScore[i]==0L) continue;

			//�滭��Ŀ
			TCHAR szBuffer[64];
			_sntprintf(szBuffer,CountArray(szBuffer),SCORE_STRING,m_lTableScore[i]/100.);

			//����λ��
			CRect rcDrawRect;
			rcDrawRect.top=m_CardControl[i].GetBenchmarkPosY();
			rcDrawRect.left=m_CardControl[i].GetBenchmarkPosX()+((i>2)?(150):(-50));
			rcDrawRect.right=rcDrawRect.left+30;
			rcDrawRect.bottom=rcDrawRect.top+12;

			DrawTextString(pDC,szBuffer,RGB(250,200,40),RGB(0,0,0),&rcDrawRect,14,700,DT_CENTER|DT_VCENTER|DT_NOCLIP|DT_SINGLELINE);
			
		}
		//�滭��Ϣ
		if(m_lCellScore>0)
		{
			//����λ��
			CRect rcDrawRect;

			//�滭��Ŀ
			TCHAR szBuffer[64];
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�ⶥ���ƣ�")SCORE_STRING,m_lMaxScore/100.);
			rcDrawRect.top=nHeight/2-340;
			rcDrawRect.left=nWidth/2-90;
			rcDrawRect.right=rcDrawRect.left+230;
			rcDrawRect.bottom=rcDrawRect.top+12;
			CDFontEx::DrawText(this, pDC, 12, 500,szBuffer,&rcDrawRect, RGB(250,250,0), DT_LEFT|DT_SINGLELINE);

			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("��ע��")SCORE_STRING,m_lCellScore/100.);
			rcDrawRect.top=nHeight/2-320;
			rcDrawRect.left=nWidth/2-90;
			rcDrawRect.right=rcDrawRect.left+230;
			rcDrawRect.bottom=rcDrawRect.top+12;
			CDFontEx::DrawText(this, pDC, 12, 500,szBuffer,&rcDrawRect, RGB(250,250,0), DT_LEFT|DT_SINGLELINE);

			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("��ǰ������%I64d"),m_wCurrentTimes);
			rcDrawRect.top=nHeight/2-300;
			rcDrawRect.left=nWidth/2-90;
			rcDrawRect.right=rcDrawRect.left+230;
			rcDrawRect.bottom=rcDrawRect.top+12;
			CDFontEx::DrawText(this, pDC, 12, 500,szBuffer,&rcDrawRect, RGB(250,250,0), DT_LEFT|DT_SINGLELINE);


			rcDrawRect.top=nHeight/2-280;
			rcDrawRect.left=nWidth/2-90;
			rcDrawRect.right=rcDrawRect.left+230;
			rcDrawRect.bottom=rcDrawRect.top+12;
			CDFontEx::DrawText(this, pDC, 12, 500,TEXT("���Ʊ�����2 ���Ʊ�����2"),&rcDrawRect, RGB(250,250,0), DT_LEFT|DT_SINGLELINE);

		}
		//ѡ�ȱ�־
		if(m_bCompareCard)
		{
			for (int i=0;i<GAME_PLAYER;i++)
			{
				if(m_bCompareUser[i]==TRUE)
				{
					CPoint cPost(m_CardControl[i].GetBenchmarkPosX(),m_CardControl[i].GetBenchmarkPosY());
					m_ImageArrowhead.TransDrawImage(pDC,cPost.x,cPost.y-m_ImageArrowhead.GetHeight(),
						m_ImageArrowhead.GetWidth(),m_ImageArrowhead.GetHeight(),0,0,RGB(255,0,255));
					//m_bCompareUser[i] = FALSE;
				}
			}
		}
	}

	//���ƻ滭
	if (m_SendCardItemArray.GetCount()>0)
	{
		//��������
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//��ȡ��С
		int nItemWidth=m_ImageCard.GetWidth()/13;
		int nItemHeight=m_ImageCard.GetHeight()/5;

		//�滭�˿�
		m_ImageCard.DrawImage(pDC,m_SendCardPos.x,m_SendCardPos.y,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4);
	}

	//�����˿�
	if (m_wLoserUser!=INVALID_CHAIR)
	{
		//����״̬
		if (m_wConmareIndex<m_wConmareCount)
		{
			//�滭�˿�
			for (BYTE i=0;i<2;i++)
			{
				//��������
				CPoint PointCard;
				CPoint PointStart=m_ptUserCard[m_wCompareChairID[i]];

				//����λ��
				INT nQuotiety=(i==0)?-1:1;
				PointCard.y=PointStart.y+(m_ptUserCompare.y-PointStart.y)*m_wConmareIndex/m_wConmareCount;
				PointCard.x=PointStart.x+(m_ptUserCompare.x-PointStart.x+COMPARE_RADII*nQuotiety)*m_wConmareIndex/m_wConmareCount;

				//�滭�˿�
				DrawCompareCard(pDC,PointCard.x,PointCard.y,false);
			}
		}

		//����״̬
		if ((m_wConmareIndex>=m_wConmareCount)&&((m_wConmareIndex<m_wConmareCount+VSCARD_COUNT*VSCARD_TIME)))
		{
			//�滭�˿�
			for (BYTE i=0;i<2;i++)
			{
				//��������
				CPoint PointCard;
				CPoint PointStart=m_ptUserCard[m_wCompareChairID[i]];

				//����λ��
				INT nQuotiety=(i==0)?-1:1;
				PointCard.y=m_ptUserCompare.y;
				PointCard.x=m_ptUserCompare.x+COMPARE_RADII*nQuotiety;

				//�滭�˿�
				DrawCompareCard(pDC,PointCard.x,PointCard.y,false);
			}

			//���Ʊ�־
			CSize SizeVSCard;
			CPngImage ImageVSCard;
			ImageVSCard.LoadImage(AfxGetInstanceHandle(),TEXT("VS_CARD"));
			SizeVSCard.SetSize(ImageVSCard.GetWidth()/VSCARD_COUNT,ImageVSCard.GetHeight());

			//�滭��־
			INT nImageIndex=(m_wConmareIndex-m_wConmareCount)%VSCARD_COUNT;
			ImageVSCard.DrawImage(pDC,m_ptUserCompare.x-SizeVSCard.cx/2,m_ptUserCompare.y-SizeVSCard.cy/2,
				SizeVSCard.cx,SizeVSCard.cy,nImageIndex*SizeVSCard.cx,0);

			//����Ч��
			CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
			if(pGameClientEngine!=NULL)
			{
				//����
				pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("COMPARE_CARD"));
				
			}

		}

		//����״̬
		if (m_wConmareIndex>=m_wConmareCount+VSCARD_COUNT*VSCARD_TIME)
		{
			//��������
			WORD wConmareIndex=m_wConmareIndex-m_wConmareCount-VSCARD_COUNT*VSCARD_TIME;

			//�滭�˿�
			for (BYTE i=0;i<2;i++)
			{
				//��������
				CPoint PointCard;
				CPoint PointStart=m_ptUserCard[m_wCompareChairID[i]];

				//����λ��
				INT nQuotiety=(i==0)?-1:1;
				PointCard.y=m_ptUserCompare.y+(PointStart.y-m_ptUserCompare.y)*wConmareIndex/m_wConmareCount;
				PointCard.x=m_ptUserCompare.x+COMPARE_RADII*nQuotiety+(PointStart.x-m_ptUserCompare.x-COMPARE_RADII*nQuotiety)*wConmareIndex/m_wConmareCount;

				//�滭�˿�
				DrawCompareCard(pDC,PointCard.x,PointCard.y,(m_wLoserUser==m_wCompareChairID[i])?true:false);
			}

			//����Ч��
			if(m_wConmareIndex==m_wConmareCount+VSCARD_COUNT*VSCARD_TIME)
			{
				CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
				if(pGameClientEngine!=NULL)
					pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("CENTER_SEND_CARD"));
			}
		}
	}

	//�ȴ�ѡ��
	if(m_wWaitUserChoice!=INVALID_CHAIR)
	{
		TCHAR szBuffer[64]=TEXT("");

		//�����Ϣ
		if(m_wWaitUserChoice==TRUE)_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�ȴ����ѡ�����"));
		else _sntprintf(szBuffer,CountArray(szBuffer),TEXT("��ѡ����ҽ��б���"));
		DrawTextString(pDC,szBuffer,RGB(50,50,50),RGB(0,170,0),nWidth/2+10,nHeight/2+45,15,700,DT_CENTER);

	}

	if(m_bSystemOpen)
	{
		DrawTextString(pDC,TEXT("ϵͳ�Զ����ơ�����"),RGB(50,50,50),RGB(0,170,0),nWidth/2,nHeight/2-10,30,700,DT_CENTER);
	}

	m_JettonView.DrawView(this,pDC,nWidth,nHeight);

	// ������Ϣ
	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		if( m_lEndScore[i] !=0 )
		{
			DrawOverScore(pDC, m_lEndScore[i]/100., m_ptGameEndScore[i].x, m_ptGameEndScore[i].y);
		}
	}
	return;
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	if ( m_JettonView.PreTranslateMessage(pMsg) )
	{
		CRect rect = m_JettonView.GetRect();
		InvalidGameView(rect.left, rect.top, rect.Width(), rect.Height());
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//������ע
void CGameClientView::SetUserTableScore(WORD wChairID, LONGLONG lTableScore,LONGLONG lCurrentScore)
{
	//��������
	if (wChairID!=INVALID_CHAIR) 
	{
		m_lTableScore[wChairID]=lTableScore;


		//�γ��������� �����϶����������BUG,��ʱʹ���ⷽ������
		if(lCurrentScore>0&&m_lCellScore>0){
			//����������ֵ ����ԭ��ϵͳ�滭�붼���������ע���������������,�Ҹ����϶��������˷�����������ע�����г���,�ҰѶ���Ĳ���ȥ���Ϳ�����
			//lCurrentScore=lCurrentScore-(lCurrentScore-(lCurrentScore/m_lCellScore*m_lCellScore));
			m_JettonControl.AddScore(lCurrentScore,m_ptJettons[wChairID]);
		}

	}
	else 
	{
		ZeroMemory(m_tcBuffer,sizeof(m_tcBuffer));

		ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	}

	//���½���
	RefreshGameView();

	return;
}

//��ʼ���붯��
void CGameClientView::BeginMoveJettons()
{
	if( m_JettonControl.BeginMoveJettons() )
	{
		SetTimer( IDI_MOVE_JETTON,TIME_MOVE_JETTON,NULL );

		//���½���
		RefreshGameView();
	}
	return;
}

//ֹͣ���붯��
void CGameClientView::StopMoveJettons()
{
	if( m_JettonControl.FinishMoveJettons() )
	{
		KillTimer( IDI_MOVE_JETTON );

		//���½���
		RefreshGameView(  );
	}
	return;
}

//ʤ�����
void CGameClientView::SetGameEndInfo( WORD wWinner )
{
	if( wWinner == INVALID_CHAIR ) return ;

	m_JettonControl.RemoveAllScore( m_ptJettons[wWinner] );
	if( m_JettonControl.BeginMoveJettons() )
		SetTimer( IDI_MOVE_JETTON,TIME_MOVE_JETTON,NULL );

	//���½���
	RefreshGameView(  );
	return;
}

//�ȴ�ѡ��
void CGameClientView::SetWaitUserChoice(WORD wChoice)
{
	m_wWaitUserChoice = wChoice;

	//���½���
	RefreshGameView();

	return;
}

void CGameClientView::SetSystemOpen(bool bSystem)
{
	m_bSystemOpen = bSystem;
	//���½���
	RefreshGameView();
	return;
}
//ׯ�ұ�־
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	m_wBankerUser=wBankerUser;

	//���½���
	RefreshGameView();
	return;
}

////�ȴ���־
//void CGameClientView::SetWaitInvest(bool bInvest)
//{
//	if(m_bInvest!=bInvest)
//	{
//		m_bInvest=bInvest;
//
//		//���½���
//		RefreshGameView(NULL);
//	}
//	return;
//}

//������Ϣ
void CGameClientView::SetScoreInfo(LONGLONG lTurnMaxScore,LONGLONG lTurnLessScore)
{
	m_lMaxCellScore=lTurnMaxScore;
	m_lCellScore=lTurnLessScore;

	//���½���
	RefreshGameView();
	return;
}

////��ʾ����
//void CGameClientView::DisplayType(bool bShow)
//{
//	if(m_bShow!=bShow)
//	{
//		m_bShow=bShow;
//
//		//���½���
//		RefreshGameView(NULL);
//	}
//}

//���Ʊ�־
void CGameClientView::SetCompareCard(bool bCompareCard,BOOL bCompareUser[])
{
	m_bCompareCard=bCompareCard;
	if(bCompareUser!=NULL)
	{
		CopyMemory(m_bCompareUser,bCompareUser,sizeof(m_bCompareUser));
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(m_bCompareUser[i]==TRUE)m_CardControl[i].SetCompareCard(true);
			else m_CardControl[i].SetCompareCard(false);
		}
	}
	//���½���
	RefreshGameView();

	if(!bCompareCard)
	{
		for(int i=0;i<GAME_PLAYER;i++)
		{
			m_CardControl[i].SetCompareCard(false) ;
		}
	}

	return;
}

//ֹͣ����
void CGameClientView::StopUpdataScore(bool bStop)
{
	//���½���
	RefreshGameView();

	return;
}

//�滭�˿�
void CGameClientView::DrawCompareCard(CDC * pDC, INT nXPos, INT nYPos, bool bChapped)
{
	//������Դ
	CSize SizeGameCard(m_ImageCard.GetWidth()/13,m_ImageCard.GetHeight()/5);

	//����λ��
	nYPos-=SizeGameCard.cy/2;
	nXPos-=(SizeGameCard.cx+DEFAULT_PELS*(MAX_COUNT-1))/2;

	//�滭�˿�
	for (BYTE i=0;i<MAX_COUNT;i++)
	{
		INT nXDrawPos=nXPos+DEFAULT_PELS*i;
		m_ImageCard.DrawImage(pDC,nXDrawPos,nYPos,SizeGameCard.cx,SizeGameCard.cy,
			SizeGameCard.cx*((bChapped==true)?4:2),SizeGameCard.cy*4);
	}

	return;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,int nWidth, int nWeight,UINT nFormat)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�

	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,nXPos+nXExcursion[i],nYPos+nYExcursion[i], crFrame, nFormat);
	}

	//�滭����

	CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,nXPos,nYPos, crText, nFormat);

	return;
}


//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect,int nWidth, int nWeight,UINT nFormat)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	//pDC->SetTextColor(crFrame);
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,&rcDraw, crFrame, nFormat);

	}

	//�滭����
	rcDraw.CopyRect(lpRect);
	CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,&rcDraw, crText, nFormat);
	return;
}

//������ͼ
void CGameClientView::RefreshGameView()
{
	CRect rect;
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

	return;
}
void CGameClientView::DrawUserClockEx(CDC *pDC,INT nXPos, INT nYPos,WORD wChairID)
{
	WORD wTime=GetUserClock(wChairID);
	if(wTime !=0)
		m_ImageTime.DrawImage(pDC,nXPos,nYPos,m_ImageTime.GetWidth()*wTime/30,m_ImageTime.GetHeight(),0,0);
}

void CGameClientView::SetGameEndScore(LONGLONG score[GAME_PLAYER])
{
	if(score==NULL)
		ZeroMemory(m_lEndScore,sizeof(m_lEndScore));
	else
		CopyMemory(m_lEndScore,score,sizeof(m_lEndScore));
	return;
}


// �滭����
void CGameClientView::DrawNumber(CDC * pDC , CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat /*= DT_LEFT*/)
{
	// ������Դ
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

	if ( uFormat == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= lstrlen(szOutNum) * nNumberWidth;
	}

	for ( INT i = 0; i < lstrlen(szOutNum); ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}

// �滭��������
void CGameClientView::DrawOverScore( CDC * pDC, SCORE lOutNum, INT nXPos, INT nYPos )
{
	if( lOutNum <=0.001 && lOutNum >= -0.001 )
		return;

	CPngImage* ImageNumber = NULL;
	TCHAR szOutNum[128] = {0};
	TCHAR szImageNum[128] = { TEXT("+-.0123456789") };
	if ( lOutNum >0.001)
		_sntprintf(szOutNum,CountArray(szOutNum),TEXT("+%0.2f"),lOutNum);
	else
		_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%0.2f"),lOutNum);

	if ( lOutNum >0.001 )
		ImageNumber = &m_ImgaeScoreWin;
	else
		ImageNumber = &m_ImgaeScoreLose;

	INT nNumberHeight = ImageNumber->GetHeight();
	INT nNumberWidth  = ImageNumber->GetWidth() / lstrlen(szImageNum);

	int nBackX = nXPos - ((INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth) ) + 35;
	int nBackY = nYPos;
	int nBackWidth = (int)lstrlen(szOutNum) * nNumberWidth - 35;

	m_ImgaeScoreBackL.DrawImage( pDC, nBackX - 50, nBackY);
	for( int i = 0; i < nBackWidth; i += m_ImgaeScoreBackM.GetWidth() )
	{
		m_ImgaeScoreBackM.DrawImage( pDC, nBackX, nBackY);
		nBackX += m_ImgaeScoreBackM.GetWidth();
	}
	m_ImgaeScoreBackR.DrawImage( pDC, nBackX, nBackY);

	DrawNumber( pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos + 10, DT_CENTER);
}


//���ƶ���
void CGameClientView::PerformCompareCard(WORD wCompareUser[2], WORD wLoserUser)
{
	//Ч��״̬
	ASSERT(m_wLoserUser==INVALID_CHAIR);
	if (m_wLoserUser!=INVALID_CHAIR) return;

	//���ñ���
	m_wConmareIndex=0;
	m_wLoserUser=wLoserUser;
	m_wConmareCount=LESS_SEND_COUNT;

	//�Ա��û�
	m_wCompareChairID[0]=wCompareUser[0];
	m_wCompareChairID[1]=wCompareUser[1];

	//��������
	for (BYTE i=0;i<2;i++)
	{
		//λ�Ƽ���
		INT nXCount=abs(m_ptUserCard[wCompareUser[i]].x-m_ptUserCompare.x)/SEND_PELS;
		INT nYCount=abs(m_ptUserCard[wCompareUser[i]].y-m_ptUserCompare.y)/SEND_PELS;

		//��������
		m_wConmareCount=__max(m_wConmareCount,__max(nXCount,nYCount));
	}

	//�����˿�
	m_CardControl[wCompareUser[0]].SetCardData(NULL,0);
	m_CardControl[wCompareUser[1]].SetCardData(NULL,0);

	//����ʱ��
	if (!m_bCartoon)
	{
		m_bCartoon=true;
		SetTimer(IDI_CARTOON,TIME_CARTOON/2,NULL);
	}

	//����Ч��
	CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	if(pGameClientEngine!=NULL)
		pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("CENTER_SEND_CARD"));

	InvalidGameView(0,0,0,0);
	return;
}

//���ƶ���
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//���ñ���
	tagSendCardItem SendCardItem;
	ZeroMemory(&SendCardItem,sizeof(SendCardItem));
	SendCardItem.wMoveIndex = 0;
	SendCardItem.wChairID=wChairID;
	SendCardItem.cbCardData=cbCardData;
	m_SendCardItemArray.Add(SendCardItem);

	//��������
	if (m_SendCardItemArray.GetCount()==1) 
	{
		if(!m_bCartoon)
		{
			m_bCartoon = true;
			//���ö�ʱ��
			SetTimer(IDI_CARTOON,TIME_CARTOON/3,NULL);
		}

		//��������
		CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
		if(pGameClientEngine!=NULL)
			pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//���ƶ���
bool CGameClientView::bFalshCard(WORD wFalshUser[])
{
	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(wFalshUser[i]<GAME_PLAYER)m_wFlashUser[i] = wFalshUser[i];
	}

	//���ö�ʱ��
	m_bStopDraw=true;

	if(!m_bCartoon)
	{
		m_bCartoon = true;
		SetTimer(IDI_CARTOON,TIME_CARTOON,NULL);
	}

	return true;
}

//���ƴ���
bool CGameClientView::SendCard()
{
	//�����ж�
	if (m_SendCardItemArray.GetCount()==0) return false;

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����λ��
	tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];
	WORD wChairID = pSendCardItem->wChairID;
	m_SendCardPos.x=m_KeepPos.x + (pSendCardItem->wMoveIndex*(m_ptUserCard[wChairID].x-m_KeepPos.x)/SPEED);
	m_SendCardPos.y=m_KeepPos.y + (pSendCardItem->wMoveIndex*(m_ptUserCard[wChairID].y-m_KeepPos.y)/SPEED);

	pSendCardItem->wMoveIndex++;
	//���½���
	InvalidGameView(0,0,0,0);

	//ֹͣ�ж�
	if(pSendCardItem->wMoveIndex == SPEED)
	{
		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//�����˿�
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}

		//ɾ������
		m_SendCardItemArray.RemoveAt(0);

		//��������
		if (m_SendCardItemArray.GetCount()>0)
		{
			//��������
			CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
			if(pGameClientEngine!=NULL)
				pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
			
			InvalidGameView(0,0,0,0);

			return true;
		}
		else
		{
			//��ɴ���
			SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

			InvalidGameView(0,0,0,0);
			return false;
		}
	}


	return true;
}

//���ƴ���
bool CGameClientView::FlashCard()
{	
	//�����ж�
	int i=0;
	for( i=0;i<GAME_PLAYER;i++) if(m_wFlashUser[i]<GAME_PLAYER)break;
	if(i==GAME_PLAYER)return false;

	//���ñ���
	m_wFalshCount++;
	WORD wTimes=m_wFalshCount*TIME_CARTOON;

	//���½���
	RefreshGameView();
	m_bFalsh=!m_bFalsh;

	//�˿˱���
	WORD wValue=(rand()%2==0)?((m_wFalshCount+1)%6):((m_wFalshCount+2)%6);
	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(m_wFlashUser[i]<GAME_PLAYER)m_CardControl[m_wFlashUser[i]].SetCardColor(wValue);
	}

	//ֹͣ����
	if(2200 <= wTimes)
	{
		//���ò���
		m_bFalsh=false;
		m_bStopDraw=false;
		m_wFalshCount=0;
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(m_wFlashUser[i]<GAME_PLAYER)
			{
				m_CardControl[m_wFlashUser[i]].SetCardColor(INVALID_CHAIR);
				m_wFlashUser[i]=INVALID_CHAIR;
			}
		}

		//��ɴ���
		SendEngineMessage(IDM_FALSH_CARD_FINISH,0,0);

		//���½���
		RefreshGameView();

		return false; 
	}

	return true; 
}

//���ƴ���
bool CGameClientView::CompareCard()
{
	//�����ж�
	if (m_wLoserUser==INVALID_CHAIR) return false;

	//��������
	m_wConmareIndex++;

	//���½���
	RefreshGameView();

	//ֹͣ�ж�
	if (m_wConmareIndex>=((m_wConmareCount*2)+VSCARD_COUNT*VSCARD_TIME))
	{
		//�����˿�
		BYTE bTemp[]={0,0,0};
		m_CardControl[m_wLoserUser].SetCardColor(2);
		m_CardControl[m_wCompareChairID[0]].SetCardData(bTemp,MAX_COUNT);
		m_CardControl[m_wCompareChairID[1]].SetCardData(bTemp,MAX_COUNT);

		//���ñ���
		m_wConmareIndex=0;
		m_wConmareCount=0;
		m_wLoserUser=INVALID_CHAIR;
		ZeroMemory(m_wCompareChairID,sizeof(m_wCompareChairID));

		//������Ϣ
		SendEngineMessage(IDM_FALSH_CARD_FINISH,0,0);
		InvalidGameView(0,0,0,0);
		return false;
	}

	return true;
}

//ֹͣ����
void CGameClientView::StopFlashCard()
{
	//�����ж�
	int i=0;
	for( i=0;i<GAME_PLAYER;i++) if(m_wFlashUser[i]<GAME_PLAYER)break;
	if(i==GAME_PLAYER)return ;

	//���ò���
	m_bFalsh=false;
	m_bStopDraw=false;
	m_wFalshCount=0;
	for(int i=0;i<GAME_PLAYER;i++)
	{
		if(m_wFlashUser[i]<GAME_PLAYER)
		{
			m_CardControl[m_wFlashUser[i]].SetCardColor(INVALID_CHAIR);
			m_wFlashUser[i]=INVALID_CHAIR;
		}
	}

	//��ɴ���
	SendEngineMessage(IDM_FALSH_CARD_FINISH,0,0);

	return ;
}

//ֹͣ����
void CGameClientView::StopCompareCard()
{
	if(m_wLoserUser>GAME_PLAYER)return ;

	//�����˿�
	BYTE bTemp[]={0,0,0};
	m_CardControl[m_wLoserUser].SetCardColor(2);
	m_CardControl[m_wCompareChairID[0]].SetCardData(bTemp,MAX_COUNT);
	m_CardControl[m_wCompareChairID[1]].SetCardData(bTemp,MAX_COUNT);
	

	//���ñ���
	m_wConmareIndex=0;
	m_wConmareCount=0;
	m_wLoserUser=INVALID_CHAIR;
	ZeroMemory(m_wCompareChairID,sizeof(m_wCompareChairID));

	//������Ϣ
	SendEngineMessage(IDM_FALSH_CARD_FINISH,0,0);

	InvalidGameView(0,0,0,0);

	return ;
}

//��ɷ���
void CGameClientView::FinishDispatchCard()
{
	//����ж�
	if (m_SendCardItemArray.GetCount()==0) return;

	//��ɶ���
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//��ȡ����
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//�����˿�
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//ɾ������
	m_SendCardItemArray.RemoveAll();

	//��ɴ���
	SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

	//���½���
	RefreshGameView();

	return;
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDI_CARTOON)		//������ʱ
	{
		//��������
		bool bKillTimer=true;

		//���ƶ���
		if(m_bStopDraw && (FlashCard()==true))
		{
			bKillTimer=false;
		}

		//���ƶ���
		if ((m_wLoserUser!=INVALID_CHAIR)&&(CompareCard()==true))
		{
			bKillTimer=false;
		}

		//���ƶ���
		if ((m_SendCardItemArray.GetCount()>0)&&(SendCard()==true))
		{
			bKillTimer=false;
		}

		//ɾ��ʱ��
		if (bKillTimer==true)
		{
			m_bCartoon=false;
			KillTimer(nIDEvent);
		}

		return;
	}

	//���붯��
	if( IDI_MOVE_JETTON==nIDEvent)
	{
		if( !m_JettonControl.PlayMoveJettons() )
		{
			KillTimer(IDI_MOVE_JETTON);

			////������Ϣ
			//AfxGetMainWnd()->PostMessage(IDM_MOVE_JET_FINISH,0,0);

			////��ʾ���������Ϣ
			//if( m_lJettonScore < m_JettonControl.GetScore() )
			//{
			//	SetTimer( IDI_ACTION_ADD_JETTON,TIME_ACTION_ADD_JETTON,NULL );
			//}

			////��������
			//if( m_bPlayingActionSound )
			//	m_bMovingJetton = true;
			//else
			//{
			//	CGameClientEngine * pGameDlg=(CGameClientDlg *)AfxGetMainWnd();
			//	pGameDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
			//}

			RefreshGameView();

			return;
		}

		//��ȡ��������
		CRect rcDraw;
		m_JettonControl.GetDrawRect(rcDraw);
		RefreshGameView();

		return;
	}

	__super::OnTimer(nIDEvent);
}

void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_CardControl[i].OnClickJudge(Point)==TRUE)
		{
			m_wButtonDownUser = i;
			break;
		}
	}

	return;
}

//�����Ϣ
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{

	__super::OnLButtonDown(nFlags,Point);

	if(m_wButtonDownUser !=INVALID_CHAIR)
	{
		if(m_CardControl[m_wButtonDownUser].OnClickJudge(Point)==TRUE)
		{
			SendEngineMessage(m_CardControl[m_wButtonDownUser].m_wMessageID,m_CardControl[m_wButtonDownUser].m_wControlID,m_CardControl[m_wButtonDownUser].m_wControlID);
		}
	}


	return;
}


//////////////////////////////////////////////////////////////////////////
void CGameClientView::OpenAdminWnd()
{
	//����д�Ȩ��
	CGameClientEngine *pGameClientDlg=(CGameClientEngine*)CGameFrameEngine::GetInstance();
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->m_pIClientKernel;
	if(CUserRight::IsGameCheatUser(pIClientKernel->GetUserAttribute()->dwUserRight))
	{
		if (NULL==m_AdminDlg.m_hWnd) m_AdminDlg.Create(IDD_DIALOG_ADMIN,this);

		if(!m_AdminDlg.IsWindowVisible()) m_AdminDlg.ShowWindow(SW_SHOW);
		else m_AdminDlg.ShowWindow(SW_HIDE);
	}
}

