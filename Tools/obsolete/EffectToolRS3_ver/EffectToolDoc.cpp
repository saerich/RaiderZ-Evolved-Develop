// EffectToolDoc.cpp : CEffectToolDoc Ŭ������ ����
//

#include "stdafx.h"
#include "EffectTool.h"

#include "EffectToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEffectToolDoc

IMPLEMENT_DYNCREATE(CEffectToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CEffectToolDoc, CDocument)
END_MESSAGE_MAP()


// CEffectToolDoc ����/�Ҹ�

CEffectToolDoc::CEffectToolDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CEffectToolDoc::~CEffectToolDoc()
{
}

BOOL CEffectToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ �ٽ� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CEffectToolDoc serialization

void CEffectToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CEffectToolDoc ����

#ifdef _DEBUG
void CEffectToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEffectToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEffectToolDoc ���
