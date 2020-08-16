#pragma once

// ���� ������
enum XGameFrameID
{
	NONE_FRAME_ID			= -1,
	LOGIN_FRAME_ID			= 0,			///< �α��� ������
	CHAR_FRAME_ID,							///< ĳ���� ���� ������
	PLAY_FRAME_ID,							///< ���� ������
	LOADING_FRAME_ID,

	REPLAY_FRAME_ID,
	DUMMYCLIENT_FRAME_ID,					///< ���� Ŭ���̾�Ʈ ������		- DummyClient ������Ʈ���� ���
	MINT_DESIGN_FRAME_ID,					///< UI �׽�Ʈ ������
	ENGINE_DEBUG_FRAME_ID,					///< �ܺ� ���� ����� ������
	TEST_FRAME_ID,
	HELLOWORLD_FRAME_ID,					///< �� �������� ���� ������ �̰��� ������ �����ؼ� ������. - birdkr
};


// Ư���� ���ڰ� ���� ��� ���� ���� �����ϴ� ���� ������
const XGameFrameID START_FRAME_ID = LOGIN_FRAME_ID;

