#include "pch.h"
#include "CProject.h"

CProject::CProject() : CWindow()
, m_pSceneTex(nullptr)
, m_pSceneDepth(nullptr)
, m_rtW(0), m_rtH(0)
{
}

CProject::CProject(LPDIRECT3DDEVICE9 pGraphicDev) : CWindow(pGraphicDev)
, m_pSceneTex(nullptr)
, m_pSceneDepth(nullptr)
, m_rtW(0), m_rtH(0)
{
}

CProject::CProject(const CProject& rhs) : CWindow(rhs)
, m_pSceneTex(rhs.m_pSceneTex)
, m_pSceneDepth(rhs.m_pSceneDepth)
, m_rtW(rhs.m_rtW), m_rtH(rhs.m_rtH)
{
}

CProject::~CProject()
{
}

void CProject::Update_Window()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));

    ImGui::Begin("Project");
    ImGui::End();
}

void CProject::InvalidateDeviceObjects()
{
    Safe_Release(m_pSceneTex);
    Safe_Release(m_pSceneDepth);
    m_rtW = 0; m_rtH = 0;
}

HRESULT CProject::Ready_Window()
{
    m_pCubeBuffer = Engine::CCube::Create(m_pGraphicDev);

    return S_OK;
}

CWindow* CProject::Clone()
{
    CWindow* pWindow = new CProject(*this);
    return pWindow;
}

CProject* CProject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CProject* pWindow = new CProject(pGraphicDev);

    if (FAILED(pWindow->Ready_Window()))
    {
        MSG_BOX("CProject Create Failed");
        Safe_Release(pWindow);
        return nullptr;
    }

    return pWindow;
}

void CProject::Free()
{
    InvalidateDeviceObjects();
    CWindow::Free();
}