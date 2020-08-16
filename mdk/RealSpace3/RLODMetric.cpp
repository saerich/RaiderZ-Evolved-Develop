#include "stdafx.h"
#include "RLODMetric.h"
#include "RLODMetricInfo.h"

namespace rs3
{

RLODMetric::RLODMetric()
: m_pMetricInfo(NULL)
{

}

RLODMetric::~RLODMetric()
{
	DeleteMetricInfo();
}

bool RLODMetric::CreateMetricInfo()
{
	if(!m_pMetricInfo)
		m_pMetricInfo = new RLODMetricInfo;

	return true;
}


bool RLODMetric::DeleteMetricInfo()
{
	SAFE_DELETE( m_pMetricInfo);
	return true;
}

bool RLODMetric::ClearMetricInfo()
{
	if( m_pMetricInfo)
		m_pMetricInfo->ClearMetricList();

	return true;
}

bool RLODMetric::CreateMetricInfoFromXML(const char* fname)
{
	char szDescName[256];

	strcpy(szDescName,fname);
	strcat(szDescName,".lod.xml");

	MXml aXml;
	if(!aXml.LoadFile(szDescName))
		return false;

	CreateMetricInfo();
	ClearMetricInfo();

	////////////////////////////////////////////////////

	MXmlElement*	pChild;
	MXmlHandle		docHandle = aXml.DocHandle();

	pChild = docHandle.FirstChildElement().Element();

	MXmlElement* pMetricElement = pChild->FirstChildElement("METRICLIST");
	if(pMetricElement)
	{
		MXmlElement* pMetricNode;
		char szTagName[256];
		pMetricNode = pMetricElement->FirstChildElement();
		for(pMetricNode; pMetricNode != NULL; pMetricNode = pMetricNode->NextSiblingElement())
		{
			strcpy(szTagName, pMetricNode->Value());
			if(stricmp(szTagName,"METRIC")==0)
			{
				m_pMetricInfo->AddMetricFromXML( pMetricNode);
			}
		}
	}

	MXmlElement* pHysteresisElement = pChild->FirstChildElement("HYSTERESIS");
	if(pHysteresisElement)
	{
		m_pMetricInfo->SetHysteresisFromXML( pHysteresisElement);
	}

	return true;
}



}