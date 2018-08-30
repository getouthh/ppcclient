#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
//#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include <vector>
using namespace std;
//#include "GameLogic.h"


//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				6									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagJettonInfo
{
	int								nXPos;								//����λ��
	int								nYPos;								//����λ��
	BYTE							cbJettonIndex;						//��������
};
struct BetInfo
{
public:
	BetInfo()
	{
		m_money = 0;
	}
	uint32 m_money; //Ѻע�����Ľ��
};
typedef vector<BetInfo> BetCFList;
//��������
typedef CArrayTemplate<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//������Ϣ
protected:
	static const	int					m_iOddsArray[JETTON_AREA_COUNT];					//����������

	__int64							m_iMeMaxScore;							//�����ע
	__int64							m_iMeAreaScore[JETTON_AREA_COUNT];		//�����ע					
	__int64							m_iAllAreaScore[JETTON_AREA_COUNT];		//ȫ����ע
	__int64							m_iAreaScoreLimit[JETTON_AREA_COUNT];	//������ǰ���·�
	CRect							m_rcJettonArea[JETTON_AREA_COUNT];		//����ע����
	CRect							m_rcScoreLimit[JETTON_AREA_COUNT];		//�����������·���д��
	CRect                           m_rcJettonAreaBK[JETTON_AREA_COUNT];
	CRect							m_rcApplayBankerNo[4];
	CRect							m_rcApplayBankerName[4];
	CRect							m_rcApplayBankerScore[4];
	CRect							m_rcHistoryRecord[MAX_SHOW_HISTORY];
	int								m_nCurCartoonIndex;						//��ǰӦ�û����ĸ���ͨͼ��
	int								m_CartoonTimeElapseArray[160];			//���ƿ�ͨ��ת��ʱ����
	int								m_CartoonElapsIndex;					//��ͨ��ʱ�������±�
	BOOL							m_bCanShowResult;						//�Ƿ���ʾ�����
	BYTE							m_cbGoalAnimal;							//ÿ����תֹͣ��λ��

	//λ����Ϣ
protected:
	int								m_nScoreHead;							//�ɼ�λ��


	//��������
protected:
	bool							m_bBombEffect[JETTON_AREA_COUNT];		//��ըЧ��
	BYTE							m_cbBombFrameIndex[JETTON_AREA_COUNT];	//֡������

	//��ʷ��Ϣ
protected:
	__int64							m_iMeStatisticScore;					//��Ϸ�ɼ�
	__int64							m_iMeStatisticScoreOld;					//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼

	//״̬����
public:
	uint32							m_wMeChairID;						//�ҵ�λ��
protected:
	BYTE							m_cbWinnerSide;						//ʤ�����
	BYTE							m_cbAreaFlash;						//ʤ�����
	__int64							m_iCurrentJetton;					//��ǰ����
	CString							m_strDispatchCardTips;				//������ʾ
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������

	//ׯ����Ϣ
protected:	
	uint32							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	__int64							m_iBankerScore;						//ׯ�һ���
	__int64							m_iBankerWinScore;					//ׯ�ҳɼ�	
	__int64							m_iBankerWinScoreOld;					//ׯ�ҳɼ�	
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ
	CFont							m_fontScoreLimit;					//ר�Ż��ƿ��·ֵĻ���

	//���ֳɼ�
public:
	__int64							m_iMeCurGameScore;					//�ҵĳɼ�
	__int64							m_iMeCurGameReturnScore;			//�ҵĳɼ�
	__int64							m_iBankerCurGameScore;				//ׯ�ҳɼ�
	__int64							m_iGameRevenue;						//��Ϸ˰��

	//���ݱ���
protected:
	CPoint							m_PointJetton[JETTON_AREA_COUNT];					//����λ��
	CPoint							m_PointJettonNumber[JETTON_AREA_COUNT];				//����λ��
	CJettonInfoArray				m_JettonInfoArray[JETTON_AREA_COUNT];				//��������
	CPoint							m_PtCardArray[32];					//��ת·���ϸ���������ĵ�

	//�ؼ�����
public:
	//CSkinButton						m_btJetton100;						//���밴ť
	//CSkinButton						m_btJetton1000;						//���밴ť
	//CSkinButton						m_btJetton10000;					//���밴ť
	//CSkinButton						m_btJetton100000;					//���밴ť
	//CSkinButton						m_btJetton1000000;					//���밴ť
	//CSkinButton						m_btJetton10000000;					//���밴ť
	vector<CSkinButton*>            m_btJettons;						//���밴ť����
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��

	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�
	CSkinButton						m_btBankerMoveUp;					//���Ϲ�������ׯ���б�
	CSkinButton						m_btBankerMoveDown;					//���¹�������ׯ���б�
	//CSkinButton						m_btBankButtonIn;						//���а�ť
	//CSkinButton						m_btBankButtonOut;						//���а�ť
	BetCFList						m_betCFlist;
	vector<HICON>					m_hBetIcon;
	//�ؼ�����
public:
	CApplyUser						m_ApplyUser;						//�����б�
	CGameRecord						m_GameRecord;						//��¼�б�	
	CGameClientDlg					*m_pGameClientDlg;					//����ָ��

	//�������
protected:
	CSkinImage						m_ImageViewFill;					//����λͼ
	CSkinImage						m_ImageViewBack;					//����λͼ
	std::map<uint32,CSkinImage*>     m_ImageJettonMap;                   //������ͼ
	CSkinImage						m_ImageJettonView;					//������ͼ
	CSkinImage						m_ImageScoreNumber;					//������ͼ
	CSkinImage						m_ImageMeScoreNumber;				//������ͼ
	CSkinImage						m_ImageTimeFlag;					//ʱ���ʶ
	CSkinImage						m_ImageHistoryRecord;				//��ʷ��¼ͼƬ
	CSkinPngImage						m_ImageBombEffect;					//����ͼƬ
	CSkinPngImage						m_ImageCardCartoon;					//��ת����

	//�߿���Դ
protected:
	CSkinImage						m_ImageWinFrame;					//��ע���б߿�

	CSkinImage						m_ImageMeBanker;					//�л�ׯ��
	CSkinImage						m_ImageChangeBanker;				//�л�ׯ��
	CSkinImage						m_ImageNoBanker;					//�л�ׯ��

	//������Դ
protected:
	CSkinImage						m_ImageGameEnd;						//�ɼ�ͼƬ

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//���ú���
public:
	//������Ϣ
	void SetMeMaxScore(__int64 iMeMaxScore);
	//�ҵ�λ��
	inline void SetMeChairID(uint32 wMeChairID) { m_wMeChairID=wMeChairID;}
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, __int64 iJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(uint32 wBankerUser, __int64 iBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, __int64 iWinScore) {m_wBankerTime=wBankerTime; m_iBankerWinScore=iWinScore;}
	//���ֳɼ�
	void SetCurGameScore(__int64 iMeCurGameScore, __int64 iMeCurGameReturnScore, __int64 iBankerCurGameScore, __int64 iGameRevenue);
	void SetGameEndState(bool bSucess){m_bSucess = bSucess;m_bCanShowResult = TRUE;};
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, __int64 iScoreCount);
	//��������
	void SetAreaLimitScore(__int64 * pLimitArray, int ArraySize);	
	//��������
	void SetAreaLimitScoreEx(__int64 iLimitArray, BYTE cbArea);	

	//���ó���
	void SetCurrentJetton(__int64 iCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(tagGameRecord * pGameRecord, int nRecordCount);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(__int64 iMeCurGameScore, __int64 iMeCurGameReturnScore, __int64 iBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}

	void CreateApplayBankerListRect(int nBaseX, int nBaseY);
	void CreateHistoryRecordRect(int nWidth, int nHeight);
	void UpdateCartoonTimeElapse(BYTE cbStopStep);	
	void CreateTurnPathAreaBasePoint(int nWidth, int nHeight);
	//��������
public:	
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//�滭��ը
	void DrawBombEffect(CDC *pDC);

	//����������ׯ����б�
	void DrawApplyBankerUserList(CDC * pDC);

	//������ʷ��¼
	void DrawHistoryRecord(CDC * pDC);

	//����ʱ����ʾ
	void DrawTimeTip(CDC * pDC, int nWidth, int nHeight);

	//����������ע
	void DrawAreaLimitScore(CDC * pDC);

	//���Ƴ���
	void DrawAreaJetton(CDC * pDC);

	//����ׯ����Ϣ
	void DrawBankerInfo(CDC * pDC, int nWidth, int nHeight);

	//�����м���Ϣ
	void DrawUserInfo(CDC * pDC, int nWidth, int nHeight);

	//�л�ׯ��
	void DrawChangeBanker(CDC * pDC, int nWidth, int nHeight);

	//��������
protected:

	//��ȡ��ǰ��һ�����ע������
	__int64 GetUserCurrentLeaveScore(bool bIsArea=false, BYTE cbArea=0x00);
	
	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//��ʼ��ת
	void PlayingCartoonEffect(bool playSound=true);
	
	//������ת
	void EndCartoonEffect(bool playSoud=true);


	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(BYTE cbWinnerSide);

	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	void SetMeCacheMoney(uint64 money){m_nMeCacheMoney = money;};

	// �������ôӷ������·��ĳ���
	void ReSetJetton(uint32 * pJettonArray, int ArraySize);

	//��Ϸ������ǿ�н���ת��
	void ForceStopScroll();
	// ͬ���������������
	void AnsyAreaJettonTotal(__int64 areaJettonArray[],int count,bool draw=false);
	//ͬ���Լ���������ע��������
	void AnsyMyselfAearJettonTotal(__int64 areaJettonArray[],int count);

	// ������ʷ��¼
	void CleanHistoryRecord();
	//��������
public:
	//��ǰ����
	inline __int64 GetCurrentJetton() { return m_iCurrentJetton; }	

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, __int64 iNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, __int64 iNumber, CRect rcRect, INT nFormat=-1);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);

	void DrawCardCartoon(CDC * pDC);
	uint64 m_nMeCacheMoney;
	bool  InitialBetCFInfo(); //����Ѻע��ť����Ϣ
	
	//���س�����ͼͼƬ
	void DestroyJettonViewImg();

	void SimulateJetton(BYTE cbViewIndex, __int64 iScoreCount,__int64 total);
	//��ť��Ϣ
protected:
	//���밴ť
	afx_msg void OnJettonButton100();
	//���밴ť
	afx_msg void OnJettonButton1000();
	//���밴ť
	afx_msg void OnJettonButton10000();
	//���밴ť
	afx_msg void OnJettonButton100000();
	//���밴ť
	afx_msg void OnJettonButton1000000();
	//���밴ť
	afx_msg void OnJettonButton10000000();
	afx_msg void OnJettonButtons(UINT nID);
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();

	afx_msg void OnBankerListMoveUp();
	afx_msg void OnBankerListMoveDown();

	afx_msg void OnClickedBankIn();
	afx_msg void OnClickedBankOut();

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	DECLARE_MESSAGE_MAP()
private:
	bool m_bSucess;
	
	//bool m_bAutoMode;
	CDC  m_jettonDC[JETTON_AREA_COUNT];
	CBitmap m_jettonBmp[JETTON_AREA_COUNT];
	CSkinImage                     m_jettonBK[JETTON_AREA_COUNT];
	HBITMAP                        m_jettonBMP[JETTON_AREA_COUNT];
	bool							m_bNeedFlash;
private:

	void DrawMemory();
	void DrawAllJetton(CDC* pDC);

	void DrawJettonBK();
	void DrawOneAreaJettonToMemory(int idx,CJettonInfoArray& ary,bool all);

	void DrawUserJettonNumber(CDC* pDC);

	void DeleteJettonButton();
};

//////////////////////////////////////////////////////////////////////////

#endif
