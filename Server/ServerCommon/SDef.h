#pragma once

#include "CSDef.h"

// ������Ÿ�� ���Ʈ, ������ �B��� - praptor
template<bool> struct CompileTimeAssert;
template<> struct CompileTimeAssert<true> {};
#define COMPILE_TIME_ASSERT(exp) (CompileTimeAssert< (exp) >())


// ���� �̸� -----------------
#define PURE_FILENAME_ZONELIST			L"zonelist.xml"
#define PURE_FILENAME_FIELDLIST			L"fieldlist.xml"
#define PURE_FILENAME_FIELDLIST_DEBUG	L"fieldlist_debug.xml"


#define FILEEXT_ZONE_COLLISION		L".server.cl2"	// �浹 �� ���� Ȯ����
#define FILEEXT_ZONE				L".zone.xml"
#define FILEEXT_FIELD				L".field.xml"
#define FILEEXT_COLT				L".colt.xml"


#define SERVER_UID_SEED_POWER		10000

// ��ƿ��Ƽ ��ũ�� ----------------------------------------------------------------------------
#ifndef _PUBLISH
	#define		DCHECK(x)			{ if (!(x)) { mlog3("\""#x"\" is invalid. (%s() ==> %s:%d)\n", __FUNCTION__, __FILE__, __LINE__); }}
	#define		RVALID(x)			{ if (!(x)) { mlog3("\""#x"\" is invalid. (%s() ==> %s:%d)\n", __FUNCTION__, __FILE__, __LINE__); return; }}
	#define		RVALID_RET(x, r)	{ if (!(x)) { mlog3("\""#x"\" is invalid. (%s() ==> %s:%d)\n", __FUNCTION__, __FILE__, __LINE__); return (r); }}
	#define		VALID(x)			{ assert(x); RVALID(x) }
	#define		VALID_RET(x, r)		{ assert(x); RVALID_RET(x,r) }
#else
	#define		DCHECK(x)			
	#define		RVALID(x)			{ if (!(x)) return; }
	#define		RVALID_RET(x, r)	{ if (!(x)) return(r); }
	#define		VALID(x)			{ RVALID(x) }
	#define		VALID_RET(x, r)		{ RVALID_RET(x,r) }
#endif
// ---------------------------------------------------------------------------------------------


/// ���� �ʵ� Ÿ��
enum DYNAMIC_FIELD_TYPE
{
	DFT_NONE = 0,

	DFT_TRIAL,
	DFT_INNROOM,
	DFT_BATTLEARENA,

	DFT_MAX
};

/// �ʵ� �̵� ���� Ÿ��
enum RESPONSE_GATE_TYPE
{
	RGT_FAIL = 0,			///< ���� - ����ó��
	RGT_REMAIN,				///< ���� - GM ��� ������ ���� �ʵ� �̵� �õ�, �����ʵ� ������������ �߰����� ����
	RGT_EXECUTE,			///< �̵����� - �����ͼ����� �ʵ带 ������ �״�
	RGT_RETRY,				///< ��õ� - ��� �ڿ� �ٽ� �õ�
};

/// ���� ���ӽ� ������ �ʵ� ID - GConfig::TEST_ENABLE_TUTORIAL
#define FIRST_ENTER_FIELD_ID	109			///< �η��� ���� �ʵ�
#define FIRST_TUTORIAL_FIELD_ID 1090000		///< �η��� ���� Ʃ�丮�� �ʵ�
#define FIRST_ENTER_MARKER_ID	1			///< 1�� ��Ŀ


/// �Ǹ�
#define PMANG_USER_SERIAL_LEN	10	///< �Ǹ� UserSerial ����, UserSerial�� long���̳� ���ڿ��� �ٷ��.
#define PMANG_USER_ID_LEN		12	///< �Ǹ� UserID ����
#define PMANG_PCBID_LEN			12  ///< �Ǹ� PC BID ����
#define PMANG_SERVICE_TYPE_LEN	12  ///< �Ǹ� ServiceType ���� (���Ż�ǰ��ȣ)


#define TIMEOUT_TICK_MOVE_SERVER	180000		///< ���� �̵��� Timeout üũ �ð�. (3��)
