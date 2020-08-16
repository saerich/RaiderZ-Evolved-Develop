#pragma once
#include <MiNetCommon.h>

/*
Ŀ�ǵ� ���ε� ����

* �ϴ� �Ķ���� 5�������� ���������ϴ�.

* ù��° �Ķ���ʹ� �׻� MCommand* �̴�.
* �� �Ķ���� Ÿ�Կ� ���� ������ �Լ� �Ķ���� �������� �޾ƾ� �Ѵ�
* ���� Ÿ���� Ŭ������ �ΰ��̻��� �ν��Ͻ��� ���ε��ϸ� �ȵ˴ϴ�. ( �������Ҽ� �־� ��� �޾Ƴ����ϴ� )

MPT_INT		-> int
MPT_UINT	-> unsigned int
MPT_FLOAT	-> float
MPT_DOUBLE	-> double
MPT_CHAR	-> char
MPT_UCHAR	-> unsigned char
MPT_SHORT	-> short
MPT_USHORT	-> unsigned short
MPT_INT64	-> int64
MPT_UINT64	-> unsigned int64
MPT_BOOL	-> bool
MPT_STR		-> const char*
MPT_VECTOR	-> MVector3
MPT_SVECTOR	-> MShortVector
MPT_UID		-> MUID
MPT_ID		-> MID
MPT_BLOB	-> MBlob*


// Ŀ�ǵ� ��ũ����
	C(MC_MATCH_LOGIN, MCDT_MACHINE2MACHINE, "Login Match Server" )
		P(MPT_STR, "UserID");
		P(MPT_STR, "Password");
		P(MPT_INT, "CommandVersion");
		P(MPT_UINT, "nChecksumPack");
		P(MPT_BLOB,	"MD5Value");


	C(ZC_TEST_TEST, MCDT_LOCAL, "testCommand");
		P( MPT_STR, "UserID" )

// ���� Ŀ�ǵ忡 ���� �ڵ鷯 �ۼ� ����
class GunzCommandHandler : public MHandlerBase
{
public:
	MCommandResult OnLogin(MCommand* pCommand, const char* szUserID, const char* szPassword, int nCommandVersion, unsigned int nCheckSum, MBlob blob);
	MCommandResult OnTestTest(MCommand* pCommand, const char* szBlah);
} g_gunzCommandHandler;

// Ŀ�´�������
class GunzServer : public MCommandBinderable
{
public:
} g_gunzServer;


main()
{
	MCommandBinder<GunzCommandHandler> binder(&g_gunzCommandHandler,&g_gunzServer);
	binder.bind(MC_MATCH_LOGIN,	&MMatchServer::OnLogin);
	binder.bind(MC_TEST_TEST,	&MMatchServer::OnTestTest);

	//	or

	//	MCommandBinder<GunzCommandHandler>(&g_gunzCommandHandler,&g_gunzServer)
	//		.bind(MC_MATCH_LOGIN,	&MMatchServer::OnLogin);
	//		.bind(MC_TEST_TEST,		&MMatchServer::OnTestTest);
	//

}



*/

namespace minet {

class MCommand;
class MCommandHandlerCaller;
class MHandlerBase;

// MCommandBinder �� ���ε��Ҽ� �ִ� Ŭ����
class MCommandDistributer
{
protected:
	std::map<int,MCommandHandlerCaller> m_cmdHandlers;	///< map : command id -> Ŀ�ǵ� �ڵ鷯

public:
	void			BindCommandHandler(int nCmdID, MCommandHandlerCaller& info);
	MHandlerBase*	GetCommandHandlerInstance(int nCmdID);
	MCommandResult	Distribute(MCommand* pCommand);
};

/// Command Handler�� ���̽� Ŭ����
class MHandlerBase
{
public:
	// Ŀ�ǵ带 �����Ѵ�, �������� true, ������ ������ false �� �����ؾ� �Ѵ�
	virtual bool Validate(MCommand* pCommand) { return true; }
};

/// MHandlerFunctionTable �� ���̽�Ŭ����, MHandlerFunctionTable�� ���� �Ķ���� ���� ������ �ڵ鷯 ��ǵ��� ���̺��� �����Ѵ�
class MHandlerFunctionTableBase
{
	friend MCommandHandlerCaller;

protected:
	MHandlerBase*	m_pInstance;			// �ڵ鷯�� �ν��Ͻ�

	virtual MCommandResult OnCommand(MCommand* pCommand, int nHandlerID) { return CR_FALSE; }
};

/// ���� Ŀ�ǵ� �ڵ鷯�� �ҷ��ִ� Ŭ����
class MCommandHandlerCaller
{
	MHandlerFunctionTableBase*	m_pHandlerFunctionTable;
	int							m_nHandlerID;

public:
	MCommandHandlerCaller(MHandlerFunctionTableBase* pHandlerFunctionTable, int	nHandlerID) 
		: m_pHandlerFunctionTable(pHandlerFunctionTable), m_nHandlerID(nHandlerID) { }
	MCommandHandlerCaller() { }

	inline MCommandResult OnCommand(MCommand* pCommand) { 
		return m_pHandlerFunctionTable->OnCommand(pCommand, m_nHandlerID);
	}

	MHandlerBase* GetHandlerInstance() { return m_pHandlerFunctionTable->m_pInstance; }
	
};

/// MCommunicator �� Handler ����� ���ε����ִ� Ŭ����
template<typename T_HANDLER_CLASS>
class MCommandBinder {
	T_HANDLER_CLASS*			m_pInstance;
	MCommandDistributer*		m_pDistributer;
public:
	/// pInstance : Ŀ�ǵ� �ڵ鷯, T ������ instance, pCommunicator : Ŀ�ǵ� Ŀ�´������� ( Ŀ�ǵ尡 ������ �� )
	MCommandBinder(T_HANDLER_CLASS* pInstance, MCommandDistributer* pDistributer) 
		: m_pInstance(pInstance), m_pDistributer(pDistributer) { }

	template< typename _FPT > struct Type2Enum { enum { Result = -1 }; };
	template<> struct Type2Enum< int >				{ enum { Result = MPT_INT }; };
	template<> struct Type2Enum< unsigned int >		{ enum { Result = MPT_UINT }; };
	template<> struct Type2Enum< float >			{ enum { Result = MPT_FLOAT }; };
	template<> struct Type2Enum< double >			{ enum { Result = MPT_DOUBLE }; };
	template<> struct Type2Enum< char >				{ enum { Result = MPT_CHAR }; };
	template<> struct Type2Enum< unsigned char >	{ enum { Result = MPT_UCHAR }; };
	template<> struct Type2Enum< short >			{ enum { Result = MPT_SHORT }; };
	template<> struct Type2Enum< unsigned short>	{ enum { Result = MPT_USHORT }; };
	template<> struct Type2Enum< int64 >			{ enum { Result = MPT_INT64 }; };
	template<> struct Type2Enum< uint64 >			{ enum { Result = MPT_UINT64 }; };
	template<> struct Type2Enum< bool >				{ enum { Result = MPT_BOOL }; };
	template<> struct Type2Enum< const char* >		{ enum { Result = MPT_STR }; };
	template<> struct Type2Enum< MVector3>			{ enum { Result = MPT_VECTOR }; };
	template<> struct Type2Enum< MSVector >			{ enum { Result = MPT_SVECTOR }; };
	template<> struct Type2Enum< MUID >				{ enum { Result = MPT_UID }; };
	template<> struct Type2Enum< MID >				{ enum { Result = MPT_ID }; };
	template<> struct Type2Enum< MBlob* >			{ enum { Result = MPT_BLOB }; };
	template<> struct Type2Enum< const wchar_t* >	{ enum { Result = MPT_WSTR }; };


	/// MHandlerFunctionTable ���� �Ķ���� ���� ������ �ڵ鷯 ��ǵ��� ���̺��� �����Ѵ�
	template<typename T_PFUNC>
	class MHandlerFunctionTable : public MHandlerFunctionTableBase {
	protected:
//		T_HANDLER_CLASS*	m_pInstance;			// �ڵ鷯�� �ν��Ͻ�
		vector<T_PFUNC>		m_handlerFunctionTable;	// �ڵ鷯�� (������) ������̺�

	public:
		MHandlerFunctionTable(T_HANDLER_CLASS* pInstance) { m_pInstance = pInstance; }

		/// �ڵ鷯�� ����Ѵ�, ���ϰ��� nHandlerID (= m_handlerFuncTable�� �ε���)
		unsigned int AddHandler(T_PFUNC pFunc)
		{
			m_handlerFunctionTable.push_back(pFunc);
			return (unsigned int)m_handlerFunctionTable.size()-1;
		}

		T_PFUNC GetHandler( int nIndex)
		{
			return m_handlerFunctionTable[nIndex];
		}

		T_HANDLER_CLASS* GetHandlerInstance() { return static_cast<T_HANDLER_CLASS*> (m_pInstance); }
	};

	/// �Ķ���Ͱ� 0���� Ŀ�ǵ� �ڵ鷯 ����� �ҷ��ִ� �Լ�
	class MHandlerCaller0 : public MHandlerFunctionTable< MCommandResult (T_HANDLER_CLASS::*)(MCommand*) >
	{
		typedef MCommandResult (T_HANDLER_CLASS::*PFUNC)(MCommand*);

	public:
		MHandlerCaller0(T_HANDLER_CLASS* pInstance) : MHandlerFunctionTable(pInstance) { }

		MCommandResult OnCommand(MCommand* pCommand, int nHandlerID)
		{
			PFUNC func = GetHandler(nHandlerID);
			return (GetHandlerInstance()->*func)(pCommand);
		}
	};

	/// �Ķ���Ͱ� 1���� Ŀ�ǵ� �ڵ鷯 ����� �ҷ��ִ� �Լ�
	template<typename T1>
	class MHandlerCaller1 : public MHandlerFunctionTable< MCommandResult (T_HANDLER_CLASS::*)(MCommand*,T1) >
	{
		typedef MCommandResult (T_HANDLER_CLASS::*PFUNC)(MCommand*,T1);

	public:
		MHandlerCaller1(T_HANDLER_CLASS* pInstance) : MHandlerFunctionTable(pInstance) { }

		MCommandResult OnCommand(MCommand* pCommand, int nHandlerID)
		{
			// ����� �Ķ���� Ÿ�԰� Ŀ�ǵ��� �Ķ���� Ÿ�� �´��� �˻�
			_ASSERT(Type2Enum<T1>::Result == pCommand->GetParameter(0)->GetType());

			T1 p1 = *(T1*)pCommand->GetParameter(0)->GetPointerRaw();

			PFUNC func = GetHandler(nHandlerID);
			return (GetHandlerInstance()->*func)(pCommand,p1);
		}
	};

	/// �Ķ���Ͱ� 2���� Ŀ�ǵ� �ڵ鷯 ����� �ҷ��ִ� �Լ�
	template<typename T1,typename T2>
	class MHandlerCaller2 : public MHandlerFunctionTable< MCommandResult (T_HANDLER_CLASS::*)(MCommand*,T1,T2) >
	{
		typedef MCommandResult (T_HANDLER_CLASS::*PFUNC)(MCommand*,T1,T2);

	public:
		MHandlerCaller2(T_HANDLER_CLASS* pInstance) : MHandlerFunctionTable(pInstance) { }

		MCommandResult OnCommand(MCommand* pCommand, int nHandlerID)
		{
			// ����� �Ķ���� Ÿ�԰� Ŀ�ǵ��� �Ķ���� Ÿ�� �´��� �˻�
			_ASSERT(Type2Enum<T1>::Result == pCommand->GetParameter(0)->GetType());
			_ASSERT(Type2Enum<T2>::Result == pCommand->GetParameter(1)->GetType());

			T1 p1 = *(T1*)pCommand->GetParameter(0)->GetPointerRaw();
			T2 p2 = *(T2*)pCommand->GetParameter(1)->GetPointerRaw();

			PFUNC func = GetHandler(nHandlerID);
			return (GetHandlerInstance()->*func)(pCommand,p1,p2);
		}
	};

	/// �Ķ���Ͱ� 3���� Ŀ�ǵ� �ڵ鷯 ����� �ҷ��ִ� �Լ�
	template<typename T1,typename T2,typename T3>
	class MHandlerCaller3 : public MHandlerFunctionTable< MCommandResult (T_HANDLER_CLASS::*)(MCommand*,T1,T2,T3) >
	{
		typedef MCommandResult (T_HANDLER_CLASS::*PFUNC)(MCommand*,T1,T2,T3);

	public:
		MHandlerCaller3(T_HANDLER_CLASS* pInstance) : MHandlerFunctionTable(pInstance) { }

		MCommandResult OnCommand(MCommand* pCommand, int nHandlerID)
		{
			// ����� �Ķ���� Ÿ�԰� Ŀ�ǵ��� �Ķ���� Ÿ�� �´��� �˻�
			_ASSERT(Type2Enum<T1>::Result == pCommand->GetParameter(0)->GetType());
			_ASSERT(Type2Enum<T2>::Result == pCommand->GetParameter(1)->GetType());
			_ASSERT(Type2Enum<T3>::Result == pCommand->GetParameter(2)->GetType());

			T1 p1 = *(T1*)pCommand->GetParameter(0)->GetPointerRaw();
			T2 p2 = *(T2*)pCommand->GetParameter(1)->GetPointerRaw();
			T3 p3 = *(T3*)pCommand->GetParameter(2)->GetPointerRaw();

			PFUNC func = GetHandler(nHandlerID);
			return (GetHandlerInstance()->*func)(pCommand,p1,p2,p3);
		}
	};

	/// �Ķ���Ͱ� 4���� Ŀ�ǵ� �ڵ鷯 ����� �ҷ��ִ� �Լ�
	template<typename T1,typename T2,typename T3,typename T4>
	class MHandlerCaller4 : public MHandlerFunctionTable< MCommandResult (T_HANDLER_CLASS::*)(MCommand*,T1,T2,T3,T4) >
	{
		typedef MCommandResult (T_HANDLER_CLASS::*PFUNC)(MCommand*,T1,T2,T3,T4);

	public:
		MHandlerCaller4(T_HANDLER_CLASS* pInstance) : MHandlerFunctionTable(pInstance) { }

		MCommandResult OnCommand(MCommand* pCommand, int nHandlerID)
		{
			// ����� �Ķ���� Ÿ�԰� Ŀ�ǵ��� �Ķ���� Ÿ�� �´��� �˻�
			_ASSERT(Type2Enum<T1>::Result == pCommand->GetParameter(0)->GetType());
			_ASSERT(Type2Enum<T2>::Result == pCommand->GetParameter(1)->GetType());
			_ASSERT(Type2Enum<T3>::Result == pCommand->GetParameter(2)->GetType());
			_ASSERT(Type2Enum<T4>::Result == pCommand->GetParameter(3)->GetType());

			T1 p1 = *(T1*)pCommand->GetParameter(0)->GetPointerRaw();
			T2 p2 = *(T2*)pCommand->GetParameter(1)->GetPointerRaw();
			T3 p3 = *(T3*)pCommand->GetParameter(2)->GetPointerRaw();
			T4 p4 = *(T4*)pCommand->GetParameter(3)->GetPointerRaw();

			PFUNC func = GetHandler(nHandlerID);
			return (GetHandlerInstance()->*func)(pCommand,p1,p2,p3,p4);
		}
	};

	/// �Ķ���Ͱ� 5���� Ŀ�ǵ� �ڵ鷯 ����� �ҷ��ִ� �Լ�
	template<typename T1,typename T2,typename T3,typename T4,typename T5>
	class MHandlerCaller5 : public MHandlerFunctionTable< MCommandResult (T_HANDLER_CLASS::*)(MCommand*,T1,T2,T3,T4,T5) >
	{
		typedef MCommandResult (T_HANDLER_CLASS::*PFUNC)(MCommand*,T1,T2,T3,T4,T5);

	public:
		MHandlerCaller5(T_HANDLER_CLASS* pInstance) : MHandlerFunctionTable(pInstance) { }

		MCommandResult OnCommand(MCommand* pCommand, int nHandlerID)
		{
			// ����� �Ķ���� Ÿ�԰� Ŀ�ǵ��� �Ķ���� Ÿ�� �´��� �˻�
			_ASSERT(Type2Enum<T1>::Result == pCommand->GetParameter(0)->GetType());
			_ASSERT(Type2Enum<T2>::Result == pCommand->GetParameter(1)->GetType());
			_ASSERT(Type2Enum<T3>::Result == pCommand->GetParameter(2)->GetType());
			_ASSERT(Type2Enum<T4>::Result == pCommand->GetParameter(3)->GetType());
			_ASSERT(Type2Enum<T5>::Result == pCommand->GetParameter(4)->GetType());

			T1 p1 = *(T1*)pCommand->GetParameter(0)->GetPointerRaw();
			T2 p2 = *(T2*)pCommand->GetParameter(1)->GetPointerRaw();
			T3 p3 = *(T3*)pCommand->GetParameter(2)->GetPointerRaw();
			T4 p4 = *(T4*)pCommand->GetParameter(3)->GetPointerRaw();
			T5 p5 = *(T5*)pCommand->GetParameter(4)->GetPointerRaw();

			PFUNC func = GetHandler(nHandlerID);
			return (GetHandlerInstance()->*func)(pCommand,p1,p2,p3,p4,p5);
		}
	};

	/// �Ķ���Ͱ� 0���� �Լ��� ���ε�(�ڵ��� communicator)���ִ� �Լ�
	MCommandBinder<T_HANDLER_CLASS>& Bind(int nID, MCommandResult (T_HANDLER_CLASS::*func)(MCommand*)) 
	{ 
		static MHandlerCaller0 handlerCaller( m_pInstance );
		_ASSERT(handlerCaller.GetHandlerInstance()==m_pInstance);
		MCommandHandlerCaller info(&handlerCaller,handlerCaller.AddHandler(func));
		m_pDistributer->BindCommandHandler(nID,info);
		return *this;
	};

	/// �Ķ���Ͱ� 1���� �Լ��� ���ε����ִ� �Լ�
	template<typename T1>
	MCommandBinder<T_HANDLER_CLASS>& Bind(int nID, MCommandResult (T_HANDLER_CLASS::*func)(MCommand*,T1)) 
	{ 
		static MHandlerCaller1<T1> handlerCaller( m_pInstance );
		_ASSERT(handlerCaller.GetHandlerInstance()==m_pInstance);	///< ���� ���ε��� �ι��Ǹ� �������մϴ�.
		MCommandHandlerCaller info(&handlerCaller,handlerCaller.AddHandler(func));
		m_pDistributer->BindCommandHandler(nID,info);
		return *this;
	};

	/// �Ķ���Ͱ� 2���� �Լ��� ���ε����ִ� �Լ�
	template<typename T1,typename T2>
	MCommandBinder<T_HANDLER_CLASS>& Bind(int nID, MCommandResult (T_HANDLER_CLASS::*func)(MCommand*,T1,T2)) 
	{ 
		static MHandlerCaller2<T1,T2> handlerCaller( m_pInstance );
		_ASSERT(handlerCaller.GetHandlerInstance()==m_pInstance);	///< ���� ���ε��� �ι��Ǹ� �������մϴ�.
		MCommandHandlerCaller info(&handlerCaller,handlerCaller.AddHandler(func));
		m_pDistributer->BindCommandHandler(nID,info);
		return *this;
	};

	/// �Ķ���Ͱ� 3���� �Լ��� ���ε����ִ� �Լ�
	template<typename T1,typename T2,typename T3>
	MCommandBinder<T_HANDLER_CLASS>& Bind(int nID, MCommandResult (T_HANDLER_CLASS::*func)(MCommand*,T1,T2,T3)) 
	{ 
		static MHandlerCaller3<T1,T2,T3> handlerCaller( m_pInstance );
		_ASSERT(handlerCaller.GetHandlerInstance()==m_pInstance);	///< ���� ���ε��� �ι��Ǹ� �������մϴ�.
		MCommandHandlerCaller info(&handlerCaller,handlerCaller.AddHandler(func));
		m_pDistributer->BindCommandHandler(nID,info);
		return *this;
	};

	/// �Ķ���Ͱ� 4���� �Լ��� ���ε����ִ� �Լ�
	template<typename T1,typename T2,typename T3,typename T4>
	MCommandBinder<T_HANDLER_CLASS>& Bind(int nID, MCommandResult (T_HANDLER_CLASS::*func)(MCommand*,T1,T2,T3,T4)) 
	{ 
		static MHandlerCaller4<T1,T2,T3,T4> handlerCaller( m_pInstance );
		_ASSERT(handlerCaller.GetHandlerInstance()==m_pInstance);	///< ���� ���ε��� �ι��Ǹ� �������մϴ�.
		MCommandHandlerCaller info(&handlerCaller,handlerCaller.AddHandler(func));
		m_pDistributer->BindCommandHandler(nID,info);
		return *this;
	};

	/// �Ķ���Ͱ� 5���� �Լ��� ���ε����ִ� �Լ�
	template<typename T1,typename T2,typename T3,typename T4,typename T5>
	MCommandBinder<T_HANDLER_CLASS>& Bind(int nID, MCommandResult (T_HANDLER_CLASS::*func)(MCommand*,T1,T2,T3,T4,T5)) 
	{ 
		static MHandlerCaller5<T1,T2,T3,T4,T5> handlerCaller( m_pInstance );
		_ASSERT(handlerCaller.GetHandlerInstance()==m_pInstance);	///< ���� ���ε��� �ι��Ǹ� �������մϴ�.
		MCommandHandlerCaller info(&handlerCaller,handlerCaller.AddHandler(func));
		m_pDistributer->BindCommandHandler(nID,info);
		return *this;
	};
};

} // namespace minet
