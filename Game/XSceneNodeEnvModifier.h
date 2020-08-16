/**
@brief ȯ�濡 ���� �ų����� ��ȭ�� �����ϱ� ���� ������� Ŭ����, Ŭ���̾�Ʈ/CoreTool �������� ����
�������� �ϳ��� ȯ���� ����/�ε�/�����Ǵ°ͱ��� �����Ǿ��ְ�, 
ȯ���� ��ȭ�� ť��(1��)�� �ϴ°��� �� Ŭ�����Դϴ�.
�㳷�� ���� ���Ѵ°��� SceneNodeController �� XSceneNodeNightVisibilityController�� �����Ǿ��ְ�,
��Ʈ�ѷ��� �޾��ְų� �����ϴ� ������ NightNodeOperator �� �մϴ�
*/

#ifndef _X_SCENE_NODE_ENV_MODIFIER
#define _X_SCENE_NODE_ENV_MODIFIER

#include "REngine.h"

#include "RSceneNode.h"
#include "RMesh.h"
#include "RMeshNode.h"
#include "RActorNode.h"
#include "RLightSceneNode.h"

#include "RStaticMeshSceneNode.h"
#include "REnvironmentSet.h"
#include "RPointLightSceneNode.h"
#include "RSceneManager.h"
#include "RListener.h"
#include "RSceneNodeController.h"
#include "MLocale.h"

#include "MLocale.h"

class RCameraSceneNode;
class rs3::RCollisionTree;

class XSceneNodeEnvModifier
{
public:
	struct XEnvironmentFadingInfo
	{
		rs3::REnvironment::ETime eTime;
		rs3::REnvironment::EWeather eWeather;
		float fFadingTime;
	};

#define NIGHT_CONTROLLER_ID_STRING "_NightController"

	class NightNodeOperator : public rs3::RSceneNodeConstVisitor
	{
		XSceneNodeEnvModifier* m_pModifier;
	public:
		enum OPERATION {
			UPDATE,
			ENABLER,
			ATTACH,
			DETACH,
		};
		NightNodeOperator(XSceneNodeEnvModifier* pModifier, OPERATION eOperation)
			: m_eOperation(eOperation)
			, m_pModifier(pModifier)
		{}

		// actornode
		void Operate( rs3::RActorNode* pActorNode )
		{
			using namespace rs3;

			switch( m_eOperation )
			{
			case UPDATE :
				{
					RSceneNodeController* pController = pActorNode->GetFirstControllerByName( RSNCTRL_POSTUPDATE, NIGHT_CONTROLLER_ID_STRING );
					if(pController)
						pController->Update(NULL);
				}break;
			case ENABLER :
				{
					RSceneNodeController* pController = pActorNode->GetFirstControllerByName( RSNCTRL_POSTUPDATE, NIGHT_CONTROLLER_ID_STRING );
					if(pController)
						pController->Enable(true);
				}break;
			case ATTACH :
				{
					RSceneNodeController* pExistController = pActorNode->GetFirstControllerByName( RSNCTRL_POSTUPDATE, NIGHT_CONTROLLER_ID_STRING );
					_ASSERT( pExistController == NULL );	// �̹� ��ϵǾ������� �ȵ���.

					XSceneNodeNightVisibilityController* pController = new XSceneNodeNightVisibilityController(m_pModifier);
					pActorNode->AddController( pController );
				}break;
			case DETACH :
				{
					RSceneNodeController* pController = pActorNode->GetFirstControllerByName( RSNCTRL_POSTUPDATE, NIGHT_CONTROLLER_ID_STRING );
					if(pController)
					{
						pActorNode->RemoveController(pController);
						delete pController;
					}
				}break;
			}
		}

		// point light
		void Operate( rs3::RPointLightSceneNode* pLightSceneNode )
		{
			switch( m_eOperation )
			{
			case UPDATE :
				pLightSceneNode->m_fApplyIntensity = pLightSceneNode->m_fIntensity * m_pModifier->GetVisibleAtNightVisibility();
				break;
			}
		}

		virtual bool OnPreOrderVisit( const rs3::RSceneNode* pSceneNode ){ return true; }
		virtual void OnPostOrderVisit( const rs3::RSceneNode* pSceneNode )
		{
			using namespace rs3;

			// actornode ����
			RActorNode* pActorNode = MDynamicCast(RActorNode, pSceneNode);
			if (pActorNode)
			{
				RMeshNode* pMeshNode = pActorNode->m_pMeshNode;
				if (NULL == pMeshNode) return;

				// set_visible_at_night = true ������Ƽ�� �������� �㳷ó��
				if ( pMeshNode->FindUserProperty("set_visible_at_night = true") )
					Operate( pActorNode	);
				return;
			}

			// point light
			RPointLightSceneNode* pLight = MDynamicCast(RPointLightSceneNode, pSceneNode);
			const char* VISIBLE_AT_NIGHT = "night_";
			if(pLight && strnicmp(pLight->GetNodeName().c_str(), VISIBLE_AT_NIGHT, strlen(VISIBLE_AT_NIGHT))==0 )
				Operate( pLight );
		}
	protected:
		OPERATION				m_eOperation;
	};

	XSceneNodeEnvModifier(rs3::REnvironmentSet* pEnvironmentSet, rs3::RWorldSceneNode * pWorldSceneNode) 
		: m_nQueueSize(0)
		, m_pEnvironmentSet(pEnvironmentSet)
		, m_fNightVisibility(1)
		, m_pWorldSceneNode(pWorldSceneNode)
	{
	}

	virtual ~XSceneNodeEnvModifier()
	{
	}

	void SetQueueSize(int nQueue) { m_nQueueSize = nQueue; }

	void Clear()
	{
		m_bBeforeNight = false;
		m_bCurrentNight = false;
	}

	void ProcessQueue( rs3::RCollisionTree* pCollisionTree )
	{
		// ó���� ť�� ������ ����
		if(m_queueEnvNexts.empty()) return;

		// ȯ���� ���� ���̶� ó�� �Ұ�
		if( m_pEnvironmentSet && m_pEnvironmentSet->IsBlending()) 
		{
			// ť�� 0 �̸� ������ �������Ѽ� ó���ؾ� ������ �ʴ´�
			if(m_nQueueSize==0) 
				m_pEnvironmentSet->EndBlending();
			else
				return;
		}

		// ������ ó��
		XEnvironmentFadingInfo info = m_queueEnvNexts.front();
		m_queueEnvNexts.pop_front();

		if( m_pEnvironmentSet)
			m_pEnvironmentSet->SetCurrentEnvSet( info.eTime, info.eWeather, true, info.fFadingTime, true, pCollisionTree );

		bool bNight = IsNightTime( info.eTime );
		ChangeEnv(bNight, false, pCollisionTree);

		UpdateVisibleAtNightVisibility();

		// fading�� ������ ��� ������Ʈ ���ش�.
		if( info.fFadingTime == 0.0f )
		{
			UpdateVisibility();
		}
	}

	void Update( rs3::RCollisionTree* pCollisionTree )
	{
		ProcessQueue( pCollisionTree );

		if (m_pEnvironmentSet )
		{
			// ȯ���� ���� ������Ʈ ���ش�.
			m_pEnvironmentSet->Update( pCollisionTree );

			if( m_pEnvironmentSet->IsBlending() )
			{
				UpdateVisibleAtNightVisibility();
				UpdateVisibility();
			}
		}
	}

	void UpdateVisibility()
	{
		NightNodeOperator updater( this, NightNodeOperator::UPDATE );
		m_pWorldSceneNode->Traverse( &updater );
	}

	void ChangeEnv(bool bNight, bool bBackgroundLoading, rs3::RCollisionTree* pCollisionTree)
	{
		m_bBeforeNight = m_bCurrentNight;
		m_bCurrentNight = bNight;

		NightNodeOperator controllerEnabler( this, NightNodeOperator::ENABLER );
		m_pWorldSceneNode->Traverse(&controllerEnabler);

		// ��׶��尡 �ƴϸ� �ٷ� ������Ʈ ���ش�.
		if (false == bBackgroundLoading)
			Update(pCollisionTree);
	}

	void SetEnvSet(const wchar_t* szEnvname)
	{
		if( m_pEnvironmentSet)
			m_pEnvironmentSet->SetCurrentEnvironment(MLocale::ConvUTF16ToAnsi(szEnvname));
	}

	bool SetCurrentEnvSet(rs3::REnvironment::ETime eTime, rs3::REnvironment::EWeather eWeather, bool bFade, float fFadingTime, rs3::RCollisionTree* pCollisionTree)
	{
		XEnvironmentFadingInfo env;
		env.fFadingTime = fFadingTime;
		env.eWeather = eWeather;
		env.eTime = eTime;

		m_queueEnvNexts.push_back( env );
		
		ProcessQueue( pCollisionTree );

		while( m_queueEnvNexts.size() > m_nQueueSize )
			m_queueEnvNexts.pop_front();

		return true;
	}

	bool IsNightTime( rs3::REnvironment::ETime time )
	{
		if( time == rs3::REnvironment::NIGHT )
			return true;

		return false;
	}

	void UpdateVisibleAtNightVisibility()
	{
		float fBlendingFactor = ( m_pEnvironmentSet) ? m_pEnvironmentSet->GetBlendingFactor() : 1.f;
		float fLightBefore = m_bBeforeNight ? 1.f : 0.f;
		float fLightCurrent = m_bCurrentNight ? 1.f : 0.f;
		float fNightVisibility = fLightBefore * fBlendingFactor + fLightCurrent * (1.0f - fBlendingFactor);
		m_fNightVisibility = fNightVisibility;
	}

	class XSceneNodeNightVisibilityController : public rs3::RSceneNodeController
	{
		XSceneNodeEnvModifier*	m_pModifier;
		float					m_fVisibility;

	public:
		XSceneNodeNightVisibilityController(XSceneNodeEnvModifier* pModifier) 
			: RSceneNodeController(rs3::RSNCTRL_POSTUPDATE)
			, m_pModifier(pModifier)
			, m_fVisibility(1.f)
		{
			m_strControllerName = NIGHT_CONTROLLER_ID_STRING;
			m_bManaged = true;
		}

		virtual void Update(rs3::RCameraSceneNode* pCamera)
		{
			float fNightVisibility = m_pModifier->GetVisibleAtNightVisibility();
//			mlog("%3.3f \n",fNightVisibility );
			if(fNightVisibility <= 0)
				m_pSceneNode->SetVisible( false );
			else
				m_pSceneNode->SetVisible( true );

			m_pSceneNode->SetVisibility( m_fVisibility * fNightVisibility );
			m_pSceneNode->UpdateVisibility();

			// fading �� �������� disable
			if(fNightVisibility <= 0 || fNightVisibility >= 1)
				Enable(false);
		}

		virtual void OnAdd()
		{
			m_fVisibility = m_pSceneNode->GetVisibility();
		}
	};

	void		AttachNightNodeController( const rs3::RSceneNode* pNodeRootToAttach )
	{
		NightNodeOperator controllerAttacher( this, NightNodeOperator::ATTACH );
		pNodeRootToAttach->Traverse( &controllerAttacher );
	}

	/// attach �� root scenenode ���Ϸ� traverse �ؼ� controller ���� ������ �ش�
	void		DetachNightNodeController( const rs3::RSceneNode* pNodeRootToDetatch )
	{
		NightNodeOperator controllerDetacher( this, NightNodeOperator::DETACH );
		pNodeRootToDetatch->Traverse( &controllerDetacher );
	}

	float	GetVisibleAtNightVisibility()	///< visible_at_night ����� �������� visibility
	{
		return m_fNightVisibility;
	}

private:
	unsigned int				m_nQueueSize;		///< ���� ���� ��
	deque<XEnvironmentFadingInfo> m_queueEnvNexts;	// ���� ����� ȯ��

	rs3::REnvironmentSet*		m_pEnvironmentSet;
	rs3::RWorldSceneNode*		m_pWorldSceneNode;

	bool						m_bCurrentNight;
	bool						m_bBeforeNight;

	float						m_fNightVisibility;			///< �㿡�� ���̴� ������Ʈ�� ���
};

#endif // _X_SCENE_NODE_ENV_MODIFIER