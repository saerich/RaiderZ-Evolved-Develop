#pragma once

// Ÿ���� �����ð����� �����ϴ� ����
class GHateTableLocker
{
public:
	GHateTableLocker();

	// ��밡������ ���� ��ȯ
	bool IsEnable();
	// ��밡������ ����
	void Enable(bool bEnable);
	// ��� ���͸� ���� ������
	void Lock(MUID uidTarget, float fDurationTime);
	// ������ ���¸� �����
	void Unlock();
	// ������ ���Ͱ� �ִ��� ����
	bool IsLock() const;
	// ������ ��� ��ȯ
	MUID GetTarget() const;
	// ƽ ó��
	void Update(float fDelta);
private:
	// ��밡������ ����
	bool m_bEnable;
	// ������ ��� UID
	MUID m_uidTarget;
	// �ð� ����
	MRegulator m_rgrReleaser;
};
