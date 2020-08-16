#include "StdAfx.h"
#include "M_ToolCmdMgr.h"
#include "M_CommandBuffer.h"
#include <algorithm>

#include "RSceneNode.h"
#include "C_ControlMediator.h"
#include "C_ControlMediatorHelperAction.h"
#include "C_ControlSceneProperty.h"
#include "C_StatusBarHandler.h"

#include "VL_SceneNodeList.h"

//CToolCmdMgr Method
CToolCmdMgr::CToolCmdMgr(void)
:	m_bPreMouseCmdL_(false),
	m_bPostMouseCmdL_(false),
	m_bPreMouseCmdM_(false),
	m_bPostMouseCmdM_(false),
	m_strNextUndoMsg_("���� ����� ������ �����ϴ�.")
{
	m_tTotalCmdSize_ = 0;
	m_bMapChanged_ = false;
}

CToolCmdMgr::~CToolCmdMgr(void)
{
}

void CToolCmdMgr::ResetCommandManager()
{
	//CommandBuffer �ʱ�ȭ
	for(std::deque<ICommandBuffer*>::iterator itr = m_dqCommandBuffer_.begin();
		itr != m_dqCommandBuffer_.end(); ++itr)
		delete *itr;

	m_dqCommandBuffer_.clear();
	m_tTotalCmdSize_ = 0;
	m_bMapChanged_ = false;
}

//null Ŀ�ǵ� ����� ���� ����Ǿ��ٴ� ��Ǹ� �˷��ִ� ����
void CToolCmdMgr::RecordCommand(ICommandBuffer* _pCommandBuffer)
{
	m_bMapChanged_ = true;

	if(_pCommandBuffer != NULL)
	{
		//�ּ� ������ : 5
		ASSERT( _pCommandBuffer->m_tCommandBufferSize > 4 );

		//check deque for buffer container capacity
		while(true)
		{
			if(m_tTotalCmdSize_ < 1000) // 50���� ������ Ŀ�ǵ常 ����
			{
				break;
			}
			else //��ġ�� �������� �տ��� ���� �̾� �����ش�.
			{
				ICommandBuffer* pCommandBuffer = m_dqCommandBuffer_.front();
				m_tTotalCmdSize_ -= pCommandBuffer->m_tCommandBufferSize;
				delete pCommandBuffer;

				m_dqCommandBuffer_.pop_front();
			}
		}

		//Buffer ó��
		m_tTotalCmdSize_ += _pCommandBuffer->m_tCommandBufferSize;
		m_dqCommandBuffer_.push_back(_pCommandBuffer);
		SetNextUndoMsg_();

		#ifdef _DEBUG_RED_ALERT
		_pCommandBuffer->LogUndoMessage();
		#endif
	}
}

void CToolCmdMgr::UndoCommand()
{
	//Undo�� Ŀ�ǵ尡 �ִ��� ����
	if(m_dqCommandBuffer_.empty())
	{
		CStatusBarHandler::GetInstance()->ShowStatusBarMessage("����� ������ �����ϴ�.");
		return;
	}

	ICommandBuffer* pCommandBuffer = m_dqCommandBuffer_.back();

	m_strCurrentUndoMsg_ = pCommandBuffer->GetUndoMessage() + "�� ��ҵǾ����ϴ�. ";
	m_tTotalCmdSize_ -= pCommandBuffer->m_tCommandBufferSize;

	//Undo With Controller
	VIEW_MESSAGE::ID undo_message = pCommandBuffer->Undo(m_pControlMediator_);

	//������Ʈ ���������� ��θ� ���� �Ϸ���� UpdateView
	m_pControlMediator_->BroadCastMessage(undo_message);

	delete pCommandBuffer;
	m_dqCommandBuffer_.pop_back();

	//StatusBar�� ���� �˸���
	SetNextUndoMsg_();
	m_strFullMsg_ = m_strCurrentUndoMsg_ + m_strNextUndoMsg_;
	CStatusBarHandler::GetInstance()->ShowStatusBarMessage(m_strFullMsg_.c_str());
}

void CToolCmdMgr::SetNextUndoMsg_()
{
	if(m_dqCommandBuffer_.empty())
	{
		m_strNextUndoMsg_ = "���� ����� ������ �����ϴ�.";
		return;
	}

	m_strNextUndoMsg_ = "���� ����� ������ " + m_dqCommandBuffer_.back()->GetUndoMessage() + "�Դϴ�.";
}
