/*=======================================================================
��������    :2013 /04 /10
�ļ���      : LockMap.hpp
����        : zcckm
�汾        : 1.0.0.0
�ļ�ʵ�ֹ��� :
���ܽ���:���߳������ϣ����������߳������������ʹ��
ǰ��: ��Ҫ�ȵ��� LockMap::getInstance()->addNewLockInMap(GENSTR(TESTLOCK1)); ��������
1.һ������������ģ��������ǰʵ���������ڽ������Զ�����
	OneStepLock<LockMap> lockNow(LockMap::getInstance(),GENSTR(TESTLOCK1)); 
2.�ֶ�����,�ȼ�����������ɺ��ٽ���
	LockMap::getInstance()->Lock(GENSTR(TESTLOCK1));
	LockMap::getInstance()->UnLock(GENSTR(TESTLOCK1));
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
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