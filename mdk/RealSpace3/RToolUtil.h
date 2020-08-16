#pragma once

#include "RTypes.h"

namespace rs3 {
	/**
	 TOOL���� ���콺�� �巡�� �� ��, ������Ʈ�� �������� ���¿� ���� ���ѵǰ� ����� �ִ� �� .
	 baseMat�� �������� ����ϸ� �巡���� �������� ������ Max�� �����.
	*/
	const UINT MOVEAXIS_X = 1;
	const UINT MOVEAXIS_Y = 2;
	const UINT MOVEAXIS_Z = 3;
	const UINT MOVEAXIS_XY = 4;
	const UINT MOVEAXIS_YZ = 5;
	const UINT MOVEAXIS_XZ = 6;

	RVector GetMoveVector(	UINT nAxisType, const RMatrix &baseMat, const RVector &vPosition,
							const RVector &p1, const RVector &q1, const RVector &q2);

	RS_API RVector GetMoveVector(	UINT nAxisType,const RVector &q1, const RVector &q2);
}
