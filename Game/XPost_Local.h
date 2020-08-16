#ifndef _XPOST_LOCAL_H
#define _XPOST_LOCAL_H

#include "XNetClient.h"
#include "XCommandTable.h"

// �׽�Ʈ ���� --------------------------------------------------------------------------------------
inline void XPostLocal_ReloadUI()
{
	XPOSTCMD0(MC_LOCAL_RELOAD_UI);
}


// ���� ����
inline void XPostLocal_ChangeFrameFrame(int nGameFrame)
{
	XPOSTCMD1(MC_LOCAL_CHANGE_GAMEFRAME, MCommandParameterInt(nGameFrame));
}



#endif