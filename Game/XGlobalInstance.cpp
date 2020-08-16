#include "stdafx.h"
#include "XGlobalInstance.h"
#include "XItemManager.h"
#include "XNPCInfo.h"
#include "XFieldInfo.h"
#include "XQuestInfo.h"
#include "XTalentInfo.h"
#include "XRecipeInfoMgr.h"
//#include "CSMeshInfo.h"
#include "XMeshInfoLoader.h"
#include "XMotion.h"
#include "XBuffEffect.h"
#include "XNetClient.h"
#include "XHistory.h"
#include "XRealSound.h"
#include "XRealSoundLoader.h"
#include "XCameraManager.h"
#include "XUISystem.h"
#include "XSystem.h"
#include "XEffectManager.h"
#include "XResourceManager.h"
#include "XTextureMtInfo.h"
#include "XBandiCapturer.h"
#include "XPhysX.h"
#include "XGameEffectInfo.h"
#include "XTalentInfoMgr.h"
#include "XTalentEffectMgr.h"
#include "XColorTable.h"
#include "XEngine.h"
#include "XSwordTrailSamplingData.h"
#include "XPath.h"
#include "XFeedbackSystem.h"
#include "XNetwork.h"
#include "XFactionInfoMgr.h"
#include "XGameFrameUILoader.h"
#include "XConditionsInfoMgr.h"
#include "XMsgCommandFacade.h"
#include "XItem_DroperInfoMgr.h"
#include "XItem_QuestRewarderInfoMgr.h"
#include "XItem_RecipeProductInfoMgr.h"
#include "XChatChannelManager.h"
#include "XInnInfoMgr.h"
#include "XModelAnimationInfo.h"
#include "XDialogTextInfoMgr.h"
#include "XRecipeNPCInfoMgr.h"
#include "XAppErrorCollector.h"
#include "MPhysX.h"
#include "CSFormatString.h"


XGlobalInstance::XGlobalInstance()
{
	var = new GlobalVar();
	system = new XSystem();

	script = new XScript();
	sound = new XRealSound();
	ui = new XUISystem();
	engine = new XEngine();

	net = new XNetwork();
	net->Create();

	history = new XHistory();
	camera = new XCameraManager();
	emgr = new XEffectManager();
	res = new XResourceManager();

	bandi_capturer = new XBandiCapturer();

	// physx
	phx = new XPhysX();

	feedback = new XFeedbackSystem();

	msg = new XMsgCommandFacade();
}

XGlobalInstance::~XGlobalInstance()
{
	SAFE_DELETE(msg);
	SAFE_DELETE(feedback);
	SAFE_DELETE(phx);
	SAFE_DELETE(bandi_capturer);
	SAFE_DELETE(camera);
	SAFE_DELETE(history);
	SAFE_DELETE(net);
	SAFE_DELETE(ui);
	SAFE_DELETE(sound);
	SAFE_DELETE(emgr);
	SAFE_DELETE(res);

	SAFE_DELETE(script);
	SAFE_DELETE(system);
	SAFE_DELETE(var);
	SAFE_DELETE(engine);
}

bool XGlobalInstance::Init(HWND nHwnd)
{
	// ���� ����
	XPath::CreateDirectoryInMyDocuments( PATH_MYDOC_SAVE);
	XPath::CreateDirectoryInMyDocuments( PATH_MYDOC_VIDEO);
	XPath::CreateDirectoryInMyDocuments( PATH_MYDOC_REPLAY);
	XPath::CreateDirectoryInMyDocuments( PATH_MYDOC_SCREEN_SHOT);
	XPath::CreateDirectoryInMyDocuments( PATH_MYDOC_SHADER_CACHING);
	XPath::CreateDirectoryInMyDocuments( PATH_MYDOC_CUSTOM);
	XPath::CreateDirectoryInMyDocuments( PATH_MYDOC_CUSTOM_CROSSHAIR);


	system->Init();

	script->Init();
	if ( !script->Create() )
	{
		_ASSERTE(!"XGlobalInstance::Init() - ��ũ��Ʈ �ʱ�ȭ ����");
		SetError(L"��ũ��Ʈ �ʱ�ȭ�� �����߽��ϴ�.");

		return false;
	}

	// ���� �ý��� �ʱ�ȭ
 	if(!sound->Create( 100, 20))
 	{
 		_ASSERTE(!"XGlobalInstance::Init() - Sound �ʱ�ȭ ����");
		SetError(L"Sound �ʱ�ȭ�� �����߽��ϴ�.");
 	}

	// Load samples
	XRealSoundLoader _SoundLoader;
	_SoundLoader.LoadSamples( MLocale::ConvUTF16ToAnsi(FILENAME_SOUNDLIST).c_str());

 	sound->SetVolume( RSD_CHANNELGROUP_MASTER, XGETCFG_AUDIO_MASTERVOLUME);
	sound->SetVolume( RSD_CHANNELGROUP_GENERIC, XGETCFG_AUDIO_GENERICVOLUME);
	sound->SetVolume( RSD_CHANNELGROUP_BGM, XGETCFG_AUDIO_BGMVOLUME);
 	sound->SetVolume( RSD_CHANNELGROUP_EFFECT, XGETCFG_AUDIO_EFFECTVOLUME);

	// UI �ý��� �ʱ�ȭ
	if ( !ui->Create((rs3::RDeviceD3D*)(REngine::GetDevicePtr()),
		rs3::REngine::GetConfig().m_nWidth, 
		rs3::REngine::GetConfig().m_nHeight, 
		nHwnd, 
		script->GetLua()) )
	{
		_ASSERTE(!"XGlobalInstance::Init - UI System �ʱ�ȭ ����");
		SetError( L"UI System �ʱ�ȭ�� �����߽��ϴ�.");
	}


	// Load interface common resource and template
	XGameFrameUILoader ui_loader;
	ui_loader.Load(L"resource");
	ui_loader.Load(L"template");


	// history
	wstring strHistoryFileName = XPath::GetHistoryFileFullPath();
	history->Load(strHistoryFileName.c_str());


	if ( !camera->Create() )
	{
		SetError(L"ī�޶� �ʱ�ȭ�� �����߽��ϴ�.");
		return false;
	}

	// PhysX
	if ( !phx->Init(XCONST::USE_PHYSX_REMOTE_DEBUGGER))
	{
		wstring strPhysxLastError = phx->GetLastErrorMessage();
		int _errcode = phx->GetLastError();

		wstring strErrorMessage = CSFormatString::Format(L"PhysX ������ �߻��Ͽ����ϴ�.\n\n���� �ڵ�={0}\n���� �޽���={1}\n\nPhysX�� ���������� ��ġ�Ǿ� ���� �ʰų�, �׷��� ī�� ����̹��� �������Դϴ�.\n�ֽ� ������ PhysX�� �׷��� ī�� ����̹��� ��ġ(Ȥ�� �缳ġ)�غ��ñ� �ٶ��ϴ�.", FSParam(_errcode, strPhysxLastError));
		SetError(strErrorMessage.c_str());

		return false;
	}

	return true;
}

bool XGlobalInstance::InitForDummyClient(HWND nHwnd)
{
	system->Init();

	script->Init();
	if ( !script->Create() )
	{
		mlog("XGlobalInstance::Init() - ��ũ��Ʈ �ʱ�ȭ ����\n");
		return false;
	}

 	// ���� �ý��� �ʱ�ȭ
	if(!sound->Create( 100, 10, XGETCFG_AUDIO_SPEAKERMODE, 44100))
	{
		_ASSERTE(!"XGlobalInstance::Init() - Sound �ʱ�ȭ ����");
		mlog("XGlobalInstance::Init() - Sound �ʱ�ȭ ����\n");
	}

	// Load samples
	XRealSoundLoader _SoundLoader;
	_SoundLoader.LoadSamples( MLocale::ConvUTF16ToAnsi(FILENAME_SOUNDLIST).c_str());

	// UI �ý��� �ʱ�ȭ
	if ( !ui->Create((rs3::RDeviceD3D*)(REngine::GetDevicePtr()),
		rs3::REngine::GetConfig().m_nWidth, 
		rs3::REngine::GetConfig().m_nHeight, 
		nHwnd, 
		script->GetLua()) )
	{
		mlog("XGlobalInstance::Init - UI System �ʱ�ȭ ����\n");
	}

	// history
	wstring strHistoryFileName = XPath::GetHistoryFileFullPath();
	history->Load(strHistoryFileName.c_str());


	if ( !camera->Create() )
	{
		mlog("XGlobalInstance::Init - ī�޶� �ʱ�ȭ ����\n");
		return false;
	}

	return true;
}

void XGlobalInstance::Fini()
{
	camera->Destroy();

	if (XPath::CreateDirectoryInMyDocuments(PATH_MYDOC_SAVE))
	{
		wstring strHistoryFileName = XPath::GetHistoryFileFullPath();
		history->Save(strHistoryFileName.c_str());
	}

	ui->Destroy();
	//sound->Destroy();
	script->Destroy();

	emgr->Destroy();

	// PhysX
	phx->Destroy();

	// RealSound
	sound->Destroy();
}

void XGlobalInstance::SetError(const wchar_t* szMessage)
{
	XGetAppErrorCollector()->SetLastError( ERRORTYPE_SYSTEM, szMessage);
	mlog(szMessage);
	mlog("\n");
}

XGlobalInfoInstance::XGlobalInfoInstance()
{
	this->item = new XItemManager();
	this->npc = new XNPCInfoMgr();
	this->field = new XFieldInfoMgr();
	this->quest = new XQuestInfoMgr();
	this->talent = new XTalentInfoMgr();
	this->talent_effect = new XTalentEffectMgr(this->talent);
	this->recipe = new XRecipeInfoMgr();
	this->mesh_info = new CSMeshInfoMgr();
	this->motion = new XMotionMgr();
	this->buff = new XBuffInfoMgr();
	this->textureMt = new XTextureMtInfoMgr();
	this->effect_Info = new XGameEffectInfoMgr();
	this->colorTable = new XColorTable();
	this->swordtrail_sampling = new XSwordTrailSamplingData();
	this->faction = new XFactionInfoMgr();
	this->cond = new XConditionsInfoMgr();
	this->cond_item = new XItemConditionsInfoMgr();
	this->item_droper = new XItem_DroperInfoMgr();
	this->item_questrewarder = new XItem_QuestRewarderInfoMgr();
	this->item_recipeproduct = new XItem_RecipeProductInfoMgr();
	this->chat_channel = new XChatChannelManager();
	this->inn = new XInnInfoMgr();
	this->model_info = new XModelAnimationInfo();
	this->dialog_text = new XDialogTextInfoMgr();
	this->recipenpc = new XRecipeNPCInfoMgr();
	this->lang = new CLangInfo();
}

XGlobalInfoInstance::~XGlobalInfoInstance()
{
	SAFE_DELETE(this->lang);
	SAFE_DELETE(this->item);
	SAFE_DELETE(this->npc);
	SAFE_DELETE(this->field);
	SAFE_DELETE(this->quest);
	SAFE_DELETE(this->talent);
	SAFE_DELETE(this->talent_effect);
	SAFE_DELETE(this->recipe);
	SAFE_DELETE(this->mesh_info);
	SAFE_DELETE(this->motion);
	SAFE_DELETE(this->buff);
	SAFE_DELETE(this->textureMt);
	SAFE_DELETE(this->effect_Info);
	SAFE_DELETE(this->colorTable);
	SAFE_DELETE(this->swordtrail_sampling);
	SAFE_DELETE(this->faction);
	SAFE_DELETE(this->cond);
	SAFE_DELETE(this->cond_item);
	SAFE_DELETE(this->item_droper);
	SAFE_DELETE(this->item_questrewarder);
	SAFE_DELETE(this->item_recipeproduct);
	SAFE_DELETE(this->chat_channel);
	SAFE_DELETE(this->inn);
	SAFE_DELETE(this->model_info);
	SAFE_DELETE(this->dialog_text);
	SAFE_DELETE(this->recipenpc);
}

bool XGlobalInfoInstance::Init()
{
	// item
	if ( !item->LoadFromFile(FILENAME_ITEM) )
	{
		_ASSERTE(!"m_ItemMgr.LoadFromFile() - ������ ���̺� �ҷ����� ����");
		SetError(L"������ ������ �дµ� �����߽��ϴ�.");
		return false;
	}

	if ( !item->LoadFromFile_SubMtrl(FILENAME_ITEM_CHANGEMTRL) )
	{
		SetError(L"������ ���� ������ �дµ� �����߽��ϴ�.");
		return false;
	}
	if (false == item_droper->Load(FILENAME_ITEM_DROPER))
	{
		SetError(L"NPC ������ ������ �дµ� �����߽��ϴ�.");
		return false;
	}
	if (false == item_questrewarder->Load(FILENAME_ITEM_QUESTREWARDER))
	{
		SetError(L"����Ʈ ������ ������ �дµ� �����߽��ϴ�.");
		return false;
	}

	// colorTable
	colorTable->Load(FILENAME_COLOR_TABLE);	

	// �޽� ���� �ε�
	XMeshInfoLoader meshInfoLoader;
	meshInfoLoader.LoadMeshInfo(mesh_info, FILENAME_MESH_INFO);

	// npc
	npc->Load(FILENAME_NPC, FILENAME_NPC_CHANGEMTRL, mesh_info);	

	// Field
	field->Load(FILENAME_ZONELIST, FILENAME_FIELDLIST);

	// quest 
	quest->Load(FILENAME_QUEST);

	// Talent
	talent->Load(FILENAME_TALENT, FILENAME_TALENT_EXT, FILENAME_TALENTHITINFO);

	// Talent Effect
	talent_effect->Load(FILENAME_EFFECTINFO);

	// recipe
	recipe->Load(FILENAME_RECIPE);

	// item_recipeproduct
	item_recipeproduct->Load(item, recipe);

	// Motion
	motion->Init();

	// Buff
	buff->Load(FILENAME_BUFFINFO, FILENAME_BUFF_EFFECTINFO, FILENAME_ENCHANT_BUFF_EFFECTINFO);

	//���͸��� ����
	textureMt->Load(FILENAME_TEXTUREMATERIAL);

	// Ŀ���� �̺�Ʈ ����
	effect_Info->Load(FILENAME_EFFECT_INFO);

	// �˱� ���ø�
	swordtrail_sampling->Load(FILENAME_SWORD_TRAIL_SAMPLING);

	faction->Load(FILENAME_FACTION);

	// conditions
	if (cond->Load(FILENAME_CONDITIONS) == false)
	{
		mlog("Failed To Load File : %s\n", MLocale::ConvUTF16ToAnsi(FILENAME_CONDITIONS).c_str());
	}
	if (cond_item->Load(FILENAME_CONDITIONS_ITEM) == false)
	{
		mlog("Failed To Load File : %s\n", MLocale::ConvUTF16ToAnsi(FILENAME_CONDITIONS_ITEM).c_str());
	}

	dialog_text->Load(FILENAME_DIALOG);

	item->Cooking();

	recipenpc->Load(FILENAME_RECIPENPC);

	return true;
}

void XGlobalInfoInstance::SetError( wchar_t* szMessage )
{
	XGetAppErrorCollector()->SetLastError( ERRORTYPE_SYSTEM, szMessage);
	mlog(szMessage);
	mlog("\n");
}
