#include "stdafx.h"
#include "RSceneNodeInstance.h"

namespace rs3 {

MImplementRTTI(RSceneNodeInstance, RSceneNode);

RSceneNodeInstance::RSceneNodeInstance() : RSceneNode()
{
	m_bCullingNode	= false;
}

RSceneNodeInstance::~RSceneNodeInstance()
{
}

bool RSceneNodeInstance::CreateFromXML(MXmlElement &element, bool bBackgroundCreation)
{
	char szBuffer[256] = {0,};

	MXmlElement *pElement = element.FirstChildElement("FILENAME");
	_Contents(szBuffer,pElement);

	return Create(szBuffer, bBackgroundCreation);
}

bool RSceneNodeInstance::Create(const char* szFileName, bool bBackgroundCreation)
{
	bool bRet = REngine::GetSceneManager().LoadSceneNodeInstanceFromRootXMLFile(this, szFileName, bBackgroundCreation);

	if( bRet == false ) 
		return false;

	m_strFileName = szFileName;

	// �����ϴ� �ڽ� ������ RSNA_DO_NOT_SAVE �Ӽ��� �ش�
	for ( RSceneNodeList::iterator itr = m_children.begin(); itr != m_children.end(); ++itr ) 
	{
		RSceneNode* pNode = *itr;
		pNode->AddAttribute( RSNA_DO_NOT_SAVE );
	}

	return bRet;
}

void RSceneNodeInstance::SaveToXML(MXmlElement &element)
{
	// ������ ���� : scene ���� �̸� ��
	MXmlElement *pElement = new MXmlElement("FILENAME");
	element.LinkEndChild(pElement);
	
	std::string strFileName = MGetRemovePathName( m_strFileName );
	_SetContents(pElement, strFileName );
}

}