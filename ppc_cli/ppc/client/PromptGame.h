#pragma once
#include "afxwin.h"
#include "resource.h"
#include "SkinControl/SkinButton.h"
// CPromptGame 对话框

class CPromptGame : public CDialogEx
{
	DECLARE_DYNAMIC(CPromptGame)

public:
	CPromptGame(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPromptGame();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT		OnPromptGame(WPARAM wparam, LPARAM lparam);
	CSkinButton	 m_btOK;
	afx_msg void OnBnClickedOk();
private:
	CFont m_ShowFont;
};
