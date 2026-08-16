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
	// 1. ������ ������ sdk �� ��ü ����

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (nullptr == m_pSDK)
		return E_FAIL;

	// 2. �׷��� ī�� ���� ����
	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// D3DADAPTER_DEFAULT : ���� ��ǻ�Ϳ� ������ �⺻ �׷��� ī��

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

	// D3DSWAPEFFECT_DISCARD = ���� ü�� ���,
	// D3DSWAPEFFECT_FLIP = ���۸� �ϳ��ΰ� �����ư��� ����,
	// D3DSWAPEFFECT_COPY = ���� ���۸��� ������ ���,

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = hWnd;

	d3dpp.Windowed = eMode;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// ��ü ȭ�� ����� �� �ݿ�
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	// 3. sdk �� ��ü�� �̿��Ͽ� �׸��� �� ��ü ����

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwFlag, &d3dpp, &m_pGraphicDev)))
	{
		MSG_BOX("GraphicDev Create Failed");
		return E_FAIL;
	}

	*ppGraphicDev = this;

	return S_OK;
}

// �ĸ� ����

// 1. �����
// 2. ���� ȭ�鿡 ����� ���� �غ�

void	CGraphicDev::Render_Begin(D3DXCOLOR Color)
{
	// Clear�� ���� Viewport ������ �����.
	// ���� �������� ������ Viewport�� �������� ������ Clear���� �ʾ� �ܻ��� ���´�.
	D3DVIEWPORT9 tFullView{};
	tFullView.X = 0;
	tFullView.Y = 0;
	tFullView.Width = WINCX;
	tFullView.Height = WINCY;
	tFullView.MinZ = 0.f;
	tFullView.MaxZ = 1.f;
	m_pGraphicDev->SetViewport(&tFullView);

	m_pGraphicDev->Clear(0,		// ��Ʈ�� ����
						NULL,	// ��Ʈ�� �ּ�
						D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						Color,		// �ĸ� ���� �ؽ�ó ����
						1.f,		// "z���� �ʱ�ȭ ��"
						0);			// ���ٽ� ���� �ʱ�ȭ ��

	m_pGraphicDev->BeginScene();
}

void	CGraphicDev::Render_End(HWND hWnd)
{
	m_pGraphicDev->EndScene();

	// 1. ��Ʈ�� �ּ�
	// 2. ��Ʈ�� �ּ�
	// 3. â �ڵ�(����ؾ� ��) : null �� ��� ��ġ �ʱ�ȭ ��, ������ �ڵ�� �ڵ� ����
	// 4. ����

	// 1, 2, 4 �Ű� ������ ����Ϸ��� SwapEffect ���� D3DSWAPEFFECT_COPY �� ���� ��� ����
	m_pGraphicDev->Present(NULL, NULL, hWnd, NULL);
}

void	CGraphicDev::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pSDK);
}