#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientView.h"
#include "GameClientEngine.h"

#ifdef DEBUG
//#define ALONE
#endif

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ť��ʶ
#define IDC_HINT_OX						130								//��ʾ��ť
#define IDC_START						100								//��ʼ��ť
#define IDC_MAX_SCORE					104								//���ť
#define IDC_MIN_SCORE					105								//���ٰ�ť
#define IDC_ADD_SCORE					110								//��ע��ť

#define IDC_READY						111								//̯�ư�ť
#define IDC_RESORT						112								//���Ű�ť
#define IDC_OX							113								//ţţ��ť

#define IDC_SHORTCUT					114								//��ݼ���ť
#define IDC_IDLER						115								//�мҰ�ť
#define IDC_BANKER						116								//ׯ�Ұ�ť
#define IDC_SCORE						117								//��ְ�ť
#define IDC_YU_YIN						118								//������ť
 #define IDC_ADMIN						119									//��ť��ʶ

//��ʱ����ʶ
#define IDI_SEND_CARD					98								//���ƶ�ʱ��
#define IDI_GOLD						97								//���ƶ�ʱ��
#define IDI_USER_ACTION					101								//������ʱ��
#define IDI_UPDATA_GAME					102								//���¶�ʱ��
#define IDI_JETTON_ACTION				103								//������ʱ��

//��ʱ��ʱ��
#define TIME_USER_ACTION				4000							//������ʱ��

//�ƶ��ٶ�
#define	SEND_PELS						80								//�����ٶ�
#define	TIME_SENDSPEED					50								//����ٶ�


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SCORE, OnBnClickedScore)
	ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)

	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��������
	m_cbIndex=0;
	m_bJettonAction=false;
	m_cbDynamicJoin=FALSE;
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_bOxValue[i]=0xff;
		m_SendEndingPos[i].SetPoint(0,0);
	}
	m_wSendCount=0;
	m_wSendIndex=0;
	m_SendCardPos.SetPoint(0,0);
	m_SendCardItemArray.RemoveAll();
	ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));
	ZeroMemory(m_lMoveGold,sizeof(m_lMoveGold));

	//���ݱ���
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bOpenCardUser,sizeof(m_bOpenCardUser));
	ZeroMemory(m_bShowSocre,sizeof(m_bShowSocre));
	m_wBankerUser=INVALID_CHAIR;
	m_bCallUser=0xff;
	m_bShow=false;
	m_bInvest=false;
	m_bOpenCard=false;
	m_bUserAction=false;
	m_bLookOnUser=false;
	m_bShowScore=false;
	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	return;
}

//��������
CGameClientView::~CGameClientView()
{
	m_ViewFontEx.DeleteFont();
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageScore.LoadFromResource(this,hInstance,IDB_SCORE);
	m_ImageJeton.LoadFromResource(this,hInstance,IDB__MONEY);
	m_ImageCard.LoadFromResource(this,hInstance,IDB_CARD);
	m_ImageViewBack.LoadFromResource(this,hInstance,IDB_VIEW_BACK);
	m_ImageViewCenter.LoadFromResource(this,hInstance,IDB_VIEW_FILL);

	//������Դ
	m_PngReady.LoadImage(this,hInstance,TEXT("READY"));
	m_PngUserBack.LoadImage(this,hInstance,TEXT("USER_HEAD_BACK"));
	m_ImageOxValueZero.LoadImage(this,hInstance,TEXT("COW_VALUE_ZERO"));
	m_ImageDoulbeOx.LoadImage(this,hInstance,TEXT("DOUBLE_COW"));
	m_ImageNumber.LoadImage(this,hInstance,TEXT("NUMBER"));
	m_ImageOxValue.LoadImage(this,hInstance,TEXT("COW_VALUE"));
	m_ImageWaitCall.LoadImage(this,hInstance,TEXT("CALLBANKER"));
	m_ImageTimeBack.LoadImage(this,hInstance,TEXT("TIME_BACK"));
	m_ImageTimeNumber.LoadImage(this,hInstance,TEXT("TIME_NUMBER"));
	m_ImageBanker.LoadImage(this,hInstance,TEXT("BANKER_FLAG"));
	m_PngAutoOpen.LoadImage(this,hInstance,TEXT("AUTO_OPEN"));
	m_PngWaitBet.LoadImage(this,hInstance,TEXT("WAIT_BET"));
	m_PngWaitOpen.LoadImage(this,hInstance,TEXT("WAIT_OPEN"));
	m_PngOpenTag.LoadImage(this,hInstance,TEXT("OPEN_TAG"));
	m_PngHistoryScore.LoadImage(this,hInstance,TEXT("HISTORY_SCORE"));
	m_PngOffLine.LoadImage(this,hInstance,TEXT("OFF_LINE"));
	
	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_btYuYin.Create(NULL,WS_CHILD,rcCreate,this,IDC_YU_YIN);
	m_btScore.Create(NULL,WS_CHILD,rcCreate,this,IDC_SCORE);
	m_btHintOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_HINT_OX);
	m_btStart.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_START);
	m_btOpenCard.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_READY);
	m_btOx.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_OX);
	m_btBanker.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BANKER);
	m_btIdler.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_IDLER);

	//����λͼ
	m_btYuYin.SetButtonImage(IDB_BT_YU_YIN,hInstance,false,false);
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);
	m_btHintOx.SetButtonImage(IDB_HINT_OX,hInstance,false,false);
	m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
	m_btOpenCard.SetButtonImage(IDB_OPENCARD,hInstance,false,false);
	m_btOx.SetButtonImage(IDB_COW,hInstance,false,false);
	m_btBanker.SetButtonImage(IDB_BANKER,hInstance,false,false);
	m_btIdler.SetButtonImage(IDB_IDLER,hInstance,false,false);

	//�����ؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+i);
		m_CardControlOx[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+GAME_PLAYER+i);
		m_CardControlOx[i].SetDrawHeight(DRAW_HEIGHT);
	}

    //��ť��ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);


	//���ÿؼ�
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_CardControlOx[i].SetDisplayFlag(true);
		m_CardControlOx[i].SetCardSpace(18);
		m_CardControl[i].SetCardSpace(18);
	}

	m_JettonView.InitView(this);

	//��ȡ��С
	m_sizeHistory.SetSize(m_PngHistoryScore.GetWidth(),m_PngHistoryScore.GetHeight());
	m_SizeFace.SetSize(48,48);

	//��������
	m_ViewFontEx.CreateFont(this,TEXT("����"),12,400);
		//���ư�ť
	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,10,10),this,IDC_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	return 0;
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_HINT_OX:			//��ʾ��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_HINT_OX,0,0);
			return TRUE;
		}
	case IDC_START:				//��ʼ��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_START,0,0);
			return TRUE;
		}
	case IDC_READY:				//̯�ư�ť
		{
			//������Ϣ
			SendEngineMessage(IDM_READY,0,0);
			return TRUE;
		}
	case IDC_OX:				//ţţ��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_OX,0,0);
			return TRUE;
		}
	case IDC_RESORT:			//���Ű�ť
		{
			//������Ϣ
			SendEngineMessage(IDM_RESORT,0,0);
			return TRUE;
		}
	case IDC_SHORTCUT:			//��ݼ���ť
		{
			//������Ϣ
			SendEngineMessage(IDM_SHORTCUT_KEY,0,0);
			return TRUE;
		}
	case IDC_BANKER:			//ׯ�Ұ�ť
		{
			//������Ϣ
			SendEngineMessage(IDM_BANKER,1,1);
			return TRUE;
		}
	case IDC_IDLER:				//�мҰ�ť
		{
			//������Ϣ
			SendEngineMessage(IDM_BANKER,0,0);
			return TRUE;
		}
	case IDC_YU_YIN:
		{
			//������Ϣ
			SendEngineMessage(IDM_YU_YIN,0,0);
			return TRUE;
		}
	}

	if(m_JettonView.OnCommand(wParam,lParam)==TRUE)
	{
		CRect rc=m_JettonView.GetRect();
		InvalidGameView(rc.left,rc.right,rc.Width(),rc.Height());
		return TRUE;
	}
	return __super::OnCommand(wParam, lParam);
}

//���ý���
VOID  CGameClientView::ResetGameView()
{
	//��������
	m_cbIndex=0;
	m_bJettonAction=false;
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_bOxValue[i]=0xff;
	}
	m_wSendCount=0;
	m_wSendIndex=0;
	m_cbDynamicJoin=FALSE;
	m_SendCardPos.SetPoint(0,0);
	m_SendCardItemArray.RemoveAll();
	ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));
	ZeroMemory(m_lMoveGold,sizeof(m_lMoveGold));

	//���ݱ���
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_bOpenCardUser,sizeof(m_bOpenCardUser));
	ZeroMemory(m_bShowSocre,sizeof(m_bShowSocre));
	m_wBankerUser=INVALID_CHAIR;				
	m_bCallUser=0xff;
	m_bShow=false;
	m_bInvest=false;
	m_bOpenCard=false;
	m_bLookOnUser=false;
	m_bUserAction=false;
	m_bShowScore=false;
	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	//ɾ����ʱ��
	KillTimer(IDI_SEND_CARD);

	//���ذ�ť
	m_btScore.ShowWindow(SW_HIDE);
	m_btHintOx.ShowWindow(SW_HIDE);
	m_btStart.ShowWindow(SW_HIDE);
//	m_btReSort.ShowWindow(SW_HIDE);
	m_btOpenCard.ShowWindow(SW_HIDE);
	m_btOx.ShowWindow(SW_HIDE);

//	m_btShortcut.ShowWindow(SW_HIDE);
	m_btBanker.ShowWindow(SW_HIDE);
	m_btIdler.ShowWindow(SW_HIDE);
	m_btYuYin.EnableWindow(FALSE);

	//��ԭ��ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);

	//���ؿؼ�
//	m_ScoreView.ShowWindow(SW_HIDE);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_CardControl[i].SetDisplayFlag(false);
		//m_CardControlOx[i].SetDisplayFlag(false);
		m_CardControl[i].SetCardData(NULL,0);
		m_CardControlOx[i].SetCardData(NULL,0);
	}



	return ;
}

//�����ؼ�
void CGameClientView::RectifyControl(int nWidth, int nHeight)
{
	SetTimer(IDI_UPDATA_GAME,1000,NULL);
	


	//����λ��
	m_SendStartPos.SetPoint(nWidth/2,nHeight/2-30);

	m_ptAvatar[0].x=nWidth/2-m_SizeFace.cx/2;
	m_ptAvatar[0].y=9;
	m_ptNickName[0].x=nWidth/2+32;
	m_ptNickName[0].y=9; 
	m_ptJeton[0].x=nWidth/2-36;
	m_ptJeton[0].y=206;
	m_ptCard[0].x=m_ptJeton[0].x;
	m_ptCard[0].y=m_ptJeton[0].y-100; 
	m_ptReady[0].x=m_ptAvatar[0].x-10-69-4;
	m_ptReady[0].y=m_ptAvatar[0].y+(m_SizeFace.cy-48)/2+8;
	m_ptClock[0].x=m_ptAvatar[0].x+(m_SizeFace.cx-65)/2+32;
	m_ptClock[0].y=m_ptAvatar[0].y+m_SizeFace.cy+80;
	m_PointBanker[0].x = m_ptAvatar[0].x - 55;
	m_PointBanker[0].y =m_ptAvatar[0].y + 14;
	m_ptValue[0].x=nWidth/2+85;
	m_ptValue[0].y=129;

	m_ptAvatar[2].x=nWidth/2-m_SizeFace.cx/2;
	m_ptAvatar[2].y=nHeight-62;
	m_ptNickName[2].x=nWidth/2+32;
	m_ptNickName[2].y=nHeight-62;
	m_ptJeton[2].x=nWidth/2-36;
	m_ptJeton[2].y=nHeight-285;
	m_ptCard[2].x=m_ptJeton[2].x;
	m_ptCard[2].y=m_ptJeton[2].y+90;
	m_ptReady[2].x=m_ptAvatar[2].x-69-10-4;
	m_ptReady[2].y=m_ptAvatar[2].y+(m_SizeFace.cy-48)/2+8;
	m_ptClock[2].x=m_ptAvatar[2].x+(m_SizeFace.cx-65)/2+32;
	m_ptClock[2].y=m_ptAvatar[2].y+-57-140;
	m_PointBanker[2].x = m_ptAvatar[2].x - 54;
	m_PointBanker[2].y =m_ptAvatar[2].y +m_SizeFace.cy-33;
	m_ptValue[2].x=nWidth/2+85;
	m_ptValue[2].y=nHeight-161;

	m_ptAvatar[1].x=nWidth-m_SizeFace.cx-16;
	m_ptAvatar[1].y=nHeight/2-m_SizeFace.cy/2+5;
	m_ptNickName[1].x=m_ptAvatar[1].x;  
	m_ptNickName[1].y=m_ptAvatar[1].y+m_SizeFace.cy+6;
	m_ptJeton[1].x=nWidth-328+25;
	m_ptJeton[1].y=nHeight/2-49;
	m_ptCard[1].x=m_ptJeton[1].x+100; 
	m_ptCard[1].y=m_ptJeton[1].y;
	m_ptReady[1].x=m_ptAvatar[1].x-10-69-4;
	m_ptReady[1].y=m_ptAvatar[1].y+(m_SizeFace.cy-48)/2+8;
	m_ptClock[1].x=m_ptAvatar[1].x-80;
	m_ptClock[1].y=m_ptAvatar[1].y+(m_SizeFace.cy-57)/2+29;
	m_PointBanker[1].x = m_ptAvatar[1].x+5;
	m_PointBanker[1].y = m_ptAvatar[1].y-40;
	m_ptValue[1].x=nWidth-180;
	m_ptValue[1].y=nHeight/2+40;

	m_ptAvatar[3].x=16;
	m_ptAvatar[3].y=nHeight/2-m_SizeFace.cy/2+5;
	m_ptNickName[3].x=m_ptAvatar[3].x;
	m_ptNickName[3].y=m_ptAvatar[3].y+m_SizeFace.cy+6;
	m_ptJeton[3].x=263-30;
	m_ptJeton[3].y=nHeight/2-49;
	m_ptCard[3].x=m_ptJeton[3].x-100; 
	m_ptCard[3].y=m_ptJeton[3].y;
	m_ptReady[3].x=m_ptAvatar[3].x+m_SizeFace.cx+10+4;
	m_ptReady[3].y=m_ptAvatar[3].y+(m_SizeFace.cy-48)/2+8;
	m_ptClock[3].x=m_ptAvatar[3].x+m_SizeFace.cx+80;
	m_ptClock[3].y=m_ptAvatar[3].y+(m_SizeFace.cy-57)/2+29;
	m_PointBanker[3].x =m_ptAvatar[3].x+5;
	m_PointBanker[3].y =m_ptAvatar[3].y-40;
	m_ptValue[3].x=99;
	m_ptValue[3].y=nHeight/2+40;


	m_ptOffLine[0].SetPoint(m_ptAvatar[0].x+155,m_ptAvatar[0].y);
	m_ptOffLine[1].SetPoint(m_ptAvatar[1].x,m_ptAvatar[1].y-m_PngOffLine.GetHeight()-40);
	m_ptOffLine[2].SetPoint(m_ptAvatar[2].x+155,m_ptAvatar[2].y);
	m_ptOffLine[3].SetPoint(m_ptAvatar[3].x,m_ptAvatar[3].y-m_PngOffLine.GetHeight()-40);

	m_CardControl[0].SetBenchmarkPos(nWidth/2,80,enXCenter,enYTop); 
	m_CardControlOx[0].SetBenchmarkPos(nWidth/2,80-DRAW_HEIGHT,enXCenter,enYTop);
	m_CardControl[2].SetBenchmarkPos(nWidth/2,nHeight-68,enXCenter,enYBottom);
	m_CardControlOx[2].SetBenchmarkPos(nWidth/2,nHeight-68-DRAW_HEIGHT,enXCenter,enYBottom);
	m_CardControl[1].SetBenchmarkPos(nWidth-70,nHeight/2-30,enXRight,enYCenter);
	m_CardControlOx[1].SetBenchmarkPos(nWidth-80,nHeight/2-30-DRAW_HEIGHT,enXRight,enYCenter);
	m_CardControl[3].SetBenchmarkPos(74,nHeight/2-30,enXLeft,enYCenter);
	m_CardControlOx[3].SetBenchmarkPos(84,nHeight/2-30-DRAW_HEIGHT,enXLeft,enYCenter);


	//����λ��
	m_SendEndingPos[0].x=m_CardControl[0].GetCardPos().x;
	m_SendEndingPos[0].y=m_CardControl[0].GetCardPos().y;
	m_SendEndingPos[1].x=m_CardControl[1].GetCardPos().x;
	m_SendEndingPos[1].y=m_CardControl[1].GetCardPos().y;
	m_SendEndingPos[2].x=m_CardControl[2].GetCardPos().x;
	m_SendEndingPos[2].y=m_CardControl[2].GetCardPos().y;
	m_SendEndingPos[3].x=m_CardControl[3].GetCardPos().x;
	m_SendEndingPos[3].y=m_CardControl[3].GetCardPos().y;


	//ţֵλ��
	for(WORD i=0;i<GAME_PLAYER;i++)m_ptOpenTag[i]=m_ptValue[i]; 

	//��ť�ؼ�
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2-44,nHeight-170-56-20,0,0,uFlags);

	//���ư�ť 
	m_btOpenCard.GetWindowRect(&rcButton);
	//DeferWindowPos(hDwp,m_btReSort,NULL,nWidth/2-32,nHeight-237,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOpenCard,NULL,nWidth/2+15,nHeight-237,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOx,NULL,nWidth/2-118,nHeight-237,0,0,uFlags);
	DeferWindowPos(hDwp,m_btHintOx,NULL,nWidth/2-102,nHeight-237,0,0,uFlags);

	//ѡׯ��ť
	DeferWindowPos(hDwp,m_btBanker,NULL,nWidth/2-99,nHeight-198+10,0,0,uFlags);
	DeferWindowPos(hDwp,m_btIdler,NULL,nWidth/2+10,nHeight-198+10,0,0,uFlags);

	m_btYuYin.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btYuYin,NULL,nWidth/2,nHeight-rcButton.Height()-5,0,0,uFlags);


	//��ְ�ť
	CRect rcScore;
	m_btScore.GetWindowRect(&rcScore);
	DeferWindowPos(hDwp,m_btScore,NULL,nWidth-66,5,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);


	m_JettonView.RectifyControl(nWidth,nHeight);
	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭���� 
	DrawViewImage(pDC,m_ImageViewCenter,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);

	//�滭�û�

	CRect rcUserInfo(0,0,0,0);
	UINT nTextFormat = DT_LEFT|DT_TOP|DT_END_ELLIPSIS|DT_NOCLIP;
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserClock(i);
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);

#ifdef DEBUG
		//int nTextWidth = (i%2==0?100:64);	
		//rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+18);
		//m_ViewFontEx.DrawText(pDC,TEXT("4564s54f5a4f4fd86f"),&rcUserInfo,RGB(255,255,255),nTextFormat);

		//rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+37,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+55);
		//m_ViewFontEx.DrawText(pDC,TEXT("4564s54f5a4f4fd86f"),&rcUserInfo,RGB(255,255,255),nTextFormat);

		//rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+19,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+37);
		//m_ViewFontEx.DrawText(pDC,TEXT("4564s54f5a4f4fd86f"),&rcUserInfo,RGB(255,255,255),nTextFormat);

		//m_PngOffLine.DrawImage(pDC,m_ptOffLine[i].x,m_ptOffLine[i].y);
#endif
		//�滭�û�
		if (pIClientUserItem!=NULL)
		{
			TCHAR szBuffer[128]=TEXT("");

			//�û�����
			int nTextWidth = (i%2==0?100:64);		
			ZeroMemory(szBuffer,sizeof(szBuffer));
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s"),pIClientUserItem->GetNickName());
			rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+18);
			m_ViewFontEx.DrawText(pDC,szBuffer,&rcUserInfo,RGB(255,255,255),nTextFormat);
			
			//�û����			
			ZeroMemory(szBuffer,sizeof(szBuffer));
			_sntprintf(szBuffer,CountArray(szBuffer),SCORE_STRING,pIClientUserItem->GetUserScore());
			rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+19,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+37);
			m_ViewFontEx.DrawText(pDC,szBuffer,&rcUserInfo,RGB(255,255,255),nTextFormat);

// 			rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+37,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+55);
// 			m_ViewFontEx.DrawText(pDC,pIClientUserItem->GetAddrDescribe(),&rcUserInfo,RGB(255,255,255),nTextFormat);

			CGameClientEngine *pGameClientDlg=(CGameClientEngine*)CGameFrameEngine::GetInstance();
			IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->m_pIClientKernel;

			if(CUserRight::IsGameCheatUser(pIClientKernel->GetUserAttribute()->dwUserRight))
			{
				_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%d"),pIClientUserItem->GetUserID());
				rcUserInfo.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+65,m_ptNickName[i].x+nTextWidth,m_ptNickName[i].y+83);
				m_ViewFontEx.DrawText(pDC,szBuffer,&rcUserInfo,RGB(255,255,255),nTextFormat);
			}

			//������Ϣ
			if (wUserTimer!=0)
			{
				if (m_bCallUser==0xff && (pIClientUserItem->GetUserStatus()==US_PLAYING||m_cbDynamicJoin))
					DrawUserTimerEx(pDC,nWidth/2,nHeight/2-60,wUserTimer,99L);
				else 
				{
					if(i==2&&pIClientUserItem->GetUserStatus()!=US_PLAYING)
						DrawUserTimerEx(pDC,m_ptAvatar[i].x-65-30-4,m_ptAvatar[2].y+(m_SizeFace.cy-57)/2+29,wUserTimer,99L);
					else
						DrawUserTimerEx(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer,99L);
				}
			}
			if (pIClientUserItem->GetUserStatus()==US_READY) 
			{
				DrawUserReadyEx(pDC,m_ptReady[i].x,m_ptReady[i].y);
			}

			if(pIClientUserItem->GetUserStatus()==US_OFFLINE)
			{
				m_PngOffLine.DrawImage(pDC,m_ptOffLine[i].x,m_ptOffLine[i].y);
			}

			//�滭����
			pDC->FillSolidRect(m_ptAvatar[i].x-2,m_ptAvatar[i].y-2,52,52,RGB(255,255,255));
			DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pIClientUserItem);
			//m_PngUserBack.DrawImage( pDC,m_ptAvatar[i].x-4,m_ptAvatar[i].y-4 );
			m_PngUserBack.AlphaDrawImage( pDC,m_ptAvatar[i].x-4,m_ptAvatar[i].y-4,255);
		}
	}

	//�ȴ���ע
	if (m_bInvest)
	{
		//m_PngWaitBet.DrawImage(pDC,nWidth/2-m_PngWaitBet.GetWidth()/2,nHeight/2-24);
		m_PngWaitBet.AlphaDrawImage(pDC,nWidth/2-m_PngWaitBet.GetWidth()/2,nHeight/2-24,255);
	}

	//�ȴ�̯��
	if (m_bOpenCard)
	{
		//m_PngWaitOpen.DrawImage(pDC,nWidth/2-m_PngWaitOpen.GetWidth()/2,nHeight/2-24);
		m_PngWaitOpen.AlphaDrawImage(pDC,nWidth/2-m_PngWaitOpen.GetWidth()/2,nHeight/2-24,255);
	}

	//��������
	if (m_bUserAction)
	{
		//m_PngAutoOpen.DrawImage(pDC,nWidth/2-m_PngAutoOpen.GetWidth()/2,nHeight/2+90);//��������,ϵͳ�Զ�Ϊ������
		m_PngAutoOpen.AlphaDrawImage(pDC,nWidth/2-m_PngAutoOpen.GetWidth()/2,nHeight/2+90,255);
	}

	//�ȴ���ׯ
	if(m_bCallUser!=0xff/* && (m_bCallUser!=2 || m_bLookOnUser)*/)
	{
		//m_ImageWaitCall.DrawImage(pDC,nWidth/2-m_ImageWaitCall.GetWidth()/2,nHeight/2-24);
		m_ImageWaitCall.AlphaDrawImage(pDC,nWidth/2-m_ImageWaitCall.GetWidth()/2,nHeight/2-24,m_ImageWaitCall.GetWidth(),m_ImageWaitCall.GetHeight(),0,0,255);
	}

	//ׯ����Ϣ
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		IClientUserItem * pIClientUserItem=GetClientUserItem(m_wBankerUser);
		if (pIClientUserItem!=NULL) 
			//m_ImageBanker.DrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y);
			m_ImageBanker.AlphaDrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y,255);
	}
 
	//�滭����
	for (BYTE i=0;i<GAME_PLAYER;i++) 
	{
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		//m_lTableScore[i]=1000;
		if (pIClientUserItem!=NULL &&(m_lMoveGold[i]>COMPARE_ZERO||m_lMoveGold[i]<-COMPARE_ZERO|| m_lTableScore[i]>COMPARE_ZERO|| m_lTableScore[i]<-COMPARE_ZERO|| m_bShowSocre[i])) 
		{
			//����λ��
			int iPos=1;
			if(m_bShowSocre[i])
			{
				if(m_lTableScore[i]>COMPARE_ZERO)iPos=0;
				else iPos=2;
			}

			INT nX = m_ptJeton[i].x;
			INT nY =m_ptJeton[i].y;

			//�滭����
			m_ImageScore.TransDrawImage(pDC,nX,nY,m_ImageScore.GetWidth()/3,
				m_ImageScore.GetHeight(),m_ImageScore.GetWidth()/3*iPos,0,RGB(255,0,255));

			//��������
			CFont ViewFont;
			ViewFont.CreateFont(-12,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����"));
	
			//���� DC
			pDC->SetTextAlign(TA_CENTER);
			CFont * pOldFont=pDC->SelectObject(&ViewFont);

			//�滭��Ϣ
			TCHAR tc[LEN_ACCOUNTS]=TEXT("");
			_sntprintf(tc,CountArray(tc),SCORE_STRING,m_lTableScore[i]);
			CRect rcDraw(nX,nY+6,nX+73,nY+28);
			DrawTextString(pDC,tc,RGB(250,250,0),RGB(0,0,0),&rcDraw);

			//������Դ
			pDC->SelectObject(pOldFont);
			ViewFont.DeleteObject();
		}
	}

	//��ʾ̯�Ʊ�־
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		if (m_bOpenCardUser[i]==TRUE && i!=2) m_PngOpenTag.AlphaDrawImage(pDC,m_ptOpenTag[i].x,m_ptOpenTag[i].y,255);
	}

	//�滭�˿�
	if (m_SendCardItemArray.GetCount()>0)
	{
		//��������
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//��ȡ��С
		int nItemWidth=m_ImageCard.GetWidth()/13;
		int nItemHeight=m_ImageCard.GetHeight()/5;

		//�滭�˿�
		int nXPos=m_SendCardPos.x-nItemWidth/2;
		int nYPos=m_SendCardPos.y-nItemHeight/2+10;
		m_ImageCard.TransDrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
	}

	//��ʾ����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bOxValue[i]!=0xff)
		{
			if(m_bShow || i==MY_VIEW_CHAIRID)
			{
				INT nXpt = m_ptValue[i].x;
				INT nypt = m_ptValue[i].y;
				if(m_bOxValue[i]!=0)
				{
					m_ImageOxValue.AlphaDrawImage(pDC,nXpt,nypt,255);
					if(m_bOxValue[i]==10 || m_bOxValue[i]>=OX_THREE_SAME)
					{
						m_ImageDoulbeOx.AlphaDrawImage(pDC,nXpt,nypt,255);
					}	
					else if(m_bOxValue[i]<10)
					{
						m_ImageNumber.AlphaDrawImage(pDC,nXpt+40,nypt+7,
							m_ImageNumber.GetWidth()/9,m_ImageNumber.GetHeight(),
							m_ImageNumber.GetWidth()/9*((int)m_bOxValue[i]-1),0,255);
					}
				}
				else if(m_bOxValue[i]==0)
				{				
					m_ImageOxValueZero.AlphaDrawImage(pDC,nXpt,nypt,255);
				}
			}
		}
	}

	//��ʷ����
	if (m_bShowScore==true)
	{
		//���ֱ���
		m_PngHistoryScore.AlphaDrawImage(pDC,nWidth-66-m_sizeHistory.cx,5,255);

		//����λ��
		INT nYBenchmark=5;
		INT nXBenchmark=nWidth-66-m_sizeHistory.cx;
		UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;

		//�滭����
		pDC->SetTextAlign(TA_LEFT|TA_TOP);

		//�滭��Ϣ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{			
			//��ȡ�û�
			IClientUserItem * pIClientUserItem=GetClientUserItem(i);			
			//test
#ifdef _DEBUG
			//pIClientUserItem = GetClientUserItem(MYSELF_VIEW_ID);
#endif
			//end test
			if ((pIClientUserItem==NULL)||(m_pHistoryScore[i]==NULL)) continue;

			//��������
			COLORREF TextColor;

			//λ�ü���
			INT nY1=27,nY2=nY1+18,nSpa=20;
			CRect rcAccounts(nXBenchmark+4,nYBenchmark+nY1+i*nSpa,nXBenchmark+68+7,nYBenchmark+nY2+i*nSpa);
			CRect rcTurnScore(nXBenchmark+70+15,nYBenchmark+nY1+i*nSpa,nXBenchmark+121+15,nYBenchmark+nY2+i*nSpa);
			CRect rcCollectScore(nXBenchmark+140,nYBenchmark+nY1+i*nSpa,nXBenchmark+191,nYBenchmark+nY2+i*nSpa);

			//������Ϣ
			TCHAR szTurnScore[16]=TEXT(""),szCollectScore[16]=TEXT("");
			_sntprintf(szTurnScore,CountArray(szTurnScore),SCORE_STRING,m_pHistoryScore[i]->lTurnScore);
			_sntprintf(szCollectScore,CountArray(szCollectScore),SCORE_STRING,m_pHistoryScore[i]->lCollectScore);

			//�滭��Ϣ
			if(i==2) TextColor = RGB(255,255,0);
			else TextColor = RGB(255,255,255);
			m_ViewFontEx.DrawText(pDC,pIClientUserItem->GetNickName(),&rcAccounts,TextColor,nFormat);
			
			if(i==2) TextColor = RGB(255,153,0);
			else TextColor = RGB(0,255,153);
			m_ViewFontEx.DrawText(pDC,szTurnScore,&rcTurnScore,TextColor,nFormat);
			m_ViewFontEx.DrawText(pDC,szCollectScore,&rcCollectScore,TextColor,nFormat);
		}
	}

	m_JettonView.DrawJettonView(pDC,&m_ViewFontEx);
	return;
}

//�滭ʱ��
void CGameClientView::DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
//#ifdef _DEBUG
//	return;
//#endif
	//��ȡ����
	const INT nNumberHeight=m_ImageTimeNumber.GetHeight();
	const INT nNumberWidth=m_ImageTimeNumber.GetWidth()/10;

	//������Ŀ
	LONG lNumberCount=0;
	WORD wNumberTemp=wTime;
	do
	{
		lNumberCount++;
		wNumberTemp/=10;
	} while (wNumberTemp>0L);

	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2+1;
	INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

	//�滭����
	CSize SizeTimeBack(m_ImageTimeBack.GetWidth(),m_ImageTimeBack.GetHeight());
	m_ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=wTime%10;
		m_ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	}

	return;
}

//��ʾ��Ϣ
void CGameClientView::SetUserAction(bool bWait)
{
	if(m_bUserAction!=bWait)
	{
		m_bUserAction=bWait;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	//����ʱ��
	if(bWait)
	{
		SetTimer(IDI_USER_ACTION,TIME_USER_ACTION,NULL);
	}
	else 
	{
		KillTimer(IDI_USER_ACTION);
	}

	return;
}

//�����ж�
bool CGameClientView::IsDispatchCard()
{
	return m_SendCardItemArray.GetCount()>0;
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

	SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

	//ɾ������
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//�����˿�
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//���ñ���
	tagSendCardItem SendCardItem;
	SendCardItem.cbCardData=cbCardData;
	SendCardItem.wChairID=wChairID;

	//��������
	m_SendCardItemArray.Add(SendCardItem);

	//��������
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//����λ��
		m_SendCardPos.x=m_SendStartPos.x;
		m_SendCardPos.y=m_SendStartPos.y;

		//λ�Ƽ���
		int nXCount=abs(m_SendEndingPos[SendCardItem.wChairID].x-m_SendStartPos.x)/SEND_PELS;
		int nYCount=abs(m_SendEndingPos[SendCardItem.wChairID].y-m_SendStartPos.y)/SEND_PELS;

		//���ô���
		m_wSendIndex=0;
		m_wSendCount=__max(1,__max(nXCount,nYCount));

		//���ö�ʱ��
		SetTimer(IDI_SEND_CARD,TIME_SENDSPEED,NULL);

		//��������
		CGameFrameEngine * pGameClientEngine=CGameClientEngine::GetInstance();
		if(pGameClientEngine!=NULL)
		pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//������ע
void CGameClientView::SetUserTableScore(WORD wChairID, SCORE lTableScore,bool bMove)
{
	//��������
	if (wChairID<GAME_PLAYER) 
	{
		if(bMove)DrawMoveAnte(wChairID,lTableScore);
		else m_lTableScore[wChairID]=lTableScore;
	}
	else ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//���½���
	InvalidGameView(0,0,0,0);
	return;
}

//ׯ�ұ�־
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	m_wBankerUser=wBankerUser;

	//���½���
	InvalidGameView(0,0,0,0);
	return;
}

//�ȴ���ׯ
void CGameClientView::SetWaitCall(BYTE bCallUser)
{
	m_bCallUser=bCallUser;

	//���½���
	InvalidGameView(0,0,0,0);

	return ;
}

//�ȴ���־
void CGameClientView::SetWaitInvest(bool bInvest)
{
	if(m_bInvest!=bInvest)
	{
		m_bInvest=bInvest;

		//���½���
		InvalidGameView(0,0,0,0);
	}
	return;
}

//������Ϣ
//void CGameClientView::SetScoreInfo(LONGLONG lTurnMaxScore,LONGLONG lTurnLessScore)
//{
//	m_lTurnMaxScore=lTurnMaxScore;
//	//m_lTurnLessScore=lTurnLessScore;
//
//	//���½���
//	InvalidGameView(0,0,0,0);
//	return;
//}

//��ʾ����
void CGameClientView::DisplayTypeEx(bool bShow)
{
	if(m_bShow!=bShow)
	{
		m_bShow=bShow;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return;
}

//��ʾ����
void CGameClientView::SetUserOxValue(WORD wChiarID,BYTE bValue)
{
	//ţţ����
	m_bOxValue[wChiarID]=bValue;

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//��ʾ̯��
void CGameClientView::ShowOpenCard(WORD wChiarID,BOOL bShow)
{
	//̯���û�
	m_bOpenCardUser[wChiarID]=bShow;

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//��ʾ����
void CGameClientView::ShowScore(WORD wChairID,bool bShow)
{
	//��ʾ��־
	m_bShowSocre[wChairID]=bShow;

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		m_ViewFontEx.DrawText(pDC,pszString,&rcDraw,crFrame,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�滭����
	rcDraw.CopyRect(lpRect);
	pDC->SetTextColor(crText);
	m_ViewFontEx.DrawText(pDC,pszString,&rcDraw,crText,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//���ְ�ť
VOID CGameClientView::OnBnClickedScore()
{
	//���ñ���
	m_bShowScore=!m_bShowScore;

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btScore.SetButtonImage((m_bShowScore==true)?IDB_BT_CLOSE_SCORE:IDB_BT_SHOW_SCORE,hInstance,false,false);

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//���û���
bool CGameClientView::SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���ñ���
	m_pHistoryScore[wChairID]=pHistoryScore;

	//���½���
	InvalidGameView(0,0,0,0);

	return true;
}

//���ƶ���, 0: ��ע����, 1: ��ע����, 2: ��ע�ϲ�����ע, 3: Ӯ���ճ���
void CGameClientView::DrawMoveAnte( WORD wViewChairID, SCORE lTableScore)
{
	//��������
	int nAnimeStep = 60;

	m_JettonStatus[wViewChairID].wMoveIndex = 0;
	m_lMoveGold[wViewChairID]=lTableScore;

	m_JettonStatus[wViewChairID].ptFrom = m_ptCard[wViewChairID]; 
	m_JettonStatus[wViewChairID].ptDest = m_ptJeton[wViewChairID];
	m_JettonStatus[wViewChairID].ptCourse= m_ptCard[wViewChairID];
	m_JettonStatus[wViewChairID].lGold =lTableScore;

	//λ�Ƽ���
	INT nXCount=abs(m_JettonStatus[wViewChairID].ptDest.x-m_JettonStatus[wViewChairID].ptFrom.x)/nAnimeStep+rand()%8;
	INT nYCount=abs(m_JettonStatus[wViewChairID].ptDest.y-m_JettonStatus[wViewChairID].ptFrom.y)/nAnimeStep+rand()%8;
	m_JettonStatus[wViewChairID].wMoveCount = __max(1,__max(nXCount,nYCount));;

	//����ʱ��
	if(!m_bJettonAction)
	{
		m_bJettonAction = true;
		SetTimer(IDI_JETTON_ACTION,50,NULL);	//--------��ǰ��50
	}

	return;
}

//�ƶ�����
bool CGameClientView::MoveJetton()
{
	bool bAllClean = true;

	//���ñ���
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		//�ƶ�����
		if(m_JettonStatus[i].wMoveIndex<m_JettonStatus[i].wMoveCount)
		{
			bAllClean = false;
			m_JettonStatus[i].wMoveIndex++;
			WORD wMoveIndex = m_JettonStatus[i].wMoveIndex;
			WORD wMoveCount = m_JettonStatus[i].wMoveCount;
			m_JettonStatus[i].ptCourse.x = m_JettonStatus[i].ptFrom.x + (m_JettonStatus[i].ptDest.x-m_JettonStatus[i].ptFrom.x)*wMoveIndex/wMoveCount;
			m_JettonStatus[i].ptCourse.y = m_JettonStatus[i].ptFrom.y + (m_JettonStatus[i].ptDest.y-m_JettonStatus[i].ptFrom.y)*wMoveIndex/wMoveCount;
		}
		else if(m_JettonStatus[i].wMoveCount>0)
		{
			//���봦��
			m_lTableScore[i] = m_JettonStatus[i].lGold ;

			//������Ϣ
			m_lMoveGold[i]=0;
			ZeroMemory(&m_JettonStatus[i],sizeof(m_JettonStatus[i]));
		}
	}

	if(bAllClean) m_bJettonAction = false;

	//���½���
	InvalidGameView(0,0,0,0);

	return !bAllClean;
}

//�����û�ͼ��
void CGameClientView::DrawUserReadyEx(CDC *pDC,int nXPos,int nYPos)
{
	m_PngReady.DrawImage(pDC,nXPos,nYPos);
}


//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//���ʹ���
	switch(nIDEvent)
	{
	case IDI_JETTON_ACTION:	//���붯��
		{
			bool bKillTime = true;

			//�����ƶ�
			if(m_bJettonAction && MoveJetton()) bKillTime = false;

			//�����ƶ�
			//if(m_bCardAction && MoveCard()) bKillTime = false;

			//ɾ����ʱ��
			if(bKillTime)
			{
				m_bJettonAction = false;
				//m_bCardAction = false;
				KillTimer(IDI_JETTON_ACTION);
			}

			return;
		}
	case IDI_UPDATA_GAME:	//������Ϸ
		{
			m_cbIndex++;

			//���½���
			InvalidGameView(0,0,0,0);

			return ;
		}
	case IDI_SEND_CARD:		//�ƶ��˿�
		{
			//��������
			m_wSendIndex++;

			//����λ��
			tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];
			if(pSendCardItem->wChairID%2!=0)
				m_SendCardPos.x=m_SendStartPos.x+(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)*m_wSendIndex/m_wSendCount;
			else
				m_SendCardPos.y=m_SendStartPos.y+(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)*m_wSendIndex/m_wSendCount;

			//ֹͣ�ж�
			if (m_wSendIndex>=m_wSendCount)
			{
				//��ȡ�˿�
				BYTE cbCardData[MAX_COUNT];
				WORD wChairID=pSendCardItem->wChairID;
				BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

				//�����˿�
				cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
				m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);

				//ɾ������
				m_SendCardItemArray.RemoveAt(0);

				//��������
				if (m_SendCardItemArray.GetCount()>0)
				{
					//��������
					tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

					//����λ��
					m_SendCardPos.x=m_SendStartPos.x;
					m_SendCardPos.y=m_SendStartPos.y;

					//λ�Ƽ���
					int nXCount=abs(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)/SEND_PELS;
					int nYCount=abs(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)/SEND_PELS;

					//���ô���
					m_wSendIndex=0;
					m_wSendCount=__max(1,__max(nXCount,nYCount));

					//��������
					CGameFrameEngine * pGameClientEngine=CGameClientEngine::GetInstance();
					if(pGameClientEngine!=NULL)
					pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
				}
				else 
				{
					//��ɴ���
					KillTimer(IDI_SEND_CARD);
					SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);
				}
			}
			
			//���½���
			InvalidGameView(0,0,0,0);

			return ;
		}
	case IDI_USER_ACTION: 	//�û�����
		{
			//��������
			m_bUserAction = false;
			KillTimer(IDI_USER_ACTION);

			//���½���
			InvalidGameView(0,0,0,0);

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
//����Ա����
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
