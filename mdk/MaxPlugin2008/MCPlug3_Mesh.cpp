#include "stdafx.h"

#include "MCplug3.h"
#include "PHYEXP.H"

#include "modstack.h"
#include "iparamm2.h"
#include "iskin.h"
#include "meshadj.h"

#include "RMaxMaterial.h"
#include "RMaterial.h"
#include "MDebug.h"

#include "IGame.h"

//////////////////////////////////////////////////////////////
// object export

Modifier* FindPhysiqueModifier (INode* nodePtr)
{
	Object* ObjectPtr = nodePtr->GetObjectRef();

	if (!ObjectPtr) return NULL;

	if (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID) {

		IDerivedObject* DerivedObjectPtr = static_cast<IDerivedObject*>(ObjectPtr);

		int ModStackIndex = 0;

		while (ModStackIndex < DerivedObjectPtr->NumModifiers()) {

			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);

			if (ModifierPtr->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B)) {

				return ModifierPtr;
			}

			ModStackIndex++;
		}
	}

	return NULL;
}

Modifier* FindSkinModifier(INode* nodePtr)
{
	Object* ObjectPtr = nodePtr->GetObjectRef();

	if (!ObjectPtr) return NULL;

	while (ObjectPtr && ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject *DerivedObjectPtr = (IDerivedObject *)(ObjectPtr);

		int ModStackIndex = 0;

		while (ModStackIndex < DerivedObjectPtr->NumModifiers())
		{
			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);

			if (ModifierPtr->ClassID() == SKIN_CLASSID)
			{
				return ModifierPtr;
			}
			ModStackIndex++;
		}

		ObjectPtr = DerivedObjectPtr->GetObjRef();
	}
	return NULL;
}

bool MCplug2::ExportSkin(INode* node,RMaxMeshNode* pMeshNode)
{
	Modifier *phyMod = FindSkinModifier(node);

    if (!phyMod) return false;    

	ISkin *pSkin = (ISkin*)phyMod->GetInterface(I_SKIN);

	if(!pSkin)	return false;

	ISkinContextData *pSkinMC = pSkin->GetContextInterface(node);

	int i, j;
	
	int numBone = pSkin->GetNumBones();

	//for (i = 0; i < numBone; i++) 
	//{ 
	//	INode *bone = pSkin->GetBone(i); 

	//	if (bone) {
	//		string Name = bone->GetName();
	//	}
	//} 

	int numVerts = pSkinMC->GetNumPoints();

	pMeshNode->m_PhysiqueCnt = numVerts;
	pMeshNode->m_PhysiqueTable = new RPhysiqueInfo [numVerts];
	memset(pMeshNode->m_PhysiqueTable,0,sizeof(RPhysiqueInfo)*numVerts);

	RPhysiqueInfo* pPData = NULL;

	int nVertBoneCnt = 0;

	string Name;
	float fWeight;

	for (i = 0; i<numVerts; i++) { 

		pPData = &pMeshNode->m_PhysiqueTable[i];

		nVertBoneCnt = pSkinMC->GetNumAssignedBones(i);

		pPData->Create(nVertBoneCnt);

		for(j = 0; j < nVertBoneCnt; j++) {

			int x = pSkinMC->GetAssignedBone(i, j);

			INode *Bone = pSkin->GetBone(x);			
			Name	= Bone->GetName();					// ����� �����ϰ� �ϱ�����.

			fWeight = pSkinMC->GetBoneWeight(i, j);

			pPData->SetName(j,Name);
			pPData->SetWeight(j,fWeight);
		}
	}

	phyMod->ReleaseInterface(I_SKIN, pSkin);

	return true;
}


bool MCplug2::ExportPhysique( ObjectState *os, INode *node,RMaxMeshNode* pMeshNode)
{
    Modifier *phyMod = FindPhysiqueModifier(node);

    if (!phyMod) return false;    

    IPhysiqueExport *phyExport = (IPhysiqueExport *)phyMod->GetInterface(I_PHYINTERFACE);

	// Remove the non uniform scale

//	IBipedExport *BipIface = (IBipedExport *) c->GetInterface(I_BIPINTERFACE);
//	BipIface->RemoveNonUniformScale(1);

    if (phyExport) {

		IPhyContextExport *mcExport = (IPhyContextExport *)phyExport->GetContextInterface(node);

		if (mcExport) {

			mcExport->ConvertToRigid(TRUE);
			mcExport->AllowBlending(TRUE);		//<--

			int p_num = os->obj->NumPoints();

			pMeshNode->m_PhysiqueCnt = p_num;
			pMeshNode->m_PhysiqueTable = new RPhysiqueInfo [p_num];
			memset(pMeshNode->m_PhysiqueTable,0,sizeof(RPhysiqueInfo)*p_num);

			RPhysiqueInfo* pPData = NULL;

			phyMod->DisableMod();
			phyExport->SetInitialPose( true );
//			phyExport->GetInitNodeTM( this, MeshRefMatrix );

			for (int i = 0; i < p_num;  i++) {

				IPhyVertexExport *vtxExport = mcExport->GetVertexInterface(i);

				if (vtxExport) {

					pPData = &pMeshNode->m_PhysiqueTable[i];

					if (vtxExport->GetVertexType() & BLENDED_TYPE) {

						IPhyBlendedRigidVertex *vtxBlend = (IPhyBlendedRigidVertex *)vtxExport;
			
						int no = vtxBlend->GetNumberNodes();

						pPData->Create(no);

						Point3 BlendP(0.0f, 0.0f, 0.0f);

						for (int n = 0; n < no; n++) {

							INode *Bone		= vtxBlend->GetNode(n);
//							Point3 Offset	= vtxBlend->GetOffsetVector(n);
							float Weight	= vtxBlend->GetWeight(n);

//							pPData->SetOffset(n,D3DXVECTOR3(Offset.x,Offset.z,Offset.y));
							pPData->SetName(n,string(Bone->GetName()));
							pPData->SetWeight(n,Weight);
						}
		
					} else {

						IPhyRigidVertex *vtxNoBlend = (IPhyRigidVertex *)vtxExport;
						INode *Bone = vtxNoBlend->GetNode();
//						Point3 Offset = vtxNoBlend->GetOffsetVector();

						pMeshNode->m_PhysiqueTable[i].Create(1);

//						pPData->SetOffset(0,D3DXVECTOR3(Offset.x,Offset.z,Offset.y));
						pPData->SetName(0,string(Bone->GetName()));
						pPData->SetWeight(0,1.f);

					}

					mcExport->ReleaseVertexInterface(vtxExport);
					vtxExport = NULL;	

				}
			}

			phyExport->SetInitialPose( false );
			phyMod->EnableMod();

			phyExport->ReleaseContextInterface(mcExport);
		}
        phyMod->ReleaseInterface(I_PHYINTERFACE, phyExport);
    }

	return true;
}

// ���̿� �Ϲ� ������Ʈ ���� ���.
void MCplug2::ExportGeometryObject(INode* node)
{
	ObjectState os = node->EvalWorldState(GetStaticFrame());

	if (!os.obj)	return;
	
	if (os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))	return;

	string szNodeName = FixupName( node->GetName() );

	// ground_ �� pathengine���̹Ƿ� export ��󿡼� ����
	const char *prefixGround = "ground_";
	if (strstr(szNodeName.c_str(), prefixGround)) return;

	// physx_ �� PhysX Scene Export���̹Ƿ� export ��󿡼� ����
	const char *prefixPhysX = "physx_";
	if (strstr(szNodeName.c_str(), prefixPhysX)) return;

	// external_�� shape ���� �뵵�̹Ƿ� export ��󿡼� ����
	const char *prefixExternal = "ex_";

	RMaxMeshNode* pMeshNode = new RMaxMeshNode;

	pMeshNode->m_Name = szNodeName;

	mlog("%s\n",pMeshNode->m_Name.c_str());

	pMeshNode->CalcName2Type(node);

	INode* parent = node->GetParentNode();

	if (parent && !parent->IsRootNode()) {
		pMeshNode->m_ParentName = FixupName( parent->GetName() );
	}

	DWORD iFlags = node->GetTMController()->GetInheritanceFlags();
	Matrix3 pivot = node->GetNodeTM(GetStaticFrame());

	if(TMNegParity(pivot))
		pivot = matNegative * pivot; 

	ConvertToD3DXMatrix(pMeshNode->m_matBase, pivot );

	// local matrix ����
	Matrix3 matParent = node->GetParentTM( GetStaticFrame());
	if(TMNegParity(matParent))
		matParent = matNegative * matParent; 

	pivot = pivot	* Inverse( matParent );
	ConvertToD3DXMatrix(pMeshNode->m_matLocal, pivot );

	// base visiblity
	pMeshNode->m_fBaseVisibility = node->GetVisibility(GetStaticFrame());

	const char *pcNodeName = pMeshNode->m_Name.c_str();

	if ( strstr(szNodeName.c_str(), prefixExternal))
	{
		ExportMesh(node, GetStaticFrame(), pMeshNode);
		m_meshList.m_ObstacleMeshList.push_back(pMeshNode);
		return;
	}

	if( pMeshNode->CheckFlag(RM_FLAG_COLLISION_MESHONLY) ||			// Gunz2 ���� ���浹�뵵�� ���Ǵ� ĸ��
		pMeshNode->CheckFlag(RM_FLAG_COLLISION_CLOTH_MESH) )	// �浹�� �޽��̴�
	{
		ExportCollision(node, pMeshNode);
		delete pMeshNode;
		return;
	}

	if( pMeshNode->CheckFlag(RM_FLAG_OCCLUDER) )
	{
		ExportOccluder(node, pMeshNode);
		delete pMeshNode;
		return;
	}

	// �����ϴ� ������� üũ
	bool bIgnore = false;
	GetUserPropertyString( node, "ignore_at_export", &bIgnore );

	if( bIgnore == false )
		GetUserPropertyString( node, "set_export_ignore", &bIgnore );

	if( bIgnore ) return;

	// �� �޽�
	bool bWaterMesh = false;
	GetUserPropertyString( node, "set_water", &bWaterMesh );
	if( bWaterMesh )
	{
		m_waterList.push_back( node );
	}

	// static mesh �Ǵ� elu mesh
	bool bExportPhysique = false;
	if( !pMeshNode->CheckFlag(RM_FLAG_DUMMY_MESH) &&
		!pMeshNode->CheckFlag(RM_FLAG_BONE_MESH) ) // ���̶Ǵ� �� ���� ������ ä�� �ʿ䰡 ����..
	{
		if(m_options.m_bStaticMesh)
		{
			bool bStatic = false;
			GetUserPropertyString( node, "set_static", &bStatic );

			// Static Mesh ����϶� Static �Ӽ����� ������ �޽��� ����Ѵ�.
			if (bStatic)
				ExportStaticMesh( node, GetStaticFrame() );	
		}
		else
		{
			// �ִϸ��̼Ǹ� �ͽ���Ʈ�ϴ� ��쿡�� AABB�� ����ϱ� ���� �޽� �ͽ���Ʈ �ؾ� �Ѵ�.
			if( m_options.m_bMeshOut || m_options.m_bKeyAnimationOut || m_options.m_bVertexAnimationOut )
			{
				// XRef�� �ƴ� Mesh�� ����Ѵ�
				Object *obj = node->GetObjectRef();
				if ( obj->ClassID()!=XREFOBJ_CLASS_ID ) 
				{
					ExportMesh(node, GetStaticFrame(), pMeshNode);
					bExportPhysique = true;
				}
			}
		}
	}

	ExportEffect( node, pMeshNode );

	Mtl* mtl = node->GetMtl();
	// ���̹� ���� ������Ʈ�� ������ �ʿ��Ѱ� ?
	pMeshNode->m_pMtrl = m_meshList.m_pMaterialKeeper->GetMaterial(mtl);

	// Ư��ȿ�� ���̴� ������Ʈ �÷��� ������ ����� ���׸��� ����.
	pMeshNode->m_pMtrl->GetRMaterial()->AddShaderComponent(pMeshNode->m_rscShaderComponentFlag);
	
	if(bExportPhysique)
	{
		if(ExportPhysique(&os,node,pMeshNode)==false)	// physique �� ������� skin �� �õ��� ����.
			ExportSkin(node,pMeshNode);

		if(pMeshNode->m_PhysiqueCnt!=0 && (pMeshNode->m_PointCnt!=pMeshNode->m_PhysiqueCnt))
		{
			mlog3("%s ������Ʈ�� (%d) ����� ����ũ ���ؽ� ������ ����������Ʈ�� ���ؽ� ����(%d) �� �ٸ��ϴ�. ������ �Ǵ� ���ؽ��� ã�Ƽ� �������ּ���", 
				pMeshNode->m_Name.c_str(), pMeshNode->m_PhysiqueCnt, pMeshNode->m_PointCnt );
		}
	}

	if( m_options.m_bKeyAnimationOut || m_options.m_bVertexAnimationOut )	
	{
		// �ִϸ��̼� �ͽ���Ʈ �� physique ���� �����ϴºκ��� �־ ���� �ڿ� �Ѵ�
		ExportAnimation(node, pMeshNode);
	}

	m_meshList.Add(pMeshNode);
}

//////////////////////////////////////////////////////////////////////////////
// ��������� convex �̸� true
bool MCplug2::CheckCoplanarAndConvex(MNMesh& mm, int iFace)	
{

#define MVECTOR(x) MVector3((float*)mm.P(x))

	// fnormal�� �������� �ϰ� vertex 0�� ������ ����� �������� �����
	Point3 center;
	Point3 fnormal;
	mm.ComputeNormal(iFace,fnormal,&center);

	MNFace *f=mm.F(iFace);
	MPlane plane(MVector3((float*)fnormal),MVector3((float*)mm.P(f->vtx[0])));

	// ��������� ��� �� ��� ���� �ְ�, ���� �ٰ����̸� �ɰ� �ʿ䰡����
	bool bCoplanar=true;
	bool bConvex=true;

	int nSign=0;

	for(int j=0;j<f->deg;j++)
	{
		MVector3 normal;
		MVector3 edgea,edgeb;

#define COPLANAR_TOLER	1.f
		MVector3 testpoint = MVECTOR(f->vtx[j]);

		if(fabs(plane.GetDistanceTo(testpoint))>COPLANAR_TOLER)		// ���� ������� �ִ��� Ȯ��
		{
			bCoplanar=false;
			break;
		}

		edgea=MVECTOR(f->vtx[j]);
		edgeb=MVECTOR(f->vtx[(j+1)%f->deg]);
		normal=CrossProduct(MVector3((float*)fnormal),edgeb-edgea);
		normal.Normalize();

		MPlane testplane(normal,edgea);

		// ������ edge �� ���ؼ� ����� ����� �� ����� + �ʿ� ���� ���ؽ��� �ִ��� Ȯ���Ѵ�
#define CONVEX_TOLER	1.f

		double ftestv = testplane.GetDistanceTo(MVECTOR(f->vtx[(j+2)%f->deg]));
#define CONVEX_SIGN(x) ((x)<-CONVEX_TOLER ? -1 : (x)>CONVEX_TOLER ? 1 : 0)

		if(nSign==0)
			nSign=CONVEX_SIGN(ftestv);
		else
		{
			int sign=CONVEX_SIGN(ftestv);
			if(nSign!=sign && 0!=sign)
			{
				bConvex=false;
				break;
			}
		}
	}
	return bCoplanar && bConvex;
}

void MCplug2::ExportMesh(INode* node, TimeValue t, RMaxMeshNode* pMeshNode)
{
	// Normal Data ��� �ÿ� Ascii ������δ� Normal �� ����� ������ �ʴ� ���װ� �־
	// IGameMesh �� ��ȯ ���Ѽ� Normal Data �� �����
	IGameNode* pGameNode = m_pIGame->GetIGameNode(node);
	if(!pGameNode)
	{
		MessageBox(NULL, "GetIGameNode() ����", "ExportMesh", MB_OK);
		return;
	}

	// Skin Mesh �ε� �����ϴ� ���� ������ ������� �ʴ´�.
	Modifier *phyMod = FindSkinModifier(node);
	if (phyMod)
	{
		ISkin *pSkin = (ISkin*)phyMod->GetInterface(I_SKIN);
		if(pSkin)
		{
			int numBone = pSkin->GetNumBones();
			if( numBone <= 0 )
			{
				char szErrorMessage[MAX_PATH];
				sprintf(szErrorMessage, "<%s> �޽��� Skin Modify �̸鼭 �����ϴ� ���� 0�� �Դϴ�.", pMeshNode->m_Name.c_str());
				MessageBox(NULL, szErrorMessage, "Mesh Export Failed!!", MB_OK);
				return;
			}
		}
	}

	//string strGameNodeName = pGameNode->GetName();
	IGameObject* obj = pGameNode->GetIGameObject();
	IGameMesh* gmesh = (IGameMesh*)obj;
	bool bGMeshInitializeSuccess = gmesh->InitializeData();
	// ������ �ϴ� �ּ�ó��
	if(bGMeshInitializeSuccess)
		gmesh->SetCreateOptimizedNormalList();


	Mtl* nodeMtl	= node->GetMtl();
	Matrix3 tm		= node->GetObjTMAfterWSM(t);
	BOOL negScale	= TMNegParity(tm);
	Matrix3 pivot = tm;
	pivot.NoTrans();
	Matrix3 nodetm = node->GetNodeTM(0);
	Matrix3 invNodetm = Inverse(nodetm);
	Matrix3 matSign(1);
	if(negScale)
		matSign = matNegative * matSign;

	ObjectState os = node->EvalWorldState(t);
	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID)
		return; 

	BOOL needDel = FALSE;
	TriObject* tri = GetTriObjectFromNode(node, t, needDel);
	if (!tri)
		return;

	Mesh* mesh = &tri->GetMesh();

	mesh->buildNormals();

	int point_num = mesh->getNumVerts();
	int face_num  = mesh->getNumFaces();

	pMeshNode->m_PointCnt = point_num;
	pMeshNode->m_PointTable = new D3DXVECTOR3[point_num];
	memset( pMeshNode->m_PointTable,0,sizeof(D3DXVECTOR3)*point_num );

	if( mesh->vcFace )
	{
		pMeshNode->m_PointColorCnt = point_num;
		pMeshNode->m_PointColorTable = new D3DXVECTOR3[ point_num ];// ������ ���� ����
	}

	// vertex position
	for (int i=0; i<mesh->getNumVerts(); i++)
	{
		Point3 v = tm * mesh->verts[i] * invNodetm * matSign;
		//         -------------------
		//				world vertex
		// ���������� world space modifier�� ����� ���� ��ǥ�� �ȴ�

		pMeshNode->m_PointTable[i].x = v.x;
		pMeshNode->m_PointTable[i].y = v.y;
		pMeshNode->m_PointTable[i].z = v.z;
	}

	// material
	RMaxMaterial *pNodeMaterial = m_meshList.m_pMaterialKeeper->GetMaterial(nodeMtl);
	_ASSERT(NULL!=pNodeMaterial);

	for ( int i = 0; i < face_num; ++i ) 
	{
		FaceEx* face = bGMeshInitializeSuccess ? gmesh->GetFace(i) : NULL;
		Face* f = &mesh->faces[i];
		RMaxFaceData *pFaceData = new RMaxFaceData;

		pFaceData->nDeg = 3;
		pFaceData->index = new RFaceSubData[3];

		// material id
		RMaxMaterial *pMaterial = NULL;
		if(pNodeMaterial)
		{
			pMaterial = pNodeMaterial->GetMaterialByID( mesh->faces[i].getMatID() );
		}

		pFaceData->pMaterial		= pMaterial;
		pFaceData->nSubMaterialID	= mesh->faces[i].getMatID();

		pFaceData->triangleIndices = new unsigned char[3];
		pFaceData->triangleIndices[0] = 0;
		pFaceData->triangleIndices[1] = 1;
		pFaceData->triangleIndices[2] = 2;
		pMeshNode->m_FaceTable.push_back(pFaceData);

		for( int j = 0; j < 3; ++j )
		{
			int ncorner = negScale ? 3 - j -1 : j;	// 2 1 0
			// negative Mesh �϶� IGameMesh �� �ﰢ�� ���� ������ �ٸ���
			// 2 1 0 -> 1 2 0 ���� �ﰢ�� ���� ���� ����
			int ncornernormal = negScale ? (j + 1) % 3 : j; // 1 2 0
			// Position
			pFaceData->index[j].p = (WORD)f->v[ncorner];

			// IGameMesh �� �̿��� Normal �� ����Ѵ�.
			// Normal
			Point3 n;
			if( bGMeshInitializeSuccess )
			{
				n = gmesh->GetNormal(face, ncornernormal, true);
			}
			else
			{
				int vert = f->getVert(ncorner);
				n = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
			}
			Point3 vnSign = VectorTransform( matSign, n );
			pFaceData->index[j].n = pMeshNode->m_NormalTable.Add( (float*)vnSign );

			// uv //////////////////////////////////////////////////////////////////////////

			// DiffuseMap Channel
			pFaceData->index[j].uv  = 0;	// �ʱ�ȭ
			pFaceData->index[j].uv2 = 0;

			// 0: Vertex Color channel.
			// 1: Default mapping channel (the TVert array).
			// 2 through MAX_MESHMAPS-1: The new mapping channels available in release 3.0.
			if ( mesh->mapSupport(0) || mesh->mapSupport(1) ) 
			{
				DWORD	tvert	= mesh->tvFace[i].t[ncorner];
				UVVert	tv		= mesh->tVerts[tvert];
				RVector uv;
				uv.x = tv.x;
				uv.y = 1.f - tv.y;
				uv.z = 0;

				pFaceData->index[j].uv = pMeshNode->m_TexCoordTable.Add( uv );
			}

			if ( mesh->mapSupport(2) )
			{
				DWORD	tvert	= mesh->mapFaces(2)[i].t[ncorner];
				UVVert	tv		= mesh->mapVerts(2)[tvert];
				RVector uv;
				uv.x = tv.x;
				uv.y = 1.f - tv.y;
				uv.z = 0;

				pFaceData->index[j].uv2 = pMeshNode->m_TexCoordTableExtra.Add( uv );
			}

			// vcolor
			if(mesh->vcFace)
			{
				int VertexColorIndex = -1;
				VertexColorIndex = mesh->vcFace[i].t[j];

				pMeshNode->m_PointColorTable[pFaceData->index[j].p] = (float*)mesh->vertCol[ VertexColorIndex ];
			}			
		}
	}

	pMeshNode->m_FaceCnt  = pMeshNode->m_FaceTable.size();

	// vertex color
	if( mesh->vcFace )
	{
		// check ��� 1�� �÷����̸� ��� ���Ѵ�..
		// �����̳ʰ� �÷����� ��� ����ٰ� ������� �������� ��쵵 
		// max ������ �������� �ʾƼ� �ٽ� �۾��ؾ� �Ҽ��� �ִ�..
		D3DXVECTOR3* pColor = NULL;
		bool bCheck = true;
		for( int i = 0; i < point_num; ++i )
		{
			pColor = &pMeshNode->m_PointColorTable[i];
			if( (pColor->x != 1.f) || (pColor->y != 1.f) ||	(pColor->z != 1.f)) {
				bCheck = false;
				break;
			}
		}

		if(bCheck)	//��� ����� ���� ����� �ʿ����..
		{
			pMeshNode->m_PointColorCnt = 0;
			delete [] pMeshNode->m_PointColorTable;
			pMeshNode->m_PointColorTable = NULL;
		}
	}

	pGameNode->ReleaseIGameObject();
}

TriObject* MCplug2::GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;
	if( obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)) )
	{
		TriObject *tri = (TriObject*)obj->ConvertToType( t, Class_ID(TRIOBJ_CLASS_ID, 0) );

		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri)
			deleteIt = TRUE;

		return tri;
	}
	else
	{
		return NULL;
	}
}

// mesh �� ������ set �� �ϳ��� ���������� �����. �����ϸ� null �� ����
rpolygon* MCplug2::MergePolygon( vector<rpolygon*>& triangles )
{
	vector<rvertex> vecVertices;

	RMaxMaterial*	pMaterial = NULL;
	int				nLightMapID = -1;
	DWORD			dwFlags = 0;

	int nCount = 0;
	for (size_t i=0; i<triangles.size(); i++) 
	{
		rpolygon* pPolygon = triangles[i];
		if(!pPolygon) return NULL;	// �̹� �������� ������ �������̴�
		
		// ù �������ϰ��
		if( nCount == 0 )
		{
			for(int j=0;j<3;j++)
				vecVertices.push_back( pPolygon->v[j] );

			pMaterial = pPolygon->pMaterial;
			nLightMapID = pPolygon->nLightMapID;
			dwFlags = pPolygon->dwFlags;
		}else
		{
			if(pMaterial!=pPolygon->pMaterial) return NULL;
			if(dwFlags!=pPolygon->dwFlags) return NULL;

			// �´� edge �� ã�´�
			int nEdge = -1;
			int nVertexToAdd = -1;	// �����ϴ� ���ؽ��� �ƴ� ������ �ϳ��� ���ؽ�
			for(size_t j=0; j< vecVertices.size(); j++)
			{
				// ����� �������� edge = ea, eb
				rvertex* ea = &vecVertices[j];
				rvertex* eb = &vecVertices[(j+1) % vecVertices.size() ];

				for( int k=0; k<3; k++)
				{
					// �߰��� �ﰢ���� edge = ec, ed
					rvertex* ec = &pPolygon->v[k];
					rvertex* ed = &pPolygon->v[(k+1) % 3 ];

					// �´� edge �� ã�ƺ���
					if(*ea == *ed && *eb == *ec)
					{
						// convex�� �Ǵ��� �˻�

						// edge�� �̷�� �� ���ؽ��ƴ� ���� �ϳ��� vertex
						rvertex* remain = &pPolygon->v[(k+2) % 3 ];

						// ����� �ִ� polygon ���� edge �� �̿� edge �� ���� ��鿡 ���� �׽�Ʈ

						// edge-a�� ���� edge �� ������ �����￡ �������� �����ϴ� ��鿡 ���� �׽�Ʈ 
						MVector3 ea1 = vecVertices[(j-1 + vecVertices.size() ) % vecVertices.size() ].coord;
						MVector3 normal = CrossProduct(ea->coord - ea1, eb->coord - ea->coord );
						normal.Normalize();	// �������� normal

						MVector3 planeNormal = CrossProduct( normal, ea->coord - ea1 );
						planeNormal.Normalize();

						MPlane testplaneA(planeNormal,ea->coord);

						// edge-b �� ���� edge �� ������ �����￡ �������� �����ϴ� ��� ���� �׽�Ʈ
						MVector3 eb1 = vecVertices[(j+2) % vecVertices.size() ].coord;

						MVector3 planeBNormal = CrossProduct( normal, eb1 - eb->coord );
						planeBNormal.Normalize();

						MPlane testplaneB(planeBNormal,eb->coord);

						float fDistTestPlaneA = testplaneA.GetDistanceTo(remain->coord);
						float fDistTestPlaneB = testplaneB.GetDistanceTo(remain->coord);
						// convex �� �����Ҽ� �ִ�
						if( CONVEX_SIGN(fDistTestPlaneA) == CONVEX_SIGN(fDistTestPlaneB) )
						{
							nEdge = j+1;	// ���� ���� ��ġ, �Ǿտ� ������ �� �ڿ� ������ ���Ƽ� %n �ʿ����
							nVertexToAdd = (k+2)%3;
							break;
						}
					}
				}
				if(nEdge!=-1)
					break;
			}

			// �´� edge �� ������ ����
			if(nEdge==-1) return NULL;

			vecVertices.insert( vecVertices.begin() + nEdge, pPolygon->v[nVertexToAdd] );
		}
		nCount++;
	}

	rpolygon* pMerged = new rpolygon;
	pMerged->nCount = vecVertices.size();
	pMerged->v = new rvertex[pMerged->nCount];
	pMerged->dwFlags = dwFlags;
	pMerged->pMaterial = pMaterial;
	pMerged->nLightMapID = nLightMapID;
	memcpy( &pMerged->v[0], &vecVertices[0], sizeof(rvertex)*pMerged->nCount );

	return pMerged;
}

void MCplug2::ExportStaticMesh(INode* node, TimeValue t)
{
	ObjectState os = node->EvalWorldState(t);

	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID) {
		return; 
	}

	BOOL needDel = FALSE;
	Object *obj = node->EvalWorldState(t).obj;

	TriObject* tri = GetTriObjectFromNode(node, t, needDel);
	if (!tri) {
		return;
	}

	Mtl* nodeMtl	= node->GetMtl();
	Matrix3 tm		= node->GetObjTMAfterWSM(t);
	BOOL negScale	= TMNegParity(tm);

	Matrix3 pivot = tm;
	pivot.NoTrans();

	//Matrix3 nodetm = node->GetNodeTM(0);
	//Matrix3 invNodetm = Inverse(nodetm);

	RMaxMaterial *pNodeMaterial = m_meshList.m_pMaterialKeeper->GetMaterial(nodeMtl);
	_ASSERT(NULL!=pNodeMaterial);

	// lightmap id
// 	string strLightMapName;
// 	GetUserPropertyString( node, "lightmap", &strLightMapName );
//	int nLightMapID = m_meshList.m_pMaterialKeeper->FindLightMap(node);
	int nLightMapID = 0;

	Mesh* mesh = &tri->GetMesh();
	mesh->buildNormals();

	//////////////////////////////////////////////////////////////////////////
	// ���� Ÿ�Կ� ���� RGeometry�� �����Ѵ�
	// �⺻ �����̸�		: RFVF_XYZ
	// ���� �÷��� ����ϸ� : RFVF_DIFFUSE
	//////////////////////////////////////////////////////////////////////////
	bool bVertexColor = false;
	GetUserPropertyString( node, "set_vertexcolor", &bVertexColor );
	bool bShade = true;
	GetUserPropertyString( node, "set_shade", &bShade );
	if( bShade == false )
		bVertexColor = true;

	//////////////////////////////////////////////////////////////////////////
	// ������ FLAG ����
	// RM_FLAG_COLLISION_MESH : �浹 ������ cl2 ����� �������ΰ�
	//////////////////////////////////////////////////////////////////////////
	DWORD dwPolygonFlag = 0;
	bool bCollision = true;
	GetUserPropertyString( node, "set_collision", &bCollision );
	if( bCollision )
		dwPolygonFlag |= RM_FLAG_COLLISION_MESH;
	
	// �浹 ���� �޽� (Box, Sphere, Capsule �� �����ϴ�)
	bool bCollisionOnly = false;
	GetUserPropertyString( node, "set_collisiononly", &bCollisionOnly );
	if( bCollision )
		dwPolygonFlag |= RM_FLAG_COLLISION_MESHONLY;


	//////////////////////////////////////////////////////////////////////////
	// Visible
	// InVisible �̸� FVF �� 0 �̴�. FVF�� 0�̸� ������ ���� �ʴ´�.
	//////////////////////////////////////////////////////////////////////////
	bool bVisible = true;
	GetUserPropertyString( node, "set_visible", &bVisible );

	float fVisibility = 1.0f;
	fVisibility = node->GetVisibility(t);
	
	DWORD idFVF = 0;

	if( bVisible )
	{
		idFVF |= RFVF[RFVF_XYZ].fvf;
		idFVF |= RFVF[RFVF_NORMAL].fvf;
		idFVF |= RFVF[RFVF_TEX2].fvf;
		if( bVertexColor || (fVisibility < 1.0f) )
			idFVF |= RFVF[RFVF_DIFFUSE].fvf;
	}

	RGeometry* pCurrentGeometry = m_pCurrentCell->GetGeometry(idFVF);
	
	vector<rpolygon*> vecTriangles;
	for (int i=0; i<mesh->getNumFaces(); i++) 
	{
		// material id
		RMaxMaterial *pPolygonMaterial = NULL;
		if(pNodeMaterial)
		{
			pPolygonMaterial = pNodeMaterial->GetMaterialByID( mesh->faces[i].getMatID() );
		}

		rpolygon *rf= new rpolygon;
		rf->v		= new rvertex[3];
		rf->nCount	= 3;
		rf->dwFlags = dwPolygonFlag;
		rf->pMaterial = pPolygonMaterial;
		rf->nLightMapID = nLightMapID;
		rf->bColor	= false;

		Face* f = &mesh->faces[i];

		for(int j=0;j<3;j++)
		{
			int ncorner = negScale ? 3 - j -1 : j;

			Point3 coord = tm * mesh->verts[ f->v[ncorner] ];
			rf->v[j].coord	= MVector3( (float*)coord );

			DWORD vert = f->getVert(ncorner);
			Point3 vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
			Point3 p		= pivot * vn;
			rf->v[j].normal=MVector3((float*)p);

			//0: Vertex Color channel.
			//1: Default mapping channel (the TVert array).
			//2 through MAX_MESHMAPS-1: The new mapping channels available in release 3.0
			if (mesh->mapSupport(0) || mesh->mapSupport(1)) {
				DWORD tvert = mesh->tvFace[i].t[ncorner];
				UVVert tv = mesh->tVerts[tvert];
				rf->v[j].u = tv.x;
				rf->v[j].v = 1.f - tv.y;
			}

			// mesh->vertCol�� ������� �ʴ� VColor�� �����ϴ� ��찡 �ִ�. �̸� �����ϱ� ���� vcFace���� �ε����� ��´�.
			int VertexColorIndex = -1;
			if(mesh->vcFace)
				VertexColorIndex = mesh->vcFace[i].t[j];

			if( fVisibility < 1.0f )	// �ֿ켱 ���� // Visibility �� ������ ���� �÷��� �־��ش�.
				rf->v[j].color = MVector3(fVisibility, fVisibility, fVisibility);
			else if( mesh->numCVerts && ( VertexColorIndex >= 0 ) )
				rf->v[j].color = MVector3( (float*)mesh->vertCol[ VertexColorIndex ] );//f->v[ncorner] ] );
			else
				rf->v[j].color = MVector3(1.0f, 1.0f, 1.0f);

			// 3�� ��ä��
			if (mesh->mapSupport(3)) {
				DWORD tvert3 = mesh->mapFaces(3)[i].t[ncorner];
				UVVert tv3 = mesh->mapVerts(3)[tvert3];
				rf->v[j].u2 = tv3.x;
				rf->v[j].v2 = 1.f - tv3.y;
			}

		}
		vecTriangles.push_back(rf);
	}

	//////////////////////////////////////////////////////////////////////////
	// �浹�� �޽� ����
	for(size_t i=0;i<vecTriangles.size();i++)
	{
		rpolygon *rf= vecTriangles[i];
		DWORD dwMaterialID = m_collisionTreeBuilder.GetZoneMaterialID(rf->pMaterial->GetRMaterial()->GetName());
		m_collisionTreeBuilder.AddPolygon(rf->v[0].coord, rf->v[1].coord, rf->v[2].coord, 0, 0, dwMaterialID);
	}

	// polygon ���� ������ ���Ѵ�
	BitArray bitAdded;
	bitAdded.SetSize ( mesh->getNumFaces() );
	bitAdded.ClearAll();
	BitArray bitCurrentPolygon;
	bitCurrentPolygon.SetSize ( mesh->getNumFaces() );

	MeshTempData* pTemp = new MeshTempData;
	pTemp->SetMesh (mesh);
	AdjFaceList *af = pTemp->AdjFList ();

	for (int i=0; i<mesh->getNumFaces(); i++) 
	{
		if(bitAdded[i]) continue;

		bitCurrentPolygon.ClearAll();

		// max ���� �⺻���� polygon �����ϴ� �ɼ�
		bool ignoreVisEdge = false;
		mesh->PolyFromFace (i, bitCurrentPolygon, DegToRad( 45 ) , ignoreVisEdge, af);

		vector<rpolygon*> vecCurrentPolygon;
		for (int j=0; j<mesh->getNumFaces(); j++) 
		{
			if(!bitCurrentPolygon[j]) continue;
			vecCurrentPolygon.push_back( vecTriangles[j] );
		}

		rpolygon *rf = MergePolygon( vecCurrentPolygon );
		if(rf)
		{
			// ���������� ����� ����, �ﰢ������ �����
			for (size_t j=0; j<vecCurrentPolygon.size(); j++) 
			{
				if(!bitCurrentPolygon[j]) continue;
				delete vecTriangles[j];
				vecTriangles[j] = NULL;
			}
			// ������ �ﰢ���� üũ�Ѵ�
			bitAdded |= bitCurrentPolygon;
		}
		else	// �����ϸ� triangle�� ���� ���Ѵ�
			rf = vecTriangles[i];

		// ���� Geometry �� �������� ���Ѵ�
		pCurrentGeometry->m_sourcePolygons.push_back(rf);
	}

	return;
}

Point3 MCplug2::GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
	Face* f = &mesh->faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals;
	Point3 vertexNormal;
	
	if (rv->rFlags & SPECIFIED_NORMAL) {
		vertexNormal = rv->rn.getNormal();
	}
	else if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) {
		if (numNormals == 1) {
			vertexNormal = rv->rn.getNormal();
		}
		else {
			for (int i = 0; i < numNormals; i++) {
				if (rv->ern[i].getSmGroup() & smGroup) {
					vertexNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else {
		vertexNormal = mesh->getFaceNormal(faceNo);
	}
	
	return vertexNormal;
}

RCell* MCplug2::MakeCell(INode* node)
{
	TimeValue t = GetStaticFrame();

	int i;
	Matrix3 tm		= node->GetObjTMAfterWSM(t);
	BOOL negScale	= TMNegParity(tm);

	ObjectState os = node->EvalWorldState(t);

	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID) {
		return NULL; 
	}

	BOOL needDel = FALSE;
	Object *obj = node->EvalWorldState(t).obj;

	PolyObject *poly = NULL;
	if (obj->CanConvertToType(Class_ID(POLYOBJ_CLASS_ID, 0))) { 
		poly = (PolyObject *) obj->ConvertToType(t, 
			Class_ID(POLYOBJ_CLASS_ID, 0));

		if (obj != poly) needDel = TRUE;
	}
	if(!poly) return NULL;

	Matrix3 normalTM;
	normalTM = tm;
	normalTM.NoTrans();

	RCell* pCell = new RCell;
	pCell->m_strName = node->GetName();
	// polygons
	for (i=0; i<poly->mm.FNum(); i++) 
	{

		// ������鿡 �ְ� convex �̸� �ϳ��� ���, �׷��� ������ �ﰢ������ �ɰ���
		if(!CheckCoplanarAndConvex(poly->mm,i))
		{
			mlog3( "cell �� �̷�� ������ ��, �� ������� ���� �ʰų� ���ϴٰ����� �ƴ� ��찡 �ֽ��ϴ�\n" );
			return false;
		}

		// world ������ ����� ���Ѵ�

		Point3 center;
		Point3 fnormal;
		poly->mm.ComputeNormal(i,fnormal,&center);
		fnormal = normalTM * fnormal;

		MNFace *f = poly->mm.F(i);
		Point3 aPoint = tm * poly->mm.P(f->vtx[0]);

		MPlane plane(MVector3((float*)fnormal),MVector3((float*)aPoint));
		pCell->m_planes.push_back( plane );

		// face vertices
		RCellFace* pCellFace = new RCellFace;
		pCellFace->nVerts = f->deg;
		pCellFace->pVerts = new RVector[f->deg];
		for (int j=0; j< f->deg; j++) 
		{
			Point3 facePoint = tm * poly->mm.P(f->vtx[j]);
			pCellFace->pVerts[j].Set( facePoint.x, facePoint.y, facePoint.z );
		}
		pCell->m_cellFaces.push_back( pCellFace );
	}

	return pCell;
}

// property string �� ã�´�, 
// ex) target = test1 �� ������Ƽ�� ������� GetUserPro..( node, "target" ) �� pOutString���� "test1" �� �־��ش�, ã���� true ������ false�� ����
bool MCplug2::GetUserPropertyString(INode* node, const char* szPropertyName, string* pOutString)
{
	TSTR userPropBuffer;
	node->GetUserPropBuffer(userPropBuffer);

	istringstream iStringBuffer(userPropBuffer.data());
	string aLine;
	while(getline(iStringBuffer,aLine))
	{
		// trim right
		int n = aLine.find_last_not_of(" \n\r");
		if(n==string::npos) continue;
		string buf = aLine.substr(0,n+1);

		stringstream sstream(buf); // string�� stream�� �ִ´�
		sstream >> aLine;

		if( (strncmp(aLine.c_str(),szPropertyName,aLine.length())==0) 
			&& (strncmp(aLine.c_str(),szPropertyName,strlen(szPropertyName))==0) )
		{
			if(pOutString)
			{
				string strTargetName = buf.substr(buf.find_first_of("=")+1);
				int n = strTargetName.find_first_not_of(" \t\v\n");	// trim left
				if(n!=string::npos) strTargetName = strTargetName.substr(n);
				*pOutString = strTargetName;
			}
			return true;
		}
	}

	return false;
}

bool MCplug2::GetUserPropertyString(INode* node, const char* szPropertyName, bool* pBool)
{
	string strValue;
	if(!GetUserPropertyString(node,szPropertyName,&strValue))
		return false;

	if(pBool)
	{ 
		if(_stricmp( strValue.c_str(), "true" ) == 0 )
			*pBool = true;
		else
			*pBool = false;
	}
	return true;
}

RPortal* MCplug2::MakePortal(INode* node)
{
	TimeValue t = GetStaticFrame();

	Matrix3 tm		= node->GetObjTMAfterWSM(t);
	BOOL negScale	= TMNegParity(tm);

	ObjectState os = node->EvalWorldState(t);

	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID) {
		return NULL; 
	}

	BOOL needDel = FALSE;
	Object *obj = node->EvalWorldState(t).obj;

	PolyObject *poly = NULL;
	if (obj->CanConvertToType(Class_ID(POLYOBJ_CLASS_ID, 0))) { 
		poly = (PolyObject *) obj->ConvertToType(t, 
			Class_ID(POLYOBJ_CLASS_ID, 0));

		if (obj != poly) needDel = TRUE;
	}
	if(!poly) return NULL;

	if(poly->mm.FNum()!=1 || !CheckCoplanarAndConvex(poly->mm,0))
	{
		mlog3("%s : portal �� �̷�� �������� ���ϳ��� ���ϴٰ����̾�� �մϴ�\n", node->GetName());
		return NULL;
	}

	const char* TARGET = "target";
	string strTargetName;
	GetUserPropertyString( node, TARGET, &strTargetName);

	if( m_pCurrentCell->m_strName == strTargetName )
	{
		mlog3("%s : portal�� �θ� ���� �����Ϸ��� ���� ���Ƽ� �ͽ���Ʈ���� ���ܵǾ����ϴ�.\n", node->GetName());
		return NULL;
	}

	RPortal* pPortal = new RPortal;
	pPortal->m_strName = node->GetName();

	MNFace *f = poly->mm.F(0);
	pPortal->nVertices = f->deg;
	pPortal->m_pVertices = new Point3[f->deg];

	for(int j=0; j< f->deg; j++)
	{
		pPortal->m_pVertices[j] = tm * poly->mm.P(f->vtx[j]);
	}
	pPortal->m_strParentCellName = m_pCurrentCell->m_strName;
	pPortal->m_strTargetCellName = strTargetName;

	return pPortal;
}

void MCplug2::ExportEffect( INode* node, RMaxMeshNode* pMeshNode)
{
	const char *ADD_EFFECT = "add_effect";
	const char *SET_ADD_EFFECT = "set_addeffect";
	const char *SET_EFFECT_NAME = "set_effect_name";
	const char *SET_EFFECT_SCALE = "set_effect_scale";
	const char *SET_STATIC = "set_static";

	float fEffectScale = 1.0f;

	bool bStatic = false;
	GetUserPropertyString( node, SET_STATIC, &bStatic );

	bool bAddEffect = false;

	TSTR userPropBuffer;
	node->GetUserPropBuffer(userPropBuffer);

	istringstream iStringBuffer(userPropBuffer.data());
	string aLine;
	string aValue;
	while(getline(iStringBuffer,aLine))
	{
		// trim right
		int n = aLine.find_last_not_of(" \n\r");
		if(n==string::npos) continue;
		string buf = aLine.substr(0,n+1);

		stringstream::pos_type spos;
		stringstream sstream(buf); // string�� stream�� �ִ´�
		sstream >> aLine;

		if(strncmp(aLine.c_str(),ADD_EFFECT,strlen(ADD_EFFECT))==0) 
		{
			string strEffectName = buf.substr(buf.find_first_of("=")+1);
			int n = strEffectName.find_first_not_of(" \t\v\n");	// trim left
			if(n!=string::npos) strEffectName = strEffectName.substr(n);
			m_effectList.Add( pMeshNode, node->GetName(),strEffectName, bStatic, fEffectScale);
		}
		else if(strncmp(aLine.c_str(),SET_ADD_EFFECT,strlen(SET_ADD_EFFECT))==0) 
		{
			sstream >> aLine;	// =

			// set_addeffect = true // �� ��
			if(strcmp(aLine.c_str(), "=") == 0)	
			{
				sstream >> aLine;
				if(strcmp(aLine.c_str(), "true") == 0)
					bAddEffect = true;
			}
		}
		else if(strncmp(aLine.c_str(),SET_EFFECT_SCALE,strlen(SET_EFFECT_SCALE))==0) 
		{
			sstream >> aLine;	// =

			// set_effect_scale = 1.0 // �� ��
			if(strcmp(aLine.c_str(), "=") == 0)	
			{
				sstream >> fEffectScale;
			}
		}
		else if(strncmp(aLine.c_str(),SET_EFFECT_NAME,strlen(SET_EFFECT_NAME))==0) 
		{
			if( bAddEffect == true )
			{
				string strEffectName = buf.substr(buf.find_first_of("=")+1);
				int n = strEffectName.find_first_not_of(" \t\v\n");	// trim left
				if(n!=string::npos) strEffectName = strEffectName.substr(n);
				m_effectList.Add(pMeshNode, FixupName( node->GetName() ),strEffectName, bStatic, fEffectScale);

				bAddEffect = false;
			}
		}
	}
}