//-----------------------------------------------------------------------------
// File: EdgeDetect.fx
//
// Desc: ���� ����
//-----------------------------------------------------------------------------



const float maskLaplacian[9] = 
{ -1, -1, -1, 
  -1,  8, -1, 
  -1, -1, -1 }; // Laplacian����



const float coord[3] = { -1, 0, +1 };


// x = RT width
// y = RT height
// z = �ܰ��� ���� ���� 
// w = far
float4		vViewDimensions;


// ���� ���ۿ� ���� �ִ� ���� 0~1������ z/w���̴�. �̸� ���� ���� ������ ��ȯ�ϱ� ���ؼ��� ���������� �ι��� ��Ʈ������ �ʿ��ϴ�.
// ������ ���̸� �ʿ��ϰ� x,y�� �ʿ䰡 �����Ƿ� 3,4���� ������ ����
//matrix	g_matProjInv;
float4 g_vProjInvZ;
float4 g_vProjInvW;

// x = default occlusion value
// y = kernel sphere radius 
// z = multiple of kernel radius to use for range truncation
// w = delta scale multiplier
float4		vSSAOParams = float4( 0.5f, 40.0f, 4.0f, 200.0f );



sampler DepthSampler	: register(s0);
sampler RandomSampler	: register(s1);
sampler AOSampler		: register(s2);	// AO ���� �ô� ����ũ�� ���
sampler NormalSampler	: register(s3);



float4 finalColor( in float fValue)  
{  
	float sample = fValue;
	
	return float4( sample, sample, sample, sample);
}  


float3 finalNormal( in float3 norm)
{
	norm.z = sqrt(1.0f - norm.x*norm.x - norm.y*norm.y);
	return norm;
}


//-----------------------------------------------------------------------------
float readDepth( in float2 coord)  
{  
	// ����ũ�� ����. ���̰� 0�� �Ǹ� AO�� �߻� ���ϰ���..
	//float fMaskValue = saturate( 1 - tex2D( AOSampler, coord ).a) * vViewDimensions.w * 3;
	
	// ���ø� �� ���� ���̸� ����. 
	// fDepthValue : 0~1. ���Ͼ� �ƴ�.
	float fDepthValue = tex2D( DepthSampler, coord ).r;
	float4 vDepthValue = float4( 0,0,fDepthValue,1);
	
	// �� �������� �ι��� ��Ʈ������ ���Ͽ� ���� ����� w�� ���� �Ͱ� ���� ȿ��
	//return (dot( vDepthValue, g_vProjInvZ) / dot( vDepthValue, g_vProjInvW));
	return -(dot( vDepthValue, g_vProjInvZ) / dot( vDepthValue, g_vProjInvW));
}   



// shaderX 7�� �ҽ��� ��Ŭ���� ���ϴ� ���� �κи� ��Ű ������ ����
float4 PSLaplacian( in float2 Tex : TEXCOORD0 ) : COLOR
{
	float Color = 0;
	float fDepthLimit = 40;

	// ���ö� ��������    
    for( int i = 0 ; i < 9 ; i++ )
		Color += maskLaplacian[i] * readDepth(Tex + float2( coord[i%3]/vViewDimensions.x, coord[i/3]/vViewDimensions.y)) / vViewDimensions.w * vSSAOParams.y * vSSAOParams.z;


	// ����� �������� �ܰ����� ���ϴ� ���¼��� �����̶� ������ �� ������� �ٲپ� ������.
	float fBias = readDepth( Tex) / vViewDimensions.w;
	Color = smoothstep( fBias/2, fBias, Color);


	//Color = Color - 0.05f;

	//Color = smoothstep();

	//Color = saturate(Color);

	// �÷��� Gray�� �ٲ۴�
	Color = float4( Color, Color, Color, 1 );

    return Color;

}



technique TLaplacian
{
    pass P0
    {
		PixelShader  = compile ps_3_0 PSLaplacian();
    }
}


