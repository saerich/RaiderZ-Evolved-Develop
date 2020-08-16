//Sampler
sampler BakedMap : register(s0);
sampler LightBufferMap : register(s11);

/// One-pass ���÷��� �� ���
sampler MaskMap0 : register(s0);
sampler MaskMap1 : register(s1);

sampler TileMap0 : register(s2);
sampler TileMap1 : register(s3);
sampler TileMap2 : register(s4);
sampler TileMap3 : register(s5);
sampler TileMap4 : register(s6);
sampler TileMap5 : register(s7);
sampler TileMap6 : register(s8);

sampler NormalMap0 : register(s9);
sampler NormalMap1 : register(s10);
sampler NormalMap2 : register(s11);
sampler NormalMap3 : register(s12);
sampler NormalMap4 : register(s13);
sampler NormalMap5 : register(s14);
sampler NormalMap6 : register(s15);

shared matrix	g_matWorld;
shared matrix	g_matWorldView;
shared matrix	g_matWorldViewProj;
shared float	g_fFarDist;
shared matrix  	g_textureTransform;
shared float4 	g_tileTextureScaleAndTranslate;
shared float4 	g_alphaTextureScaleAndTranslate;
shared float2  	g_Resolution;
shared float4	g_vFogFactor;   
shared float3	g_vFogColor;
shared float	g_fMarkingLayer;

/// One-pass ���÷��� �� ���
shared float4 	g_tileTextureScaleAndTranslateArray[7];

#define MAX_LAYER_PER_PATCH	7

#if LAYER_COUNT_7 == 1
 #define LAYER_COUNT 7
#elif LAYER_COUNT_6 == 1
 #define LAYER_COUNT 6
#elif LAYER_COUNT_5 == 1
 #define LAYER_COUNT 5
#elif LAYER_COUNT_4 == 1
 #define LAYER_COUNT 4
#elif LAYER_COUNT_3 == 1
 #define LAYER_COUNT 3
#elif LAYER_COUNT_2 == 1
 #define LAYER_COUNT 2
#else
 #define LAYER_COUNT 1
#endif

struct VSINPUT
{
	float4 vPosition		: POSITION;
	float3 vNormal			: NORMAL;
	float2 vDiffuseCoord	: TEXCOORD0;
};

struct VSOUTPUT
{
	float4 vPosition		: POSITION;
	float4 vNormal			: TEXCOORD0;		// ���״� normal�� w�� �����ִ´�.
	float3 vPosInView		: TEXCOORD1;
#if SPLATTING_ONEPASS == 1
	float4 vDiffuseCoord01	: TEXCOORD2;		//float2 vDiffuseCoord1	: TEXCOORD3;	// BAKED������ ����
#if LAYER_COUNT > 2
	float4 vDiffuseCoord23	: TEXCOORD3;		//float2 vDiffuseCoord3	: TEXCOORD5;
#endif
#if LAYER_COUNT > 4
	float4 vDiffuseCoord45	: TEXCOORD4;		//float2 vDiffuseCoord5	: TEXCOORD7;
#endif
#if LAYER_COUNT > 6
	float2 vDiffuseCoord67	: TEXCOORD5;		//float2 vDiffuseCoord7	: TEXCOORD9;
#endif
	float2 vAlphaCoord 		: TEXCOORD6;
#else	// SPLATTING_ONEPASS
	float4 vDiffuseCoord01	: TEXCOORD2;		//float2 vDiffuseCoord1	: TEXCOORD3;	// BAKED������ ����
#endif	// SPLATTING_ONEPASS
	/// TEXCOORD�� �ε����� �������� ���� ����. SPLATTING_ONEPASS ���� USE_NORMALMAP�� Ȱ�� �Ǵ� ���� �����Ƿ� �̸� ������.
#if USE_NORMALMAP == 1
	float3 vWPos			: TEXCOORD7;
	float3 vWorldNormal	: TEXCOORD8;
#endif	// USE_NORMALMAP
};



void SetTexCoord( inout VSOUTPUT output, in float4 vWorldPosition)
{
	output.vDiffuseCoord01.xy = mul(vWorldPosition ,g_textureTransform).xy ;	
}

void SetTexCoordSplattingOnepass( inout VSOUTPUT output, in float4 vWorldPosition, in VSINPUT input, bool bUseNormalmap)
{
#if	SPLATTING_ONEPASS == 1
	// Tile 0
	output.vDiffuseCoord01.xy	= vWorldPosition.xy * g_tileTextureScaleAndTranslateArray[0].xy;
	output.vDiffuseCoord01.xy 	= output.vDiffuseCoord01.xy + g_tileTextureScaleAndTranslateArray[0].zw;
	// Tile 1
#if LAYER_COUNT > 1
		output.vDiffuseCoord01.zw	= vWorldPosition.xy * g_tileTextureScaleAndTranslateArray[1].xy;
		output.vDiffuseCoord01.zw 	= output.vDiffuseCoord01.zw + g_tileTextureScaleAndTranslateArray[1].zw;
#endif
	// Tile 2
#if LAYER_COUNT > 2
		output.vDiffuseCoord23.xy	= vWorldPosition.xy * g_tileTextureScaleAndTranslateArray[2].xy;
		output.vDiffuseCoord23.xy 	= output.vDiffuseCoord23.xy + g_tileTextureScaleAndTranslateArray[2].zw;
#endif
	// Tile 3
#if LAYER_COUNT > 3
		output.vDiffuseCoord23.zw	= vWorldPosition.xy * g_tileTextureScaleAndTranslateArray[3].xy;
		output.vDiffuseCoord23.zw 	= output.vDiffuseCoord23.zw + g_tileTextureScaleAndTranslateArray[3].zw;
#endif
	// Tile 4
#if LAYER_COUNT > 4
		output.vDiffuseCoord45.xy	= vWorldPosition.xy * g_tileTextureScaleAndTranslateArray[4].xy;
		output.vDiffuseCoord45.xy 	= output.vDiffuseCoord45.xy + g_tileTextureScaleAndTranslateArray[4].zw;
#endif
	// Tile 5
#if LAYER_COUNT > 5
		output.vDiffuseCoord45.zw	= vWorldPosition.xy * g_tileTextureScaleAndTranslateArray[5].xy;
		output.vDiffuseCoord45.zw 	= output.vDiffuseCoord45.zw + g_tileTextureScaleAndTranslateArray[5].zw;
#endif
	// Tile 6
#if LAYER_COUNT > 6
		output.vDiffuseCoord67.xy	= vWorldPosition.xy * g_tileTextureScaleAndTranslateArray[6].xy;
		output.vDiffuseCoord67.xy 	= output.vDiffuseCoord67.xy + g_tileTextureScaleAndTranslateArray[6].zw;
#endif
	// ����ŷ �ؽ���
	output.vAlphaCoord		= input.vDiffuseCoord.xy * g_alphaTextureScaleAndTranslate.xy;
	output.vAlphaCoord		= output.vAlphaCoord + g_alphaTextureScaleAndTranslate.zw;
#endif	/// SPLATTING_ONEPASS
	// �븻�� ��� ��
#if USE_NORMALMAP == 1
	if( bUseNormalmap)
	{		
		output.vWorldNormal	= normalize(mul( input.vNormal.xyz , (float3x3)g_matWorld));
		output.vWorldNormal = normalize( output.vWorldNormal);
		output.vWPos			= mul(vWorldPosition, g_matWorld).xyz;
	}
#endif	// USE_NORMALMAP
}

float GetFogValue( float fPositionZ)
{
	float fogNear = g_vFogFactor.x;
	float fogEnd = g_vFogFactor.y;
	float reciprocalfogDist = g_vFogFactor.z;
	float fFactor = g_vFogFactor.w;
	return saturate( (fPositionZ - fogNear)*reciprocalfogDist) + fFactor;
}


// -------------------------------------------------------------------------------------
// Vertex Shader
// -------------------------------------------------------------------------------------
VSOUTPUT VSMAIN(VSINPUT input)
{
	VSOUTPUT output = (VSOUTPUT)0;


	float4 vLocalPosition = input.vPosition;
	float4 vWorldPosition = mul(vLocalPosition, g_matWorld);
	float4 vWorldViewPosition = mul(vLocalPosition, g_matWorldView);
	output.vPosition = mul(vLocalPosition, g_matWorldViewProj);
	output.vPosInView = vWorldViewPosition.xyz;

	if( SPLATTING_ONEPASS)
		SetTexCoordSplattingOnepass( output, vLocalPosition, input, USE_NORMALMAP);
	else
		SetTexCoord( output, vWorldPosition);
	
	float3 vLocalNormal = input.vNormal.xyz;
	float3 vWorldNormal = normalize(mul(vLocalNormal, (float3x3)g_matWorld));
	
	float3 vWorldViewNormal = normalize(mul(vLocalNormal, (float3x3)g_matWorldView));
	output.vNormal.xyz = vWorldViewNormal;

	if( FOG)
		output.vNormal.w = GetFogValue( output.vPosition.z);
	
	return output;
}


float3x3 invert_3x3_nodet( float3x3 M )
{
	float3x3 T = transpose( M );	

	return float3x3(
		cross( T[1], T[2] ),
		cross( T[2], T[0] ),
		cross( T[0], T[1] ) );	
}


void CalcNormalMapBasedNormal( inout float3 _vNormal, in float4 _vNormalColor, in float3 _vWPos, in float2 _vForNormalMapTex, in float3 _vWordlViewNormal )
{
	_vNormalColor = 2.f * _vNormalColor - 1.f;
	_vNormalColor = normalize(_vNormalColor);
	
	float3 dPdx		= ddx( _vWPos );
	float3 dPdy		= ddy( _vWPos );
	float2 dUVdx	= ddx(_vForNormalMapTex);
	float2 dUVdy	= ddy(_vForNormalMapTex);
	
	float3x3 M = float3x3( dPdx, dPdy, cross( dPdx, dPdy ) );
	float3x3 inverseM = invert_3x3_nodet( M );
	float3 T = mul( inverseM, float3( dPdx.x, dPdy.x, 0 ) );
	float3 B = mul( inverseM, float3( dPdx.y, dPdy.y, 0 ) );
	
	float maxLength = max( length( T ), length( B ) );
	
	float3x3 finalM = float3x3( T / maxLength, B / maxLength, _vWordlViewNormal );
	_vNormal = normalize( mul( _vNormalColor, finalM ) );

	_vNormal = mul( _vNormal, (float3x3)g_matWorldView).xyz;
	_vNormal = normalize(_vNormal);

}

void CalcSplattingOnepass( out float4 vDiffuseColor, out float fSpecular, inout float3 vNormal, in VSOUTPUT input)
{
	const int SpecularArray[MAX_LAYER_PER_PATCH] = {
		LAYER_0_SPECULAR,
		LAYER_1_SPECULAR,
		LAYER_2_SPECULAR,
		LAYER_3_SPECULAR,
		LAYER_4_SPECULAR,
		LAYER_5_SPECULAR,
		LAYER_6_SPECULAR,
	};
	
	const int NormalmapArray[MAX_LAYER_PER_PATCH] = {
		LAYER_0_NORMALMAP,
		LAYER_1_NORMALMAP,
		LAYER_2_NORMALMAP,
		LAYER_3_NORMALMAP,
		LAYER_4_NORMALMAP,
		LAYER_5_NORMALMAP,
		LAYER_6_NORMALMAP,
	};
	
	int i;

	const float4 vClrDefault = float4( 0, 0, 0 ,1);
	float4 vClr[MAX_LAYER_PER_PATCH] = { vClrDefault, vClrDefault, vClrDefault, vClrDefault, vClrDefault, vClrDefault, vClrDefault};
	float fMask[MAX_LAYER_PER_PATCH] = { 1,1,1,1,1,1,1};
	float fReadMask[MAX_LAYER_PER_PATCH] = { 0,1,1,1,1,1,1};

	float4 vNormalColor = 1;	// �ϴ� �ʱ�ȭ
	const float4 vClrNormalDefault = float4( 0.5f, 0.5f, 1 , 1);	/// �븻���� ���� -1~1�� ��ȯ ������ �ֱ� ������ ��ȯ ���� ���� ������.
	float4 vClrNormal[MAX_LAYER_PER_PATCH] = { vClrNormalDefault, vClrNormalDefault, vClrNormalDefault, vClrNormalDefault, vClrNormalDefault, vClrNormalDefault, vClrNormalDefault};
	float2 vForNormalMapTex = 0;
	
#if SPLATTING_ONEPASS == 1
		/// NORMALMAP ��� ������ ���̤Ӿ� ī��Ʈ�� �Ѱܼ� ���� �� ���� ������, ������ ���ؼ� ���̾� ���� üũ�� �� ����.
		/// normal 1
#if LAYER_COUNT > 0 && LAYER_0_NORMALMAP == 1
		vClrNormal[0]	= tex2D( NormalMap0,	input.vDiffuseCoord01.xy);
		vForNormalMapTex = input.vDiffuseCoord01.xy;
#endif
		/// normal 2
#if LAYER_COUNT > 1 && LAYER_1_NORMALMAP == 1
		vClrNormal[1]	= tex2D( NormalMap1,	input.vDiffuseCoord01.zw);
		vForNormalMapTex = input.vDiffuseCoord01.zw;
#endif
		/// normal 2
#if LAYER_COUNT > 2 && LAYER_2_NORMALMAP == 1
		vClrNormal[2]	= tex2D( NormalMap2,	input.vDiffuseCoord23.xy);
		vForNormalMapTex = input.vDiffuseCoord23.xy;
#endif
		/// normal 2
#if LAYER_COUNT > 3 && LAYER_3_NORMALMAP == 1
		vClrNormal[3]	= tex2D( NormalMap3,	input.vDiffuseCoord23.zw);
		vForNormalMapTex = input.vDiffuseCoord23.zw;
#endif
		/// normal 2
#if LAYER_COUNT > 4&& LAYER_4_NORMALMAP == 1
		vClrNormal[4]	= tex2D( NormalMap4,	input.vDiffuseCoord45.xy);
		vForNormalMapTex = input.vDiffuseCoord45.xy;
#endif
		/// normal 2
#if LAYER_COUNT > 5 && LAYER_5_NORMALMAP == 1
		vClrNormal[5] 	= tex2D( NormalMap5, 	input.vDiffuseCoord45.zw);
		vForNormalMapTex = input.vDiffuseCoord45.zw;
#endif
		/// normal 2
#if LAYER_COUNT > 6 && LAYER_6_NORMALMAP == 1
		vClrNormal[6] 	= tex2D( NormalMap6, 	input.vDiffuseCoord67.xy);
		vForNormalMapTex = input.vDiffuseCoord67.xy;
#endif

	// tile read
#if LAYER_COUNT > 0
	vClr[0]	= tex2D( TileMap0,		input.vDiffuseCoord01.xy);
#endif
#if LAYER_COUNT > 1
	vClr[1]	= tex2D( TileMap1,		input.vDiffuseCoord01.zw);
#endif
#if LAYER_COUNT > 2
	vClr[2]	= tex2D( TileMap2,		input.vDiffuseCoord23.xy);
#endif
#if LAYER_COUNT > 3
	vClr[3]	= tex2D( TileMap3,		input.vDiffuseCoord23.zw);
#endif
#if LAYER_COUNT > 4
	vClr[4]	= tex2D( TileMap4, 		input.vDiffuseCoord45.xy);
#endif
#if LAYER_COUNT > 5
	vClr[5]	= tex2D( TileMap5, 		input.vDiffuseCoord45.zw);
#endif
#if LAYER_COUNT > 6
	vClr[6]	= tex2D( TileMap6, 		input.vDiffuseCoord67.xy);
#endif

	for( i=0 ; i<LAYER_COUNT;i++)
	{
		fMask[i] = vClr[i].a;
		vClr[i].a *= SpecularArray[i];	// ����ŧ�� �÷��װ� ������ ����ŧ�� ���� �״�� �� ���̰�, �÷��װ� ������ 0
		/// ���̾� ��ŷ ����̸� �ش� ���̾� ���� �÷��� ������
		if( MARKINGLYER)
		{
			if( i == g_fMarkingLayer)
				// ���������� ȭ�鿡
				vClr[i].rgb	 = float4( 1,0,1,1);
			else
				vClr[i].rgb	 = float4( 0,0,0,1);
		}
	}
	
	float4 vMask0 = (float4)0;
	float4 vMask1 = (float4)0;
	vMask0 = tex2D( MaskMap0, input.vAlphaCoord );	
	// �ι�° ����ũ�� ���̾� ī��Ʈ�� 6 �̻��� ���� ���δ�. 5������� ����ũ �ϳ��� Ŀ�� ����.
	// LAYER_COUNT�� �ܺο��� ������ �� ����̱� ������ �����Ϸ��� �˾Ƽ� �ھ� ó�� ���� ����.
	if( LAYER_COUNT > 5)
		vMask1 = tex2D( MaskMap1, input.vAlphaCoord );	
	// �ؽ��Ŀ� ���� ���� ����
	if( LAYER_COUNT > 1)
		fReadMask[1] = vMask0.r;
	if( LAYER_COUNT > 2)
		fReadMask[2] = vMask0.g;
	if( LAYER_COUNT > 3)
		fReadMask[3] = vMask0.b;
	if( LAYER_COUNT > 4)
		fReadMask[4] = vMask0.a;
	if( LAYER_COUNT > 5)
		fReadMask[5] = vMask1.r;
	if( LAYER_COUNT > 6)
		fReadMask[6] = vMask1.g;
	/// ����ŧ�� ������� ������ Ÿ�� �ؽ����� aä���� ���� ���� ���� ��. �� ������ �ݿ� �Ͽ� ������ ���ؾ� ��.
	for( i = 1 ; i < LAYER_COUNT ; i++ )
		fMask[i] = fReadMask[i] * ( SpecularArray[i] == 0 ? fMask[i] : 1);
#endif	/// SPLATTING_ONEPASS
		
	/// output vDiffuseColor	
	vDiffuseColor = vClr[0];
	for( i = 1 ; i < LAYER_COUNT ; i++ )
		vDiffuseColor	= lerp( vDiffuseColor, vClr[i], fMask[i]);
	
	/// output vNormal
#if USE_NORMALMAP == 1
	vNormalColor = vClrNormal[0];
	for( i = 1 ; i < LAYER_COUNT ; i++ )
		vNormalColor	= lerp( vNormalColor, vClrNormal[i], fMask[i]);		
	CalcNormalMapBasedNormal( vNormal, vNormalColor, input.vWPos, vForNormalMapTex, input.vWorldNormal );
#endif /// USE_NORMALMAP
	
	/// output fSpecular
	fSpecular = vDiffuseColor.a;
}

float GetSpecularCompress( in float fGlossiness, in float fSpecular)
{
	// ���� ����ŧ�� �� ���ۿ� ����.
	// ����ŧ��� (vSpecular.y).(vSpecular.x)������ �����Ѵ�. 
	// �÷��� ����Ʈ ���̹Ƿ� ��Ƽ�� ������ x,y�� saturate(vSpecular.x/1), saturate(vSpecular.y/10)�̷��� ��� �� �ϳ��� ���� ����.
	// x: SpecularLevel, y: Glossiness
	// SpecularLevel�� ����ŧ�� ���� ���̰ų� 1�̹Ƿ� 0~1 ������ ���̴�. 1 ��� 0.99������ ��ü�Ͽ� ���ظ� ���� �ϵ��� ����. 0.001�� ������ ���� ���� ���� ���̴�.
	// -��ȣ�� �ٿ��� �� �������� �۵� ���ϵ��� ����.
	float fSpecularCompress = max( fGlossiness, 0) + min( fSpecular, 0.999f);
	return -fSpecularCompress;	
}

struct PSOUTPUT {

#if RT1 == 1
	float4 color  : COLOR0;
#elif RT2 == 1
	float4 color  : COLOR0;
	float4 color1 : COLOR1;
#elif RT3 == 1
	float4 color  : COLOR0;
	float4 color1 : COLOR1;
	float4 color2 : COLOR2;
#endif
};


// -------------------------------------------------------------------------------------
// Pixel Shader
// -------------------------------------------------------------------------------------
PSOUTPUT PSMAIN(
	VSOUTPUT input
#if LIGHTBUFFER == 1
	, float4 vPos : VPOS
#endif // LIGHTBUFFER
	) : COLOR
{
	const float fRimScale = 0;
	const float fAO = 0;
	const float fSpecularLevel = 1.0f;
	const float fGlossiness = 10.0f;
	
	float2 vDiffuseCoord = 0;
	float4 vDiffuseColor = 1;
	float fSpecular = 0;
	float2 vAlphaCoord = 0;
	float4 vAlphaColor = 1;
	float fAlpha = 1;
	float3 vNormal = normalize(input.vNormal.xyz);
	
	if( SPLATTING_ONEPASS)
		CalcSplattingOnepass( vDiffuseColor, fSpecular, vNormal, input);
	else if( GRID)
		vDiffuseColor = 1;
	else
		vDiffuseColor = tex2D( BakedMap, input.vDiffuseCoord01.xy );
	
#if LIGHTBUFFER == 1
	float2 vLightCoord = vPos.xy / g_Resolution;
	float4 fLighting = tex2D( LightBufferMap, vLightCoord);
	vDiffuseColor.rgb *= fLighting.rgb;
	fSpecular = fLighting.a;
	if( FOG)
		vDiffuseColor.rgb  = lerp( vDiffuseColor.rgb, g_vFogColor, input.vNormal.w);	
#endif // LIGHTBUFFER

	int nZDirection = (sign(vNormal.z)>=0)*2-1;	
	float fLinearDepth = -input.vPosInView.z/g_fFarDist;
	float4 vDepthSpecBufferOut = float4( fLinearDepth * nZDirection, 0, 1, 1);
	float4 vNormalBufferOut = float4( vNormal.x, vNormal.y, 1, 1);	// ���� �ڵ�
	float4 vDiffuseAoBufferOut = float4( vDiffuseColor.rgb, fAO);
	
	if( SPECULAR)
		vDepthSpecBufferOut.y = GetSpecularCompress( fGlossiness, fSpecular);

	PSOUTPUT output;
	
#if RT1 == 1
	vDiffuseAoBufferOut.a = 1;	// RT1�� ���� Ȥ�� �𸣴� ���� ä���� 1�� ä�� ����.
	output.color = vDiffuseAoBufferOut;
#elif RT2 == 1
	output.color = vNormalBufferOut;
	output.color1 = vDepthSpecBufferOut;
#elif RT3 == 1
	output.color = vNormalBufferOut;
	output.color1 = vDepthSpecBufferOut;
	output.color2 = vDiffuseAoBufferOut;
#endif
	
	return output;
}

technique MainTechnique
{
    pass P0
    {
		VertexShader = compile vs_3_0 VSMAIN();
		PixelShader  = compile ps_3_0 PSMAIN();
    }
}
