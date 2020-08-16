interface()
{
	$name = diffusemap
	$define = diffusemap
	$dependency = texcoord
}

sampler DiffuseMap : register(s0);

void PSMAIN( inout PSOUTPUT output)
{
	float2 vDiffuseCoord = 0;
	IMPORT( E_vDiffuseCoord, vDiffuseCoord = E_vDiffuseCoord);
	float4 vDiffuseColor = tex2D( DiffuseMap, vDiffuseCoord);
	
	g_vSavedAlpha[0] = vDiffuseColor.a;
	
	EXPORT(float4, E_vDiffuseColor, vDiffuseColor);
	EXPORT(float,  E_fDiffuseAlpha, vDiffuseColor.a);
	// alpha�ҽ��� �̰������� �ͽ���Ʈ ���� ����. 
	// ���� �ҽ��� ���� �ǹǷ� ���⼭���� �����Ǹ� �ȵ�.
	// EXPORT(float,  E_fAlpha, vDiffuseColor.a);
	
	// alpha ó���� ���⼭ ���� �ʴ´�.
	vDiffuseColor.a = 1.0;
	output.color = vDiffuseColor;
}