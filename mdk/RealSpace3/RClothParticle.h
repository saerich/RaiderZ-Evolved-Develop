#pragma once

#include "RTypes.h"

namespace rs3
{
	class RClothParticle
	{
	public:
		RClothParticle(void);
		~RClothParticle(void);

	public:
		rs3::RVector	oriP;			//ó����ġ
		rs3::RVector	p;				//��ġ
		rs3::RVector	newP;			//�����ӵ�
		rs3::RVector	v;				//�ӵ�
		rs3::RVector	n;				//�븻
		rs3::RVector    color;			//����
		float			inverseMass;	// 1/����

	};
}
