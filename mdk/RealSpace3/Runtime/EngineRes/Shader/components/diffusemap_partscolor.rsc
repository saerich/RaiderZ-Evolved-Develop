interface()
{
	$name = diffusemap_partscolor
	$define = diffusemap_partscolor
	$dependency = diffusemap_partscolor_constant
	$dependency = diffusemap
}

void PSMAIN( inout PSOUTPUT output)
{
	float fDiffuseAlpha = 0;
	IMPORT ( E_fDiffuseAlpha, fDiffuseAlpha = E_fDiffuseAlpha);
	
	// ��ǻ�� ���İ��� ���� Į�� ���� �Ķ���ͷ� ����ϹǷ� ���İ��� �����Ѵ�.
	float fAlpha = 1;
	EXPORT(float, E_fAlpha, fAlpha);

	// parts �÷� ���� vDiffuseColor.rgb * vDiffuseColor.a + g_vPartsColor.rgb * ( 1 - vDiffuseColor.a )
	output.color.rgb = lerp( output.color.rgb, g_vPartsColor.rgb, 1 - fDiffuseAlpha );
}