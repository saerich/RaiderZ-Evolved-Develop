#pragma once

/// ������ �ӽ÷� ����ϴ� ������
struct GlobalVar_Temp
{
	wchar_t	szPassword[256];		///< �α����Ҷ� ����ϴ� �ӽ� �н����� - �α����� �ʱ�ȭ�ȴ�.

	string		szTemp;				/// c���� ��Ʒ� �ѱ� �ӽ� ����. �ѹ������ ����Ÿ ������.

	int			nNextFieldID;

	GlobalVar_Temp()
	{
		szPassword[0] = 0;
	}
};
