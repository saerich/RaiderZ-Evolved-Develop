interface()
{
	$name = ppl_alpha_from_normal_a
	$dependency = normalmap
}

void PSMAIN( inout PSOUTPUT output)
{
	/// alpha ���� ����
	float fAlpha = 1;
	IMPORT( E_fAlpha, fAlpha = E_fAlpha);
	/// �ҽ� ����
	float fAlphaSrc = 1;
	IMPORT( E_fNormalAlpha, fAlphaSrc = E_fNormalAlpha);
	/// alpha ���� ����
	float fTargetAlpha = fAlpha * fAlphaSrc;
	EXPORT(float, E_fAlpha, fTargetAlpha);
	
	output.color.a = fTargetAlpha;
}