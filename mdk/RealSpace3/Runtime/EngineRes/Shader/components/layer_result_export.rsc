interface()
{
	$name = layer_result_export
	$define = diffusemap
}

void PSMAIN( inout PSOUTPUT output)
{
	float4 vCompositeResult;
	IMPORT( E_vDiffuseColor, vCompositeResult = E_vDiffuseColor);
	
	EXPORT(float4, E_vDiffuseColor, vCompositeResult);
	EXPORT(float, E_fDiffuseAlpha, vCompositeResult.a);
	
	// alpha ó���� ���⼭ ���� �ʴ´�.
	vCompositeResult.a = 1.0;
	output.color = vCompositeResult;
}