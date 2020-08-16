#pragma once

#include "BaseObject.h"
#include "SceneNodeCommonProperty.h"
#include "REffectBase.h"

using namespace rs3;

class CEffectBasePropertyRef : public CRTTIClass<CEffectBasePropertyRef, CBaseObject, SPID_EFFECT_BASE>
{
public:
	CEffectBasePropertyRef() : m_pEffectBase(NULL) {}

	virtual void SetEffect( REffectBase* pEffectBase )
	{
		m_pEffectBase = pEffectBase;
	}

	char* GetTextureName()
	{
		return const_cast<char*>(m_pEffectBase->GetTextureName().c_str());
	}
	void SetTextureName( char* szName )
	{
		_ASSERT( szName );
		string str = MGetPureFileNameExt( string( szName ) );
		m_pEffectBase->ReplaceTexture( str.c_str() );
	}
	
	static void RegisterReflection()
	{
		RegisterProperty<char*>("�ؽ��� ����", &GetTextureName, &SetTextureName, 0, 0, "�ؽ��� ����", "����Ʈ ���� �Ӽ�", PE_FILENAME);
	}

private:
	REffectBase* m_pEffectBase;
};