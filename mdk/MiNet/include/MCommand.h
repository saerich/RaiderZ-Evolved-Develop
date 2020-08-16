#pragma once
#ifndef MCOMMAND_H
#define MCOMMAND_H

#include "MiNetLib.h"
#include "MUID.h"
#include "MCommandParameter.h"
#include "MCommandDesc.h"

#include <vector>
#include <list>
#include <set>
#include <deque>
using namespace std;

namespace minet {

class MCommandManager;

/// �ӽŰ� �ӽ� Ȥ�� ���ÿ� ���޵Ǵ� Ŀ���
class MINET_API MCommand
{
public:
	MUID						m_Sender;				///< �޼����� ������ �ӽ�(Ȥ�� ������Ʈ) UID
	MUID						m_Receiver;				///< �޼����� �޴� �ӽ�(Ȥ�� ������Ʈ) UID
	const MCommandDesc*			m_pCommandDesc;			///< �ش� Ŀ�ǵ��� Description
	vector<MCommandParameter*>	m_Params;				///< �Ķ����
protected:
	list<MUID>					m_AdditiveReceiverList;	///< �޼����� �޴� �ӽ��� �������� ��� ����� �߰��ؼ� �ִ´�. ������ ���.

	/// �ʱ�ȭ
	void Reset(void);
	/// �Ķ���� �ʱ�ȭ
	void ClearParam(void);

	MCommand(const MCommandDesc* pCommandDesc, MUID Receiver, MUID Sender);
public:
	MCommand(void);
	MCommand(int nDescID, MUID Sender, MUID Receiver);
	virtual ~MCommand(void);

	/// ID ���
	int GetID(void) const { return (m_pCommandDesc) ? m_pCommandDesc->GetID() : 0; }
	/// ���� ���
	const char* GetDescription(void){ return (m_pCommandDesc) ? m_pCommandDesc->GetDescription() : ""; }

	/// �Ķ���� �߰�
	bool AddParameter(MCommandParameter* pParam);
	/// �Ķ���� ���� ���
	int GetParameterCount(void) const;
	/// �Ķ���� ���
	MCommandParameter* GetParameter(int i) const;

	/// �ε����� �Ķ���͸� ��´�. �Ķ���� Ÿ���� ��Ȯ�ϰ� ������� ������ false�� �����Ѵ�.
	/// @brief �Ķ���� ���
	/// @param pValue	[out] �Ķ���� ��
	/// @param i		[in] �Ķ���� �ε���
	/// @param t		[in] �Ķ���� Ÿ��, ��Ȯ�� Ÿ���� �������� �Ѵ�.
	bool GetParameter(void* pValue, int i, MCommandParameterType t, int nBufferSize=-1) const;
    bool GetParameter(string& str, int i, MCommandParameterType t) const;	
	bool GetParameter(wstring& str, int i, MCommandParameterType t) const;
	
	template<typename T>
	bool GetBlob(T*& pPointer, int& nCount, int i) const
	{
		MCommandParameter* pParam = GetParameter(i);
		if(pParam->GetType()!=MPT_BLOB) return false;
		pPointer = static_cast<T*>(pParam->GetPointer());
		nCount = pParam->GetBlobCount();

		_ASSERT(NULL != pPointer || 0 >= nCount);
		if (NULL == pPointer && 0 < nCount) return false;

		return true;
	};

	template<typename T>
	bool GetBlob(vector<T>& vecT, int i) const
	{
		MCommandParameter* pParam = GetParameter(i);
		if(pParam->GetType()!=MPT_BLOB) return false;
		T* pPointer = static_cast<T*>(pParam->GetPointer());
		int nCount = pParam->GetBlobCount();

		_ASSERT(NULL != pPointer || 0 >= nCount);
		if (NULL == pPointer && 0 < nCount) return false;

		vecT.insert(vecT.end(), pPointer, pPointer + nCount);

		return true;
	};

	template<typename T>
	bool GetSingleBlob(T*& pPointer, int i) const
	{
		MCommandParameter* pParam = GetParameter(i);
		if(pParam->GetType()!=MPT_SINGLE_BLOB) return false;
		pPointer = static_cast<T*>(pParam->GetPointer());

		_ASSERT(NULL != pPointer);
		if (NULL == pPointer) return false;

		return true;
	};

	template<typename T>
	bool GetSingleBlob(T& data, int i) const
	{
		MCommandParameter* pParam = GetParameter(i);
		if(pParam->GetType()!=MPT_SINGLE_BLOB) return false;
		T* pPointer = static_cast<T*>(pParam->GetPointer());

		_ASSERT(NULL != pPointer);
		if (NULL == pPointer) return false;

		if (0 < pParam->GetSingleBlobValueSize())
		{
			data = (*pPointer);
		}		

		return true;
	};

	// ���ù��� �˾Ƽ� �߰�
	void AddReceiver(const MUID& uidReceiver);
	void AddReceiver(const vector<MUID>& vecReceivers);
	/// ���ù��� �ΰ������� �� �߰��Ѵ�.
	void AddAdditiveReceiver(const MUID& uidReceiver);

	MUID GetSenderUID(void)	const			{ return m_Sender; }
	void SetSenderUID(const MUID &uid)		{ m_Sender = uid; }
	MUID GetReceiverUID(void) const			{ return m_Receiver; }
	void SetReceiverUID(const MUID& uid)	{ m_Receiver = uid; }
	bool IsNoReceiver()						{ return (m_Receiver.IsInvalid()); }

	bool IsLocalCommand(void)				{ return (m_Sender==m_Receiver); }
	bool HasAdditiveReceivers()				{ return (!m_AdditiveReceiverList.empty()); }
	int GetReceiverCount();
	list<MUID>::iterator GetAdditiveReceiverListBegin()	{ return m_AdditiveReceiverList.begin(); }
	list<MUID>::iterator GetAdditiveReceiverListEnd()	{ return m_AdditiveReceiverList.end(); }

	/// ���� ������ ���� Ŀ�ǵ� ����
	MCommand* Clone(void) const;

	/// Description�� �°Բ� �����Ǿ��°�?
	bool CheckRule(void);	

	/// Ŀ�ǵ� �޸� �� �����ͷ� ����
	/// @param pData	[out] Ŀ�ǵ� ������ ��
	/// @param nSize	[in] Ŀ�ǵ� ������ �� �ִ� ������
	/// @return			�� ������
	int GetData(char* pData, uint16 nSize);
	/// Ŀ�ǵ� �޸� �� �����ͷκ��� ����
	/// @param pData	[in] Ŀ�ǵ� ������ ��
	/// @param pPM		[in] Ŀ�ǵ� �Ŵ���(MCommandDesc�� enum�� �� �ִ�.)
	/// @return			���� ����
	bool SetData(char* pData, uint16 nDataLen=USHRT_MAX);

	int GetSize();

	/// MCommandDesc���� ID ����
	void SetID(const MCommandDesc* pCommandDesc);
	/// ID ����
	void SetID(int nID);

	virtual MCommandResult Run();
};





// ���μ� ���̱� ���� ��ũ��
#define NEWCMD(_ID)		(new MCommand(_ID))
#define AP(_P)			AddParameter(new _P)
#define MKCMD(_C, _ID)									{ _C = NEWCMD(_ID); }
#define MKCMD1(_C, _ID, _P0)							{ _C = NEWCMD(_ID); _C->AP(_P0); }
#define MKCMD2(_C, _ID, _P0, _P1)						{ _C = NEWCMD(_ID); _C->AP(_P0); _C->AP(_P1); }
#define MKCMD3(_C, _ID, _P0, _P1, _P2)					{ _C = NEWCMD(_ID); _C->AP(_P0); _C->AP(_P1); _C->AP(_P2); }
#define MKCMD4(_C, _ID, _P0, _P1, _P2, _P3)				{ _C = NEWCMD(_ID); _C->AP(_P0); _C->AP(_P1); _C->AP(_P2); _C->AP(_P3); }
#define MKCMD5(_C, _ID, _P0, _P1, _P2, _P3, _P4)		{ _C = NEWCMD(_ID); _C->AP(_P0); _C->AP(_P1); _C->AP(_P2); _C->AP(_P3); _C->AP(_P4); }
#define MKCMD6(_C, _ID, _P0, _P1, _P2, _P3, _P4, _P5)	{ _C = NEWCMD(_ID); _C->AP(_P0); _C->AP(_P1); _C->AP(_P2); _C->AP(_P3); _C->AP(_P4); _C->AP(_P5); }


// Short Name
typedef MCommand				MCmd;
typedef MCommandDesc			MCmdDesc;

} // namespace minet

#endif