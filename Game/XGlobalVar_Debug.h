#pragma once

/// ����� ����
struct GlobalVar_Debug
{




	// Render ����
	bool	bShowAABB;			// AABB �ڽ�
	bool	bShowCollision;		// �浹���� �����ٰ����� ����   
	bool	bShowHitCapsule;	// ���� ĸ���� �����ٰ����� ����
	bool	bShowHitEffectCapsule;	// ���� ����Ʈ ĸ���� �����ٰ����� ����
	bool	bShowSector;		// ���� ��輱�� ������ ������ ���� (����)
	bool	bShowSector2;		// ���� ��輱�� ������ ������ ���� (����)
	bool	bShowSensor;		// ������ �׷��� �� ����
	bool	bShowMoveCollision_Real;	// �̵��浹 ������ ������ ������ ����
	bool	bShowMoveCollision_Poly;	// ������ �̵��浹 ������ ������ ������ ����
	bool	bShowmovecollision_Down;	// �Ʒ��� �浹�� �������� ������ ������ ����
	bool	bShowMoveCollision_View;	// �ֺ� �浹 ������ ������ ������ ����
	bool	bShowMoveCollision;
	bool	bShowmovecollision_Investigate;	// �浹 �˻縦 Tick ������ �� �� �ְ� ����
	bool	bShowObjectCollision_Investigate; // ������Ʈ �浹 �˻縦 ��꺰�� ����
	bool	bShowMagicArea;		// ������ ������ �����ٰ����� ����
	bool	bShowArea;
	bool	bShowID;
	bool	bShowNpcMoveInfo;	// NPC �̵� ����(�����κ����� ����)

	bool	bShowSoundRange;	// ���� ������ ������ ������ ����

	// Debug Information
	wstring		strTestMotionName;	// �׽�Ʈ�� ��� �̸�
	MF_STATE	nTestMF;			// �׽�Ʈ�� ��� ����
	int			nTestMFWeight;		// �׽�Ʈ�� ��� ���� weight
	MUID		uidTargetNPC;		// �׽�Ʈ�� NPC

	// Server Logic Debug
	bool	bWarpToPickingPos;
	map<MUID, vector<vec3> >	mapNPCMovePathList;	// NPC�� �̵� ���
	vector< TD_DEBUG_POS > vecActorPos;	// NPC�� �̵� ���
	map<MUID, bool >			mapNPCMovePathFailed;	
	map<MUID, vec3 >			mapNPCMovePathFailedPos;
	map<MUID, TD_DEBUG_SPHERE >	mapNPCSignalSphere;
	map<MUID, vector<RCapsule> >	mapActorCapsule;
	vector<pair<vec3, vec3> >	mapNPCPickingList;
	vector<pair<vec3, vec3> >	mapNPCValidPickingList;
	bool						bRenderMoveDebug;

	// benchmark
	bool	bAutoReportCrashDump;

	// general message
	map< wstring, wstring >		mapGeneralDebuggingMsg;

	// Show Move Collision
	int		nShowMoveCollision_InvestigateIndex;
	int		nShowObjectCollision_InvestigateIndex;

	// Camera
	bool	bCameraLock;
	MUID	uidCameraLock;
	wstring strCameraLockBoneName;
	float	fTestCameraLockRightValue;
	float	fTestCameraLockBackValue;
	float	fTestCameraLockHeightValue;

	// guard
	bool	bForceGuard;

	GlobalVar_Debug();
};
