interface()
{
	$name = ppl_g_write_dsf_rt3
	$define = ppl_g_write_color3
	$dependency = g_depth
	$dependency = g_constant
}

shared float g_fDSFID = 0;

void PSMAIN(inout PSOUTPUT output)
{
	// ���Ĵ� G�н� ���������� ���� ������Ʈ �κп��� ä������. �� �������� �̸� ������ ����.
	// ���� �ڵ���� color.a�� ���� �����ϱ⵵ �ϰ� E_fAlpha�� ��� �ϱ⵵ �ϴµ� 
	// E_fAlpha�� ���� �ݿ��� �Ͽ��� ���������� �ȴ�.
	float fAlpha = 1;
	IMPORT( E_fAlpha, fAlpha = E_fAlpha);

	// DSFID�� �״�� ���
	output.color2 = float4( g_fDSFID, 1, 1, fAlpha);
}
