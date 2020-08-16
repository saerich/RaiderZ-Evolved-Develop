#include "stdafx.h"
#include "ZFixtureHelper.h"
#include "ZSharedFieldMaster.h"
#include "ZSharedField.h"


SUITE(SharedFieldMaster)
{
	struct Fixture : public FBaseFieldInfo, FBaseField
	{
		Fixture()
		{
			m_pFieldInfo = AddFieldInfo();
			m_pSharedField = AddSharedField(m_pFieldInfo);
			m_pSharedFieldMaster = gmgr.pFieldManager->FindSharedFieldMaster(m_pFieldInfo->m_nFieldID);
		}
		~Fixture()
		{

		}

		ZFieldInfo* m_pFieldInfo;
		ZSharedField* m_pSharedField;
		ZSharedFieldMaster* m_pSharedFieldMaster;
	};

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_Remove)
	{
		CHECK_EQUAL(1, m_pSharedFieldMaster->GetChannelCount());

		/// SUT ����
		m_pSharedFieldMaster->Remove(m_pSharedField->GetChannelID());

		/// ����
		CHECK_EQUAL(0, m_pSharedFieldMaster->GetChannelCount());
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetAllocableChannelID1)
	{
		/// �������� Ȯ��
		int nFirstChannelID = m_pSharedField->GetChannelID();
		CHECK_EQUAL(1, nFirstChannelID);
		CHECK_EQUAL(true, m_pSharedFieldMaster->IsExistChannel(nFirstChannelID));
		
		/// SUT ����
		int nAllocableChannelID = m_pSharedFieldMaster->GetAllocableChannelID();

		/// ����
		CHECK_EQUAL(2, nAllocableChannelID);
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetAllocableChannelID2)
	{
		/// Fixture ��ġ
		int nChannelID = 2;
		AddSharedField(m_pFieldInfo, nChannelID);

		/// SUT ����
		int nAllocableChannelID = m_pSharedFieldMaster->GetAllocableChannelID();

		/// ����
		CHECK_EQUAL(3, nAllocableChannelID);
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetAllocableChannelID3)
	{
		/// Fixture ��ġ
		int nChannelID[3] = {2, 3, 4};
		AddSharedField(m_pFieldInfo, nChannelID[0]);
		AddSharedField(m_pFieldInfo, nChannelID[1]);
		AddSharedField(m_pFieldInfo, nChannelID[2]);

		/// SUT ����
		int nAllocableChannelID = m_pSharedFieldMaster->GetAllocableChannelID();

		/// ����
		CHECK_EQUAL(5, nAllocableChannelID);
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetEnterableChannel)
	{
		/// Fixture ��ġ
		int nChannelID2 = 2;
		ZSharedField* pSharedField2 = AddSharedField(m_pFieldInfo, nChannelID2);

		CHECK_EQUAL(true, m_pSharedField->IsEnterable());	///< 1�� ä��
		CHECK_EQUAL(true, m_pSharedField->IsQuiet());
		CHECK_EQUAL(true, pSharedField2->IsEnterable());	///< 2�� ä��
		CHECK_EQUAL(true, pSharedField2->IsQuiet());

		/// SUT ����, ���� - Should Return Channel1
		CHECK_EQUAL(1, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());

		/// SUT ����, ���� - Should Return Channel1
		CHECK_EQUAL(1, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetEnterableChannel_Sorting)
	{
		/// Fixture ��ġ
		int nChannelID1 = 1;
		m_pSharedFieldMaster->Remove(nChannelID1); ///< 1�� ä���� ����.

		int nChannelID3 = 3;
		AddSharedField(m_pFieldInfo, nChannelID3); ///< 3�� ä���� ���� �߰�.

		int nChannelID2 = 2;
		AddSharedField(m_pFieldInfo, nChannelID2); ///< 2�� ä���� ���߿� �߰�.

		/// SUT ����, ���� - Should Return Channel2 By Sorting
		CHECK_EQUAL(2, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_AddChannel_Sort)
	{
		/// SUT ����
		int nChannelID4 = 4;
		AddSharedField(m_pFieldInfo, nChannelID4); ///< 4�� ä�� �߰�.

		int nChannelID2 = 2;
		AddSharedField(m_pFieldInfo, nChannelID2); ///< 2�� ä�� �߰�.

		int nChannelID3 = 3;
		AddSharedField(m_pFieldInfo, nChannelID3); ///< 3�� ä�� �߰�.

		/// ����
		ZSharedFieldMaster::ZChannelVector vecChannel = m_pSharedFieldMaster->GetChannelVec();
		CHECK_EQUAL(1, vecChannel[0]->GetChannelID());
		CHECK_EQUAL(2, vecChannel[1]->GetChannelID());
		CHECK_EQUAL(3, vecChannel[2]->GetChannelID());
		CHECK_EQUAL(4, vecChannel[3]->GetChannelID());
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetEnterableChannel_FirstChannel_IsNotQuiet_Should_Return_SecondChannel)
	{
		/// Fixture ��ġ
		int nChannelID2 = 2;
		ZSharedField* pSharedField2 = AddSharedField(m_pFieldInfo, nChannelID2);
		while (m_pSharedField->IsQuiet())
			m_pSharedField->IncreasePlayerCount();

		CHECK_EQUAL(true, m_pSharedField->IsEnterable());
		CHECK_EQUAL(false, m_pSharedField->IsQuiet());
		CHECK_EQUAL(true, pSharedField2->IsEnterable());
		CHECK_EQUAL(true, pSharedField2->IsQuiet());

		/// SUT ����, ���� - Should Return Channel1
		CHECK_EQUAL(2, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());

		/// SUT ����, ���� - Should Return Channel1
		CHECK_EQUAL(2, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetEnterableChannel_FirstChannelIsEnterable)
	{
		/// Fixture ��ġ
		int nChannelID = 2;
		ZSharedField* pSharedField2 = AddSharedField(m_pFieldInfo, nChannelID);
		CHECK_EQUAL(true, m_pSharedField->IsEnterable());
		CHECK_EQUAL(true, pSharedField2->IsEnterable());

		/// SUT ����
		ZSharedField* pEnterableField = m_pSharedFieldMaster->GetEnterableChannel();

		/// ����
		CHECK_EQUAL(1, pEnterableField->GetChannelID());
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetEnterableChannel_NotEnterableFirstChannel)
	{
		/// Fixture ��ġ
		int nChannelID = 2;
		ZSharedField* pSharedField2 = AddSharedField(m_pFieldInfo, nChannelID);
		for (int i = 0; i < m_pSharedField->GetPlayerLimit(); ++i) m_pSharedField->IncreasePlayerCount();
		
		CHECK_EQUAL(false, m_pSharedField->IsEnterable());
		CHECK_EQUAL(true, pSharedField2->IsEnterable());

		/// SUT ����
		ZSharedField* pEnterableField = m_pSharedFieldMaster->GetEnterableChannel();

		/// ����
		CHECK_EQUAL(2, pEnterableField->GetChannelID());
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetEnterableChannel_AllChannelIsNotEnterable)
	{
		/// Fixture ��ġ
		int nChannelID = 2;
		ZSharedField* pSharedField2 = AddSharedField(m_pFieldInfo, nChannelID);
		for (int i = 0; i < m_pSharedField->GetPlayerLimit(); ++i)
		{
			m_pSharedField->IncreasePlayerCount();
			pSharedField2->IncreasePlayerCount();
		}

		CHECK_EQUAL(false, m_pSharedField->IsEnterable());
		CHECK_EQUAL(false, pSharedField2->IsEnterable());

		/// SUT ����
		ZSharedField* pEnterableField = m_pSharedFieldMaster->GetEnterableChannel();

		/// ����
		CHECK_EQUAL((ZSharedField*)NULL, pEnterableField);
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetEnterableChannel_NotExistQuietChannel_Sequencial)
	{
		/// Fixture ��ġ
		int nChannelID2 = 2;
		int nChannelID3 = 3;
		ZSharedField* pSharedField2 = AddSharedField(m_pFieldInfo, nChannelID2);
		ZSharedField* pSharedField3 = AddSharedField(m_pFieldInfo, nChannelID3);
		while (m_pSharedField->IsQuiet())
		{
			m_pSharedField->IncreasePlayerCount();
			pSharedField2->IncreasePlayerCount();
			pSharedField3->IncreasePlayerCount();
		}
		CHECK_EQUAL(true, m_pSharedField->IsEnterable());	///< 1�� ä��
		CHECK_EQUAL(false, m_pSharedField->IsQuiet());
		CHECK_EQUAL(true, pSharedField2->IsEnterable());	///< 2�� ä��
		CHECK_EQUAL(false, pSharedField2->IsQuiet());
		CHECK_EQUAL(true, pSharedField3->IsEnterable());	///< 3�� ä��
		CHECK_EQUAL(false, pSharedField3->IsQuiet());

		/// SUT ����, ���� - Should Return Channel1
		CHECK_EQUAL(1, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());

		/// SUT ����, ���� - Should Return Channel2
		CHECK_EQUAL(2, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());

		/// SUT ����, ���� - Should Return Channel3
		CHECK_EQUAL(3, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());

		/// SUT ����, ���� - Should Return Channel1
		CHECK_EQUAL(1, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetEnterableChannel_NotExistQuietChannel_DeleteMiddleChannel)
	{
		/// Fixture ��ġ
		int nChannelID2 = 2;
		int nChannelID3 = 3;
		ZSharedField* pSharedField2 = AddSharedField(m_pFieldInfo, nChannelID2);
		ZSharedField* pSharedField3 = AddSharedField(m_pFieldInfo, nChannelID3);
		while (m_pSharedField->IsQuiet())
		{
			m_pSharedField->IncreasePlayerCount();
			pSharedField2->IncreasePlayerCount();
			pSharedField3->IncreasePlayerCount();
		}

		m_pSharedFieldMaster->GetEnterableChannel(); ///< Return Channel 1
		m_pSharedFieldMaster->GetEnterableChannel(); ///< Return Channel 2

		m_pSharedFieldMaster->Remove(2); ///< 2�� ä�� ����

		/// SUT ����, ���� - Should Return Channel1
		CHECK_EQUAL(1, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());

		/// SUT ����, ���� - Should Return Channel3
		CHECK_EQUAL(3, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());

		/// SUT ����, ���� - Should Return Channel1
		CHECK_EQUAL(1, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());
	}

	TEST_FIXTURE(Fixture, TestSharedFieldMaster_GetEnterableChannel_NotExistQuietChannel_DeleteLastChannel)
	{
		/// Fixture ��ġ
		int nChannelID2 = 2;
		int nChannelID3 = 3;
		ZSharedField* pSharedField2 = AddSharedField(m_pFieldInfo, nChannelID2);
		ZSharedField* pSharedField3 = AddSharedField(m_pFieldInfo, nChannelID3);
		while (m_pSharedField->IsQuiet())
		{
			m_pSharedField->IncreasePlayerCount();
			pSharedField2->IncreasePlayerCount();
			pSharedField3->IncreasePlayerCount();
		}

		m_pSharedFieldMaster->GetEnterableChannel(); ///< Return Channel 1
		m_pSharedFieldMaster->GetEnterableChannel(); ///< Return Channel 2

		m_pSharedFieldMaster->Remove(3); ///< 3�� ä�� ����

		/// SUT ����, ���� - Should Return Channel1
		CHECK_EQUAL(1, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());

		/// SUT ����, ���� - Should Return Channel2
		CHECK_EQUAL(2, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());

		/// SUT ����, ���� - Should Return Channel1
		CHECK_EQUAL(1, m_pSharedFieldMaster->GetEnterableChannel()->GetChannelID());
	}

}