interface()
{
	$name = ppl_alpha_from_vcolor_a
	$dependency = vcolor
}

void PSMAIN( inout PSOUTPUT output)
{
	// alpha ���� ����
	float fAlpha = 1;
	IMPORT( E_fAlpha, fAlpha = E_fAlpha);
	// �ҽ� ����
	float fAlphaSrc = 1;
	IMPORT( E_fVAlpha, fAlphaSrc = E_fVAlpha);
	// alpha ���� ����
	float fTargetAlpha = fAlpha * fAlphaSrc;
	EXPORT(float, E_fAlpha, fTargetAlpha);
	
	output.color.a = fTargetAlpha;
}
