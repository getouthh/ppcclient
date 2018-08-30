#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "include/log.h"
//Ӧ�ó������
CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientApp::CGameClientApp()
{
//#ifdef GAME_LOG
	int logLv = GetLogLevel();
	//Log::open("GAME", logLv);
//#endif
	//AfxMessageBox("ok");
}

//��������
CGameClientApp::~CGameClientApp() 
{
}

int CGameClientApp::GetLogLevel()
{
	int logLevel = 0;

	char dir[MAX_PATH]={0};
	GetModuleFileName(NULL,dir,MAX_PATH);
	CString path = dir;
	path = path.Left(path.ReverseFind('\\'));
	path = path.Left(path.ReverseFind('\\'));
	path = path.Left(path.ReverseFind('\\')+1);
	path += _T("config\\sys.cfg");
	if(!PathFileExists(path.GetBuffer()))
		return logLevel;

	logLevel =GetPrivateProfileInt("LOG","level",0,path);
	return logLevel;
}
//////////////////////////////////////////////////////////////////////////
