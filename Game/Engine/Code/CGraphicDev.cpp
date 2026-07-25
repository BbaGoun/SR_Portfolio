#include "CGraphicDev.h"

IMPLEMENT_SINGLETON(CGraphicDev)

CGraphicDev::CGraphicDev() : m_pSDK(nullptr), m_pGraphicDev(nullptr)
{

}
CGraphicDev::~CGraphicDev()
{
	Free();
}

HRESULT		CGraphicDev::Ready_GraphicDev(HWND hWnd, WINMODE eMode, 
	const _uint& iSizeX,
	const _uint& iSizeY,
	CGraphicDev** ppGraphicDev)
{
	// 1. 성능을 조사할 sdk 컴 객체 생성

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (nullptr == m_pSDK)
		return E_FAIL;

	// 2. 그래픽 카드 성능 조사
	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// D3DADAPTER_DEFAULT : 현재 컴퓨터에 장착된 기본 그래픽 카드

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
		return E_FAIL;

	_ulong	dwFlag(0);

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = iSizeX;
	d3dpp.BackBufferHeight = iSizeY;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD = 스왑 체인 방식,
	// D3DSWAPEFFECT_FLIP = 버퍼를 하나두고 번갈아가며 실행,
	// D3DSWAPEFFECT_COPY = 더블 버퍼링과 유사한 방식,

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = hWnd;

	d3dpp.Windowed = eMode;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 전체 화면 모드일 때 반영
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	// 3. sdk 컴 객체를 이용하여 그리기 컴 객체 생성

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwFlag, &d3dpp, &m_pGraphicDev)))
	{
		MSG_BOX("GraphicDev Create Failed");
		return E_FAIL;
	}

	*ppGraphicDev = this;

	return S_OK;
}

// 후면 버퍼

// 1. 지우기
// 2. 다음 화면에 출력할 것을 준비

void	CGraphicDev::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0,		// 렉트의 개수
						NULL,	// 렉트의 주소
						D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						Color,		// 후면 버퍼 텍스처 색상
						1.f,		// "z버퍼 초기화 값"
						0);			// 스텐실 버퍼 초기화 값

	m_pGraphicDev->BeginScene();
}

void	CGraphicDev::Render_End(HWND hWnd)
{
	m_pGraphicDev->EndScene();

	// 1. 렉트의 주소
	// 2. 렉트의 주소
	// 3. 창 핸들(기억해야 함) : null 인 경우 장치 초기화 시, 대입한 핸들로 자동 지정
	// 4. 몰라

	// 1, 2, 4 매개 변수를 사용하려면 SwapEffect 값이 D3DSWAPEFFECT_COPY 일 때만 사용 가능
	m_pGraphicDev->Present(NULL, NULL, hWnd, NULL);
}

void	CGraphicDev::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pSDK);
}