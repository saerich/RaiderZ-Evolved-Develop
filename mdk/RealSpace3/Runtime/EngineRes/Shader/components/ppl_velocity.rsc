interface()
{
	$name = pplvblur
	$define = pplvblur
	$dependency = transform_vblur
}

// pplvblur�� COLOR2�� ����.
struct VSOUTPUT
{
	float4 vVelocity	: TEXCOORD0;
};

//shared matrix	g_matViewProj;
//shared matrix	g_matWorld;
//shared matrix	g_matWorldPrev;		// ������ ���� ������


void VSMAIN(inout VSOUTPUT output)
{
	float4 vLocalPosition = 0;
	
	// ���û��� ���� ������ ��ġ	
	IMPORT ( E_vLocalPosition, vLocalPosition = E_vLocalPosition);
	// ���������
	float4 vWorldPosition = mul(vLocalPosition, g_matWorld);
	
	// ���û��� ���� ������ ��ġ
	IMPORT ( E_vLocalPositionPrev, vLocalPosition = E_vLocalPositionPrev);
	// ���������
	float4 vWorldPositionPrev = mul(vLocalPosition, g_matWorldPrev);
	
	float4 vPosProjSpaceCurrent = mul( vWorldPosition, g_matViewProj);
	vPosProjSpaceCurrent /= vPosProjSpaceCurrent.w;
	float4 vPosProjSpaceLast = mul( vWorldPositionPrev, g_matViewProj);
	vPosProjSpaceLast /= vPosProjSpaceLast.w;
	
	float vVelocityFactor = 1;
	IMPORT ( E_fVelocityFactor, vVelocityFactor = E_fVelocityFactor);
	
	output.vVelocity = vPosProjSpaceCurrent - vPosProjSpaceLast;
	output.vVelocity *= vVelocityFactor;
	output.vVelocity.x *= +0.5f;
	output.vVelocity.y *= -0.5f;
}

void PSMAIN(VSOUTPUT input, inout PSOUTPUT output)
{
	output.color = input.vVelocity;
	output.color.a = 1;
}
