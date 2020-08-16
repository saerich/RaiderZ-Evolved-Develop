#pragma once

#include "RSceneNode.h"
#include "RSceneNodeID.h"

namespace rs3 {

class RS_API RSceneNodeInstance : public RSceneNode
{
public:
	MDeclareRTTI;

	DECLARE_NODE_ID_STRING(RSID_SCENEINSTANCE);

public:
	RSceneNodeInstance();
	virtual ~RSceneNodeInstance();

	bool Create(const char* szFileName, bool bBackgroundCreation);	///< ���� ������ �о���δ�

	virtual bool CreateFromXML( MXmlElement &element, bool bBackgroundCreation ) override;
	virtual void SaveToXML( MXmlElement &element ) override;

	const string& GetFileName() const;

protected:

	// Path ����
	string	m_strFileName;		// ���� Scene ������ FullPath // NodeName�� ���̸��� ������.(ex test.scene.xml)
};

//////////////////////////////////////////////////////////////////////////
inline const string& RSceneNodeInstance::GetFileName() const { return m_strFileName; }

}