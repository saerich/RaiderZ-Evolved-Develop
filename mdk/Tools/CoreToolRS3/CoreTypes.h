/// @file CoreTypes.h
#pragma once

#include <map>
#include <list>

// ���� ���� �� �������� ���̴� Ÿ�Ե� ����

namespace rs3 {
class RSceneNode;
};

class CEntityObject;
class CLogicObject;

/// ���̽� Ŭ����
class CCoreObject
{
public:
	virtual ~CCoreObject() {}
};

typedef std::map<rs3::RSceneNode*, void*>	SCENEOBJECTMAP;
typedef std::list<CEntityObject*>			ENTITY_OBJECTLIST;
typedef std::list<CLogicObject*>			LOGIC_OBJECTLIST;

// �� ���� Ÿ��
enum AXIS_STATE
{
	AXIS_NULL = 0,
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_XY,
	AXIS_YZ,
	AXIS_XZ
};

/** @brief Listenable View�� �����ϴ� �޽��� */
namespace VIEW_MESSAGE {
	enum ID{
		DEVICE_CREATED = 0,		///< ��ġ�� ������� �˸�
		ACTIVE_VIEW,			///< View�� Ȱ��ȭ��

		PRE_OPEN_DOCUMENT,		///< ������ ������ ����
		SCENE_LOADED,			///< Scene Load
		SCENE_IMPORTED,			///< Scene Imported(Zone�� ��쿡�� ����)
		LOGIC_LOADED,			///< Logic Load
		PRE_SAVE_DOCUMENT,		///< ������ ����Ǳ� �������� �˸�
		POST_SAVE_DOCUMENT,		///< ������ ����� �������� �˸�
		PRE_CLOSE_DOCUMENT,		///< ������ ������ ����
		POST_CLOSE_DOCUMENT,	///< ������ ���� ����


		BEFORE_SCENE_CLEAR,		///< Scene Clear �Ǳ� ����
		BEFORE_RES_DESTROY,		///< ���α׷��� ����Ǿ� Resource�� Destroy �Ǳ���

		LOGIC_NEW,				///< LogicObject �߰�
		LOGIC_DELETE,			///< LogicObject ����
		LOGIC_MOVE,				///< LogicObject �̵�
		LOGIC_CHANGED,			///< LogicObject �Ӽ��� �ٲ������
		LOGIC_SHOW,				///< LogicObject �� ��������
		LOGIC_HIDE,				///< LogicObject �� ��������

		LOGIC_SELECT,			///< LogicObject ����
		LOGIC_UNSELECT,			///< LogicObject ��������

		LOGIC_RENDER,			///< LogicObject ���� ������
		LOGIC_TEXTINFO_RENDER,	///< LogicObject �� ���� �ؽ�Ʈ�� ������

		LOGIC_LAYER_CHANGED,	///< LogicObject ���� Layer �� ����Ǿ�����

		SELECTION_CHANGED,		///< Selection �� �ٲ������
		SELECTION_CLEAR,		///< Selection �� ��� ��������

		UNDO_GENERAL,			///< General Undo Message
		UNDO_MOVE,				///< Move �� ���� ���
		UNDO_OBJ_NEW_DEL,		///< ������Ʈ �߰�/���� ���

		UNDO_ENTITY_EDIT,		///< ��ƼƼ ���� ���

		SCENE_BREAK_INSTANCE,	///< SceneNodeInstance �� Editable �� �ٲ����

		WB_STATE_CHANGED,		///< WB state �� �ٲ������ �˸���
		WB_MOUSE_L_DOWN,		///< ���콺�� Down�Ǿ����� �信 �˸���
		WB_MOUSE_L_UP			///< ���콺�� Up�Ǿ����� �信 �˸���
	};
};

/** @brief CWorkBench Edit State */
namespace WORKBENCH_STATE {
	enum STATE {
		TERRAIN_HEIGHT = 0,	///< ���� �������� ����
		TERRAIN_AVER,		///< ���� ��պ귯�� ����
		TERRAIN_FLAT,		///< ���� �����ϰ� ����
		TERRAIN_SPLAT,		///< ���� ���÷��� ����
		TERRAIN_RELAX,
		TERRAIN_GRASS,

		//�Ϲ� ������Ʈ�� Entity ������Ʈ
		//��ü���� ������ Map Mode�� ���� ����
		OBJECT_NEW,
		OBJECT_SELECT,
		OBJECT_MOVE,
		OBJECT_ROTATE,
		OBJECT_SCALE,

		//��Ÿ
		ETC_PATH_SELECT,
		ETC_CAMERA,
		ETC_MAP_VIEW,
		ETC_DEV,
		ETC_DECAL,

		STATE_NUM ///< ��ũ��ġ ������Ʈ�� �� ��
	};
};

/** @brief Map Edit Type */
enum MAP_EDIT_TYPE
{
	MAP_EDIT_TYPE_SCENE = 0,
	MAP_EDIT_TYPE_LOGIC,
	MAP_EDIT_TYPE_GUNZSERVERLOGIC,
	MAP_EDIT_TYPE_NPCTYPE,
	MAP_EDIT_TYPE_AIBEHAVIOR,
	MAP_EDIT_TYPE_ERROR,

	MAP_EDIT_TYPE_EXIT_SUCCESSED,
	MAP_EDIT_TYPE_EXIT_FAILD,
	MAP_EDIT_TYPE_UNKNOWN
};

/** @brief CoreTool View Type */
enum CORE_VIEW_TYPE
{
	CVT_GENERAL = 0,
	CVT_ROLLUP,			// �Ѿ�
	CVT_DIALOG,			// ������ ���̾�α�
	CVT_FIXED			// ������ ���̾�α� - ����/����ǥ����
};