#ifndef _MCOMMAND_DESC_H
#define _MCOMMAND_DESC_H

#include "MiNetLib.h"
#include "MSingleton.h"
#include "MCommandParameter.h"
#include "MCommandHandler.h"

namespace minet {

// Command Description Flag
enum MCDFlags
{
	MCDT_NOTINITIALIZED			= 0x0000,	///< �ʱ�ȭ�� �ȵ� ����

	MCDT_LOCAL					= 0x0001,	///< ���÷� ���޵Ǵ� Ŀ�ǵ�
	MCDT_MACHINE2MACHINE		= 0x0002,	///< �ӽſ��� �ӽ����� ���޵Ǵ� Ŀ�ǵ�
	MCDT_PEER2PEER				= 0x0004,	///< Peer���� Peer�� ���޵Ǵ� Ŀ�ǵ�, MACHINE2MACHINE�ʹ� ���� �����Ѵ�.
	MCDT_NON_ENCRYPTED			= 0x0008,	///< ��ȣȭ���� �ʴ� Ŀ�ǵ�

	MCDT_FILTER_CROWDED			= 0x0010,	///< ���� ȥ�⵵ ���Ϳɼ� ����
	MCDT_FILTER_IGNORE			= 0x0020,	///< � ���� ���Ϳ� ������� �ʴ´�
	
};

// ���� �� ���� Ŀ�ǵ�� �⺻������ ��ȣȭ���� �ʴ´�.
#define MCDT_SERVER2SERVER (MCDT_MACHINE2MACHINE | MCDT_NON_ENCRYPTED)

enum MCommandFlow
{
	MCF_ANY	= 0,	///< ������� �ʰų� C<->S
	MCF_C2S	= 1,	///< Client -> Server
	MCF_S2C	= 2		///< Server -> Client
};

/// Ŀ�ǵ��� �Ӽ��� ����
/// - MCommandDesc::m_nID �� ���� ID�� ���� MCommand�� MCommandDesc::m_ParamDescs�� ���ǵ� �Ķ���͸� ������ �ȴ�.
class MINET_API MCommandDesc {
protected:
	int				m_nID;					///< Command ID
	char			m_szName[256];			///< Name for Parsing
	char			m_szDescription[256];	///< Description of Command
	unsigned int	m_nFlag;				///< Command Description Flag
	MCommandFlow	m_nFlow;				///< Flow

	vector<MCommandParameterDesc*>	m_ParamDescs;	///< Parameters
public:
	MCommandHandler*	m_pHandler;		///< Ŀ�ǵ� �ڵ鷯
	MCommandHanderFunc* m_pFunc;		///< Ŀ�ǵ� �ڵ鷯 �Լ� ������
public:
	/// @param nID				Ŀ�ǵ� ID
	/// @param nFlag			Ŀ�ǵ� �÷���, MCDT_NOTINITIALIZED
	/// @param szName			Ŀ�ǵ� �̸�
	/// @param szDescription	Ŀ�ǵ� ����
	MCommandDesc(int nID, int nFlag, MCommandFlow nFlow, const char* szName, const char* szDescription);
	virtual ~MCommandDesc(void);

	/// MCommandParameterDesc �߰�
	void AddParamDesc(MCommandParameterDesc* pParamDesc);

	/// �÷��� �˻�
	bool IsFlag(int nFlag) const;
	/// ���̵� ���
	int GetID(void) const { return m_nID; }
	/// �̸� ���
	const char* GetName(void) const { return m_szName; }
	/// ���� ���
	const char* GetDescription(void) const { return m_szDescription; }
	MCommandFlow GetFlow() const { return m_nFlow; }
	/// Parameter Description ���
	MCommandParameterDesc* GetParameterDesc(size_t i) const {
		if(i<0 || i>=m_ParamDescs.size()) return NULL;
		return m_ParamDescs[i];
	}
	/// Parameter Description ���� ���
	size_t GetParameterDescCount(void) const 
	{
		return m_ParamDescs.size();
	}
	MCommandParameterType GetParameterType(size_t i) const
	{
		if(i<0 || i>=m_ParamDescs.size()) return MPT_END;
		return m_ParamDescs[i]->GetType();
	}
};

///< Command Description Map
class MINET_API MCommandDescMap
{
private:
	typedef map<int, MCommandDesc*>	MDescMap;
	MDescMap						m_DescMap;
public:
	MCommandDescMap();
	~MCommandDescMap();
	static MCommandDescMap* GetInstance();
	void Clear();

	int GetCount(void) const		{ return (int)m_DescMap.size(); }
	MCommandDesc* GetCommandDesc(int i);
	MCommandDesc* GetCommandDescByID(int nID);
	MCommandDesc* GetCommandDescByName(const char* chName);
	void AddCommandDesc(MCommandDesc* pCD);
};

inline MCommandDescMap* MGetCommandDescMap()
{
	return MCommandDescMap::GetInstance();
}

// Command Description Add Macro For Convenience
#define BEGIN_CMD_DESC()				{ MCommandDesc* pCD4m; 
#define END_CMD_DESC()					}
#define C(_a, _b, _c, _d)				{ pCD4m = new MCommandDesc(_a, _b, _c, #_a, _d); MGetCommandDescMap()->AddCommandDesc(pCD4m); }
#define P(_a, _b)						{ pCD4m->AddParamDesc(new MCommandParameterDesc(_a, _b)); }
#define CA(_a, _b)						{ pCM4m->AddAlias(_a, _b); }
#define P_MINMAX(_a, _b, _min, _max)	{ MCommandParameterDesc* pNewDesc = new MCommandParameterDesc(_a, _b);  \
	                                      pNewDesc->AddCondition(new MCommandParamConditionMinMax(_min, _max)); \
										  pCD4m->AddParamDesc(pNewDesc); }

} // namespace minet

#endif