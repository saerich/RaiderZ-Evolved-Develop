#pragma once

#include "REffectSceneNode.h"

namespace rs3 {

RS_API extern const char *RSID_EFFECTINSTANCE;

/// Effect.scene.xml �� ���� �����Ǵ� ����Ʈ �ν��Ͻ�
class RS_API REffectInstanceSceneNode : public REffectSceneNode
{
public:
	MDeclareRTTI;
	DECLARE_NODE_ID_STRING(RSID_EFFECTINSTANCE);

	REffectInstanceSceneNode();
	~REffectInstanceSceneNode();

	virtual bool			Create(const char* szFileName, bool bBackground = false);

	void					ClearForReload();
	void					Reload();

	REffectSceneNode*		GetEffectSceneNode();
	const string&			GetFileName() const;

	bool					GetAutoStart() const { return m_bAutoStart; }
	void					SetAutoStart(bool bAutoStart) { m_bAutoStart = bAutoStart; }

	virtual RSceneNode*		Clone();

protected:
	virtual bool			CreateFromXML(MXmlElement &element, bool bBackgroundCreation);
	virtual void			SaveToXML(MXmlElement &element);

	// �̹� ������� ����Ʈ�� �ε��ؼ� ������, �̰�� Ŭ������ �и��ϴ°� ������ ?
	string					m_strEffectSourceName;
	bool					m_bAutoStart;		///< �ε��� �ڵ����� ����
	REffectSceneNode*		m_pEffectSceneNode;	///< ���� ����Ʈ ���
};

//////////////////////////////////////////////////////////////////////////
inline REffectSceneNode* REffectInstanceSceneNode::GetEffectSceneNode() { return m_pEffectSceneNode; }
//////////////////////////////////////////////////////////////////////////
inline const string& REffectInstanceSceneNode::GetFileName() const { return m_strEffectSourceName; }


} //namespace rs3
