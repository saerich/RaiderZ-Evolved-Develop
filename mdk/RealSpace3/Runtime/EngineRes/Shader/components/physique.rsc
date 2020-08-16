interface()
{
	$name = physique
	$define = physique
	$dependency = position
}

struct VSINPUT
{
	float2 vWeight		: BLENDWEIGHT;
	float4 vIndex		: BLENDINDICES;	
};

static const int MAX_MATRICES = 54;	// RActorNodePhysique�� MAX_BONE_COUNT�� 54�� �Ǿ����� - 090720, OZ
shared float4x4 g_blendMatrices[MAX_MATRICES];	

void VSMAIN(VSINPUT input)
{
	float4 vPosition = (float4)0;
	float3 vNormal = 0;
	float4 vTangent = 0;
	float3 Weight;
	
	Weight[0] = input.vWeight[0];
	Weight[1] = input.vWeight[1];
	Weight[2] = 1.0f-(input.vWeight[0]+input.vWeight[1]);
	
	float3 Index = input.vIndex;

	for( int i=0;i<3;i++)
	{
		IMPORT(E_vLocalPosition, vPosition += mul(E_vLocalPosition, g_blendMatrices[Index[i]])*Weight[i]);
		IMPORT(E_vLocalNormal, vNormal += mul(E_vLocalNormal,(float3x3)g_blendMatrices[Index[i]])*Weight[i]);	
		IMPORT(E_vLocalTangent, vTangent.xyz += mul(E_vLocalTangent.xyz,(float3x3)g_blendMatrices[Index[i]])*Weight[i]);
		IMPORT(E_vLocalTangent, vTangent.w = E_vLocalTangent.w);
	}
	
	EXPORT(float4, E_vLocalPosition, vPosition);
	EXPORT(float4, E_vLocalPositionPrev, vPosition);
	EXPORT(float3, E_vLocalNormal, vNormal);
	EXPORT(float4, E_vLocalTangent, vTangent);
}









// interface()
// {
// 	$name = physique
// 	$define = physique
// 	$dependency = position
// }
// 
// struct VSINPUT
// {
// 	float3 vWeight		: BLENDWEIGHT;				// blendweight�� 4��.. �ڷᱸ���� 2->3 ���� - 090707, OZ
// 	float4 vIndex		: BLENDINDICES;	
// };
// 
// static const int MAX_MATRICES = 55;
// 
// shared float4x4 g_blendPrevMatrices[MAX_MATRICES];		// ���� �������� ��. ȯ���ϰԽ꿬. - 090629, OZ
// shared float4x4 g_blendMatrices[MAX_MATRICES];	
// 
// 
// void VSMAIN(VSINPUT input)
// {
// 	float4 vPosition = (float4)0;
// 	float4 vPositionPrev = (float4)0;	// 090629, OZ
// 	float3 vNormal = 0;
// 	float3 vTangent = 0;
// 	float4 Weight;											// blend weight 4 - 090707, OZ 
// 	
// 	Weight[0] = input.vWeight[0];
// 	Weight[1] = input.vWeight[1];
// 	Weight[2] = input.vWeight[2];							// blend weight 4 - 090707, OZ 
// 	//Weight[2] = 1.0f-(input.vWeight[0]+input.vWeight[1]);
// 	Weight[3] = 1.0f-(input.vWeight[0]+input.vWeight[1]+input.vWeight[2]);	// blend weight 4 - 090707, OZ 
// 	
// 	float4 Index = input.vIndex;							// blend weight 4 - 090707, OZ 
// 
// 	for( int i=0;i<4;i++)	// blend weight 3 -> 4 - 090707, OZ 
// 	{
// 		// ���÷� ���������� ������� - 090629, OZ
// 		//IMPORT(E_vLocalPosition, vPosition += mul(E_vLocalPosition, g_blendMatrices[Index[i]])*Weight[i]);
// 		//IMPORT(E_vLocalPosition, vPositionPrev += mul(E_vLocalPosition, g_blendPrevMatrices[Index[i]])*Weight[i]);	// 090629, OZ
// 		float4 vP;
// 		IMPORT(E_vLocalPosition, vP = E_vLocalPosition);
// 		
// 		// ��Ʈ������ �߸��� ������ �׽�Ʈ - TESTOZ
// 		//vPositionPrev += mul( vP, g_blendPrevMatrices[Index[i]])*Weight[i];
// 		// �� �ڵ�� �ϸ� ���� �۵��Ѵ�.. Ȧ�����ͺ�!! 
// 		//vPositionPrev += mul( vP, g_blendMatrices[Index[i]])*Weight[i];
// 		// aoxmflrtm tjfwjd gksms rjtdmf goqhwk TESTOZ
// 		vPositionPrev += mul( vP, g_blendPrevMatrices[Index[i]])*Weight[i];
// 		
// 		vPosition += mul( vP, g_blendMatrices[Index[i]])*Weight[i];
// 		
// 		IMPORT(E_vLocalNormal, vNormal += mul(E_vLocalNormal,(float3x3)g_blendMatrices[Index[i]])*Weight[i]);	
// 		IMPORT(E_vLocalTangent, vTangent += mul(E_vLocalTangent,(float3x3)g_blendMatrices[Index[i]])*Weight[i]);
// 	}
// 	
// 	EXPORT(float4, E_vLocalPosition, vPosition);
// 	
// 	EXPORT(float4, E_vLocalPositionPrev, vPositionPrev);	// 090629, OZ	TESTOZ
// 	
// 	
// 	// ���� �Ȱ�ġ�� �״�� �ٹ��� - TESTOZ 090707, OZ
// 	IMPORT(E_vLocalPosition, vPosition = E_vLocalPosition);
// 	EXPORT(float4, E_vLocalPosition, vPosition);
// 	
// 	
// 	
// 	// �Ʒ��� �ϸ� �� �ȴ�.
// 	// g_blendPrevMatrices�� ���� ���� ��� �ϸ� �̻��Ѱǰ� �Ф�
// 	//EXPORT(float4, E_vLocalPositionPrev, vPosition);	// 090629, OZ	TESTOZ
// 	
// 	EXPORT(float3, E_vLocalNormal, vNormal);
// 	EXPORT(float3, E_vLocalTangent, vTangent);
// }