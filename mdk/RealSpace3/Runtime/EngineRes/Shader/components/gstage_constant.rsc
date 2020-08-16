interface()
{
	$name = g_constant
	$define = g_constant
}

struct VSOUTPUT
{
	float3 vNormal		: TEXCOORD%;
};

void VSMAIN(inout VSOUTPUT output)
{
	// G-Stage�� ��� �ϴ� ���� �� �����̽� ���� �븻�̴�. �׷��Ƿ� WorldView�� ���� �븻�� ������� ���.
	float3 vWorldViewNormal = 0;
	IMPORT ( E_vWorldViewNormal, vWorldViewNormal = E_vWorldViewNormal);

	output.vNormal 		= vWorldViewNormal;
}

void PSMAIN(VSOUTPUT input, inout PSOUTPUT output)
{
	//EXPORT( float2, E_pgSpecular, float2(0,0));
	EXPORT( float, E_pgDepthZ, 0);
	EXPORT( float, E_pgDepthW, 0);	
	float3 interpolatedNormal = normalize(input.vNormal);
	EXPORT( float3, E_pgNormal, interpolatedNormal);
	EXPORT( float3, E_pgNormalInterpolated, interpolatedNormal);
	
}
