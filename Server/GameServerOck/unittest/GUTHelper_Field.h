#pragma once

class MockField;
class GFieldInfo;
class GSharedField;
class GDynamicFieldMaster;

class GUTHelper_Field
{
public:
	// ��Ȱ��Ǵ� ���ʵ带 ��ȯ�Ѵ�.  ����� �� Destroy() �Լ��� ȣ���ؾ��Ѵ�.
	static MockField*		DefaultMockField();

	// �ʵ����� ����
	static GFieldInfo*		NewFieldInfo(int nID=INVALID_ID);
	// �ʵ�׷� ���� ����
	static FIELD_GROUP_INFO*		NewFieldGroupInfo(GFieldInfo* pFieldInfo=NULL, int nGroupID=0);	
	// ��Ŀ ���� ����
	static MARKER_INFO*			InsertNewMarkerInfo(const GFieldInfo* pFieldInfo, uint32 nMarkerID=0);
	// ���� ���� ����
	static SENSOR_INFO*			InsertNewSensorInfo(const GFieldInfo* pFieldInfo, uint32 nSensorID=0);
	// ���� ���� ����
	static SPAWN_INFO*			InsertNewSpawnInfo(const GFieldInfo* pFieldInfo, SPAWN_ID nSpawnID=0, int nNPCID=INVALID_ID);

	// ���� �ʵ� ����
	static GSharedField*	NewSharedField(GFieldInfo* pFieldInfo=NULL, int nChannelID=1);
	// Ʈ���̾� �ʵ� ������ ����
	static GDynamicFieldMaster*	NewTrialFieldMaster(FIELD_GROUP_INFO* pFieldGroupInfo=NULL);
	// ���� �ʵ� ������ ����
	static GDynamicFieldMaster*	NewBattleArenaFieldMaster(FIELD_GROUP_INFO* pFieldGroupInfo=NULL);
	// ���ʵ� ����
	static MockField*				NewMockField(GFieldInfo* pFieldInfo=NULL);

	// ���ʵ�� �÷��̾� ����
	static void NewMockFieldAndPlayer(MockField*& pNewField, GEntityPlayer*& pNewEntityPlayer, vec3 vPlayerPos = vec3::ZERO, MUID uidNewPlayer=MUID(0,0));
};
