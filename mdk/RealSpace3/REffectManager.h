#pragma once

#include <map>
#include <string>

#include "RParticlePool.h"
#include "RVType.h"

namespace rs3 {

class REffectSceneNode;

class REffectSharedBuffer
{
private:
	HVERTEXBUFFER		m_hVertexBuffer;
	unsigned int		m_nVertexCount;
	PVERTEXFORMAT*		m_pVertexArray;
	int					m_nVertexWriteCount;
	int					m_nVertexBufferSize;
	int					m_nNextVertexData;		//D3DLOCK_NOOVERWRITE ���ÿ� gpu�� ����ϰ� ���� �ʴ� ������ �޸𸮿��� �����ּ�
	int					m_nCurrentVertexData;	//���� ���� �ɾ ����� ������ �ּ�. �������ÿ� ����


	HINDEXBUFFER		m_hIndexBuffer;
	unsigned int		m_nIndexCount;
	WORD*				m_pIndexArray;
	int					m_nIndexWriteCount;

public:
	REffectSharedBuffer( int nVertexCount, int nIndexCount );
	~REffectSharedBuffer();

	PVERTEXFORMAT*		GetVertexArray( unsigned int nCount );
	WORD*				GetIndexArray( unsigned int nCount );

	bool	WriteVertex( unsigned int nCount );
	bool	WriteIndex( unsigned int nCount );

	int		DrawPrimitiveTriangleList();
	int		DrawIndexedPrimitiveTriangleStrip();

private:
	void	ReCreateVertexBuffer();
	void	ReCreateIndexBuffer();

	void	DeleteVertexBuffer();
	void	DeleteIndexBuffer();
};

class RS_API REffectManager
{
	typedef std::pair<int, REffectSceneNode*> REF_EFFECT_SRC;
	typedef std::map<std::string, REF_EFFECT_SRC> EFFECT_SRC_MAP;
	EFFECT_SRC_MAP					m_mapEffectSources;
	static RParticlePool*			m_pParticlePool;
	static REffectSharedBuffer*		m_pSharedBuffer;

	static float					m_fCreateParticleRange; ///< ī�޶� ��ġ ���� ��ƼŬ ���� ����

public:
	REffectManager();
	~REffectManager();

	void				Destroy();

	// �̸� ĳ���صα� ����..
	REffectSceneNode*	CreateEffectSource(const char* szFileName, bool bBackgroundCreation, bool bAddInfiniteEffectSource = true);
	int					AddRefEffectSource(const std::string& _strName, REffectSceneNode* _pEffectSceneNode);
	void				RemoveRefEffectSource(const std::string& _strName, REffectSceneNode* _pNotDeleteLastSrc_If_EqualToThis = NULL);

	// ������ �ٲ��� ���ӵ��߿� ���� �б� ���ؼ� ĳ�õ� ����Ʈ�� �����ִ� �뵵�� ����Ѵ�.
	void				ClearEffectSource();

	/// �� REffectSceneNode �� �����Ѵ�. ������ ���� ȣ���� �ʿ��� delete ���־�� �Ѵ�
	REffectSceneNode*	CreateEffect(const char* szFileName, bool bBackgroundCreation = false);
	REffectSceneNode*	GetEffect(const char* szFileName);

	/// ����Ʈ�� �����Ѵ�.
	RSceneNode*			CloneEffect(RSceneNode* pSource);

	static void			SetCreateParticleRange( float fRange )	{ m_fCreateParticleRange = fRange; }
	static float		GetCreateParticleRange()				{ return m_fCreateParticleRange; }

	/// ��ƼŬ Pool �������̽�
	static RParticle*	GetParticle(unsigned int nHandle)		{ return m_pParticlePool->Get(nHandle); }
	static unsigned int	GetNewParticle()						{ return m_pParticlePool->GetNew(); }
	static void			RemoveParticle(unsigned int nHandle)	{ m_pParticlePool->Remove(nHandle); }
	static unsigned int	GetParticleCount()						{ return m_pParticlePool->GetCount(); }
	static unsigned int	GetParticleCapacity()					{ return m_pParticlePool->GetCapacity(); }

	// ���� ���� �������̽�
	static PVERTEXFORMAT*		GetVertexArray(unsigned int nCount)		{ return m_pSharedBuffer->GetVertexArray(nCount); }
	static WORD*				GetIndexArray(unsigned int nCount)		{ return m_pSharedBuffer->GetIndexArray(nCount); }
	static bool					WriteVertexBuffer(unsigned int nCount)	{ return m_pSharedBuffer->WriteVertex(nCount); }
	static bool					WriteIndexBuffer(unsigned int nCount)	{ return m_pSharedBuffer->WriteIndex(nCount); }
	static int					DrawPrimitiveTriangleList()				{ return m_pSharedBuffer->DrawPrimitiveTriangleList(); }
	static int					DrawIndexedPrimitiveTriangleStrip()		{ return m_pSharedBuffer->DrawIndexedPrimitiveTriangleStrip(); }

protected:
	void				CloneControllers(RSceneNode* pDestNode, RSceneNode* pSource);
	void				CloneChildren(RSceneNode* pDestNode, RSceneNode* pSource);
};

}
