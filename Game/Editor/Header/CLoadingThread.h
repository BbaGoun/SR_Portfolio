#pragma once

#include "CBase.h"
#include "Engine_Define.h"

class CLoadingThread : public CBase
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

public:
	explicit CLoadingThread(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoadingThread();

public:
	template <int N>
	_tchar* Get_String(_tchar (&bufStr)[N]) {
		EnterCriticalSection(Get_Crt());
		lstrcpyn(bufStr, m_szLoading, N);
		LeaveCriticalSection(Get_Crt());
		return bufStr;
	}
	LOADINGID		Get_LoadingID() { return m_eLoadingID; }
	_bool			Get_Finish()	{
		EnterCriticalSection(Get_Crt());
		bool bFinish = m_bFinish;
		LeaveCriticalSection(Get_Crt());
		return bFinish; }

	CRITICAL_SECTION* Get_Crt()		{ return &m_Crt; }

public:
	HRESULT		Ready_Loading(LOADINGID eID);
	_uint		Loading_Stage();

public:
	static unsigned int CALLBACK Thread_Main(void* pArg);


private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_tchar				m_szLoading[128];		// 로딩 상태 문자열

	HANDLE				m_hThread;				// 쓰레드 핸들
	LOADINGID			m_eLoadingID;

	CRITICAL_SECTION	m_Crt;					// 동기화 기법 사용 객체
	_bool				m_bFinish;


public:
	static CLoadingThread* Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
	virtual void	Free();

};



