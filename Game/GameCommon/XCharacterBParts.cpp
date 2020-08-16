#include "stdafx.h"
#include "XCharacterBParts.h"
#include "RMesh.h"
#include "XCharacter.h"
#include "MStatisticsProfiler.h"
#include "MProfiler.h"

XCharacterBParts::XCharacterBParts( XCharacter* pCharacter )
: XCharacterParts(pCharacter)
{

}

RActorNode* XCharacterBParts::CreatePartsNode( RMesh* pMesh, const CSItemData* pItemData, tstring& strNodeName, tstring& strParentName, tstring& strActorNodeName, bool bNotifyToController /*= true */ )
{
	PFC_THISFUNC;
	PFI_BLOCK_THISFUNC(1207);

	// �̹� �ִ� ActorNode ����
	//DeleteActorNode(strActorNodeName.c_str(), false);

	RActorNode* retActorNode = NULL;

	// item mesh ����
	RMeshNode* pMeshNode = pMesh->GetNode(MLocale::ConvTCHARToAnsi(strNodeName.c_str()).c_str());

	RActorNode* pParentNode = NULL;
	if (!strParentName.empty())
	{
		pParentNode = m_pCharacter->GetActorNode(MLocale::ConvTCHARToAnsi(strParentName.c_str()).c_str());
	}

	// �޽� �߰�
	if(pMeshNode)
	{
		retActorNode = m_pCharacter->AddMeshNode(pMeshNode, pParentNode, MLocale::ConvTCHARToAnsi(strActorNodeName.c_str()).c_str(), m_bUsingBackgroundLoad, false);
	}

	if(bNotifyToController)
		m_pCharacter->ReNewContollers();

	return retActorNode;
}
