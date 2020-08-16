#include "stdafx.h"
#include "MDatabaseDesc.h"
#include <sstream>
#include "MLocale.h"

namespace mdb
{
	MDatabaseDesc::MDatabaseDesc(string driver, string server, string database_name, string user_name, string password) 		
	{
		strServer = MLocale::ConvAnsiToUCS2(server.c_str());
		strDatabaseName = MLocale::ConvAnsiToUCS2(database_name.c_str());
		strUserName = MLocale::ConvAnsiToUCS2(user_name.c_str());
		strPassword = MLocale::ConvAnsiToUCS2(password.c_str());
		strDriver = MLocale::ConvAnsiToUCS2(driver.c_str());
	}

	MDatabaseDesc::MDatabaseDesc(wstring driver, wstring server, wstring database_name, wstring user_name, wstring password) 
		: strDriver(driver), strServer(server), strDatabaseName(database_name), strUserName(user_name), strPassword(password)
	{
	}


	string MDatabaseDesc::BuildDSNString() const
	{
		return MLocale::ConvUTF16ToAnsi(BuildDSNStringW().c_str());
	}

	wstring MDatabaseDesc::BuildDSNStringW() const
	{
		// SQL2005 : "Driver={SQL Native Client};Server=myServerAddress;Database=myDataBase;Uid=myUsername;Pwd=myPassword;"
		// ���� : http://www.connectionstrings.com/

		// DB����, DBŬ���̾�Ʈ�� �ӽ��� �����ϼ����� �ٸ� ���, ODBC ����̹��� �ڵ����� ANSI �ڵ带 ��ȣ ��ȯ���ݴϴ�. [2010/7/16 praptor]
		// ANSI�� ��� �������� ���ӿ��� ���� �ٷ絵�� �ڵ���ȯ ����� ���ϴ�. [2010/7/16 praptor]

		std::wstringstream strStream;
		strStream	<< L"Driver={"	<< strDriver		<< L"};"
					<< L"Server="	<< strServer		<< L";"
					<< L"Database="	<< strDatabaseName	<< L";"
					<< L"Uid="		<< strUserName		<< L";"
					<< L"Pwd="		<< strPassword		<< L";"
					<< L"AutoTranslate=no;";						

		return strStream.str(); 
	}

	//string MDatabaseDesc::BuildDSNString()
	//{
	//	// SQL2005 : "Driver={SQL Native Client};Server=myServerAddress;Database=myDataBase;Uid=myUsername;Pwd=myPassword;"
	//	// ���� : http://www.connectionstrings.com/

	//	std::strstream strStream;
	//	strStream	<< "Driver={"	<< strDriver		<< "};"
	//				<< "Server="	<< strServer		<< ";"
	//				<< "Database="	<< strDatabaseName	<< ";"
	//				<< "Uid="		<< strUserName		<< ";"
	//				<< "Pwd="		<< strPassword		<< ";"
	//				<< L"AutoTranslate=no;";

	//	return strStream.str(); 
	//}
}