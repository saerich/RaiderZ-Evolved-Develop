interface()
{
	$name = ppl_alpha_from_si_a
	$dependency = selfilluminationmap
}

void PSMAIN( inout PSOUTPUT output)
{
  // alpha ���� ����
  float fAlpha = 1;
  IMPORT( E_fAlpha, fAlpha = E_fAlpha);
  // �ҽ� ����
	float fAlphaSrc = 1;
	IMPORT( E_fSelfilluminationAlpha, fAlphaSrc = E_fSelfilluminationAlpha);
	// alpha ���� ����
	float fTargetAlpha = fAlpha * fAlphaSrc;
	EXPORT(float, E_fAlpha, fTargetAlpha);
	
	output.color.a = fTargetAlpha;
}
