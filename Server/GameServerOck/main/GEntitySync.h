#ifndef _GENTITYSYNC_H
#define _GENTITYSYNC_H

#include "GEntity.h"
#include "GFieldGrid.h"

class ProcessChangedSectorEvent;

class GEntitySync : public GEntity
{
public:
	GEntitySync();
	virtual				~GEntitySync();

	virtual bool		Create(MUID& uid);
	virtual bool		SetPos(const vec3& pos) override;
	virtual bool		IsHuge() const		{ return false; }

	inline MPoint GetSectorPoint()		{ return m_sectorPt; }
	inline int	GetSectorX()			{ return m_sectorPt.x; }
	inline int	GetSectorY()			{ return m_sectorPt.y; }

	// ���� ����
	void VisitNeighborSectorEntity(GFieldGrid::Sector::IVisitor& visitor,const ENTITY_TYPEID& type) const;
	void GetNeighborSectorNPC(vector<MUID>& vecNPC) const;
	void GetNeighborSectorPlayer(vector<MUID>& vecPlayer) const;

	// �޼��� ����
	virtual void RouteInEntity(const vector<MUID>& vecPlayers) {};
	virtual void RouteOutEntity(const vector<MUID>& vecPlayers) {};
	virtual void RouteToThisCell(MCommand* pCommand, MUID uidExcept=0) const;
	virtual void RouteToThisCellExceptMe(MCommand* pCommand) const;
	virtual void RouteToField(MCommand* pCommand) const;
	virtual void CollectEssentialReceiver(vector<MUID>& vecReceiverUID)	{ }

protected:
	virtual void OnEnter() override;
	virtual void OnLeave() override;
	
	// ���� ����/���� ���� ���ø��޼����..
public:
	virtual MCommand* CreateFieldInCmd();	// ���� ���� �� ���ο� ������ ��ƼƼ�鿡�� ���� Ŀ�ǵ� ����
	virtual MCommand* CreateFieldOutCmd();	// ���� ��Ż �� ���� ���Ϳ� �����ִ� ��ƼƼ�鿡�� ���� Ŀ�ǵ� ����
protected:
	friend class ProcessChangedSectorEvent;
	virtual void OnVisitedNewSectorPC(const MUID& newSightedPC,MCommand* const fieldInCmd);
	virtual void OnVisitedOldSectorPC(const MUID& oldSIghtedPC,MCommand* const fieldOutCmd);
	virtual void OnVisitedNewSectorNPC(const MUID& newSightedNPC,MCommand* const fieldInCmd);
	virtual void OnVisitedOldSectorNPC(const MUID& oldSIghtedNPC,MCommand* const fieldOutCmd);
	virtual void OnSectorChanged(const MPoint& oldSectorPt);

private:
	void UpdateFieldCell();
	void ResetFieldCell();
	void RouteEnterField(bool bEnter);
	int  GetCellSize() const;
private:
	MPoint m_sectorPt;
};






#endif