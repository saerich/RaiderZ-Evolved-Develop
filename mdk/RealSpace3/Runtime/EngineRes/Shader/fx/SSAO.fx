//-----------------------------------------------------------------------------
// File: SSAO.fx
//
// Desc: Screen Space Ambient Occlusion
// ���� ���� ����
//  - http://wiki.gamedev.net/index.php/D3DBook:Screen_Space_Ambient_Occlusion
//  - http://kyruie.tistory.com/31
// ��Ŭ���� ���
//  - shaderx7 6.1
// halo removal : ���� ����. �ð��� �� �ΰ� ����
// - http://www.gamedev.net/community/forums/topic.asp?topic_id=550699
// - �� ����� ��� �ϸ� blur �н��� �ʿ� ���������...
// - �׷��� �������� SSAO���ٴ� �������ٴµ�, ���� blur�н��� ��ġ�°� ������..
//-----------------------------------------------------------------------------















#define NUM_SAMPLE_NUM 16


// x = RT width
// y = RT height
// z = near
// w = far
float4		vViewDimensions;

// x = default occlusion value
// y = kernel sphere radius 
// z = multiple of kernel radius to use for range truncation
// w = delta scale multiplier
//float4		vSSAOParams = float4( 0.5f, 40.0f, 4.0f, 200.0f );
float4		vSSAOParams = float4( 0.0f, 30.0f, 4.0f, 200.0f );



// ���� ���ۿ� ���� �ִ� ���� 0~1������ z/w���̴�. �̸� ���� ���� ������ ��ȯ�ϱ� ���ؼ��� ���������� �ι��� ��Ʈ������ �ʿ��ϴ�.
// ������ ���̸� �ʿ��ϰ� x,y�� �ʿ䰡 �����Ƿ� 3,4���� ������ ����
//matrix	g_matProjInv;
float4 g_vProjInvZ;
float4 g_vProjInvW;


// for Gaussian ---------------------------
// Contains sampling offsets used by the techniques
float2	g_avSampleOffsets[13];
float4	g_avSampleWeights[13];
int		g_nSampleCount = 13;


#define NUM_GAUSS	6
float	g_fWeight[NUM_GAUSS];

float	g_fBright = 0.5f;



sampler DepthSampler	: register(s0);
sampler RandomSampler	: register(s1);
sampler AOSampler		: register(s2);	// AO ���� �ô� ����ũ�� ���
sampler NormalSampler	: register(s3);




float4 finalColor( in float fValue)  
{  
	float sample = fValue;
	
 	sample = saturate( fValue + 0.05f);
   	//sample = ( pow( sample, 4) /*+ sample*/);
   	//sample = /*saturate*/( sample * 2);
   	//sample = saturate( pow( sample, 8));

 	// ��ο� ������ �߷�����..
	// ���������� ��⸦ ���̰�
	//sample = sample * 2;
	//sample = saturate( sample);

	// ���������� ��� ���̰�
	sample = pow( sample, 2);
	// ���� �̻� ���� ����������
 	//sample = smoothstep( 0, 0.5f, sample);
	// 0 ~ 1���� �Ǿ��� ������ 0.5 �̻��� ���ư��������Ƿ�
	// �ٽ� 0.5������ ����� ����
	//sample = sample / 2;



	
	return float4( sample, sample, sample, 1);
}  


float3 finalNormal( in float3 norm)
{
	norm.z = -sqrt(1.0f - norm.x*norm.x - norm.y*norm.y);
	return norm;
}


//-----------------------------------------------------------------------------
float readDepth( in float2 coord)  
{  
	// ����ũ�� ����. ���̰� 0�� �Ǹ� AO�� �߻� ���ϰ���..
	//float fMaskValue = saturate( 1 - tex2D( AOSampler, coord ).a) * vViewDimensions.w * 3;
	
	// ���ø� �� ���� ���̸� ����. 
	// fDepthValue : 0~1. ���Ͼ� �ƴ�.
	float fDepthValue = abs(tex2D( DepthSampler, coord ).r);
	float4 vDepthValue = float4( 0,0,fDepthValue,1);
	
	// �� �������� �ι��� ��Ʈ������ ���Ͽ� ���� ����� w�� ���� �Ͱ� ���� ȿ��
	return (dot( vDepthValue, g_vProjInvZ) / dot( vDepthValue, g_vProjInvW));
}   



// shaderX 7�� �ҽ��� ��Ŭ���� ���ϴ� ���� �κи� ��Ű ������ ����
float4 PSmainCRYTECKadjustGDWIKI( in float2 screenTC : TEXCOORD0 ) : COLOR
{
	const float3 pSphere[10] = {
		float3(-0.010735935, 0.01647018, 0.0062425877),
		float3(-0.06533369, 0.3647007, -0.13746321),
		float3(-0.6539235, -0.016726388, -0.53000957),
		float3(0.40958285, 0.0052428036, -0.5591124),
		float3(-0.1465366, 0.09899267, 0.15571679),
		float3(-0.44122112, -0.5458797, 0.04912532),
		float3(0.03755566, -0.10961345, -0.33040273),
		float3(0.019100213, 0.29652783, 0.066237666),
		float3(0.8765323, 0.011236004, 0.28265962),
		float3(0.29264435, -0.40794238, 0.15964167)
	};

 

	float2 screenSize = vViewDimensions.xy;
	// �� Ŭ���� �� �Ÿ� �� ����
	float farClipDist = vViewDimensions.w;	// - vViewDimensions.z;
	
	// �����̼� ���� ����. 
	float2 rotationTC = screenTC * 18;


	rotationTC = screenTC * screenSize / 4;


	float3 vRotation = 2 * tex2D( RandomSampler, rotationTC).rgb - 1;

		
	// ���� �ȼ��� ���� ����. �Ǵ���
	float fPixelDepth = readDepth( screenTC);
	float3 vPixelNorm = finalNormal( tex2D( NormalSampler, screenTC).xyz);
	//float fPixelMask = tex2D( AOSampler, screenTC).a;
	//bool bNoSelfAO = ((0.1f < fPixelMask) && (fPixelMask < 0.9f));
	float fRadius = vSSAOParams.y; 
	float fFullDepth = vViewDimensions.w - vViewDimensions.z;
	float fFar = vViewDimensions.w;
	float fRadiusTruncation = vSSAOParams.z;
	float fDefaultValue = vSSAOParams.x;

	// vSSAOParams
	// x = default occlusion value
	// y = kernel sphere radius 
	// z = multiple of kernel radius to use for range truncation
	// w = delta scale multiplier


	float3 vKernelScale = float3( fRadius / fPixelDepth, fRadius / fPixelDepth, fRadius) ;
	
		
	float Accessibility = 0;
	float AccessibilityTmp = 0;
	float SamCnt = 0.0000001f;	// devide by zero �ȳ�����
	//int nSelfAOCnt = 0;

	float fDotAssum = 0;

	//float fAOIgnoreValue = lerp( saturate( vRotatedKernel.z), vSSAOParams.x, saturate( vRotatedKernel.z / ( vSSAOParams.y * vSSAOParams.z ) ) );
	
	// ���� ������ Accessibility ����
	for( int i=0; i<10; i++)
	{
		// generate offset vector ( this code line is executed only at shader compile stage)
		// ť�� �ڳʸ� ��� �׸��� ���� �ٸ� ���̸� �ο�
		float3 vOffset = pSphere[i];
		// ������ ���͸� ������Ʈ�� ����.
		float3 vRotatedKernel = reflect( vOffset, vRotation) * vKernelScale;
		float2 vSamTC = vRotatedKernel.xy + screenTC.xy;
 		float fSampleDepth = readDepth( vSamTC);
		float3 vSampleNorm = finalNormal( tex2D( NormalSampler, vSamTC).xyz);
 		// AO�� �߻� ��Ű���� �ƴ����� ����ũ ����
 		//float fSampleMask = tex2D( AOSampler, vSamTC).a;
 		
//  		if( fSampleMask == 0)
// 		{
// 			// AO�� ���� �ʴ� ���� ��� �� ���� ���� �����ν� AO�� �ߵ� ���ϰ�
// 			//fSampleDepth = fPixelDepth;
// 		}
// 		else
 		{
			// ���� ���� ���·� ����� ��������
			float fDelta = max( fSampleDepth - fPixelDepth + vRotatedKernel.z, 0.0f );
			float fRange = abs( fDelta ) / ( vKernelScale.z * fRadiusTruncation );
			float fDot = dot( vSampleNorm, vPixelNorm);

			// ������ �����ؼ� ��� ���ø��� �븻�� ���� ��ġ�ϸ� �̳��� ��Ȱ�� ��鿡 �ִ� ���̹Ƿ� AO��...
			fDotAssum += fDot;

			// ���� ���� ���� �̻��� ��츸 ���� �ǰ�....
			fDot = smoothstep( 0.95f, 1, fDot);
			float fFactor = saturate( 1 - fDot);
			//fFactor = pow( fFactor, 2);

			fFactor = 1;

	
			Accessibility += lerp( fDefaultValue, lerp( saturate( fDelta), fDefaultValue, saturate( fRange ) ), fFactor);
			SamCnt++;
		}

		// ���� AO�� ���� �ʴ� ���� .. ���? ������ �Ȱ���.. �Ѥ�? ���� ������ �ڵ带 ����ȭ �غ���..
		// ���� ���� ���� �� Self �� ����
		// �븻 ���� ������ �����ִ� ��쿡�� �ͷ����� ���� �� ������ �߻����� ���� ���̴�.
		// �ֳĸ� �������� �븻�� �����̹Ƿ�... 
// 		if( 0.1f < fSampleMask && fSampleMask < 0.9f )
// 		{
// 			// notthing
// 			nSelfAOCnt++;
// 			AccessibilityTmp += fDefaultValue;
// 		}

	}



	// ��� ��
	Accessibility = Accessibility / SamCnt;
	Accessibility = 1 - Accessibility;



// 	if( bNoSelfAO && (nSelfAOCnt==NUM_SAMPLE_NUM))
// 	{
// 		Accessibility = 1 - (AccessibilityTmp / NUM_SAMPLE_NUM);
// 	}



	float4 vOut = finalColor( Accessibility);



	// ���� ������ ���
	float fDotAverage = saturate( fDotAssum / SamCnt);
	// 1�� ����� �� ���� -> ���� : 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 1
	//fDotAverage = smoothstep( 0.95f, 1, fDotAverage);
	fDotAverage = pow( fDotAverage, 3);
	//fDotAverage = 1 - fDotAverage;
	// ���� �ݿ�
	// 0 : ����̹Ƿ� �⺻ �� �ݿ�
	// ~ 
	// 1 : �������
	return lerp( vOut, float4(1,1,1,1), fDotAverage);

	
}


technique Tmain
{
    pass P0
    {
		PixelShader  = compile ps_3_0 PSmainCRYTECKadjustGDWIKI();
    }
}
//-----------------------------------------------------------------------------



// --------------------- denoise -------------------------------------------

float4 Blur4x4PS
    (
    in float2 vScreenPosition : TEXCOORD0
    ) : COLOR
{
	
	float fFactorX = 1/vViewDimensions.x;
	float fFactorY = 1/vViewDimensions.y;

	vScreenPosition += float2( 0.5f * fFactorX, 0.5f * fFactorY);
	
    float sample = 0.0f;

	for( int x=0; x<4; x++)
	{
		for( int y=0; y<4; y++)
		{	
			//float2 vSamplePostion = vScreenPosition + float2( (x-0.5f)/vViewDimensions.x, (y-0.5f)/vViewDimensions.y);
			float2 vSamplePostion = vScreenPosition + float2( (x-2)*fFactorX, (y-2)*fFactorY);

			sample += tex2D( AOSampler, vSamplePostion).r;
		}
	}

	sample = sample / 16.0f;


	return float4( sample, sample, sample, 1);

}


technique Blur4x4
{
    pass P0
    {
        PixelShader  = compile ps_3_0 Blur4x4PS();
    }
}

//-----------------------------------------------------------------------------





// --------------------- Blur4x4BaiasEdge -------------------------------------------

float4 Blur4x4BaiasEdgePS
    (
    in float2 vScreenPosition : TEXCOORD0
    ) : COLOR
{
	
	float fFactorX = 1/vViewDimensions.x;
	float fFactorY = 1/vViewDimensions.y;

	vScreenPosition += float2( 0.5f * fFactorX, 0.5f * fFactorY);
	
    float sample = 0.0f;
	float cnt = 0.0000001f;	// devide by zero ����

	for( int x=0; x<4; x++)
	{
		for( int y=0; y<4; y++)
		{	
			//float2 vSamplePostion = vScreenPosition + float2( (x-0.5f)/vViewDimensions.x, (y-0.5f)/vViewDimensions.y);
			float2 vSamplePostion = vScreenPosition + float2( (x-2)*fFactorX, (y-2)*fFactorY);

			// ���� ���÷��� �ε����� ������ ����
			if( tex2D( RandomSampler, vSamplePostion).r < 0.5f)
			{
				sample += tex2D( AOSampler, vSamplePostion).r;
				cnt = cnt + 1;
			}
		}
	}

	sample = sample / cnt;


	//TESTOZ
	//sample = tex2D( DepthSampler, vScreenPosition).r;
	//sample = tex2D( AOSampler, vScreenPosition).r;


	return float4( sample, sample, sample, 1);

}


technique Blur4x4BaiasEdge
{
    pass P0
    {
        PixelShader  = compile ps_3_0 Blur4x4BaiasEdgePS();
    }
}

//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// ��ο� ������ �� �ȼ��� Ȯ��
float4 PSexpandDarkforce( in float2 vTexCoord : TEXCOORD0 ) : COLOR
{
	float fFactorX = 1/vViewDimensions.x;
	float fFactorY = 1/vViewDimensions.y;
	
	float fS = 1;
	float fV = 1;
	
	// ��
	fS = tex2D( AOSampler, vTexCoord).g;
	fV = min( fS, fV);
	
	// ��
	fS = tex2D( AOSampler, vTexCoord + float2( 0, -fFactorY)).r;
	fV = min( fS, fV);
	// ��
	fS = tex2D( AOSampler, vTexCoord + float2( 0,  fFactorY)).r;
	fV = min( fS, fV);
	// ��
	fS = tex2D( AOSampler, vTexCoord + float2( -fFactorX, 0)).r;
	fV = min( fS, fV);
	// ��
	fS = tex2D( AOSampler, vTexCoord + float2(  fFactorX, 0)).r;
	fV = min( fS, fV);
	
	
	// �»�
	fS = tex2D( AOSampler, vTexCoord + float2( -fFactorX, -fFactorY)).r;
	fV = min( fS, fV);
	// ����
	fS = tex2D( AOSampler, vTexCoord + float2( -fFactorX,  fFactorY)).r;
	fV = min( fS, fV);
	// ���
	fS = tex2D( AOSampler, vTexCoord + float2( fFactorX, -fFactorY)).r;
	fV = min( fS, fV);
	// ����
	fS = tex2D( AOSampler, vTexCoord + float2( fFactorX, fFactorY)).r;
	fV = min( fS, fV);

	
	
	return float4( fV, fV, fV, 1);
}

technique TexpandDarkforce
{
	pass P0
	{
		PixelShader = compile ps_3_0 PSexpandDarkforce();
	}
}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// ���� ������ �� �ȼ��� Ȯ��
// ������ Ÿ��Ʈ�ϰ� ���� �ϱ� ���� ���� ���� Ȯ�� �ϴ� ���� ���� �κ� �������� Ȯ���Ͽ��� �Ѵ�,
float4 PSexpandBrightforce( in float2 vTexCoord : TEXCOORD0 ) : COLOR
{
	float fFactorX = 1/vViewDimensions.x;
	float fFactorY = 1/vViewDimensions.y;
	
	float fS = 0;
	float fV = 0;
	
	// ��
	fS = tex2D( DepthSampler, vTexCoord).r;
	fV = max( fS, fV);
	
	// ��
	fS = tex2D( DepthSampler, vTexCoord + float2( 0, -fFactorY)).r;
	fV = max( fS, fV);
	// ��
	fS = tex2D( DepthSampler, vTexCoord + float2( 0,  fFactorY)).r;
	fV = max( fS, fV);
	// ��
	fS = tex2D( DepthSampler, vTexCoord + float2( -fFactorX, 0)).r;
	fV = max( fS, fV);
	// ��
	fS = tex2D( DepthSampler, vTexCoord + float2(  fFactorX, 0)).r;
	fV = max( fS, fV);
	
	
	// �»�
	fS = tex2D( DepthSampler, vTexCoord + float2( -fFactorX, -fFactorY)).r;
	fV = max( fS, fV);
	// ����
	fS = tex2D( DepthSampler, vTexCoord + float2( -fFactorX,  fFactorY)).r;
	fV = max( fS, fV);
	// ���
	fS = tex2D( DepthSampler, vTexCoord + float2( fFactorX, -fFactorY)).r;
	fV = max( fS, fV);
	// ����
	fS = tex2D( DepthSampler, vTexCoord + float2( fFactorX, fFactorY)).r;
	fV = max( fS, fV);

	
	
	return float4( fV, fV, fV, 1);
}

technique TexpandBrightforce
{
	pass P0
	{
		PixelShader = compile ps_3_0 PSexpandBrightforce();
	}
}
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
float4 PScopy( in float2 vTexCoord : TEXCOORD0 ) : COLOR
{
	float fVal = tex2D( AOSampler, vTexCoord).r;
	return float4( fVal, fVal, fVal, 1);
}

technique Tcopy
{
	pass P0
	{
		PixelShader = compile ps_3_0 PScopy();
		//PixelShader = compile ps_3_0 PSexpandDarkforce();
	}
}
//-----------------------------------------------------------------------------







//-----------------------------------------------------------------------------

float4 GaussBlurPS
    (
    in float2 vScreenPosition : TEXCOORD0
    ) : COLOR
{
	
    float sample = 0.0f;

	//float loopCnt = (float)g_nSampleCount % 15;
	
	//for( int i=0; i < loopCnt; i++ )
	for( int i=0; i < 5; i++ )
	{
		sample += g_avSampleWeights[i] * tex2D( AOSampler, vScreenPosition + g_avSampleOffsets[i]).r;
	}

	return float4( sample, sample, sample, 1);
	
}


technique GaussBlur
{
    pass P0
    {
        PixelShader  = compile ps_3_0 GaussBlurPS();
    }
}



//-----------------------------------------------------------------------------
float4 GaussBlurPSX( in float2 vTexCoord : TEXCOORD0 ) : COLOR
{   
	float weightFactorN[NUM_GAUSS];
	float weightFactorP[NUM_GAUSS];

	int i;
	for( i=0; i<NUM_GAUSS; i++)
	{
		// ���ø� �� ��ġ ����
		weightFactorN[ i] = -i / vViewDimensions.x;
		//weightFactorP[ NUM_GAUSS - (i+1)] = i / vViewDimensions.x; ????
		weightFactorP[ i] = i / vViewDimensions.x;
	}


    float Color = 0;
	bool bIgnoreN = false; 
	bool bIgnoreP = false;
    float fWeightSum = 0.0000001f;	// devide by zero ������

	float2 vTexN, vTexP;
    for( i=0; i<NUM_GAUSS; i++)
    {
		// ���ø� ��� ��ġ ����
		vTexN = vTexCoord + float2( weightFactorN[i], 0 );
		vTexP = vTexCoord + float2( weightFactorP[i], 0 );

		// ����ŷ Ȯ��
		if( i > 0)
		{
			if(!bIgnoreN)
				bIgnoreN = ( tex2D( DepthSampler, vTexN).r > 0.9f) ? true : false;
			if(!bIgnoreP)
				bIgnoreP = ( tex2D( DepthSampler, vTexP).r > 0.9f) ? true : false;
		}

		// �÷� ����
		if(bIgnoreN && bIgnoreP)
		{
			break;
		}
		if(!bIgnoreN)
		{
			Color += g_fWeight[i] * tex2D( AOSampler, vTexN).r;
			fWeightSum += g_fWeight[i];
		}
		if(!bIgnoreP)
		{
			Color += g_fWeight[i] * tex2D( AOSampler, vTexP).r;
			fWeightSum += g_fWeight[i];
		}
    }
	Color = Color / fWeightSum;

	// TESTOZ
	//Color = tex2D( AOSampler, vTexCoord).r;
   
    return float4( Color, Color, Color, 1);
}


technique GaussBlurX
{
    pass P0
    {
        PixelShader  = compile ps_3_0 GaussBlurPSX();
    }
}


float4 GaussBlurPSY( in float2 vTexCoord : TEXCOORD0 ) : COLOR
{  
	float weightFactorN[NUM_GAUSS];
	float weightFactorP[NUM_GAUSS];

	int i;
	for( i=0; i<NUM_GAUSS; i++)
	{
		weightFactorN[ i] = -i / vViewDimensions.y;
		//weightFactorP[ NUM_GAUSS - (i+1)] = i / vViewDimensions.y;  ????
		weightFactorP[ i] = i / vViewDimensions.y;
	}

	float Color = 0;
	bool bIgnoreN = false; 
	bool bIgnoreP = false;
    float fWeightSum = 0.0000001f;	// devide by zero ������
    
	float2 vTexN, vTexP;
    for( i=0; i<NUM_GAUSS; i++)
    {
		// ���ø� ��� ��ġ ����
		vTexN = vTexCoord + float2( 0, weightFactorN[i]);
		vTexP = vTexCoord + float2( 0, weightFactorP[i]);

		// ����ŷ Ȯ��
		if( i > 0)
		{
			if(!bIgnoreN)
				bIgnoreN = ( tex2D( DepthSampler, vTexN).r > 0.9f) ? true : false;
			if(!bIgnoreP)
				bIgnoreP = ( tex2D( DepthSampler, vTexP).r > 0.9f) ? true : false;
		}

		// �÷� ����
		if(bIgnoreN && bIgnoreP)
		{
			break;
		}
		if(!bIgnoreN)
		{
			Color += g_fWeight[i] * tex2D( AOSampler, vTexN).r;
			fWeightSum += g_fWeight[i];
		}
		if(!bIgnoreP)
		{
			Color += g_fWeight[i] * tex2D( AOSampler, vTexP).r;
			fWeightSum += g_fWeight[i];
		}

    }
	Color = Color / fWeightSum;

	// TESTOZ
	//Color = tex2D( AOSampler, vTexCoord).r;
    
    //return float4( Color, Color, Color, 1);
	// �� �ֵ� �ܰ迡�� ���� �����Ѵ�. ���� -> ���� �ƴ� �� -> ���� �ǰԤ�,
	return finalColor( Color);
}

technique GaussBlurY
{
    pass P0
    {
        PixelShader  = compile ps_3_0 GaussBlurPSY();
    }
}

//-----------------------------------------------------------------------------






