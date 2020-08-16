/*
	������Ʈ�� ���� ������ �̸� �����ִ� Ŭ����.
	������Ʈ�� �°� �����ؼ� ����ϼ���.
*/


#pragma once



enum PROJECT_TYPE
{
	PT_NONE			= 0,

	PT_RAIDERZ,						// Raiderz
	PT_GUNZ,						// Gunz

	PT_END
};


const char FIND_FILE_NAME[ PT_END][ 256] = { "", "Raiderz.exe", "Gunz2.exe" };



class ProjectTypeInfo
{
public:
	PROJECT_TYPE	m_ProjectType;
	string			m_strPrefix;
	string			m_strPathRoot;				// ������ �� ����
	string			m_strPathRuntime;			// Runtime �о�
	string			m_strPathSound;				// Sound �о�
	string			m_strFnameSndXml;			// sound.xml �о�

	ProjectTypeInfo() : m_ProjectType( PT_NONE)		{ }
};




class ProjectType
{
protected:
	ProjectTypeInfo*	m_pProjectTypeInfo;


public:
	ProjectType()							{ m_pProjectTypeInfo = new ProjectTypeInfo;					}
	virtual ~ProjectType()					{ if ( m_pProjectTypeInfo)	delete m_pProjectTypeInfo;		}

	void			SetProjectType( const char* szPath);
	PROJECT_TYPE	GetProjectType()		{ return m_pProjectTypeInfo->m_ProjectType;					}
	const char*		GetPrefix()				{ return m_pProjectTypeInfo->m_strPrefix.c_str();			}
	const char*		GetPathRoot()			{ return m_pProjectTypeInfo->m_strPathRoot.c_str();			}
	const char*		GetPathRuntime()		{ return m_pProjectTypeInfo->m_strPathRuntime.c_str();		}
	const char*		GetPathSound()			{ return m_pProjectTypeInfo->m_strPathSound.c_str();		}
	const char*		GetFnameSoundXml()		{ return m_pProjectTypeInfo->m_strFnameSndXml.c_str();		}
};




class ProjectTypeInfoSoulHunt : public ProjectTypeInfo
{
public:
	ProjectTypeInfoSoulHunt( const char* szPathRuntime);
};


class ProjectTypeInfoGunz : public ProjectTypeInfo
{
public:
	ProjectTypeInfoGunz( const char* szPathRuntime);
};