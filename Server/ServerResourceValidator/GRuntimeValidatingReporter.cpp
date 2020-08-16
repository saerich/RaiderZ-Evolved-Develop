#include "stdafx.h"
#include "GRuntimeValidatingReporter.h"
#include "GRuntimeValidatingLogger.h"
#include "CSStrings.h"
#include "MLocale.h"


#define PATH_RUNTIME_VALIDATOR			L"RuntimeValidate/"
#define FILENAME_RUNTIME_VALIDATOR		PATH_RUNTIME_VALIDATOR L"ServerRuntimeValidatorResult.xml"

#define REPORT_XML_TAG_RESULTS			"maiettest-results"
#define REPORT_XML_TAG_REPORT			"maietreport"

#define REPORT_XML_ATTR_TESTS			"tests"
#define REPORT_XML_ATTR_FAILEDTESTS		"failedtests"
#define REPORT_XML_ATTR_TIME			"time"
#define REPORT_XML_ATTR_TEXT			"text"


// ������ �B��Խ��ϴ� - praptor
inline string trim_left(const string& str) 
{ 
	int n = (int)(str.find_first_not_of(" \t\v\n")); 
	return n == wstring::npos ? str : str.substr(n, str.length()); 
} 

inline string trim_right(const string& str) 
{ 
	int n = (int)(str.find_last_not_of(" \t\v\n")); 
	return n == wstring::npos ? str : str.substr(0, n + 1); 
} 


GRuntimeValidatingReporter::GRuntimeValidatingReporter()
{
	// do nothing
}

GRuntimeValidatingReporter::~GRuntimeValidatingReporter()
{
	// do nothing
}

void GRuntimeValidatingReporter::Report(wstring strMessage)
{
	MXml Xml;

	// ���� ���� ����
	if (!Xml.LoadFile(MLocale::ConvUTF16ToAnsi(FILENAME_RUNTIME_VALIDATOR).c_str()))
	{
		MXmlDeclaration* pDec = Xml.CreateDeclaration("1.0", "UTF-8", "");
		Xml.Doc()->LinkEndChild(pDec);
	}


	// ��Ʈ ��� ã��
	MXmlHandle docHandle = Xml.DocHandle();
	MXmlElement* pRootElement = docHandle.FirstChild(REPORT_XML_TAG_RESULTS).Element();

	if (pRootElement == NULL)
	{
		// ������ ����
		pRootElement = new MXmlElement(REPORT_XML_TAG_RESULTS);
		Xml.Doc()->LinkEndChild(pRootElement);

		_SetAttribute(pRootElement, REPORT_XML_ATTR_TESTS, 0);
		_SetAttribute(pRootElement, REPORT_XML_ATTR_FAILEDTESTS, 0);
		_SetAttribute(pRootElement, REPORT_XML_ATTR_TIME, 0);
	}


	// ����Ʈ �߰�
	{
		// ���ڿ� ���� - trim + UTF8
		string strReportingMessage = MLocale::ConvUTF16ToAnsi(CSStrings::XmlEscape(strMessage).c_str());
		strReportingMessage = trim_left(strReportingMessage);
		strReportingMessage = trim_right(strReportingMessage);
		strReportingMessage = MLocale::ConvAnsiToUTF8(strReportingMessage.c_str(), LANG_KOREAN);


		// ������Ʈ �߰�
		MXmlElement* pMessageElement = new MXmlElement(REPORT_XML_TAG_REPORT);
		_SetAttribute(pMessageElement, REPORT_XML_ATTR_TEXT, strReportingMessage);

		pRootElement->LinkEndChild(pMessageElement);
	}


	// ���Ͽ� ����
	::CreateDirectory(PATH_RUNTIME_VALIDATOR, NULL);
	Xml.SaveFile(MLocale::ConvUTF16ToAnsi(FILENAME_RUNTIME_VALIDATOR).c_str());
}
