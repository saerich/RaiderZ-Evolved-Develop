#pragma once

#include "MPhysX.h"

//////////////////////////////////////////////////////////////////////////
//
// XPhysX
//
//////////////////////////////////////////////////////////////////////////

// XPhysX Collision Group
#define SH_PX_COL_RAGDOLL "ragdoll"

class XPhysX : public physx::MPhysX
{
public:
	XPhysX();
	virtual ~XPhysX();

	bool				Init(bool bUseRemoteDebugger=false);

	void				UpdateXPhysX(float _fElapsed);

	wstring				GetLastErrorMessage();
	int					GetLastError();

	//////////////////////////////////////////////////////////////////////////
	// ���׸���
	//
	NxMaterialIndex					GetMaterialIdx_BPart() const { return m_nMaterialIdx_BPart; }

	// �׷�
	int					GetBPartGroupID();

	//void				AddRagDollInformation(const std::wstring& strModelName);
	//MXmlElement*		GetRagDollInformation(const std::wstring& strModelName);

	//void				ReloadRagDollInformation();
	//void				ClearRagDollInformation();
private:
	//std::map< std::wstring, MXmlElement* > m_mapRagdollInfo;

	//////////////////////////////////////////////////////////////////////////
	// ���׸���
	//
	NxMaterialIndex m_nMaterialIdx_BPart;
};
