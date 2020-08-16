interface()
{
	$name		= reflect_map
	$define 	= reflect_map
	$dependency	= texcoord
}

sampler sEnv_Map : register(s2);

shared int g_iUseEnvAlpha;


float fresnel(float3 eyevec, float3 normal, float power, float bias)
{
	normal = normalize(normal);
	eyevec = normalize(eyevec);
	
	float fresnel = saturate(abs(dot(normal,eyevec))); // �븻�� �� ���͸� ���� �𼭸� ���Ѵ�.
	//fresnel = 1 - fresnel; 								// �𼭸� ���� ���� �����ϴ�.
	fresnel = pow(fresnel, power);						// �𼭸� �κ��� ��ī�ο����� ����.
	fresnel += bias; 									// ��������� ������ ������ΰ�.
	
	return saturate(fresnel);
}

struct VSOUTPUT
{
	float4 vWorld_Position	: TEXCOORD%;
	float3 vViewWorld		: TEXCOORD%;
};


void VSMAIN(out VSOUTPUT output)
{	
	float4 vWorldPosition = 0;
	IMPORT(E_vWorldPosition, vWorldPosition = E_vWorldPosition);
	output.vWorld_Position = vWorldPosition;

	output.vViewWorld = vWorldPosition - g_vEyePosition;
}

void PSMAIN(in VSOUTPUT input, inout PSOUTPUT output )
{	
	// �ϴ� �ݻ籤 �� ���� ä���� ������ Ź���� ǥ���Ѵ�.
	float2 vSpecularCoord = 0;
	IMPORT( E_vSpecularCoord, vSpecularCoord = E_vSpecularCoord);
	
	float fGlossiness = g_vSavedAlpha[ g_iUseEnvAlpha ];
	IMPORT( E_fEnvGlossiness, fGlossiness = E_fEnvGlossiness );
	
	// �븻�� �븻 �����´�.
	float3 vBumpNormal = 0;
	IMPORT( E_vNormalColor, vBumpNormal = E_vNormalColor.xyz);
	
	float3 vGetNormal = 0;
	IMPORT(E_pgNormal, vGetNormal = E_pgNormal );	
	vGetNormal = mul(vGetNormal, (float3x3)g_mViewInv).xyz;
		
	float3 I = input.vViewWorld;		// �� ���������� ���� ��ġ
	float3 R = reflect( I, vGetNormal );
	
	R.yz = R.zy;
	
	float4 returnColor = texCUBElod( sEnv_Map, float4(R, 6.01329f - (fGlossiness * 6.01329f)));	

	vGetNormal = 0;
	IMPORT(E_pgNormal, vGetNormal = E_pgNormal );
	float fFresnel = fresnel( I, vGetNormal, 2.26f, 0.55f);	// 4���� ����. ���� ���� ������ ����, ���� ���� �а� ���´�.
	
	EXPORT( float, E_vEnv_Fresnel, fFresnel );
	EXPORT( float4, E_vEnvironmentColor, returnColor );
	EXPORT( float,  E_fEnvironmentAlpha, returnColor.a );	// ���ĸ� ���� �� ��� �ϴ°�?
}
