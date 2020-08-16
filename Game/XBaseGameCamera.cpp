#include "stdafx.h"
#include "XBaseGameCamera.h"
#include "XGame.h"
#include "XWorld.h"
#include "XCollisionTree.h"
#include <math.h>

XBaseGameCamera::XBaseGameCamera( void )
{
	m_vOldPos.Set(0, 0, 0);
	m_vOldDir.Set(0, 0, 0);
	m_fCameraSmoothMoveTime		= 0.0f;

	m_bSmoothMove				= false;
	m_bColTerrain				= false;
}

XBaseGameCamera::~XBaseGameCamera( void )
{

}

bool XBaseGameCamera::CheckTerrainCollision( vec3& eye, vec3& tar, vec3& outPos )
{
	if(gg.currentgamestate->GetWorld()->GetTerrain() &&
		gg.currentgamestate->GetWorld()->GetTerrain()->GetHeightFieldPtr() == NULL)
		return false;

	if ( gg.currentgamestate )
	{
		_ASSERT( gg.currentgamestate->GetWorld() != NULL );
		m_Collision.SetTargetCollisionTree(gg.currentgamestate->GetWorld()->GetCollisionTree());
	}
	
	return m_Collision.CameraMoveTest(tar, eye, outPos, NULL);
}

void XBaseGameCamera::UpdateCameraSmoothMove( vec3& eye, vec3& tar, vec3& dir, vec3& outPos, vec3& outDir, float fDelta, bool bColTerrain )
{
	// ���� �浹�� ���ؼ� ī�޶� �̵��� �ϸ��ϰ� �Ѵ�.
	if(m_vOldPos == vec3(0, 0, 0))
	{
		m_vOldPos = eye;
	}

	// ���� �浹�ΰ�?
	if(bColTerrain)
	{
		// ���� �浹�� �ٷ� �ɸ��Ÿ�... �������ϰ� ����.
		if(m_bColTerrain == false)
		{
			m_fCameraSmoothMoveTime = 0.0f;
			m_bSmoothMove			= true;
		}
		else
		{
			// ��� �浹 ���̶��... �Ÿ����ؼ� �������ϰ� ����.
			// �������� �Ÿ� ��
			float fPreDist			= (tar - m_vOldPos).Length();
			float fNewDist			= (tar - eye).Length();
			if(fabs(fNewDist - fPreDist) > 30.0f)
			{
				// �Ÿ����� 30 �̻��϶��� �Ѵ�.
				m_bSmoothMove		= true;
			}
		}

		m_bColTerrain = true;
	}
	else
	{
		// ������ ���� �浹�� �Ͼ����... �������ϰ� ����
		if(m_bColTerrain == true)
		{
			m_fCameraSmoothMoveTime = 0.0f;
			m_bSmoothMove = true;
		}

		m_bColTerrain = false;
	}

	//------------------------------------------------------------------------
	// ī�޶� ������ �̵� ���
	if(m_bSmoothMove == true)
	{
		// �ε巴�� �̵����̶��...
		m_fCameraSmoothMoveTime += fDelta;

		// ������ �̵��� 0.5�� ���Ŀ� ������.
		float fCurrentSmoothMoveTimeRatio = 1.0f - (m_fCameraSmoothMoveTime / 0.5f);
		if(fCurrentSmoothMoveTimeRatio < 0.0f)
		{
			// �Ϸ�
			fCurrentSmoothMoveTimeRatio = 0.0f;
			m_bSmoothMove				= false;
		}
		else
		{
			outPos		= m_vOldPos.GetInterpolated(eye, fCurrentSmoothMoveTimeRatio);
			outDir		= m_vOldDir.GetInterpolated(dir, fCurrentSmoothMoveTimeRatio);
		}
	}

	m_bSmoothMove = false;

	//------------------------------------------------------------------------
	// �˻� ����� ������ �̵��� �ƴ϶��...
	if(m_bSmoothMove == false)
	{
		outPos = eye;
		outDir = dir;
	}

	m_vOldPos = outPos;
	m_vOldDir = outDir;
}

float XBaseGameCamera::BezierCurve( float fStart, float fP1, float fP2, float fEnd, float fRatio )
{
	return ((1 - fRatio) * (1 - fRatio) * (1 - fRatio) * fStart) + 
		(3 * fRatio * (1 - fRatio) * (1 - fRatio) * fP1) + 
		(3 * fRatio * fRatio * (1 - fRatio) * fP2) + 
		(fRatio * fRatio * fRatio * fEnd);
}

float XBaseGameCamera::GetCameraRadiusNearZ( float fNearZ, float fFoveX, float fFoveY )
{
	return sqrt(pow(fNearZ, 2) + pow(tan(fFoveX / 2.0f) * fNearZ, 2) + pow(tan(fFoveY / 2.0f) * fNearZ, 2));
}

void XBaseGameCamera::SetCollisionRadius( float fNearZ, float fFoveX, float fFoveY )
{
	m_Collision.SetRadius(GetCameraRadiusNearZ(fNearZ, fFoveX, fFoveY) + 10.0f);
}