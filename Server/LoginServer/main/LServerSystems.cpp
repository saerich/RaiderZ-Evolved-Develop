#include "stdafx.h"
#include "LServerSystems.h"
#include "LServer.h"
#include "LSystem.h"
#include "LDBManager.h"
//#include "LPMSSystem.h"

LServerSystems::LServerSystems() : LGlobalSystem()
{
}

LServerSystems::~LServerSystems()
{

}

void LServerSystems::Create()
{
	pServer				= new LServer();
	pSystem				= new LSystem();
	pDBManager			= new LDBManager();
	//pPMS				= new LPMSSystem();

	gsys.pServer			= pServer;
	gsys.pSystem			= pSystem;
	gsys.pDBManager			= pDBManager;
	//gsys.pPMS				= pPMS;
}

void LServerSystems::Destroy()
{
	SAFE_DELETE(pServer);
	SAFE_DELETE(pSystem);
	SAFE_DELETE(pDBManager);
	//SAFE_DELETE(pPMS);

	gsys.SetNull();
}

