#pragma once

#include "MemPool.h"

//////////////////////////////////////////////////////////////////////////
// MImagePackerNode
// 2D �̹����� �ϳ��� ��ġ�� ���� Ŭ����
struct MImagePackerNode
{
public:
	// ������ �̹���. ũ�⸸ �������ִ�
	struct MImage
	{
		int nID;
		int nWidth;
		int nHeight;

		bool Fit(const MRect& targetRc) const
		{
			if(targetRc.GetWidth()<nWidth) return false;
			if(targetRc.GetHeight()<nHeight) return false;
			return true;
		}
		bool FitPerfectly(const MRect& targetRc) const
		{
			if(targetRc.GetWidth()==nWidth &&
				targetRc.GetHeight()==nHeight) return true;
			return false;
		}
	};

	// ��ȸ �湮��
	struct MImagePackerNodeVisitor
	{
		virtual void Visit( MImage& _Image, const MRect& _ImageRange ) = 0;
	};

protected:
	MRect				rc;
	MImage				m_img;	// m_img.nID �� 0 �̸� ����ִ� �����̴�
	int					m_nAvailableSpace;	///< ��밡���� �ȼ� ��
	int					m_nMaxWidth;		///< ��밡���� ���� �� �ִ� �ʺ�
	int					m_nMaxHeight;		///< ��밡���� ���� �� �ִ� ����
	
public:
	MImagePackerNode(const MRect& _rc);
	virtual ~MImagePackerNode();

	MImagePackerNode*	parent;
	MImagePackerNode*	child[2];

	int					GetWidth() const { return rc.GetWidth(); }
	int					GetHeight() const { return rc.GetHeight(); }

	int					GetTop() const { return rc.top; }
	int					GetLeft() const { return rc.left; }
	int					GetRight() const { return rc.right; }
	int					GetBottom() const { return rc.bottom; }

	int					GetImageID() const { return m_img.nID; }

	void				UpdateAvailable();
	int					GetAvailableSpace() { return m_nAvailableSpace; }

	/// �̹����� �����Ѵ�, ������ ������ NULL �� �����Ѵ�.
	MImagePackerNode*	Insert(const MImage& img);
	
	/// �̹����� �����Ѵ�. ���� �������� �����Ҷ��� ������ �����Ѵ�.
	void				Remove(MImagePackerNode* pNode);

	/// �ڽ� ������ ��ȸ�Ѵ�.
	void				Traverse( MImagePackerNodeVisitor* pVisitor );
};

//////////////////////////////////////////////////////////////////////////
// ������ ����
/*

	// ��ü ��带 �ϳ� �����
	MImagePackerNode aRootNode(MRect(0,0,256,256));

	for(int i=0;i< nImages ; i++)
	{
		MImagePackerNode::MImage img;
		img.nID = i+1;	// 0 �� �ȵ�
		img.nWidth = nWidth;	// i��° �̹��� �ʺ�
		img.nHeight = nHeight;	// i��° �̹��� ����

		MImagePackerNode* pNode = aRootNode.Insert(img);
		if(!pNode) {
			// ���� : �뷮�� ���ڶ�
			return false;
		}
	}

	// ����. Ʈ���� �����Ǿ��ִ�. ������ �����Ͽ� �����Ѵ�
*/