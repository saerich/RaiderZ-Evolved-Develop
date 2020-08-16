#include "StdAfx.h"
#include "MImagePacker.h"

// ���� : http://www.flipcode.org/cgi-bin/fcarticles.cgi?show=64423

MImagePackerNode::MImagePackerNode(const MRect& _rc)
{ 
	parent = child[0] = child[1] = NULL; 
	m_img.nID = 0;
	rc = _rc;
	m_nAvailableSpace = rc.GetHeight() * rc.GetWidth();
	m_nMaxWidth = rc.GetWidth();
	m_nMaxHeight = rc.GetHeight();
}

MImagePackerNode::~MImagePackerNode() 
{ 
	if(child[0]) 
	{ 
		delete child[0]; 
		_ASSERT(child[1]);
		delete child[1]; 
	}
}

void MImagePackerNode::Remove(MImagePackerNode* pChildNode)
{
	_ASSERT(pChildNode->child[0]==NULL && pChildNode->child[1]==NULL); // ���� ��忩�� �Ѵ�

	int nNodeToDel;	// �ΰ��� �ڽĳ���� ���� ���
	int nTheOtherNode; // �ٸ� �ڽĳ��
	if(child[0]==pChildNode)
	{
		nNodeToDel = 0;
		nTheOtherNode = 1;
	}else
	{
		_ASSERT(child[1]==pChildNode);
		nNodeToDel = 1;
		nTheOtherNode = 0;
	}

	// nNodeToDel�� ����µ�, nTheOtherNode�� ���ܳ���̰� ��������� ���� ���� ��带 ��ģ��
	if(child[nTheOtherNode]->m_img.nID==0 && child[nTheOtherNode]->child[0]==NULL)
	{
		delete child[nTheOtherNode];
		child[nTheOtherNode] = NULL;
		delete child[nNodeToDel];
		child[nNodeToDel] = NULL;
		UpdateAvailable();
		_ASSERT(m_img.nID==0);
	}else
	{
		// �׷��� ������ �׸��� ����
		child[nNodeToDel]->m_img.nID = 0;
		child[nNodeToDel]->UpdateAvailable();
	}
}

MImagePackerNode* MImagePackerNode::Insert(const MImage& img)
{
	if (child[0]!=NULL) // if we're not a leaf then
	{
		// ������ ���ڶ�� ������ �ʿ����
		if(m_nAvailableSpace < img.nWidth * img.nHeight)
			return NULL;

		_ASSERT(m_nAvailableSpace == (child[0]->m_nAvailableSpace + child[1]->m_nAvailableSpace) );

		// �̹����� w,h �� ���� ��� w,h ���� ũ�ٸ� Ž���� ������ �ʿ����
		if(m_nMaxWidth < img.nWidth || m_nMaxHeight < img.nHeight)
			return NULL;

		_ASSERT(child[1]!=NULL);
		// (try inserting into first child)
		MImagePackerNode* newNode = child[0]->Insert( img );
		if (newNode != NULL) return newNode;
		// (no room, insert into second)
		return child[1]->Insert( img );
	}
	else
	{
		// (if there's already a lightmap here, return)
		if (m_img.nID != 0) return NULL;

		// (if we're too small, return)
		if (!img.Fit(rc))
			return NULL;

		// (if we're just right, accept)
		if(img.FitPerfectly(rc))
		{
			m_img = img;
			UpdateAvailable();
			return this;
		}

		// (otherwise, gotta split this node and create some kids)

		// (decide which way to split)
		int dw = rc.GetWidth() - img.nWidth;
		int dh = rc.GetHeight() - img.nHeight;

		int width = img.nWidth;
		int height = img.nHeight;

		if ( dw > dh )
		{
			child[0] = new MImagePackerNode(MRect(rc.left, rc.top, rc.left+width, rc.bottom) );
			child[1] = new MImagePackerNode(MRect(rc.left+width, rc.top, rc.right, rc.bottom) );
		}
		else
		{
			child[0] = new MImagePackerNode( MRect(rc.left, rc.top, rc.right, rc.top+height) );
			child[1] = new MImagePackerNode( MRect(rc.left, rc.top+height, rc.right, rc.bottom) );
		}
		child[0]->parent = this;
		child[1]->parent = this;

		_ASSERT(m_nAvailableSpace = child[0]->m_nAvailableSpace + child[1]->m_nAvailableSpace);
//		UpdateAvailable();

		// (insert into first child we created)
		return child[0]->Insert( img );
	}
}

void MImagePackerNode::UpdateAvailable()
{
	if(child[0])
	{
		_ASSERT(child[1]!=NULL);
		m_nAvailableSpace = child[0]->m_nAvailableSpace + child[1]->m_nAvailableSpace;
		m_nMaxWidth = max( child[0]->m_nMaxWidth , child[1]->m_nMaxWidth );
		m_nMaxHeight = max( child[0]->m_nMaxHeight , child[1]->m_nMaxHeight );
	}
	else
	{
		if(GetImageID()==0)
		{
			m_nAvailableSpace = GetWidth() * GetHeight();
			m_nMaxHeight = GetHeight();
			m_nMaxWidth =  GetWidth();
		}
		else
		{
			m_nAvailableSpace = 0;
			m_nMaxHeight = 0;
			m_nMaxWidth = 0;
		}
	}

	if(parent)
		parent->UpdateAvailable();
}

void MImagePackerNode::Traverse( MImagePackerNodeVisitor* pVisitor )
{
	// Pre Order Traverse
	/*_ASSERT( pVisitor );
	pVisitor->Visit(m_img, rc);

	if (child[0])
		child[0]->Traverse(pVisitor);

	if (child[1])
		child[1]->Traverse(pVisitor);*/

	// by pok. RLightMapPacker�� Post Order�� �Ǿ��ֱ淡 �����ϰ� �����߽��ϴ�.
	_ASSERT(pVisitor);
	if (child[0]!=NULL)
	{
		child[0]->Traverse(pVisitor);
		child[1]->Traverse(pVisitor);
	}
	else
	{
		if(m_img.nID!=0)
			pVisitor->Visit(m_img, rc);
	}
}
