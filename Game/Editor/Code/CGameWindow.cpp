#include "pch.h"
#include "CGameWindow.h"

CGameWindow::CGameWindow() : CWindow()
, m_pSceneTex(nullptr)
, m_pSceneDepth(nullptr)
, m_rtW(0), m_rtH(0)
{
}

CGameWindow::CGameWindow(LPDIRECT3DDEVICE9 pGraphicDev) : CWindow(pGraphicDev)
, m_pSceneTex(nullptr)
, m_pSceneDepth(nullptr)
, m_rtW(0), m_rtH(0)
{
}

CGameWindow::CGameWindow(const CGameWindow& rhs) : CWindow(rhs)
, m_pSceneTex(rhs.m_pSceneTex)
, m_pSceneDepth(rhs.m_pSceneDepth)
, m_rtW(rhs.m_rtW), m_rtH(rhs.m_rtH)
{
}

CGameWindow::~CGameWindow()
{
}

void CGameWindow::Update_Window()
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));
    ImGuiIO& io = ImGui::GetIO();

    static int selected = 0;

    // 오브젝트의 월드 행렬
    static float objectMatrix[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        -1.25,1,0,1
    };
    static float objectMatrix2[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        1.25,1,0,1
    };
    // Transform 조작 창의 설정
    static ImGuizmo::OPERATION op = ImGuizmo::TRANSLATE;
    static ImGuizmo::OPERATION op2 = ImGuizmo::TRANSLATE;
    static ImGuizmo::MODE mode = ImGuizmo::WORLD;
    static ImGuizmo::MODE mode2 = ImGuizmo::WORLD;

    // 뷰 행렬 세팅
    static _vec3 vEye = { 0, 5.f, -5.f };
    static float yaw = 0.f, pitch = -0.7f;
    _vec3 vLook = { cosf(pitch) * sinf(yaw), sinf(pitch), cosf(pitch) * cosf(yaw) };
    _vec3 vAt = vEye + vLook;
    static _vec3 vUp = { 0.f, 1.f, 0.f };
    _matrix matView, matInvView;
    D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
    D3DXMatrixInverse(&matInvView, 0, &matView);

    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
    ImGui::Begin("Game");

    ImVec2 viewPos = ImGui::GetCursorScreenPos(); // content 영역의 좌상단
    ImVec2 viewSize = ImGui::GetContentRegionAvail(); // content 영역의 크기

    // 투영 행렬 세팅
    _matrix matProj, matInvProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.f), viewSize.x / viewSize.y, 0.1f, 100.f);
    D3DXMatrixInverse(&matInvProj, 0, &matProj);

    // ### 기존 큐브를 RT에 그리는 과정
    if (!m_pSceneTex || m_rtW != viewSize.x || m_rtH != viewSize.y) {
        Safe_Release(m_pSceneTex);
        Safe_Release(m_pSceneDepth);

        D3DXCreateTexture(
            m_pGraphicDev,
            viewSize.x, viewSize.y,
            1,
            D3DUSAGE_RENDERTARGET,
            D3DFMT_A8R8G8B8,
            D3DPOOL_DEFAULT,
            &m_pSceneTex);

        m_pGraphicDev->CreateDepthStencilSurface(
            viewSize.x, viewSize.y,
            D3DFMT_D24S8,
            D3DMULTISAMPLE_NONE, 0, TRUE,
            &m_pSceneDepth, nullptr);

        m_rtW = viewSize.x; m_rtH = viewSize.y;
    }

    if (m_pSceneTex) {
        LPDIRECT3DSURFACE9 pOldRT = nullptr;
        LPDIRECT3DSURFACE9 pOldDepth = nullptr;
        m_pGraphicDev->GetRenderTarget(0, &pOldRT);
        m_pGraphicDev->GetDepthStencilSurface(&pOldDepth);

        LPDIRECT3DSURFACE9 pRT = nullptr;
        m_pSceneTex->GetSurfaceLevel(0, &pRT);

        m_pGraphicDev->SetRenderTarget(0, pRT);
        m_pGraphicDev->SetDepthStencilSurface(m_pSceneDepth);

        m_pGraphicDev->Clear(0, nullptr,
            D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
            D3DCOLOR_XRGB(40, 40, 40), 1.f, 0);

        m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
        m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

        m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
        m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

        // 큐브 1
        m_pGraphicDev->SetTransform(D3DTS_WORLD, (_matrix*)objectMatrix);
        m_pCubeBuffer->Render_Buffer();

        // 큐브 2
        m_pGraphicDev->SetTransform(D3DTS_WORLD, (_matrix*)objectMatrix2);
        m_pCubeBuffer->Render_Buffer();

        // 복구
        m_pGraphicDev->SetRenderTarget(0, pOldRT);
        m_pGraphicDev->SetDepthStencilSurface(pOldDepth);
        pRT->Release(); pOldRT->Release(); pOldDepth->Release();

        // ### ImGui Scene 창에 RT 이미지를 붙이기
        ImGui::Image((ImTextureRef)m_pSceneTex, viewSize);
    }

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(
        viewPos.x,
        viewPos.y,
        viewSize.x,
        viewSize.y);

    ImGui::End();
}

void CGameWindow::InvalidateDeviceObjects()
{
	Safe_Release(m_pSceneTex);
	Safe_Release(m_pSceneDepth);
	m_rtW = 0; m_rtH = 0;
}

HRESULT CGameWindow::Ready_Window()
{
    m_pCubeBuffer = Engine::CCube::Create(m_pGraphicDev);

	return S_OK;
}

CWindow* CGameWindow::Clone()
{
	CWindow* pWindow = new CGameWindow(*this);
	return pWindow;
}

CGameWindow* CGameWindow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGameWindow* pWindow = new CGameWindow(pGraphicDev);

	if (FAILED(pWindow->Ready_Window())) 
	{
		MSG_BOX("CGameWindow Create Failed");
		Safe_Release(pWindow);
		return nullptr;
	}

	return pWindow;
}

void CGameWindow::Free()
{
	InvalidateDeviceObjects();
	CWindow::Free();
}