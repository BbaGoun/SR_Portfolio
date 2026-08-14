#include "pch.h"
#include "CSequence.h"

CSequence::CSequence() : CWindow()
, m_pSceneTex(nullptr)
, m_pSceneDepth(nullptr)
, m_rtW(0), m_rtH(0)
{
}

CSequence::CSequence(LPDIRECT3DDEVICE9 pGraphicDev) : CWindow(pGraphicDev)
, m_pSceneTex(nullptr)
, m_pSceneDepth(nullptr)
, m_rtW(0), m_rtH(0)
{
}

CSequence::CSequence(const CSequence& rhs) : CWindow(rhs)
, m_pSceneTex(rhs.m_pSceneTex)
, m_pSceneDepth(rhs.m_pSceneDepth)
, m_rtW(rhs.m_rtW), m_rtH(rhs.m_rtH)
{
}

CSequence::~CSequence()
{
}

void CSequence::Update_Window()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));

    ImGui::Begin("Sequence");
    ImGui::End();
}

void CSequence::InvalidateDeviceObjects()
{
    Safe_Release(m_pSceneTex);
    Safe_Release(m_pSceneDepth);
    m_rtW = 0; m_rtH = 0;
}

HRESULT CSequence::Ready_Window()
{
    m_pCubeBuffer = Engine::CCube::Create(m_pGraphicDev);

    return S_OK;
}

CWindow* CSequence::Clone()
{
    CWindow* pWindow = new CSequence(*this);
    return pWindow;
}

CSequence* CSequence::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSequence* pWindow = new CSequence(pGraphicDev);

    if (FAILED(pWindow->Ready_Window()))
    {
        MSG_BOX("CSequence Create Failed");
        Safe_Release(pWindow);
        return nullptr;
    }

    return pWindow;
}

void CSequence::Free()
{
    InvalidateDeviceObjects();
    CWindow::Free();
}