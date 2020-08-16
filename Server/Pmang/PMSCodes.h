#ifndef __PMSCODES_H_
#define __PMSCODES_H_

enum PMSCONN_STATUSCODE
{
	PMSC_STATUS_INIT_BEFORE = 0, // PMS�ʱ�ȭ��
	PMSC_STATUS_INIT, // PMS �ʱ�ȭ��
	PMSC_STATUS_RUN,  // PMS ������
	PMSC_STATUS_STOP, // PMS ������
	PMSC_STATUS_NOUSE, // PMS ��� ���� �ʴ� MODE ������
	PMSC_STATUS_TRY_CONNECTING, // PMS�� HA�� ������ �õ� �Ϸ��� �ϰ� ����
};

enum PMSCONN_ERRORCODE
{
	PMSC_ERR_OK = 0, // ����
	PMSC_ERR_NOT_INITIALIZED, // PMS�� ���� �ʱ�ȭ ���� �ʾ���
	PMSC_ERR_INIT_INVALIDPARAM, // �߸��� ���� ����
	PMSC_ERR_RUN_ALREADY,	// PMS�� �̹� ��������
	PMSC_ERR_RUN_THREAD_FAILED, // PMS ADL Message ó���� ���� Thread ���� ����
};

enum FAULT_LEVEL
{
	FL_WARNING = 1, // only saved DB
	FL_ALERT ,	// saved DB and MC notified as ALERT Level
	FL_CRITICAL , // saved DB and MC notified as Critical Level
	FL_MAX, 
};

#endif 