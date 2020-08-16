/// @file WS_Zone.h
#pragma once

#include "WS_WorkSpaceManager.h"

class CWorkSpaceZone : public CWorkSpaceImpl<CWorkSpaceZone>
{
public:
								CWorkSpaceZone(){}
	virtual						~CWorkSpaceZone(){}

	virtual const char*			GetWorkSpaceKeyword();												///< WorkSpace Keyword
	virtual void				InitWorkSpace(CWorkSpaceUiSetter& _rUiSetter, CWorkSpaceHelpersAndActions& _rHelperAndAction);	///< WorkSpace UI
	virtual void				HaltWorkSpace(CWorkSpaceHelpersAndActions& _rHelperAndAction);

	virtual const char*			GetFileExtension()	{ return "*.zone.xml"; }
	virtual const char*			GetFileExtensionDesc()	{ return "�� ����"; }

	void						LoadZoneCommonXmlAndNavigationMesh(const char* _pSzZoneName);

protected:

	virtual void				OnRollupExpandClicked(CListenableView* pRollup);		///< When Rollup Expand Clicked
	virtual void				OnWBObjectEditMouseMessage(VIEW_MESSAGE::ID message);	///< Workbench Mouse Edit Message

	virtual MAP_EDIT_TYPE		OnOpenFile(const char* _pSzFileName);					///< ���� ����
	virtual std::string			OnSaveFile(const char* _pSzFileName, bool bSaveAs);		///< ���� ����
	virtual void				OnCloseFile() {}
	virtual void				OnSceneSaved( const char* _filename );					///< ���� ����ɶ�


	virtual void				OnBetweenCoreToolUpdateAndRS3Render(){}
	virtual void				OnResetResourceAfterSceneCleared(bool bOnDestroy){}

	virtual void				OnWBDevAction(){}
	virtual void				OnWBHelperRender(){}

	virtual void				OnFileModified( const string& strFileName ) override;		///< ������ ����Ǿ����� ó��

	void SaveRestoreDirectionalLight(rs3::RDirectionalLightSceneNode* pLight, bool bSave);
};
