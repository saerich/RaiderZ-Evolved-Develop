interface()
{
	$name = ppl_g_write_normal_depth_specular_rim_rt01
	$define = ppl_g_write_normal_depth_rt01
	$define = ppl_g_write_normal_depth
	$define = ppl_g_write_normal
	$dependency = g_depth
	$dependency = g_constant
}

void PSMAIN(inout PSOUTPUT output)
{
	// �븻������� ���� ������ �����ؼ� ������ �Ѵ�. ppl_g_write_no_lighting_normal���� 10,10�� ����� ���̹Ƿ� �д� ������ �븻������ �ϸ� �ȵ�.
	float3 vNormal;
	IMPORT( E_pgNormal, vNormal = E_pgNormal);

	// ���Ĵ� G�н� ���������� ���� ������Ʈ �κп��� ä������. �� �������� �̸� ������ ����.
	// ���� �ڵ���� color.a�� ���� �����ϱ⵵ �ϰ� E_fAlpha�� ��� �ϱ⵵ �ϴµ� 
	// E_fAlpha�� ���� �ݿ��� �Ͽ��� ���������� �ȴ�.
	float fAlpha = 1;
	IMPORT( E_fAlpha, fAlpha = E_fAlpha);

	// �븻�� xy �ΰ��� ���� ����ϰ� ��� �� z�� �����س��� ����.
	output.color = float4( vNormal.x, vNormal.y, 1, fAlpha);

	// ������ 0~1. z/w. z�� �ʿ�� �˾Ƽ� �̾Ƴ��� ���.
	// normal�� Z�� ������ DepthBuffer�� ���� �����Ѵ�.
	int nZDirection = (sign(vNormal.z)>=0)*2-1;	// if(vNormal.z >= 0) nZDirection =1, else nZDirection = -1
	
	// Specular
	float3 vSpecularLevel = 0;
	IMPORT( E_vLitSpecular, vSpecularLevel = E_vLitSpecular);
	float fSpecularGlossiness = 0;
	IMPORT( E_fSpecularGlossiness, fSpecularGlossiness = E_fSpecularGlossiness);
	// ����ŧ��� (Glossiness).(Level)������ �����Ѵ�. 
	// SpecularLevel�� ����ŧ�� ���� ���̰ų� 1�̹Ƿ� 0~1 ������ ���̴�. 1 ��� 0.99������ ��ü�Ͽ� ���ظ� ���� �ϵ��� ����. 0.001�� ������ ���� ���� ���� ���̴�.
	float fSpecularCompress = max( fSpecularGlossiness, 0) + min( vSpecularLevel, 0.99f);
	
	// ����ŧ�� ������ ��ȣ�� �� ����ũ�� ���
	float fRimMask = -1;
	IMPORT( E_pgRimMask, fRimMask = E_pgRimMask);
	fSpecularCompress *= fRimMask;

	float fLinearDepth;
	IMPORT( E_fLinearDepth, fLinearDepth = E_fLinearDepth);
	output.color1 = float4( fLinearDepth *nZDirection, fSpecularCompress, 1, 1);
}
