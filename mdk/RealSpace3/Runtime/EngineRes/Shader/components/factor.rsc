interface()
{
	$name = factor
}

shared float4  	g_textureFactor;

void PSMAIN(inout PSOUTPUT output)
{
	output.color  = output.color * g_textureFactor ;
	
	// ������ alpha�� �ݿ��ϱ� ������ �ٷ� ���۽�Ƽ �ƿ����� ��� ������ ����.
	// alpha ���� ����
	float fAlpha = 1;
	IMPORT( E_fAlpha, fAlpha = E_fAlpha);
	// �ҽ� ����
	float fAlphaSrc = g_textureFactor.a;
	// alpha ���� ����
	float fTargetAlpha = fAlpha * fAlphaSrc;
	EXPORT(float, E_fAlpha, fTargetAlpha);
	
}