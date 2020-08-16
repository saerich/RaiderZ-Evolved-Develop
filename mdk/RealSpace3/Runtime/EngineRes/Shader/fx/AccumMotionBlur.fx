//-----------------------------------------------------------------------------
// File: ScreenMotionBlur.fx
//
// Desc: ȭ�� ��ü�� ���� ��� �� ����. - 090619, OZ
// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch27.html
// http://aronze.egloos.com/1378393
// Waht a simple!!!!!!! 
//-----------------------------------------------------------------------------
	
//-----------------------------------------------------------------------------
// Texture samplers
//-----------------------------------------------------------------------------
sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);


float g_fDecreaseAlpha = 0.1f;

// ------------------------------------------------------------
// �ȼ����̴�
// ------------------------------------------------------------
float4 PS_Accum( in float2 vTexCoord : TEXCOORD0 ) : COLOR
{   
	float4 Out = 0;

	
	
	// ���� ����
	float4 C0 = tex2D( s0, vTexCoord);
	// �߰� ��� ����ŷ
	float4 C1 = tex2D( s1, vTexCoord);	
	// �߰� ��� ���� : �÷�����
	float4 C2 = tex2D( s2, vTexCoord);
	
 	// �߰� ����� ���Ĵ� ���� �׷��� ������ 1, �ƴϸ� 0
 	// �н� ���� �������� ������� �׸��Ƿ� rgb�� ��� �ϳ��� �Ӽ��� �״�� ���ķ� ��� �ص� ��
 	C2.a = sign( C1.r);
	C2.a = max( C2.a, 0);

	// ���� ������ ���� ����
	C0.a -= g_fDecreaseAlpha;
	C0.a = max( C0.a, 0);
	
	// ���� ����� ���� ��� ���� ���� ������ ����	
	Out.xyz = ( C0 * C0.a)
			+ ( C2 * (1-C0.a));
	
	Out.a = max( C0.a, C2.a);
	Out.a = min( Out.a, 1);
	
	return Out;
	
}
// ------------------------------------------------------------
// ��ũ��
// ------------------------------------------------------------
technique Accum
{
    pass P0
    {
        PixelShader  = compile ps_2_0 PS_Accum();
    }
}



// ------------------------------------------------------------
// �ȼ����̴�
// ------------------------------------------------------------
float4 PS_Blur( in float2 vTexCoord : TEXCOORD0 ) : COLOR
{   
	float4 C0 = tex2D( s0, vTexCoord);	// ���� ���
	float4 C1 = tex2D( s1, vTexCoord);	// ���� �ҽ�
	
	float4 Out = 0;
	Out.xyz = ( C1.xyz * C1.a) + ( C0.xyz * (1-C1.a));
	Out.a = 1;
	
	return Out;
}
// ------------------------------------------------------------
// ��ũ��
// ------------------------------------------------------------
technique Blur
{
    pass P0
    {
        PixelShader  = compile ps_2_0 PS_Blur();
    }
}



// ------------------------------------------------------------
// �ȼ����̴�
// ------------------------------------------------------------
float4 PS_MaskInfo( in float2 vTexCoord : TEXCOORD0 ) : COLOR
{   
	float4 C0 = tex2D( s0, vTexCoord);	// ��������
	
	float4 Out = float4( C0.r, C0.g, C0.b, 1);
	
	
	Out = C0;
	
//	Out.x = max( max( Out.x, Out.y), Out.z);
//	Out.y = Out.a;
	Out.a = 1;
	
	return Out;
}
// ------------------------------------------------------------
// ��ũ��
// ------------------------------------------------------------
technique MaskInfo
{
    pass P0
    {
        PixelShader  = compile ps_2_0 PS_MaskInfo();
    }
}



// ------------------------------------------------------------
// �ȼ����̴�
// ------------------------------------------------------------
float4 PS_Overlay( in float2 vTexCoord : TEXCOORD0 ) : COLOR
{   
	float4 C0 = tex2D( s0, vTexCoord);	// ��������
	
	float4 Out = float4( C0.a, C0.a, C0.a, 1);
	
	
	Out = C0;
	
//	Out.x = max( max( Out.x, Out.y), Out.z);
//	Out.y = Out.a;
	Out.a = 1;
	
	return Out;
}
// ------------------------------------------------------------
// ��ũ��
// ------------------------------------------------------------
technique Overlay
{
    pass P0
    {
        PixelShader  = compile ps_2_0 PS_Overlay();
    }
}
