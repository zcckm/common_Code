/*=======================================================================
创建日期    :2013 /04 /10
文件名      : LockMap.hpp
作者        : zcckm
版本        : 1.0.0.0
文件实现功能 :
功能介绍:多线程锁集合，管理多个多线程锁与加锁解锁使用
前提: 需要先调用 LockMap::getInstance()->addNewLockInMap(GENSTR(TESTLOCK1)); 增加新锁
1.一步锁定：创建模板锁，当前实例生命周期结束，自动解锁
	OneStepLock<LockMap> lockNow(LockMap::getInstance(),GENSTR(TESTLOCK1)); 
2.手动锁定,先加锁，工作完成后再解锁
	LockMap::getInstance()->Lock(GENSTR(TESTLOCK1));
	LockMap::getInstance()->UnLock(GENSTR(TESTLOCK1));
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
=======================================================================*/

#ifndef _LOCK_MAP_H_
#define _LOCK_MAP_H_
#pragma warning(disable : 4786)
#include <map>
#include <string>
#include <assert.h>
#define GENSTR(str) #str
using namespace std;
class LockMap
{
private:
	LockMap()
	{

	}
public:
	~LockMap()
	{
		LOCKMAP::iterator iter = m_mapLockMap.begin();
		LOCKMAP::iterator iterEnd = m_mapLockMap.end();
		for(; iter != iterEnd; ++iter)
		{
			CloseHandle(iter->second);
		}
	}
	static LockMap * getInstance()
	{
		static LockMap dtLock;
		return &dtLock;
	}
	BOOL addNewLockInMap(const char * pLockName)
	{
		if (findLock(pLockName) != NULL)
		{
			return FALSE;
		}
		string strTmpName(pLockName);
		HANDLE hTmpHd = CreateMutex(NULL,FALSE,pLockName);
		m_mapLockMap.insert(make_pair(strTmpName,hTmpHd));
		printf("LockMap:: new lock %s\n",pLockName);
		return TRUE;
	}
	BOOL delLockInMap(char * pLockName)
	{
		HANDLE hTmpLock;
		if ((hTmpLock = findLock(pLockName)) != NULL)
		{
			CloseHandle(hTmpLock);
			m_mapLockMap.erase(pLockName);
			return TRUE;
		}
		return FALSE;
	}
	void Lock(const char * strLockName)
	{
		HANDLE tmplock = findLock(strLockName);
		if (NULL == tmplock)
		{
			printf("lock %s find and lock failed\n",strLockName);
			return;
		}
		WaitForSingleObject(tmplock,INFINITE);
	}
	void UnLock(const char * strLockName)
	{
		HANDLE tmplock = findLock(strLockName);
		if (NULL ==tmplock)
		{
			return;
		}
		ReleaseMutex(tmplock);
	}
	HANDLE findLock(const char * strLockName)
	{
		LOCKMAP::iterator iter = m_mapLockMap.begin();
		LOCKMAP::iterator iterEnd = m_mapLockMap.end();
		for(; iter != iterEnd; ++iter)
		{
			if(0 == iter->first.compare(strLockName))
			{
				return iter->second;
			}
		}
		return NULL;
	}
private:
	typedef map<string,HANDLE> LOCKMAP;
	LOCKMAP m_mapLockMap;
};


template <class theLock> class OneStepLock
{
public:
	OneStepLock(theLock * pLock,char * pLockName)
	{
		assert(pLock);
		m_pLock = pLock;
		pLock->Lock(pLockName);
		string strtmp(pLockName);
		strLockname = strtmp;
	}
	~OneStepLock()
	{
		m_pLock->UnLock(strLockname.c_str());
	}
private:
	theLock * m_pLock;
	string strLockname;
};

#endif