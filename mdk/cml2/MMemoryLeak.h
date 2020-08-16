#ifndef _MMEMORY_LEAK_H
#define _MMEMORY_LEAK_H

/************************************************************
MODULE NAME : MemoryLeak

HISTORY : 
Date		Name		Description
2007-02-12	�����		
DESCRIPTION
	- MMemoryLeak.h�� ��Ŭ����ϱ⸸ �ϸ� ����� Leak �޼����� ��µ˴ϴ�.
	- Debug�� ������ �����մϴ�.
	- http://msdn.microsoft.com/library/kor/default.asp?url=/library/KOR/vsdebug/html/_core_Using_the_Debug_Heap.asp

	- �޸� �Ҵ� ����
	  - ���ú��� : CC CC CC CC
	  - �Ҵ��Ѹ޸� : CD CD CD CD
	  - �����ȸ޸�: DD DD DD DD
	  - �Ҵ��Ѹ޸𸮾糡: FD FD FD FD

*************************************************************/

#ifdef _MSC_VER
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC	// �޸� ���� Ž���� ���ؼ� ������ �־�� �Ѵ�.
#include <crtdbg.h>

#ifndef _CONSOLE
#include <cstdlib>
#endif

class MMemoryLeak
{
public:
	MMemoryLeak()
	{
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		// �ܼ� ���α׷��� ���
#ifdef _CONSOLE

		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
		_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
		_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
		_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );


#endif	// _CONSOLE

// mfc ����ϴ� ������Ʈ�� DEBUG_NEW ����ϼ���.
#if defined (__AFX_H__)

#ifndef new
#define new DEBUG_NEW 
#endif

#else

#define DEBUG_NORMALBLOCK new(_NORMAL_BLOCK, __FILE__, __LINE__)

#ifdef new
#undef new
#endif

#define new DEBUG_NORMALBLOCK


#endif




#ifdef malloc
#undef malloc
#endif
#define malloc(s) (_malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ ))

#ifdef calloc
#undef calloc
#endif
#define calloc(c, s)      _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)

#ifdef realloc
#undef realloc
#endif
#define realloc(p, s)     _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)

#ifdef _expand
#undef _expand
#endif
#define _expand(p, s)     _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)

#ifdef free
#undef free
#endif
#define free(p)           _free_dbg(p, _NORMAL_BLOCK)

#ifdef _msize
#undef _msize
#endif
#define _msize(p)         _msize_dbg(p, _NORMAL_BLOCK)

	}

	/// ���� ���Ἲ�� Ȯ��. assert(MMemoryLeak::CheckMemory()); �̷������� ����ϸ� �ȴ�.
	static int CheckMemory()			
	{
		return _CrtCheckMemory();
	}

	/// �ش� �޸� �Ҵ��ȣ�� �극��ũ ����Ʈ�� �����Ѵ�.
	static void BreakAlloc(long ncrtBreakAlloc)
	{
		//_crtBreakAlloc ������ ���� �����ص� ��.
		_CrtSetBreakAlloc(ncrtBreakAlloc);
	}
};

static MMemoryLeak g_MemoryLeak;

#endif	// _DEBUG
#endif	// _MSC_VER

#endif // _MMEMORY_LEAK_H