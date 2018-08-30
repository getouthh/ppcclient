#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "DirectSound.h"
#include <vector>
using namespace std;
//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ԫ����
	friend class CGameClientView;

	//������Ϣ
protected:
	__int64							m_iMeMaxScore;						//�����ע
	__int64							m_iApplyBankerCondition;			//ׯ����������

	//������ע
protected:
	__int64							m_iMeAreaScoreArray[JETTON_AREA_COUNT];			

	//ׯ����Ϣ
protected:
	__int64							m_iBankerScore;						//ׯ�һ���
	uint32							m_wCurrentBanker;					//��ǰׯ��
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ
	int								m_nCountgame;                      //����ϵͳ����������ڿ��Ƶ�һ�β�����ע

	//״̬����
protected:
	bool							m_bMeApplyBanker;					//���Ƿ���������ׯ

	DWORD							m_tid;
	HANDLE							m_hthread;
	//�ؼ�����
protected:
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//������Դ
protected:
	CDirectSound					m_DTSDBackground;					//��������
	CDirectSound					m_DTSDCheer[3];						//��������
	CDirectSound					m_DTStopJetton;						
	CDirectSound					m_DTSDispatch_Card;						

	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

public:
	void PlayDisptchCard();
	virtual void EnableSound(bool bEnable);
	bool CanBet();
	//����̳�
private:
	//��ʼ����
	virtual bool InitGameFrame();
	//���ÿ��
	virtual void ResetGameFrame();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//ʱ����Ϣ
	virtual bool OnTimerMessage(uint32 wChairID, UINT nElapse, UINT nTimerID);
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);

	virtual void __cdecl OnEventUserScore(tagUserData2 * pUserData, uint32 wChairID, bool bLookonUser);

	virtual void __cdecl OnEventUserEnter(tagUserData2 * pUserData, uint32 wChairID, bool bLookonUser);
	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubPlaceJetton(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	bool OnSubGameCarRun(const void *pBuffer,WORD wDataSize);
	//������ׯ
	bool OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize);
	//ȡ����ׯ
	bool OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize);
	//�л�ׯ��
	bool OnSubChangeBanker(const void * pBuffer, WORD wDataSize);
	//��Ϸ��¼
	bool OnSubGameRecord(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize);

	//��ֵ����
protected:
	//����ׯ��
	void SetBankerInfo(uint32 wBanker,__int64 iScore);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, __int64 iJettonCount);

	//���ܺ���
protected:
	//���¿���
	void UpdateButtonContron();	

	//��Ϣӳ��
protected:
	//��ע��Ϣ
	void OnPlaceJetton(BYTE iJettonArea, __int64 iJettonNum,__int64 totalScore);
	//������Ϣ
	LRESULT OnApplyBanker(WPARAM wParam, LPARAM lParam);

	void OpenBank(int iIndex);

	void SwitchBanker(uint32 newBanker,__int64 score);
	DECLARE_MESSAGE_MAP()
private:
	vector<CString> m_carinfovec;
	void InitialCarInfo();
};

//////////////////////////////////////////////////////////////////////////
