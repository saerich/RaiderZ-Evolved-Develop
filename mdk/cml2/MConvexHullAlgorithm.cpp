#include "stdafx.h"
#include "MConvexHullAlgorithm.h"


bool MConvexHullAlgorithm::MakeConvexHullByCounterClockWise3D( const std::vector<MVector3> &Points,std::vector<MVector3> &SortedPoints )
{
	_ASSERT( 3 <= Points.size() );
	SortedPoints.clear();

	//1. 3d�������� ����� ���� XY������� ��ǥ�̵�
	std::vector<MVector3> TransformedPoints3D;
	MMatrix M = MConvexHullAlgorithm::TransformPlanePointsToXYPlane(Points,TransformedPoints3D);

	std::vector<MVector2> TransformedPoints2D;
	for(unsigned int i=0;i<TransformedPoints3D.size();++i)
	{
		if( false ==  MMath::Equals(0.0f,TransformedPoints3D[i].z) )
		{
			return false;
		}
		TransformedPoints2D.push_back(MVector2(TransformedPoints3D[i].x,TransformedPoints3D[i].y));
	}

	//2. xy�����¿��� convex hull����
	std::vector<MVector2> ConvexHullPoints;
	MConvexHullAlgorithm::MakeConvexHullByCounterClockWise2D(TransformedPoints2D,ConvexHullPoints);

	//3. xy��鿡�� �ٽ� ���� ������� �����̵�
	for(unsigned int i=0;i<ConvexHullPoints.size();++i)
	{
		MVector3 Pin2D(ConvexHullPoints[i].x,ConvexHullPoints[i].y,0);
		MVector3 Pin3D;
		M.TransformVect(Pin2D,Pin3D);
		SortedPoints.push_back(Pin3D);
	}

	return true;
}

MMatrix MConvexHullAlgorithm::TransformPlanePointsToXYPlane( const std::vector<MVector3> &Points,std::vector<MVector3>& TransformedPoints )
{
	_ASSERT( 3 <= Points.size() );
	TransformedPoints.clear();

	//1. 3d�������� ����� 3d�������� ����
	MVector3 n = MMath::RightHandedNormal(Points[0],Points[1],Points[2]);
	n.Normalize();
	MVector3 X = Points[1] - Points[0]; X.Normalize();
	MMatrix	M; M.SetLocalMatrix(Points[0],X,n);
	MMatrix MI;  M.GetInverse(&MI);

	//2. ��ǥ�̵��Ͽ� XY������� ���� �̵�
	for(unsigned int i=0;i<Points.size();++i)
	{
		MVector3 TransformedV;
		MI.TransformVect(Points[i],TransformedV);
		TransformedPoints.push_back( TransformedV );
	}

	return M;
}

//Computational Geometry in C , p.84
//Graham scan�˰���
void MConvexHullAlgorithm::MakeConvexHullByCounterClockWise2D(const std::vector<MVector2> &Points,std::vector<MVector2> &SortedPoints )
{
	_ASSERT( 3 <= Points.size() );

	//1. SortedPoints�ȿ� Points�� ����
	SortedPoints.clear();
	for(unsigned int i=0;i<Points.size();++i)
	{
		SortedPoints.push_back(Points[i]);
	}

	//2. SortedPoints�� ������ ����
	MConvexHullAlgorithm::SortByAngle(SortedPoints);
	_ASSERT( Points.size() == SortedPoints.size() );

	//3. covex���� �ƴϸ� ���ֹ���
	for(unsigned int i=1;i<SortedPoints.size();)
	{
		int P0 = i-1;
		int P1 = i;
		int P2 = i+1;

		if( P1 ==  SortedPoints.size() -1 ) P2 = 0;

		if( MConvexHullAlgorithm::IsConvexPoint( SortedPoints[P0],SortedPoints[P1],SortedPoints[P2] ) )
		{
			++i;
		}
		else
		{
			std::vector<MVector2>::iterator itor = SortedPoints.begin()+i;
			SortedPoints.erase(itor);
		}
	}

}

int MConvexHullAlgorithm::BottomRightMostPointIndex( const std::vector<MVector2> &Points )
{
	int Size = Points.size();

	int BottomMostIndex = 0;	//���� �Ʒ�/�����ʿ� ��ġ�� ���� �ε���
	for(int i=0;i<Size;i++)
	{
		if(  Points[i].y < Points[BottomMostIndex].y )
		{
			BottomMostIndex =  i;
		}
		else if( Points[i].y == Points[BottomMostIndex].y )
		{
			if( Points[i].x > Points[BottomMostIndex].x ) BottomMostIndex = i;
		}
	}

	return BottomMostIndex;
}

//��A,X��,��B�� �̷�� ���� 
float MConvexHullAlgorithm::GetAngle( const MVector2& A,const MVector2& B )
{
	double DeltaX = B.x - A.x;
	double DeltaY = B.y - A.y;

	if( DeltaX == 0 && DeltaY == 0 )
	{
		return 0;
	}

	if( DeltaX == 0 )
	{
		if( DeltaY > 0 ) return 90;
		else if( DeltaY < 0 ) return 270;
	}

	if( DeltaY == 0 )
	{
		if( DeltaX > 0 ) return 0;
		else if( DeltaX < 0 ) return 180;
	}

	double Angle = atan(DeltaY/DeltaX)*(180.0/3.141592); //  -90 degree <= Angle <= 90 degree
	Angle = fabs(Angle);	// 0 degree <= Angle <= 90 degree

	if (DeltaX>=0 && DeltaY>=0)
	{//1��и�
		Angle=Angle;
	}
	else if (DeltaX<0 && DeltaY>0)
	{//2��и�
		Angle=180-Angle;
	}
	else if (DeltaX<0 && DeltaY<=0)
	{//3��и�
		Angle=180+Angle;
	}
	else if (DeltaX>=0 && DeltaY<0)
	{//4��и�
		Angle=360-Angle;
	}

	return static_cast<float>(Angle);

}

void MConvexHullAlgorithm::SortByAngle(std::vector<MVector2>& Points)
{
	int NumPoints = Points.size();
	if( 0 == NumPoints || 1 == NumPoints ) return;

	//1. BottomRight �� ã��
	int BottomRightMostIndex = MConvexHullAlgorithm::BottomRightMostPointIndex(Points);

	//2. BottomRight���� �ٸ������� ���� ���ϱ�
	std::vector<float> Angles;
	for(int i=0;i<NumPoints;i++)
	{
		Angles.push_back( MConvexHullAlgorithm::GetAngle(Points[BottomRightMostIndex],Points[i]) );
	}
	_ASSERT( Angles.size() == Points.size() );

	//3. ������ ���� ����
	std::multimap<float,MVector2> PointsSortedByAngle;
	for(int i=0;i<NumPoints;i++)
	{
		PointsSortedByAngle.insert(std::pair<float,MVector2>(Angles[i],Points[i]));
	}
	_ASSERT( PointsSortedByAngle.size() == Points.size() );

	//4, ������ ���ĵ� ������ Points�� �־��ֱ�
	Points.clear();
	for(std::multimap<float,MVector2>::iterator itor = PointsSortedByAngle.begin(); itor != PointsSortedByAngle.end() ; itor++ )
	{
		const MVector2& P =itor->second;
		Points.push_back(P);	
	}
	_ASSERT( Points.size() == PointsSortedByAngle.size() );
}

//Computational Geometry in C , p.27
float MConvexHullAlgorithm::QuadrangleArea(const MVector2 &A,const MVector2 &B,const MVector2 &C )
{
	return (B.x-A.x)*(C.y-A.y) - (C.x-A.x)*(B.y-A.y);
}

//P2���� P0-P1������ ���ʿ� �ֳ�?
//������̸� P2���� ���ʿ� �ִ�.
float MConvexHullAlgorithm::IsLeft( const MVector2& P0,const MVector2& P1,const MVector2& P2 )
{
	return (P1.x - P0.x)*(P2.y - P0.y) - (P2.x - P0.x)*(P1.y - P0.y);
}

//P1�� convex point�� true�� ����
bool MConvexHullAlgorithm::IsConvexPoint( const MVector2& P0,const MVector2& P1,const MVector2& P2 )
{
	if( MConvexHullAlgorithm::IsLeft(P0,P1,P2) > 0 ) return true;
	return false;
}
