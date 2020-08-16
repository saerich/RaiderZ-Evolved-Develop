#pragma once

#pragma warning( disable : 4251 )

#ifndef GVALIDATOR_API

/////////////////////////////////////////////////////////////////////
// DLL�� export�� �� �ʿ��� �͵�
#ifdef _USRDLL

#ifdef GVALIDATOR_EXPORT
// DLL library project uses this
#define GVALIDATOR_API __declspec(dllexport)
#else
#ifdef GVALIDATOR_IMPORT
// client of DLL uses this
#define GVALIDATOR_API __declspec(dllimport)
#else
// static library project uses this
#define GVALIDATOR_API
#endif
#endif // #ifdef GVALIDATOR_EXPORT

#else
#define GVALIDATOR_API
#endif // #ifdef _USRDLL

#endif