interface()
{
	$name = pplvblur
	$define = pplvblur
	$dependency = transform_vblur
}


struct VSOUTPUT
{
	float4 vAlphaForce	: COLOR2;
};

void VSMAIN(inout VSOUTPUT output)
{
	float fAlphaForce;
	IMPORT( E_fTailAlpha, fAlphaForce = E_fTailAlpha);
	output.vAlphaForce.a = fAlphaForce;
	
	
}

void PSMAIN(VSOUTPUT input, inout PSOUTPUT output)
{
	output.color.a = output.color.a * input.vAlphaForce.a ;		// ���� ���İ� ���� �Ǿ� �ִ� ���� ����Ͽ� ���� ���� ���� ���� ä��  - 090722, OZ
}
