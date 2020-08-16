#ifndef _G_NPC_DECAY_H_
#define _G_NPC_DECAY_H_

class GEntityNPC;
class MRegulator;

class GNPCDecay : public MTestMemPool<GNPCDecay>
{
private:
	GEntityNPC* m_pOwner;
	MRegulator*	m_pDecayRegulator;		///< �Ҹ�ɶ����� �ɸ��� �ð�
	float		m_fCustomDecayTick;		///< �⺻���� �ƴ� Ư�� �̺�Ʈ�� ���ؼ� ���.
										///< ex) ����Ʈ NPC���� ���� �⺻������ �� ��ð� ��ü�� ���� �־�� �Ѵ�.
private:
	void _Decay(float fDecayInterval);

public:
	GNPCDecay(GEntityNPC* pOwner);	
	~GNPCDecay();
	
	void Decay();

	void RemoveToSpawnMgr();
	void Update(float fDelta);
	void SetCustomDecayTick(const float fTick);
};

#endif
