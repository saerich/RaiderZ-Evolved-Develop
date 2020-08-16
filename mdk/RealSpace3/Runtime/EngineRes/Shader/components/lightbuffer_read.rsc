interface()
{
	$name = lightbuffer_read
	$define = lightbuffer_read
	$define = ppl_specular
	$dependency = m_constant
}


sampler LightBufferMap : register(s8);


void PSMAIN( inout PSOUTPUT output)
{
	float4 vBufferCoord;
	IMPORT ( E_vBufferCoord, vBufferCoord = E_vBufferCoord);
	
	float4 fLighting = tex2Dproj( LightBufferMap, vBufferCoord);
	// ������ ��
	float3 vLitColor = fLighting.rgb;
	EXPORT(float3, E_vLitColor, vLitColor);
	// �� ����ŧ���� ���� ���⼭ �� �̾��ش�.
	// ���� ppl_specular�� E_vPixelSpecular�� ��� �ϴ� �͵��� ���� ���� �Ѱ�.
	EXPORT(float3, E_vPixelSpecular, fLighting.aaa);
}