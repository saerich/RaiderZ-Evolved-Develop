#include "StdAfx.h"
#include "M_ToolTerrain.h"
#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperAction.h"

#include "M_CommandBuffer.h"
#include "M_ToolCmdMgr.h"

#include "C_ControlDoAction.h"
#include "C_XMLConfig.h"
#include "C_ControlCursor.h"

#include "VR_TerrainHeight.h"
#include "VR_TerrainAverage.h"
#include "VR_TerrainFlat.h"
#include "VR_TerrainSplat.h"
#include "VR_TerrainRelax.h"
#include "VR_TerrainGrass.h"
#include "VR_TerrainUserMaterial.h"

#include "RGrassSystem.h"
#include "M_ToolTerrain.h"
#include "RDeviceD3D.h"
#include "RTexture.h"

#include "MDebug.h"
#include "RUtil.h"

//#include "RToolUtil.h"

using namespace rs3;

//DoAction Stuff Namespace
namespace WB_TerrainAction_NS {

//�Ϲ����� ���� �� ��ȭ �Լ�
float fEditHeight(float fHeight, float fRange, float fScale, float fFactor)
{
	return fHeight + ( fRange * fScale );
}

//���ȭ �Լ�
float fEditFlat(float fHeight, float fRange, float fScale, float fFactor)
{
	if ( fRange > 0.0f )
		return fHeight*(1.0f-fScale) + fFactor*fScale;
	else
		return fHeight;
}

//��հ��� ���� ����
float ns_fSum;
int ns_nCount;

void InitAver()
{
	ns_fSum = 0.0f;
	ns_nCount = 0;
}

//����� ���ϴ� �Լ�
float fGetAver(float fHeight, float fRange, float fScale, float fFactor)
{
	if ( fRange > 0.0f )
	{
		ns_fSum += fHeight;
		++ns_nCount;
	}

	return fHeight;
}

inline void RenderTerrainActionCursor(
	const rs3::RVector* _pSetVec, const rs3::RVector& _refScaleVec, const DWORD _dwColor)
{
	L3DCursor::GetInstance()->SetType(L3DCursor::CYLINDER, true);
	L3DCursor::GetInstance()->SetTransform(*_pSetVec, _refScaleVec);
	L3DCursor::GetInstance()->SetColor(_dwColor);
	//�Ϳ�, Į�� �Ǵ±���.
	//L3DCursor::GetInstance()->SetColor(D3DCOLOR_XRGB(255, 255, 255));
	//*_pGetVec = *L3DCursor::GetInstance()->GetPos();
}

} //namespace ��

using namespace WB_TerrainAction_NS;

void CControlDoAction::TerrainHeight(CControlMediator* _pController)
{
	_pController->m_mouseInfo.setIgnoreMouseMovedPickingRActor( true );

	//Ȯ�� - ������ �ְ�, �������� �׼��� �� ���콺�� �ִ���
	RToolTerrain* pTerrain = (RToolTerrain*) _pController->GetToolSceneManager()->GetCurrentTerrain();
	if ( pTerrain == NULL)
		return;

	const RVector* pMousePosVec = _pController->GetHelperAction()->GetTerrainContactPos();
	if( pMousePosVec == NULL )
		return;

	float fSize = _pController->GetViewPtr<CRollupTerrainHeight>()->m_fEditSize;
	float fStrength = _pController->GetViewPtr<CRollupTerrainHeight>()->m_fStr;

	//Set Scale value
	float fScale = 1.0f;
	if ( GetAsyncKeyState(VK_LSHIFT) ) fScale = 2.0f;
	if ( GetAsyncKeyState(VK_LCONTROL) ) fScale = -fScale;
	fStrength *= fScale;

	//Apply Cursor
	RenderTerrainActionCursor(pMousePosVec,
		RVector(fSize * pTerrain->GetXScale(), fSize * pTerrain->GetYScale(), pTerrain->GetHSize() * 2.01f),
		D3DCOLOR_XRGB(60, 0, 250));

	//Doing Action
	if ( _pController->m_mouseInfo.mouseState.bLButton == true )
	{
		//Undo Entry Point : DoAction�� �������̸� �������� ���� �����Ѵ�.
		CToolCmdMgr* pCommandManager = _pController->GetToolCmdMgr();
		if(pCommandManager->IsMouseCmdEntryL())
		{
			CTerrainHeightCmdBuffer* pCmdBuffer = new CTerrainHeightCmdBuffer(pTerrain);
			pCommandManager->RecordCommand(pCmdBuffer);
			pCommandManager->DoneMouseCmdL();
		}
		pTerrain->ApplyCircleActionWithFuncPtr(pMousePosVec, fSize, fStrength, 0.0f, fEditHeight);
		// ���� ������ �븻 ����
		pTerrain->UpdateNormal( _pController->m_mapInfo.m_colFace + 1, _pController->m_mapInfo.m_rowFace + 1, _pController->m_mapInfo.m_width, _pController->m_mapInfo.m_height );
	}
}

void CControlDoAction::TerrainAver(CControlMediator* _pController)
{
	_pController->m_mouseInfo.setIgnoreMouseMovedPickingRActor( true );

	//Ȯ�� - ������ �ְ�, �������� �׼��� �� ���콺�� �ִ���
	RToolTerrain* pTerrain = (RToolTerrain*) _pController->GetToolSceneManager()->GetCurrentTerrain();
	if ( pTerrain == NULL )
		return;

	const RVector* pMousePosVec = _pController->GetHelperAction()->GetTerrainContactPos();
	if( pMousePosVec == NULL )
		return;

	float fSize = _pController->GetViewPtr<CRollupTerrainHeight>()->m_fEditSize;
	float fAverRatePer = _pController->GetViewPtr<CRollupTerrainAverage>()->m_fStr;

	//���ӱ��
	float fScale = 1.0f;
	if ( GetAsyncKeyState(VK_LSHIFT) ) fScale = 2.0f;
	fAverRatePer = fAverRatePer * fScale;

	//Apply Cursor
	RenderTerrainActionCursor(pMousePosVec,
		RVector(fSize * pTerrain->GetXScale(), fSize * pTerrain->GetYScale(), pTerrain->GetHSize() * 2.01f),
		D3DCOLOR_XRGB(60, 0, 250));

	//Doing Action
	if ( _pController->m_mouseInfo.mouseState.bLButton == true )
	{
		//Undo Entry Point : DoAction�� �������̸� �������� ���� �����Ѵ�.
		CToolCmdMgr* pCommandManager = _pController->GetToolCmdMgr();
		if(pCommandManager->IsMouseCmdEntryL())
		{
			CTerrainHeightCmdBuffer* pCmdBuffer = new CTerrainHeightCmdBuffer(pTerrain);
			pCommandManager->RecordCommand(pCmdBuffer);
			pCommandManager->DoneMouseCmdL();
		}

		// ��� ���� ��� ...
		InitAver();
		pTerrain->ApplyCircleActionWithFuncPtr(pMousePosVec, fSize, 0.0f, 0.0f, fGetAver, false);
		float averHeight = ns_fSum / ns_nCount;

		// �ۼ�Ʈ�� 0.0~1.0 ���� �ٲ㼭
		float f = fAverRatePer / 100.0f;
		if ( f > 1.0f )		f = 1.0f;
		if ( f < 0.0f )		f = 0.0f;

		//fEditFlat ����
		pTerrain->ApplyCircleActionWithFuncPtr( pMousePosVec, fSize, f, averHeight, fEditFlat);
		pTerrain->UpdateNormal( _pController->m_mapInfo.m_colFace + 1, _pController->m_mapInfo.m_rowFace + 1, _pController->m_mapInfo.m_width, _pController->m_mapInfo.m_height );
	}
}

void CControlDoAction::TerrainFlat(CControlMediator* _pController)
{
	_pController->m_mouseInfo.setIgnoreMouseMovedPickingRActor( true );

	RToolTerrain* pTerrain = (RToolTerrain*) _pController->GetToolSceneManager()->GetCurrentTerrain();
	if ( pTerrain == NULL)
		return;

	const RVector* pMousePosVec = _pController->GetHelperAction()->GetTerrainContactPos();
	if( pMousePosVec == NULL )
		return;

	float fSize = _pController->GetViewPtr<CRollupTerrainHeight>()->m_fEditSize;
	float fAverRatePer = _pController->GetViewPtr<CRollupTerrainFlat>()->m_fStr;

	//���ӱ��
	float fScale = 1.0f;
	if ( GetAsyncKeyState(VK_LSHIFT) ) fScale = 2.0f;
	fAverRatePer = fAverRatePer * fScale;

	//Apply Cursor
	RenderTerrainActionCursor(
		pMousePosVec,
		RVector(fSize * pTerrain->GetXScale(), fSize * pTerrain->GetYScale(), pTerrain->GetHSize() * 2.01f),
		D3DCOLOR_XRGB(60, 0, 250));

	static float fActionEntryHeight;
	//Doing Action
	if( _pController->m_mouseInfo.mouseState.bLButton )
	{
		//Undo Entry Point : DoAction�� �������̸� �������� ���� �����Ѵ�.
		CToolCmdMgr* pCommandManager = _pController->GetToolCmdMgr();
		if(pCommandManager->IsMouseCmdEntryL())
		{
			//ó�� ���������Ҷ� ���� base�� ��������.
			fActionEntryHeight = pMousePosVec->z;

			CTerrainHeightCmdBuffer* pCmdBuffer = new CTerrainHeightCmdBuffer(pTerrain);
			pCommandManager->RecordCommand(pCmdBuffer);
			pCommandManager->DoneMouseCmdL();
		}

		float f = fAverRatePer / 100.0f;
		if ( f > 1.0f )		f = 1.0f;
		if ( f < 0.0f )		f = 0.0f;

		//apply action
		pTerrain->ApplyCircleActionWithFuncPtr( pMousePosVec, fSize, f, fActionEntryHeight, fEditFlat);
		pTerrain->UpdateNormal( _pController->m_mapInfo.m_colFace + 1, _pController->m_mapInfo.m_rowFace + 1, _pController->m_mapInfo.m_width, _pController->m_mapInfo.m_height );
	}
}

void CControlDoAction::TerrainSplat(CControlMediator* _pController)
{
	_pController->m_mouseInfo.setIgnoreMouseMovedPickingRActor( true );

	RToolTerrain* pTerrain = (RToolTerrain*) _pController->GetToolSceneManager()->GetCurrentTerrain();
	if ( pTerrain == NULL )
		return;

	const RVector* pMousePosVec = _pController->GetHelperAction()->GetTerrainContactPos();
	if( pMousePosVec == NULL )
		return;

	CRollupTerrainSplat* pEditSplat = _pController->GetViewPtr<CRollupTerrainSplat>();
	int nLayerIndex = pEditSplat->m_nCurrentLayer;
	float fSize     = pEditSplat->m_fEditSize;
	float fStrength = pEditSplat->m_fStr;

	//Apply Cursor
	RenderTerrainActionCursor(
		pMousePosVec,
		RVector(fSize * pTerrain->GetXScale(), fSize * pTerrain->GetYScale(), pTerrain->GetHSize() * 2.01f),
		D3DCOLOR_XRGB(60, 0, 250));

	if ( _pController->m_mouseInfo.mouseState.bLButton )
	{
		float fScale = 1.0f;
		if ( GetAsyncKeyState(VK_LSHIFT) )
			fScale = 2.0f;

		if ( GetAsyncKeyState(VK_LCONTROL) )
			fScale = -fScale;

		if ( nLayerIndex < 0 )
			return;
		if ( nLayerIndex >= pTerrain->GetLayerNum())
			return;

		// �ش�Ǵ� u, v ��ǥ�� ��´�.
		float u = (pMousePosVec->x / pTerrain->GetWidthSize());
		float v = (pMousePosVec->y / pTerrain->GetHeightSize());

		BYTE *pByte = pTerrain->GetAlphaTexturePtr(nLayerIndex);
		if (pByte == NULL)
		{
			pTerrain->CreateAlphaTexture(nLayerIndex);
			pByte = pTerrain->GetAlphaTexturePtr(nLayerIndex);
			_ASSERT(pByte);
		}

		DWORD dwWidth = pTerrain->GetAlphaTextureWidth(nLayerIndex);
		DWORD dwHeight = pTerrain->GetAlphaTextureHeight(nLayerIndex);

		//Undo Entry Point : DoAction�� �������̸� �������� ���� �����Ѵ�.
		CToolCmdMgr* pCommandManager = _pController->GetToolCmdMgr();
		if(pCommandManager->IsMouseCmdEntryL())
		{
			CTerrainSplatCmdBuffer* pCmdBuffer = new CTerrainSplatCmdBuffer(nLayerIndex, dwWidth, dwHeight, pByte);
			pCommandManager->RecordCommand(pCmdBuffer);
			pCommandManager->DoneMouseCmdL();
		}

		int nX = int(u*dwWidth+0.5f);
		int nY = int(v*dwHeight+0.5f);

		float fUOffset = 1.0f/(float)dwWidth;
		float fVOffset = 1.0f/(float)dwHeight;

		float fUnitU = pTerrain->GetXScale()/pTerrain->GetWidthSize();
		float fUnitV = pTerrain->GetYScale()/pTerrain->GetHeightSize();

		RECT rt;

		int sizeppx = (int)((fSize + fSize/2)*fUnitU*dwWidth + 0.5f);
		int sizeppy = (int)((fSize + fSize/2)*fUnitV*dwHeight + 0.5f);

		rt.left = nX - sizeppx;
		rt.right = nX + sizeppx;
		rt.top = nY - sizeppy;
		rt.bottom = nY + sizeppy;

		float lx = (fUnitU*fSize);
		float ly = (fUnitV*fSize);
		float minlen = min(lx,ly);

		if ( rt.left < 0 )	rt.left = 0;
		if ( rt.left  >= (int)dwWidth )		rt.left = dwWidth-1;
		if ( rt.right >= (int)dwWidth )		rt.right = dwWidth-1;

		if ( rt.top < 0 )	rt.top = 0;
		if ( rt.top >= (int)dwHeight )		rt.top = dwHeight-1;
		if ( rt.bottom >= (int)dwHeight )	rt.bottom = dwHeight-1;

		for ( int y = rt.top; y<=rt.bottom; ++y )
		{
			for ( int x = rt.left; x<=rt.right; ++x )
			{
				int nIndex = (dwWidth*y + x)*4;
				BYTE *pPixel = &pByte[nIndex+3];

				float fx = (float)x/(float)(dwWidth) + fUOffset*0.5f;
				float fy = (float)y/(float)(dwHeight) + fUOffset*0.5f;

				float dx = fx - u;
				float dy = fy - v;

				float f = sqrtf(dx*dx+dy*dy) / minlen;
				if ( f < 0.0f )
					f = 0.0f;
				if ( f > 1.0f )		
					f = 1.0f;

				f = cosf(f*MMath::PI/2);
				int val = int(f*fStrength*fScale + 0.5f);
				int n = *pPixel + val;

				if ( n < 0 )
					n = 0;
				if ( n > 255 )
					n = 255;

				*pPixel = n;
			}
		}
		pTerrain->UpdateAlphaTexture( nLayerIndex);

		// AlphaMask map�� ������ ���� ���ϴ� DescMap�� �����Ѵ�.
		RECT dRt;
		dRt.left = ((float)rt.left/(float)dwWidth)* pTerrain->GetWidthMap();
		dRt.right = ((float)rt.right/(float)dwWidth)*pTerrain->GetWidthMap() + 1;
		dRt.top = ((float)rt.top/(float)dwHeight)*pTerrain->GetHeightMap();
		dRt.bottom = ((float)rt.bottom/(float)dwHeight)*pTerrain->GetHeightMap() + 1;
		pTerrain->UpdateDescriptMapAndMaterial(dRt);
	}
}

void CControlDoAction::TerrainRelax(CControlMediator* _pController)
{
	_pController->m_mouseInfo.setIgnoreMouseMovedPickingRActor( true );

	RToolTerrain* pTerrain = (RToolTerrain*) _pController->GetToolSceneManager()->GetCurrentTerrain();
	if ( pTerrain == NULL )
		return;

	const RVector* pMousePosVec = _pController->GetHelperAction()->GetTerrainContactPos();
	if( pMousePosVec == NULL )
		return;

	float fSize = _pController->GetViewPtr<CRollupTerrainRelax>()->m_fEditSize;
	float fstrength = _pController->GetViewPtr<CRollupTerrainRelax>()->m_fStr;

	//Apply Cursor
	float fTerrainXScale = pTerrain->GetXScale();
	float fTerrainYScale = pTerrain->GetYScale();

	RenderTerrainActionCursor(
		pMousePosVec,
		RVector(fSize * fTerrainXScale, fSize * fTerrainYScale, pTerrain->GetHSize() * 2.01f),
		D3DCOLOR_XRGB(60, 0, 250));

	//Action
	if ( _pController->m_mouseInfo.mouseState.bLButton )
	{
		CToolCmdMgr* pCommandManager = _pController->GetToolCmdMgr();
		if(pCommandManager->IsMouseCmdEntryL())
		{
			CTerrainHeightCmdBuffer* pCmdBuffer = new CTerrainHeightCmdBuffer(pTerrain);
			pCommandManager->RecordCommand(pCmdBuffer);
			pCommandManager->DoneMouseCmdL();
		}

		float fScale = 1.0f;
		if ( GetAsyncKeyState(VK_LSHIFT) )
			fScale = 2.0f;
		fstrength = fstrength * fScale;

		RECT rt;
		int nX = (int)(pMousePosVec->x / fTerrainXScale + 0.5f);
		int nY = (int)(pMousePosVec->y / fTerrainYScale + 0.5f);

		int sizepp = int(fSize+0.5f) + int(fSize/2.0f);
		if ( fSize < 2.5f )
			sizepp += 1;

		rt.left = nX - sizepp;
		rt.right = nX + sizepp;
		rt.top = nY - sizepp;
		rt.bottom = nY + sizepp;

		pTerrain->ApplyRelaxAction(fstrength, sizepp, &rt);
	}
}

void CControlDoAction::TerrainUserMaterial(CControlMediator* _pController)
{
#if 0
	RToolTerrain* pTerrain = (RToolTerrain*) _pController->GetToolSceneManager()->GetCurrentTerrain();
	if ( pTerrain == NULL )
		return;

	const RVector* pMousePosVec = _pController->GetHelperAction()->GetTerrainContactPos();
	if( pMousePosVec == NULL )
		return;

	CEditTerrainUserMaterial *pVEditTerrainUserMaterial = _pController->T_GetView<CEditTerrainUserMaterial>(VIEW_ID::PANEL_TERRAIN_USERMATERIAL);
	_ASSERT(pVEditTerrainUserMaterial!=NULL);

	float fSize = pVEditTerrainUserMaterial->m_fEditSize;

	// Apply Cursor
	float fTerrainXScale = pTerrain->GetXScale();
	float fTerrainYScale = pTerrain->GetYScale();

	RenderTerrainActionCursor(
		pMousePosVec,
		RVector(fSize * fTerrainXScale, fSize * fTerrainYScale, pTerrain->GetHSize() * 2.01f),
		D3DCOLOR_XRGB(250, 0, 60));

	// Action
	if ( _pController->m_mouseInfo.mouseState.bLButton )
	{
		// TODO : Undo ��� ����
		pVEditTerrainUserMaterial->UpdateData(TRUE);
		DWORD dwMaterialID = pVEditTerrainUserMaterial->m_dwEditID;

		pTerrain->ApplyUserMaterial(pMousePosVec, fSize, dwMaterialID);
		if ( pVEditTerrainUserMaterial->m_btnView.GetCheck())
			pTerrain->UpdateUserMaterialMap(dwMaterialID);
	}
#endif
}

void CControlDoAction::TerrainGrass(CControlMediator* _pController)
{
	_pController->m_mouseInfo.setIgnoreMouseMovedPickingRActor( true );

	RToolTerrain* pTerrain = (RToolTerrain*) _pController->GetToolSceneManager()->GetCurrentTerrain();
	if ( pTerrain == NULL )
		return;

	const RVector* pMousePosVec = _pController->GetHelperAction()->GetTerrainContactPos();
	if( pMousePosVec == NULL )
		return;

	rs3::RGrassSystem* pGrassSystem = pTerrain->GetGrassSystem();
	if(pGrassSystem == NULL) return;

	CRollupTerrainGrass* pTerrainGrassView =  _pController->GetViewPtr<CRollupTerrainGrass>();

	//Set Argument Value
	float fSize = pTerrainGrassView->m_fEditSize;
	float lx = fSize*pTerrain->GetXScale();
	float ly = fSize*pTerrain->GetYScale();
	float fRealSize = min(lx,ly);

	if(!GetAsyncKeyState(VK_CONTROL) ) //Add
	{
		//Ŀ�� ����
		RenderTerrainActionCursor(
			pMousePosVec,
			RVector(lx, ly, pTerrain->GetHSize() * 2.01f),
			D3DCOLOR_XRGB(60, 0, 250) );

		//���� ����Ʈ 1 evented ���� - ontime���� �ݹ��� �˱�����
		if(_pController->m_mouseInfo.mouseState.bEvented == false) return;

		//evented ��ȿȭ(Ȯ�������Ƿ� ������ event ���������� event �ƴ����� �Ǻ�)
		_pController->m_mouseInfo.mouseState.bEvented = false;

		//���� ����Ʈ 2
		if( _pController->m_mouseInfo.mouseState.bDrag == false &&
			_pController->m_mouseInfo.mouseState.bLButton == false)
			return;

		if( _pController->m_mouseInfo.mouseState.bDrag )
		{
			//SENSITIVE �� Ŀ������ �� �ΰ��Ѱ�
			int SENSITIVE = (int)pTerrainGrassView->m_nSpace;
			static int moved_x = 0;
			static int moved_y = 0;
			int abs_x = abs(_pController->m_mouseInfo.m_movedMouseX );
			int abs_y = abs(_pController->m_mouseInfo.m_movedMouseY );
			moved_x += abs_x;
			moved_y += abs_y;
			if(moved_x < SENSITIVE && moved_y < SENSITIVE)
				return;
			else
			{
				moved_x = 0;
				moved_y = 0;
			}
		}
		else if (_pController->m_mouseInfo.mouseState.bLButton == false)
			return;

		std::vector<SPieceGrassInfo*> grassCategory;
		pTerrainGrassView->GetCurrentPieceGrassVector(grassCategory);
		if(grassCategory.empty()) return;

		int nCategorySize = (int)grassCategory.size();
		for(int i_category = 0; i_category < nCategorySize; ++i_category)
		{
			SPieceGrassInfo* pGrassInfo = grassCategory[i_category];
			if(pGrassInfo == NULL) return;

			rs3::RVector vTerrainZ;

			// 1 fSize�� �ɱ�� ��
			int grassNum = pGrassInfo->m_nNumber * fSize * fSize * pTerrainGrassView->m_fStr * 0.2;
			//�ּҰ��� 1��
			if(grassNum < 1 ) grassNum = 1;
			
			int pieceNum = pGrassSystem->GetGrassID(pGrassInfo->m_strPieceGrassName.c_str());
			int grassSize = pGrassInfo->m_nSize;
			_ASSERT(pieceNum>=0);
	
			// by rOseria
			// Grass Size�� 256px�� �������� �ٽ� ��� ( Move to GetGrassVertex Funtion)
			//float fGrassWidth = grassSize * pGrassSystem->GetGrassWidth(pieceNum)/256.f;
			//float fGrassHeight = grassSize * pGrassSystem->GetGrassHeight(pieceNum)/256.f;

			float fX = pMousePosVec->x;
			float fY = pMousePosVec->y;
			float fRate = 1.0f;

			int nFailCnt = 0;
			rs3::RGrassBlade b;
			for ( int i = 0; i < grassNum; ++i)
			{
				if(grassNum > 1)
				{
					//�����ϰ� �ѷ����
					//b.m_afPos[0] = RandomNumber(fX-fRealSize, fX+fRealSize); //x ��ǥ
					//b.m_afPos[1] = RandomNumber(fY-fRealSize, fY+fRealSize); //y ��ǥ
					// ���� �������� �����ϰ� �Ѵ�. - 090610, OZ
					float fRandCir = RandomNumber( 0.0f, 31.4f);
					float fRandFar = RandomNumber( 0.0f, fRealSize);
					b.m_afPos[0] = fX + sin( fRandCir) * fRandFar; //x ��ǥ
					b.m_afPos[1] = fY + cos( fRandCir) * fRandFar; //y ��ǥ
				}
				else
				{
					b.m_afPos[0] = fX;
					b.m_afPos[1] = fY;
				}

				//�� ���� �ȿ����� Ǯ�ɱ�
				RVector vNorm;
				if(pTerrain->GetPickVertical(vTerrainZ, &vNorm, b.m_afPos[0], b.m_afPos[1]))
				{
					RVector4 vColor;
					//pTerrain->GetPickLightMapColor(vColor, vTerrainZ);
					vColor.x = vColor.y = vColor.z = vColor.a = 1.0f;
					
					
					DWORD dwColor = D3DCOLOR_COLORVALUE(vColor.x, vColor.y, vColor.z, vColor.a);
					b.m_dwBottomColor = dwColor;
					b.m_dwTopColor = dwColor;

					b.m_afPos[2] = vTerrainZ.z; //z��ǥ
					b.m_fNoise = CSpeedGrassRT::NOISE;

					//������� 80 ~ 120 ���� ����
					fRate = RandomNumber(0.8f, 1.2f);

					// �븻 : �븻������� GetPickVertical������ ���� ��.
					b.m_afNorm[0] = vNorm[0];
					b.m_afNorm[1] = vNorm[1];
					b.m_afNorm[2] = vNorm[2];

					//�¿�� 30%  ����
					if ( ::rand()%10 < 3 ) fRate = -fRate;

					if(grassNum > 1)
					{
						b.m_fSize = grassSize * fRate;
						b.m_fThrow = CSpeedGrassRT::THROW;
						b.m_nWhichTexture = pieceNum;
						if(!pGrassSystem->AddBlade(b))
							nFailCnt++;
					}
					else //�Ѱ��̸� �Ѱ��� �����ϰ� ����ְ� ���´�.
					{
						pGrassInfo = grassCategory[RandomNumber(0, nCategorySize-1)];
						pieceNum = pGrassSystem->GetGrassID(pGrassInfo->m_strPieceGrassName.c_str());
						grassSize = pGrassInfo->m_nSize;
						_ASSERT(pieceNum>=0);

						b.m_fSize = grassSize * fRate;
						b.m_fThrow = CSpeedGrassRT::THROW;
						b.m_nWhichTexture = pieceNum;
						if(!pGrassSystem->AddBlade(b))
							nFailCnt++;

						//��Ƽ �÷��� üũ
						_pController->GetToolCmdMgr()->RecordCommand(NULL);
						return;
					}
				}
				else
				{
					nFailCnt++;
				}
			}

			//��Ƽ �÷��� üũ
			_pController->GetToolCmdMgr()->RecordCommand(NULL);

			if( nFailCnt)	// Ǯ ���� ����� ���� �α� - 090609, OZ
			{
				if( nFailCnt != grassNum)
					mlog( " Ǯ %d�� �� %d���� �ɴ� �� �����Ͽ����ϴ�. �е��� ���ų� ���� ���� �� �ֽ��ϴ�. \n", grassNum, nFailCnt);
				else
					mlog( " Ǯ %d �� ���� ���� ���մϴ�. �е��� ���ų� ���� ���� �� �ֽ��ϴ�. \n", grassNum);
			}
		}
	}
	else //�����
	{
		RenderTerrainActionCursor(
			pMousePosVec,
			RVector(lx, ly, pTerrain->GetHSize() * 2.01f),
			D3DCOLOR_XRGB(255, 255, 255) );

		//evented ���� - ontime���� �ݹ��� �˱�����
		if(_pController->m_mouseInfo.mouseState.bEvented == false) return;

		//evented ��ȿȭ(Ȯ�������Ƿ� ������ event ���������� event �ƴ����� �Ǻ�)
		_pController->m_mouseInfo.mouseState.bEvented = false;

		if( _pController->m_mouseInfo.mouseState.bLButton)
		{
			_pController->GetToolCmdMgr()->RecordCommand(NULL);
			pGrassSystem->DeleteBlade(pMousePosVec->x, pMousePosVec->y, fRealSize);
		}
	}
}
