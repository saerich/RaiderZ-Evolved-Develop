#ifndef _LIGHTLIST_H
#define _LIGHTLIST_H

#include <string>
#include <list>

#include "RTypes.h"
#include "MXml.h"

using namespace std;

namespace rs3 {

// light �÷��׵�
#define RL_FLAG_CASTSHADOW		0x0010

class MCLIGHT
{
public:
	LightType lightType;
	string	ParentName;
	string	Name;
	RVector Color;
	RVector Position;
	RVector Direction;
	RVector Up;
	float	fIntensity;
	float	fAttnStart,fAttnEnd;
	DWORD	dwFlags;
	bool	bStatic;
	float	fFalloff;

	list<string>	m_userPropertyString;	// Light ���� Property �� �ʿ��ϴ�.,

	void CalcName2Type(INode* node);
	void ExportUserProperty(MXmlElement *pElement);
};

class LightList : public list<MCLIGHT*> {
public:
	virtual ~LightList();

	bool Save(MXmlElement *pElement, bool bStaticMesh);
};

}


#endif
