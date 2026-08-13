#include "pch.h"
#include "CLoading.h"
#include "CLoadingThread.h"
#include "CProtoMgr.h"
#include "CFontMgr.h"
#include "CManagement.h"
#include "CRcTex.h"
#include "CEditorScene.h"
#include "CBackGround.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev), m_pLoadingThread(nullptr)
{
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Scene()
{
    m_pLoadingThread = CLoadingThread::Create(m_pGraphicDev, CLoadingThread::LOADING_STAGE);
   
    if (nullptr == m_pLoadingThread)
        return E_FAIL;

    return S_OK;
}

_int CLoading::Update_Scene(const _float& fDeltaTime)
{
    return 0;
}

void CLoading::LateUpdate_Scene(const _float& fDeltaTime)
{
    CScene::LateUpdate_Scene(fDeltaTime);
}

void CLoading::Render_Scene()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
    
    _tchar bufStr[128];
    m_pLoadingThread->Get_String(bufStr);

    ImGui::SetNextWindowSize(ImVec2(480, 270));
    ImGui::SetNextWindowPos(ImVec2(g_d3dpp.BackBufferWidth*0.5f, g_d3dpp.BackBufferHeight*0.5f));
    ImGui::Begin("Loading Status");
    ImGui::Text(ToUtf8(bufStr).c_str());

    if (m_pLoadingThread->Get_Finish())
    {
        if (ImGui::Button("Loading Done!"))
        {
            //Engine::CScene* pStage = CScene_Test::Create(m_pGraphicDev);
            Engine::CScene* pStage = CEditorScene::Create(m_pGraphicDev);

            CManagement::GetInstance()->Request_Scene(pStage);
        }
    }

    ImGui::End();

    ImGuiIO& io = ImGui::GetIO();

    // Rendering
    ImGui::EndFrame();
    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
    m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
    if (m_pGraphicDev->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        m_pGraphicDev->EndScene();
    }

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    HRESULT result = m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);
    if (result == D3DERR_DEVICELOST)
        g_DeviceLost = true;
}

HRESULT CLoading::Ready_Environment_Layer(const _tchar* pLayerTag)
{
    return S_OK;
}

HRESULT CLoading::Ready_Prototype()
{
    return S_OK;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLoading* pLogo = new CLoading(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        Safe_Release(pLogo);
        MSG_BOX("Logo Create Failed");
        return nullptr;
    }

    return pLogo;
}

void CLoading::Free()
{
    Safe_Release(m_pLoadingThread);

    CScene::Free();
}
