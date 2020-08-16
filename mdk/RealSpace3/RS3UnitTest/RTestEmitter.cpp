#include "stdafx.h"
#include "RS3UnitTestUtility.h"
#include "RDevice.h" // TODO: ��Ŭ��� ���ص� �ǰ� ���� ����.
#include "REmitter.h"
#include "RUtil.h"

using namespace rs3;

class TestREmitter : public REmitter
{
public:
	using REmitter::m_sEmitterDesc;
	using REmitter::Update;
	using REmitter::GetVisibilityAppliedColor;
};

class TestEmitter : public NullDeviceFixture
{
public:
	virtual ~TestEmitter()
	{
		Clear();
	}

	/**
		emitterLife : �̹��� ������(ms)
		amount : �ʴ� ��ƼŬ ���� ��
	*/
	TestREmitter* CreateEmitter( float emitterLife, unsigned int amount )
	{
		TestREmitter* pEmitter = new TestREmitter;
		_ASSERT( pEmitter );
		pEmitter->CreateNew();

		REmitterDescription& desc = pEmitter->m_sEmitterDesc;
		desc.fEmitterLife = emitterLife;
		desc.fParticleLife = CMinMax<float>( 10.f, 10.f ); // ��ƼŬ�� ���� �� �� �ְ� �������� ������ �ش�.
		for( size_t i = 0; i < desc.vecEmitterVar.size(); ++i )
		{
			desc.vecEmitterVar[i].uAmount = amount;
		}
		pEmitter->Init();

		m_emitterList.push_back( pEmitter );
		return pEmitter;
	}

private:
	void Clear()
	{
		for( size_t i = 0; i < m_emitterList.size(); ++i )
		{
			delete m_emitterList[i];
		}
		m_emitterList.clear();
	}

	vector<TestREmitter*> m_emitterList;
};

const float EMITTER_LIFE = 100.f;
const unsigned int PARTICLE_AMOUNT = 100;

// �̹��� ������ Ÿ���� 0�϶��� ���� �׽�Ʈ
TEST_F( TestEmitter, EmitterStateAtLifeTimeZero )
{
	TestREmitter* pEmitter = CreateEmitter( 0.f, 0 );

	pEmitter->Start();
	EXPECT_EQ( REmitter::NOT_PLAY, pEmitter->GetCurrentPlayState() ) << "������ Ÿ���� 0�� �� Play�Ǹ� �ȵȴ�.";

	DWORD dwCreatedParticleNum = pEmitter->Update( 1000.f );
	EXPECT_EQ( 0, dwCreatedParticleNum ) << "������ Ÿ���� 0�� �� ��ƼŬ�� �����Ǹ� �ȵȴ�.";
}

TEST_F( TestEmitter, ParticleCountAtStart )
{
	TestREmitter* pEmitter = CreateEmitter( EMITTER_LIFE, PARTICLE_AMOUNT );

	pEmitter->Start();
	EXPECT_EQ( 0, pEmitter->GetCurrentCount() ) << "Start ���ڸ��� ��ƼŬ�� �����Ǿ�� �ȵȴ�.";
}

TEST_F( TestEmitter, ParticleCountAtUpdateZeroTime )
{
	TestREmitter* pEmitter = CreateEmitter( EMITTER_LIFE, PARTICLE_AMOUNT );

	pEmitter->Start();

	int prevParticleCount = pEmitter->GetCurrentCount();
	pEmitter->Update( 0.f );
	int currentParticleCount = pEmitter->GetCurrentCount() - prevParticleCount;
	EXPECT_EQ( 0, currentParticleCount ) << "Update �ð��� 0�̸� ��ƼŬ�� �����Ǿ�� �ȵȴ�.";
}

TEST_F( TestEmitter, ParticleCountAtUpdate )
{
	TestREmitter* pEmitter = CreateEmitter( EMITTER_LIFE, PARTICLE_AMOUNT );

	pEmitter->Start();

	int prevParticleCount = pEmitter->GetCurrentCount();
	pEmitter->Update( 1.f );
	int currentParticleCount = pEmitter->GetCurrentCount() - prevParticleCount;
	EXPECT_EQ( PARTICLE_AMOUNT, currentParticleCount ) << "1�ʿ� Amount��ŭ�� ��ƼŬ�� �����Ǿ�� �Ѵ�.";
}

TEST_F( TestEmitter, EmitterPositionUpdate )
{
	const RVector EMITTER_POS_START( 100.f, 200.f, 300.f );
	const RVector EMITTER_POS_END( 400.f, 500.f, 600.f );
	const float START_TIME = 3.f;
	const float CHECK_LIFE_TIME = 0.5f;
	const RVector EXPECT_POS( EMITTER_POS_START + ( EMITTER_POS_END - EMITTER_POS_START ) * CHECK_LIFE_TIME );

	TestREmitter* pEmitter = CreateEmitter( EMITTER_LIFE, 0 );
	REmitterDescription& desc = pEmitter->m_sEmitterDesc;
	ASSERT_GE( 2, (int)desc.vecEmitterVar.size() ) << "�̹��� Ű �Ķ���ʹ� �ּ� 2���� �־�� �Ѵ�.";
	desc.vecEmitterVar[0].vPos = EMITTER_POS_START;
	desc.vecEmitterVar[1].vPos = EMITTER_POS_END;
	pEmitter->Init();

	// Start�� ��ġ �׽�Ʈ
	pEmitter->SetPosition( RVector::ZERO );
	pEmitter->Start();
	EXPECT_EQ( EMITTER_POS_START, pEmitter->GetPosition() ) << "�̹��Ͱ� Start�Ǹ� vecEmitterVar�� ù��° Ű�� ��ġ���� �����Ǿ�� �Ѵ�.";

	// �߰� ������ Ÿ�� ��ġ �׽�Ʈ
	pEmitter->Update( EMITTER_LIFE * CHECK_LIFE_TIME );
	RVector vr = pEmitter->GetPosition();
	EXPECT_EQ( EXPECT_POS, pEmitter->GetPosition() ) << "�̹��� �߰� ������ ��ġ���� ��밪�� �ٸ��ϴ�.";

	// ������ ������ Ÿ�� ��ġ �׽�Ʈ
	pEmitter->Update( EMITTER_LIFE * ( 1.f - CHECK_LIFE_TIME ) );
	EXPECT_NE( EMITTER_POS_END, pEmitter->GetPosition() ) << "�̹��� Life�� ������ �� ��ġ ������Ʈ�� ���� �ʴ´�."; // �̰� �´� �����ϱ�?

	// ����۽� ��ġ �׽�Ʈ
	pEmitter->SetStartTime( START_TIME ); // StartTime�� 0�� �ƴϿ��� ����� �����ؾ� ��.
	pEmitter->Start();
	EXPECT_EQ( EMITTER_POS_START, pEmitter->GetPosition() ) << "�̹��Ͱ� Start�Ǹ� vecEmitterVar�� ù��° Ű�� ��ġ���� �����Ǿ�� �Ѵ�.";
}

TEST_F( TestEmitter, GetVisibilityAppliedColor )
{
	TestREmitter* pEmitter = CreateEmitter( EMITTER_LIFE, PARTICLE_AMOUNT );

	const float VISIBILITY = 0.5f;
	const RVector4 vTestColor( 1.f, 0.5f, 0.2f, 1.f );
	const RVector4 vExpectedColor = RVector4( vTestColor.r, vTestColor.g, vTestColor.b, vTestColor.a * VISIBILITY );
	const DWORD dwExpectedResultColor = RVector4ToDWORD( vExpectedColor );

	DWORD dwVisibilityAppliedColor = pEmitter->GetVisibilityAppliedColor( vTestColor, VISIBILITY );
	EXPECT_EQ( dwExpectedResultColor, dwVisibilityAppliedColor ) << "�̹����� VisibilityResult���� ���� ���İ��� ����� �ݿ����� �ʾҴ�.";
}

// ������ Ÿ���� 0�϶� IsAlive()üũ.
TEST_F( TestEmitter, IsAliveAtLifeTimeZero )
{
	TestREmitter* pEmitter = CreateEmitter( 0, 0 );
	bool bAlive = pEmitter->IsAlive();
	EXPECT_FALSE( bAlive ) << "�̹��� �������� 0�̸� �ı��ǿ��� �Ѵ�.";

	pEmitter->Start();
	bAlive = pEmitter->IsAlive();
	EXPECT_FALSE( bAlive ) << "�̹��� �������� 0�̸� �ı��ǿ��� �Ѵ�.";

	RSceneNodeUpdateVisitor v;
	pEmitter->Traverse( &v );

	bAlive = pEmitter->IsAlive();
	EXPECT_FALSE( bAlive ) << "�̹��� �������� 0�̸� �ı��ǿ��� �Ѵ�.";
}

TEST_F( TestEmitter, IsAlive )
{
	TestREmitter* pEmitter = CreateEmitter( EMITTER_LIFE, 0 );
	bool bAlive = pEmitter->IsAlive();
	EXPECT_TRUE( bAlive ) << "�̹��Ͱ� �ѹ��� ������Ʈ ���� ������ ����־�� �Ѵ�.";

	pEmitter->Start();
	bAlive = pEmitter->IsAlive();
	EXPECT_TRUE( bAlive ) << "�̹��Ͱ� �ѹ��� ������Ʈ ���� ������ ����־�� �Ѵ�.";

	pEmitter->Update( EMITTER_LIFE );
	bAlive = pEmitter->IsAlive();
	EXPECT_FALSE( bAlive ) << "�̹��� ������ ��ŭ ������Ʈ �ǰ� ��ƼŬ�� ������ �ı��Ǿ�� �Ѵ�";
}
