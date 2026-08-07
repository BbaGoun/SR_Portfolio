#include "pch.h"
#include "CMainEditor.h"
#include "CProtoMgr.h"
#include "CDInputMgr.h"
#include "CFontMgr.h"
#include "CCameraMgr.h"
#include "CRenderer.h"

CMainEditor::CMainEditor()
	: m_pDeviceClass(nullptr), m_pGraphicDev(nullptr)
	, m_pManagementClass(CManagement::GetInstance())
{
}

CMainEditor::~CMainEditor()
{
}

HRESULT CMainEditor::Ready_MainEditor()
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


void CMainEditor::FixedUpdate_MainEditor(const float& fFixedDeltaTime)
{
	m_pManagementClass->FixedUpdate_Scene(fFixedDeltaTime);
}

int CMainEditor::Update_MainEditor(const float& fDeltaTime)
{
	CDInputMgr::GetInstance()->Update_InputDev();
	m_pManagementClass->Update_Scene(fDeltaTime);

	return 0;
}

void CMainEditor::LateUpdate_MainEditor(const float& fDeltaTime)
{
	m_pManagementClass->LateUpdate_Scene(fDeltaTime);
	CDInputMgr::GetInstance()->LateUpdate_InputDev();
}

void CMainEditor::Render_MainEditor()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	m_pDeviceClass->Render_End();
}

HRESULT CMainEditor::Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	/*if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
		return E_FAIL;*/

	m_pDeviceClass->AddRef();

	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	m_pGraphicDev->AddRef();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	if (FAILED(CFontMgr::GetInstance()->Ready_Font(m_pGraphicDev, L"Font_Default", L"°ß¸íÁ¶", 20, 15, FW_HEAVY)))
		return E_FAIL;

	if (FAILED(CFontMgr::GetInstance()->Ready_Font(m_pGraphicDev, L"Font_Jinji", L"±Ã¼­", 20, 15, FW_THIN)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainEditor::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	//Engine::CScene* pLoading = CLoading::Create(pGraphicDev);

	/*if (nullptr == pLoading)
		return E_FAIL;

	if (FAILED(m_pManagementClass->Set_Scene(pLoading)))
	{
		Safe_Release(pLoading);

		MSG_BOX("Logo Create Failed");
		return E_FAIL;
	}*/

	return S_OK;
}

CMainEditor* CMainEditor::Create()
{
	CMainEditor* pMainEditor = new CMainEditor;

	if (FAILED(pMainEditor->Ready_MainEditor()))
	{
		MSG_BOX("MainEditor Create Failed");
		Safe_Release(pMainEditor);
		return nullptr;
	}

	return pMainEditor;
}

void CMainEditor::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);

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
