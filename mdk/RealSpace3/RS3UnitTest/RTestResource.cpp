#include "stdafx.h"
#include "RTypes.h"
#include "RResourceID.h"
#include "RResource.h"

using namespace rs3;

TEST( TestResource, GetResourceIDString )
{
	EXPECT_EQ( RRESOURCETYPE::RR_UNKNOWN, RRESOURCETYPE::RR_MAX );

	// GetResourceIDString ���� �´��� �˻�
	EXPECT_STREQ( RRESOURCETYPE::GetResoruceIDString(RRESOURCETYPE::RR_MAX), "_managed_max" );
	EXPECT_STREQ( RRESOURCETYPE::GetResoruceIDString(RRESOURCETYPE::RR_UNMANAGED_MAX), "_unmanaged_max" );
}

TEST( TestResource, ResourceLoader )
{
	class TestLoadingController : public RResource::LoadingController
	{
	public:
		int m_nCount;

		TestLoadingController() : m_nCount(0) {}

		virtual void	OnLoadingComplete(RResource*)	{  m_nCount++; }

	};

	TestLoadingController* testLoadingController = new TestLoadingController;

	class TestResource : public RResource
	{
	public:
		size_t GetLoadingControllerCount() { return m_loadingControllers.size(); }
	};

	TestResource* aResource = new TestResource;

	aResource->BuildAll( false, testLoadingController);

	// �Ϸ� ������ ������ Ȯ��
	EXPECT_EQ( 1, testLoadingController->m_nCount );
	EXPECT_EQ( 0, aResource->GetLoadingControllerCount() );

	// �ι� ���ϸ� ������ �ι� ������ Ȯ��
	aResource->AddLoadingController( testLoadingController ) ;
	aResource->AddLoadingController( testLoadingController ) ;
	aResource->BuildAll( false );
	EXPECT_EQ( 3, testLoadingController->m_nCount );

	// �ߺ��ؼ� controller�� ���������� Ȯ��
	aResource->AddLoadingController( testLoadingController ) ;
	aResource->AddLoadingController( testLoadingController ) ;
	EXPECT_EQ( 2, aResource->GetLoadingControllerCount() );

	// LoadingController �� �����Ǿ �̻������ Ȯ��
	delete testLoadingController;
	EXPECT_EQ( 0, aResource->GetLoadingControllerCount() );

	testLoadingController = new TestLoadingController;

	aResource->AddLoadingController( testLoadingController ) ;
	aResource->AddLoadingController( testLoadingController ) ;
	
	// ���� ���� ���ҽ� ���� 1������ Ȯ��
	EXPECT_EQ( 1, testLoadingController->GetResourceNotifyCount() );
	// ���� ���� ȸ���� 2�� ���� Ȯ��
	EXPECT_EQ( 2, testLoadingController->GetNotifyCount(aResource) );

	// Resource �� �����Ǿ �̻������ Ȯ��
	delete aResource;

	EXPECT_EQ( 0, testLoadingController->GetResourceNotifyCount() );
	delete testLoadingController;
}