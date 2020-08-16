#pragma once

#include "RLib.h"
#include "MVector2.h"
#include "MVector3.h"
#include "MMath.h"
#include <vector>
#include <functional>
#include <algorithm>


class CML2_API MConvexHullAlgorithm
{
public:
	//Points : �Է����� ���� ����
	//SortedPoints : ConvexHull�� �̷�� ������ CounterClockWise�������� ���ĵǰ� ����
	static void MakeConvexHullByCounterClockWise2D(const std::vector<MVector2>	&Points,std::vector<MVector2>	&SortedPoints);
	static bool MakeConvexHullByCounterClockWise3D(const std::vector<MVector3>	&Points,std::vector<MVector3>	&SortedPoints);

public://MakeConvexHullByCounterClockWise()�� ����� �Լ���
	static int		BottomRightMostPointIndex(const std::vector<MVector2> &Points);
	static float	GetAngle(const MVector2& A,const MVector2& B);
	static void		SortByAngle(std::vector<MVector2>& Points);
	static float	IsLeft(const MVector2& P0,const MVector2& P1,const MVector2& P2);
	static bool		IsConvexPoint(const MVector2& P0,const MVector2& P1,const MVector2& P2);
	static MMatrix	TransformPlanePointsToXYPlane(const std::vector<MVector3>	&Points,std::vector<MVector3>& TransformedPoints);
	


public:
	//3���� �̷�� 4������ ����. ������� �����ε� ���� �� �ִ�.
	static float QuadrangleArea(const MVector2 &A,const MVector2 &B,const MVector2 &C);

	//true�� ���� : ABC�� positive area, A->B->C�� counterclockwise
	static bool	 CounterClockWise(const MVector2 &A,const MVector2 &B,const MVector2 &C) { return QuadrangleArea(A,B,C) > 0; }
};



