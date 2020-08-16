#pragma once

#include "RDevice.h"
#include "RViewFrustum.h"

class MOBox;

namespace rs3 {

class RS_API RRenderHelper
{
public:

	static void SetRenderState(bool bWireframe = false);
	static void EndRenderState();

	static void RenderBox(const RMatrix &matWorld, const RBoundingBox &box, DWORD dwColor = 0xffffffff, bool bLine = true);
	static void RenderBox(const RMatrix &matWorld, float _fBoxSize, DWORD dwColor = 0xffffffff, bool bLine = true);
	static void RenderObb(const MOBox &obb, DWORD dwColor = 0xffffffff);
	static void RenderPlane(const RMatrix &matWorld, RVector3 AxisX, RVector3 AxisY, float width, float height
		, RTexture *pTexture = NULL);

	static void RenderSphere(const RMatrix &matWorld, float fRadius, DWORD dwColor = 0xffffffff);
	static void RenderSphere(const RSphere &sphere, DWORD dwColor = 0xffffffff);

	// �ظ��� �߽�(0,0,0), ������ fRadius, z������ fHeight������ ������ �׸���
	static void RenderCorn(const RMatrix &matWorld, float fRadius, float fHeight, DWORD dwColor);
	// pos = �ظ��� �߽�, radius �ظ��� ������, height ����
	static void RenderCorn(const RVector& pos, const RVector& dir, float fRadius, float fHeight, DWORD dwColor);

	// �߽�(0,0,0), z������ 0~fHeight��ŭ ���ִ� �ݰ� fRadius , �Ǹ����� �׸���
	static void RenderCylinder(const RMatrix &matWorld, float fRadius, float fHeight, DWORD dwColor = 0xffffffff, bool bLine = false);
	// pos = �ظ��� �߽�, dir = ���������� �����
	static void RenderCylinder(const RVector& pos, const RVector& dir, float fRadius, float fHeight, DWORD dwColor);

	static void RenderCapsule(const RMatrix &matWorld, float fRadius, float fHeight, DWORD dwColor = 0xffffffff, bool bLine = false);
	static void RenderCapsule(const RCapsule& c, DWORD dwColor = 0xffffffff);
	static void RenderCapsule(const RMatrix &matWorld, const RCapsule& c, DWORD dwColor = 0xffffffff);

	static void RenderDonut(const RVector& pos, const RVector& dir, float fRadius, float fHeight, DWORD dwColor);
	static void RenderDonut(const RMatrix &matWorld, float fRadius, float fHeight, DWORD dwColor = 0xffffffff, bool bLine = false);

	static void RenderCircle(const RMatrix &matWorld, float fRadius, DWORD dwColor = 0xffffffff, bool bRenderCross = false);

	static void RenderRing(const RMatrix &matWorld, float fRadius, float fHeight, DWORD dwColor = 0xffffffff, bool bLine = false);

	static void RenderGrid(const RMatrix &matWorld, float fSegmentSize, int nSegmentCountX, int nSegmentCountY, DWORD dwColor = 0xffffffff);
	static void RenderLine(const RMatrix &matWorld, RVector &v1,RVector &v2,DWORD dwColor);
	static void RenderLineArray(const RMatrix &matWorld, RVector *pVertices, int nPrimCount, DWORD dwColor);
	static void RenderLines(const RMatrix &matWorld, const RVector *pVertices, int nPrimCount, DWORD dwColor,RPRIMITIVETYPE type = RPT_LINELIST);
	static void RenderCenter(const RMatrix &matWorld, float size_w,float size_h,DWORD color);

	static void RenderTriangles(const RMatrix &matWorld, RVector *pVertices, int nCount, DWORD dwColor,RPRIMITIVETYPE type = RPT_TRIANGLELIST);

	// �� �׸���
	static void RenderAxis(const RMatrix &matWorld, float fSize, float fCylRad = .5f, float fCornRad = 2.f, float fCornLen = 3.f);
	
	// ȭ�� �׸���
	static void RenderArrow(const RVector& pos, const RVector& dir, float fCylinderRadius, float fCylinderLength, float fCornRadius, float fCornLength, DWORD dwColor);

	// �簢�� �׸���
	static void	RenderQuad(float x, float y, float width, float height, float tuS=0, float tvS=0, float tuE=1, float tvE=1);

	// ���� �ִ� �簢�� �׸���
	static void	RenderQuadColor(float x, float y, float width, float height, DWORD dwColorLT, DWORD dwColorRT, DWORD dwColorLB, DWORD dwColorRB );

	// �ؽ��� �׸���
	static void	RenderTexture(float x, float y, float width, float height, RTexture* hTexture, float tu=0, float tv=0, RBLEND blend = RBLEND_NONE, RTEXTUREFILTERTYPE filter = RTF_POINT);

	// 2D �ؽ�Ʈ �׸���
	static void RenderText(const RVector& vPos, const char* fontName, float fSize, DWORD dwColor, const char* chText, const int nOffset_x = 0, const int nOffset_y = 0);

	static void	RenderPyramid(const RVector& vTop, const RViewFrustum& viewFrustum, DWORD dwColor, bool bLine);

	static class RBatchLineDrawer
	{
		vector<RVector> m_vertices;

	public:
		void Add(RVector& p1, RVector& p2);
		void Render(const RMatrix &matWorld, DWORD dwColor = 0xffffffff);
	} m_batchLineDrawer;

	//// 3D��ǥ�� 2D�� ��������
	//static void Get2DPosFrom3DPos(MVector2 *pScreen, const MVector3 *position);
};

//////////////////////////////////////////////////////////////////////////
// inline functions

inline void RRenderHelper::RBatchLineDrawer::Add(RVector& p1, RVector& p2)
{
	m_vertices.push_back(p1);
	m_vertices.push_back(p2);
}

inline void RRenderHelper::RBatchLineDrawer::Render(const RMatrix &matWorld, DWORD dwColor)
{
	const int MAX_LINE_COUNT = 1000;

	size_t nRemain = m_vertices.size() / 2;
	for(size_t i=0;i< nRemain / MAX_LINE_COUNT; i++)
	{
		RRenderHelper::RenderLineArray(matWorld,&m_vertices[i*2*MAX_LINE_COUNT],MAX_LINE_COUNT,dwColor);
	}
	// ������ ������ ���� �׸���
	nRemain =  nRemain % MAX_LINE_COUNT;
	if(nRemain)
		RRenderHelper::RenderLineArray(matWorld,&m_vertices[m_vertices.size() - 2*nRemain], (int)nRemain,dwColor);
	m_vertices.clear();
}

}
