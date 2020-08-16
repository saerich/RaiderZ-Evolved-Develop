#pragma once

class XModuleEntity;

/// �� ĳ������ ���� ������
class XMyCharacterAlphaMgr
{
private:
	bool		m_bAlphaState;
public:
	XMyCharacterAlphaMgr() : m_bAlphaState(false) {}
	void Check(float fCameraDist, XModuleEntity* pModuleEntity);

	const float CONST_ALPHA_VALUE()	{ return 0.1f; }			///< ����ȭ�Ǿ��� �� ���İ�
	const float CONST_ALPHA_DIST()	{ return 40.0f; }			///< ī�޶�� ��ĳ���Ͱ��� �Ÿ��� �� �Ÿ����� ª���� ���� ó����
	const float CONST_FADE_TIME()	{ return 0.2f; }			///< ���� ó�� �ð�
};
