
// SpaceViewerDoc.h : CSpaceViewerDoc Ŭ������ �������̽�
//


#pragma once


class CSpaceViewerDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CSpaceViewerDoc();
	DECLARE_DYNCREATE(CSpaceViewerDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CSpaceViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


