#pragma once

#include "XGameFrame.h"

/// ���� ������ ������
class XGameFrameManager
{
protected:
	map<int, XBaseFrame*>		m_FrameMap;
	int							m_nNextFrame;
	int							m_nPrevFrame;
	bool						m_bReserved;
	XBaseFrame*					m_pCurFrame;

	bool						PerformChange( int nFrameID, const wchar_t* szArgs =NULL);
public:
	/// ������
	XGameFrameManager();
	/// �ı���
	virtual						~XGameFrameManager();
	/// ���� ������ ����Ʈ �ε�
	void						Load(const wchar_t* szFileName);		// �����丵 ���
	/// �޼��� ó��
	bool						OnEvent(MEvent& e);
	/// ���� �������� �߰�
	void						Add(XBaseFrame* pGameFrame);
	/// �ٸ� �������������� �̵�
	void						Change(int nFrameID);
	/// �� ó�� ���������� ����.
	/// ������ ���� ó�� �����Ҷ����� �ѹ� �����Ѵ�.
	bool						Start(int nFrameID, const wchar_t* szArgs=NULL);
	/// ��� ������������ �����Ѵ�. �޸���ü���� ����.
	void						DeleteAll();
	/// ������Ʈ
	void						Update(float fDelta);
	/// ���� �غ�
	void						OnPreRender();
	/// ����
	void						OnRender();
	/// Flip��
	void						OnPreFlip();
	/// Ȱ��ȭ, ��Ȱ��ȭ ��
	void						OnActive(bool bActive);
	/// ���� �����ӿ��� �޼��� ����
	void						Message(const wchar_t* szMsg);
	/// ���� ������ ID ��ȯ
	int							GetCurrFrameID();
	/// ���� ������ ID ��ȯ
	int							GetPrevFrameID();
	/// ������ ��ȯ
	XBaseFrame*					GetFrame( int nFrameID);
	/// ������ �̸����� ������ ��ȯ
	XBaseFrame*					GetFrameFromName(const wchar_t* szFrameName);
	/// ���� ������ ��ȯ
	XBaseFrame*					GetCurrFrame()	{ return m_pCurFrame; }
};
