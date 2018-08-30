#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "include/Markup.h"
#include "include/log.h"
//////////////////////////////////////////////////////////////////////////

//计时器标识
#define IDI_FREE					99									//空闲时间
#define IDI_PLACE_JETTON			100									//下注时间
#define IDI_DISPATCH_CARD			301									//发牌时间
#define  THREAD_MSG_BET 100
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

DWORD WINAPI ThreadFunc(LPVOID param)
{
	CGameClientDlg* pDlg = (CGameClientDlg*) param;
	MSG msg;
	while(GetMessage(&msg,0,0,0))
	{
		if (msg.message == THREAD_MSG_BET)
		{
			if (pDlg!=NULL)
			{
				CMD_C_PlaceJetton* pjet = (CMD_C_PlaceJetton*)msg.wParam;
				CMD_C_PlaceJetton PlaceJetton;
				ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

				//构造变量
				PlaceJetton.cbJettonArea=pjet->cbJettonArea;
				PlaceJetton.iJettonScore=pjet->iJettonScore;
				PlaceJetton.totalScore = pjet->totalScore;
				delete pjet;
				pDlg->SendData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
			}
		}
		::Sleep(1);
	}
	return 0;
}

//构造函数
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{

	m_hthread = CreateThread(NULL,0,ThreadFunc,this,0,&m_tid);
	//限制信息
	m_iMeMaxScore=0L;			
	m_iApplyBankerCondition=0L;	

	//个人下注
	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));

	//庄家信息
	m_iBankerScore=0L;
	m_wCurrentBanker=0L;
	m_nCountgame = 0;

	//状态变量
	m_bMeApplyBanker=false;

	return;
}

bool CGameClientDlg::CanBet()
{
	return m_nCountgame>1?true:false;
}
//析构函数
CGameClientDlg::~CGameClientDlg()
{
	PostThreadMessage(m_tid, WM_QUIT, 0, 0);
	WaitForSingleObject(m_hthread,500);
	CloseHandle(m_hthread);
}

//初始函数
bool CGameClientDlg::InitGameFrame()
{
	//设置标题
	SetWindowText(TEXT("百变碰碰车"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//加载声音
	VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	VERIFY(m_DTSDCheer[0].Create(TEXT("CHEER1")));
	VERIFY(m_DTSDCheer[1].Create(TEXT("CHEER2")));
	VERIFY(m_DTSDCheer[2].Create(TEXT("CHEER3")));
	VERIFY(m_DTStopJetton.Create("STOP_JETTON"));
	VERIFY(m_DTSDispatch_Card.Create( TEXT("DISPATCH_CARD") ));


	

	return true;
}

//重置框架
void CGameClientDlg::ResetGameFrame()
{
	//限制信息
	m_iMeMaxScore=0L;			

	//个人下注
	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));

	//庄家信息
	m_iBankerScore=0L;
	m_wCurrentBanker=0L;

	//状态变量
	m_bMeApplyBanker=false;

	return;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	return;
}

//时间消息
bool CGameClientDlg::OnTimerMessage(uint32 wChairID, UINT nElapse, UINT nTimerID)
{
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);

		//禁止按钮
		/*m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000000.EnableWindow(FALSE);*/
		for (int i =0 ;i<m_GameClientView.m_btJettons.size(); i++)
		{
			m_GameClientView.m_btJettons[i]->EnableWindow(FALSE);
		}


		//庄家按钮
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
	}

	if (IsEnableSound()) 
	{
		if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5) 
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
			if( nElapse <= 0 )
			{
				PlayGameSound(AfxGetInstanceHandle(), TEXT("STOP_JETTON"));
			}
		}
	}

	return true;
}

//旁观状态
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//网络消息
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	bool ret = true;
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//游戏空闲
		{
			ret =  OnSubGameFree(pBuffer,wDataSize);
		}
		break;
	case SUB_S_GAME_START:		//游戏开始
		{
			ret =  OnSubGameStart(pBuffer,wDataSize);
		}
		break;
	case SUB_S_PLACE_JETTON:	//用户加注
		{
			ret =  OnSubPlaceJetton(pBuffer,wDataSize);
		}
		break;
	case SUB_S_APPLY_BANKER:	//申请做庄
		{
			ret =  OnSubUserApplyBanker(pBuffer, wDataSize);
		}
		break;
	case SUB_S_CANCEL_BANKER:	//取消做庄
		{
			ret =  OnSubUserCancelBanker(pBuffer, wDataSize);
		}
		break;
	case SUB_S_CHANGE_BANKER:	//切换庄家
		{
			ret =  OnSubChangeBanker(pBuffer, wDataSize);
		}
		break;
	case SUB_S_GAME_END:		//游戏结束
		{
			ret = OnSubGameEnd(pBuffer,wDataSize);
		}
		break;
	case SUB_S_CARRUN:
		{
			ret =  OnSubGameCarRun(pBuffer,wDataSize);
		}
		break;
	case SUB_S_SEND_RECORD:		//游戏记录
		{
			ret =  OnSubGameRecord(pBuffer,wDataSize);
		}
		break;
	case SUB_S_PLACE_JETTON_FAIL:	//下注失败
		{
			ret =  OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
		break;
	case SUB_S_PLACE_JETTONS_FAIL:
		{
			CMD_S_PlaceJettonsFail* pData = (CMD_S_PlaceJettonsFail*)pBuffer;
			for (int i =0;i<pData->m_size;i++)
			{
				OnSubPlaceJettonFail(&pData->m_JettonFaile[i],sizeof(CMD_S_PlaceJettonFail));
			}
			CString tipstr;
			tipstr.Format("第%ld局下注失败!",pData->m_round);
			InsertSystemString(tipstr);
			ret = true;
		}
		break;
	}

	//错误断言
	//ASSERT(FALSE);
	LOG(4)("CGameClientDlg::OnGameMessage,ret=%d,cmd=%d\n",ret,wSubCmdID);
	//注意，这里必须返回true，否则程序退出
	return true;
}

//游戏场景
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	return true;
	switch (cbGameStation)
	{
	case GS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) 
				return false;


			const tagUserData2 *pMeUserData = GetUserData(GetMeChairID());
			if (pMeUserData)
			{
				m_GameClientView.SetMeCacheMoney(pMeUserData->m_money);
			}
			else
				m_GameClientView.SetMeCacheMoney(0);

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;
			if( pStatusFree == NULL )
				return FALSE;

			//设置时间
			SetGameTimer(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

            //玩家信息
			m_iMeMaxScore=pStatusFree->iUserMaxScore;

#ifdef GAME_LOG
			LOG(4)("CGameClientDlg::OnGameSceneMessage GS_FREE meMAXScor = %I64d\r\n",m_iMeMaxScore);
#endif
			m_GameClientView.SetMeMaxScore(m_iMeMaxScore);
			uint32 wMeChairID=GetMeChairID();
		
			uint32 wSwitchViewChairID=SwitchViewChairID(wMeChairID);
			m_GameClientView.SetMeChairID(wSwitchViewChairID);
			
            //庄家信息
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->iBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->iBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //控制信息
			m_iApplyBankerCondition=pStatusFree->iApplyBankerCondition;
			m_GameClientView.SetAreaLimitScore(pStatusFree->iAreaLimitScore, JETTON_AREA_COUNT);

			//设置状态
			SetGameStatus(GS_FREE);

			//更新控制
			UpdateButtonContron();
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_PLAYING:		//游戏状态
	case GS_GAME_END:		//结束状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) 
				return false;

			const tagUserData2 *pMeUserData = GetUserData(GetMeChairID());
			if (pMeUserData)
			{
				m_GameClientView.SetMeCacheMoney(pMeUserData->m_money);
			}
			else
				m_GameClientView.SetMeCacheMoney(0);

			//消息处理
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			if( pStatusPlay == NULL )
				return FALSE;

			//全局下注
			for( int i=0; i<JETTON_AREA_COUNT; i++)
				m_GameClientView.PlaceUserJetton(i, pStatusPlay->iTotalAreaScore[i]);

			//玩家下注
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				SetMePlaceJetton(i, pStatusPlay->iUserAreaScore[i]);

			//玩家积分
			m_iMeMaxScore=pStatusPlay->iUserMaxScore;		
#ifdef GAME_LOG
			LOG(4)("CGameClientDlg::OnGameSceneMessage GS_GAME_END meMAXScor = %I64d\r\n",m_iMeMaxScore);
#endif
			m_GameClientView.SetMeMaxScore(m_iMeMaxScore);
			uint32 wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));

			//控制信息
			m_iApplyBankerCondition=pStatusPlay->iApplyBankerCondition;
			m_GameClientView.SetAreaLimitScore(pStatusPlay->iAreaScoreLimit, JETTON_AREA_COUNT);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//设置成绩
				m_GameClientView.SetCurGameScore(pStatusPlay->iEndUserScore,pStatusPlay->iEndUserReturnScore,pStatusPlay->iEndBankerScore,pStatusPlay->iEndRevenue);
			}
			else
			{
				//播放声音
				if (IsEnableSound()) m_DTSDBackground.Play(0,true);
			}

			//庄家信息
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->iBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->iBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//设置状态
			SetGameStatus(pStatusPlay->cbGameStatus);

		
			//设置时间
			SetGameTimer(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			//更新按钮
			UpdateButtonContron();
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	}

	return false;
}

void __cdecl CGameClientDlg::OnEventUserEnter(tagUserData2 * pUserData, uint32 wChairID, bool bLookonUser)
{
//	LOG(4)("CGameClientDlg::OnEventUserEnter,wChairID=%d\n",wChairID);
	if(wChairID == GetMeChairID())
	{
		m_GameClientView.SetMeChairID(wChairID);
	}
}

void __cdecl CGameClientDlg::OnEventUserScore(tagUserData2 * pUserData, uint32 wChairID, bool bLookonUser)
{
	if(NULL == pUserData)
		return;
	if(pUserData->m_uin == GetUserData(GetMeChairID())->m_uin)
	{
		LOG(4)("CGameClientDlg::OnEventUserScore,money=%I64u\n",pUserData->m_money);
		m_GameClientView.SetMeCacheMoney(pUserData->m_money);
		//玩家积分
		m_iMeMaxScore = pUserData->m_money;
		m_GameClientView.SetMeMaxScore(m_iMeMaxScore);
	}
}
//游戏开始
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if( pGameStart == NULL )
		return FALSE;

	if(m_wCurrentBanker != pGameStart->wBankerUser)
	{
		SwitchBanker(pGameStart->wBankerUser,(__int64)pGameStart->iBankerScore);
	}
	//庄家信息
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->iBankerScore);

	//玩家信息
	m_iMeMaxScore=pGameStart->iUserMaxScore;

	m_iApplyBankerCondition = pGameStart->bankerCondition;
#ifdef GAME_LOG
	LOG(4)("CGameClientDlg::OnSubGameStart  meMAXScor = %I64d\r\n",m_iMeMaxScore);
#endif
	m_GameClientView.SetMeMaxScore(m_iMeMaxScore);

	//设置各下注区域初始可下分
	m_GameClientView.SetAreaLimitScore(pGameStart->iAreaLimitScore, JETTON_AREA_COUNT);
	m_GameClientView.ReSetJetton(pGameStart->iJettonSetting,JETTON_COUNT);
	//KillGameTimer(IDI_FREE);
	//KillGameTimer(IDI_DISPATCH_CARD);
	//设置时间
	SetGameTimer(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//设置状态
	SetGameStatus(GS_PLACE_JETTON);
	if (m_nCountgame < 2)
	{
		//if (m_nCountgame == 0)
			//InsertSystemString("进入游戏后下一局才能正常压注!!!");
		m_nCountgame ++;
	}
	//更新控制
	UpdateButtonContron();

	//更新界面
	m_GameClientView.UpdateGameView(NULL);

	//播放声音
	if (IsEnableSound()) 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
		m_DTSDBackground.Play(0,true);
	}

	return true;
}

void CGameClientDlg::PlayDisptchCard()
{
	//播放声音
	if (IsEnableSound()) 
	{
		m_DTSDispatch_Card.Play();
	}
}

//游戏空闲
bool CGameClientDlg::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) 
		return false;

	//消息处理
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	if( pGameFree == NULL )
		return false;

	m_GameClientView.ForceStopScroll();
	//KillGameTimer(IDI_DISPATCH_CARD);
	//KillGameTimer(IDI_PLACE_JETTON);
	//设置时间
	SetGameTimer(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	if(pGameFree->cbGameRecord != 0xFF)
	{
		tagGameRecord GameRecord;
		ZeroMemory(&GameRecord, sizeof(GameRecord));
		GameRecord.cbGameRecord = pGameFree->cbGameRecord;
		GameRecord.cbGameTimes = pGameFree->iGameTimes;

		m_GameClientView.m_GameRecord.FillGameRecord(&GameRecord, 1);
	}

	//设置状态
	SetGameStatus(GS_FREE);

	uint32 wMeChairID=GetMeChairID();

	uint32 wSwitchViewChairID=SwitchViewChairID(wMeChairID);
	m_GameClientView.SetMeChairID(wSwitchViewChairID);

	//清理桌面
	m_GameClientView.SetWinnerSide(0xFF);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=ID_BIG_TIGER; nAreaIndex<=ID_SMALL_SNAKE; ++nAreaIndex) 
		SetMePlaceJetton(nAreaIndex,0);

	//更新控件
	UpdateButtonContron();

//	m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->iBankerWinScore);
	const tagUserData2 *pMeUserData = GetUserData(GetMeChairID());
	if (pMeUserData)
	{
		TRACE("CGameClientDlg::OnSubGameFree,money=%I64u\n",pMeUserData->m_money);
		m_GameClientView.SetMeCacheMoney(pMeUserData->m_money);
	}
	else
	{
		TRACE("CGameClientDlg::OnSubGameFree,money=0,user is null\n");
		m_GameClientView.SetMeCacheMoney(0);
	}

	return true;
}

//用户加注
bool CGameClientDlg::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	//if (wDataSize!=sizeof(CMD_S_PlaceJetton)) 
	//	return false;

	//消息处理
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	if( pPlaceJetton == NULL )
	{
		LOG(4)("CGameClientDlg::OnSubPlaceJetton\n");
		return false;
	}
	if( pPlaceJetton->cbJettonArea>ID_SMALL_SNAKE || pPlaceJetton->cbJettonArea<ID_BIG_TIGER )
		return false;

	LOG(4)("CGameClientDlg::OnSubPlaceJetton,area=%d,iJettonScore=%I64d\n",pPlaceJetton->cbJettonArea,pPlaceJetton->iJettonScore);
	if(SwitchViewChairID(pPlaceJetton->wChairID) == m_GameClientView.m_wMeChairID)
	{
		//根据下注区域，设置本人下注筹码
		m_iMeAreaScoreArray[pPlaceJetton->cbJettonArea] += pPlaceJetton->iJettonScore;
		m_GameClientView.SetMePlaceJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->iJettonScore);
		//m_GameClientView.SetMePlaceJetton(pPlaceJetton->cbJettonArea, m_iMeAreaScoreArray[pPlaceJetton->cbJettonArea]); // topzoo ++
	}

	m_GameClientView.SetAreaLimitScoreEx(pPlaceJetton->iMaxPlaceArea, pPlaceJetton->cbJettonArea);

	//加注界面
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea, pPlaceJetton->iJettonScore);
	m_GameClientView.AnsyAreaJettonTotal(pPlaceJetton->iAreaJetton,JETTON_AREA_COUNT);
	
	//更新按钮
	UpdateButtonContron();


	//播放声音
	if (IsEnableSound()) 
	{
		if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
		{
			if (pPlaceJetton->iJettonScore>=10000000) 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
			else 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
			m_DTSDCheer[rand()%3].Play();
		}
	}
    
	return true;
}

bool CGameClientDlg::OnSubGameCarRun(const void *pBuffer,WORD wDataSize)
{
	//消息处理
	CMD_S_CARRUN * pCarRun=(CMD_S_CARRUN *)pBuffer;
	if( pCarRun == NULL )
		return FALSE;

	//取消动画
	for (WORD wAreaIndex=ID_BIG_TIGER; wAreaIndex<=ID_SMALL_SNAKE; ++wAreaIndex) 
		m_GameClientView.SetBombEffect(false,wAreaIndex);

	uint32 wMeChairID=GetMeChairID();

	uint32 wSwitchViewChairID=SwitchViewChairID(wMeChairID);
	m_GameClientView.SetMeChairID(wSwitchViewChairID);
	m_GameClientView.AnsyMyselfAearJettonTotal(pCarRun->iMyselfAreaJetton,JETTON_AREA_COUNT);
	if(pCarRun->cbScence == 0)
	{
		// 同步各区域筹码
		m_GameClientView.AnsyAreaJettonTotal(pCarRun->iAreaJetton,JETTON_AREA_COUNT,true);
		m_GameClientView.UpdateCartoonTimeElapse(pCarRun->cbGoalAnimal);	//构造卡通计时器间隔数组

		//设置发牌计时器
		SetGameTimer(GetMeChairID(),IDI_DISPATCH_CARD, pCarRun->cbTimeLeave >= 3 ? 3 : pCarRun->cbTimeLeave);
		
	}
	//m_GameClientView.SetWinnerSide(0xFF);
	
	//设置状态
	SetGameStatus(GS_GAME_END);

	//更新控件
	UpdateButtonContron();

	//停止声音
	for (int i=0; i<CountArray(m_DTSDCheer); ++i) 
		m_DTSDCheer[i].Stop();

	return true;
}
//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	//ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	if( pGameEnd == NULL )
		return FALSE;

	
	m_GameClientView.SetMeChairID(GetMeChairID());
	//庄家信息
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->iBankerTotallScore);
	m_GameClientView.SetGameEndState(pGameEnd->m_bSucess);
	//成绩信息
	m_GameClientView.SetCurGameScore(pGameEnd->iUserScore,pGameEnd->iUserReturnScore,pGameEnd->iBankerScore,pGameEnd->iRevenue);
#ifdef GAME_LOG
		LOG(4)("OnSubGameEnd:usersocre = %I64d,userreturnscore = %I64d,bankscore = %I64d,irevenue = %I64d\r\n",pGameEnd->iUserScore,pGameEnd->iUserReturnScore,pGameEnd->iBankerScore,pGameEnd->iRevenue);
#endif
	const tagUserData2 *pMeUserData = GetUserData(GetMeChairID());
	if (pMeUserData)
	{
		TRACE("CGameClientDlg::OnSubGameEnd,money=%I64u",pMeUserData->m_money);
		m_GameClientView.SetMeCacheMoney(pMeUserData->m_money);
	}
	else
	{
		TRACE("CGameClientDlg::OnSubGameEnd,money=0,user is null\n");
		m_GameClientView.SetMeCacheMoney(0);
	}

	if (pGameEnd->m_bWin)
	{
		CString tip;
		uint8 gogalcar = (pGameEnd->cbGoalAnimal) % 8;
		if (gogalcar >=0 && gogalcar < m_carinfovec.size())
		{
			tip = m_carinfovec[gogalcar];
		}
		else
			tip = "车";
	/*	if (gogalcar == 0)
		{
			tip = "大奔驰 X40";
		}
		else if (gogalcar == 2)
		{
			tip = "大宝马 X30";
		}
		else if (gogalcar == 4)
		{
			tip = "大奥迪 X20";
		}
		else if (gogalcar == 6)
		{
			tip = "大大众 X10";
		}
		else if (gogalcar == 1)
		{
			tip = "小奔驰 X5";
		}
		else if (gogalcar == 3)
		{
			tip = "小宝马 X5";
		}
		else if (gogalcar == 5)
		{
			tip = "小奥迪 X5";
		}
		else if (gogalcar == 7)
		{
			tip = "小大众 X5";
		}*/
		CString roundstr;
		roundstr.Format("%ld局",pGameEnd->m_rounds);
		tip =roundstr + "中得" + tip;
		InsertSystemString(tip);
	}
	//设置状态
	SetGameStatus(GS_GAME_END);

	//更新成绩
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; wUserIndex++)
	{
		tagUserData2 const *pUserData = GetUserData(wUserIndex);

		if ( pUserData == NULL ) 
			continue;

		tagApplyUser ApplyUser ;
		ZeroMemory(&ApplyUser, sizeof(ApplyUser));

		//更新信息
		ApplyUser.iUserScore = pUserData->m_money;
		lstrcpyn(ApplyUser.szUserName, pUserData->szNick.c_str(), lstrlen(pUserData->szNick.c_str()));
		ApplyUser.dwUserID = pUserData->m_uin;
		m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
	}

	//更新控件
	UpdateButtonContron();
	//停止声音
	//for (int i=0; i<CountArray(m_DTSDCheer); ++i) 
		//m_DTSDCheer[i].Stop();

	return true;
}

//更新控制
void CGameClientDlg::UpdateButtonContron()
{
	//置能判断
	bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
		bEnablePlaceJetton=false;

	if (GetGameStatus()!=GS_PLACE_JETTON) 
		bEnablePlaceJetton=false;

	if (m_wCurrentBanker==GetMeChairID()) 
		bEnablePlaceJetton=false;

	if (IsLookonMode())
		bEnablePlaceJetton=false;

	if (!CanBet())
		bEnablePlaceJetton = false;

	//if (m_bEnableSysBanker==true)
		//bEnablePlaceJetton=true;

	//下注按钮
	if (bEnablePlaceJetton==true)
	{
		//计算积分
		__int64 iCurrentJetton=m_GameClientView.GetCurrentJetton();
		__int64 iLeaveScore= 0;
		__int64	iTemp = 0;
		for( int i=0; i<JETTON_AREA_COUNT; i++ )
			iTemp += m_iMeAreaScoreArray[i];
		iLeaveScore = m_iMeMaxScore - iTemp;

		//设置光标
		if (iCurrentJetton>iLeaveScore)
		{
			__int64 maxjetton = 0;
			for (int i =0;i<m_GameClientView.m_betCFlist.size();i++)
			{
				if (iLeaveScore > m_GameClientView.m_betCFlist[i].m_money)
				{
					if (maxjetton < m_GameClientView.m_betCFlist[i].m_money)
						maxjetton = m_GameClientView.m_betCFlist[i].m_money;
				}
			}
			m_GameClientView.SetCurrentJetton(maxjetton);

		}

		//控制按钮
		for (int i =0;i<m_GameClientView.m_btJettons.size();i++)
		{
			if (i<m_GameClientView.m_betCFlist.size())
			{
				LOG(4)("CGameClientDlg::UpdateButtonContron,lv=%I64d,m_iMeMaxScore=%I64d,m_GameClientView.m_betCFlist[i].m_money=%d\n",iLeaveScore,m_iMeMaxScore,m_GameClientView.m_betCFlist[i].m_money);;
				m_GameClientView.m_btJettons[i]->EnableWindow((iLeaveScore>=m_GameClientView.m_betCFlist[i].m_money)?TRUE:FALSE);
			}
			else
				m_GameClientView.m_btJettons[i]->EnableWindow(FALSE);
		}
	}
	else
	{
		//设置光标
		m_GameClientView.SetCurrentJetton(0L);

		//禁止按钮
		for (int i =0;i<m_GameClientView.m_btJettons.size();i++)
		{
			m_GameClientView.m_btJettons[i]->EnableWindow(FALSE);
		}
	}

	//庄家按钮
	if (!IsLookonMode())
	{
		//获取信息
		const tagUserData2 *pMeUserData=GetUserData(GetMeChairID());



		//申请按钮
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) 
			bEnableApply=false;

		if (m_bMeApplyBanker) 
			bEnableApply=false;

		if (pMeUserData && pMeUserData->m_money<m_iApplyBankerCondition) 
			bEnableApply=false;

		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//取消按钮
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GS_FREE) 
			bEnableCancel=false;

		if (m_bMeApplyBanker==false) 
			bEnableCancel=false;

		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false);

		//显示判断
		if (m_bMeApplyBanker)
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);
		}
		else
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
	}

	//控制申请上庄玩家列表
	int nApplayCount = m_GameClientView.m_ApplyUser.GetApplayBankerUserCount();
	if( nApplayCount <= MAX_SCORE_BANKER )
	{
		m_GameClientView.m_btBankerMoveUp.EnableWindow(FALSE);
		m_GameClientView.m_btBankerMoveDown.EnableWindow(FALSE);
	}
	else
	{
		m_GameClientView.m_btBankerMoveUp.EnableWindow(TRUE);
		m_GameClientView.m_btBankerMoveDown.EnableWindow(TRUE);
	}

	//控制游戏记录按钮控件
	int nRecord = m_GameClientView.m_GameRecord.GetRecordCount();
	if( nRecord <= MAX_SHOW_HISTORY )
	{
		m_GameClientView.m_btScoreMoveL.EnableWindow(FALSE);
		m_GameClientView.m_btScoreMoveR.EnableWindow(FALSE);
	}
	else
	{
		int nReadPos = m_GameClientView.m_GameRecord.GetCurReadPos();
		int nWritePos = m_GameClientView.m_GameRecord.GetCurWritePos();
		m_GameClientView.m_btScoreMoveL.EnableWindow(TRUE);
		if( nReadPos != nWritePos-1 )
			m_GameClientView.m_btScoreMoveR.EnableWindow(TRUE);
	}
	return;
}

//加注消息
void CGameClientDlg::OnPlaceJetton(BYTE iJettonArea, __int64 iJettonNum,__int64 totalScore)
{
	//变量定义
	BYTE cbJettonArea=iJettonArea;			//下注区域
	__int64 iJettonScore=iJettonNum;		//筹码数量

	if( cbJettonArea > ID_SMALL_SNAKE || cbJettonArea < ID_BIG_TIGER )
		return;

	//庄家不能下注
	if ( GetMeChairID() == m_wCurrentBanker )
		return;

	//非下注状态，直接返回
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return;
	}

	//根据下注区域，设置本人下注筹码
	//m_iMeAreaScoreArray[cbJettonArea] += iJettonScore;
	//m_GameClientView.SetMePlaceJetton(cbJettonArea, m_iMeAreaScoreArray[cbJettonArea]); // topzoo --



	//变量定义
	CMD_C_PlaceJetton *PlaceJetton = new CMD_C_PlaceJetton();;
	ZeroMemory(PlaceJetton,sizeof(PlaceJetton));

	//构造变量
	PlaceJetton->cbJettonArea=cbJettonArea;
	PlaceJetton->iJettonScore=iJettonScore;
	PlaceJetton->totalScore = totalScore;
	//发送网络消息
	//SendData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
	::PostThreadMessage(m_tid,THREAD_MSG_BET,(WPARAM)PlaceJetton,0);

	//更新按钮
	UpdateButtonContron();

	//播放声音
	if (IsEnableSound()) 
	{
		if (iJettonScore>=10000000) 
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else 
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
		m_DTSDCheer[rand()%3].Play();
	}

	//SetMePlaceJetton(iJettonArea,iJettonNum);
	return;
}

void CGameClientDlg::OpenBank(int iIndex)
{
		IClientKernel *pIClientKernel = (IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);
	ASSERT(pIClientKernel!=NULL);
	if (pIClientKernel==NULL) return;

	//pIClientKernel->ShowBankDialog(iIndex,0);
	return;
}
//申请消息
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//合法判断
	tagUserData2 const *pMeUserData = GetUserData( GetMeChairID() );
	if( pMeUserData == NULL )
		return true;

	if (pMeUserData->m_money < m_iApplyBankerCondition) 
		return true;

	//旁观判断
	if (IsLookonMode()) 
		return true;

	//转换变量
	bool bApplyBanker = wParam ? true:false;

	//如果当前就是庄家，则不允许再次申请
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) 
		return true;

	if (bApplyBanker)
	{
		//发送申请上庄消息
		SendData(SUB_C_APPLY_BANKER, NULL, 0);
		//m_bMeApplyBanker=true;
	}
	else
	{
		//发送取消上庄消息
		SendData(SUB_C_CANCEL_BANKER, NULL, 0);
		//m_bMeApplyBanker=false;
	}

	//设置按钮
	UpdateButtonContron();

	return true;
}

//申请做庄
bool CGameClientDlg::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) 
		return false;

	//消息处理
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;
	if( pApplyBanker == NULL )
		return FALSE;

	//获取玩家
	tagUserData2 const *pUserData=GetUserData(pApplyBanker->wApplyUser);
	if( pUserData == NULL )
		return FALSE;

	//插入玩家
	if (m_wCurrentBanker != pApplyBanker->wApplyUser)
	{
		tagApplyUser ApplyUser;
		::ZeroMemory(&ApplyUser, sizeof(ApplyUser));

		lstrcpyn(ApplyUser.szUserName, pUserData->szNick.c_str(), sizeof(ApplyUser.szUserName));

		ApplyUser.iUserScore=pUserData->m_money;
		ApplyUser.dwUserID = pUserData->m_uin;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
	}

	//自己判断
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) 
		m_bMeApplyBanker=true;

	//更新控件
	UpdateButtonContron();

	return true;
}

//取消做庄
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//消息处理
	CMD_S_ApplyBanker * pCancelyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//获取玩家
	tagUserData2 const *pUserData=GetUserData(pCancelyBanker->wApplyUser);
	if (pUserData)
	{
		//删除玩家
		tagApplyUser ApplyUser;
		ApplyUser.dwUserID=pUserData->m_uin;
		ApplyUser.iUserScore=pUserData->m_money;
		m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
	}



	//自己判断
	if (IsLookonMode()==false && GetMeChairID()==pCancelyBanker->wApplyUser)
		m_bMeApplyBanker=false;

	//更新控件
	UpdateButtonContron();

	return true;
}

//切换庄家
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) 
		return false;

	//消息处理
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;
	if( pChangeBanker == NULL )
		return FALSE;

	//显示图片
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	//自己判断
	if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}

	//庄家信
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->iBankerScore);
	
	//删除玩家
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		tagUserData2 const *pBankerUserData=GetUserData(m_wCurrentBanker);
		if (pBankerUserData != NULL)
		{
			tagApplyUser ApplyUser;
			::ZeroMemory(&ApplyUser, sizeof(ApplyUser));
			lstrcpyn(ApplyUser.szUserName, pBankerUserData->szNick.c_str(), lstrlen(pBankerUserData->szNick.c_str()));
			ApplyUser.dwUserID = pBankerUserData->m_uin;
			m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
		}
	}

	//更新界面
	UpdateButtonContron();
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

void CGameClientDlg::SwitchBanker(uint32 newBanker,__int64 score)
{
	if(newBanker == 0)
	{
		m_bEnableSysBanker = true;
	}
	else
		m_bEnableSysBanker = false;


	m_GameClientView.EnableSysBanker(m_bEnableSysBanker);
	//显示图片
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker!= newBanker);
	
	
	//m_bMeApplyBanker = false;
	if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && newBanker!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && newBanker==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}
	//庄家信
	SetBankerInfo(newBanker,score);
	//删除玩家
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		tagUserData2 const *pBankerUserData=GetUserData(m_wCurrentBanker);
		if (pBankerUserData != NULL)
		{
			tagApplyUser ApplyUser;
			::ZeroMemory(&ApplyUser, sizeof(ApplyUser));
			lstrcpyn(ApplyUser.szUserName, pBankerUserData->szNick.c_str(), lstrlen(pBankerUserData->szNick.c_str()));
			ApplyUser.dwUserID = pBankerUserData->m_uin;
			m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
		}
	}

	//更新界面
	//UpdateButtonContron();
	m_GameClientView.UpdateGameView(NULL);
}
//游戏记录
bool CGameClientDlg::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize !=0 && wDataSize%sizeof(tagServerGameRecord)!=0) 
		return false;

	if( pBuffer == NULL )
		return true;

	//结果变量
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//设置记录
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	if(wRecordCount>0)
		m_GameClientView.CleanHistoryRecord();
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);
		if( pServerGameRecord == NULL )
			continue;

		GameRecord.cbGameTimes = pServerGameRecord->cbGameTimes;
		GameRecord.cbGameRecord = pServerGameRecord->cbRecord;
		m_GameClientView.SetGameHistory(&GameRecord, 1);
	}
	UpdateButtonContron();
	return true;
}

//下注失败
bool CGameClientDlg::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) 
		return false;

	//消息处理
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
	if( pPlaceJettonFail == NULL )
		return FALSE;

	for (int nAreaIndex=ID_BIG_TIGER; nAreaIndex<=ID_SMALL_SNAKE; ++nAreaIndex) 
		SetMePlaceJetton(nAreaIndex,0);

	//效验参数
	/*BYTE cbViewIndex=pPlaceJettonFail->cbJettonArea;	//取得下注区域
	ASSERT(cbViewIndex<=ID_SMALL_SNAKE);
	if (cbViewIndex>ID_SMALL_SNAKE) 
		return false;

	//下注数量
	__int64 iJettonCount=pPlaceJettonFail->iPlaceScore;
	__int64 iMaxJettonArea=pPlaceJettonFail->iMaxPlaceArea;

	m_GameClientView.SetAreaLimitScoreEx(iMaxJettonArea, pPlaceJettonFail->cbJettonArea);*/
	return true;
}

//设置庄家
void CGameClientDlg::SetBankerInfo(uint32 wBanker,__int64 iScore)
{
	m_wCurrentBanker=wBanker;
	m_iBankerScore=iScore;
	uint32 wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_iBankerScore);
}

//个人下注
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, __int64 iJettonCount)
{
	//效验参数
	ASSERT(cbViewIndex<= ID_SMALL_SNAKE);
	if (cbViewIndex>ID_SMALL_SNAKE) 
		return;

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( cbViewIndex == i )
			m_iMeAreaScoreArray[i] = iJettonCount;
	}

	//设置界面
	m_GameClientView.SetMePlaceJetton(cbViewIndex,iJettonCount);
}

void CGameClientDlg::InitialCarInfo()
{
	char path[MAX_PATH]={0};
	GetModuleFileName(NULL,path,MAX_PATH);
	CString infofile = path;
	infofile = infofile.Left(infofile.ReverseFind('\\')+1);
	infofile +="resource\\carinfo.xml";
	CMarkup xml;
	if (xml.Load(infofile))
	{
		if (xml.FindElem("CARINFO"))
		{
			while(xml.FindChildElem("INFO"))
			{
				m_carinfovec.push_back(xml.GetChildData());
			}
			
		}
	}


}

void CGameClientDlg::EnableSound( bool bEnable )
{
	__super::EnableSound(bEnable);
	if (!bEnable)
		m_DTSDBackground.Stop();
	else
		m_DTSDBackground.Play(0,true);
}

//////////////////////////////////////////////////////////////////////////
