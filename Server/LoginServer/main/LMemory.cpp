#include "stdafx.h"
#include "LMemory.h"
#include "LDBTaskHeaderList.h"


LScopeMempoolReleaser::~LScopeMempoolReleaser()
{
	LDBTaskAccountInsert::release();
	LDBTaskCharDelete::release();
	LDBTaskCharGetLookList::release();
	LDBTaskCharInsert::release();
	LDBTaskLogin::release();
	LDBTaskServerGetStatus::release();
	LAccountDBTaskQuery::release();
	LGameDBTaskQuery::release();
	LLogDBTaskQuery::release();
	LDBTaskGetWorldInfo::release();
}
