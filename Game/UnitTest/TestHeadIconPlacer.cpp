#include "stdafx.h"
#include "XHeadIcon.h"
#include "XCharacterCaption.h"

// * [ok] �� �Լ��� �׽�Ʈ
// * Caption�� Visible ���̽����� �� Caption �� Offset ���
// * Caption�� Visible ���̽����� ����Ʈ�� Offset ���
// * Refresh �Ǿ��� �� ����Ʈ�� Offset ���


SUITE(HeadIconPlacer)
{
	class TestHeadIconPlacer : public XHeadIconPlacer
	{
	public:
		TestHeadIconPlacer(XCharacterCaption* pNameCaption, XCharacterCaption* pGuildCaption, XMiniHpBar* pMiniHPBar) 
			: XHeadIconPlacer(pNameCaption, pGuildCaption, pMiniHPBar)
		{

		}
		virtual ~TestHeadIconPlacer() {}

		virtual	float GetFontHeight() override
		{
			return 12.0f;
		}

		using XHeadIconPlacer::IsCaptionVisible;
		using XHeadIconPlacer::NextOffset;
	};

	class FHeadIconPlacer
	{
	public:
		FHeadIconPlacer()
		{
			m_pNameCaption = new XCharacterCaption(1, NULL);
			m_pGuildCaption = new XCharacterCaption(2, NULL);
			m_pMiniHPBar = new XMiniHpBar(3, NULL);
			m_pMiniHPBar->Create();

			m_pPlacer = new TestHeadIconPlacer(m_pNameCaption, 
											m_pGuildCaption,
											m_pMiniHPBar);
		}
		~FHeadIconPlacer()
		{
			SAFE_DELETE(m_pPlacer);
			SAFE_DELETE(m_pNameCaption);
			SAFE_DELETE(m_pGuildCaption);
			SAFE_DELETE(m_pMiniHPBar);
		}
		void SetCaptionVisible(bool bName, bool bGuild)
		{
			m_pNameCaption->SetVisible(bName);
			m_pGuildCaption->SetVisible(bGuild);
		}
		void SetCaptionText(const wchar_t* szName, const wchar_t* szGuild)
		{
			if (szName)
			{
				m_pNameCaption->SetText(szName);
			}
			if (szGuild)
			{
				m_pGuildCaption->SetText(szGuild);
			}
		}
		float FH()
		{
			return m_pPlacer->GetFontHeight();
		}

		XCharacterCaption*	m_pNameCaption;
		XCharacterCaption*	m_pGuildCaption;
		XMiniHpBar*			m_pMiniHPBar;

		TestHeadIconPlacer*	m_pPlacer;
	};

	TEST_FIXTURE(FHeadIconPlacer, TestHeadIconPlacer_GetFontHeight)
	{
		float fFontHeight = m_pPlacer->GetFontHeight();
		CHECK_CLOSE(12.0f, fFontHeight, 0.001f);
	}

	TEST_FIXTURE(FHeadIconPlacer, TestHeadIconPlacer_IsCaptionVisible_Text_Negative)
	{
		SetCaptionVisible(true, true);

		// text�� �����Ƿ� IsCaptionVisible�� false�̴�.

		CHECK_EQUAL(false, m_pPlacer->IsCaptionVisible(m_pNameCaption));
		CHECK_EQUAL(false, m_pPlacer->IsCaptionVisible(m_pGuildCaption));
	}

	TEST_FIXTURE(FHeadIconPlacer, TestHeadIconPlacer_IsCaptionVisible)
	{
		SetCaptionVisible(true, false);
		SetCaptionText(L"Name", L"Guild");

		CHECK_EQUAL(true, m_pPlacer->IsCaptionVisible(m_pNameCaption));
		CHECK_EQUAL(false, m_pPlacer->IsCaptionVisible(m_pGuildCaption));
	}

	TEST_FIXTURE(FHeadIconPlacer, TestHeadIconPlacer_IsCaptionVisible2)
	{
		SetCaptionVisible(false, true);
		SetCaptionText(L"Name", L"Guild");

		CHECK_EQUAL(false, m_pPlacer->IsCaptionVisible(m_pNameCaption));
		CHECK_EQUAL(true, m_pPlacer->IsCaptionVisible(m_pGuildCaption));
	}

	TEST_FIXTURE(FHeadIconPlacer, TestHeadIconPlacer_NextOffset)
	{
		float fOffset = 0.0f;
		const float fFontHeight = 12.0f;

		for (int i = 0 ; i < 10; i++)
		{
			float fLastOffset = fOffset;
			m_pPlacer->NextOffset(fOffset, fFontHeight);
			CHECK_CLOSE(fLastOffset + (fFontHeight + CAPTION_MARGIN), fOffset, 0.001f);
		}
	}

	// �÷��̾��� Caption ��ġ ���
	TEST_FIXTURE(FHeadIconPlacer, TestHeadIconPlacer_UpdatePlacerNameCaption_OnlyName)
	{
		SetCaptionVisible(true, false);
		SetCaptionText(L"Name", L"Guild");

		m_pPlacer->UpdatePlayerNameCaption();

		// minihpbar
		CHECK_CLOSE(PLAYER_ID_OFFSET_Z + FH() + MINIBAR_MARGIN, m_pMiniHPBar->GetOffSet(), 0.001f);

		// name
		CHECK_CLOSE(PLAYER_ID_OFFSET_Z + FH(), m_pNameCaption->GetOffSet(), 0.001f);
	}

	TEST_FIXTURE(FHeadIconPlacer, TestHeadIconPlacer_UpdatePlacerNameCaption_OnlyGuild)
	{
		SetCaptionVisible(false, true);
		SetCaptionText(L"Name", L"Guild");

		m_pPlacer->UpdatePlayerNameCaption();

		// minihpbar
		CHECK_CLOSE(PLAYER_ID_OFFSET_Z + FH() + MINIBAR_MARGIN, m_pMiniHPBar->GetOffSet(), 0.001f);

		// guild
		CHECK_CLOSE(PLAYER_ID_OFFSET_Z + FH(), m_pGuildCaption->GetOffSet(), 0.001f);
	}

	TEST_FIXTURE(FHeadIconPlacer, TestHeadIconPlacer_UpdatePlacerNameCaption_Both)
	{
		SetCaptionVisible(true, true);
		SetCaptionText(L"Name", L"Guild");

		m_pPlacer->UpdatePlayerNameCaption();

		// guild�� �Ʒ�
		CHECK_CLOSE(PLAYER_ID_OFFSET_Z + FH(), m_pGuildCaption->GetOffSet(), 0.001f);

		// name
		CHECK_CLOSE(PLAYER_ID_OFFSET_Z + FH() + CAPTION_MARGIN + FH(), m_pNameCaption->GetOffSet(), 0.001f);
	}

	TEST_FIXTURE(FHeadIconPlacer, TestHeadIconPlacer_MakeNPCIconLocalMatrix)
	{
		SetCaptionVisible(true, true);
		SetCaptionText(L"Name", L"Guild");

		RMatrix matLocal;
		m_pPlacer->MakeNPCIconLocalMatrix(matLocal, PLAYER_ID_OFFSET_Z, vec3(vec3::AXISY));

		CHECK_CLOSE(PLAYER_ID_OFFSET_Z + FH() + CAPTION_MARGIN + FH() + CAPTION_MARGIN + NPC_ICON_MARGIN, matLocal._43, 0.001f);
	}

	TEST_FIXTURE(FHeadIconPlacer, TestHeadIconPlacer_MakeNPCIconLocalMatrix_NoCaption)
	{
		SetCaptionVisible(false, false);
		SetCaptionText(L"Name", L"Guild");

		RMatrix matLocal;
		m_pPlacer->MakeNPCIconLocalMatrix(matLocal, PLAYER_ID_OFFSET_Z, vec3(vec3::AXISY));

		CHECK_CLOSE(PLAYER_ID_OFFSET_Z + NPC_ICON_MARGIN, matLocal._43, 0.001f);
	}

}