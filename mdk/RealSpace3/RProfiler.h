#pragma once

#include "MProfiler.h"
#include "RGPUProfiler.h"
//#include "RProfile.h"

//MProfiler.h�� ���� ������ ��ũ��
//������ MProfiler.h�� �����ܿ����� ����. �׷��� ������ �������� ���α׷��� �������ϸ��� �и��ϰ��� �Ȱ��� ��ũ�θ� �̸��� �ٲ�
//�׷��� ������ MProfiler.h���� �������ϸ� ��ũ�θ� ���� �������� ���α׷��� �������ϸ��� ����,
//RProfiler.h������ �������ϸ� ��ũ�θ� �Ѽ� ������ �������ϸ��� ų �� �ִ�. �ᱹ ������ �������ϸ� ������ ����

#ifdef _USE_PROFILER
#define _USE_ENGINE_CPU_PROFILE
#endif 

//#define _USE_ENGINE_RESOURCE_PROFILE
//#define _USE_ENGINE_CPU_PROFILE		// CPU
//#define _USE_ENGINE_GPU_PROFILE		// GPU

#ifdef _USE_ENGINE_CPU_PROFILE

// ProFilerCallstack version Simple Macro
#define RPFC_B(_szProfileName)			GetProfileManager().Start_CallStackProfile(_szProfileName)
#define RPFC_E							GetProfileManager().Stop_CallStackProfile()
#define RPFC_THISFUNC					CCallStackProfileSample __CallStackProfile( __FUNCTION__ )
#define RPFC_BLOCK(x)					CCallStackProfileSample __CallStackProfile( x )
#define RPFC_RESET						GetProfileManager().Reset()
#define RPFC_REST_CURRENTINFO			GetProfileManager().ResetCurrentInfo()
#define RPFC_FINALANALYSIS(_szFileName)	GetProfileManager().SaveFile(_szFileName)

// ProFilerNoCallstack version Simple Macro
#define RPFN_B(_szProfileName)			GetProfileManager().Start_NoCallStackProfile(_szProfileName)
#define RPFN_E(_szProfileName)			GetProfileManager().Stop_NoCallStackProfile(_szProfileName)
#define RPFN_THISFUNC					CNoCallStackProfileSample __NoCallStackProfile( __FUNCTION__ )

#else

#define RPFC_B(_szProfileName)			;
#define RPFC_E							;
#define RPFC_THISFUNC					;
#define RPFC_BLOCK(x)					;
#define RPFC_RESET						;
#define RPFC_REST_CURRENTINFO			;
#define RPFC_FINALANALYSIS(_szFileName)	;

#define RPFN_B(_szProfileName)			;
#define RPFN_E							;
#define RPFN_THISFUNC					;

#endif

#ifdef _USE_ENGINE_GPU_PROFILE

#define RPFC_GPU_RESET					GetGPUProfiler().Create()
#define RPFC_GPU_CALL					GetGPUProfiler().Call( GetProfileManager().GetCallStackProfileCurrentNode() )
//#define RPFN_GPU_THISFUNC				rs3::RGPUProfileSample __GPUProfile( __FUNCTION__ )

#else

#define RPFC_GPU_RESET					;
#define RPFC_GPU_CALL					;
#define RPFN_GPU_THISFUNC				;

#endif