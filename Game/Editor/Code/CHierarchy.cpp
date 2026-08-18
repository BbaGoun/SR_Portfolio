#include "pch.h"
#include "CHierarchy.h"
#include "CEmpty.h"
#include "CManagement.h"

CHierarchy::CHierarchy() : CWindow()
{
}

CHierarchy::CHierarchy(LPDIRECT3DDEVICE9 pGraphicDev) : CWindow(pGraphicDev)
{
}

CHierarchy::CHierarchy(const CHierarchy& rhs) : CWindow(rhs)
{
}

CHierarchy::~CHierarchy()
{
}

void CHierarchy::Update_Window()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));

    ImGui::Begin("Hierarchy");

    // Hierarchy에 씬 이름 표기
    ImGuiStyle& st = ImGui::GetStyle();
    ImVec2 winPos = ImGui::GetWindowPos();
    float  winW = ImGui::GetWindowSize().x;
    ImVec2 p0 = ImVec2(winPos.x, ImGui::GetCursorScreenPos().y - st.WindowPadding.y);
    float  h = ImGui::GetFrameHeight();

    ImGui::GetWindowDrawList()->AddRectFilled(
        p0,
        ImVec2(p0.x + winW, p0.y + h),
        IM_COL32(50, 50, 50, 255));   // WindowBg보다 어둡게

    ImGui::SetCursorScreenPos(ImVec2(p0.x + ImGui::GetStyle().FramePadding.x, p0.y));
    ImGui::AlignTextToFramePadding();
    
    std::wstring sceneName = GetSceneName(CManagement::GetInstance()->Get_ScenePath());
    if (!sceneName.compare(L""))
        sceneName = L"untitled";
    if (CManagement::GetInstance()->Get_SceneDirty())
        sceneName += L"*";
    ImGui::TextUnformatted(ToUtf8(sceneName.c_str()).c_str());

    bool sceneFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);

    if (!m_bRenaming && sceneFocused) {
        if (ImGui::IsKeyDown(ImGuiKey_Q))
            g_GizmoOp = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyDown(ImGuiKey_W))
            g_GizmoOp = ImGuizmo::ROTATE;
        if (ImGui::IsKeyDown(ImGuiKey_E))
            g_GizmoOp = ImGuizmo::SCALE;
    }

    Show_Hierarchy();

    ImVec2 viewPos = ImGui::GetCursorScreenPos(); 
    ImVec2 viewSize = ImGui::GetContentRegionAvail(); 

    ImGui::InvisibleButton("##dummy", viewSize);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
        g_bSelected = false;
        g_uSelected = 0;
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("HIERARCHY_GO"))
        {
            CGameObject* pSrc = *(CGameObject**)payload->Data;
            if (pSrc)
            {
                // 최상위 부모로
                pSrc->To_Root();
            }
        }
        ImGui::EndDragDropTarget();
    }

    const bool windowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_None);
    const bool insideRect = ImGui::IsMouseHoveringRect(
        viewPos,
        ImVec2(viewPos.x + viewSize.x, viewPos.y + viewSize.y),
        false);
    const bool viewHovered = windowHovered && insideRect;

    // 오른쪽 클릭을 하면 팝업을 연다.
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && viewHovered)
        ImGui::OpenPopup("Hierarchy_popup");

    // 팝업이 열려있나 여부와 관계없이 팝업 if문은 매 프레임 실행
    RightClick_PopUp();

    ImGui::End();
}

void CHierarchy::Show_Hierarchy()
{
    const auto& roots = CManagement::GetInstance()->Get_Roots(L"Default");
    for (CGameObject* pObj : roots) {
        if (pObj == nullptr)
            continue;
        Draw_TreeNode(pObj);
    }
}

void CHierarchy::Draw_TreeNode(CGameObject* pObj)
{
    const auto& children = pObj->Get_Children();
    const bool hasChild = !children.empty();
    
    // 화살표를 클릭했을 때만 노드가 열린다 
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
        | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    // 이름이 같아도 GUID로 ID 구분
    std::string label = ToUtf8(pObj->GetName()); // WCHAR* → UTF-8

    const bool bRenaming = m_bRenaming && (m_uRenameGuid == pObj->GetGuid());

    // Input Text가 있을 때 너비를 전부 먹어버리면 입력 칸을 위한 공간이 없어짐.
    // 노드의 히트박스를 콘텐츠 영역의 전체 가로 너비로 확장
    if (!bRenaming)
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    // 자식이 없으면 잎 노드, 열 수 있는 하위가 없다.
    if (!hasChild)
        flags |= ImGuiTreeNodeFlags_Leaf;
    if (g_uSelected == pObj->GetGuid())
        flags |= ImGuiTreeNodeFlags_Selected;

    if(bRenaming)
        ImGui::AlignTextToFramePadding();
    
    if (m_uOpenGuid == pObj->GetGuid()) {
        ImGui::SetNextItemOpen(true, ImGuiCond_Always);
        m_uOpenGuid = 0;
    }

    // 전달하는 Guid를 ID로 하여 노드가 분리됨.
    bool open = ImGui::TreeNodeEx(
        (void*)(uintptr_t)pObj->GetGuid(),
        flags,
        "%s", bRenaming ? "" : label.c_str());

    if (!bRenaming && ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        g_bSelected = true;
        g_uSelected = pObj->GetGuid();
    }

    if (!bRenaming && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
    {
        m_bRenaming = true;
        m_uRenameGuid = pObj->GetGuid();
        strcpy_s(m_renameBuf, label.c_str());
        m_bRenameFocus = true;
    }

    if (bRenaming) {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1.0f);

        if (m_bRenameFocus)
        {
            ImGui::SetKeyboardFocusHere(); // 다음 위젯 = InputText
            m_bRenameFocus = false;
        }

        if (ImGui::InputText("##Rename", m_renameBuf, sizeof(m_renameBuf),
            ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            pObj->SetName(FromUtf8(m_renameBuf).c_str());
            m_bRenaming = false;
            m_uRenameGuid = 0;
        }

        // Esc
        if (ImGui::IsKeyPressed(ImGuiKey_Escape)){
            m_bRenaming = false;
            m_uRenameGuid = 0;
        }

        if (ImGui::IsItemDeactivatedAfterEdit()) {
            pObj->SetName(FromUtf8(m_renameBuf).c_str());
            m_bRenaming = false;
            m_uRenameGuid = 0;
        }

        if (!ImGui::IsItemDeactivatedAfterEdit() && ImGui::IsItemDeactivated()) {
            m_bRenaming = false;
            m_uRenameGuid = 0;
        }

        if (open && hasChild)
        {
            // 자식 그리기를 부모쪽에서 담당하며 TreePush를 따로 안하기 때문에
            // 같은 그리기 내의 객체로 인식 -> 열린 상태에서 이동하면 그대로 열려있음
            ImGui::Indent();
            for (CGameObject* pChild : children)
                Draw_TreeNode(pChild);
            ImGui::Unindent();
        }
    }
    else {
        // ----- Drag Source: 이 노드를 드래그 -----
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            CGameObject* pPayload = pObj;
            ImGui::SetDragDropPayload("HIERARCHY_GO", &pPayload, sizeof(pPayload));
            ImGui::Text("%s", label.c_str()); // 드래그 중 툴팁 미리보기
            ImGui::EndDragDropSource();
        }
        // ----- Drop Target: 여기로 드롭하면 이 노드의 자식 -----
        if (ImGui::BeginDragDropTarget())
        {
            ImVec2 rmin = ImGui::GetItemRectMin();
            ImVec2 rmax = ImGui::GetItemRectMax();
            float y = ImGui::GetMousePos().y;
            float h = rmax.y - rmin.y;
            enum { Before, Onto, After } zone;
            if (y < rmin.y + h * 0.25f)      zone = Before;
            else if (y > rmax.y - h * 0.25f) zone = After;
            else                             zone = Onto;

            const ImGuiPayload* p = ImGui::AcceptDragDropPayload(
                "HIERARCHY_GO",
                ImGuiDragDropFlags_AcceptBeforeDelivery |
                ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

            if (p)
            {
                ImDrawList* dl = ImGui::GetWindowDrawList();
                const ImU32 col = ImGui::GetColorU32(ImGuiCol_DragDropTarget);
                if (zone == Onto)
                    dl->AddRect(rmin, rmax, col, 0.f, 0, 2.f);
                else
                {
                    const float ly = (zone == Before) ? rmin.y : rmax.y;
                    dl->AddLine(ImVec2(rmin.x, ly), ImVec2(rmax.x, ly), col, 3.f);
                }

                if (p->Delivery)
                {
                    CGameObject* pSrc = *(CGameObject**)p->Data;
                    CGameObject* pDst = pObj;
                    if (pSrc && pDst && pSrc != pDst)
                    {
                        switch (zone) {
                        case Before:
                            pDst->Insert_Before(pSrc);
                            break;
                        case Onto:
                            pDst->Set_Child(pSrc);
                            m_uOpenGuid = pDst->GetGuid();
                            break;
                        case After:
                            pDst->Insert_After(pSrc);
                            break;
                        }
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }

        // 오른쪽 클릭 시 메뉴 생성
        ImGui::PushID(pObj->GetGuid());
        if (ImGui::BeginPopupContextItem("node_menu")) {
            if (ImGui::Selectable("Create Empty Child")) {
                CGameObject* pChild = CEmpty::Create(m_pGraphicDev);
                pChild->SetName(L"Empty");
                uint64_t guid = CManagement::GetInstance()->GenerateGuid();
                pChild->SetGuid(guid);

                wstring s = std::to_wstring(guid);
                CManagement::GetInstance()->Add_GameObject(L"Default", s.c_str(), pChild);
                g_bSelected = true;
                g_uSelected = guid;

                pObj->Set_Child(pChild);
                m_uOpenGuid = pObj->GetGuid();
            }
            if (ImGui::Selectable("Rename")) {
                m_bRenaming = true;
                m_uRenameGuid = pObj->GetGuid();
                strcpy_s(m_renameBuf, label.c_str());
                m_bRenameFocus = true;
            }
            if (ImGui::Selectable("Delete")) {
                CManagement::GetInstance()->Delete_GameObject(L"Default", pObj);
            }

            ImGui::EndPopup();
        }
        ImGui::PopID();

        if (open && hasChild)
        {
            ImGui::Indent();
            for (CGameObject* pChild : children)
                Draw_TreeNode(pChild);
            ImGui::Unindent();
        }
    }
}

void CHierarchy::RightClick_PopUp()
{
    if (ImGui::BeginPopup("Hierarchy_popup"))
    {
        if (ImGui::Selectable("Create Empty"))
        {
            CGameObject* obj = CEmpty::Create(m_pGraphicDev);
            obj->SetName(L"Empty");
            uint64_t guid = CManagement::GetInstance()->GenerateGuid();
            obj->SetGuid(guid);

            wstring s = std::to_wstring(guid);
            CManagement::GetInstance()->Add_GameObject(L"Default", s.c_str(), obj);
            g_bSelected = true;
            g_uSelected = guid;
        }
        ImGui::EndPopup();
    }
}

void CHierarchy::InvalidateDeviceObjects()
{

}

HRESULT CHierarchy::Ready_Window()
{

    return S_OK;
}

CWindow* CHierarchy::Clone()
{
    CWindow* pWindow = new CHierarchy(*this);
    return pWindow;
}

CHierarchy* CHierarchy::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CHierarchy* pWindow = new CHierarchy(pGraphicDev);

    if (FAILED(pWindow->Ready_Window()))
    {
        MSG_BOX("CHierarchy Create Failed");
        Safe_Release(pWindow);
        return nullptr;
    }

    return pWindow;
}

void CHierarchy::Free()
{
    InvalidateDeviceObjects();
    CWindow::Free();
}