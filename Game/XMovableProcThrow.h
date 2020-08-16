
#pragma  once

#include "XMovableProcessor.h"


class XMovableProcThrow : public XMovableProcessor
{
private:
	XModuleMovable*			m_pMovableProcOwner;
	XModuleCollision*		m_pModuleCollision;

	MUID					m_uidAttacker;			// ������ UID
	bool					m_bGrip;				// �������� �ִ��� ����
	bool					m_bGripStart;


public:
	XMovableProcThrow();
	virtual ~XMovableProcThrow();

	virtual void Init( XModuleMovable* pProcOwner);
	virtual void Fini();

	virtual bool IsProcessable(const XMovableFactor& _rMovableFactor);
	virtual void Run( XMovableFactor& rMovFactor, const vec3& vPosCurr, const vec3& vDirCurr, vec3& vPosTo, vec3& vDirTo, vec3& vUpVec, float fDelta);

	virtual const wchar_t* GetDebugName() { return L"ProcessThrow"; }


public:


protected:
	// ��� ���۽�... ���� ������Ʈ
	void _UpdateGrapplStart(XMovableFactor& rMovFactor, const vec3& vPosCurr, vec3& vPosTo, vec3& vDirTo, vec3& vUpVec, float fDelta);

	// ���� ���� ������Ʈ
	void _UpdateGrappled( XMovableFactor& rMovFactor, const vec3& vPosCurr, vec3& vPosTo, vec3& vDirTo, vec3& vUpVec, float fDelta);

	// ������ ���󰡴� ���� ������Ʈ
	void _UpdateThrow( XMovableFactor& rMovFactor, const vec3& vPosCurr, vec3& vPosTo, vec3& vDirTo, float fDelta);


	// ���� ����� ��ġ �� ������ ����
	bool _GetGripPosDir( const MUID& uidAttacker, vec3& vPos, vec3& vDir, vec3& vUpVec);
};
