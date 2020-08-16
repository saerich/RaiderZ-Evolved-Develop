#include "stdafx.h"
#include "RFMODSoundMgr.h"
#include "MTypes.h"
#include "MFileSystem.h"

#ifndef _DEBUG
	#define ACCESSTIMELIMIT				60000			///< 1�� ���� �ҷ����� ������ ����?
#else
	#define ACCESSTIMELIMIT				10000
#endif

//�� ���忡 ȿ���� �������� ����Ʈ����� �����ؾ� �Ѵ�.
namespace RS2
{
	extern void ERRCHECK(FMOD_RESULT result);

	RFMODSound::RFMODSound()
	: m_pSound(NULL)
	, m_dReadTime(0)
	, m_nRefCount(0)
	, m_pStreamBuff( NULL)
	{
	}

	RFMODSound::~RFMODSound(void)
	{
		ReleaseSound();

		m_nRefCount = 0;
		m_dReadTime = 0;
	}

	void RFMODSound::ReleaseSound()
	{
		FMOD_RESULT			result;

		if (m_pSound)
		{
			result = m_pSound->release();
			ERRCHECK(result);

			m_pSound = NULL;
		}

		if ( m_pStreamBuff != NULL)
		{
			delete [] m_pStreamBuff;
			m_pStreamBuff = NULL;
		}
	}
	
	// TODO : �����̸�, �÷���
	bool RFMODSound::CreateSound(FMOD::System* pFMODSystem, string szKey, FMOD_MODE mode)
	{
		_ASSERT(m_pSound == NULL);

		FMOD_RESULT result;
		if ( mode & FMOD_OPENMEMORY)
		{
			MFile mf;
			if ( mf.Open( szKey.c_str()) == false)
				result = FMOD_ERR_FILE_NOTFOUND;

			else
			{
				unsigned int _size = mf.GetLength();
				char* pStreamBuff = new char[ _size];
				mf.ReadAll( pStreamBuff, _size);
				mf.Close();

				FMOD_CREATESOUNDEXINFO	exinfo;
				memset( &exinfo, 0, sizeof( FMOD_CREATESOUNDEXINFO));
				exinfo.cbsize = sizeof( FMOD_CREATESOUNDEXINFO);
				exinfo.length = _size;

				result = pFMODSystem->createSound( pStreamBuff, mode, &exinfo, &m_pSound);

				delete [] pStreamBuff;
				pStreamBuff = NULL;
			}
		}
		else
			result = pFMODSystem->createSound(szKey.c_str(), mode, NULL, &m_pSound);

		ERRCHECK(result);

		if (m_pSound == NULL) return false;

		return true;
	}

	bool RFMODSound::CreateMusic(FMOD::System* pFMODSystem, string szKey, FMOD_MODE mode)
	{
		_ASSERT(m_pSound == NULL);
	
		FMOD_RESULT result;
		if ( mode & FMOD_OPENMEMORY)
		{
			MFile mf;
			if ( mf.Open( szKey.c_str()) == false)
				result = FMOD_ERR_FILE_NOTFOUND;

			else
			{
				if ( m_pStreamBuff != NULL)
				{
					delete [] m_pStreamBuff;
					m_pStreamBuff = NULL;
				}

				unsigned int _size = mf.GetLength();
				m_pStreamBuff = new char[ _size];
				mf.ReadAll( m_pStreamBuff, _size);
				mf.Close();

				FMOD_CREATESOUNDEXINFO	exinfo;
				memset( &exinfo, 0, sizeof( FMOD_CREATESOUNDEXINFO));
				exinfo.cbsize = sizeof( FMOD_CREATESOUNDEXINFO);
				exinfo.length = _size;

				result = pFMODSystem->createStream( m_pStreamBuff, mode, &exinfo, &m_pSound);
			}
		}
		else
			result = pFMODSystem->createStream(szKey.c_str(), mode, NULL, &m_pSound);

		ERRCHECK(result);

		if (m_pSound == NULL) return false;

		return true;
	}

	unsigned int RFMODSound::GetSoundLength()
	{
		FMOD_RESULT			result;
		unsigned int	length = 0;

		result = m_pSound->getLength(&length, FMOD_TIMEUNIT_MS);
		ERRCHECK(result);

		return length;
	}

	void RFMODSound::Set3DMinMaxDistance( float fMinDistance, float fMaxDistance )
	{
		FMOD_RESULT result = m_pSound->set3DMinMaxDistance(fMinDistance, fMaxDistance);
		ERRCHECK(result);
	}
	//////////////////////////////////////////////////////////////////////////
	// RFMODSoundMgr

	RFMODSoundMgr::RFMODSoundMgr( FMOD::System* pSystem )
	: m_pFMODSystem(pSystem)
	{
		_ASSERT(pSystem != NULL);
	}
	RFMODSoundMgr::~RFMODSoundMgr( void )
	{
		m_pFMODSystem = NULL;

		while(!empty())
		{
			iterator itor = begin();
			RFMODSound* pSound = (*itor).second;
			SAFE_DELETE(pSound);
			erase(itor);
		}

		clear();
	}

	void RFMODSoundMgr::Insert( string szKey, RFMODSound* pFMODSound )
	{
		insert(value_type(szKey, pFMODSound));
	}

	// �������Ͽ� ���� ����
	// TODO : �÷��׵� �ʿ�
	RFMODSound* RFMODSoundMgr::GetFMODSound(string szKey, FMOD_MODE mode, float fDistMin, float fDistMax)
	{
		if (m_pFMODSystem == NULL) return NULL;

		RFMODSound* pFMODSound = NULL;

		string filename = MGetRemovePathName(szKey);

		iterator itor = find(string(filename));
		if (itor != end())
		{
			pFMODSound = (*itor).second;
			if( pFMODSound != NULL)
			{
				FMOD::Sound* pSound = pFMODSound->GetSound();
				if (pSound == NULL)
				{
					if(pFMODSound->CreateSound(m_pFMODSystem, szKey, mode) == false)
					{
						//mlog("����(%s) ������ �����߽��ϴ�.\n", filename.c_str());
						//SAFE_DELETE(pFMODSound);
						//erase(itor);
						return NULL;
					}
					pFMODSound->Set3DMinMaxDistance(fDistMin, fDistMax);
				}
			}
		}
		else
		{
			pFMODSound = new RFMODSound;
			if(pFMODSound->CreateSound(m_pFMODSystem, szKey, mode) == false)
			{
				mlog_filter("madduck","����(%s) ������ �����߽��ϴ�.\n", filename.c_str());
				//SAFE_DELETE(pFMODSound);
				//return NULL;
			}

			pFMODSound->Set3DMinMaxDistance(fDistMin, fDistMax);

			Insert(filename, pFMODSound);
		}

		_ASSERT(pFMODSound != NULL);
		if (pFMODSound == NULL) return NULL;

		pFMODSound->refAdd();
		pFMODSound->SetAccessTime(timeGetTime());

		mlog_filter("madduck", "%s : ref count : %d, accesstime = %d \n"
			, filename.c_str(), pFMODSound->GetrefCount(), pFMODSound->GetAccessTime());

		return pFMODSound;
	}

	// TODO : ������ �� �÷��װ� �ʿ��Ѱ�?
	RFMODSound* RFMODSoundMgr::GetFMODMusic(string szKey, FMOD_MODE mode)
	{
		if (m_pFMODSystem == NULL) return NULL;

		RFMODSound* pFMODSound = NULL;

		string filename = MGetRemovePathName(szKey);

		iterator itor = find(string(filename));
		if (itor != end())
		{
			pFMODSound = (*itor).second;
			if( pFMODSound != NULL)
			{
				FMOD::Sound* pSound = pFMODSound->GetSound();
				if (pSound == NULL)
				{
					if(pFMODSound->CreateMusic(m_pFMODSystem, szKey, mode) == false)
					{
						//mlog("����(%s) ������ �����߽��ϴ�.\n", filename.c_str());
						//SAFE_DELETE(pFMODSound);
						//erase(itor);

						return NULL;
					}
				}
			}
		}
		else
		{
			pFMODSound = new RFMODSound;
			if(pFMODSound->CreateMusic(m_pFMODSystem, szKey, mode) == false)
			{
				mlog_filter("madduck", "����(%s) ������ �����߽��ϴ�.\n", filename.c_str());
				//SAFE_DELETE(pFMODSound);
				//return NULL;
			}

			Insert(filename, pFMODSound);
		}

		_ASSERT(pFMODSound != NULL);
		if (pFMODSound == NULL) return NULL;

		pFMODSound->refAdd();

		// ������ ��Ʈ�����̶� ���۷����� �׻� 1��
		//�ι��ҷ����� �̻��ϰ���...
		//_ASSERT(pFMODSound->GetrefCount() == 1);
		
		pFMODSound->SetAccessTime(timeGetTime());

		mlog_filter("madduck", "%s : ref count : %d, accesstime = %d \n"
			, filename.c_str(), pFMODSound->GetrefCount(), pFMODSound->GetAccessTime());

		return pFMODSound;
	}

	void RFMODSoundMgr::UpdateReleaseSound()
	{
		for (iterator itor = begin(); itor != end(); ++itor)
		{
			RFMODSound* pFMODSound = (RFMODSound*)(*itor).second;
			if (pFMODSound->GetSound() == NULL ) continue;
			if (pFMODSound->GetrefCount() > 0) continue;

			DWORD time = timeGetTime();
			if ( (time - pFMODSound->GetAccessTime()) > ACCESSTIMELIMIT )
			{
				// TODO : �÷������� ä���� ���� �� �ִ�...
				pFMODSound->ReleaseSound();
			}
		}
	}

	int RFMODSoundMgr::GetLoadedFMODSoundCount()
	{
		size_t size = this->size();

		int count = 0;
		for (iterator itor = begin(); itor != end(); ++itor)
		{
			RFMODSound* pFMODSound = (*itor).second;
			if( pFMODSound != NULL)
			{
				FMOD::Sound* pSound = pFMODSound->GetSound();
				if (pSound != NULL)
				{
					++count;
				}
			}
		}

		return count;
	}

	RFMODSound* RFMODSoundMgr::GetFMODSoundOldAccessTime()
	{
		size_t size = this->size();

		RFMODSound* returnsound = NULL;
		DWORD  time = timeGetTime();

		int count = 0;
		for (iterator itor = begin(); itor != end(); ++itor)
		{
			RFMODSound* pFMODSound = (*itor).second;
			if( pFMODSound != NULL && pFMODSound->GetrefCount() == 0)
			{
				if(pFMODSound->GetAccessTime() < time)
				{
					returnsound = pFMODSound;
					time = pFMODSound->GetAccessTime();
				}
			}
		}

		return returnsound;
	}
}
