#include "StdAfx.h"
#include "REffectInstance.h"
#include "REffectManager.h"

namespace rs3 {

MImplementRTTI(REffectInstanceSceneNode, REffectSceneNode);

REffectInstanceSceneNode::REffectInstanceSceneNode()
: REffectSceneNode()
, m_pEffectSceneNode(NULL)
, m_bAutoStart(false)
{
}

REffectInstanceSceneNode::~REffectInstanceSceneNode()
{
}

bool REffectInstanceSceneNode::Create(const char* szFileName, bool bBackground)
{
	_ASSERT( m_pEffectSceneNode == NULL );
	m_pEffectSceneNode = REngine::GetSceneManager().GetEffectManager()->CreateEffect( szFileName, bBackground );
	if(!m_pEffectSceneNode)
	{
		mlog("Failed to create effect %s.\n", szFileName );
		return false;
	}

	// �̷��� ������ ����Ʈ�� ����Ǵ°��� �ƴϴ�.
	m_strEffectSourceName = szFileName;
	m_pEffectSceneNode->AddAttribute(RSNA_DO_NOT_SAVE);

	AddChild( m_pEffectSceneNode );

	// scene.xml �� ���� ������ ���̹Ƿ� �� ��尡 �����Ѵ�
	AddToSceneNodeGroup( m_pEffectSceneNode );
	return true;
}

void REffectInstanceSceneNode::ClearForReload()
{
	// �ڽ� ���� ����
	if(m_pEffectSceneNode)
		m_pEffectSceneNode->Clear();

	SAFE_DELETE(m_pEffectSceneNode);
}

void REffectInstanceSceneNode::Reload()
{
	ClearForReload();
	Create( m_strEffectSourceName.c_str() );
}


bool REffectInstanceSceneNode::CreateFromXML(MXmlElement &element, bool bBackgroundCreation)
{
	char szBuffer[256];
	MXmlElement *pElement = element.FirstChildElement("FILENAME");
	bool bRet = _Contents(szBuffer,pElement);
	if( bRet == false )
	{
		mlog("Failed to create effect. File name is empty.\n");
		return false;
	}

	bRet = Create(szBuffer, bBackgroundCreation);
	if( bRet )
	{
		// �ڵ����� ������Ƽ
		MXmlElement *pStartElement = element.FirstChildElement("AUTOSTART");
		if(pStartElement)
		{
			m_bAutoStart = true;
			Start();
		}
	}

	return bRet;
}

void REffectInstanceSceneNode::SaveToXML(MXmlElement &element)
{
	if( !m_strEffectSourceName.empty() )
	{
		MXmlElement *pElement = new MXmlElement("FILENAME");
		element.LinkEndChild(pElement);
		_SetContents(pElement, m_strEffectSourceName );

		if(m_bAutoStart)
		{
			MXmlElement *pStartElement = new MXmlElement("AUTOSTART");
			element.LinkEndChild(pStartElement);
		}
	}
}

RSceneNode* REffectInstanceSceneNode::Clone()
{
	REffectInstanceSceneNode* pNewNode = new REffectInstanceSceneNode;
	pNewNode->Create( m_strEffectSourceName.c_str(), false );

	pNewNode->SetUsable(true);
	pNewNode->SetLastFramePause(m_bLastFramePause);
	pNewNode->SetTransform( GetLocalTransform() );
	pNewNode->SetScale( GetScale() );

	return pNewNode;
}

}