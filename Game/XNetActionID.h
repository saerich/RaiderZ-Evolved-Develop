#pragma once

/// NetPlayer �׼� ����
enum NET_ACTION_STATE
{
	NET_ACTION_STATE_NONE			= -1,
	NET_ACTION_STATE_NA				= 0,		///< �̺з�


	NET_ACTION_STATE_GRAPPLED,					///< ������
	NET_ACTION_STATE_SWALLOWED,					///< ������
	NET_ACTION_STATE_SIT,						///< �ɾ�����
	NET_ACTION_STATE_GUARD,						///< ����

	NET_ACTION_STATE_GESTURE,

	NET_ACTION_STATE_HIT,						///< ��Ʈ

	NET_ACTION_STATE_TALENT,					///< �ŷ�Ʈ

	MAX_NET_ACTION_STATE
};

