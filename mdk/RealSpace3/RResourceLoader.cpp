#include "StdAfx.h"
#include "RResourceLoader.h"
#include "RResourceID.h"
#include "MTime.h"

namespace rs3 {

RResourceLoader::RResourceLoader(RResource* pResource, bool bBackground ) : m_pResource(pResource), m_bBackgroundCreate(bBackground)
{
}

const char* RResourceLoader::GetName()
{
	if (m_pResource)
	{
		m_strWorkUnitName = "RResourceLoader, " + m_pResource->GetName();
		return m_strWorkUnitName.c_str();
	}
	else
	{
		return "RResourceLoader";
	}
}

int RResourceLoader::CalcPriority()
{
	if( m_pResource->m_loadingControllers.empty() )
		return m_pResource->m_nLoadingPriority;

	RResource::LOADINGCONTROLLERLIST::iterator itr = m_pResource->m_loadingControllers.begin();
	int nPriority = (*itr)->CalcPriority();
	++itr;

	for( ; itr!= m_pResource->m_loadingControllers.end(); ++itr)
	{
		nPriority = min ( nPriority, (*itr)->CalcPriority() );
	}

	return nPriority;
}

void RResourceLoader::OnBackgroundWork()
{

	if(m_pResource->GetState() == RRS_FILLED ||
		m_pResource->GetState() == RRS_LOADED) return;

	if ( m_pResource->GetState() != RRS_EMPTY )
	{
		m_pResource->m_state = RRS_WRONG;

		return;
	}

	_ASSERT( m_pResource->GetState() == RRS_EMPTY );
	bool bRet = doFill(m_pResource);

	if( !bRet )
	{
		m_pResource->m_state = RRS_WRONG;
		return;
	}

	m_pResource->m_state = RRS_FILLED;
}

void RResourceLoader::OnUpdateCompleteWork()
{
	if ( m_pResource->GetState() != RRS_FILLED && 
		m_pResource->GetState() != RRS_LOADED)	// �ε� ����
	{
		FinishWork();
		m_pResource->SendNotifyAll();
		return;
	}

	if(m_pResource->GetState() == RRS_FILLED)
	{
		RResource::LOAD_RESULT lr = doLoad(m_pResource);

		if(lr == RResource::WRONG)
		{
			FinishWork();
			m_pResource->m_state = RRS_WRONG;
		}else
		if(lr == RResource::WAITING)
		{
			// filled �� ���°� �����Ǹ� ��� ���
			if (m_pBackgroundWorker)
				m_pBackgroundWorker->AddToReadyQueue(this);
		}else
		{
			_ASSERT( lr == RResource::LOADED );
			m_pResource->m_state = RRS_LOADED;
		}
	}

	if(m_pResource->GetState() == RRS_LOADED)	// �ε� �Ϸ�
	{
		if(!m_pResource->SendNotify1())
		{
			FinishWork();
		}else
		{
			if (m_pBackgroundWorker)
				m_pBackgroundWorker->AddToReadyQueue(this);
		}
	}
}

bool RResourceLoader::Load(bool bBackground)
{
	INIT_TCHECK("/check_resource_loadtime");

	if( m_pResource->IsLoadingCompleted() )	// �̹� �ε���. �ε��� �ʿ� ����
	{
		BEGIN_TCHECK;

		m_pResource->SendNotifyAll();

		END_TCHECK("RResourceLoader::Load, SendNotifyAll / " + m_pResource->GetName());

		return true;
	}

	RBackgroundWorker& bg_worker = REngine::GetBackgroundWorker();

	// ��׶��� �ε���û
	if(bBackground && bg_worker.IsInited())
	{
		// �ش� ���ҽ��� �̹� ��׶��� �ε����ΰ��
		if( m_pResource->IsBackgroundCreated() )
		{
			return true;
		}

		_ASSERT( m_pResource->m_pLoader == NULL );

		m_pResource->m_pLoader = new RResourceLoader(m_pResource,true);

		bg_worker.AddToReadyQueue(m_pResource->m_pLoader);
		bg_worker.FlushReadyQueueAndRun();

		return true;
	}else
	// foreground �ε� ��û�ΰ��
	{
		BEGIN_TCHECK;

		// �ش� ���ҽ��� �̹� ��׶��� �ε����ΰ��
		if( m_pResource->IsBackgroundCreated() )
		{
			// ��׶��� �ε� �� ���ҽ��� ���带 �̷�� �ִٸ� �ٽ� ���׶��� ��û�� ���ͼ� �ȵȴ�.
			_ASSERT( !m_pResource->IsBuildLater() );

			// �̹� �ε����̸� �Ϸ�ɶ� ���� ��ٸ���
			while( !m_pResource->IsLoadingCompleted() && bg_worker.GetRemainedWorkableTicketSize() != 0) 
			{
				//Sleep(10);
				m_pResource->SetLoadingPriority(RBGWK_HIGHEST - RBGWK_HIGH_OFFSET);
				m_pResource->m_pLoader->SetPriority(RBGWK_HIGHEST - RBGWK_HIGH_OFFSET);
				bg_worker.ReOrderRunningQueue();
				bg_worker.Update();
			};

			// �ε��� �������Ƿ� �˸�
			m_pResource->SendNotifyAll();

		}else
		{
			// �ε����� �ƴϸ� �ε�
			_ASSERT(m_pResource->GetState() == RRS_EMPTY );
			WorkInTheForeground();
		}

		if(m_pResource->GetState() != RRS_LOADED)
			return false;

		END_TCHECK("RResourceLoader, Foreground / " + m_pResource->GetName());
	}

	// �Ŀ� buildall �� ����
	return true;
}

RResource::LOAD_RESULT RResourceLoader::doLoad( RResource* pResource )
{
	const bool bLog = REngine::GetConfig().m_bLogResource;

	MTimeChecker tc;
	if (bLog)
	{
		tc.Start();
	}

	RResource::LOAD_RESULT ret = m_pResource->Load();

	if (bLog)
	{
		unsigned int nCheckTime = tc.Check();

		const char* szResID = RRESOURCETYPE::GetResoruceIDString(pResource->GetResID());
		string strRet;
		switch (ret)
		{
		case RResource::WRONG: strRet = "Wrong"; break;
		case RResource::LOADED: strRet = "Loaded"; break;
		case RResource::WAITING: strRet = "Waiting"; break;
		}

		mlog(",RResource::Load, %s, %s, %x, %s, %s, (Time=%u)\n", 
			szResID, 
			IsBackgroundCreated() ? "bg" : "fg" , 
			pResource, 
			pResource->GetDebugName(),
			strRet.c_str(),
			nCheckTime);
	}

	return ret;
}


//extern set<string>		g_setResources;

bool RResourceLoader::doFill( RResource* pResource )
{
	const bool bLog = REngine::GetConfig().m_bLogResource;

	MTimeChecker tc;
	if (bLog)
	{
		tc.Start();
	}

	bool bRet = pResource->Fill();

	if (bLog)
	{
		unsigned int nCheckTime = tc.Check();
		string strRet;
		switch (bRet)
		{
		case true: strRet = "TRUE"; break;
		case false: strRet = "FALSE"; break;
		}

		const char* szResID = RRESOURCETYPE::GetResoruceIDString(pResource->GetResID());
		mlog(",RResource::Fill, %s, %s, %x, %s, %s, (Time=%u)\n", 
			szResID, 
			IsBackgroundCreated() ? "bg" : "fg" , 
			pResource, 
			pResource->GetDebugName(),
			strRet.c_str(),
			nCheckTime);

	}

	//string str = pResource->GetDebugName();
	//if (str != "")
	//{
	//	if (g_setResources.find(str) != g_setResources.end())
	//	{
	//		const char* szResID = RRESOURCETYPE::GetResoruceIDString(pResource->GetResID());

	//		mlog("�ι� �ε�: %s %s\n", szResID, str.c_str());
	//	}
	//}

	return bRet;
}


}