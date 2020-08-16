#include "stdafx.h"
#include "TActorMgr.h"

TActorMgr::TActorMgr( RSceneManager* pSceneManager, TSoulHuntAnimationEventListener* pEventListener )
{
	m_pSceneManager = pSceneManager;
	m_pEventListener = pEventListener;
	m_pCurrentActor = NULL;

	Init();
}

TActorMgr::~TActorMgr()
{
	Destroy();
}

void TActorMgr::Update( float fDelta )
{
	// ���� �ð� üũ
	CheckDelTime(fDelta);

	// ���� ī��Ʈ üũ
	CheckDelCount();

	// ������Ʈ �ִϸ��̼�
	UpdateAnimation(fDelta);
}

bool TActorMgr::CheckDelTime( float fDelta )
{
	bool bDel = false;

	vector<stACTORDATA>::iterator itData = m_vecActorList.begin();
	while(itData != m_vecActorList.end())
	{
		(*itData).fRemainTime -= fDelta;
		if((*itData).fRemainTime <= 0.0f)
		{
			DelCharacter(&(*itData));

			itData = m_vecActorList.erase(itData);

			bDel = true;
		}

		itData++;
	}

	return bDel;
}

bool TActorMgr::CheckDelCount()
{
	// ���� 15���� �Ѿ�� ���Ų����� �����.
	if(m_vecActorList.size() > 15)
	{
		vector<stACTORDATA>::iterator itData = m_vecActorList.begin();
		if(itData != m_vecActorList.end())
		{
			DelCharacter(&(*itData));

			m_vecActorList.erase(itData);

			return true;
		}
	}

	return false;
}

void TActorMgr::DelCharacter( stACTORDATA* pData )
{
	if(pData->pActor == NULL)
		return;

	if(m_pCurrentActor == pData->pActor)
		m_pCurrentActor = NULL;

	pData->pActor->RemoveFromParent();
	pData->pActor->Destroy();

	SAFE_DELETE(pData->pActor);
}

TCharacter* TActorMgr::ModelLoad( const char* FileName )
{
	// ���� ���� ����
	if(m_pCurrentActor)
	{
		m_pCurrentActor->RemoveFromParent();
		m_pCurrentActor = NULL;
	}

	TCharacter* pActor = NULL;

	// �÷��̾� ���� �˻�
	stACTORDATA* pPlayerData = CheckPlayerActor(FileName);
	if(pPlayerData)
	{
		// �÷��̾���ΰ�?
		if(pPlayerData->pActor == NULL)
		{
			pPlayerData->pActor = CreateActor(FileName);
			if(pPlayerData->pActor == NULL)
				return NULL;
		}

		pActor = pPlayerData->pActor;
	}
	else
	{
		// �̹� �ִ°�?
		pActor = CheckGetActor(FileName);
		if(pActor == NULL)
		{
			// �߰�
			pActor = CreateActor(FileName);
			if(pActor == NULL)
				return NULL;

			stACTORDATA actorData;
			actorData.pActor = pActor;
			actorData.strFileName = string(FileName);
			CreateNPCFaceAnimation(pActor, FileName);

			m_vecActorList.push_back(actorData);
		}
	}

	m_pCurrentActor = pActor;
	m_pSceneManager->AddSceneNode(m_pCurrentActor);

	return pActor;
}

TCharacter* TActorMgr::CreateActor( const char* FileName, bool bUseBackgroud /*= false*/ )
{
	// �߰�
	TCharacter* pActor = new TCharacter;

	if(!pActor->TCharacter::Create(FileName, bUseBackgroud))
	{
		SAFE_DELETE(pActor);

		mlog("���� : Mesh����(%s) �ε� ���� \r", FileName);

		return NULL;
	}

	pActor->SetListener(m_pEventListener);

	return pActor;
}

TCharacter* TActorMgr::CheckGetActor( string strFileName )
{
	for(vector<stACTORDATA>::iterator itData = m_vecActorList.begin(); itData != m_vecActorList.end(); itData++)
	{
		if((*itData).strFileName == strFileName)
		{
			// �ִٸ�... �����͸� �������� �ű��.
			stACTORDATA actorData;
			actorData.pActor = (*itData).pActor;
			actorData.strFileName = (*itData).strFileName;

			m_vecActorList.erase(itData);
			m_vecActorList.push_back(actorData);

			return actorData.pActor;
		}
	}

	return NULL;
}

void TActorMgr::SetDefaultPlayerModel()
{
	m_FemalePlayerModel.strFileName = GetPlayerModelFileName(SEX_FEMALE);
	m_FemalePlayerModel.pActor = CreateActor(m_FemalePlayerModel.strFileName.c_str(), true);
	m_FemalePlayerModel.pActor->CreateFaceAnimation();

	m_MalePlayerModel.strFileName = GetPlayerModelFileName(SEX_MALE);
	m_MalePlayerModel.pActor = CreateActor(m_MalePlayerModel.strFileName.c_str(), true);
	m_MalePlayerModel.pActor->CreateFaceAnimation();
}

void TActorMgr::Init()
{
	// �⺻ �� ��������...
	SetDefaultPlayerModel();
}

void TActorMgr::Destroy()
{
	DelCharacter(&m_FemalePlayerModel);
	DelCharacter(&m_MalePlayerModel);

	ActorListDestroy();
}

string TActorMgr::GetPlayerModelFileName( SEX nSex )
{
	string modelSex = TStrings::SexTypeToString(nSex);

	TreeNode^ node = g_pMainApp->SearchNode(GlobalObjects::g_pMainForm->m_ModelTabPage->GetNodes(), gcnew String(modelSex.c_str()));

	Object^ tag = node->Tag;
	String^ modelname(tag->ToString());

	return MStringToCharPointer(modelname);
}

bool TActorMgr::ModelDel( const char* FileName )
{
	// �켱 �÷��̾� ���� üũ
	if(m_FemalePlayerModel.strFileName == string(FileName))
	{
		DelCharacter(&m_FemalePlayerModel);
		return true;
	}
	
	if(m_MalePlayerModel.strFileName == string(FileName))
	{
		DelCharacter(&m_MalePlayerModel);
		return true;
	}

	for(vector<stACTORDATA>::iterator itData = m_vecActorList.begin(); itData != m_vecActorList.end(); itData++)
	{
		if((*itData).strFileName == string(FileName))
		{
			// �ִٸ�... ��������?
			DelCharacter(&(*itData));
			m_vecActorList.erase(itData);

			return true;
		}
	}

	return false;
}

TActorMgr::stACTORDATA* TActorMgr::CheckPlayerActor( string strFileName )
{
	// �÷��̾� üũ
	if(m_FemalePlayerModel.strFileName == strFileName)
		return &m_FemalePlayerModel;
	if(m_MalePlayerModel.strFileName == strFileName)
		return &m_MalePlayerModel;

	return NULL;
}

void TActorMgr::CreateNPCFaceAnimation( TCharacter* pActor, string FileName )
{
	if(pActor == m_FemalePlayerModel.pActor ||
		pActor == m_MalePlayerModel.pActor)
		return;

	string strFileName = FileName;
	if(strFileName.find("hf") != std::string::npos ||
		strFileName.find("hm") != std::string::npos)
		pActor->CreateFaceAnimation(true);
}

void TActorMgr::UpdateAnimation( float fDelta )
{
	if(m_pCurrentActor)
		m_pCurrentActor->UpdateAnimationTC(fDelta);
}

void TActorMgr::ActorListDestroy()
{
	for(vector<stACTORDATA>::iterator itData = m_vecActorList.begin(); itData != m_vecActorList.end(); itData++)
	{
		DelCharacter(&(*itData));
	}

	m_vecActorList.clear();
}