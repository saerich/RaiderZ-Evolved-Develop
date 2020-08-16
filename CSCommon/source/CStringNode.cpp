#include "stdafx.h"
#include "CStringNode.h"

CStringNode::CStringNode()
{
	// do nothing
}

CStringNode::~CStringNode()
{
	// do nothing
}

const TCHAR* CStringNode::GetText(void) const
{
	return m_strText.c_str();
}

const TCHAR* CStringNode::GetAttribute(Attribute attr) const
{
	attribute_map::const_iterator it = m_mapAttribute.find(attr);

	if (it == m_mapAttribute.end())
	{
		// �Ӽ��� ���ٸ�, ���� ���ڿ��� ����Ѵ�.
		return m_strText.c_str();
	}

	return it->second.c_str();
}

void CStringNode::SetText(tstring strText)
{
	m_strText = strText;
}

void CStringNode::SetAttribute(Attribute attr, tstring strText)
{
	m_mapAttribute.insert(make_pair(attr, strText));
}
