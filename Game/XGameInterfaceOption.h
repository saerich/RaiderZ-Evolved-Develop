

#pragma once

#ifndef _XGAME_INTERFACE_OPTION
#define _XGAME_INTERFACE_OPTION

#include "XVirtualKeyDef.h"


/// class XGameInterfaceOption
class XGameInterfaceOption
{
// Member variables
private:
	bool		m_bModifyOption;							///< �ɼ� ����������...



// Member functions
public:
	XGameInterfaceOption();
	virtual ~XGameInterfaceOption();

	void BeginModifyOption()				{ m_bModifyOption = true;		}
	void EndModifyOption()					{ m_bModifyOption = false;		}

	bool LoadOption();										///< �ɼ��� �о����
	void SaveOption();										///< ���� �ɼ� ������ ������

	bool ValidateOption();									///< ���� �ɼ��� ��ȿ�� �˻�

	void UpdateSystemOption();								///< ���� �ý��� �ɼ��� �缳���Ѵ�

	bool GetDisplayResInfo( vector< MSIZE>& info);			///< �ػ� ����� ���Ѵ�


	// ����
	void SetScreenResolution( int w, int h);
	void GetScreenResolution( int* w, int* h);
	void SetLighting( int nLighting);
	void SetTextureMipmapBias( float fBias);
	void SetTextureReduction( int nTextureReduction);
	void SetGrndObjVisibleRange( int nRange);
	void EnableNormalMapping( bool bEnable);
	void SetFullScreen( bool bSet);
	void EnableHDR( bool bEnable);
	void EnableDOF( bool bEnable);
	void SetSSAO( int nSet);
	void SetShadow( int nShadow, bool bSave =true);
	void SetDistortion( bool bSet);
	void SetMotionBlur( bool bSet);
	void SetSoftParticle( bool bSet);
	void MuteAll( bool bMute);
	void MuteGenericSound( bool bMute);
	void MuteBgmSound( bool bMute);
	void MuteEffectSound( bool bMute);
	void SetMasterVolume( float fVolume);
	void SetGenericVolume( float fVolume);
	void SetBgmVolume( float fVolume);
	void SetEffectVolume( float fVolume);
	void EnableHardwareAccelerate( bool bEnable);
	void EnableReverb( bool bEnable);
	void EnableCompressor( bool bEnable);
	void SetSpeakerMode( int nSpeakerMode);
	void MuteInActiveSound( bool bMute);
	void SetSystemMessage(int nMode);

	void SetDefaultSetting();								///< �⺻ ��������...
	void SetDefaultControlKey();							///< �⺻ Ű ��������...
	void ApplyControlKeyMap();								///< Ű ������ ������

	void ChangeControlKey( const char* szName, int nCode, bool bShift, bool bCtrl, bool bAlt);
	void ChangeControlKey( const char* szName, int nCode);

protected:
	void SetControlKeyConfig( const string& strName, const string& strCode, const string& strExt);
};


#endif
