#ifndef _XGAME_INPUT_H
#define _XGAME_INPUT_H

#include "MEvent.h"
#include "XVirtualKeyDef.h"
#include "XKeyCodes.h"
#include "XKeyMapTable.h"
#include "XKeyBuffer.h"
#include "XActionKeyDef.h"
#include <queue>


class XGameInput : public XKeyMapTable
{
protected:
	bool						m_bEnable;

	XActionKeyDef				m_ActionKeyDef;

	bool						m_VirtualKeyPressedTable[ VKEY_COUNT];
	XKeyBuffer					m_KeyBuffer;
	int							m_nLastSequenceActions;
	
	queue< XVirtualKey>			m_qReservedKey;
	unsigned int				m_nQueueSize;



public:
	XGameInput();
	virtual ~XGameInput();

	
	virtual bool OnEvent( MEvent& e);

	
	virtual int CheckSequenceKeyCommand();								///< ����Ű üũ

	void SetEnabled( bool bEnable);
	bool IsEnabled();

	virtual bool CheckEnableKey( XVirtualKey nVKey);					///< Ű ��� ���� ���� Ȯ��
	virtual bool IsVirtualKeyPressed( XVirtualKey nKey);				///< Ű ���۷� Ű ���� Ȯ��
	virtual bool IsVirtualKeyPressedAsync( XVirtualKey nKey);			///< Async�� Ű ���� Ȯ��

	void AddReservedKey( XVirtualKey nKey);
	queue< XVirtualKey>& GetReservedKey();

	XKeyBuffer& GetKeyBuffer();
	void ClearKeyBuffer( bool bPostReleasedKeyEvent =false);
};


#endif // _XGAME_INPUT_H