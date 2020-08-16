#ifndef _MCSVPARSER_H
#define _MCSVPARSER_H

/*
===============================================================================

	MCsvParser.h

	�������� �ͽ���Ʈ�� �� �ִ� Ȯ���ڰ� csv�� ������ �Ľ��Ѵ�.

	�����(2005/12/27)

===============================================================================
*/


#include <string.h>
#include <stdio.h>
#include "RLib.h"

/// CSV ������ �ٷ�� �ļ�
class CML2_API MCsvParser
{
private :
	char*		m_pBuffer;
	int*		m_pOffset;
	int			m_nLine;

	int			CountLine(const char* buffer, int* offset);
	int			PassToken(const char* str);
	char*		ReadFile(const char* fname);
public :
				/// ������
				MCsvParser();
				/// �ı���
				~MCsvParser();
				/// ������ �д´�.
	bool		Load(const char* fname);
	bool		LoadFromStream(const char* szStream);
				/// ������ ��ȯ
	int			GetData(int col, int row, char* outptr, int outlen);
	int			GetColCount(int row);
	void		Clear();
	void		TransComma( char* outptr );
};



/////////////////////////////////////////////////////
// sample
//	FILE* fp = fopen("c:\\output.txt", "wt");
//
//	MCsvParser Parser;
//	Parser.Load("c:\\��Ģ��.csv");
//
//	const int LEN = 256;
//	char text[LEN] = "";
//
//	for (int i = 0; i < 1000; i++)
//	{
//		for (int j = 0; j < 1000; j++)
//		{
//          Parser.GetData(j, i, text, 256);
//			if ((text[0] != '\0') && (text[0] != 13))
//			{
//				fprintf(fp, "%s\n", text);
//			}
//		}
//	}
//
//	fclose(fp);
//	printf("OK!!\n");
//	return 0;
//






#endif