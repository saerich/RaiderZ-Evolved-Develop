//
//	Misc Functions about File
//
//                                              written by lee jang ho
//
//////////////////////////////////////////////////////////////////////
#ifndef _FILEINFO_H
#define _FILEINFO_H

#include <windows.h>
#include <time.h>
#include "RLib.h"

// ������ �ֱ� ������Ʈ�� �ð� ���
CML2_API BOOL GetLastUpdate(const char *pFileName, FILETIME *ft);

// ������ �ֱ� ������Ʈ�� �ð� ���
CML2_API BOOL GetLastDirectoryUpdate(const char* pDirectoryName, FILETIME *ft);

// ���ϸ��� ���� ��Ī���� ����
CML2_API void TrimFilePathName( char* szFileName);

// ������ ������Ʈ �Ǿ��°�?
//		pFileName
//		ot			:	���� �ð� ( ���� �������� �� �ð� )
CML2_API BOOL IsModifiedOutside(const char *pFileName, FILETIME ot);

// Extension ����
CML2_API BOOL RemoveExtension(char *pRemoveExt, const char *pFileName);

// Extension ��ü
CML2_API void ReplaceExtension(char *pTargetName, const char *pSourceName, char *pExt);

// �� ��� ���̱�
CML2_API void GetMergedPath( char* szOutPath, const char* szBasePath, const char* szRelPath);

// ��� ��� ���
CML2_API void GetRelativePath(char *pRelativePath, const char *pBasePath, const char *pPath);

// ���� ��� ���
CML2_API void GetFullPath(char *pFullPath, const char *pBasePath, const char *pRelativePath);
//�� ���� ��� ���... current directory�� ��������...
CML2_API void GetFullPath(char *pFullPath, const char *pRelativePath);

// path �� ����..
CML2_API void GetPurePath(char *pPurePath,const char *pFilename);
// path �� extension�� ������ ������ �����̸� ���.
CML2_API void GetPureFilename(char *pPureFilename,const char *pFilename);
// extension �� ����
CML2_API void GetPureExtension(char *pPureExtension,const char *pFilename);
// �����̸� + extension �� ����
CML2_API void GetPureFilenameExt(char *pPureFilenameExt,const char *pFilename);

// ���� ����ΰ�? ( ��Ʈ��ũ ��δ� �������� ���� )
CML2_API BOOL IsFullPath(const char *pPath);

// ��� �б� ( ����� ���� int�� ��� ��ü ũ�⸦ ������ �ִ�. )
CML2_API BOOL ReadHeader(HANDLE hFile, void *pHeader, int nHeaderSize);

// File�� CheckSum�� ���Ѵ�. (��¥, ������, ������ ����)
CML2_API DWORD GetFileCheckSum(char* pszFileName);

// ������ �����ϴ°�?
CML2_API bool IsExist(const char *filename);
CML2_API bool IsExist(const wchar_t *filename);

// �θ� ���丮 �̸��� ��´�.
CML2_API void GetParentDirectory(char* pszFileName);

// �־��� ��α����� ���丮�� �����Ѵ�.
CML2_API bool MakePath(const char* pszFileName);

// time_t �� FILETIME ���� ��ȯ Code from MSDN
CML2_API void time_tToFILETIME(time_t t, LPFILETIME pft);

// ������ WriteTime �� �����Ѵ�
CML2_API BOOL MSetFileTime(LPCTSTR lpszPath, FILETIME ft);

CML2_API bool GetMyDocumentsPath(char* path);

CML2_API bool CreatePathA(char* path);
CML2_API bool CreatePathW(wchar_t* path);

#ifdef _UNICODE
#define CreatePath		CreatePathW
#else
#define CreatePath		CreatePathA
#endif


// '\\' �� '/'�� ��ü�Ѵ�
CML2_API void ReplaceBackSlashToSlash(char* szPath);

// ���� / �� ���Ѵ�
CML2_API void AddSlash(char *szPath);

#ifdef WIN32
#pragma comment(lib, "Shlwapi.lib")
#endif

#endif	// _FILEINFO_H
