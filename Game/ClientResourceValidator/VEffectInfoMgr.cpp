#include "stdafx.h"
#include "VEffectInfoMgr.h"

VEffectInfoMgr::VEffectInfoMgr()
: XEffectInfoMgr()
{

}

VEffectInfoMgr::~VEffectInfoMgr()
{

}

void VEffectInfoMgr::ParseEffectInfo( MXmlElement* pElement, MXml* pXml )
{
	ParseEffect(pElement, pXml);
}

void VEffectInfoMgr::LoadingFileInfo()
{
	m_vecEffectFileList.clear();

	vector<string> vecAddString;
	vecAddString.push_back("effect.scene.xml");
	//------------------------------------------------------------------------
	// ���� ����Ʈ ��������
	VFileName::GetFileList(V_PATH_EEFECT_TOOL, m_vecEffectFileList, &vecAddString, NULL, FA_FILE);
}

FILENAME_DATA* VEffectInfoMgr::GetEffectFileInfo( string& strName )
{
	// ������ �ҹ��ڷ� �ԷµǾ� �����Ƿ� �񱳵� �ҹ��ڷ� ������ �Ѵ�.
	string strEffectFile = strName;
	_strlwr( (char*)strEffectFile.c_str());	

	for(vector<FILENAME_DATA>::iterator it = m_vecEffectFileList.begin(); it != m_vecEffectFileList.end(); ++it)
	{
		if(it->strPureFileName == strEffectFile)
			return &(*it);
	}

	return NULL;
}