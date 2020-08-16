#pragma once

#include "MMCode.h"


/// ���ڿ� ���� �ڵ� Proc 
////////////////////////////////////////////

class XAniControlCodeProc : public mmcode::MControlCodeProc
{
private:
	void _Proc(const string& strAniName, MUID uidOwner);
public:
	XAniControlCodeProc() : mmcode::MControlCodeProc() {}
	virtual ~XAniControlCodeProc() { }
	virtual void Proc(const string& strCodeParam, void* pParam=0)
	{
		_Proc(strCodeParam, *(MUID*)(pParam));
	}
};

class XMotionControlCodeProc : public mmcode::MControlCodeProc
{
private:
	void _Proc(const string& strMotionName, MUID uidOwner);
public:
	XMotionControlCodeProc() : mmcode::MControlCodeProc() {}
	virtual ~XMotionControlCodeProc() { }
	virtual void Proc(const string& strCodeParam, void* pParam=0)
	{
		_Proc(strCodeParam, *(MUID*)(pParam));
	}
};


class XTalentControlCodeProc : public mmcode::MControlCodeProc
{
private:
	void _Proc(int nTalentID, MUID uidOwner);
public:
	XTalentControlCodeProc() : mmcode::MControlCodeProc() {}
	virtual ~XTalentControlCodeProc() { }
	virtual void Proc(const string& strCodeParam, void* pParam=0)
	{
		int nTalentID = atoi(strCodeParam.c_str());
		_Proc(nTalentID, *(MUID*)(pParam));
	}
};


class XSoundControlCodeProc : public mmcode::MControlCodeProc
{
private:
	void _Proc(const string& strSoundName, MUID uidOwner);
public:
	XSoundControlCodeProc() : mmcode::MControlCodeProc() {}
	virtual ~XSoundControlCodeProc() { }
	virtual void Proc(const string& strCodeParam, void* pParam=0)
	{
		_Proc(strCodeParam, *(MUID*)(pParam));
	}
};


/// ���ڿ� ġȯ�� Proc 
////////////////////////////////////////////

// �̸�
class XNameReplaceCharsProc : public mmcode::MReplaceCharsProc
{
private:
	string _Proc(MUID uidOwner);

public:
	virtual string Proc(void* pParam=0)
	{
		return _Proc(*(MUID*)(pParam));
	}
};

// ����
class XSexReplaceCharsProc : public mmcode::MReplaceCharsProc
{
private:
	string _Proc(MUID uidOwner);

public:
	virtual string Proc(void* pParam=0)
	{
		return _Proc(*(MUID*)(pParam));
	}
};

// ����
class XRaceReplaceCharsProc : public mmcode::MReplaceCharsProc
{
private:
	string _Proc(MUID uidOwner);

public:
	virtual string Proc(void* pParam=0)
	{
		return _Proc(*(MUID*)(pParam));
	}
};