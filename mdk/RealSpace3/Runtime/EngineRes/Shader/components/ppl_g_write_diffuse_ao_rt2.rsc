interface()
{
	$name = ppl_g_write_diffuse_ao_rt2
	$define = ppl_g_write_color3
	$dependency = g_constant
}

void PSMAIN(inout PSOUTPUT output)
{
	float fAO = 1;	/// 1�� ��찡 AO ó�� ���ϴ� ��
	IMPORT( E_fAO, fAO = E_fAO);
	
	// �ƿ�ǲ�� color�� �״�� ����. 
	// output.color�� �״�� ��� �ϰ� �ٸ� G���۵��� color 1,2,3�� �����ϴ� ���� ���� ��������, color3�� diffuse, DSF, noInfo ���� ���� �������� �ξ�� �ϱ⿡...
	
	
	output.color2 = float4( output.color.r, output.color.g, output.color.b, fAO);
}
