#pragma once

#pragma push_macro("_SECURE_SCL")
#pragma push_macro("_HAS_ITERATOR_DEBUGGING")
#pragma warning ( disable : 4005)

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

////////////////////////////////////////////////////////////////////////////////////////////
// �⺻ �������Ϸ��� GPG3�� (1.17)�� ���� ����մϴ�
/*

 ȣ�� ���ÿ� ���� ���������� ��ϵǴ� �������Ϸ��̸�
 _BEGINPROFILE(<name>) ~ _ENDPROFILE �� �� ������ ����ϰų�
 �Ǵ� PFC_THISFUNC �� ����� �� �ֽ��ϴ� ( �� ��ũ�δ� PFC_B(<functionname>)�� �ڵ����� �����ϰ�, ���ð������� ��� �� PFC_E�� �����մϴ� )

 ����� ����ϱ� ���ؼ� PFC_FINALANALYSIS(<filename>)�� ���

 ��1)
 int main()
 {
	PFC_B("main");
	...
	PFC_E;

	PFC_FINALANALYSIS("profile.log");
 }

 ��2)
 bool run()
 {
	PFC_THISFUNC;
 }

 int main()
 {
	Initialize();
	do {
	 ...
	} while (run());

	PFC_FINALANALYSIS("profile.log");
 }

*/

#include "Profile/profile.h"
#include <set>
#include <string>
#include "RLib.h"

#ifdef _USE_PROFILER

	// ProFilerCallstack version Simple Macro
	#define PFC_B(_szProfileName)			GetProfileManager().Start_CallStackProfile(_szProfileName)
	#define PFC_E							GetProfileManager().Stop_CallStackProfile()
	#define PFC_THISFUNC					CCallStackProfileSample __CallStackProfile( __FUNCTION__ )
	#define PFC_BLOCK(x)					CCallStackProfileSample __CallStackProfile( x )
	#define PFC_RESET						GetProfileManager().Reset()
	#define PFC_FINALANALYSIS(_szFileName)	GetProfileManager().SaveFile(_szFileName)

	// ProFilerNoCallstack version Simple Macro
	#define PFN_B(_szProfileName)			GetProfileManager().Start_NoCallStackProfile(_szProfileName)
	#define PFN_E(_szProfileName)			GetProfileManager().Stop_NoCallStackProfile(_szProfileName)
	#define PFN_THISFUNC					CNoCallStackProfileSample __NoCallStackProfile( __FUNCTION__ )

#else

	#define PFC_B(_szProfileName)			;
	#define PFC_E							;
	#define PFC_THISFUNC					;
	#define PFC_BLOCK(x)					;
	#define PFC_RESET						;
	#define PFC_FINALANALYSIS(_szFileName)	;

	#define PFN_B(_szProfileName)			;
	#define PFN_E							;
	#define PFN_THISFUNC					;


#endif


#pragma pop_macro("_SECURE_SCL")
#pragma pop_macro("_HAS_ITERATOR_DEBUGGING")

