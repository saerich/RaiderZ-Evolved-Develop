interface()
{
	$name = ppl_clip_alpha
	$define = ppl_clip_alpha
}

shared float	g_fAlphaRef = 0;

void PSMAIN(inout PSOUTPUT output)
{
	if( output.color.a <= g_fAlphaRef )
		discard;
	// or
	// clip �� arg < 0 �ε� arg<=0�̵Ƿ���... ��¼��...
	// clip( output.color.a - g_fAlphaRef);
}
