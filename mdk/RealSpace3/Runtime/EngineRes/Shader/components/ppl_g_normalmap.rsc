interface()
{
	$name 		= ppl_g_normalmap
	$define 	= ppl_g_normalmap
	$dependency = transform
	$dependency = tangent
	$dependency = g_constant
	$dependency = normalmap
}

struct VSOUTPUT
{
	float3 vBinormal	: TEXCOORD%;
	float3 vTangent		: TEXCOORD%;
};

void VSMAIN(inout VSOUTPUT output)
{
	// ź��Ʈ
	float4 vLocalTangent;
	IMPORT ( E_vLocalTangent, vLocalTangent = E_vLocalTangent);
	// G���������� �븻�� �� �����̽� �������� ����� �Ǿ�� �Ѵ�. �׷��Ƿ� ������ ���� ���� ��� �Ѵ�.
	float3 vTangent = normalize( mul( vLocalTangent.xyz, (float3x3)g_matWorldView));
	// �븻 : G���������� �븻�� �� �����̽� �������� ����� �Ǿ�� �Ѵ�. �׷��Ƿ� ������ ���� ���� ��� �Ѵ�.
	float3 vNormal;
	IMPORT ( E_vWorldViewNormal,  vNormal  = E_vWorldViewNormal);
	// ���̳븻
	float3 vBinormal = normalize( cross( vTangent, vNormal)) * vLocalTangent.w;
	
	output.vBinormal = normalize(vBinormal);
	output.vTangent  = normalize(vTangent);
}


void PSMAIN(VSOUTPUT input, inout PSOUTPUT output)
{
	// bumpmap ------------------------------------------------------------------------------------------------------------------
	float3 vBumpNormal = 0;
	IMPORT( E_vNormalColor, vBumpNormal = E_vNormalColor.xyz);
	vBumpNormal = vBumpNormal * 2 - 1;		//expand to -1 to 1 range 

	// �������� ���ƾ� �������� ������ �Ǵ� �� ó�� ���δ�. ������..
	//vBumpNormal.y = -vBumpNormal.y; 		// green channel flipped 
	vBumpNormal = normalize(vBumpNormal); 	//normalized the normal vector 

	float3 B = input.vBinormal;
	float3 T = input.vTangent;
	float3 N;
	IMPORT( E_pgNormal, N = E_pgNormal);
	
	float3x3 TangentToView;
	TangentToView[0] = B;
	TangentToView[1] = T;
	TangentToView[2] = N;

	float3 vNormal = normalize( mul( vBumpNormal, TangentToView));
	
	EXPORT(float3, E_pgNormal, vNormal);
}
