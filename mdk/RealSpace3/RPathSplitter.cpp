#include "StdAfx.h"
#include "RPathSplitter.h"

namespace rs3 {

void RPathSplitter::AddPath(const string& strPath)
{
	if (strPath.length() <= 0) return;

	// �������� / (������) �߰�
	if(strPath[strPath.length()-1]!='/')
		m_strPathList.push_back(strPath+"/");
	else
		m_strPathList.push_back(strPath);
}

void RPathSplitter::SetPath(const string& strPath)
{
	m_strPathList.clear();

	string::size_type i = 0;

	string strTexturePath = strPath;

	// \ �� / �� ��ü�Ѵ�
	while((i=strTexturePath.find("\\",i)) != -1)
	{
		strTexturePath[i]='/';
	}

	// ; ���� ��� pathlist�� �ִ´�
	i = 0;
	string::size_type lastSemicolon = 0;
	string path;
	while((i=strTexturePath.find(";",i))!=-1)
	{
		path = strTexturePath.substr(lastSemicolon,i-lastSemicolon);
		AddPath(path);
		i++;
		lastSemicolon = i;
	}

	// �������� ;�� ��� �������� ������ �ִ´�.
	path = strTexturePath.substr(lastSemicolon);
	if(!path.empty())
		AddPath(path);

	// ���Ŀ� �µ��� ��ó���� ������ �н� ���
	m_strSourcePath.clear();
	for( size_t i=0; i<GetPathCount(); i++ )
	{
		m_strSourcePath+= GetPath(i);
		if( i!= GetPathCount()-1 )
			m_strSourcePath += ";";
	}
}

}