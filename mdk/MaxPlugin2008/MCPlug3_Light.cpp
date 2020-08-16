#include "stdafx.h"
#include "MCplug3.h"
#include "LightList.h"

void MCplug2::ExportLight(INode* node)
{
	// �����ϴ� ������� üũ
	bool bIgnore = false;	// ���� ����
	GetUserPropertyString( node, "ignore_at_export", &bIgnore );
	if( bIgnore == false )	// �� ����
		GetUserPropertyString( node, "set_export_ignore", &bIgnore );
	if( bIgnore == true)	// �Ѵ� ���и� ������� �ʴ´�
		return;

	// �׻� ����ϵ� static �̸� staticmesh �� �ڽ����� �ƴϸ� actor �� �ڽ����� ���
	ExportLightObject(node);

// 	if (m_options.m_bStaticMesh)
// 	{
// 		bool bStatic = false;
// 		GetUserPropertyString( node, "set_static", &bStatic );
// 		if(bStatic)	// Static Mesh ����϶� Static �Ӽ����� ������ �޽��� ����Ѵ�.
// 		{					
// 			ExportLightObject(node);
// 		}
// 	}
// 	else	// �ƴϸ� �ƿ� ������� �ʴ´�.
// 	{
// 		bool bStatic = false;
// 		GetUserPropertyString( node, "set_static", &bStatic );
// 
// 		if(!bStatic) // elu�� static�� �ƴϰ�
// 		{
// 			// XRef�� �ƴ� Mesh�� ����Ѵ�
// 			Object *obj = node->GetObjectRef();
// 			if ( obj->ClassID() != XREFOBJ_CLASS_ID ) 
// 			{
// 				ExportLightObject(node);
// 			}
// 		}
// 	}
}

void MCplug2::ExportLightObject(INode* node)
{
	TimeValue t = GetStaticFrame();

	ObjectState os = node->EvalWorldState(t);
	if (!os.obj) return;

	GenLight* light = (GenLight*)os.obj;
	struct LightState ls;
	Interval valid = FOREVER;
	Interval animRange = ip->GetAnimRange();

	light->EvalLightState(t, valid, &ls);

	if(ls.type==OMNI_LIGHT || ls.type==DIR_LIGHT || ls.type==FSPOT_LIGHT || ls.type==TSPOT_LIGHT)
	{
		MCLIGHT *plight=new MCLIGHT;
		Matrix3 pivot = node->GetNodeTM(GetStaticFrame());
		AffineParts ap;
		decomp_affine(pivot, &ap);

		Matrix3 matParentNodeMT(true);
		INode* parent = node->GetParentNode();
		if (parent && !parent->IsRootNode()) {
			plight->ParentName = FixupName( parent->GetName() );

			matParentNodeMT = parent->GetNodeTM(GetStaticFrame());
			matParentNodeMT.Invert();
		}

		plight->Name=FixupName( node->GetName() );
		plight->Color=RVector(ls.color.r,ls.color.g,ls.color.b);

		Point3 vPositon = ap.t * matParentNodeMT;
		plight->Position=RVector(vPositon.x, vPositon.y, vPositon.z);

		RMatrix matTM;
		ConvertToMMatrix(matTM, pivot * matParentNodeMT);

		// -z ������ direction, y ������ up
		plight->Direction = -RVector(matTM._31, matTM._32, matTM._33);
		plight->Up = RVector(matTM._21, matTM._22, matTM._23);

		plight->fIntensity=ls.intens;
		plight->fAttnStart=ls.attenStart;
		plight->fAttnEnd=ls.attenEnd;
		plight->lightType = ls.type;

		plight->dwFlags=0;
		if(light->GetShadow()!=FALSE)
			plight->dwFlags|=RL_FLAG_CASTSHADOW;

		bool bStatic = false;
		GetUserPropertyString( node, "set_static", &bStatic );
		plight->bStatic = bStatic;

		plight->fFalloff = ls.fallsize;

		plight->CalcName2Type(node);

		m_lightList.push_back(plight);
	}
}