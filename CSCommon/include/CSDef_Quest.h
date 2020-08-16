#pragma once

#define QUEST_XML_TAG_MAIET					"maiet"
#define QUEST_XML_TAG_QUEST					"QUEST"
#define QUEST_XML_TAG_ITEM					"ITEM"
#define QUEST_XML_TAG_OBJECTIVES			"OBJECTIVES"
#define QUEST_XML_TAG_OBJECTIVE				"OBJECTIVE"

#define QUEST_XML_TAG_REWARDS				"REWARDS"
#define QUEST_XML_TAG_REWARD				"REWARD"
#define QUEST_XML_ATTR_DIALOG				"dialog"
#define QUEST_XML_ATTR_INCOMPLETE_DIALOG	"incomplete_dialog"
#define QUEST_XML_ATTR_SHARE_CONDITION		"share_condition"
#define QUEST_XML_ATTR_FIELD_GROUP			"field_group"

#define QUEST_XML_ATTR_ID					"id"
#define QUEST_XML_ATTR_TITLE				"title"
#define QUEST_XML_ATTR_DESC					"desc"
#define QUEST_XML_ATTR_CATEGORY				"category"
#define QUEST_XML_ATTR_TIMELIMIT			"timelimit"
#define QUEST_XML_ATTR_REPEATABLE			"repeatable"
#define QUEST_XML_ATTR_LEVEL				"level"
#define QUEST_XML_ATTR_SHARE				"share"
#define QUEST_XML_ATTR_PUBLIC_VAR			"public_var"
#define QUEST_XML_ATTR_PARTYFIELDID			"party_field"
#define QUEST_XML_ATTR_PARTYSENSORID		"party_sensor"
#define QUEST_XML_ATTR_VALIDATION			"validation"
#define QUEST_XML_ATTR_PLAYERS				"players"
#define QUEST_XML_ATTR_DESTROY_ITEM			"destroy_item"
#define QUEST_XML_ATTR_REWARDER				"rewarder"
#define QUEST_XML_ATTR_TRIALTYPE			"trialtype"
#define QUEST_XML_ATTR_CQ_CONDITION			"cq_condition"
#define QUEST_XML_ATTR_PARTY_MEMBER_COUNT	"party_member_count"

#define QUEST_XML_ATTR_TYPE					"type"
#define QUEST_XML_ATTR_PARAM1				"param1"
#define QUEST_XML_ATTR_PARAM2				"param2"
#define QUEST_XML_ATTR_FIELD				"field"
#define QUEST_XML_ATTR_MARKER				"marker"
#define QUEST_XML_ATTR_FIELD2				"field2"
#define QUEST_XML_ATTR_MARKER2				"marker2"
#define QUEST_XML_ATTR_PUBLIC_PROGRESS		"public_progress"

#define QUEST_XML_VALUE_TYPE_DESTROY		"DESTROY"
#define QUEST_XML_VALUE_TYPE_COLLECT		"COLLECT"
#define QUEST_XML_VALUE_TYPE_ACT			"ACT"
#define QUEST_XML_VALUE_TYPE_SCOUT			"SCOUT"


#define QUEST_TITLE_LEN						32		// �븮�����Ϳ��� ����Ϸ� �����մϴ�. - praptor [2009.02.24]
#define QUEST_DESC_LEN						512		// �븮�����Ϳ��� ����Ϸ� �����մϴ�. - praptor [2009.02.25]
#define QUEST_OBJECTIVE_DESC_LEN			128		// �븮�����Ϳ��� ����Ϸ� �����մϴ�. - praptor [2009.02.25]
#define QUEST_CATEGORY_LEN					32		// �븮�����Ϳ��� ����Ϸ� �����մϴ�. - praptor [2009.02.24]


#define QBEGIN_ITEM_COUNT					5		// ����Ʈ �����Ҷ� �޴� ������ �ִ� ��(����Ʈ ���࿡ �ʿ��� ������).
#define QREWARD_ITEM_COUNT					5		// ����Ʈ ���� ������ �ִ� ��.
#define MAX_QUEST_INSTACNE_COUNT			50		// ���� �� �ִ� �ִ� ����Ʈ ��

#define QOBJECTIVE_MIN_ID					1		// ����Ʈ ���� �ּ� ID
#define QOBJECTIVE_MAX_ID					5		// ����Ʈ ���� �ִ� ID

#define MAX_PLAYERQOBJECTIVE				5		// ����Ʈ ���� �ִ� ��
#define MAX_QOBJECTIVE_DESTROY_TARGET		30		// ���͸� ���̴� ����Ʈ ������ �ִ� Ÿ�� ��



/// ���� Ÿ��
/// - DB������ ���
enum QOBJECTIVE_TYPE
{
	QOT_NONE	= 0,
	QOT_DESTROY	= 1,	///< Ư�� ��ü�� �ı��ϰų� ���̴� ����
	QOT_COLLECT	= 2,	///< Ư�� ��ü�� �Լ��ϴ� ����	
	QOT_ACT		= 3,	///< ��ȹ�ڰ� ���Ƿ� var���� ���������� �Ϸ�Ǵ� ����
	QOT_SCOUT	= 4,	///< Ư�� ��ġ�� �����ϴ� ����
	QOT_INTERACT= 5,	///< quest_count Ÿ������ ���ͷ����� �ϴ� ����
	QOT_ESCORT	= 6,

	QOT_MAX
};

enum QUEST_REWARD_TYPE
{
	QRT_NONE = 0,
	QRT_MONEY,
	QRT_XP,
	QRT_TITLE,
	QRT_ITEM,
	QRT_FACTION,	
	QRT_MAX
};

enum QUEST_STATE_TYPE
{
	QST_NOEXIST = 0,
	QST_EXIST,
	QST_COMPLETE,
	QST_FAIL,

	QST_MAX
};

enum QUEST_SHARE_TYPE
{
	QSHAT_NONE = 0,
	QSHAT_MANUAL,
	QSHAT_AUTO,

	QSHAT_MAX
};

enum QUEST_TRIAL_TYPE
{
	QTT_PARTY = 0,
	QTT_SINGLE,

	QTT_MAX
};

enum QO_PUBLIC_PROGRESS
{
	QPP_NONE = 0,
	QPP_SELF,
	QPP_PARTY,
	QPP_SECTOR,

	QPP_MAX
};

enum QUEST_SHARE_RESULT
{
	QSR_NONE = 0,
	QSR_SUCCESS,
	QSR_FAIL_DISTANCE,		/// �Ÿ��� �ʹ� �־
	QSR_FAIL_CONDITION,		/// ����� üũ�� �����ؼ�
	QSR_FAIL_EXIST,			/// �̹� ������ �ִ� ����Ʈ��
	QSR_FAIL_DONE,			/// �̹� ������ ����Ʈ��

	QSR_MAX
};