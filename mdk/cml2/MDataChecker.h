//// Memory Hack�� ���, �߿��������� Checksum�� �����Ѵ�. ////
#ifndef _MDATACHECKER_H
#define _MDATACHECKER_H

//#pragma once


#include <map>
using namespace std;


#ifndef BYTE
typedef unsigned char BYTE;
#endif

class MDataChecker;
class MDataCheckNode {
protected:
	unsigned int	m_nID;	// ������ �ĺ���
	BYTE*			m_pData;
	unsigned int	m_nLen;
	unsigned int	m_nChecksum;
	unsigned int	m_nLastChecksum;

public:
	MDataCheckNode(BYTE* pData, unsigned int nLen);
	virtual ~MDataCheckNode();

	unsigned int GetID()			{ return m_nID; }
	unsigned int GetChecksum()		{ return m_nChecksum; }
	unsigned int GetLastChecksum()	{ return m_nLastChecksum; }
	bool UpdateChecksum();	// ������Ʈ�� ���� ������ true, �ٸ��� false
	void Validate()	{ m_nLastChecksum = m_nChecksum; }

friend MDataChecker;
};
class MDataCheckMap : public map<BYTE*, MDataCheckNode*>{};


class MDataChecker {
protected:
	unsigned int	m_nTotalChecksum;
	unsigned int	m_nLastTotalChecksum;

	MDataCheckMap	m_DataCheckMap;

public:
	MDataChecker();
	virtual ~MDataChecker();

	void Clear();
	unsigned int GetChecksum()	{ return m_nTotalChecksum; }
	MDataCheckNode* FindCheck(BYTE* pData);
	MDataCheckNode* AddCheck(BYTE* pData, unsigned int nLen);
	void RenewCheck(BYTE* pData, unsigned int nLen);
	bool UpdateChecksum();
	void BringError();
};

#endif