#include "pch.h"
#include "CInspector.h"
#include "CManagement.h"

CInspector::CInspector() : CWindow()
, m_pSceneTex(nullptr)
, m_pSceneDepth(nullptr)
, m_rtW(0), m_rtH(0)
{
}

CInspector::CInspector(LPDIRECT3DDEVICE9 pGraphicDev) : CWindow(pGraphicDev)
, m_pSceneTex(nullptr)
, m_pSceneDepth(nullptr)
, m_rtW(0), m_rtH(0)
{
}

CInspector::CInspector(const CInspector& rhs) : CWindow(rhs)
, m_pSceneTex(rhs.m_pSceneTex)
, m_pSceneDepth(rhs.m_pSceneDepth)
, m_rtW(rhs.m_rtW), m_rtH(rhs.m_rtH)
{
}

CInspector::~CInspector()
{
}

void CInspector::Update_Window()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));

    ImGui::Begin("Inspector");
    
    bool sceneFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);

    if (!ImGui::IsAnyItemActive() && sceneFocused) {
        if (ImGui::IsKeyDown(ImGuiKey_Q))
            g_GizmoOp = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyDown(ImGuiKey_W))
            g_GizmoOp = ImGuizmo::ROTATE;
        if (ImGui::IsKeyDown(ImGuiKey_E))
            g_GizmoOp = ImGuizmo::SCALE;
    }

    if (!g_bSelected) { 
        ImGui::Text("Nothing selected"); 
        ImGui::End(); 
        return; 
    }

    const auto& map = CManagement::GetInstance()->Get_GameObjects(L"Default");

    CGameObject* pObj = FindByGuid(g_uSelected, map);
    if (!pObj) { ImGui::End(); return; }

    static char s_nameBuf[128];
    strcpy_s(s_nameBuf, ToUtf8(pObj->GetName()).c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Name");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-1.0f);  // 남은 가로를 입력칸이 채움
    if (ImGui::InputText("##Name", s_nameBuf, sizeof(s_nameBuf),
        ImGuiInputTextFlags_EnterReturnsTrue))
    {
        pObj->SetName(FromUtf8(s_nameBuf).c_str());
    }
    ImGui::Text("GUID: %llu", pObj->GetGuid());

    // 직접 수정에 따른 오염을 방지하기 위해 복사
    _matrix matLocal = *pObj->Get_Transform()->Get_LocalWorld();
    
    float t[3], r[3], s[3];
    // 위치, 회전, 스케일로 행렬을 분해한다.
    ImGuizmo::DecomposeMatrixToComponents((float*)&matLocal, t, r, s);

    CTransform* pTF = pObj->Get_Transform();

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Position");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##Tr", t, 0.25f)) {
        ImGuizmo::RecomposeMatrixFromComponents(t, r, s, matLocal);
        pTF->Set_LocalWorld(&matLocal);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Rotation");
    ImGui::SameLine();
    if (ImGui::DragFloat3("##Rt", r)) {
        ImGuizmo::RecomposeMatrixFromComponents(t, r, s, matLocal);
        pTF->Set_LocalWorld(&matLocal);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Scale   ");

    ImGui::SameLine();
    if (ImGui::DragFloat3("##Sc", s, 0.1f)) {
        ImGuizmo::RecomposeMatrixFromComponents(t, r, s, matLocal);
        pTF->Set_LocalWorld(&matLocal);
    }

    ImGui::End();
}

void CInspector::InvalidateDeviceObjects()
{
    Safe_Release(m_pSceneTex);
    Safe_Release(m_pSceneDepth);
    m_rtW = 0; m_rtH = 0;
}

HRESULT CInspector::Ready_Window()
{
    m_pCubeBuffer = Engine::CCube::Create(m_pGraphicDev);

    return S_OK;
}

CWindow* CInspector::Clone()
{
    CWindow* pWindow = new CInspector(*this);
    return pWindow;
}

CInspector* CInspector::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CInspector* pWindow = new CInspector(pGraphicDev);

    if (FAILED(pWindow->Ready_Window()))
    {
        MSG_BOX("CInspector Create Failed");
        Safe_Release(pWindow);
        return nullptr;
    }

    return pWindow;
}

void CInspector::Free()
{
    InvalidateDeviceObjects();
    CWindow::Free();
}