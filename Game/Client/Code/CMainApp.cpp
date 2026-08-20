#include "pch.h"
#include "CMainApp.h"
#include "CScene_Test.h"
#include "CProtoMgr.h"
#include "CDInputMgr.h"
#include "CLoading.h"
#include "CFontMgr.h"
#include "CCameraMgr.h"
#include "CRenderer.h"
#include "CCollisionMgr.h"
CMainApp::CMainApp()
	: m_pDeviceClass(nullptr), m_pGraphicDev(nullptr)
	, m_pManagementClass(CManagement::GetInstance())
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
#ifdef _DEBUG

	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

#endif // _DEBUG

	if (FAILED(Ready_DefaultSetting(&m_pGraphicDev)))
		return E_FAIL;

	if (FAILED(Ready_Scene(m_pGraphicDev)))
		return E_FAIL;

	return S_OK;
}


void CMainApp::FixedUpdate_MainApp(const float& fFixedDeltaTime)
{
	m_pManagementClass->FixedUpdate_Scene(fFixedDeltaTime);
}

int CMainApp::Update_MainApp(const float& fDeltaTime)
{
	CDInputMgr::GetInstance()->Update_InputDev();
	m_pManagementClass->Update_Scene(fDeltaTime);

	return 0;
}

void CMainApp::LateUpdate_MainApp(const float& fDeltaTime)
{
	m_pManagementClass->LateUpdate_Scene(fDeltaTime);
	CDInputMgr::GetInstance()->LateUpdate_InputDev();
}

void CMainApp::Render_MainApp()
{
	CRenderer::GetInstance()->Render_TargetPass(m_pGraphicDev);

	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	if (CCameraMgr::GetInstance()->GetCamerState() != CAMERA_END) {
		CameraInfo camInfo = CCameraMgr::GetInstance()->GetCameraInfo();
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &camInfo.matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &camInfo.matProj);
	}

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	HRESULT result = m_pDeviceClass->Render_End();
	if (result == D3DERR_DEVICELOST)
		m_bDeviceLost = true;
}

bool CMainApp::ResetTest()
{
	HRESULT hr = m_pGraphicDev->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST)
	{
		cout << "Lost" << "\n";
		::Sleep(10);
		return true;
	}
	if (hr == D3DERR_DEVICENOTRESET)
	{
		Reset_MainApp();
		return false;
	}
	m_bDeviceLost = false;
	return false;
}

void CMainApp::Reset_MainApp()
{
	cout << "Reset" << "\n";
	CFontMgr::GetInstance()->OnLostDevice();
	m_pManagementClass->OnLostDevice();

	m_pDeviceClass->Reset_GraphicDev();

	OnResetDevice();
	CFontMgr::GetInstance()->OnResetDevice();
	m_pManagementClass->OnResetDevice();
}

void CMainApp::Resize_MainApp(UINT uWidth, UINT uHeight)
{
	m_pDeviceClass->Resize_GraphicDev(uWidth, uHeight);
}

HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
		return E_FAIL;

	m_pDeviceClass->AddRef();

	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	m_pGraphicDev->AddRef();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	if (FAILED(CFontMgr::GetInstance()->Ready_Font(m_pGraphicDev, L"Font_Default", L"견명조", 20, 15, FW_HEAVY)))
		return E_FAIL;

	if (FAILED(CFontMgr::GetInstance()->Ready_Font(m_pGraphicDev, L"Font_Jinji", L"궁서", 20, 15, FW_THIN)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::CScene* pLoading = CLoading::Create(pGraphicDev);

	if (nullptr == pLoading)
		return E_FAIL;

	if (FAILED(m_pManagementClass->Set_Scene(pLoading)))
	{
		Safe_Release(pLoading);

		MSG_BOX("Logo Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CMainApp::OnResetDevice()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

CMainApp* CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp;

	if (FAILED(pMainApp->Ready_MainApp()))
	{
		MSG_BOX("MainApp Create Failed");
		Safe_Release(pMainApp);
		return nullptr;
	}

	return pMainApp;
}

void CMainApp::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);

	CCollisionMgr::DestroyInstance();
	CFrameMgr::DestroyInstance();
	CTimerMgr::DestroyInstance();
	m_pManagementClass->DestroyInstance();
	m_pDeviceClass->DestroyInstance();
	CProtoMgr::DestroyInstance();
	CDInputMgr::DestroyInstance();
	CFontMgr::DestroyInstance();
	CCameraMgr::DestroyInstance();
	CRenderer::DestroyInstance();
}
