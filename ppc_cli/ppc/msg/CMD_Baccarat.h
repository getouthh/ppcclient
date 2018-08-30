#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						30								    //��Ϸ I D
#define GAME_PLAYER					500									//��Ϸ����
#define GAME_NAME					TEXT("�°ٱ�������")					//��Ϸ����
#define CLIENT_MODULE_NAME			TEXT("PPC_CLIENT.exe")				//�ͻ���ģ������
#define	SERVER_MODULE_NAME			TEXT("PPC_SERVER.DLL")				//�����ģ������

//״̬����
#define	GS_PLACE_JETTON				GS_PLAYING							//��ע״̬
#define	GS_GAME_END					GS_PLAYING+1						//����״̬

//�������
#define ID_BIG_TIGER			0									//��
#define ID_SMALL_TIGER			1									//С��
#define ID_BIG_DOG				2									//������
#define ID_SMALL_DOG			3									//С������
#define ID_BIG_HORSE			4									//��������
#define ID_SMALL_HORSE			5									//С������
#define ID_BIG_SNAKE			6									//��������
#define ID_SMALL_SNAKE			7									//С������
#define ID_CANCLE				9									//ȡ��


#define MAX_REPERTORY			100									//100�̹���
#define MAX_REPERTORY40			4									//Ĭ�Ϲ���
#define MIN_REPERTORY40			3									//Ĭ�Ϲ���
#define MAX_REPERTORY30			5									//Ĭ�Ϲ���
#define MIN_REPERTORY30			4									//Ĭ�Ϲ���
#define MAX_REPERTORY20			7									//Ĭ�Ϲ���
#define MIN_REPERTORY20			5									//Ĭ�Ϲ���
#define MAX_REPERTORY10			12									//Ĭ�Ϲ���
#define MIN_REPERTORY10			10									//Ĭ�Ϲ���

#define	PATH_STEP_NUMBER		32									//ת�������
#define JETTON_AREA_COUNT		8									//��ע��������
#define JETTON_COUNT            6                                   // �������ã����6��
//��¼��Ϣ
struct tagServerGameRecord
{
	__int64 	cbGameTimes;										//�ڼ���
	BYTE		cbRecord;											//��Χ��ID_BIG_TIGER��ID_SMALL_SNAKE
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				99									//��Ϸ����
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_CANCEL_BANKER			108									//ȡ������
#define SUB_S_PLACE_JETTONS_FAIL    109
#define SUB_S_CARRUN				110									//����
#define SUB_C_GUNMSG				4									//ǹ����Ϣ

//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	BYTE							cbJettonArea;						//��ע����
	__int64							iPlaceScore;						//��ǰ��ע
	__int64							iMaxPlaceArea;						//��ǰ���������ע
};

struct CMD_S_PlaceJettonsFail
{
	UINT32 m_round;
	BYTE m_size;
	CMD_S_PlaceJettonFail m_JettonFaile[8];
};
struct CMD_C_GUNMAN
{
	BYTE							cbType;								//����
	bool							cbGun;								//״̬����
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//���Ӻ���
	__int64							iScore;								//��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	__int64							iCurrentBankerScore;				//ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	unsigned int							wApplyUser;							//�������
};

//ȡ������
struct CMD_S_CancelBanker
{
	unsigned int							szCancelUser;					//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	__int64							iBankerScore;						//ׯ������
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	__int64							iUserMaxScore;						//�������

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	__int64							iBankerWinScore;					//ׯ�ҳɼ�
	__int64							iBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	__int64							iApplyBankerCondition;				//��������
	__int64							iAreaLimitScore[JETTON_AREA_COUNT];	//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	__int64								iTotalAreaScore[JETTON_AREA_COUNT];

	//�����ע
	__int64								iUserAreaScore[JETTON_AREA_COUNT];

	//��������ע
	__int64								iAreaScoreLimit[JETTON_AREA_COUNT];		

	//��һ���
	__int64							iUserMaxScore;					//�����ע							

	//������Ϣ
	__int64							iApplyBankerCondition;			//��������

	//�˿���Ϣ
 	BYTE							cbCardCount[2];					//�˿���Ŀ
	BYTE							cbTableCardArray[2][3];			//�����˿�

	//ׯ����Ϣ
	WORD							wBankerUser;					//��ǰׯ��
	WORD							cbBankerTime;					//ׯ�Ҿ���
	__int64							iBankerWinScore;				//ׯ��Ӯ��
	__int64							iBankerScore;					//ׯ�ҷ���
	bool							bEnableSysBanker;				//ϵͳ��ׯ

	//������Ϣ
	__int64							iEndBankerScore;				//ׯ�ҳɼ�
	__int64							iEndUserScore;					//��ҳɼ�
	__int64							iEndUserReturnScore;			//���ػ���
	__int64							iEndRevenue;					//��Ϸ˰��

	//ȫ����Ϣ
	BYTE							cbTimeLeave;					//ʣ��ʱ��
	BYTE							cbGameStatus;					//��Ϸ״̬
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameRecord;						//���ο����Ľ��
	__int64							iGameTimes;						//��ǰ����Ϸ���������ĵڼ���
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	unsigned int					wBankerUser;						//ׯ��λ��
	DWORD							bankerUIN;							//ׯ�ңɣĺ�
	__int64							iBankerScore;						//ׯ������
	__int64							iUserMaxScore;						//�ҵ�����
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	unsigned long long              bankerCondition;                    // ��ׯ����
	__int64							iAreaLimitScore[JETTON_AREA_COUNT];		//��������·�
	unsigned int                    iJettonSetting[JETTON_COUNT];          // ��������
};

//�û���ע
struct CMD_S_PlaceJetton
{
	unsigned int					wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	__int64							iJettonScore;						//��ע��Ŀ
	__int64							iMaxPlaceArea;						//��ǰ���������ע
	__int64 iAreaJetton[JETTON_AREA_COUNT]; // ����ע�������ͬ��
	CMD_S_PlaceJetton():wChairID(0),cbJettonArea(0),iJettonScore(0),iMaxPlaceArea(0)
	{
		memset(iAreaJetton,0,sizeof(iAreaJetton));
	}
};

struct CMD_S_CARRUN
{
	BYTE cbTimeLeave;
	BYTE cbGoalAnimal;						//����ֹͣ��λ��
	BYTE cbScence;                          // �Ƿ��ǳ��������ǳ������أ�����Ҫ���ö�ʱ������Ϣ��
	__int64 iAreaJetton[JETTON_AREA_COUNT];
	__int64 iMyselfAreaJetton[JETTON_AREA_COUNT]; // �������Լ���עͬ��
	CMD_S_CARRUN() : cbTimeLeave(0),cbGoalAnimal(0),cbScence(0)
	{
		memset(iAreaJetton,0,sizeof(iAreaJetton));
		memset(iMyselfAreaJetton,0,sizeof(iMyselfAreaJetton));
	}
};
//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	UINT32							m_rounds;
	//�˿���Ϣ
	BYTE							cbGoalAnimal;						//����ֹͣ��λ��
 
	//ׯ����Ϣ
	__int64							iBankerScore;						//ׯ�ҳɼ�
	__int64							iBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	__int64							iUserScore;							//��ҳɼ�
	__int64							iUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	__int64							iRevenue;							//��Ϸ˰��
	bool							m_bSucess;
	bool							m_bWin;
};


//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	__int64							iJettonScore;						//��ע��Ŀ
	__int64                         totalScore;                         //����ע��
};

//�������
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost,
	enOperateResult_Ping
};

//��¼��Ϣ
struct tagClientGameRecord
{
	enOperateResult					enOperateFlags;						//������ʶ
	BYTE							cbPlayerCount;						//�мҵ���
	BYTE							cbBankerCount;						//ׯ�ҵ���
	BYTE							cbKingWinner;						//����Ӯ��
	bool							bPlayerTwoPair;						//���ӱ�ʶ
	bool							bBankerTwoPair;						//���ӱ�ʶ
};
//������Ӯ
struct CMD_C_PlayAnimal
{
	BYTE							bPlayAnimal;						//��������
	bool							bAnimal;							//����״̬
};
//��ʷ��¼
#define MAX_SCORE_HISTORY			1000								//��ʷ����
#define MAX_SHOW_HISTORY			8									//һ�������ʾ����ʷ��¼����

//ͬʱ��ʾ������ׯ�������
#define MAX_SCORE_BANKER			4
//////////////////////////////////////////////////////////////////////////

#endif
