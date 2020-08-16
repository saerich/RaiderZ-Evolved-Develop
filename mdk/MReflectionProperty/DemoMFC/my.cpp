#include "stdafx.h"
#include "my.h"

void REBox::RegisterReflection()
{
	RegisterProperty<DWORD>("��",&GetColor,&SetColor, 0, 0, "��", "���׷�", PE_COLOR);

	RegisterProperty<float>("width",&getWidth,&setWidth,&getMinWidth,&getMaxWidth);
	RegisterProperty<float>("height",&getHeight,&setHeight);
	RegisterProperty<int>("ID",&getID,&setID,&getMinID,&getMaxID);
	RegisterProperty<int>("Number",&getNumber,&setNumber);
	RegisterProperty<char*>("TestName",&getTestName,&setTestName);
	RegisterProperty<char*>("Name",&getName_cstr,&setName_str);
	RegisterProperty<vector<int>>("VectorInt",&getVectorInt,&setVectorInt);

	REGISTER_PROPERTY_ENUM( "SimulateType", m_nSimulateType, "@enum[Animation,Kinematic,Dynamic]���� �ùķ��̼� Ÿ���� ���մϴ�");
}

void RECircle::RegisterReflection()
{
	RegisterProperty<bool>("draw",&getDraw,&setDraw);
}