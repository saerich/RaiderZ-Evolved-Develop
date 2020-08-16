#ifndef _RAnimationNode_h
#define _RAnimationNode_h

#include "RAnimationDef.h"
#include "RMeshUtil.h"
#include "RAnimationKey.h"

namespace rs3 {

class RMesh;

// key controller �������� �ٲ��ֱ�..�������� ������ ���밡���ϵ���..
class RS_API RAnimationNode : public RBaseObject
{
public:
	RAnimationNode();
	~RAnimationNode();


	//////////////////////////////////////////////////////////////////////////
	// Data Getter
	//	keyIterator �� ���� �������� Ű�� ���������ӿ��� ���� Ȯ���� ���� ������, 
	//	���� Ű�ε����� �����س��� ���Դϴ�.
	//	���������÷��� ������ 0�� �Ҵ��Ͻð� ������ �Ķ���ͷ� ����ϼ���. - dubble
	//
	bool			GetRotValue(RQuaternion* pOut, int frame,unsigned int &rotKeyIterator);
	bool			GetPosValue(RVector* pOut, int frame,unsigned int &posKeyIterator);
	int				GetVecValue(int frame,RVector* pVecTable);
	bool			GetVisValue(float* pOut, int frame, unsigned int &visKeyIterator);
	bool			GetScaleValue(RVector* pOut, int frame,unsigned int &scaleKeyIterator);

	void			GetSamplingData( int nStartFrame, int nEndFrame, int nSampleInterval, std::vector< RMatrix >& _rOUt, const RMatrix* pBaseCoordIfUse );

public:

	string			m_parentName;

	RMatrix			m_matLocal;				///< �ʱ� �������� (Local) tm

	// ������� �ʴ� base ���� �ּ�ó���մϴ�. - birdkr
	//RVector			m_baseScale;			///< �ʱ� �������� tm �� �����ϰ�
	//RQuaternion		m_baseRotation;			///< �ʱ� �������� tm �� �ִϸ��̼��� ȸ����
	//RVector			m_baseTranslation;		///< �ʱ� �������� tm �� �ִϸ��̼��� �̵���

	int				m_nNodeID;

	/// ��� �����ӿ� ���� �ٿ�� �ڽ�
	RBoundingBox	m_bboxVertexAni;

	RAnimationTrackTest<RPosKey>	m_positionKeyTrack;
	RAnimationTrackTest<RQuatKey>	m_rotationKeyTrack;
	RAnimationTrackTest<RScaleKey>	m_scaleKeyTrack;
	RAnimationTrackTest<RVisKey>	m_visKeyTrack;

	int				m_nVertexCnt;	// ���ؽ� �ִϸ��̼� ������ ��
	int				m_nVertexVCnt;	// ���ؽ� �ִϸ��̼��� �޽��� ���ؽ� ����
	DWORD*			m_pVertexFrame;	// ���ؽ� �ִϸ��̼��� Ű������ �迭
	RVector**		m_vertex;		// ���� ���ؽ� �ִϸ��̼� ������
};


}

#endif//_RAnimationNode_h
