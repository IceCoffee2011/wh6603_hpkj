#include "stdafx.h"
#include "Gameplaza.h"
#include "TzhxxDlg.h"
#include <map>

//TCHAR szHaoma[1024000] = TEXT("");
char szHaoma[512000] = "";
char szHaomaTemp[102400] = "";

static const int wnd_width = 483;
static const int wnd_height = 328;

static const int edit_x = 92;
static const int edit_y = 50;
static const int edit_width = 374;
static const int edit_height = 72;

//行宽
static const int row_width = 140;
//行高
static const int row_height = 23;
//第一列
static const int col_1_x = 85;
static const int row_top_y = 126;
//玩家账号
static const int wanjiazhanghao_y = row_top_y;
static CRect rc_wanjiazhanghao(col_1_x, wanjiazhanghao_y,
							   col_1_x + row_width, wanjiazhanghao_y + row_height);
//订单号
static const int dingdanhao_y = row_top_y + row_height;
static CRect rc_dingdanhao(col_1_x, dingdanhao_y,
						   col_1_x + row_width, dingdanhao_y + row_height);
//期号
static const int qihao_y = row_top_y + row_height*2;
static CRect rc_qihao(col_1_x, qihao_y,
					  col_1_x + row_width, qihao_y + row_height);
//彩种
static const int caizhong_y = row_top_y + row_height*3;
static CRect rc_caizhong(col_1_x, caizhong_y,
						 col_1_x + row_width, caizhong_y + row_height);
//玩法
static const int wanfa_y = row_top_y + row_height*4;
static CRect rc_wanfa(col_1_x, wanfa_y,
					  col_1_x + row_width, wanfa_y + row_height);
//开奖号码
static const int kaijianghaoma_y = row_top_y + row_height*5;
static CRect rc_kaijianghaoma(col_1_x, kaijianghaoma_y,
							  col_1_x + row_width+10, kaijianghaoma_y + row_height);

//下单时间
static const int xiadanshijian_y = row_top_y + row_height*6;
static CRect rc_xiadanshijian(col_1_x, xiadanshijian_y,
							  col_1_x + row_width, xiadanshijian_y + row_height);

//状态
static const int zhuangtai_y = row_top_y + row_height*7;
static CRect rc_zhuangtai(col_1_x, zhuangtai_y,
						  col_1_x + row_width, zhuangtai_y + row_height);

//第一列
static const int col_2_x = 313;
//投注数量
static const int touzhushuliang_y = row_top_y;
static CRect rc_touzhushuliang(col_2_x, touzhushuliang_y,
							   col_2_x + row_width, touzhushuliang_y + row_height);
//单注金额
static const int danzhujine_y = row_top_y + row_height;
static CRect rc_danzhujine(col_2_x, danzhujine_y,
						   col_2_x + 60, danzhujine_y + row_height);

//倍数
static const int beishu_y = row_top_y + row_height;
static CRect rc_beishu(col_2_x + 100, beishu_y,
					   col_2_x + 100 + 50, beishu_y + row_height);

//投注总额
static const int touzhuzonge_y = row_top_y + row_height*2;
static CRect rc_touzhuzonge(col_2_x, touzhuzonge_y,
							col_2_x + row_width, touzhuzonge_y + row_height);


//中奖注数
static const int zhongjiangzhushu_y = row_top_y + row_height*3;
static CRect rc_zhongjiangzhushu(col_2_x, zhongjiangzhushu_y,
								 col_2_x + row_width, zhongjiangzhushu_y + row_height);

//单注奖金
static const int danzhujiangjin_y = row_top_y + row_height*4;
static CRect rc_danzhujiangjin(col_2_x, danzhujiangjin_y,
							   col_2_x + row_width, danzhujiangjin_y + row_height);


//中奖金额
static const int zhongjiangjine_y = row_top_y + row_height*5;
static CRect rc_zhongjiangjine(col_2_x, zhongjiangjine_y,
							   col_2_x + row_width, zhongjiangjine_y + row_height);


//返点
static const int fandian_y = row_top_y + row_height*6;
static CRect rc_fandian(col_2_x - 30, fandian_y,
						col_2_x + 50, fandian_y + row_height);

//返点金额
static const int fandianjine_y = row_top_y + row_height*6;
static CRect rc_fandianjine(col_2_x + 102, fandianjine_y,
							col_2_x + 102 + 60, fandianjine_y + row_height);

//盈亏
static const int yingkui_y = row_top_y + row_height*7;
static CRect rc_yingkui(col_2_x, yingkui_y,
						col_2_x + row_width, yingkui_y + row_height);

// CTzhxxDlg 对话框

IMPLEMENT_DYNAMIC(CTzhxxDlg, CDialog)

CTzhxxDlg::CTzhxxDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTzhxxDlg::IDD, pParent)
, m_id(0)
, b_hideName(false)
, m_dingdanhao(0)
, m_gametype(0)
, m_gamekind(0)
, m_state(0)
, m_touzhushuliang(0)
, m_danzhujine(0.0)
, m_beishu(0)
, m_touzhuzonge(0.0)
, m_zhongjiangzhushu(0)
, m_danzhujiangjin(0.0)
, m_zhongjiangjine(0.0)
, m_fandian(0.0)
, m_fandianjine(0.0)
, m_yinhkui(0.0)
, m_bmpBk(NULL)
{
	// 	ZeroMemory(&szTempHaoma,sizeof(szTempHaoma));
	byte szInput[8]={0};
	byte  szOutput[8]={0};
	int nRet = CWHService::LZCompressData(szInput,8,szOutput,8);

}

CTzhxxDlg::~CTzhxxDlg()
{
	if (m_bmpBk != NULL) 
	{
		m_bmpBk = NULL;
		delete m_bmpBk;
	}
}

void CTzhxxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HAOMA, m_editHaoma);
	DDX_Control(pDX, IDC_RICHEDIT_HAOMA, m_RichEditHaoma);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX,IDC_BTN_TOOLTIP,m_btnKaiJiang);
}


BEGIN_MESSAGE_MAP(CTzhxxDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CTzhxxDlg::OnBnClickedBtnClose)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CTzhxxDlg 消息处理程序

void CTzhxxDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);

	CDC cacheDC;
	cacheDC.CreateCompatibleDC(&dc);
	CBitmap cacheBmp;
	cacheBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap *pOldCacheBmp = cacheDC.SelectObject(&cacheBmp);

	Graphics graphics(cacheDC.m_hDC);
	if (m_bmpBk != NULL) 
	{
		graphics.DrawImage(m_bmpBk, Rect(0, 0, rect.Width(), rect.Height()), 0, 0, m_bmpBk->GetWidth(), m_bmpBk->GetHeight(), UnitPixel);

	}

	int oldBkMode = cacheDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = cacheDC.SelectObject(&m_font);
	COLORREF oldTextColor = cacheDC.SetTextColor(RGB(88, 78, 77));

	CString strTmp;
	cacheDC.DrawText(b_hideName?_T("匿名"):m_wanjiazhanghao, rc_wanjiazhanghao, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%d"), m_dingdanhao);
	cacheDC.DrawText(strTmp, rc_dingdanhao, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_qihao, rc_qihao, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(theGameType[m_gametype], rc_caizhong, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(theGameKind[(m_gametype << 16) | (unsigned short)m_gamekind], rc_wanfa, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_kaijianghaoma, rc_kaijianghaoma, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	cacheDC.DrawText(m_xiadanshijian, rc_xiadanshijian, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	if (0 == m_state)
	{
		bool bKaijiang = false;
		CString strNextQishu;
		if(m_gametype == CZChongQingSSC)
		{
			strNextQishu = m_chqRule.GetNextExpect();
		}
		else if(m_gametype == CZ_TianJinSSC)
		{
			strNextQishu = m_jxRule.GetNextExpect();
		}
		else if(m_gametype == CZXinJiangSSC)
		{
			strNextQishu = m_xjRule.GetNextExpect();
		}
		else if(m_gametype == CZ_WUFEN_CAI)
		{
			strNextQishu = m_wfcRule.GetNextExpect();
		}
		else if(m_gametype == CZ_FENFEN_CAI)
		{
			strNextQishu = m_ffcRule.GetNextExpect();
		}
		else if(m_gametype == CZ_HGYDWFC)
		{
			strNextQishu = m_hgRule.GetNextExpect();
		}
		else if(m_gametype == CZCQ11Xuan5)
		{
			strNextQishu = m_chq11x5Rule.GetNextExpect();
		}
		else if(m_gametype == CZGD11Xuan5)
		{
			strNextQishu = m_gd11x5Rule.GetNextExpect();
		}
		else if(m_gametype == CZJX11Xuan5)
		{
			strNextQishu = m_jx11x5Rule.GetNextExpect();
		}
		else if(m_gametype == CZSD11Xuan5)
		{
			strNextQishu = m_sd11x5Rule.GetNextExpect();
		}
		else if(m_gametype == CZ_PK10)
		{
			strNextQishu = m_pk10Rule.GetNextExpect();
		}
		else if(m_gametype == CZHLJ11Xuan5)
		{
			strNextQishu = m_hlj11x5Rule.GetNextExpect();
		}
		else if(m_gametype == CZ_QiXingCai)
		{
			strNextQishu = m_qxcRule.GetNextExpect();
		}
		else if(m_gametype == CZPaiLie3)
		{
			strNextQishu = m_dlgPaiLie3.GetNextExpect();
		}
		else if(m_gametype == CZ3D)
		{
			strNextQishu = m_dlg3D.GetNextExpect();
		}

		__int64  nQishu = 0L;
		nQishu = _ttoi64(strNextQishu);
		nQishu-=1;
		CString strNowQishu;

		strNowQishu.Format(L"%I64d",nQishu);
		CString strQihao;
		strQihao.Format(L"%s", m_qihao);

		if(strQihao<strNowQishu)
		{
			bKaijiang = true;
		}

		if(bKaijiang)
			strTmp = _T("未派奖");
		else
		{
			/*			if(pTouzhuLogRet->n_t_zhuihao>=1)
			strTmp = _T("追号");
			else
			*/				strTmp = _T("未开奖");
		}
	}
	else if(1 == m_state)
	{
		if(0 == m_zhongjiangzhushu)
		{
			strTmp = _T("未中奖");
		}
		else
		{
			strTmp = _T("中奖");
		}
	}
	else if(2 == m_state)
	{
		strTmp = _T("撤单");
	}
	cacheDC.DrawText(strTmp, rc_zhuangtai, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	strTmp.Format(_T("%d"), m_touzhushuliang);
	cacheDC.DrawText(strTmp, rc_touzhushuliang, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.3lf"), m_danzhujine);
	cacheDC.DrawText(strTmp, rc_danzhujine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%d"), m_beishu);
	cacheDC.DrawText(strTmp, rc_beishu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.3lf"), m_touzhuzonge);
	cacheDC.DrawText(strTmp, rc_touzhuzonge, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%d"), m_zhongjiangzhushu);
	cacheDC.DrawText(strTmp, rc_zhongjiangzhushu, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	if(m_danzhujine == 0.02)
	{
		strTmp.Format(_T("%.3lf"), m_danzhujiangjin/100);
	}
	else if(m_danzhujine == 0.002)
	{
		strTmp.Format(_T("%.3lf"), m_danzhujiangjin/1000);
	}
	else if(m_danzhujine == 0.2)
	{
		strTmp.Format(_T("%.3lf"), m_danzhujiangjin/10);
	}
	else if(m_danzhujine == 2.00)
	{
		strTmp.Format(_T("%.3lf"), m_danzhujiangjin);
	}


	cacheDC.DrawText(strTmp, rc_danzhujiangjin, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.3lf"), m_zhongjiangjine);

// 	if(m_danzhujine == 0.002)
// 	{
// 		strTmp.Format(_T("%.3lf"), m_beishu * m_zhongjiangzhushu* m_danzhujiangjin/1000);
// 	}
// 	if(m_danzhujine == 0.02)
// 	{
// 		strTmp.Format(_T("%.3lf"), m_beishu * m_zhongjiangzhushu* m_danzhujiangjin/100);
// 	}
// 	else if(m_danzhujine == 0.2)
// 	{
// 		strTmp.Format(_T("%.3lf"), m_beishu * m_zhongjiangzhushu* m_danzhujiangjin/10);
// 	}
// 	else if(m_danzhujine == 2.00)
// 	{
// 		strTmp.Format(_T("%.3lf"), m_beishu * m_zhongjiangzhushu* m_danzhujiangjin);
// 	}

	cacheDC.DrawText(strTmp, rc_zhongjiangjine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.2lf"), m_fandian * 100);
	cacheDC.DrawText(strTmp, rc_fandian, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.2lf"), m_fandianjine);
	cacheDC.DrawText(strTmp, rc_fandianjine, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	strTmp.Format(_T("%.3lf"), m_yinhkui);
	cacheDC.DrawText(strTmp, rc_yingkui, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	cacheDC.SetTextColor(oldTextColor);
	cacheDC.SelectObject(pOldFont);
	cacheDC.SetBkMode(oldBkMode);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &cacheDC, 0, 0, SRCCOPY);

	cacheDC.SelectObject(pOldCacheBmp);
	cacheDC.DeleteDC();
	cacheBmp.DeleteObject();
}


void CTzhxxDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ÔÚ´ËÌí¼ÓÏûÏ¢´¦Àí³ÌÐò´úÂëºÍ/»òµ÷ÓÃÄ¬ÈÏÖµ
	//Ê¹´°¿Ú¿ÉÒÔÍÏ¶¯
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CTzhxxDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTzhxxDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	if (m_btnClose.GetSafeHwnd() != NULL)
	{
		m_btnClose.SetWindowPos(NULL, 
			wnd_width - m_btnClose.Width() - 5, 
			5, m_btnClose.Width(), m_btnClose.Height(), SWP_NOZORDER);
	}

 	if (m_btnKaiJiang.GetSafeHwnd() != NULL)
 	{
 		m_btnKaiJiang.SetWindowPos(NULL, rc_kaijianghaoma.left, rc_kaijianghaoma.top, rc_kaijianghaoma.Width(), rc_kaijianghaoma.Height(), SWP_NOZORDER);
 	}
	if (m_RichEditHaoma.GetSafeHwnd() != NULL)
	{
		m_RichEditHaoma.SetWindowPos(NULL, edit_x, edit_y, edit_width, edit_height, SWP_NOZORDER);
	}
}

void CTzhxxDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		m_MissionManager.InsertMissionItem(this);
		SendToServer(1);

	}
	else
	{
		if(GetMissionManager()!=NULL)
			GetMissionManager()->DeleteMissionItem(this);		

	}
}

void CTzhxxDlg::OnBnClickedBtnClose()
{
	CDialog::OnOK();
}

BOOL CTzhxxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bmpBk = new Bitmap(CBmpUtil::GetExePath() + _T("skin\\tzhxx.png"));

	m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _T("Arial")); 

	m_btnClose.SetImage(CBmpUtil::GetExePath() + _T("skin\\close_bt.png"));

	//SetWindowPos(NULL, 0, 0, wnd_width, wnd_height, SWP_NOMOVE|SWP_NOZORDER);
	SetWindowPos(NULL,0,0,m_bmpBk->GetWidth(),m_bmpBk->GetHeight(),SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);


	CenterWindow();
	//提示控件
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	//m_btnKaiJiang.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


CString	CTzhxxDlg::ChangeStringToT(CStringA strInput)
{
	CString  strOutput;
	USES_CONVERSION;
	strOutput = A2T(strInput);

	return strOutput;
}
//连接事件
bool CTzhxxDlg::OnEventMissionLink(INT nErrorCode,INT nSocketID)
{
	//错误处理
	if (nErrorCode!=0L)
	{
		//设置控件
		//	m_btOk.EnableWindow(TRUE);

		//显示提示
		CInformation Information(this);
		//Information.ShowMessageBox(TEXT("尝试了所有的服务器都无法成功连接服务器，请留意网站维护公告！"),MB_ICONERROR);
	}
	else
	{

		CMD_GP_GetTouzhu GetTouzhuXX;
		ZeroMemory(&GetTouzhuXX,sizeof(GetTouzhuXX));

		GetTouzhuXX.n_t_id = m_id;

		ASSERT(GetMissionManager()!=NULL);
		GetMissionManager()->SendData(MDM_GP_USER_SERVICE,SUB_GP_GET_TOU_ZHU_XX,&GetTouzhuXX,sizeof(GetTouzhuXX));




	}

	return true;
}

//关闭事件
bool CTzhxxDlg::OnEventMissionShut(BYTE cbShutReason)
{
	//关闭处理
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{

	}

	return true;
}
//消息过虑
BOOL CTzhxxDlg::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//读取事件
bool CTzhxxDlg::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{

	//命令处理
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_GET_TOU_ZHU_XX_RET:
			{
				//	return true;

				ASSERT(wDataSize == sizeof(CMD_GP_GetTouzhuRet));
				if(wDataSize!=sizeof(CMD_GP_GetTouzhuRet)) return false;
				CMD_GP_GetTouzhuRet* pGetTouzhuRet = (CMD_GP_GetTouzhuRet*)pData;


				if( pGetTouzhuRet->n_Index == 0)
				{
					memset(szHaomaTemp, 0, sizeof(szHaomaTemp));
					memcpy(szHaomaTemp,	pGetTouzhuRet->s_t_touzhuhaoma,pGetTouzhuRet->nHaoMaLen);

				}
				else
				{
					int nPos = sizeof(pGetTouzhuRet->s_t_touzhuhaoma)* pGetTouzhuRet->n_Index;
					memcpy(szHaomaTemp+nPos,	pGetTouzhuRet->s_t_touzhuhaoma,pGetTouzhuRet->nHaoMaLen);

				}
				if(pGetTouzhuRet->n_t_End == 1)
				{
					return true;
				}
				memset(szHaoma, 0, sizeof(szHaoma));
				int nLeng =(sizeof(pGetTouzhuRet->s_t_touzhuhaoma)* pGetTouzhuRet->n_Index+pGetTouzhuRet->nHaoMaLen);



				int nRet =  CWHService::LZUnCompressData((byte*)szHaomaTemp, nLeng+1, (byte*)szHaoma, sizeof(szHaoma));



				m_RichEditHaoma.SetSel(-1, -1);

				if(pGetTouzhuRet->n_t_gamekind == NiuNiu_Num||pGetTouzhuRet->n_t_gamekind == NiuNiu_None)
				{
					CString strInsert;
					CString strHaoma = ChangeStringToT(szHaoma);

					int nIndex = 0;
					do
					{
						CString strHaomaTemp;

						if(!AfxExtractSubString(strHaomaTemp, strHaoma, nIndex++, '|'))
							break;


						if(strHaomaTemp.IsEmpty())
							break;

						int nCount = 0;
						int nLength = strHaomaTemp.GetLength()/3+1;
						do
						{
							CString strHaomaSingle;

							if(!AfxExtractSubString(strHaomaSingle, strHaomaTemp, nCount++, ','))
								break;


							if(strHaomaSingle.IsEmpty())
								break;
							int nSingleLeng = strHaomaSingle.GetLength();
							nLength --;
							strInsert += GetNiuString(strHaomaSingle);
							if(nLength>0)
								strInsert += L",";
						}
						while(nCount < 10);
						strInsert += L"|";
					}
					while(nIndex < 80000);
					m_RichEditHaoma.ReplaceSel(strInsert);
				}
				else if((pGetTouzhuRet->n_t_gametype == CZXingYun28 && (pGetTouzhuRet->n_t_gamekind == enXY28_DXDS || pGetTouzhuRet->n_t_gamekind == enXY28_JiZhi))||(pGetTouzhuRet->n_t_gametype == CZ_PK10 && pGetTouzhuRet->n_t_gamekind == WF_DXDS))
				{
					CString strAddHaoma;
					CString strHaoMa = ChangeStringToT(szHaoma);
					int nAddLength = strHaoMa.GetLength();
					int nAddIndex = 0;
					int nDanIndex = 0;
					CString strDanzhu;
					do 
					{
						if(!AfxExtractSubString(strDanzhu, strHaoMa, nDanIndex++, '|'))
							break;
						if(strDanzhu.IsEmpty())
							break;
						do 
						{
							CString strAdd;
							if(!AfxExtractSubString(strAdd, strDanzhu, nAddIndex++, ','))
								break;
							if(nAddIndex!=1)
							{
								strAddHaoma += _T(",");
							}
							int nNum = _ttoi(strAdd);
							CString strDesc = GetDxdshStringDesc(nNum);
							strAddHaoma += strDesc;

						} while (nAddIndex<1000);
						strAddHaoma += L"|";
						nAddIndex=0;
					} while (nDanIndex<1000);
					m_RichEditHaoma.ReplaceSel(strAddHaoma);

				}
				else if((pGetTouzhuRet->n_t_gametype == CZChongQingSSC||pGetTouzhuRet->n_t_gametype == CZ_TianJinSSC||pGetTouzhuRet->n_t_gametype == CZXinJiangSSC||pGetTouzhuRet->n_t_gametype == CZ_FENFEN_CAI||pGetTouzhuRet->n_t_gametype == CZ_WUFEN_CAI||pGetTouzhuRet->n_t_gametype == CZ_HGYDWFC)&&(pGetTouzhuRet->n_t_gamekind == SSC_LongHu))
				{
					CString strHaoMa = ChangeStringToT(szHaoma);

					CString strAddHaoma,strTe;
					AfxExtractSubString(strTe, strHaoMa, 0, ',');
					if(strTe.IsEmpty())
					{
						return true;
					}
					strTe.Replace(L"4",L"虎");
					strTe.Replace(L"5",L"和");
					strTe.Replace(L"6",L"龙");
					strAddHaoma+=strTe;
					strAddHaoma += _T(",");
					AfxExtractSubString(strTe, strHaoMa, 1, ',');
					strTe = strTe.Left(strTe.GetLength()-1);
					CString str[10] = {_T("万千"),_T("万百"),_T("万十"),_T("万个"),_T("千百"),_T("千十"),_T("千个"),_T("百十"),_T("百个"),_T("十个")};
					CString strTemp[10] = {_T("11000"),_T("10100"),_T("10010"),_T("10001"),_T("01100"),_T("01010"),_T("01001"),_T("00110"),_T("00101"),_T("00011")};
					int nTemp = 0;
					for(BYTE l = 0;l < 10;l++)
					{
						CString strWei = strTemp[l];
						if(strWei == strTe)
						{
							nTemp = l;
						}
					}
					if(nTemp < 10)
					{
						strAddHaoma += str[nTemp];
					}
					m_RichEditHaoma.ReplaceSel(strAddHaoma);

				}
				else if((pGetTouzhuRet->n_t_gametype == CZXingYun28) && (pGetTouzhuRet->n_t_gamekind == enXY28_Funny))
				{
					CString strAddHaoma;
					CString strHaoMa = ChangeStringToT(szHaoma);
					int nAddLength = strHaoMa.GetLength();
					int nAddIndex = 0;
					do 
					{
						CString strAdd;
						if(!AfxExtractSubString(strAdd, strHaoMa, nAddIndex++, '|'))
							break;
						if(nAddIndex!=1)
						{
							strAddHaoma += _T("|");
						}
						int nNum = _ttoi(strAdd);
						CString strDesc = GetFunnyDesc(nNum);
						strAddHaoma += strDesc;

					} while (nAddIndex<1000);
					
					m_RichEditHaoma.ReplaceSel(strAddHaoma);

				}
				else if((pGetTouzhuRet->n_t_gametype == CZKUAILE8 && (pGetTouzhuRet->n_t_gamekind == enBJK8_ShangXia || pGetTouzhuRet->n_t_gamekind == enBJK8_JiOu || pGetTouzhuRet->n_t_gamekind == enBJK8_HeZhiDXDS)))
				{
					CString strAddHaoma;
					CString strHaoMa = ChangeStringToT(szHaoma);
					int nAddLength = strHaoMa.GetLength();
					int nAddIndex = 0;
					int nDanzhuIndex = 0;
					do 
					{
						CString strDanzhu;
						if(!AfxExtractSubString(strDanzhu, strHaoMa, nDanzhuIndex++, '|'))
							break;
						if(strDanzhu.IsEmpty())
							break;
						do 
						{
							CString strAdd;
							if(!AfxExtractSubString(strAdd, strDanzhu, nAddIndex++, ','))
								break;
							if(nAddIndex!=1)
							{
								strAddHaoma += _T(",");
							}
							int nNum = _ttoi(strAdd);
							CString strDesc;
							if(pGetTouzhuRet->n_t_gamekind == enBJK8_ShangXia)
								strDesc= GetSZXDesc(nNum);
							else if(pGetTouzhuRet->n_t_gamekind == enBJK8_JiOu)
								strDesc= GetJHODesc(nNum);
							else if(pGetTouzhuRet->n_t_gamekind == enBJK8_HeZhiDXDS)
								strDesc= GetHZDXDSDesc(nNum);
							strAddHaoma += strDesc;

						} while (nAddIndex<1000);
						nAddIndex = 0;
						strAddHaoma += L"|";
					} while (nDanzhuIndex<1000);
					m_RichEditHaoma.ReplaceSel(strAddHaoma);

				}
				else if((pGetTouzhuRet->n_t_gametype == CZChongQingSSC||pGetTouzhuRet->n_t_gametype == CZ_TianJinSSC||pGetTouzhuRet->n_t_gametype == CZXinJiangSSC||pGetTouzhuRet->n_t_gametype == CZ_FENFEN_CAI||pGetTouzhuRet->n_t_gametype == CZ_WUFEN_CAI||pGetTouzhuRet->n_t_gametype == CZ_HGYDWFC) && (pGetTouzhuRet->n_t_gamekind == DaXiaoDanShuang||pGetTouzhuRet->n_t_gamekind == DaXiaoDanShuang_dw||pGetTouzhuRet->n_t_gamekind == Zonghe_DXDSLh))
				{
					CString strAddHaoma;
					CString strHaoMa = ChangeStringToT(szHaoma);
					int nAddLength = strHaoMa.GetLength();
					int nAddIndex = 0;
					int nDanzhuIndex = 0;
					do 
					{
						CString strDanzhu;
						if(!AfxExtractSubString(strDanzhu, strHaoMa, nDanzhuIndex++, '|'))
							break;
						if(strDanzhu.IsEmpty())
							break;

						do 
						{
							CString strAdd;
							if(!AfxExtractSubString(strAdd, strDanzhu, nAddIndex++, ','))
								break;
							if(nAddIndex!=1)
							{
								strAddHaoma += _T(",");
							}
							int nAddLen = strAdd.GetLength();
							for(int i = 0;i < nAddLen;i++)
							{
								CString add;
								add = strAdd.GetAt(i);
								int nNum = _ttoi(add);
								CString strDesc = GetDxdshStringDesc(nNum);
								strAddHaoma += strDesc;
							}

						} while (nAddIndex<1000);
						strAddHaoma += L"|";
						nAddIndex=0;
					} while (nDanzhuIndex<1000);
					m_RichEditHaoma.ReplaceSel(strAddHaoma);

				}
				else if((pGetTouzhuRet->n_t_gametype == CZChongQingSSC||pGetTouzhuRet->n_t_gametype == CZ_TianJinSSC||pGetTouzhuRet->n_t_gametype == CZXinJiangSSC||pGetTouzhuRet->n_t_gametype == CZ_FENFEN_CAI||pGetTouzhuRet->n_t_gametype == CZ_WUFEN_CAI||pGetTouzhuRet->n_t_gametype == CZ_HGYDWFC) && (pGetTouzhuRet->n_t_gamekind == Ren3Zu3_Fushi||pGetTouzhuRet->n_t_gamekind == Ren3Zu6_Fushi||pGetTouzhuRet->n_t_gamekind == Ren3Hunhe_Zuxuan||pGetTouzhuRet->n_t_gamekind == Ren2Zuxuan_Fushi))
				{
					int nAddIndex = 0;

					CString strHaoMa = ChangeStringToT(szHaoma);
					CString strReplace;
					do 
					{
						CString strAddHaoma;
						CString strDanzhu;
						if(!AfxExtractSubString(strDanzhu, strHaoMa, nAddIndex++, '|'))
							break;
						if(strDanzhu.IsEmpty())
							break;

						CString strT = strDanzhu.Right(5);

						AfxExtractSubString(strAddHaoma,strDanzhu,0,',');

						strAddHaoma+=TEXT(",");
						CString str;
						str= strT.GetAt(0);
						if(str ==  TEXT("1"))
						{
							strAddHaoma+=TEXT("万");
						}
						str= strT.GetAt(1);
						if(str == TEXT("1"))
						{
							strAddHaoma+=TEXT("千");
						}
						str= strT.GetAt(2);
						if(str == TEXT("1"))
						{
							strAddHaoma+=TEXT("百");
						}
						str= strT.GetAt(3);
						if(str == TEXT("1"))
						{
							strAddHaoma+=TEXT("十");
						}
						str= strT.GetAt(4);
						if(str == TEXT("1"))
						{
							strAddHaoma+=TEXT("个");
						}
						strAddHaoma += L"|";

						strReplace+= strAddHaoma;
					} while (nAddIndex<1000);
					m_RichEditHaoma.ReplaceSel(strReplace);

				}
				else if((pGetTouzhuRet->n_t_gametype == CZ3D||pGetTouzhuRet->n_t_gametype == CZPaiLie3) && (pGetTouzhuRet->n_t_gamekind == enWF_DaxiaoDanshuang))
				{
					CString strAddHaoma;
					CString strHaoMa = ChangeStringToT(szHaoma);
					int nAddLength = strHaoMa.GetLength();
					int nAddIndex = 0;
					int nDanzhuIndex = 0;
					do 
					{
						CString strDanzhu;
						if(!AfxExtractSubString(strDanzhu, strHaoMa, nDanzhuIndex++, '|'))
							break;
						if(strDanzhu.IsEmpty())
							break;

						do 
						{
							CString strAdd;
							if(!AfxExtractSubString(strAdd, strDanzhu, nAddIndex++, ','))
								break;
							if(nAddIndex!=1)
							{
								strAddHaoma += _T(",");
							}
							int nAddLen = strAdd.GetLength();
							for(int i = 0;i < nAddLen;i++)
							{
								CString add;
								add = strAdd.GetAt(i);
								int nNum = _ttoi(add);
								CString strDesc = GetDxdshStringDesc(nNum);
								strAddHaoma += strDesc;
							}

						} while (nAddIndex<1000);
						strAddHaoma += L"|";
						nAddIndex=0;
					} while (nDanzhuIndex<1000);
					m_RichEditHaoma.ReplaceSel(strAddHaoma);

				}
				else if (pGetTouzhuRet->n_t_gametype == CZ_LIUHECAI)
				{
					CString strHaoMa = ChangeStringToT(szHaoma);
					if(pGetTouzhuRet->n_t_gamekind == LiuHeCai_Tmds)
					{
						strHaoMa.Replace(L"9",L"大");
						strHaoMa.Replace(L"1",L"小");
						strHaoMa.Replace(L"3",L"单");
						strHaoMa.Replace(L"2",L"双");
					}
					else if(pGetTouzhuRet->n_t_gamekind == LiuHeCai_Tmsx)
					{
						strHaoMa.Replace(L"09",L"鼠");
						strHaoMa.Replace(L"08",L"牛");
						strHaoMa.Replace(L"07",L"虎");
						strHaoMa.Replace(L"06",L"兔");
						strHaoMa.Replace(L"05",L"龙");
						strHaoMa.Replace(L"04",L"蛇");
						strHaoMa.Replace(L"03",L"马");
						strHaoMa.Replace(L"02",L"羊");
						strHaoMa.Replace(L"01",L"猴");
						strHaoMa.Replace(L"12",L"鸡");
						strHaoMa.Replace(L"11",L"狗");
						strHaoMa.Replace(L"10",L"猪");

					}
					else if(pGetTouzhuRet->n_t_gamekind == LiuHeCai_Tmsx)
					{
						strHaoMa.Replace(L"1",L"红波");
						strHaoMa.Replace(L"2",L"蓝波");
						strHaoMa.Replace(L"3",L"绿波");
					}
					m_RichEditHaoma.ReplaceSel(strHaoMa);
				}
				else
					m_RichEditHaoma.ReplaceSel(ChangeStringToT(szHaoma));
				m_wanjiazhanghao.Format(TEXT("%s"),pGetTouzhuRet->s_t_wanjiazhanghao);
				m_dingdanhao = pGetTouzhuRet->n_t_dingdanhao;
				m_qihao.Format(L"%s",pGetTouzhuRet->s_t_qishu);
				m_gametype = pGetTouzhuRet->n_t_gametype;
				m_gamekind = pGetTouzhuRet->n_t_gamekind;
				if((m_gametype == CZGD11Xuan5)||(m_gametype == CZCQ11Xuan5)||(m_gametype == CZSD11Xuan5)||(m_gametype == CZJX11Xuan5)||(m_gametype == CZHLJ11Xuan5)||(m_gametype == CZ_PK10)||(m_gametype == CZKUAILE8))
				{
					m_kaijianghaoma.Empty();
					int nLeng = lstrlen(pGetTouzhuRet->s_t_kaijianghaoma);
					CString str;
					for(int i = 0;i < nLeng;i++)
					{
						str.Format(L"%c",pGetTouzhuRet->s_t_kaijianghaoma[i]);
						m_kaijianghaoma += str;
						if(i%2 == 1 && i!=nLeng-1)
						{
							m_kaijianghaoma += L",";
						}
					}
				}
				else if(pGetTouzhuRet->n_t_gamekind == NiuNiu_Num)
				{
					CString strHao;
					strHao.Format(L"%s",pGetTouzhuRet->s_t_kaijianghaoma);
					if(!strHao.IsEmpty())
					{
						strHao = strHao.Right(2);
						int n1 = _ttoi(strHao.Left(1));
						int n2 = _ttoi(strHao.Right(1));
						CString str;
						str.Format(L"0%d",(n1+n2)%10);

						m_kaijianghaoma.Format(L"%s（%s）",pGetTouzhuRet->s_t_kaijianghaoma,GetNiuString(str));
					}
				}
				else if(pGetTouzhuRet->n_t_gametype == CZXingYun28)
				{
					CString strKaiJiang;
					strKaiJiang.Format(L"%s",pGetTouzhuRet->s_t_kaijianghaoma);
					int nNums[3] = {0,0,0};
					int nIndex = 0;
					int nSumCount = 0;
					CString strTemp;
					for(int i = 0;i < strKaiJiang.GetLength();i++)
					{
						strTemp=strKaiJiang.GetAt(i);
						if (iswdigit(strKaiJiang.GetAt(i)) && (i%2!=0)) 
						{
							int num = _ttoi(strTemp);
							nNums[nIndex] += num;
							nSumCount++;
							if(nSumCount == 6)
							{
								while (nNums[nIndex]>=10)
								{
									nNums[nIndex]%=10;
								}
								nSumCount=0;
								nIndex++;
								if(nIndex>=3)
									break;
							}
						}
// 						else
// 						{
// 							break;
// 						}

					}
					if(!strKaiJiang.IsEmpty())
						m_kaijianghaoma.Format(L"%d+%d+%d=%d",nNums[0],nNums[1],nNums[2],nNums[0]+nNums[1]+nNums[2]);
				}
				else if(pGetTouzhuRet->n_t_gametype == CZ_QiXingCai)
				{
					CString strKaiJiang;
					strKaiJiang.Format(L"%s",pGetTouzhuRet->s_t_kaijianghaoma);

					if(!strKaiJiang.IsEmpty())
					{
						strKaiJiang = strKaiJiang.Left(4);
						for (int i = 0;i < strKaiJiang.GetLength();i++)
						{
							m_kaijianghaoma += strKaiJiang.GetAt(i);
							if(i!=3)
							{
								m_kaijianghaoma += _T(",");
							}
						}

					}
				}
				else
					m_kaijianghaoma.Format(L"%s",pGetTouzhuRet->s_t_kaijianghaoma);

				int nLength = m_kaijianghaoma.GetLength();
				m_ToolTipCtrl.SetMaxTipWidth(1000);
				m_ToolTipCtrl.AddTool(&m_btnKaiJiang,m_kaijianghaoma);
				m_btnKaiJiang.SetButtonColor(RGB(255,255,255));
				m_btnKaiJiang.SetWindowText(m_kaijianghaoma);
				CTime ct(pGetTouzhuRet->n_t_time);
				m_xiadanshijian = ct.Format(_T("%Y%m%d %H:%M:%S"));
				m_state = pGetTouzhuRet->n_t_state;
				m_touzhushuliang = pGetTouzhuRet->n_t_zhushu;
				int t_moshi = pGetTouzhuRet->n_t_moshi;

				DOUBLE fMoshi = 0.0;
				if(t_moshi == MoShi_Yuan)
				{
					fMoshi = 2.0;
				}
				else if(t_moshi == MoShi_Jiao)
				{
					fMoshi = 0.2;
				}
				else if(t_moshi == MoShi_Fen)
				{
					fMoshi = 0.02;
				}
				else if(t_moshi == MoShi_Li)
				{
					fMoshi = 0.002;
				}
				m_danzhujine =  fMoshi;
				if(pGetTouzhuRet->n_t_gametype == CZ_LIUHECAI)
				{
					m_danzhujine = 1.000;
				}

				m_beishu = pGetTouzhuRet->n_t_beishu;
				m_touzhuzonge = m_danzhujine * m_beishu * m_touzhushuliang;

				if (m_state == 1)
				{

					m_zhongjiangzhushu = pGetTouzhuRet->n_t_winzhushu;
					m_yinhkui = pGetTouzhuRet->f_t_yingkui;
				}
				else
				{
					m_zhongjiangzhushu = 0;
					m_yinhkui=0.00;
				}


				int index = (m_gametype << 16) | (unsigned short)m_gamekind;
				m_danzhujiangjin = pGetTouzhuRet->f_t_danzhujine;
				if(m_gamekind>=28&&m_gamekind<=30)
					m_danzhujiangjin = m_danzhujiangjin/6;

				if((m_gamekind == HouSanHunhe || m_gamekind == QianSanHunhe ||m_gamekind == ZhongSanHunhe)&&!m_kaijianghaoma.IsEmpty())
				{
					CString strThree;
					if(m_gamekind == HouSanHunhe)
						strThree = m_kaijianghaoma.Right(3);
					else if(m_gamekind == QianSanHunhe)
						strThree = m_kaijianghaoma.Left(3);
					else if(m_gamekind == ZhongSanHunhe)
						strThree = m_kaijianghaoma.Mid(1,3);

					string strHao;
					ChangeStringToA(strThree,strHao);
					if(GetZuxuanType(strHao) == 3)
					{
						m_danzhujiangjin *= 2;
					}
				}

// 				m_zhongjiangjine = m_zhongjiangzhushu * m_beishu * m_danzhujiangjin;
// 				if (pGetTouzhuRet->n_t_gamekind == Ren3Hunhe_Zuxuan && m_zhongjiangjine > 0)
// 				{
// 					m_zhongjiangjine = m_yinhkui + abs(m_touzhuzonge);
// 				}

				m_zhongjiangjine = min(m_zhongjiangjine,400000.00);
				if (t_moshi == MoShi_Jiao)
				{
					m_zhongjiangjine /= 10.0;
				}
				else if(t_moshi == MoShi_Fen)
				{
					m_zhongjiangjine /= 100.0;
				}
				else if(t_moshi == MoShi_Li)
				{
					m_zhongjiangjine /= 1000.0;
				}

				m_fandian = pGetTouzhuRet->f_t_fandian/100;
				m_fandianjine = m_fandian * m_touzhuzonge;

				if (m_state == 1)
				{
					m_zhongjiangjine = m_yinhkui + abs(m_touzhuzonge)-m_fandianjine;
				}
				else
				{
					m_zhongjiangjine = 0.00;
				}
				//撤销了
				if(m_state == 0)
				{
					m_fandianjine=0;
				}
				if(m_state == 2)
				{
					m_yinhkui = 0;
					m_fandianjine=0;
				}


				ZeroMemory(&szHaoma,sizeof(szHaoma));
				break;
			}

		default:
			{

				return true;
			}

		}
	}
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);

	//错误断言
	ASSERT(FALSE);

	return true;
}
CString CTzhxxDlg::GetFunnyDesc(int nNum)
{
	CString strDesc;
	switch(nNum)
	{
	case 14:
		{
			strDesc = _T("对子");
			break;
		}
	case 15:
		{
			strDesc = _T("顺子");
			break;
		}
	case 16:
		{
			strDesc = _T("豹子");
			break;
		}
	}
	return strDesc;
}

CString CTzhxxDlg::GetSZXDesc(int num)
{
	CString str;

	switch(num)
	{
	case 0:
		{
			str = _T("上");
		}
		break;
	case 1:
		{
			str = _T("中");
		}
		break;
	case 2:
		{
			str = _T("下");
		}
		break;
	}
	return str;
}
CString CTzhxxDlg::GetJHODesc(int num)
{
	CString str;

	switch(num)
	{
	case 0:
		{
			str = _T("奇");
		}
		break;
	case 1:
		{
			str = _T("和");
		}
		break;
	case 2:
		{
			str = _T("偶");
		}
		break;
	}
	return str;
}
CString CTzhxxDlg::GetHZDXDSDesc(int num)
{
	CString str;

	switch(num)
	{
	case 0:
		{
			str = _T("大单");
		}
		break;
	case 1:
		{
			str = _T("大双");
		}
		break;
	case 2:
		{
			str = _T("小单");
		}
		break;
	case 3:
		{
			str = _T("小双");
		}
		break;
	}
	return str;
}
CString CTzhxxDlg::GetDxdshStringDesc(int nNum)
{
	CString strDesc;
	switch (nNum)
	{
	case 1:
		{
			strDesc = _T("小");
			break;
		}
	case 2:
		{
			strDesc = _T("双");
			break;
		}
	case 3:
		{
			strDesc = _T("单");
			break;
		}
	case 9:
		{
			strDesc = _T("大");
			break;
		}
	case 10:
		{
			strDesc = _T("小单");
			break;
		}
	case 11:
		{
			strDesc = _T("小双");
			break;
		}
	case 12:
		{
			strDesc = _T("大单");
			break;
		}
	case 13:
		{
			strDesc = _T("大双");
			break;
		}
	case 17:
		{
			strDesc = _T("极小");
			break;
		}
	case 18:
		{
			strDesc = _T("极大");
			break;
		}
	}
	return strDesc;
}

CString CTzhxxDlg::GetNiuString(CString strNiu)
{
	CString str;
	if (strNiu == _T("00"))
	{
		str = _T("牛牛");
	}
	if (strNiu == _T("01"))
	{
		str = _T("牛一");
	}
	if (strNiu == _T("02"))
	{
		str += _T("牛二");
	}
	if (strNiu == _T("03"))
	{
		str += _T("牛三");
	}
	if (strNiu == _T("04"))
	{
		str += _T("牛四");
	}
	if (strNiu == _T("05"))
	{
		str += _T("牛五");
	}
	if (strNiu == _T("06"))
	{
		str += _T("牛六");
	}
	if (strNiu == _T("07"))
	{
		str += _T("牛七");
	}
	if (strNiu == _T("08"))
	{
		str += _T("牛八");
	}
	if (strNiu == _T("09"))
	{
		str += _T("牛九");
	}
	if (strNiu == _T("10"))
	{
		str += _T("牛牛");
	}

	return str;
}
bool	CTzhxxDlg::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}

//获得组三组六类型
int	CTzhxxDlg::GetZuxuanType(string strNumber)
{
	map<int, string>	mapStr;
	mapStr[atoi(strNumber.substr(0,1).c_str() )] =  strNumber.substr(0,1);
	mapStr[atoi(strNumber.substr(1,1).c_str() )] =  strNumber.substr(1,1);
	mapStr[atoi(strNumber.substr(2,1).c_str() )] =  strNumber.substr(2,1);
	//3个都不同，是组六
	if(mapStr.size() == 3)
		return 6;
	//2个不同，是组三
	if(mapStr.size() == 2)
		return 3;
	return 1;
}
VOID CTzhxxDlg::SendToServer(int nSendType)
{
	//变量定义
	ASSERT(GetMissionManager()!=NULL);
	CMissionManager * pMissionManager=GetMissionManager();

	//发起连接
	if (pMissionManager->AvtiveMissionItem(this,false)==false)
	{
		return;
	}


}