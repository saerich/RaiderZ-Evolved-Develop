#pragma once


namespace mint3 {

class MScriptHelper
{
public:
	MScriptHelper() {}
	virtual ~MScriptHelper() {}

	// Draw ����
	// ����
	BYTE m_a;		// ����
	BYTE m_r;		// ����
	BYTE m_g;		// ���
	BYTE m_b;		// �Ķ�

	static void glueDrawLine();//int sx, int sy, int tx);//, int ty, unsigned int color);
	void glueDrawBox(int sx, int sy, int tx, int ty);
	void glueFillBox(int sx, int sy, int tx, int ty);
};

} // namespace mint3
