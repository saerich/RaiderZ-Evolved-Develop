#ifndef _GFIELD_MGR_H
#define _GFIELD_MGR_H

enum GAME_TIME_TYPE;
enum DYNAMIC_FIELD_TYPE;
struct FIELD_GROUP_INFO;
class GFieldInfo;
class GField;
class GSharedField;
class GSharedFieldMaster;
class GDynamicField;
class GDynamicFieldMaster;
class GTrialField;
class GInnRoomField;
class GInnRoomFieldGroup;
class GWeatherMgr;
class GEntityPlayer;
class GFieldFactory;
class GFieldEnteringMgr;

/// �ʵ� ������
class GFieldMgr : public MTestMemPool<GFieldMgr>
{
	friend class GFieldFactory;
public:	
	typedef std::unordered_map<uint64, GField*>	FIELD_BY_UID_MAP;
	typedef map<int, GSharedFieldMaster*>		SHARED_FIELD_MASTER_MAP;
	typedef map<MUID, GDynamicFieldMaster*>		DYNAMIC_FIELD_GROUP_MAP;

public:
	GFieldMgr();
	virtual ~GFieldMgr();

	// �ʵ�Ŵ��� ���� �ʱ�ȭ
	void				Clear();
	// �ʵ�Ŵ��� ������Ʈ
	void				Update(float fDelta);

	// �ʵ� ���� ��ȯ
	int					GetFieldQty()								{ return (int)m_mapFields.size(); }
	// �����ʵ帶���� ���� ��ȯ
	int					GetSharedFieldQty()							{ return (int)m_mapSharedFieldMasters.size(); }
	// �����ʵ帶���� ���� ��ȯ
	int					GetDynamicFieldGroupQty()					{ return (int)m_mapDynamicFieldMasters.size(); }

	// >> �Ϲ��ʵ� ����
	// ================================================================
	/// �ʵ� �߰�
	void				AddField(GField* pField);
	/// �ʵ带 ��Ͽ��� ����
	void				EraseField(GField* pField);
	/// �ʵ� ��ȯ
	GField*				GetField(MUID& uidField);
	/// �ʵ� ����
	void				DeleteField(const MUID& uidField);
	/// ������ ��� �ʵ� ��ȯ
	vector<GField*>		GetFieldList(int nFieldID);
	
	// �ʵ� �����̳� ��ȯ
	FIELD_BY_UID_MAP&	GetFieldMap()								{ return m_mapFields; }


	// >> �����ʵ� ���� 
	// ================================================================
	// �����ʵ帶���� �����̳� ��ȯ
	SHARED_FIELD_MASTER_MAP&	GetSharedFieldMap()					{ return m_mapSharedFieldMasters; }
	// �����ʵ帶���� ��ȯ
	GSharedFieldMaster*			GetSharedFieldMaster(int nFieldID);
	// �̹� �����Ǿ��ִ� ä���� ��ȯ
	GSharedField*				GetSharedField(int nFieldID, CHANNELID nChannelID=PRIME_CHANNELID);
	// �� ä���� ������ NULL��ȯ
	GSharedField*				GetEnterableSharedField(int nFieldID);
	// �ʵ���� �߰�
	void						AddSharedField(MUID uidField, int nFieldID, int nChannelID);

	// >> �����ʵ� ����
	// ================================================================
	GDynamicFieldMaster*	CreateDynamicFieldMaster(int nFieldGroupInfoID, DYNAMIC_FIELD_TYPE nType, const MUID& uidCreator);
	GDynamicFieldMaster*	CreateDynamicFieldMaster(MUID uidNew, int nFieldGroupInfoID, DYNAMIC_FIELD_TYPE nType, const MUID& uidCreator, vector<pair<int, MUID>>& vecDynamicFieldUID=vector<pair<int, MUID>>());

	DYNAMIC_FIELD_GROUP_MAP&GetDynamicFieldGroupMap() { return m_mapDynamicFieldMasters; }
	GDynamicFieldMaster*	GetDynamicFieldMaster(const MUID& uidDynamicFieldGroup);
	GDynamicFieldMaster*	GetDynamicFieldMaster(GEntityPlayer* pPlayer);
	GField*					GetDynamicField(const MUID& uidDynamicFieldGroup, int nFieldId);

	void					OnTimeChanged( GAME_TIME_TYPE nOldTime, GAME_TIME_TYPE nNewTime );
	void					OnTimeChangedDetail( int nTime );

	GFieldFactory*			GetFactory()			{ return m_pFieldFactory; }		///< �ʵ� ���丮. �ʵ� ������ �̰��� ���ؼ� �����ؾ� �Ѵ�.
	GFieldEnteringMgr*	GetFieldEnteringQueue()	{ return m_pFieldEnteringQueue; }

protected:
	virtual MUID			NewUID();

	// �����ʵ帶���� �߰�
	GSharedFieldMaster*		CreateSharedFieldMaster(GFieldInfo* pFieldInfo);
	// �����ʵ� �߰�, ��ȯ���� �Ҵ�� �����ʵ� (�����ʵ帶���Ͱ� ���ٸ� ���缭 ����)
	GSharedField*			CreateSharedField(MUID uidNewField, int nFieldID, CHANNELID nNewChannelID); 

	void					CreateTrialField( GDynamicFieldMaster* pNewDynamicFieldGroup, FIELD_GROUP_INFO* pFieldGroupInfo, const MUID uidDynamicFieldGroup, vector<pair<int, MUID>>& vecDynamicFieldUID );
	void					CreateInnRoom( GDynamicFieldMaster* pNewDynamicFieldGroup, FIELD_GROUP_INFO* pFieldGroupInfo, const MUID uidDynamicFieldGroup, vector<pair<int, MUID>>& vecDynamicFieldUID );
	void					CreateBattleArena( GDynamicFieldMaster* pNewDynamicFieldGroup, FIELD_GROUP_INFO* pFieldGroupInfo, const MUID uidDynamicFieldGroup, vector<pair<int, MUID>>& vecDynamicFieldUID );
	MUID					_PickDynamicFieldUID( const GFieldInfo* pFieldInfo, vector<pair<int, MUID>>& vecDynamicFieldUID );

	void					OnDeleteSharedField(GSharedField* pSharedField);
	void					OnDeleteDynamicFieldMaster(GDynamicFieldMaster* pDynamicFieldGroup);

	// ����ȭ�� ������Ʈ
	void					Update_SharedField(float fDelta);
	void					Update_DynamicField(float fDelta);
	void					Update_CommonField(float fDelta);

protected:
	GFieldEnteringMgr*		m_pFieldEnteringQueue;

private:
	FIELD_BY_UID_MAP		m_mapFields;
	SHARED_FIELD_MASTER_MAP	m_mapSharedFieldMasters;
	DYNAMIC_FIELD_GROUP_MAP	m_mapDynamicFieldMasters;
	GFieldFactory*			m_pFieldFactory;
};



#endif