#include "StdAfx.h"
#include "TCalcTalentHitEffectDir.h"
#include "XTalentInfo.h"
//#include "RDebugPassRenderer.h"

TCalcTalentHitEffectDir::TCalcTalentHitEffectDir()
{

}

TCalcTalentHitEffectDir::~TCalcTalentHitEffectDir()
{

}

bool TCalcTalentHitEffectDir::CalcTalentHitEffectDir( XTalentInfo * pTalentInfo, string strModelName, bool bCheck /*= false*/ )
{
	TCharacter* pActor = g_pMainApp->GetActor();
	if(pTalentInfo == NULL ||
		g_pMainApp->GetActor() == NULL ||
		pActor == NULL ||
		pActor->GetCurAnimation() == NULL)
		return false;


	// ����Ʈ �˻�
	XTalentEffectDataMgr* pEffectData = pTalentInfo->GetEffectDataMgr();
	if(pEffectData == NULL)
		return false;						// NULL�ΰ� �ǰ� ����Ʈ�� �����Ƿ� ���غ��� �ҿ����.

	if(pEffectData->m_nRefID > 0)
	{
		XTalentInfo * pTalentInfoTemp = TTalentHelper::GetTalentInfo(pEffectData->m_nRefID, pEffectData->m_nRefMode);
		if (pTalentInfoTemp == NULL) 
			return false;

		pEffectData = pTalentInfoTemp->GetEffectDataMgr();
		if(pEffectData == NULL)
			return false;
	}

	XTalentEffectInfo* pEffectInfo = pEffectData->Get(HIT_EFFECT_POSDIR);

	// �ŷ�Ʈ �ִϸ��̼��� ������... ��� ���� ����.
	if(pTalentInfo->m_szUseAnimation.empty())
	{
		if(pEffectInfo != NULL)
		{
			// ����
			DelHitEffectDir(pEffectData);

			return true;
		}
		return false;
	}

	// ���� ���� üũ
	//if(bCheck && CheckCreateTalentHitEffectDir(pActor, g_pMainApp->GetMotion(), strModelName, pEffectInfo) == false)
	//	return false;

	// ����Ʈ üũ
	if(pEffectInfo == NULL)
	{
		pEffectInfo = new XTalentEffectInfo();
		pEffectInfo->m_nEffectType = HIT_EFFECT_POSDIR;
		pEffectData->push_back(pEffectInfo);
	}

	vector<XTalentHitEffectPosDir> vecSwordHitEffectDirTemp;

	// �ŷ�Ʈ ��Ʈ����
	if(pTalentInfo->m_HitInfo.m_nReferenceID > 0)
	{
		pTalentInfo = TTalentHelper::GetTalentInfo(pTalentInfo->m_HitInfo.m_nReferenceID, pTalentInfo->m_HitInfo.m_nReferenceMode);

		if(pTalentInfo == NULL)
			return false;
	}

	// �ñ׸�Ʈ�� �ִ� ���� Ÿ������ ����Ʈ ���ϱ�
	for(vector<CSHitSegment>::iterator itSeg = pTalentInfo->m_HitInfo.m_vSegments.begin(); itSeg != pTalentInfo->m_HitInfo.m_vSegments.end(); ++itSeg)
	{
		XTalentHitEffectPosDir dirData;
		dirData.strMeshName = strModelName;
		dirData.fHitTime = itSeg->m_fCheckTime;
		if(CalcTalentHitEffectPosDirByTalentHitTime(itSeg->m_fCheckTime, strModelName, dirData.vHitEffectPos, dirData.vSwordTrailHitUp) == false)
			continue;

		vecSwordHitEffectDirTemp.push_back(dirData);
	}

	// ��� ó��
	if(vecSwordHitEffectDirTemp.empty())
	{
		ClearSwordHitEffectDir(pEffectInfo, strModelName);

		if(pEffectInfo->m_vecTalentHitEffectPosDir.empty())
		{
			DelHitEffectDir(pEffectData);
		}

		return true;
	}
	else if(vecSwordHitEffectDirTemp.empty() == false)
	{
		vector<XTalentHitEffectPosDir> vecOrgDir;
		GetSwordTrailEffectDirByModelName(strModelName, pEffectInfo->m_vecTalentHitEffectPosDir, vecOrgDir);
		if(CheckDuplicationSwordTrailEffectDir(vecOrgDir, vecSwordHitEffectDirTemp) == false)
		{
			ClearSwordHitEffectDir(pEffectInfo, strModelName);

			for(vector<XTalentHitEffectPosDir>::iterator itSet = vecSwordHitEffectDirTemp.begin(); itSet != vecSwordHitEffectDirTemp.end(); ++itSet)
			{
				pEffectInfo->m_vecTalentHitEffectPosDir.push_back(*itSet);
			}

			return true;
		}
	}

	return false;
}

bool TCalcTalentHitEffectDir::CalcTalentHitEffectDir( XTalentInfo * pTalentInfo, string strModelName, int nSegID )
{
	TCharacter* pActor = g_pMainApp->GetActor();
	if(pTalentInfo == NULL ||
		g_pMainApp->GetActor() == NULL ||
		pActor == NULL ||
		pActor->GetCurAnimation() == NULL)
		return false;


	// ����Ʈ �˻�
	XTalentEffectDataMgr* pEffectData = pTalentInfo->GetEffectDataMgr();
	if(pEffectData == NULL)
		return false;						// NULL�ΰ� �ǰ� ����Ʈ�� �����Ƿ� ���غ��� �ҿ����.

	if(pEffectData->m_nRefID > 0)
	{
		XTalentInfo * pTalentInfoTemp = TTalentHelper::GetTalentInfo(pEffectData->m_nRefID, pEffectData->m_nRefMode);
		if (pTalentInfoTemp == NULL) 
			return false;

		pEffectData = pTalentInfoTemp->GetEffectDataMgr();
		if(pEffectData == NULL)
			return false;
	}

	XTalentEffectInfo* pEffectInfo = pEffectData->Get(HIT_EFFECT_POSDIR);
	if(pEffectInfo == NULL)
	{
		CalcTalentHitEffectDir(pTalentInfo, strModelName);
		return true;
	}

	if(pEffectInfo->m_vecTalentHitEffectPosDir.size() <= nSegID)
	{
		CalcTalentHitEffectDir(pTalentInfo, strModelName, true);
		return true;
	}

	int nIndex = 0;
	XTalentHitEffectPosDir* pModifyPosDir = NULL;
	for(vector<XTalentHitEffectPosDir>::iterator it = pEffectInfo->m_vecTalentHitEffectPosDir.begin(); it != pEffectInfo->m_vecTalentHitEffectPosDir.end(); ++it)
	{
		if(it->strMeshName == strModelName)
		{
			if(nIndex == nSegID)
			{
				pModifyPosDir = &(*it);
				break;
			}

			++nIndex;
		}
	}
	if(pModifyPosDir && pModifyPosDir->fHitTime != pTalentInfo->m_HitInfo.m_vSegments[nSegID].m_fCheckTime)
	{
		pModifyPosDir->fHitTime = pTalentInfo->m_HitInfo.m_vSegments[nSegID].m_fCheckTime;
	
		if(CalcTalentHitEffectPosDirByTalentHitTime(pTalentInfo->m_HitInfo.m_vSegments[nSegID].m_fCheckTime, strModelName, pModifyPosDir->vHitEffectPos, pModifyPosDir->vSwordTrailHitUp))
			return true;
	}

	return false;
}

bool TCalcTalentHitEffectDir::CalcTalentHitEffectPosDirByTalentHitTime( float fHitTime, string strModelName, vec3& vOutPos, vec3& vOutDir )
{
	MMatrix matStart;
	MMatrix matEnd;

	TCharacter* pActor = g_pMainApp->GetActor();

	int nCurFrame = pActor->GetCurFrame();
	int nFrame = pActor->GetSecToFrame(fHitTime);
	int nNextFrame = pActor->GetSecToFrame(fHitTime + 0.01f);

	if(g_pMainApp->GetMotion()->GetMotion() != NULL)
	{
		g_pMainApp->GetMotion()->SetFrame(nFrame);

		pActor->GetAnimatedActorNodeTransform(nFrame, string("swordtrail_start"), matStart, E_TS_LOCAL_RESULT);


		g_pMainApp->GetMotion()->SetFrame(nNextFrame);
		pActor->GetAnimatedActorNodeTransform(nNextFrame, string("swordtrail_start"), matEnd, E_TS_LOCAL_RESULT);
	}
	else
	{
		pActor->GetAnimatedActorNodeTransform(nFrame, string("swordtrail_start"), matStart, E_TS_LOCAL_RESULT);
		pActor->GetAnimatedActorNodeTransform(nNextFrame, string("swordtrail_start"), matEnd, E_TS_LOCAL_RESULT);
	}

	vec3 vStartDir = matStart.GetTranslation();
	vec3 vEndDir = matEnd.GetTranslation();
	vec3 vSwordTrailDir = (vec3(vEndDir.x, 0.0f, vEndDir.z) - vec3(vStartDir.x, 0.0f, vStartDir.z) ).Normalize();

	// �� ���ϱ�
	vOutDir		= vec3(0.0f, -1.0f, 0.0f).CrossProduct(vSwordTrailDir);
	vOutDir.Normalize();

	// ��ġ ���ϱ�
	MMatrix matTest;
	int nTestFrame = nFrame + ((nNextFrame - nFrame) / 2.0f);
	pActor->GetAnimatedActorNodeTransform(nTestFrame, string("swordtrail_start"), matTest, E_TS_LOCAL_RESULT);
	vOutPos		= matTest.GetTranslation();

	//// �ǵ�����...
	g_pMainApp->GetMotion()->SetFrame(nCurFrame);
	//REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugSphere(matStart, 5, 20.0f, D3DCOLOR_XRGB(255, 0, 0));
	//REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugSphere(matEnd, 5, 20.0f, D3DCOLOR_XRGB(0, 0, 255));
	//REngine::GetSceneManager().GetPassRenderer< RDebugPassRenderer >()->AddDebugAxis(matOut, 5, 20.0f, D3DCOLOR_XRGB(255, 0, 255));

	//mlog("�� ��ǥ %f %f %f\r", vUp.x, vUp.y, vUp.z);

	return true;
}

bool TCalcTalentHitEffectDir::CheckSwordNodeExist( TCharacter* pActor, TPlayMotion* pPlayMotion, string strModelName )
{
	RActorNode* pActorNode = pActor->GetActorNode("swordtrail_start");
	if(pActorNode)
		return true;

	return false;
}

void TCalcTalentHitEffectDir::ClearSwordHitEffectDir( XTalentEffectInfo* pEffectInfo, string strModelName )
{
	vector<XTalentHitEffectPosDir>::iterator it = pEffectInfo->m_vecTalentHitEffectPosDir.begin();
	while(it != pEffectInfo->m_vecTalentHitEffectPosDir.end())
	{
		if(it->strMeshName == strModelName)
		{
			it = pEffectInfo->m_vecTalentHitEffectPosDir.erase(it);
			continue;
		}

		++it;
	}
}

bool TCalcTalentHitEffectDir::CheckDuplicationSwordTrailEffectDir( vector<XTalentHitEffectPosDir>& vecOrg, vector<XTalentHitEffectPosDir>& vecNew )
{
	if(vecOrg.size() != vecNew.size())
		return false;

	int nSize = vecOrg.size();

	for(int i = 0; i < nSize; ++i)
	{
		if(MMath::Equals(vecOrg[i].vSwordTrailHitUp, vecNew[i].vSwordTrailHitUp, 0.01f) == false)
			return false;
	}

	return true;
}

void TCalcTalentHitEffectDir::GetSwordTrailEffectDirByModelName( string strModelName, vector<XTalentHitEffectPosDir>& vecOrg, vector<XTalentHitEffectPosDir>& vecOut )
{
	for(vector<XTalentHitEffectPosDir>::iterator it = vecOrg.begin(); it != vecOrg.end(); ++it)
	{
		if(it->strMeshName == strModelName)
		{
			vecOut.push_back(*it);
		}
	}
}

void TCalcTalentHitEffectDir::DelHitEffectDir( XTalentEffectDataMgr* pEffectData )
{
	for(vector<XTalentEffectInfo *>::iterator itErase = pEffectData->begin(); itErase != pEffectData->end(); ++itErase)
	{
		if((*itErase)->m_nEffectType == HIT_EFFECT_POSDIR)
		{
			SAFE_DELETE(*itErase);
			pEffectData->erase(itErase);
			break;
		}
	}
}

bool TCalcTalentHitEffectDir::CheckCreateTalentHitEffectDir(TCharacter* pActor, TPlayMotion* pPlayMotion, string strModelName, XTalentEffectInfo* pEffectInfo)
{
	// ��Ʈ ����Ʈ ���� �����߿��� starttrail_start ���ͳ�尡 �ʿ��ϴ�.
	bool bSwordTrailEffect = CheckSwordNodeExist(pActor, pPlayMotion, strModelName);

	if(pEffectInfo == NULL)
	{
		if(bSwordTrailEffect)
			return true;

		return false;
	}

	vector<XTalentHitEffectPosDir> vecHitEffectDir;
	GetSwordTrailEffectDirByModelName(strModelName, pEffectInfo->m_vecTalentHitEffectPosDir, vecHitEffectDir);

	if(vecHitEffectDir.empty())
	{
		if(bSwordTrailEffect)
			return true;
	}
	else
	{
		if(bSwordTrailEffect == false)
			return true;
	}

	return false;
}