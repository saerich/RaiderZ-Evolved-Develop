interface()
{
	$name = ppl_g_specular
	$define = ppl_g_specular
	$dependency = g_constant
}

shared float	g_fGlossiness;

void PSMAIN(inout PSOUTPUT output)
{
	float3 vLitSpecular = 0;
	IMPORT( E_vLitSpecular, vLitSpecular = E_vLitSpecular);

	// specular from map���� �̹� ���ũ�� ȭ������ ��ȯ �ؼ� ����ֹǷ� ���� ���� �� �ʿ� ����.
	//float fPixelSpecular = max( max( vLitSpecular.r, vLitSpecular.g), vLitSpecular.b);
	float fPixelSpecular = vLitSpecular.r;
	
	EXPORT( float2, E_pgSpecular, float2( fPixelSpecular, g_fGlossiness));
}
