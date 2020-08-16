/// @file WS_Scene.h
#pragma once

#include "WS_WorkSpaceManager.h"

class CWorkSpaceScene : public CWorkSpaceImpl<CWorkSpaceScene>
{
public:
	CWorkSpaceScene(){}
	virtual						~CWorkSpaceScene(){}

	virtual const char*			GetWorkSpaceKeyword();												///< WorkSpace Keyword
	virtual void				InitWorkSpace(CWorkSpaceUiSetter& _rUiSetter, CWorkSpaceHelpersAndActions& _rHelperAndAction);	///< WorkSpace UI
	virtual void				HaltWorkSpace(CWorkSpaceHelpersAndActions& _rHelperAndAction);

	virtual const char*			GetFileExtension()	{ return "*.scene.xml"; }
	virtual const char*			GetFileExtensionDesc()	{ return "�� ����"; }

	virtual void				OnFileModified( const string& strFileName ) override;		///< ������ ����Ǿ����� ó��

protected:

	virtual void				OnWBObjectEditMouseMessage(VIEW_MESSAGE::ID message);	///< Workbench Mouse Edit Message

	virtual MAP_EDIT_TYPE		OnOpenFile(const char* _pSzFileName);					///< ���� ����
	virtual std::string			OnSaveFile(const char* _pSzFileName, bool bSaveAs);		///< ���� ����
	virtual void				OnCloseFile() {}

	virtual void				OnBetweenCoreToolUpdateAndRS3Render(){}
	virtual void				OnResetResourceAfterSceneCleared(bool bOnDestroy){}

	virtual void				OnWBDevAction(){}
	virtual void				OnWBHelperRender(){}

private:
	void						ReloadEffect( const string& strFileName );
	void						ReloadSceneInstance( const string& strFileName );
};
