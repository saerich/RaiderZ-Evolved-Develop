//-----------------------------------------------------------------------------
// File: ScreenMotionBlur.fx
//
// Desc: ȭ�� ��ü�� ���� ��� �� ����. - 090619, OZ
// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch27.html
// http://aronze.egloos.com/1378393
// Waht a simple!!!!!!! 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global constants
//-----------------------------------------------------------------------------

float4x4 g_previousViewProjectionMatrix;
float4x4 g_ViewInverseMatrix;

//Screen-Quad�� View-���� ���� ��ġ ����
float3 g_vFarClipCornerInView;

int g_numSamples = 10;
//#define		g_numSamples	10

float g_fFarZ = 1000;
float g_fSpeed = 1;

float depthScale;

// ������ ����� ���� ����� ������ ����. ������ ��ǥ ���. 
// ��ũ�� ��ǥ�� ����� ������ ������ PS�� ������ ���� ����� �̷�� ���� ������ ���̴� �ܺο��� ����ϰ� ����� �Ѱ� �ִ� �� ȿ����.
float4 g_vEpicenter = float4( 0.5f, 0.5f, 0,0);

float g_fClrFactor = 0;

struct PSOUTPUT {
	float4 color : COLOR0;
	float4 dof : COLOR1;
};
	
//-----------------------------------------------------------------------------
// Texture samplers
//-----------------------------------------------------------------------------
sampler s0 : register(s0);		// texture SceneColor;
sampler s1 : register(s1);		// texture Depth;



float4 ReconstructPosFromDepth(float2 vScreencood, float fDepth)
{
	float3 vRay;
	vRay.xy = lerp(-g_vFarClipCornerInView.xy, g_vFarClipCornerInView.xy, vScreencood);
	vRay.z = g_vFarClipCornerInView.z;
	return float4(vRay*fDepth, 1);
}

    

//-----------------------------------------------------------------------------
// Type: Pixel shader                                      
// Desc: Do Blur!!!!!
//-----------------------------------------------------------------------------
float4 screen_motion_blur_basis_cam_move_ps( in float2 texCoord : TEXCOORD0 ) : COLOR
{
	float4 o;
	
	float2 orgCoord = texCoord;
		
	// Get the depth buffer value at this pixel. 
	// ���� ���ۿ� �븻 z�� ��ȣ�� ���������Ƿ� abs�� �̸� ��������� �Ѵ�.
	float4 zOver = abs(tex2D( s1, texCoord));
	// ������Ʈ�� �н��� ���� ���ۿ��� 0~1���� ��� �Ǿ� �ִ�.
	float zOverW = zOver.x;
	
	// H is the viewport position at this pixel in the range -1 to 1.   
	float4 H = float4(texCoord.x * 2 - 1, (1 - texCoord.y) * 2 - 1, zOverW, 1);   

	float3 viewPos = ReconstructPosFromDepth(texCoord, zOverW);
	float4 worldPos = mul(float4(viewPos,1), g_ViewInverseMatrix);

	// Current viewport position   
	float4 currentPos = H;   
	// Use the world position, and transform by the previous view-   
	// projection matrix.   
	float4 previousPos = mul(worldPos, g_previousViewProjectionMatrix);   
	// Convert to nonhomogeneous points [-1,1] by dividing by w.   
	previousPos /= previousPos.w;   
	// Use this frame's position and last frame's to compute the pixel   
	// velocity.   
	float2 velocity = (currentPos - previousPos)/2.f;  
	
	// �ӷ��� y���� ������ ��� �Ѵ�. �� �׷��� �Ф� �޼� ��ǥ�谡 �ƴ϶� �� �̻��Ѱ�����. try&err�� ��Ƴ´� �Ф� ���̻��...
	// x���� �������� ���� ������ �ݴ� ����? �� ���..
	velocity.x = -velocity.x;
	
	// %���� ���� ũ�� ������ ��´�. 15�� ��������.
	float loopCnt = (float)g_numSamples % 15;
	
	// ���̸� �����.
	velocity /= loopCnt;
	velocity *= g_fSpeed;

	// Get the initial color at this pixel.   
	float4 color = tex2D( s0, texCoord);   // sceneSampler
	texCoord += velocity; 
	float fFactorAssem = 1;
	for(float i = 1; i < loopCnt; ++i, texCoord += velocity)   
	{   
		// Sample the color buffer along the velocity vector.   
		float4 currentColor = tex2D( s0, texCoord);	// sceneSampler 
		// Add the current color to our color sum.
		float fFactor = 1 - i/loopCnt;
		color += currentColor * fFactor;	// ������ �������� Į�� ���͸� ���δ�.
		fFactorAssem += fFactor;
	}   
	// Average all of the samples to get the final blur color.   
	//float4 finalColor = color / fFactorAssem;	// �������� ������
	o = color / fFactorAssem;

	o.a = 1;
	
	return o;
}




//-----------------------------------------------------------------------------
// Name: ScreenMotionBlur
// Type: Technique                                     
// Desc: Do Blur!!!!!
//-----------------------------------------------------------------------------
technique ScreenMotionBlurBasisCamMove
{
	pass P0
	{
		PixelShader = compile ps_3_0 screen_motion_blur_basis_cam_move_ps();
	}
}







//-----------------------------------------------------------------------------
// Type: Pixel shader                                      
// Desc: Do Blur!!!!!
//-----------------------------------------------------------------------------
float4 screen_motion_blur_basis_epicenter_ps( in float2 texCoord : TEXCOORD0 ) : COLOR
{
//	PSOUTPUT o;
	float4 o;
	
	float2 orgCoord = texCoord;
	
	// �� �ؼ��� �ӵ��� �������κ����� �Ÿ��� ���� ����.
	// �������κ��� �ּ��� �ӵ��� �����ؾ��ϹǷ�. 
	float2 velocity = g_vEpicenter.xy - texCoord;
	
	// %���� ���� ũ�� ������ ��´�. 15�� ��������.
	float loopCnt = (float)g_numSamples % 15;
	
	// ���̸� �����.
	velocity /= loopCnt;
	velocity *= g_fSpeed;

	// Get the initial color at this pixel.   
	float4 color = tex2D( s0, texCoord);   // sceneSampler
	texCoord += velocity; 
	float fFactorAssem = 1;
	for(float i = 1; i < loopCnt; ++i, texCoord += velocity)   
	{   
		// Sample the color buffer along the velocity vector.   
		float4 currentColor = tex2D( s0, texCoord);	// sceneSampler 
		// Add the current color to our color sum.
		float fFactor = 1 - i/loopCnt;
		color += currentColor * fFactor;	// ������ �������� Į�� ���͸� ���δ�.
		fFactorAssem += fFactor;
	}   
	// Average all of the samples to get the final blur color.   
	//float4 finalColor = color / fFactorAssem;	// �������� ������
// 	o.color = color / fFactorAssem;
// 	o.color.a = 1;
// 	o.dof = tex2D( s1, texCoord);
// 	o.dof.a = 1;

	o = color / fFactorAssem;
	o.a = 1;
	
 	return o;
}




//-----------------------------------------------------------------------------
// Name: ScreenMotionBlur
// Type: Technique                                     
// Desc: Do Blur!!!!!
//-----------------------------------------------------------------------------
technique ScreenMotionBlurBasisEpicenter
{
	pass P0
	{
		PixelShader = compile ps_3_0 screen_motion_blur_basis_epicenter_ps();
	}
}
















//-----------------------------------------------------------------------------
// Type: Pixel shader                                      
// Desc: fill color for info depth max
//-----------------------------------------------------------------------------
float4 color_fill_ps( in float2 vTexCoord : TEXCOORD0 ) : COLOR
{
	return float4( g_fFarZ, 1, 1, 1);	
}

//-----------------------------------------------------------------------------
// Name: DepthBlurOverlay
// Type: Technique                                     
// Desc: fill color for info depth max
//-----------------------------------------------------------------------------
technique ColorFill
{
	pass P0
	{
		PixelShader = compile ps_2_0 color_fill_ps();
	}
}




//-----------------------------------------------------------------------------
// Type: Pixel shader                                      
// Desc: Helper shader for visualizing depth/blurriness
//-----------------------------------------------------------------------------
float4 depth_blur_overlay_ps( in float2 vTexCoord : TEXCOORD0 ) : COLOR
{
	float4 BlurDepth = tex2D( s0, vTexCoord);

	//return float4( BlurDepth.x/BlurDepth.y, BlurDepth.x, BlurDepth.y, 1);	
	return float4( 1, 0, 1, 1);;
}

//-----------------------------------------------------------------------------
// Name: DepthBlurOverlay
// Type: Technique                                     
// Desc: Helper shader for visualizing depth/blurriness
//-----------------------------------------------------------------------------
technique DepthOverlay
{
	pass P0
	{
		PixelShader = compile ps_3_0 depth_blur_overlay_ps();
	}
}
