interface()
{
	$name = diffusemap_noisefade
	$define = diffusemap_noisefade
	$dependency = texcoord
}

// x: ref
shared float4  	g_NoiseFadeParam;
// Ÿ�� �κ� �÷�
shared float4  	g_NoiseFadeColor1;
// �� �κ� �÷�
shared float4  	g_NoiseFadeColor2;

sampler NoiseMap : register(s3);

void PSMAIN( inout PSOUTPUT output)
{
	[branch]
	if( g_NoiseFadeParam.x < 1)
	{
		const float fRefSize = g_NoiseFadeParam.x;
		const float fBurnSize = g_NoiseFadeParam.y;
 		const float fAshSize = g_NoiseFadeParam.z;
 		 		
		float4 vDiffuseTexcoord = float4(0,0,0,0);
		IMPORT( E_vDiffuseCoord, vDiffuseTexcoord.xy = E_vDiffuseCoord);
		float fTextureFactor = tex2Dlod( NoiseMap, vDiffuseTexcoord).r;
		// �ؽ��� ���ø� ���� ���� ���۷��� �� ���� ũ�� discard
		float fFactor = fRefSize - fTextureFactor;
		if( fFactor < 0)
 			discard;

		float fBurnPoint = g_NoiseFadeParam.x - ( fBurnSize + fAshSize);
		float fAshPoint = g_NoiseFadeParam.x - ( fAshSize);

		const float M = 1;
		const float m = 0.7f;
		const float Mm = 0.3f;

		// �������� ���� ����. �ֽ� ���� ����δ�.		
		float fBurnFactor = saturate( (fAshPoint-fTextureFactor) / (fAshPoint-fBurnPoint));
		fBurnFactor = saturate( (fBurnFactor-m) / Mm);		// smoothstep( m, M, fBurnFactor);
		output.color.rgb = lerp( g_NoiseFadeColor1.rgb, output.color.rgb, fBurnFactor);
		
		float fAshFactor = saturate( (fRefSize-fTextureFactor) / (fRefSize-fAshPoint));
		fAshFactor = saturate( (fAshFactor-m) / Mm);		// smoothstep( m, M, fAshFactor);
		output.color.rgb = lerp( g_NoiseFadeColor2.rgb, output.color.rgb, fAshFactor);
		
	}
}