#pragma once

#include "RLib.h"
#include <string>
#include "MXml.h"
#include "MMath.h"

// mesh �� ����������(Ȥ�� �浹)ó���� ���� box,sphere,cylinder �� ����ȭ�� ����
// �Ŀ� RCollisionTree �� �����ؼ� ������ ���� ó���� ���ؼ� ���� ����

namespace rs3 {

class MCollision
{
public:
	enum MCollisionType {
		CYLINDER = 0,
		SPHERE,
		BOX,
		CAPSULE,

		TYPE_MAX
	};

	enum MCollisionMeshType {
		MESHTYPE_NORMAL = 0,	// �Ϲ� �浹�� �޽�
		MESHTYPE_CLOTH,			// �ʰ� �⵹�� �޽�
	};

	MCollision(MCollisionType type) : m_meshType(MESHTYPE_NORMAL), m_type(type), m_fHeight(0), m_fRadius(0), m_fLength(0) { }

	MCollisionMeshType	m_meshType;
	MCollisionType		m_type;

	MMatrix	m_matLocal;
	string	m_strName;				// Capsule Object Name
	string	m_strParentName;		// ������Ʈ�ÿ� �����ϴ� �����
	string	m_strRelatedNodeNames;	// �浹�ÿ� �����ִ� ����̸���

	float	m_fRadius;		// x
	float	m_fHeight;		// z
	float	m_fLength;		// y

	// for save
	bool Save(MXmlElement* pParentElement);

	// for load
	static MCollision* CreateFromElement(MXmlElement* pElement);
	bool Load(MXmlElement* pElement);
};

}
