#include "stdafx.h"

#include "MCplug3.h"

/////////////////////////////////////////////////////////////
// ani out

BOOL EqualPoint3(Point3 p1, Point3 p2)
{
	if (fabs(p1.x - p2.x) > _ALMOST_ZERO)	return FALSE;
	if (fabs(p1.y - p2.y) > _ALMOST_ZERO)	return FALSE;
	if (fabs(p1.z - p2.z) > _ALMOST_ZERO)	return FALSE;

	return TRUE;
}

BOOL MCplug2::IsKnownController(Control* cont)
{
	ulong partA, partB;

	if (!cont)	return FALSE;

	partA = cont->ClassID().PartA();
	partB = cont->ClassID().PartB();

	if (partB != 0x00)	return FALSE;

	switch (partA) 
	{
		case TCBINTERP_POSITION_CLASS_ID:
		case TCBINTERP_ROTATION_CLASS_ID:
		case TCBINTERP_SCALE_CLASS_ID:
		case HYBRIDINTERP_POSITION_CLASS_ID:
		case HYBRIDINTERP_ROTATION_CLASS_ID:
		case HYBRIDINTERP_SCALE_CLASS_ID:
		case LININTERP_POSITION_CLASS_ID:
		case LININTERP_ROTATION_CLASS_ID:
		case LININTERP_SCALE_CLASS_ID:
			return TRUE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// �ִϸ��̼�
void MCplug2::ExportAnimation(INode* node, RMaxMeshNode* mesh_node) 
{
	BOOL bPosAnim;
	BOOL bRotAnim;
	BOOL bScaleAnim;
	BOOL bDoKeys = FALSE;

	// dummy_loc �� �ְ�, bip01 �� ������ bip01 �� �����ϰ� dummy_loc�� �Ű��ش�.
	bool bNeedToMove = 
		m_pBip01Node && (m_eDummyLocType == DUMMY_LOC || m_eDummyLocType == DUMMY_LOC_DIR) &&
		(node==m_pBip01Node || node==m_pDummyLocNode );

	// Ű �ִϸ��̼�
	BOOL bExistAnimation = CheckForAnimation(node, bPosAnim, bRotAnim, bScaleAnim);
	if (bExistAnimation || bNeedToMove)
	{
		// �ִϸ��̼��� ��� bip01 �� dummy_loc �� ���ø��� �ؾ��Ѵ�
		if(bNeedToMove)
		{
			bPosAnim = TRUE;
			bRotAnim = TRUE;
		}

		DumpSRTSample( node, mesh_node, bPosAnim, bRotAnim, bScaleAnim );
	}
	DumpVisSample(node, mesh_node);

	// ���ؽ� �ִϸ��̼�
	if(m_options.m_bVertexAnimationOut) {
		DumpVertexAni(node, mesh_node);
	}

	// ��ü �ִϸ��̼� �������� �ִ� AABB�� ���Ѵ�. // �ִϸ��̼� ����� �ʿ��Ҷ��� ������ �Ѵ�.
	if(m_options.m_bKeyAnimationOut || m_options.m_bVertexAnimationOut) {
		DumpMaxAniAABB(node, mesh_node);
	}
}

// ���� �������� �ϳ��� animation ���Ͼȿ� AABB ũ�� ��ü�� ��
// AABB * Bip01 (Z�� �̵��� ȸ������ ������ Bip01 Transform, ������ TM�� Dummy�� ��ϵȴ�.)
// Dummy�� Biped�� ��ġ������ ����Ѵ�.
void MCplug2::DumpMaxAniAABB(INode* node, RMaxMeshNode* mesh_node)
{
	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end	= ip->GetAnimRange().End();
	TimeValue t;

	int delta = GetTicksPerFrame() * GetKeyFrameStep();

	Matrix3 tm(1);
	tm.IdentityMatrix();
	mesh_node->m_MaxAniBoundingBox.Initialize();

	//////////////////////////////////////////////////////

	BOOL	needDel = FALSE;;
	int		i;
	Point3	v;

	string nodeName = node->GetName();

	Matrix3 tmBip01(1);	// 0 �������� Bip 01 ��ġ ��
	Point3 vTransBip01(0,0,0);

	//if( m_pBip01Node )
	//{
	//	tmBip01 = m_pBip01Node->GetNodeTM(start);
	//	vTransBip01 = tmBip01.GetTrans();
	//}

	for ( t = start; t <= end; t += delta ) 
	{
		Matrix3 tmInvBip01(1);
		Matrix3 tmRemainBip01(1);
		Point3 vInvTransBip01(0, 0, 0);
		if( (m_eDummyLocType == DUMMY_LOC || m_eDummyLocType == DUMMY_LOC_DIR) && ( m_pBip01Node ) )
		{
			tmInvBip01 = m_pBip01Node->GetObjTMAfterWSM(t);
			vInvTransBip01 = tmInvBip01.GetTrans();
			vInvTransBip01 -= vTransBip01;	// start ������°�� Bip01 ��ġ���� ���ܽ�Ų��.

			// �� ���� ������ Bip01 TM�� ���� �����ش�.
			Matrix3 tmDummyRefined;
			Matrix3 tmDummyRemains;
			if(m_eDummyLocType == DUMMY_LOC_DIR)
				GetRefinedTransform_loc_dir( m_pBip01Node, t, tmDummyRefined, tmDummyRemains );
			else
				GetRefinedTransform_loc( m_pBip01Node, t, tmDummyRefined, tmDummyRemains );

			// Remain �� bip01 TM�� ���� ��Ų��.
			tmRemainBip01 = tmDummyRemains;// * Inverse( node->GetParentTM(t) );
		}

		tm = node->GetObjTMAfterWSM(t);

		PolyObject *poly = NULL;
		Object *obj = node->EvalWorldState(t).obj;
		if (obj->CanConvertToType(Class_ID(POLYOBJ_CLASS_ID, 0)))
		{
			poly = (PolyObject*)obj->ConvertToType(t, Class_ID(POLYOBJ_CLASS_ID, 0));

			if (obj != poly) 
				needDel = TRUE;
		}

		if(poly)
		{
			int point_num = poly->mm.VNum();

			for ( i = 0; i < point_num; i++ ) 
			{
				Point3 v = poly->mm.P(i) * tm;
				v -= vInvTransBip01;	// Bip01 �� �̵����� ���� ��Ų��.
				v = v * tmRemainBip01;
				mesh_node->m_MaxAniBoundingBox.Add( MVector3(v.x, v.y, v.z) );
			}

			if (needDel)
			{
				delete poly;
			}
		}
		else if( m_pDummyLocNode != node )	// Dummy_loc �� ��� Bip01 �� ������ ���� �ʱ� ������ ���� ��Ų��.
		{
			// ���� ���� ���ٸ� ��ġ���� ���
			Point3 v = tm.GetTrans();
			v -= vInvTransBip01;
			v = v * tmRemainBip01;
			mesh_node->m_MaxAniBoundingBox.Add( MVector3(v.x, v.y, v.z) );
		}
	}
}

void MCplug2::DumpVertexAni(INode* node, RMaxMeshNode* mesh_node)
{
	if( mesh_node->m_PointCnt==0 )//�������� ���ٸ�...
		return;

	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end	= ip->GetAnimRange().End();
	TimeValue t;

	int delta = GetTicksPerFrame() * GetKeyFrameStep();

	Matrix3 tm(1);

	int cnt = 0;

	for (t=start; t<=end; t+=delta) {
		cnt++;
	}

	mesh_node->m_VertexAniCnt = cnt;
	mesh_node->m_VertexAniFrame = new DWORD[cnt];
	mesh_node->m_VertexAniTable = new D3DXVECTOR3*[cnt];

	//////////////////////////////////////////////////////

	BOOL needDel = FALSE;;
//	Mesh* mesh;
//	int vcnt;
	int frame_cnt = 0;
	int i;

	Point3 v;

	D3DXVECTOR3* pVert = NULL;
	TriObject* tri = NULL;

	for (t=start; t<=end; t+=delta) {

		tm = node->GetObjTMAfterWSM(t);

		PolyObject *poly = NULL;
		Object *obj = node->EvalWorldState(t).obj;
		if (obj->CanConvertToType(Class_ID(POLYOBJ_CLASS_ID, 0))) { 
			poly = (PolyObject *) obj->ConvertToType(t, 
				Class_ID(POLYOBJ_CLASS_ID, 0));

			if (obj != poly) needDel = TRUE;
		}
		if(!poly) return;

		int point_num = poly->mm.VNum();
		int face_num  = poly->mm.FNum();

		pVert = new D3DXVECTOR3[point_num];

		mesh_node->m_VertexAniTable[frame_cnt] = pVert;
		mesh_node->m_VertexAniFrame[frame_cnt] = t-m_nBeginFrame;

		for (i=0; i<point_num; i++) {

			Point3 v = poly->mm.P(i) * tm;

			pVert[i].x = v.x;
			pVert[i].y = v.y;
			pVert[i].z = v.z;
		}
		if (needDel) {
			delete poly;
		}

		frame_cnt++;
	}
}

BOOL MCplug2::CheckForAnimation(INode* node, BOOL& bPos, BOOL& bRot, BOOL& bScale)
{
	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end = ip->GetAnimRange().End();
	TimeValue t;
	int delta = GetTicksPerFrame();
	Matrix3 tm;
	AffineParts ap;
	Point3 firstPos;
	float rotAngle, firstRotAngle;
	Point3 rotAxis, firstRotAxis;
	Point3 firstScaleFactor;

	bPos = bRot = bScale = FALSE;

	for (t=start; t<=end; t+=delta) {

		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));

		decomp_affine(tm, &ap);

		AngAxisFromQ(ap.q, &rotAngle, rotAxis);

		if (t != start) {

			if (!bPos) {

				if (!EqualPoint3(ap.t, firstPos)) 						bPos = TRUE;
			}

			if (!bRot) {
				if (fabs(rotAngle - firstRotAngle) > _ALMOST_ZERO) 		bRot = TRUE;
				else if (!EqualPoint3(rotAxis, firstRotAxis)) 			bRot = TRUE;
			}

			if (!bScale) {
				if (!EqualPoint3(ap.k, firstScaleFactor)) 				bScale = TRUE;
			}

		} else {

			firstPos = ap.t;
			firstRotAngle = rotAngle;
			firstRotAxis = rotAxis;
			firstScaleFactor = ap.k;
		}

		if (bPos && bRot && bScale)		break;
	}

	return bPos || bRot || bScale;
}

void MCplug2::GetRefinedTransform_loc_dir(INode* node, TimeValue t, Matrix3& tmDummyRefined, Matrix3& tmDummyRemains)
{
	//////////////////////////////////////////////////////////////////////////
	// node(bip01) �� �ִϸ��̼� ��
	// (���̸� ������)�̵� �� (z-up ���ĵ�) ȸ�� ������ tmDummyRefined �� 
	// ������ ������ tmDummyRemains (bip01 �� ������ toplevel��� ) �� �и��Ѵ�.

	// tmBip01   : Bip01 �� ���� tm
	// tmbip01_0 : Bip01 �� ù������ tm
	// tmDummy	: ù�����ӿ� ���� ( WORLD ���� ) ������� �ִϸ��̼�
	//
	// �Ϲ����� tm      : tmBip01 * WVP
	
	// ���ϴ� ����

	// 1. tmDummy �� ���Ѵ�
	// tmDummy			= Inv( tmBip01_0_r ) * tmBip01 * Inv(tmBip01_0_t)

	// 2. tmDummyRefined �� ���Ѵ�
	// tmDummy �� ���� ��ó�� �Ͽ� tmDummyRefined �� ��´�

	// 3. tmDummyRemains�� ���Ѵ�
	// tmDummyRefined ������ ���������� tmBip01 �� ���� �����Ѵ�


	// static frame �� tm
	Matrix3 tmBip01_0 = node->GetNodeTM( GetStaticFrame() );

	// static frame �� ȸ��
	Matrix3 tmBip01_0_r = tmBip01_0;
	tmBip01_0_r.NoTrans();

	// static frame �� translation
	Matrix3 tmBip01_0_t;
	tmBip01_0_t.IdentityMatrix();
	tmBip01_0_t.SetTrans( tmBip01_0.GetTrans() );

	Matrix3 tmBip01 = node->GetNodeTM( t );

	// 1. tmDummy �� ���Ѵ�
	Matrix3 tmDummy = Inverse( tmBip01_0_r ) * tmBip01 * Inverse(tmBip01_0_t);

	// 2. tmDummyRefined �� ���Ѵ�

	// tmDummy ���� ���� z ����/������ ���� �����Ͽ� tmDummyRefined �� ����

	// �� �Ʒ� �������� �����Ÿ� ���� ���ش�
	Point3 vecDir = tmDummy.GetRow(1);		// rs3 ���� y �� �������� ���δ�.
	vecDir.z = 0;
	vecDir = Normalize(vecDir);

	Point3 vecUp(0,0,1);
	Point3 vecRight = Normalize( CrossProd( vecDir, vecUp ) );
	vecUp = CrossProd(vecRight,vecDir);

	tmDummyRefined;
	tmDummyRefined.IdentityMatrix();
	tmDummyRefined.SetRow( 0, vecRight );
	tmDummyRefined.SetRow( 1, vecDir );
	tmDummyRefined.SetRow( 2, vecUp );

	// z�̵� (����) ���� ����
	Point3 vecPos = tmDummy.GetTrans();
	vecPos.z = 0;
	tmDummyRefined.SetTrans( vecPos );
	//*/

	// 3. tmDummyRemains�� ���Ѵ�

	// dummy ������ ���� ���̿��� ���е��� ���� tm �� �ִ´�
	
	// tmDummyRefined �� rotation ���а� translation �������� ����
	Matrix3 tmRefined_r = tmDummyRefined;
	tmRefined_r.NoTrans();

	Matrix3 tmRefined_t;
	tmRefined_t.IdentityMatrix();
	tmRefined_t.SetTrans( tmDummyRefined.GetTrans() );

	Matrix3 tmBip01_t;
	tmBip01_t.IdentityMatrix();
	tmBip01_t.SetTrans( tmBip01.GetTrans() );

	tmDummyRemains = tmBip01 * Inverse(tmBip01_t) * Inverse(tmRefined_r) * tmBip01_t * Inverse( tmRefined_t ) ;
	//                         -----------------------------------------------------   -----------------------
	//                            ȸ������ �������� (�̵����л��� ȸ���� �ٽ��̵�)        �̵����� ��������
}

void MCplug2::GetRefinedTransform_loc(INode* node, TimeValue t, Matrix3& tmDummyRefined, Matrix3& tmDummyRemains)
{
	//////////////////////////////////////////////////////////////////////////
	// node(bip01) �� �ִϸ��̼� �� �̵����� dummy_loc �� �Ű��ش�

	// static frame �� tm
	Matrix3 tmBip01_0 = node->GetNodeTM( GetStaticFrame() );

	// static frame �� ȸ��
	Matrix3 tmBip01_0_r = tmBip01_0;
	tmBip01_0_r.NoTrans();

	// static frame �� translation
	Matrix3 tmBip01_0_t;
	tmBip01_0_t.IdentityMatrix();
	tmBip01_0_t.SetTrans( tmBip01_0.GetTrans() );

	Matrix3 tmBip01 = node->GetNodeTM( t );

	// 1. tmDummy �� ���Ѵ�
	Matrix3 tmDummy = Inverse( tmBip01_0_r ) * tmBip01 * Inverse(tmBip01_0_t);

	// 2. tmDummyRefined �� ���Ѵ�

	tmDummyRefined;
	tmDummyRefined.IdentityMatrix();

	// z�̵� (����) ���� ����
	Point3 vecPos = tmDummy.GetTrans();
	vecPos.z = 0;
	tmDummyRefined.SetTrans( vecPos );
	//*/

	// 3. tmDummyRemains�� ���Ѵ�

	// dummy ������ ���� ���̿��� ���е��� ���� tm �� �ִ´�

	tmDummyRemains = tmBip01 * Inverse( tmDummyRefined ) ;
}

void MCplug2::DumpSRTSample(INode* node, RMaxMeshNode* pMeshNode, BOOL pos, BOOL rot, BOOL scale)
{
	// dummy_loc_dir �̸��� ��������̸� ������ �̸��� dummy_loc �� �����ش�
	if( node==m_pDummyLocNode && (m_eDummyLocType != DUMMY_LOC_NONE) )
		pMeshNode->m_Name = "dummy_loc";

	pMeshNode->m_isHavePosAni = (pos!=FALSE);
	pMeshNode->m_isHaveRotAni = (rot!=FALSE);

	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end	= ip->GetAnimRange().End();
	TimeValue t;

	int delta = GetTicksPerFrame() * GetKeyFrameStep() ;

	for (t=start; t<=end; ) 
	{
		Matrix3 tm,ptm;
		ptm = node->GetParentTM(t);
		tm = node->GetNodeTM(t);
		tm = tm * Inverse(ptm);
		if(TMNegParity( tm ))	// mirror �Ǿ��ִ°�� mesh �� ���������.
		{
			static Matrix3 Negative = ScaleMatrix(Point3 ( -1.0f, -1.0f, -1.0f ));
			tm = Negative * tm; 
		}

		if( (m_eDummyLocType == DUMMY_LOC || m_eDummyLocType == DUMMY_LOC_DIR) &&
			( node==m_pBip01Node || node==m_pDummyLocNode ) )
		{

			Matrix3 tmDummyRefined;
			Matrix3 tmDummyRemains;
			if(m_eDummyLocType == DUMMY_LOC_DIR)
				GetRefinedTransform_loc_dir( m_pBip01Node, t, tmDummyRefined, tmDummyRemains );
			else
				GetRefinedTransform_loc( m_pBip01Node, t, tmDummyRefined, tmDummyRemains );

			// bip01 �� ����
			if( node== m_pBip01Node )
				tm = tmDummyRemains * Inverse( node->GetParentTM(t) );
			else
			{
				// dummy_loc�� ���ؼ�...
				_ASSERT( m_pDummyLocNode == node );
				tm = node->GetNodeTM(t) * tmDummyRefined * Inverse( node->GetParentTM(t) );
			}
		}

		pMeshNode->AddKeys(tm, t-m_nBeginFrame, pos, rot, scale );

		// ���ø� ������ �ٸ��� ������ ���� ó��
		if(m_bCustomSampling && m_nCustomSamplingStart<=t && t<m_nCustomSamplingEnd)
			t+= m_nCustomSamplingInterval;
		else
			t+=delta;
	}
}

void MCplug2::DumpVisSample(INode* node, RMaxMeshNode* mesh_node)
{
	Control* cont = node->GetVisController();

//	RVisKey* pVis = NULL;

//	float fVis = node->GetVisibility();
	//���۰� ���� �ϳ��� �� �־��ش�
	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end	= ip->GetAnimRange().End();

	if(cont) {

		int i;
		IKeyControl *ikc = NULL;
		ikc = GetKeyControlInterface(cont);

		if(ikc)	{

			//����
			MCVisKey viskey;
			viskey.frame = start - m_nBeginFrame;
			viskey.v = node->GetVisibility(start);
			// 3DMax���� ���������� Visibility���� 0���� ������ Off�� ����ϱ� ������ ���� 0���� ���� �� �ִ�.
			if( viskey.v < 0.0f )
				viskey.v = 0.0f;

			mesh_node->m_visKey.push_back(viskey);

			int numKey = ikc->GetNumKeys();

			for (i=0; i<numKey; i++) {
				if(cont->ClassID() == Class_ID(TCBINTERP_FLOAT_CLASS_ID, 0)) {
					ITCBFloatKey key;
					ikc->GetKey(i, &key);
//					pVis->frame = key.time / GetTicksPerFrame();
					viskey.frame = key.time - m_nBeginFrame;
					viskey.v		= key.val;
				}
				else if(cont->ClassID() == Class_ID(HYBRIDINTERP_FLOAT_CLASS_ID, 0))
				{
					IBezFloatKey key;
					ikc->GetKey(i, &key);
					viskey.frame = key.time - m_nBeginFrame;
					viskey.v		= key.val;
				}
				else if(cont->ClassID() == Class_ID(LININTERP_FLOAT_CLASS_ID, 0)) {
					ILinFloatKey key;
					ikc->GetKey(i, &key);

					// �ҿ���Ʈ �ͽ���Ʈ ���
					// ��Ʈ�ѷ� Ÿ���� Linear Float�̸� �������� �ʵ��� Ű�� �۰� �ִ´�.
					if( numKey > 1 && i > 0 && i < numKey )
					{
						ILinFloatKey prevKey;
						ikc->GetKey(i-1, &prevKey);

							// ���� Ű�� ���� �������̸� ��ŵ
						if( prevKey.time != m_nBeginFrame &&
							// Ű���� ������ ��ŵ
							key.val != prevKey.val )
						{
							// ������ ������ 1���̸�ŭ�� ������ ���� �۰�
							viskey.frame = prevKey.time - m_nBeginFrame + 1;
							viskey.v = key.val;
						}
						else
						{
							viskey.frame = key.time - m_nBeginFrame;
							viskey.v = key.val;
						}
					}
					else
					{
						viskey.frame = key.time - m_nBeginFrame;
						viskey.v = key.val;
					}
				}

				// VisibilityŰ���� 0 ~ 1 ������ ����� �ȵȴ�.
				if( viskey.v < 0.0f )
				{
					viskey.v = 0.0f;
					mlog3( "%s %d�������� VisilibityŰ���� 0���� �۽��ϴ�. ", mesh_node->m_Name.c_str(), viskey.frame );
				}
				else
				if( viskey.v > 1.0f )
				{
					viskey.v = 1.0f;
					mlog3( "%s %d�������� VisilibityŰ���� 1���� Ů�ϴ�. ", mesh_node->m_Name.c_str(), viskey.frame );
				}

				mesh_node->m_visKey.push_back(viskey);
			}
			//��
			viskey.frame = end - m_nBeginFrame;
			viskey.v = node->GetVisibility(end);
			// 3DMax���� ���������� Visibility���� 0���� ������ Off�� ����ϱ� ������ ���� 0���� ���� �� �ִ�.
			if( viskey.v < 0.0f )
				viskey.v = 0.0f;

			mesh_node->m_visKey.push_back(viskey);
		}
	}
}
