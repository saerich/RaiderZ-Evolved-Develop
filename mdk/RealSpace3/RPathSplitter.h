////////////////////////////////////////////////////////////////////////////////////////////////////
// Resource Path Splitter
//

#pragma once

using namespace std;

namespace rs3 {

class RS_API RPathSplitter
{
	string					m_strSourcePath;
	vector<string>			m_strPathList;
	void AddPath(const string& strpath);

public:
	void					SetPath(const string& strPath);		///< ;���� ���е� path�� �����Ѵ�
	size_t					GetPathCount() const;				///< ���е� path ��
	const string&			GetPath() const;					///< ��ü path �� ��´�
	const string&			GetPath(int index) const;			///< ���е� path �� ��´�
	const vector<string>&	GetPathList() const;

};

//////////////////////////////////////////////////////////////////////////
// inlines
inline size_t RPathSplitter::GetPathCount() const { return m_strPathList.size(); }
inline const string& RPathSplitter::GetPath() const { return m_strSourcePath; }
inline const string& RPathSplitter::GetPath(int index) const { return m_strPathList[index]; }
inline const vector<string>& RPathSplitter::GetPathList() const { return m_strPathList; }

}