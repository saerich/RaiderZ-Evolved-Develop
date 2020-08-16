#pragma once

#include "XGlueFunction.h"

/// lua���� ����ϴ� ĳ���� ����,����,���� API
class XGlueCharFunction : public XGlueFunction
{
private:
	bool			CheckCurrFrame();
public:
	virtual void DeclGlueFunc(MWLua* pLua);

	/// ĳ���� ���� ��ȯ�Ѵ�.
	int				GetCharCount();
	/// ������ ĳ������ �̸��� ��ȯ�Ѵ�.
	const char*		GetCharName(int nCharIndex);
	/// ������ ĳ������ ������ ��ȯ�Ѵ�.
	int				GetCharLevel(int nCharIndex);
	/// ������ ĳ���Ͱ� �ִ� ���� �ʵ���� ��ȯ�Ѵ�.
	const char*		GetCharInFieldName( int nCharIndex );
	/// ĳ���͸� �����Ѵ�.
	void			SelectCharacter(int nCharIndex);
	/// ĳ���͸� �����Ѵ�.
	void			CreateCharacter(const char* szName, MWLua::table t);
	/// ĳ���͸� �����Ѵ�.
	void			DeleteCharacter(const char* szName, int nCharIndex);
	/// ������ ĳ���ͷ� ������ �����Ѵ�.
	void			StartGame(int nCharIndex);
	/// ĳ���� ������ ���� �����Ѵ�.
	void			ChangeCreationModel(MWLua::table t);
	/// ĳ���� �����ÿ� ������ �� �ִ� �Ӹ� ������ ��ȯ�Ѵ�.
	int				GetHairMaxCount(int nSex);
	/// ĳ���� �����ÿ� ������ �� �ִ� �� ������ ��ȯ�Ѵ�.
	int				GetFaceMaxCount(int nSex);

};
