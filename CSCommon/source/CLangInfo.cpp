#include "stdafx.h"
#include "CLangInfo.h"
#include "MLocale.h"


// lang.xml ������Ʈ �� ��Ʈ����Ʈ
#define LANGUAGES_XML_TAG_MAIET		"maiet"
#define LANGUAGES_XML_TAG_LANGS		"LANGS"
#define LANGUAGES_XML_TAG_LANG		"LANG"

#define LANGUAGES_XML_ATTR_NAME		"name"
#define LANGUAGES_XML_ATTR_COUNTRY	"country"
#define LANGUAGES_XML_ATTR_LOCALE	"locale"
#define LANGUAGES_XML_ATTR_PATH		"path"


CLangInfo::CLangInfo()
{
	// do nothing
}

CLangInfo::~CLangInfo()
{
	// do nothing
}

bool CLangInfo::Load(wstring strFileName)
{
	// ���� ����
	MXml xml;

	if (!xml.LoadFile(MLocale::ConvUTF16ToAnsi(strFileName.c_str()).c_str()))
	{
		return false;
	}


	// ���̿�Ʈ ��Ʈ
	MXmlHandle doc = xml.DocHandle();	
	MXmlElement* pRoot = doc.FirstChild(LANGUAGES_XML_TAG_MAIET).Element();
	if (pRoot == NULL)	return false;


	// ��� �׷�
	MXmlElement* pLanguages = pRoot->FirstChildElement(LANGUAGES_XML_TAG_LANGS);
	if (pLanguages == NULL)	return false;


	// ���� ��� ����
	MXmlElement* pLang = pLanguages->FirstChildElement();

	while(pLang != NULL)
	{
		if (0 == _stricmp(pLang->Value(), LANGUAGES_XML_TAG_LANG))
		{
			CLangNode node;

			if (!_Attribute(node.strName, pLang, LANGUAGES_XML_ATTR_NAME, &xml))		continue;
			if (!_Attribute(node.strCountry, pLang, LANGUAGES_XML_ATTR_COUNTRY, &xml))	continue;
			if (!_Attribute(node.strLocale, pLang, LANGUAGES_XML_ATTR_LOCALE, &xml))	continue;
			if (!_Attribute(node.strPath, pLang, LANGUAGES_XML_ATTR_PATH, &xml))		continue;

			__super::push_back(node);
		}
		
		pLang = pLang->NextSiblingElement();
	}


	// �⺻ ������ ����
	wstring strDefaultLocale;
	_Attribute(strDefaultLocale, pLanguages, LANGUAGES_XML_ATTR_LOCALE, &xml);

	for each(const CLangNode& node in *this)
	{
		// �⺻ ������ ������ ���ų� ��ã����, ù��° �� �⺻���� ����
		if (m_strLocale.empty())
		{
			m_strLocale = node.strLocale;
		}

		// �⺻ �����ϰ� ��ġ�ϴ� ���� ����
		if (strDefaultLocale == node.strLocale)
		{
			m_strLocale = node.strLocale;
			break;
		}
	}

	return true;
}

wstring CLangInfo::GetLocale(void) const
{
	return m_strLocale;
}
