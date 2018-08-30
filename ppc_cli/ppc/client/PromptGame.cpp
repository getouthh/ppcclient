// PromptGame.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PromptGame.h"
#include "afxdialogex.h"


// CPromptGame �Ի���

IMPLEMENT_DYNAMIC(CPromptGame, CDialogEx)

CPromptGame::CPromptGame(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPromptGame::IDD, pParent)
{
	m_ShowFont.CreateFont(13,0,0,0,300,NULL,NULL,NULL,GB2312_CHARSET,3,1,1,2,TEXT("������"));
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


// CPromptGame ��Ϣ�������


void CPromptGame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc,RGB(255,255,255));

	CFont* old = dc.SelectObject(&m_ShowFont);
	int lefpos = 4;
	dc.SetTextColor(RGB(96,82,69));
	dc.TextOut(lefpos,4,"�ر���ʾ");

	dc.SetTextColor(RGB(0,0,0));
	dc.TextOut(lefpos,20,"һ.���ݹ��ҹ涨,��Ϸֻ��ȡ����Ӯ�޹صĹ̶�ˮ��(̨��).");

	dc.TextOut(lefpos,40,"��.��Ϸ���÷ⶥ����,ÿ���û����ѵ����󣬲��ܼ�������Ϸ");

	dc.TextOut(lefpos,60,"��.�ٷ������κ���ʽ������Ϸ�ң����������Ϸ��������Ϊ��һ���������ദ��");

	dc.SetTextColor(RGB(255,0,0));
	dc.TextOut(lefpos,80,"�ٷ�����:");

	dc.TextOut(lefpos,100,"�ʶ���Ϸ����,������Ϸ����,������ɫ��Ϸ,���ܽ�������");
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
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPromptGame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CDialogEx::OnTimer(nIDEvent);
}



void CPromptGame::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
