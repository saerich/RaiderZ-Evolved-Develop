// ddsgen.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <conio.h>
#include <direct.h>
#include <crtdbg.h>
#include <d3dx9.h>

#include "ddsgen.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	// dds generator
	DDSGen aDDSGen;

	// set options
	char szPath[MAX_PATH]=".";
	if(argc>1)
	{
		for(int i=1;i<argc;i++)
		{
			if(_stricmp("-r",argv[i])==0)
				aDDSGen.SetUnddsOption(true);
			else
			if(_stricmp("-nomip",argv[i])==0)
			{
				aDDSGen.SetGenMipOption(false);
			}else
			if(_stricmp("-size",argv[i])==0)
			{
				if(argc<=i+1)
					goto usage;
				char szBuf[256];
				strcpy_s(szBuf, argv[i+1]);
				aDDSGen.SetTexLevelOption(atoi(szBuf));
				i++;
			}else
			if(_stricmp("-path",argv[i])==0)
			{
				if(argc<=i+1)
					goto usage;
				strcpy_s(szPath, MAX_PATH, argv[i+1]);
				i++;
			}else
			goto usage;
		}
	}

	// init d3d for texture
	if (!aDDSGen.InitD3D())
		return 0;
	printf("d3d init ok."); 

	// gen
	aDDSGen.GenDDS(szPath);

	// close d3d
	aDDSGen.CloseD3D();
	return 0;

usage:
	printf("\n usage : ddsgen [-r] [-nomip] [-path TARGETPATH] [-size SIZE]\n\n");
	printf("   -r     :  dds�� bmp�� ������\n");
	printf("   -nomip :  �Ӹ��� �������� ����\n");
	printf("   -path  :  ��� ��� ���� (�⺻���� ���� ���)\n\n");
	printf("   -size  :  ������ ������ - (0:�⺻������, 1=1/2, 2=1/4, 3=1/8)\n\n");
	printf(" Copyright (C) MAIET Entertainment\n");
	return 0;
}
