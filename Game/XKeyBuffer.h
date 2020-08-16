#ifndef _XKEY_BUFFER_H
#define _XKEY_BUFFER_H

#include "XVirtualKeyDef.h"

/// @brief Ű ���ۿ��� ����ϴ� Ű���
/// @author �����
/// @date 2007-01-05
struct KEYNODE
{
	enum KeyState
	{
		PRESSONCE = 1,	///< ó�� ������ �߻�
		PRESSED = 2,	///< Ű�� ������ �ִ�
		RELEASED = 3	///< Ű�� ��������
	};

	XVirtualKey		nKeycode;		///< Ű��
	unsigned int	nTime;			///< �ð�
	KeyState		nState;			///< Ű���� ����

	bool			Pressed() const { return ((nState == PRESSONCE) || (nState == PRESSED)); }

	/// ������
	KEYNODE() : nKeycode(VKEY_NA), nTime(0), nState(PRESSED) {}
	/// ������
	KEYNODE(XVirtualKey key, DWORD time, KeyState state) : nKeycode(key), nTime(time), nState(state) {}
};

/// @class XKeyBuffer
/// @brief �޺� ó��, ���ӱ� � ����ϴ� �Է� Ű ����. ���� ť�� �Ǿ� ����.
/// @author �����
/// @date 2007-01-05
/// @see
class XKeyBuffer
{
private:
	static const int	MAX_KEY_BUFFER = 32;		///< Ű���� ������(32)
	vector<KEYNODE>		m_Buffer;					///< ����
	int					m_nBegin;					///< �������� ������
	int					m_nEnd;						///< �������� ��
	set<XVirtualKey>	m_Filter;					///< Ű ����. ���� �ִ� ���� ���ۿ� ���̵��� �Ѵ�.
public:
	XKeyBuffer();									///< ������
	~XKeyBuffer();									///< �Ҹ���
	void AddFilter(XVirtualKey nKey);				///< ���Ϳ� �߰�. ���⿡ �߰��� Ű���� ���ۿ� �ִ´�.
	bool CheckFilter(XVirtualKey nKey);				///< ���� üũ
	void Clear();									///< ���ۿ� ���� ��� ���� �����.
	void Push(KEYNODE& key_node);					///< ���ۿ� �߰�
	void Push2(KEYNODE& key_node);					///< ���ۿ� �߰� < �ڿ��� 2��°�� �߰�-_-;
	void Pop();
	void Remove(XVirtualKey nKey);
	int GetCapacity()	{ return MAX_KEY_BUFFER; }	///< �� ������ ��ȯ
	int GetSize();									///< ���� ���ۿ� ���� Ű���� ������
	bool IsEmpty();									///< ���۰� ������� üũ
	const KEYNODE& Get(int index) const;			///< Ű �� ��ȯ. index�� �������� ������ �Է��� Ű�̴�.
	void GetDebugString(wchar_t* pszOut, int nOutLen);
	bool IsExistPressedKeyNode(XVirtualKey nVirtualKey);
};

#endif // _XKEY_BUFFER_H