#pragma once

class GFieldInfo;

class GFieldAttrGrid
{
public:
	struct Cell
	{
		bool bPvPEnable;
		uint8 nPvPFaction1;
		uint8 nPvPFaction2;

		Cell()
			:bPvPEnable(false)
			,nPvPFaction1(0) 
			,nPvPFaction2(0)
		{}
	};
public:
	GFieldAttrGrid(GFieldInfo* pFieldInfo);
	~GFieldAttrGrid(void);

	// �ʱ�ȭó����
	void Init();
	// �Ҹ�ó����
	void Fini();

	// �ʵ���ǥ�� �׸�����ǥ�� ����
	MPoint	ConvertToGridPos(float fPosX, float fPosY);
	// �Ӽ��� ��ȯ, �ʵ� ��ǥ�� ���ؼ�
	Cell*	FindCell(float fPosX, float fPosY);
	// �Ӽ��� ��ȯ, �׸��� ��ǥ�� ���ؼ�
	Cell*	FindCellViaGridPos(int rx, int ry);
	// ��ȿ�� �׸�����ǥ���� Ȯ��
	bool	IsValidGridPos(int rx, int ry);
	// Ư�� �κа��� ����
	bool	SetCellAttr(MRect rect);
	// �簢�� ������ �´� �Ӽ����� ��ȯ
	bool	GetCellsViaRect( MRect rect, vector<Cell*>& outCells );
private:
	Cell* m_pCells;
	GFieldInfo*					m_pFieldInfo;
	int							m_nGridXSize;
	int							m_nGridYSize;
};