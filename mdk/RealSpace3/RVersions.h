#pragma once

namespace rs3 {

struct RHEADER {
	DWORD dwID;
	DWORD dwVersion;
	
	RHEADER() : dwID( 0 ), dwVersion( 0 ) {}
	RHEADER(DWORD id, DWORD version) { dwID=id;dwVersion=version; }
};

#define RS_ID			0x12345678			// .rs ����
#define RS_VERSION		8

#define RBSP_ID			0x35849298			// .bsp ����
#define RBSP_VERSION	2

#define R_PAT_ID		0x09784348			// .pat ���� ( ��ε����� )
#define R_PAT_VERSION	0

#define R_LM_ID			0x30671804			// .lm ���� ( ����Ʈ�� )
#define R_LM_VERSION	3

#define R_COL_ID		0x5050178f			// .col ���� ( �浹�� bsp )
#define R_COL_VERSION	1

#define R_NAV_ID		0x8888888f			// .nav ���� (����Ʈ �׺���̼ǿ�)
#define R_NAV_VERSION	2


#define RS3_ID			0xface5678			// .rs3 ����
#define RS3_VER1		5
#define RS3_VER2		6					// ���� ���信 color �߰� ( ���ķ� ���� )
#define RS3_VER3		7					// ������ ����(m_dwFlag)�� Collision ���� �߰�
#define RS3_VER4		8					// ������ �� ���ؽ� ���� �߰�
#define RS3_VERSION		RS3_VER4

const DWORD R_COL2_ID		= 0x59249834;	// .cl2 ���� ( �浹�� AABBTree )
const DWORD R_COL2_VERSION	= 1;			// save ����, ���� ���� ������ impl Ŭ������ ���

const DWORD R_AUV_ID		= 0x97127371;	// .auv ���� ( ����Ʈ�ʿ� uv )
const DWORD R_AUV_VERSION	= 2;
// 1 : 2007.10.17 ����Ʈ���� ũ�Ⱑ Ŀ���� ����� ���� �ֵ��� ����
// 2 : �ȸ��� �Ǵ� ���� �ǳʶٵ���

const DWORD R_ZUV_ID		= 0xdefcfead;	// .zuv ���� ( zone ����Ʈ�ʿ� uv )
const DWORD R_ZUV_VERSION	= 1;

}
