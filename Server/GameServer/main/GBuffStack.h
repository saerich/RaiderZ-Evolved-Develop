#pragma once

class GBuff;

class GBuffStack : public MTestMemPool<GBuffStack>
{
public:
	GBuffStack(GBuff* pBuff);
	~GBuffStack(void);

	// ������Ʈ
	void Update_Expired( float fDelta );

	// ���� �߰�
	void Increase(float fDurationTime);
	// ���� �Ҹ�
	void Decrease(bool bRoute=true);
	// �Ҹ�Ǵ��� ����
	bool IsExpired() const;

	// ��ø���� ��ȯ
	int GetCount() const;

private:
	void RouteStackIncrease();
	void RouteStackDecrease();
private:
	GBuff*		m_pOwner;
	int			m_nCount;
	deque<float> m_qDurations;
};
