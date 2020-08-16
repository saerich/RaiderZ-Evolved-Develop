#ifndef MINET_ERRORTABLE_H
#define MINET_ERRORTABLE_H

namespace minet {

enum MErrorTable
{
	MOK				= 1,	///< ���� ����
	MERR_UNKNOWN	= -1,	///< ���� ���ϴ� ����

	MERR_COMMUNICATOR_HAS_NOT_UID						= 100,		///< Ŀ�´������Ͱ� UID�� ������ ���� ���, ���� �����ͳ� Ȥ�� UID�� �߱޹��� �� �ִ� Ŀ�´������Ϳ� �������� �ʾ��� ����.
	MERR_COMMUNICATOR_HAS_NOT_UID_SPACE					= 101,		///< Ŀ�´������Ͱ� �߱��� UID ������ ���� �ʴ�. UID�� �������� ���ɼ��� ����.
	MERR_COMMUNICATOR_INVALID_DIRECT_CONNECTION_POINTER	= 110,		///< �ϳ��� ���μ������� ���� Ŀ�´������Ͱ� ���ÿ� ������, DirectConnectionPointer�� ��ȿ���� ���� ���
	MERR_COMMAND_HAS_INVALID_RECEIVER_UID				= 200,		///< Ŀ�ǵ忡 ������ Receiver UID�� ��ȿ���� ������

	MERR_ZONESERVER_NOT_CONNECTED_TO_MASTER				= 300,		///< Zone-Server�� Master-Controller�� ������� �ʾ�����
	MERR_ZONESERVER_TRY_TO_INVALID_MASTER				= 301,		///< Zone-Server�� Master-Controller�� �ƴ� Ŀ�´������Ϳ��� Master-Controller�� �����ϰ� ������ �õ��Ҷ�

};


} // namespace minet

#endif