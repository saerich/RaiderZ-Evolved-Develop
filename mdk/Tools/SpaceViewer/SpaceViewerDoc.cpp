
// SpaceViewerDoc.cpp : CSpaceViewerDoc Ŭ������ ����
//

#include "stdafx.h"
#include "SpaceViewer.h"

#include "SpaceViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpaceViewerDoc

IMPLEMENT_DYNCREATE(CSpaceViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CSpaceViewerDoc, CDocument)
END_MESSAGE_MAP()


// CSpaceViewerDoc ����/�Ҹ�

CSpaceViewerDoc::CSpaceViewerDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CSpaceViewerDoc::~CSpaceViewerDoc()
{
}

BOOL CSpaceViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CSpaceViewerDoc serialization

void CSpaceViewerDoc::Serialize(CArchive& ar)
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


// CSpaceViewerDoc ����

#ifdef _DEBUG
void CSpaceViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSpaceViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSpaceViewerDoc ���
