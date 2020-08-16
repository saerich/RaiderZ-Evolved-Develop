/// @file C_XMLConfig.h
#pragma once

#include <string>

/**
	@brief �� ȯ�漳�� XML ���� Ŭ����

	XML ������ ������ �о� ȯ�漳���� �ϴ� �̱��� Ŭ���� �Դϴ�.
*/
class CORETOOL_API CXMLConfig
{
protected:
	CXMLConfig();
	~CXMLConfig();
public:
	// �⺻������ DEFAULT_PATH_DATA ������ �����ȴ�.
	// ������ �о����� pcDataPath�κ��� �ڵ� ����.
	char pcDataPath[MAX_PATH];
	std::string strDataFullPath;
	
	//Ʈ��
	char pcTreepath[MAX_PATH];

	//elu
	char pcEluPath[MAX_PATH];			///< model path
	char pcMapObjectPath[MAX_PATH];		///< ��ġ�� map object path
	char pcSkyPath[MAX_PATH];
	
	char pcEftpath[MAX_PATH];
	char pcShaderpath[MAX_PATH];
	char pcEffectpath[MAX_PATH];

	// ����
	char pcSoundPath[MAX_PATH];
	char pcBGMPath[MAX_PATH];
	char pcEffectSoundPath[MAX_PATH];

	//�ؽ���
	char pcTilepath[MAX_PATH];
	std::string	strTexturePath;

	//animation
	std::string strCameraAniPath;

	//field data
	std::string strServerPath;
	std::string strServerScriptPath;
	std::string strServerScriptFullPath;
	std::string strServerFieldScriptFullPath;
	std::string strServerNPCScriptFullPath;
	std::string strServerFieldPath;
	std::string strServerFieldFullPath;
	std::string strServerZonePath;
	std::string strServerZoneFullPath;

	float fMaxFarZ;
	float fMinFarZ;
	float fWheelZoomFactor;
	float fKeyZoomFactor;
	float fVelocity;

	float fRotateStep;
	float fPanStep;

	bool LoadXML(const char *filename);
	void SaveXML(const char *filename);

	static CXMLConfig* GetInstance()
	{
		static CXMLConfig kConfig;
		return &kConfig;
	};

private:
	void SetPathesFromDataPath_();
};
