#ifndef _RGI_PROCESSOR_H
#define _RGI_PROCESSOR_H



namespace rs3 {

class RTexture;
class RShaderFX;
class RCameraSceneNode;	

class RS_API RGIProcessor
{
protected:
	int m_nWidth;
	int m_nHeight;

	float m_Radius;
	float m_AngleBias;
	float m_Attenuation;
	float m_Contrast;
	float m_BrightBias;

	int	m_QualityMode;

	string	m_strConfigPath;

	RTexture*	m_pTexShadow;	/// �׸��� �������� AO�� ó�� �� ���̴�.
	bool		m_bNotShadowOnly;	/// �׸��� �������� AO�� ó�� �ϴ� ���� ������?
	bool		m_bSubtract;	/// AO�� ������ ���� �����Ҳ���? ������? �ƴ� �⺻���� ������.
	bool		m_bAllLight;	/// ��� ����Ʈ�� ��������? ��� ����Ʈ�� �����̸� ����Ʈ �н� �� �������� ó���� �ǰ� �ƴϸ� �𷺼ų� ����Ʈ���� ó���� ��.

public:
	RGIProcessor( int nQualityMode);
	virtual ~RGIProcessor(void);

	virtual void Render( RCameraSceneNode* pCamera, int nWidth, int nHeight, bool bRenderDirect) = 0;
	virtual void Init( int nW, int nH );
	virtual void Destroy();
	virtual void Resize( int nW, int nH );
	virtual void Invalidate();
	virtual void Restore();

	bool CreateBuffer();
	void DestroyBuffer();

	virtual void SaveToXML(MXmlElement &element);
	virtual void LoadToXML(MXmlElement &element);

	virtual void LoadConfig( string strPath);
	virtual void SaveConfig( string strPath);

	virtual int GetQuality() { return m_QualityMode;}
	virtual RTexture* GetRTShadow() { return m_pTexShadow;}
	virtual RTexture* GetRTGI() { return NULL;}

	bool IsShadowOnly()	{ return !m_bNotShadowOnly;	}
	bool IsSubtract()	{ return m_bSubtract;	}
	bool IsAllLight()	{ return m_bAllLight;	}
};



}

#endif