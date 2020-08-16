#pragma once

#include "UnitTest++.h"
#include "MUnitTestConfig.h"

class MBaseUnitTest
{
private:

public:
	MBaseUnitTest() {}
	virtual ~MBaseUnitTest() {}
	virtual void Create(MUnitTestConfig& config) = 0;		///< �����׽�Ʈ�� �ʿ��� �ڿ� �ʱ�ȭ
	virtual void Destroy() = 0;		///< �����׽�Ʈ�� �ʿ��� �ڿ� ����
	virtual void OnTestStart() {}		///< ������ �׽�Ʈ �׸��� �����ϱ� ���� ȣ���
	virtual void OnTestFinish() {}		///< ������ �׽�Ʈ �׸��� ������ �� ȣ���

	virtual const char* GetName() = 0;				///< �����׽�Ʈ�� �̸�
	virtual UnitTest::TestList const& GetTestList() = 0;		///< �׽�Ʈ�� ����Ʈ
	virtual const char* GetOutputFileName() = 0;	///< ��� xml ���� �̸�
};


