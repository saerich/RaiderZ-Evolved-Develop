#ifndef _TDBMANAGER_H
#define _TDBMANAGER_H


class TDatabase;

using namespace System;

/// DB ������
class TDBManager 
{
private:
	TDatabase*					m_pDB;
	std::string					m_strDSNConnect;

	vector<int>					m_vecQIDOfGetQuestFromDB;					///<DB�� ���� Quest ���̺��� �ҷ��ö��� QuestID
	vector<pair<int, int>>		m_vecQIDANDOIDOfGetOObjectiveFromDB;		///<DB�� ���� Objective ���̺��� �ҷ��ö� ������ QuestID�� ObjectiveID
	vector<pair<int, int>>		m_vecQIDANDOIDOfGetUObjectiveFromDB;		///<DB�� ���� Objective ���̺��� �ҷ��ö� ������� QuestID�� ObjectiveID
	vector<pair<int, int>>		m_vecQIDANDFIDOfGetQuestPRFactionFromDB;	///<DB�� ���� QeustRequiredFaction ���̺��� �ҷ��ö��� QuestID, FactionID

	bool GetQuestsFromDB(int nQuestID = -1);
	bool GetQuestBaseInfoFromDB(int nQuestID = -1);
	bool GetQuestPRFromDB(int nQuestID = -1);
	bool GetQuestOObjectiveFromDB(int nQuestID = -1);
	bool GetQuestUObjectiveFromDB(int nQuestID = -1);
	bool GetQuestPRFactionFromDB(int nQuestID = -1, int nFactionID = -1);

	void SetQuestToDB();
	bool InsertQuestToDB(int nQuestID);
	bool UpdateQuestToDB(int nQuestID);
	bool DeleteQuestToDB(int nQuestID = -1);

	void SetQuestOObjectiveToDB();
	void SetQuestUObjectiveToDB();
	bool InsertQuestObjectiveToDB(int nQuestID, int nObjectiveID);
	bool UpdateQuestObjectiveToDB(int nQuestID, int nObjectiveID);
	bool DeleteQuestObjectiveToDB(int nQuestID = -1, int nObjectiveID = -1);

	void SetQuestPRFactionToDB();
	bool InsertQuestPRFactionToDB(int nQuestID, int nFactionID);
	bool UpdateQuestPRFactionToDB(int nQuestID, int nFactionID);
	bool DeleteQuestPRFactionToDB(int nQuestID = -1, int nFactionID = -1);

	void SaveStateOfGetQuestsFromDB();
	void SaveStateOfGetQuestFromDB();								///< DB�� ���� Quest ���̺��� �ҷ��ö��� ���¸� �����ߴٰ�, DB�� �����Ҷ� �����Ѵ�.
	void SaveStateOfGetQuestObejctiveFromDB();						///< DB�� ���� Objective ���̺��� �ҷ��ö��� ���¸� �����ߴٰ�, DB�� �����Ҷ� �����Ѵ�.
	void SaveStateOfGetQuestPRFactionFromDB();						///< DB�� ���� QeustRequiredFaction ���̺��� �ҷ��ö��� ���¸� �����ߴٰ�, DB�� �����Ҷ� �����Ѵ�.
	void ClearStateOfGetQuests();

	void DBUOIDtoRealUOID();										///< DB���� �ҷ��ö� +100�Ǿ� �ִ� ������� Objective�� -100 ���ش�.
	void RealUOIDtoDBUOID();										///< DB�� �����Ҷ� ������� Objective�� +100 ���ش�.

	String^ ConvertToDBDateTimeFormat(String^ strDate);				///< ����, ���ĸ� 1~12, 1~24�÷� �ٲ۴�.


	
public:
	TDBManager(TDatabase* pDB);
	~TDBManager();
	bool Begin();
	void End();

	static void LogCallback( const string& strLog );

	bool Execute(String^ strSQL);
	bool Execute(String^ strSQL, DataSet^ dstDs, String^ strTable);

	void SetQuestsToDB();

	DataTable^ GetQuestBaseInfo();
	DataTable^ GetQuestPR();
	DataTable^ GetQuestPRFaction();
	DataTable^ GetQuestOObjective();
	DataTable^ GetQuestUObjective();


	DataTable^ CopyQuestBaseInfo(int nQuestID);
	void InsertQuestBaseInfo(int nQuestID);
	void UpdateQuestBaseInfo(int nQuestID, String^ strColumnName, Object^ objValue);
	void DeleteQuestBaseInfo(int nQuestID);
	
	DataTable^ CopyQuestPR(int nQuestID);
	void InsertQuestPR(int nQuestID, int nPRQuestID);
	void UpdateQuestPR(int nQuestID, String^ strColumnName, Object^ objValue);
	void DeleteQuestPR(int nQuestID);

	DataTable^ CopyQuestPRFaction(int nQuestID);
	void InsertQuestPRFaction(int nQRFID, int nQuestID);
	void UpdateQuestPRFaction(int nQRFID, String^ strColumnName, Object^ objValue);
	void DeleteQuestPRFaction(int nQRFID, int nQuestID);
	void DeleteQuestPRFaction(int nQuestID);

	DataTable^ CopyQuestOObjective(int nQuestID);
	void InsertQuestOObjective(int nQuestID, int nObjectiveID);
	void UpdateQuestOObjective(int nQuestID, int nObjectiveID, String^ strColumnName, Object^ objValue);
	void DeleteQuestOObjective(int nQuestID, int nObjectiveID);
	void DeleteQuestOObjective(int nQuestID);

	DataTable^ CopyQuestUObjective(int nQuestID);
	void InsertQuestUObjective(int nQuestID, int nObjectiveID);
	void UpdateQuestUObjective(int nQuestID, int nObjectiveID, String^ strColumnName, Object^ objValue);
	void DeleteQuestUObjective(int nQuestID, int nObjectiveID);
	void DeleteQuestUObjective(int nQuestID);

	void InsertQuests(int nQuestID, int nPRQuestID);
	void DeleteQuests(int nQuestID);

	String^ GetStringsKeyFromQuestBaseInfo(int nQuestID, String^ strColumn);
	String^ GetStringsKeyFromQuestOObjective(int nQuestID, int nObjectiveID, String^ strColumn);
	String^ GetStringsKeyFromQuestUObjective(int nQuestID, int nObjectiveID, String^ strColumn);

	bool HaveQuest(int nQuestID);
	bool HaveQuestObjective(int nQuestID, int nObjectiveID);

public:
};


#endif //_TDBMANAGER_H
