#pragma once

#include "XBaseApplication.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
// ���ø����̼� Ŭ����
//
class XGameApp : public XBaseApplication
{
public:
							XGameApp();
	virtual					~XGameApp();

	virtual const wchar_t*			GetProductVersion();
	virtual const wchar_t*			GetSVNRevision();
	virtual const wchar_t*			GetSVNDate();
};

