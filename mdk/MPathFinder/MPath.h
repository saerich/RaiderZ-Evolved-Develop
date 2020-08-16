#pragma once

class iPath;
class MPathFinder;
class MCollisionInfo;
class MPathAgentShape;
class MContext;
class MAgent;

#include <vector>
#include "MVector3.h"

/**
	All paths created on a mesh get destroyed automatically when the mesh is destroyed. 
	�̱� ������ �Ժη�, �Ҹ��ڿ��� m_pPath�� ������ ���� ����.
*/
class MPath
{
	friend MPathFinder;
	friend MAgent;
private:

	//MPath operator =(const MPath &path);
public:
	iPath *m_pPath;
	MPath();
	virtual ~MPath();

	bool IsValid()
	{
		return ( m_pPath != NULL ? true : false );
	}
	unsigned long GetLength();
	unsigned long GetSize();

	void Release();		// ����� ����, �ݵ�� ��������� Release ȣ��
};
