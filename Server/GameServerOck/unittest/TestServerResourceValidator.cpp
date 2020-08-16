#include "stdafx.h"
#include "SUnitTest.h"
#include "GServerResourceValidators.h"
#include "CSBaseValidator.h"
#include "GValidatorSilo.h"

SUITE(ServerResourceValidator)
{
	// �׻� ���� �븮������
	class SuccessValidator: public CSBaseValidator
	{
	public:
		virtual bool Check(void)	{ return true;	}
	};

	// �׻� ���� �븮������
	class FaultValidator: public CSBaseValidator
	{
	public:
		virtual bool Check(void)	{ return false;	}
	};


	// �׽�Ʈ ����
	struct Fixture
	{
		GServerResourceValidators m_validator;
		SuccessValidator m_successValidator;
		FaultValidator m_faultValidator;
	};

	TEST_FIXTURE(Fixture, ServerResourceValidatorDefaultAllCheck)
	{
		CHECK(m_validator.Check());
	}
	
	TEST_FIXTURE(Fixture, ServerResourceValidatorAllCheck)
	{
		m_validator.AddValidator(&m_successValidator);		
		CHECK(m_validator.Check());
		
		m_validator.AddValidator(&m_successValidator);		
		CHECK(m_validator.Check());
		
		m_validator.AddValidator(&m_faultValidator);
		CHECK(!m_validator.Check());
		
		m_validator.AddValidator(&m_successValidator);
		CHECK(!m_validator.Check());
	}

	TEST_FIXTURE(Fixture, ServerResourceValidatorSetSuccessValidator)
	{
		m_validator.AddValidator(&m_successValidator);
		CHECK(m_validator.Check());
	}

	TEST_FIXTURE(Fixture, ServerResourceValidatorSetFaultValidator)
	{
		m_validator.AddValidator(&m_faultValidator);
		CHECK(!m_validator.Check());
	}

	TEST(ValidatorSiloNonInitializeDependencyValidateAll)
	{
		// global �ʱ�ȭ - �⺻ �Ҵ� ������ ���
		GGlobalSystem backupSys;
		GGlobalManager backupMgr;
		backupSys = gsys;
		backupMgr = gmgr;
		gsys.SetNull();
		gmgr.SetNull();


		GValidatorSilo silo;
		CSBaseValidator* pValidator = silo.GetAllCheckValidator();
		CHECK(pValidator->Check());


		// global ����
		gsys = backupSys;
		gmgr = backupMgr;
	}
}
