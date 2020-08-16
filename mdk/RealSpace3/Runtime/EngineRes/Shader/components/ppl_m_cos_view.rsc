interface()
{
	$name = ppl_m_cos_view
	$define = ppl_cos_view
	$dependency = lightbuffer_read
}

void PSMAIN(inout PSOUTPUT output)
{
	float2 vScreenNormal = 0;
	IMPORT( E_vScreenNormal, vScreenNormal = E_vScreenNormal );
	
	// ��ũ�� �����̽� �븻�̹Ƿ� x,y���� ������ �׸�ŭ Z������ �븻�� �����ٴ� ���� ��.
	// �ݴ�� z,y�� Ŭ���� �ܰ����� ����
	// 1���� ū ���� ���� �־�� ���� �ȵ�
	float cosView = 1 - max( abs(vScreenNormal.x), abs(vScreenNormal.y));
	EXPORT( float, E_fCosView, cosView );
}

