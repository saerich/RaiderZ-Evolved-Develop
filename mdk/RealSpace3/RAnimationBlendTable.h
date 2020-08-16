#ifndef _AnimationBlendTable_h_
#define _AnimationBlendTable_h_

namespace rs3 {

enum RANIMATIONTRANSITIONTYPE;
class RMeshAnimationMgr;

struct RS_API BLENDDATA {
	RANIMATIONTRANSITIONTYPE	eType;
	unsigned int				nDuration;
	BLENDDATA(RANIMATIONTRANSITIONTYPE _eType,unsigned int _nDuration) : eType(_eType), nDuration(_nDuration) {}
	BLENDDATA() {}

	static const RANIMATIONTRANSITIONTYPE	DEFAULT_BLEND_TYPE;
	static const unsigned int				DEFAULT_BLEND_DURATION;
};

/**
	@class RAnimationBlendTable
	@brief �ִϸ��̼� ��ȯ ���� �ɼ� ���� Ŭ����
*/
class RS_API RAnimationBlendTable
{
public:
	RAnimationBlendTable();
	~RAnimationBlendTable();

	void Clear();

	/**
	@param animationMgr �ʱ�ȭ �� �ִϸ��̼� �Ŵ��� Ŭ����, NULL���� ������ �ȵȴ�.
	@note �̹� ���õ� RAnimationMgr Ŭ���� �����Ϳ� ���� �Ķ���� ���� �Ѿ���� ���� �����͸� ���� ���� �ʴ´�.
	      RAnimationMgrŬ�������� �ִϸ��̼� ������ �߰� �ϴ� ��찡 �ֱ� �����̴�.
	*/
	void Reset( RMeshAnimationMgr* const animationMgr );

	/**
	@brief �ִϸ��̼� ���� Ÿ���� �߰��ϰų� �����Ѵ�.
	@param source ���� �ִϸ��̼� �̸�
	@param target ��� �ִϸ��̼� �̸�
	@param aniTransType ���� �� �ִϸ��̼� ���� Ÿ��
	@param nDuration �ִϸ��̼� ���� �ð�
	@return Ÿ���� �߰��ϰų� ���濡 �����ϸ� true�� ��ȯ�Ѵ�.
	*/
	bool SetAnimationTransData( const char* source, const char* target, RANIMATIONTRANSITIONTYPE aniTransType, unsigned int nDuration );

	/**
	@brief �ִϸ��̼� ���� Ÿ���� �߰��ϰų� �����Ѵ�.
	@param source ���� �ִϸ��̼� id ��
	@param target ��� �ִϸ��̼� id ��
	@param aniTransType ���� �� �ִϸ��̼� ���� Ÿ��
	@param nDuration �ִϸ��̼� ���� �ð�
	@return Ÿ���� �߰��ϰų� ���濡 �����ϸ� true�� ��ȯ�Ѵ�.
	*/
	bool SetAnimationTransData( int source, int target, RANIMATIONTRANSITIONTYPE aniTransType, unsigned int nDuration );

	/**
	@brief �ִϸ��̼� ���� Ÿ���� ���´�.
	@param source ���� �ִϸ��̼� �̸�
	@param target ��� �ִϸ��̼� �̸�
	@return Ÿ���� ��ȯ�Ѵ�. �ش� ���̽��� ���� �����Ͱ� ������ �⺻ Ÿ���� ��ȯ�Ѵ�.
	*/
	BLENDDATA GetAnimationTransData( const char* source, const char* target );

	/**
	@brief �ִϸ��̼� ���� Ÿ���� ���´�.
	@param source ���� �ִϸ��̼� id ��
	@param target ��� �ִϸ��̼� id ��
	@return Ÿ���� ��ȯ�Ѵ�. �ش� ���̽��� ���� �����Ͱ� ������ �⺻ Ÿ���� ��ȯ�Ѵ�.
	*/
	BLENDDATA GetAnimationTransData( int source, int target );

	/**
	@brief �ִϸ��̼� ���� Ÿ���� �����Ѵ�.
	@param source ���� �ִϸ��̼� �̸�
	@param target ��� �ִϸ��̼� �̸�
	@return ������ �����ϸ� true�� ��ȯ�Ѵ�.
	*/
	bool DeleteAnimationTransType( const char* source, const char* target );

	/**
	@brief �ִϸ��̼� ���� Ÿ���� �����Ѵ�.
	@param source ���� �ִϸ��̼� id ��
	@param target ��� �ִϸ��̼� id ��
	@return ������ �����ϸ� true�� ��ȯ�Ѵ�.
	*/
	bool DeleteAnimationTransType( int source, int target );

	/**
	@brief ��ȯ ���̺��� XML�����ͷ� �����Ѵ�.
	@param parentXmlElement XML ���� ��� ��.
	*/
	void SaveXml( MXmlElement* parentXmlElement );

	/**
	@brief ��ȯ ���̺��� XML�����ͷ� �ҷ��´�.
	@param animationMgr �ʱ�ȭ �� �ִϸ��̼� �Ŵ��� Ŭ����.
	@param parentXmlElement XML ���� ��� ��.
	@param szFilter ���� ��Ʈ��
	*/
	void LoadXml( RMeshAnimationMgr* const animationMgr, MXmlElement* parentXmlElement, const char* szFilter ="");

	/// �⺻ ��ȯ Ÿ�� ���
	static const BLENDDATA					DEFAULT_ANIMATION_TRANSITION_DATA;

public:
	typedef std::pair<int, int> AnimationBlendPair;
	typedef map<AnimationBlendPair, BLENDDATA> AnimationBlendData;
//	typedef std::pair<AnimationBlendPair, RANIMATIONTRANSITIONTYPE> AnimationBlendDataPair;
//	typedef AnimationBlendData::const_iterator AnimationBlendDataConstIter;
//	typedef AnimationBlendData::iterator AnimationBlendDataIter;

private:
	bool IsAnimationNameMatch( const char* source, const char* target );
	int GetTransTypeId( const char* typestr );

public:
	bool IsValid();
	bool IsNotDefaultValue( BLENDDATA blendData );
	static const char* GetTransTypeString( int type );
	AnimationBlendData* GetAnimationBlendData() { return &m_AnimationBlendData; }

private:
	RMeshAnimationMgr* m_AnimationMgr;
	AnimationBlendData m_AnimationBlendData;
};

};

#endif // _AnimationBlendTable_h_
