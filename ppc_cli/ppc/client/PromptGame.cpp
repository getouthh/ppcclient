// PromptGame.cpp : 实现文件
//

#include "stdafx.h"
#include "PromptGame.h"
#include "afxdialogex.h"


// CPromptGame 对话框

IMPLEMENT_DYNAMIC(CPromptGame, CDialogEx)

CPromptGame::CPromptGame(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPromptGame::IDD, pParent)
{
	m_ShowFont.CreateFont(13,0,0,0,300,NULL,NULL,NULL,GB2312_CHARSET,3,1,1,2,TEXT("新宋体"));
}

CPromptGame::~CPromptGame()
{
	m_ShowFont.DeleteObject();
}

void CPromptGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
}


BEGIN_MESSAGE_MAP(CPromptGame, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CPromptGame::OnBnClickedOk)
END_MESSAGE_MAP()


// CPromptGame 消息处理程序


void CPromptGame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc,RGB(255,255,255));

	CFont* old = dc.SelectObject(&m_ShowFont);
	int lefpos = 4;
	dc.SetTextColor(RGB(96,82,69));
	dc.TextOut(lefpos,4,"特别提示");

	dc.SetTextColor(RGB(0,0,0));
	dc.TextOut(lefpos,20,"一.根据国家规定,游戏只收取与输赢无关的固定水费(台费).");

	dc.TextOut(lefpos,40,"二.游戏设置封顶消费,每个用户消费到顶后，不能继续玩游戏");

	dc.TextOut(lefpos,60,"三.官方不以任何形式回收游戏币，严厉打击游戏币买卖行为，一经发现严肃处理");

	dc.SetTextColor(RGB(255,0,0));
	dc.TextOut(lefpos,80,"官方倡导:");

	dc.TextOut(lefpos,100,"适度游戏益脑,沉迷游戏伤身,倡导绿色游戏,享受健康生活");
	dc.SelectObject(old);
}


BOOL CPromptGame::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	char path[MAX_PATH] = {0};
	GetModuleFileName(NULL,path,MAX_PATH);
	CString filepath = path;
	filepath = filepath.Left(filepath.ReverseFind('\\')+1);
	m_btOK.SetButtonImage(filepath + "ppc\\aree_game.bmp",false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPromptGame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnTimer(nIDEvent);
}



void CPromptGame::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
