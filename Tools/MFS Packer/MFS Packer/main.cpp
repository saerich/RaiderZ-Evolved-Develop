#include <stdafx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MFileSystem.h"
#include <regex>

MFileSystem* filesys;

void CallbackDetectedInvalidFile()
{
	printf("An invalid file detected. Please check for anti-virus program.\n");
}

bool MCreateFile(MFileInfo * fileInfo, const char* szDestPath)
{
	// Get destination path
	string strDestPath;
	if (szDestPath != NULL && szDestPath[0] != 0)
	{
		strDestPath = szDestPath;
		std::replace(strDestPath.begin(), strDestPath.end(), '\\', '/');

		// Trim left and right
		string::size_type n;
		n = strDestPath.find_first_not_of(" ./");
		if (n != string::npos)  strDestPath = strDestPath.substr(n, strDestPath.length());
		n = strDestPath.find_last_not_of(" /");
		if (n != string::npos)  strDestPath = strDestPath.substr(0, n + 1);
	}

	// Create path
	string _filename;
	if (strDestPath.empty() == false)
	{
		_filename += strDestPath;
		_filename += "/";
		_filename += fileInfo->strFileName;
	}
	else
	{
		_filename = fileInfo->strFileName;
	}

	string::size_type _p;
	_p = _filename.find_last_of("/");
	if (_p != string::npos)
	{
		char* _path = _strdup(_filename.substr(0, _p).c_str());

		string _dir;
		char* _token, * _next_tok;
		_token = strtok_s(_path, "/", &_next_tok);
		while (_token != NULL)
		{
			_dir += _token;
			_dir += "/";
			if (_access(_dir.c_str(), 0) == -1)
				CreateDirectory(_dir.c_str(), NULL);

			_token = strtok_s(NULL, "/", &_next_tok);
		}

		free(_path);
	}

	MFile file;
	if (!file.Open(fileInfo->strFileName.c_str()))
	{
		printf("Error reading file: \"%s\"\n", fileInfo->strFileName.c_str());
		return false;
	}

	// read file stream
	UINT fileSize = file.GetLength();
	char* pBuffer = new char[fileSize];
	if (file.Read((void*)pBuffer, fileSize) == false
		|| fileSize == 0)
	{
		delete[] pBuffer;
		pBuffer = NULL;
		return false;
	}
	file.Close();

	// Write data
	HANDLE hWriteFile = ::CreateFile(_filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hWriteFile != INVALID_HANDLE_VALUE)
	{
		DWORD _writenum;
		bool success = ::WriteFile(hWriteFile, pBuffer, fileSize, &_writenum, NULL);
		::CloseHandle(hWriteFile);

		if (success != TRUE)
		{
			delete[] pBuffer;
			pBuffer = NULL;

			printf("Error writing file (%lu): \"%s\"\n", GetLastError(), fileInfo->strFileName.c_str());
			return false;
		}
	}

	delete[] pBuffer;
	pBuffer = NULL;

	return true;
}

int main(int numArgs, const char* args[])
{
	// Help
	if (numArgs <= 1)
	{
		printf("\nMRS Packer 0.1 beta  Copyright (c) 2010-2020 MAIET Entertainment\n\n");
		printf("Usage : MRSPacker <command[:size(MB)]> <archive name> <file names> [target dir]\n");
		printf("  archive     : MRSPacker a out.mrf *.* outdir\n");
		printf("                MRSPacker a:100 out.mrf *.* outdir\n");
		printf("  extract all : MRSPacker x outdir\n");
		printf("  extract file: MRSPacker x out.mrf outdir\n");
		printf("  merge       : MRSPacker m index.msf outdir/*.index\n\n");
		printf("<Commands>\n  a : Add files to archive\n  x : eXtract files\n  m : Merge index file\n");
	}

	// Archive files
	else if (numArgs >= 4 && (_stricmp(args[1], "a") == 0 || _strnicmp(args[1], "a:", 2) == 0))
	{
		size_t nMaxSize = 0;
		if (args[1][1] == ':')
			nMaxSize = atoi(args[1] + 2);

		//SRes _result = EncodingDir(args[2], args[3], (numArgs > 4 ? args[4] : ""), nMaxSize);
		//if (_result != SZ_OK)		return 1;
	}

	// Extract all
	else if (numArgs >= 2  && _stricmp(args[1], "x") == 0)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		printf("Creating file system...\n");
		filesys = MCreateFileSystem(MFILEACCESS_ENCRYPTED);
		filesys->SetDetectInvalidFileCallback(CallbackDetectedInvalidFile);

		printf("Reading file index...\n");
		filesys->ReadFileIndex("fileindex.msf");

		auto fileIndex = (*filesys->GetFileIndex());

		printf("Extracting files...\n");
		for (auto it = fileIndex.begin(); it != fileIndex.end(); it++)
		{
			auto pFile = it->second;

			if (MCreateFile(pFile, args[2]))
			{
				printf("Extracted: %s\n", pFile->strFileName.c_str());
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 4);
				printf("Failed extracting: %s\n", pFile->strFileName.c_str());
				SetConsoleTextAttribute(hConsole, 7);
			}
		}

		printf("Finished extracting, reclaiming memory.\n");
		filesys->Destroy();
	}

	// Extract files
	else if (numArgs >= 3 && _stricmp(args[1], "x") == 0)
	{
		//SRes _result = DecodingDir(args[2], (numArgs > 3 ? args[3] : ""));
		//if (_result != SZ_OK)		return 1;
	}

	// Merge index
	else if (numArgs >= 2 && _stricmp(args[1], "m") == 0)
	{
		//SRes _result = MergeIndex(args[2], args[3]);
		//if (_result != SZ_OK)		return 1;
	}

	return 0;
}
