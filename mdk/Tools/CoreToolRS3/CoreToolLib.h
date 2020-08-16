#pragma once

#ifndef CORETOOL_API

/////////////////////////////////////////////////////////////////////
// DLL�� export�� �� �ʿ��� �͵�
#ifdef _USRDLL

#ifdef CORETOOL_EXPORT
// DLL library project uses this
#define CORETOOL_API __declspec(dllexport)
#else
#ifdef CORETOOL_IMPORT
// client of DLL uses this
#define CORETOOL_API __declspec(dllimport)
#else
// static library project uses this
#define CORETOOL_API
#endif
#endif // #ifdef REALSPACE3_EXPORT

#else
#define CORETOOL_API
#endif // #ifdef _USRDLL

#endif