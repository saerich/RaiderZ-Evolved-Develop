#ifndef _TDIALOG_INFO_H
#define _TDIALOG_INFO_H

enum DIALOG_CHECK_TYPE
{
	DCT_QUEST	= 0,	// ����Ʈ
	DCT_FACTION	= 1,	// �Ѽ�
};

enum DIALOG_NODE_TYPE
{
	DNT_CHECK			= 0,
	DNT_CASE_CHECKFAIL	= 1,
	DNT_CASE_NOEXIST	= 2,
	DNT_CASE_DOING		= 3,
	DNT_CASE_FINAL		= 4,
	DNT_CASE_COMPLETED	= 5,
	DNT_CASE_DEFAULT	= 6,
	DNT_SAY				= 7,
	DNT_SELECT			= 8,
	DNT_INFO			= 9
};

#define DIALOG_XML_TAG_MAIET					"maiet"

#define DIALOG_XML_TAG_DIALOG					"DIALOG"
#define DIALOG_XML_TAG_CHECK					"CHECK"

#define DIALOG_XML_TAG_CASE_CHECKFAIL			"CHECKFAIL"
#define DIALOG_XML_TAG_CASE_NOEXIST				"NOEXIST"
#define DIALOG_XML_TAG_CASE_DOING				"DOING"
#define DIALOG_XML_TAG_CASE_FINAL				"FINAL"
#define DIALOG_XML_TAG_CASE_COMPLETED			"COMPLETED"
#define DIALOG_XML_TAG_CASE_DEFAULT				"DEFAULT"

#define DIALOG_XML_TAG_SAY						"SAY"
#define DIALOG_XML_TAG_SELECT					"SELECT"

#define DIALOG_XML_ATTR_ID						"ID"
#define DIALOG_XML_ATTR_TYPE					"TYPE"
#define DIALOG_XML_ATTR_PARAM1					"PARAM1"
#define DIALOG_XML_ATTR_PARAM2					"PARAM2"
#define DIALOG_XML_ATTR_TEXT					"TEXT"
#define DIALOG_XML_ATTR_EXIT					"EXIT"

#define DIALOG_XML_VALUE_TYPE_QUEST				"QUEST"
#define DIALOG_XML_VALUE_TYPE_FACTION			"FACTION"


struct EXTRACTED_DIALOG_INFO;

/// ���̾�α� Ʈ���� �����ϴ� �⺻ ���
class TDialogNode
{
public:
	TDialogNode();
	virtual ~TDialogNode();

	TDialogNode(const TDialogNode& rDialogNode);
	virtual TDialogNode* Clone(TDialogNode* pParentDialogNode) = 0;

	void Clear();

	virtual void SaveToXml(MXmlElement* pElementMaietDialogInfo, MXmlElement* pElementMaietStrings, const int nNPCID, const int nDialogID, int& nCntSay, int& nCntSelect) = 0;
	virtual void GetInvalidDialogNode(vector<TDialogNode*>& outVecDialogNode) = 0;

	int m_nType;
	TDialogNode* m_pParentNode;
	vector<TDialogNode*> m_vecpChildNode;
};

/// ���̾�α� xml�� <DIALOG> ������ ����ִ� ��� �̸鼭, �ֻ��� ���
class TDialogInfo : public TDialogNode
{
public:
	TDialogInfo(void);
	~TDialogInfo(void);

	TDialogInfo(const TDialogInfo& rDialogInfo);
	virtual TDialogInfo* Clone(TDialogNode* pParentDialogNode);

	virtual void SaveToXml(MXmlElement* pElementMaietDialogInfo, MXmlElement* pElementMaietStrings, const int nNPCID, const int nDialogID, int& nCntSay, int& nCntSelect);
	virtual void GetInvalidDialogNode(vector<TDialogNode*>& outVecDialogNode);

	int m_nID;
	vector<int> m_vecChildSelectNodeExit;				///< �ڽ� ����Ʈ ������  exit���� ��Ƴ��ٰ� ���ο�  exit�� ���ö� ���.
};

/// ���̾�α� xml�� <CHECK> ������ ����ִ� ���
class TDialogCheckNode : public TDialogNode
{
public:
	TDialogCheckNode();
	~TDialogCheckNode();

	TDialogCheckNode(const TDialogCheckNode& rDialogCheckNode);
	virtual TDialogCheckNode* Clone(TDialogNode* pParentDialogNode);

	virtual void SaveToXml(MXmlElement* pElementMaietDialogInfo, MXmlElement* pElementMaietStrings, const int nNPCID, const int nDialogID, int& nCntSay, int& nCntSelect);
	virtual void GetInvalidDialogNode(vector<TDialogNode*>& outVecDialogNode);

	int m_nCheckType;
	int m_nParam1;
	int m_nParam2;
};

/// ���̾�α� xml�� <CHECKFAIL>, <NOEXIST>, <DOING>, <FINAL>, <COMPLETED> ������ ����ִ� ���
class TDialogCaseNode : public TDialogNode
{
public:
	TDialogCaseNode();
	~TDialogCaseNode();

	TDialogCaseNode(const TDialogCaseNode& rDialogCaseNode);
	virtual TDialogCaseNode* Clone(TDialogNode* pParentDialogNode);

	virtual void SaveToXml(MXmlElement* pElementMaietDialogInfo, MXmlElement* pElementMaietStrings, const int nNPCID, const int nDialogID, int& nCntSay, int& nCntSelect);
	virtual void GetInvalidDialogNode(vector<TDialogNode*>& outVecDialogNode);
};

/// ���̾�α� xml�� <SAY> ������ ����ִ� ���
class TDialogSayNode : public TDialogNode
{
public:
	TDialogSayNode();
	~TDialogSayNode();

	TDialogSayNode(const TDialogSayNode& rDialogSayNode);
	virtual TDialogSayNode* Clone(TDialogNode* pParentDialogNode);

	virtual void SaveToXml(MXmlElement* pElementMaietDialogInfo, MXmlElement* pElementMaietStrings, const int nNPCID, const int nDialogID, int& nCntSay, int& nCntSelect);
	virtual void GetInvalidDialogNode(vector<TDialogNode*>& outVecDialogNode);

	string m_strText;
};

/// ���̾�α� xml�� <SELCT> ������ ����ִ����� ���
class TDialogSelectNode : public TDialogNode
{
public:
	TDialogSelectNode();
	~TDialogSelectNode();

	TDialogSelectNode(const TDialogSelectNode& rDialogSelectNode);
	virtual TDialogSelectNode* Clone(TDialogNode* pParentDialogNode);

	virtual void SaveToXml(MXmlElement* pElementMaietDialogInfo, MXmlElement* pElementMaietStrings, const int nNPCID, const int nDialogID, int& nCntSay, int& nCntSelect);
	virtual void GetInvalidDialogNode(vector<TDialogNode*>& outVecDialogNode);

	string m_strText;
	int m_nExit;
};

/// ���̾�α� Ʈ������ ���� �÷��̾��� ���¿� ���� ����� ���̾�α� ����
struct EXTRACTED_DIALOG_INFO
{
	int nDialogID;										///< ���̾�α� ID
	MUID uidDialogNPC;									///< NPC ID

	TDialogSayNode* pDialogSayNode;						///< NPC�� ���ϴ� ������ ��� �ִ� ���
	vector<TDialogSelectNode*> vecpDiaolgSelectNode;	///< �÷��̾ �����Ҽ� �ִ� �������� ������ ��� �ִ� ����
	void Init()
	{
		uidDialogNPC.Invalid();
		nDialogID = 0;
		pDialogSayNode = nullptr;
		vecpDiaolgSelectNode.clear();
		pSelectedDialogSelectNode = nullptr;
	}

	TDialogSelectNode* pSelectedDialogSelectNode;		///< �÷��̾ ������ �������� ������ ����ִ� ���
};



#endif // _TDIALOG_INFO_H