#include "stdafx.h"
#include "XCoverMesh.h"

#include "XCharacter.h"
#include "RMeshNode.h"
#include "MLocale.h"

void XCoverMeshList::AddCoverMeshList( tstring strUserProperty, bool bSub, int nIndex /*= 1 */ )
{
	// �ߺ� Ȯ��
	vector<XCoverMesh>::iterator itCover = m_vecCoverMeshList.begin();
	while(itCover != m_vecCoverMeshList.end())
	{
		if((*itCover).strCoverMeshSlot == strUserProperty &&
			itCover->nIndex == nIndex)
		{
			itCover->bSub = bSub;
			return;													// �̹� ������ �ִ�.
		}

		itCover++;
	}

	XCoverMesh CoverList;
	CoverList.strCoverMeshSlot	= strUserProperty;
	CoverList.bSub				= bSub;
	CoverList.nIndex			= nIndex;

	m_vecCoverMeshList.push_back(CoverList);
}

void XCoverMeshList::ShowCoverMesh( tstring strUserProperty )
{	
	// ����� �޽��� ��� �����ִ� ���̴�.
	// �ٷ� �����ֱ� ����� �����Ѵ�.
	vector<XCoverMesh>::iterator itCover = m_vecCoverMeshList.begin();
	while(itCover != m_vecCoverMeshList.end())
	{
		if((*itCover).strCoverMeshSlot == strUserProperty)
		{
			RActorNode *pActorNode = m_pCharacter->GetActorNode(MLocale::ConvTCHARToAnsi(strUserProperty.c_str()).c_str());

			if(pActorNode)
			{
				if((*itCover).bSub == false)
				{
					pActorNode->SetAllSubMaterialMask(true);
				}
				else
				{
					pActorNode->SetSubMaterialMask(itCover->nIndex, true);
				}
			}

			// �ٷ� ����Ʈ���� ������ �Ѵ�.
			m_vecCoverMeshList.erase(itCover);
			return;
		}

		itCover++;
	}
}

void XCoverMeshList::HideCoverMesh()
{
	// ����� �޽��� ����� ����� �ִ� ���̴�.
	vector<XCoverMesh>::iterator itCover = m_vecCoverMeshList.begin();
	while(itCover != m_vecCoverMeshList.end())
	{
		RActorNode *pActorNode = m_pCharacter->GetActorNode(MLocale::ConvTCHARToAnsi((*itCover).strCoverMeshSlot.c_str()).c_str());

		if(pActorNode)
		{
			if((*itCover).bSub == false)
			{
				pActorNode->SetAllSubMaterialMask(false);
			}
			else
			{
				pActorNode->SetSubMaterialMask(itCover->nIndex, false);
			}
		}

		itCover++;
	}
}

void XCoverMeshList::SetCoverMesh( RMeshNode* pMeshNode, bool bCover )
{
	const TCHAR* SET_COVER	= _T("set_cover");
	const TCHAR* SET_SUBCOVER= _T("set_subcover");

	if(!pMeshNode) return;

	// TODO : Actor wrap Ŭ������ �ʿ��ϴ�
	for(int i=0;i<pMeshNode->GetUserPropertyCount();i++)
	{
		const tstring& strProperty = MLocale::ConvAnsiToTCHAR(pMeshNode->GetUserPropertyString(i).c_str());
		if(_tcsncmp(strProperty.c_str(),SET_COVER,_tcslen(SET_COVER))==0)
		{
			int n = (int)strProperty.find_first_of(_T("="));
			if(n != tstring::npos)
			{
				tstring strTargetNode = strProperty.substr(n+1);
				int n = (int)strTargetNode.find_first_not_of(_T(" \t\v\n"));	// trim left
				if(n != tstring::npos) strTargetNode = strTargetNode.substr(n);

				if(bCover)
				{
					// ����� ���� �ϰ�ó���ϹǷ� ����Ʈ�� �߰��� �Ѵ�.
					AddCoverMeshList(strTargetNode, false);
				}
				else
				{
					ShowCoverMesh(strTargetNode);
				}
			}
		}
		else if(_tcsncmp(strProperty.c_str(), SET_SUBCOVER, _tcslen(SET_SUBCOVER))==0)
		{
			int n = (int)strProperty.find_first_of(_T("="));
			if(n != tstring::npos)
			{
				int nIndex = 1;
				tstring strTargetNode = strProperty.substr(n+1);
				int n = (int)strTargetNode.find_first_not_of(_T(" \t\v\n"));	// trim left
				if(n != tstring::npos) strTargetNode = strTargetNode.substr(n);
				n = (int)strTargetNode.find_first_of(_T(" "));
				if(n != tstring::npos) 
				{
					tstring strIndex = strTargetNode.substr(n);
					nIndex = _ttoi(strIndex.c_str()) - 1;					// -1�� ���־�� �Ѵ�.

					strTargetNode = strTargetNode.substr(0, n);
				}

				if(bCover)
				{
					// ����� ���� �ϰ�ó���ϹǷ� ����Ʈ�� �߰��� �Ѵ�.
					AddCoverMeshList(strTargetNode, true, nIndex);
				}
				else
				{
					ShowCoverMesh(strTargetNode);
				}
			}
		}
	}

	//------------------------------------------------------------------------
	// ����� �޽� �ϰ� ó��
	if(bCover)
	{
		HideCoverMesh();
	}
}