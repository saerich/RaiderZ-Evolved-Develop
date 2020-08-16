#include "stdafx.h"
#include "RClothParameter.h"

bool RClothParameter::save( FILE* pFile )
{
	_ASSERT(NULL != pFile);

	int countToSave = 0;
	countToSave = fwrite(this,sizeof(RClothParameter),1,pFile);
	if( 1 != countToSave ) return false;
	return true;
}

bool RClothParameter::load( FILE* pFile )
{
	_ASSERT(NULL != pFile);

	int countToSave = 0;
	countToSave = fread(this,sizeof(RClothParameter),1,pFile);
	if( 1 != countToSave ) return false;
	return true;
}

RClothParameter::RClothParameter( void )
{
	//-[ simulation ]
	m_dampingCoefficientX		= 0.85f;	//���ӵ� = ��� * ����ӵ�
	m_dampingCoefficientY		= 0.85f;
	m_dampingCoefficientZ		= 0.90f;
	m_dampingJigglingVelocity	= 0.2f;	//���ӵ� = ��� * ����ӵ�
	m_timeToSimulate			= 0.032f;
	m_worldSimulation			= true;

	m_gravityX	=   0.0f;
	m_gravityY	=   0.0f;
	m_gravityZ	= -240.0f;

	memset(m_szResolvingLogic,0,sizeof(m_szResolvingLogic));
	strcpy_s(m_szResolvingLogic,"dsdsdscdsdsdsvc");

	//-[ constraint ]
	m_distanceRange					= 1.0f;		
	m_distanceStiffness				= 0.9f;	//�̵��ؾ��ϴ� �Ÿ� = ��� * �����̵��ؾ��ϴ� �Ÿ�

	m_stretchHorizontalRange		= 1.0f;
	m_stretchHorizontalStiffness	= 0.8f;

	m_stretchVerticalRange			= 1.0f;
	m_stretchVerticalStiffness		= 0.8f;

	//-[virtual translation ]
	m_virtualTranslationThresholdX	= 20.0f;	// ���������� ���� x�̵��Ÿ��� �̰ͺ��� ũ�� �۵�
	m_virtualTranslationThresholdY	= 20.0f;	
	m_virtualTranslationThresholdZ	= 3.0f;

	m_virtualTranslationSizeX = 0.0f;	//���� �̵���ų �Ÿ�
	m_virtualTranslationSizeY = 0.0f;
	m_virtualTranslationSizeZ = 1.3f;

	//-[ warp ]
	m_warpThreshold = 50.0f;

	//-[ option ]
	m_bDrawCollision = false;
}