#pragma once

#include "ZFieldManager.h"

/// ���� ���� ������Ʈ�� ������ �ִ� �ʵ� �ν��Ͻ�
/// - ZField �ν��Ͻ��� ZFieldManager�� ������ �ִ�.
class ZGameServerFieldMap : public ZBaseFieldManager
{
public:
	ZGameServerFieldMap() : ZBaseFieldManager() {}
	virtual ~ZGameServerFieldMap();

	void	AddSharedField(ZSharedField* pSharedField) override;
	void	DelSharedField(MUID uidSharedField) override;

	void	AddDynamicFieldMaster(ZDynamicFieldMaster* pDynamicFieldMaster) override;
	void	DelDynamicFieldMaster(MUID uidDynamicFieldMaster) override;
};
