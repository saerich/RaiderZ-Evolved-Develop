#pragma once

namespace rs3 {

//////////////////////////////////////////////////////////////////////////
//
// Duplicate Update Check Context
//
//////////////////////////////////////////////////////////////////////////
class RDuplicateUpdateCheckContext
{
public:
	RDuplicateUpdateCheckContext();
	~RDuplicateUpdateCheckContext();

	void	NeedUpdate() { m_bNeedUpdate = true; }
	int		GetUpdatedID() { return m_nUpdatedID; }

	// ������Ʈ�� �ʿ����� üũ�ϰ� �ߺ� ������Ʈ ���� �ʵ��� �Ķ���� ����
	bool	IsNeedUpdate( bool bNeedUpdateHierarchy, int nParentUpdatedID );

	// ���� ������Ʈ ������ ������� ���� ������Ʈ �� �� ���̻� Update�� �ʿ����� ���� �� �ҷ���
	void	SetForceUpdated( bool bNeedUpdateHierarchy, int nParentUpdatedID );	

protected:
	bool	m_bNeedUpdate;		// ������Ʈ �Ǿ�� �ϴ���
	int		m_nUpdatedID;		// �ڽ��� ������Ʈ ID, ������Ʈ �Ҷ����� ����
	int		m_nParentUpdatedID;	// ����ϰ� �ִ� �θ� ������Ʈ ID, �̰� �ٲ�� �ٽ� ����Ѵ�
};

}