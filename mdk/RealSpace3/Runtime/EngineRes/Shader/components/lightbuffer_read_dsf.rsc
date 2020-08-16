interface()
{
	$name = lightbuffer_read
	$define = lightbuffer_read
	$define = ppl_specular
	$dependency = m_constant
}


sampler LightBufferMap : register(s8);
sampler DSFBufferMap : register(s9);

shared float2 g_vScreenSizeInv;
shared float g_fDSFID = 0;

void PSMAIN( inout PSOUTPUT output)
{
	float4 vBufferCoord;
	IMPORT ( E_vBufferCoord, vBufferCoord = E_vBufferCoord);
	float2 vCoord = vBufferCoord.xy / vBufferCoord.w;
	// �̰� �ϴ°� �³�?
	//vBufferCoord.xy += float2( 0.5f * g_vScreenSizeInv.x, 0.5f * g_vScreenSizeInv.y);
	
    float sample = 0.00001f;
	float4 fLighting = 0;

	for( int x=-1; x<3; x++)
	{
		for( int y=-1; y<3; y++)
		{	
			float2 vSamplePostion = vCoord + float2( (x-0.5f)*g_vScreenSizeInv.x, (y-0.5f)*g_vScreenSizeInv.y);
			if( tex2D( DSFBufferMap, vSamplePostion).r == g_fDSFID)
			{
				fLighting += tex2D( LightBufferMap, vSamplePostion);
				sample += 1;
			}
		}
	}

	fLighting = fLighting / sample;


	// ������ ��
	float3 vLitColor = fLighting.rgb;
	EXPORT(float3, E_vLitColor, vLitColor);
	// �� ����ŧ���� ���� ���⼭ �� �̾��ش�.
	// ���� ppl_specular�� E_vPixelSpecular�� ��� �ϴ� �͵��� ���� ���� �Ѱ�.
	EXPORT(float3, E_vPixelSpecular, fLighting.aaa);
}