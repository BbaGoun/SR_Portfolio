#include "CDInputMgr.h"

IMPLEMENT_SINGLETON(CDInputMgr)

Engine::CDInputMgr::CDInputMgr(void)
{
	ZeroMemory(m_byKeyState, sizeof(m_byKeyState));
}

Engine::CDInputMgr::~CDInputMgr(void)
{
	Free();
}

HRESULT Engine::CDInputMgr::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{

	// DInput 컴객체를 생성하는 함수
	if (FAILED(DirectInput8Create(hInst,
								DIRECTINPUT_VERSION,
								IID_IDirectInput8,
								(void**)&m_pInputSDK,
								NULL)))
								return E_FAIL;

	// 키보드 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;


	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();


	return S_OK;
}

void Engine::CDInputMgr::Update_InputDev(void)
{
	if (m_vecReplay.empty())
	{
		m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	}
	else
	{
		//if(m_dwFileSize / 256 > m_dwReplayCnt)
		//	memcpy(m_byKeyState, m_vecReplay[m_dwReplayCnt++].data(), sizeof(m_byKeyState));
	}
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

void Engine::CDInputMgr::LateUpdate_InputDev(void)
{
	memcpy(m_byBeforeKeyState, m_byKeyState, 256);
	memcpy(&m_tBeforeMouseState, &m_tMouseState, sizeof(m_tMouseState));

}

HRESULT CDInputMgr::BeginRecord(const wchar_t* szPath)
{
	if (m_pRecordFile)
		return E_FAIL;

	if (0 != _wfopen_s(&m_pRecordFile, szPath, L"wb"))
		return E_FAIL;

	return S_OK;
}

HRESULT CDInputMgr::Load_Record(const wchar_t* szPath)
{
	FILE* pFile = nullptr;
	if (0 != _wfopen_s(&pFile, szPath, L"rb"))
		return E_FAIL;

	fseek(pFile, 0, SEEK_END);
	m_dwFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	DWORD dwCount = m_dwFileSize / 256;
	m_vecReplay.resize(dwCount);

	size_t uRead = fread(m_vecReplay.data(), 256, dwCount, pFile);
	fclose(pFile);

	return (uRead == dwCount) ? S_OK : E_FAIL;

	return S_OK;
}

void CDInputMgr::End_Record()
{
	if (m_pRecordFile)
	{
		fclose(m_pRecordFile);
		m_pRecordFile = nullptr;
	}
}

void CDInputMgr::Record_FixedUpdate()
{
	if (m_pRecordFile)
		fwrite(m_byKeyState, sizeof(_byte), 256, m_pRecordFile);
}

void CDInputMgr::Load_FixedUpdate()
{
	if (!m_vecReplay.empty())
	{
		if (m_dwFileSize / 256 > m_dwReplayCnt)
			memcpy(m_byKeyState, m_vecReplay[m_dwReplayCnt++].data(), sizeof(m_byKeyState));
	}
}

void Engine::CDInputMgr::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
	End_Record();
}

