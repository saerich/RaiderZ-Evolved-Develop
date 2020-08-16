
#include "StdAfx.h"
#include "ProjectType.h"
#include "MFileSystem.h"


void ProjectType::SetProjectType( const char* szPath)
{
	PROJECT_TYPE type = PT_NONE;

	// �Է��� �н��� ���� ������ Ư�� ������ �����ϴ����� ã�Ƽ� ������Ʈ Ÿ���� �Ǻ���.
	string strPathRuntime = szPath;
	while ( type == PT_NONE  &&  strPathRuntime.empty() == false)
	{
		strPathRuntime = MGetPathFileNameFromRelativePath( strPathRuntime, "../");

		for ( int i = 0;  i < PT_END;  i++)
		{
			if ( strlen( FIND_FILE_NAME[ i]) < 2)
				continue;

			string find = strPathRuntime + FIND_FILE_NAME[ i];

			WIN32_FIND_DATA findfile;
			HANDLE hFind = FindFirstFile( find.c_str(), &findfile);

			if ( hFind != INVALID_HANDLE_VALUE)
			{
				type = (PROJECT_TYPE)i;
				break;
			}
		}
	}

	if ( m_pProjectTypeInfo->m_ProjectType != type)
	{
		delete m_pProjectTypeInfo;


		// ������Ʈ ���� �߰��մϴ�.
		switch ( type)
		{
		case PT_RAIDERZ :	{ m_pProjectTypeInfo = new ProjectTypeInfoSoulHunt( strPathRuntime.c_str());	break;	}
		case PT_GUNZ :		{ m_pProjectTypeInfo = new ProjectTypeInfoGunz( strPathRuntime.c_str());		break;	}
		default :			{ m_pProjectTypeInfo = new ProjectTypeInfo();									break;	}
		}
	}

	m_pProjectTypeInfo->m_strPathRoot = szPath;
}







// Soul Hunt ����
ProjectTypeInfoSoulHunt::ProjectTypeInfoSoulHunt( const char* szPathRuntime)
{
	m_ProjectType = PT_RAIDERZ;
	m_strPrefix = "sh_";
	m_strPathRuntime = szPathRuntime;
	m_strPathSound = m_strPathRuntime + "Data/Sound/Effect";
	m_strFnameSndXml = m_strPathRuntime + "Data/Sound/sound.xml";
}


// GunZ ����
ProjectTypeInfoGunz::ProjectTypeInfoGunz( const char* szPathRuntime)
{
	m_ProjectType = PT_GUNZ;
	m_strPrefix = "gz_";
	m_strPathRuntime = szPathRuntime;
	m_strPathSound = m_strPathRuntime + "Data/Sound/Effect";
	m_strFnameSndXml = m_strPathRuntime + "Data/Sound/sound.xml";
}
