// EffectToolDoc.h : CEffectToolDoc Ŭ������ �������̽�
//


#pragma once

class CEffectToolDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CEffectToolDoc();
	DECLARE_DYNCREATE(CEffectToolDoc)

// Ư��
public:

// �۾�
public:

// ������
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ����
public:
	virtual ~CEffectToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};


