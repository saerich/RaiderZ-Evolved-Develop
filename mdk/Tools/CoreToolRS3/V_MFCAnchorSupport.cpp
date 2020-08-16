#include "StdAfx.h"
#include "V_MFCAnchorSupport.h"

CMFCAnchorSupport::CMFCAnchorSupport()
: m_pParent( NULL )
{
}

void CMFCAnchorSupport::Initialize(CWnd* pParent, int cx, int cy)
{
	m_pParent = pParent;

	RECT rt;
	rt.left = rt.top = 0;
	rt.right = cx; rt.bottom = cy;
	MapDialogRect( m_pParent->m_hWnd, &rt );

	m_nInitialCX = rt.right;
	m_nInitialCY = rt.bottom;
}


void CMFCAnchorSupport::RegisterAnchor( int nResourceID, CAnchorStyle anchorStyle )
{
	AnchorData anchorData;
	anchorData.style = anchorStyle;

	// �ʱ� ��ġ ����
	CWnd* pWnd = m_pParent->GetDlgItem( nResourceID );
	pWnd->GetWindowRect( &anchorData.initialRect );
	m_pParent->ScreenToClient( &anchorData.initialRect );

	m_mapAnchorWindows.insert( make_pair( nResourceID, anchorData ) );
}

void CMFCAnchorSupport::OnSize( int cx, int cy )
{
	for each (pair< int, AnchorData > item in m_mapAnchorWindows)
	{

		CAnchorStyle anchor = item.second.style;
		RECT rt = item.second.initialRect;

		// ������� ó��
		if( anchor & ANCHOR_LEFT )
		{
			if( anchor & ANCHOR_RIGHT )
			{
				// left&right �� ��� size �� �ٲ︸ŭ ���� ����
				rt.right += cx - m_nInitialCX;
			}

			// left only �� ��� �ƹ��͵� ����
		}else
		{
			if( anchor & ANCHOR_RIGHT )
			{
				// right only �̸� size �ٲ︸ŭ ��ġ�� �ٲ�
				rt.left += cx - m_nInitialCX;
				rt.right += cx - m_nInitialCX;
			}else
			{
				// left, right �Ѵ� ������ ��ġ�� �ٲ�
				int nHalfMove = (cx - m_nInitialCX) / 2;
				rt.left += nHalfMove;
				rt.right += nHalfMove;
			}
		}

		// �������� ó��
		if( anchor & ANCHOR_TOP )
		{
			if( anchor & ANCHOR_BOTTOM )
			{
				// top & bottom �� ��� size �� �ٲ︸ŭ ���� ����
				rt.bottom += cy - m_nInitialCY;
			}

			// top only �� ��� �ƹ��͵� ����
		}else
		{
			if( anchor & ANCHOR_BOTTOM )
			{
				// bottom only �̸� size �ٲ︸ŭ ��ġ�� �ٲ�
				rt.top += cy - m_nInitialCY;
				rt.bottom += cy - m_nInitialCY;
			}else
			{
				// top, bottom �Ѵ� ������ ��ġ�� �ٲ�
				int nHalfMove = (cy - m_nInitialCY) / 2;
				rt.top += nHalfMove;
				rt.bottom += nHalfMove;
			}
		}

		CWnd* pWnd = m_pParent->GetDlgItem( item.first );
		pWnd->MoveWindow( &rt );

	}
}
