#ifndef	_M_AUTO_TEST_INFO_H_
#define	_M_AUTO_TEST_INFO_H_

#include <string>
using namespace std;

class MAutoTestInfo
{
public:		
	string			strTestType;	///< �׽�Ʈ ����	
	int				nRepeat;		///< �ݺ� ȸ��
	DWORD			dwTime;			///< ��ȸ �ݺ� �Ҷ��� ���� �ð�
	int				nSpeed;			///< Update �ӵ�

	float			fLimitFrame;	///< ���� ������(�� ������ ������ ����)

public:
	MAutoTestInfo();
	~MAutoTestInfo();
};

#endif
