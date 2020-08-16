//
//		Global Resource Manager
//
//		MBitmapManager, MFontManager, MCursorSystem
//		MINT���� ���Ǵ� Bitmap, Font, Cursor Resource�� ���������� �����ϴ� Ŭ������
//		Application���� ���������� ���ҽ��� �����Ѵٸ� ���� �� Ŭ�������� �� �ʿ�� ����.
//
#pragma once

#include "MBitmap.h"
#include "MFont.h"
#include "MCursor.h"
#include "CMList.h"

#include <list>
#include <map>
#include <string>
using namespace std;

namespace mint3 {

class MAnimationTemplate;

// Global Bitmap Management static class
class MBitmapManager{
private:
	static map<string, MBitmap*>		m_BitmapMap;
	static int							m_nNonameCounter;
public:
	static void Destroy(void);
//	static void DestroyAniBitmap(void);
	static void Add(MBitmap* pBitmap);
//	static void Add(MAniBitmap* pAniBitmap);
	static void Delete(const char* szName);
	static void Clear();
	static MBitmap* Get(const char* szName);
	static MBitmap* Get(int i);
	static int GetCount(void);
	static bool IsExist( const char* szName);
//	static MAniBitmap* GetAniBitmap(const char* szBitmapName);
	static void Update(float fDelta);
};

// Global Font Management static class
class MFontManager{
	static map<string,MFont*>	m_Fonts;
	static MFont *m_pDefaultFont;
public:
	MFontManager();

	static void Destroy(void);
	static void Add(MFont* pFont);
	static MFont* Get(const char* szFontName);
//	static MFont* Get(int i);
	static int GetCount(void);
	static void SetDefaultFont(MFont *pFont);
	static bool IsExist( const char* szName);
};

// Default Font Set
#define MF_SMALL		"small"
#define MF_MEDIUM		"medium"
#define MF_SMALL_BOLD	"smallbold"
#define MF_MEDIUM_BOLD	"mediumbold"

// �⺻������ ���Ǵ� Ŀ�� �̸�
// �ּ��� �� ������ �̸��� ���� Ŀ���� ���� �Ǿ����� �Ѵ�.
#define MCURSOR_ARROW	"arrow"
#define MCURSOR_WAIT	"wait"
#define MCURSOR_INPUT	"input"

// Cursor System static class
class MCursorSystem{
	static MCursor*					m_pCursor;		// ���� Ŀ��
	static map<string, MCursor*>	m_mapCursors;	// ���ο� �����Ǵ� Ŀ��
	static bool						m_bShow;
public:
	static void Destroy(void);
	static MCursor* Set(MCursor* pCursor);
	static MCursor* Set(const char* szName);
	static MCursor* Get(void);
	static MCursor* Get(const char* szName);
	static void Add(MCursor* pCursor);
	static void Draw(MDrawContext* pDC, int x, int y);
	static void Show(bool bShow);
	static bool IsVisible(void);
	static bool IsExist( const char* szName);
};

template <class _TYPE>
class MResourceManager {
	map<string, _TYPE*> m_mapResources;

public:
	void Destroy(void)
	{
		while(!m_mapResources.empty())
		{
			delete (*m_mapResources.begin()).second;
			m_mapResources.erase(m_mapResources.begin());
		}
		m_mapResources.clear();
	}

	bool Add( _TYPE* pResource)
	{
		if( Get( pResource->GetName() ) !=NULL )
			return false;

		m_mapResources.insert( map<string, _TYPE*>::value_type( pResource->GetName(), pResource));
		return true;
	}

	_TYPE* Get( const string& strName )
	{
		map<string, _TYPE*>::iterator itr = m_mapResources.find(strName);
		if(itr==m_mapResources.end())
			return NULL;

		return itr->second;
	}

	void Delete ( const string& strName )
	{
		map<string, _TYPE*>::iterator itr = m_mapResources.find(strName);
		if(itr==m_mapResources.end())
			return NULL;
		delete itr->second;
		m_mapResources.erase(itr);
	}
};

class MAnimationTemplateManager : public MResourceManager<MAnimationTemplate> {
};

} // namespace mint3