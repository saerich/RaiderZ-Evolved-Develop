#include "stdafx.h"
//#include "XSoundNameMaker.h"
/*
SUITE(SoundNameMaker)
{
	class TestSoundNameMaker : public XSoundNameMaker
	{
	public:
		static wstring MakeHitSound(wstring strPrefix, wstring strWeapon, wstring strArmor, wstring strPostfix)
		{
			wstring strRet;
			_MakeHitSound(strRet, strPrefix, strWeapon, strArmor, strPostfix);
			return strRet;
		}
	};

	TEST(TestSoundNameMaker_MakeHitSound_Normal)
	{
		wstring strPrefix, strWeapon, strArmor, strPostfix;

		strPrefix = L"ss";
		strWeapon = L"slash";
		strArmor = L"flesh";
		strPostfix = L"";

		CHECK(wstring(L"ss_slash_flesh") == TestSoundNameMaker::MakeHitSound(strPrefix, strWeapon, strArmor, strPostfix));
	}

	TEST(TestSoundNameMaker_MakeHitSound_Postfix1)
	{
		wstring strPrefix, strWeapon, strArmor, strPostfix;

		strPrefix = L"ss";
		strWeapon = L"slash";
		strArmor = L"flesh";

		// postfix �տ� 'ss'�� ������ ���� �̸� ��°�� ��ü
		strPostfix = L"ss_blunt_stone";

		CHECK(wstring(L"ss_blunt_stone") == TestSoundNameMaker::MakeHitSound(strPrefix, strWeapon, strArmor, strPostfix));
	}

	TEST(TestSoundNameMaker_MakeHitSound_Postfix2)
	{
		wstring strPrefix, strWeapon, strArmor, strPostfix;

		strPrefix = L"ss";
		strWeapon = L"slash";
		strArmor = L"flesh";

		// postfix ���� �������� '_'�� ������ �տ������� 'weapon_' ���� ��ü
		strPostfix = L"ss_myshield_";

		CHECK(wstring(L"ss_myshield_flesh") == TestSoundNameMaker::MakeHitSound(strPrefix, strWeapon, strArmor, strPostfix));
	}

	TEST(TestSoundNameMaker_MakeHitSound_Postfix3)
	{
		wstring strPrefix, strWeapon, strArmor, strPostfix;

		strPrefix = L"ss";
		strWeapon = L"slash";
		strArmor = L"flesh";

		// postfix�� �̰͵� ���͵� �ƴϸ� ���� �������� ����
		strPostfix = L"sup";

		CHECK(wstring(L"ss_slash_flesh_sup") == TestSoundNameMaker::MakeHitSound(strPrefix, strWeapon, strArmor, strPostfix));
	}

	TEST(TestSoundNameMaker_MakeHitSound_Postfix4)
	{
		wstring strPrefix, strWeapon, strArmor, strPostfix;

		strPrefix = L"ss";
		strWeapon = L"slash";
		strArmor = L"flesh";

		// postfix�� �̰͵� ���͵� �ƴϸ� ���� �������� ����
		strPostfix = L"ss_fs";

		CHECK(wstring(L"ss_fs") == TestSoundNameMaker::MakeHitSound(strPrefix, strWeapon, strArmor, strPostfix));
	}

}
*/