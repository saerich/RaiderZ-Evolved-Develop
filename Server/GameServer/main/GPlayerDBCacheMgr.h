#ifndef _GPLAYER_CACHE_DB_MGR_H
#define _GPLAYER_CACHE_DB_MGR_H


#include <unordered_map>


class GPlayerDBCache;


typedef std::unordered_map<int64, GPlayerDBCache*> GPLAYER_DBCACHE_HMAP;


class GPlayerDBCacheMgr : protected std::unordered_map<int64, GPlayerDBCache*>, public MTestMemPool<GPlayerDBCacheMgr>
{
public :
	GPlayerDBCacheMgr() {}
	~GPlayerDBCacheMgr() {}

	bool			Insert(GPlayerDBCache* pPlayer);
	GPlayerDBCache* Find(const MUID& uidPlayer);
	void			Erase(const MUID& uidPlayer);

	using std::unordered_map<int64, GPlayerDBCache*>::begin;
	using std::unordered_map<int64, GPlayerDBCache*>::end;
	using std::unordered_map<int64, GPlayerDBCache*>::empty;
	using std::unordered_map<int64, GPlayerDBCache*>::clear;
	using std::unordered_map<int64, GPlayerDBCache*>::size;
};


#endif