#include "pch.h"
#include "CInspector.h"
#include "CManagement.h"
#include "CCollider.h"
#include "CCube_Collider.h"
#include "CSphere_Collider.h"
#include "CProtoMgr.h"

CInspector::CInspector() : CWindow()
{
}

CInspector::CInspector(LPDIRECT3DDEVICE9 pGraphicDev) : CWindow(pGraphicDev)
{
}

CInspector::CInspector(const CInspector& rhs) : CWindow(rhs)
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

    Header(pObj);
    TransformCom(pObj);
    if (pObj->Get_Component<CVIBuffer>())
        MeshCom(pObj);
    if (pObj->Get_Component<CCollider>())
        ColliderComs(pObj);
    if (pObj->Get_Component<CTexture>())
        TextureCom(pObj);

    Add_Component_Button(pObj);

    ImGui::End();
}

void CInspector::Header(CGameObject* _pObj)
{
    static char s_nameBuf[128];
    strcpy_s(s_nameBuf, ToUtf8(_pObj->GetName()).c_str());

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Name");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-1.0f);  // 남은 가로를 입력칸이 채움
    if (ImGui::InputText("##Name", s_nameBuf, sizeof(s_nameBuf),
        ImGuiInputTextFlags_EnterReturnsTrue))
    {
        _pObj->SetName(FromUtf8(s_nameBuf).c_str());
    }
    ImGui::Text("GUID: %llu", _pObj->GetGuid());
}

void CInspector::TransformCom(CGameObject* _pObj)
{
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        // 직접 수정에 따른 오염을 방지하기 위해 복사
        CTransform* pTF = _pObj->Get_Transform();
        _matrix matLocal = *pTF->Get_LocalWorld();

        float t[3], r[3], s[3];
        // 위치, 회전, 스케일로 행렬을 분해한다.
        ImGuizmo::DecomposeMatrixToComponents((float*)&matLocal, t, r, s);

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
    }
}

void CInspector::MeshCom(CGameObject* _pObj)
{
    CVIBuffer* pBuf = _pObj->Get_Component<CVIBuffer>();

    ImGui::PushID(pBuf);

    bool open = ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen);

    if (ImGui::BeginPopupContextItem("remove"))  // 직전 아이템 = 이 헤더
    {
        if (ImGui::MenuItem("Remove Component")) {
            _pObj->Remove_Component(pBuf);
            ImGui::EndPopup();
            ImGui::PopID();
            return;
        }
        ImGui::EndPopup();
    }

    if (open) {
        auto& prototypes = CProtoMgr::GetInstance()->Get_Prototypes();
        std::string preview = "(None)";
        for (auto& proto : prototypes) {
            const ProtoRecord& rec = proto.second;
            if (rec.kind != ProtoKind::Mesh || rec.proto == nullptr)
                continue;
            if (pBuf && typeid(*pBuf) == typeid(*rec.proto)) {
                preview = ToUtf8(rec.name);
                break;
            }
        }
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Mesh");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1.0f);

        if (ImGui::BeginCombo("##Mesh", preview.c_str()))
        {
            for (auto& proto : prototypes)
            {
                const ProtoRecord& rec = proto.second;
                if (rec.kind != ProtoKind::Mesh)
                    continue;
                const WCHAR* labelW = rec.name;
                std::string label = ToUtf8(labelW);
                
                bool selected = pBuf && rec.proto &&
                    typeid(*pBuf) == typeid(*rec.proto);

                // 같은 표시 이름이 있을 수 있으니 태그를 id로
                ImGui::PushID(ToUtf8(rec.tag).c_str());
                if (ImGui::Selectable(label.c_str(), selected))
                {
                    // 이미 선택됐을 때는 교체할 필요 없음
                    if (!selected)
                    {
                        // 메시 교체
                        _pObj->Remove_Component(pBuf);
                        _pObj->Add_Component(rec.tag, rec.tag);
                        ImGui::PopID();
                        break;
                    }
                }
                if (selected)
                    ImGui::SetItemDefaultFocus(); // 열었을 때 현재 항목으로 스크롤
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
    }

    ImGui::PopID();
}

void CInspector::ColliderComs(CGameObject* _pObj)
{
    static char s_nameBuf[128];
    static char s_idBuf[128];
    auto colliders = _pObj->Get_Components<CCollider>();
    int i = 0;
    for (auto& col : colliders) {
        ++i;
        switch (col->GetColliderType()) {
        case CUBE_COLLIDER:
        {
            strcpy_s(s_nameBuf, "Box Collider");
            CCube_Collider* boxCol = static_cast<CCube_Collider*>(col);

            ImGui::PushID(boxCol);

            bool open = ImGui::CollapsingHeader(s_nameBuf, ImGuiTreeNodeFlags_DefaultOpen);

            if (ImGui::BeginPopupContextItem("remove"))  // 직전 아이템 = 이 헤더
            {
                if (ImGui::MenuItem("Remove Component")) {
                    _pObj->Remove_Component(boxCol);
                    ImGui::EndPopup();
                    ImGui::PopID();
                    return;
                }
                ImGui::EndPopup();
            }

            if (open) {
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Is Trigger");
                ImGui::SameLine();
                sprintf_s(s_idBuf, "##Is Trigger%d", i);
                bool bTrigger = boxCol->GetIsTrigger();
                if (ImGui::Checkbox(s_idBuf, &bTrigger)) {
                    boxCol->SetIsTrigger(bTrigger);
                }

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Offset");
                ImGui::SameLine();
                sprintf_s(s_idBuf, "##Offset%d", i);
                _vec3 vOffset = boxCol->Get_Offset();
                if (ImGui::DragFloat3(s_idBuf, vOffset, 0.1f)) {
                    boxCol->Set_Offset(vOffset);
                }

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Extend");
                ImGui::SameLine();
                sprintf_s(s_idBuf, "##Extend%d", i);
                _vec3 vExtends = ToVec3(boxCol->Get_Info().Extents);
                if (ImGui::DragFloat3(s_idBuf, vExtends, 0.1f, 0.01f, FLT_MAX)) {
                    boxCol->Set_Extents(vExtends);
                }
            }

            ImGui::PopID();
        }
        break;
        case SPHERE_COLLIDER:
        {
            CSphere_Collider* sphereCol = static_cast<CSphere_Collider*>(col);
            strcpy_s(s_nameBuf, "Sphere Collider");

            ImGui::PushID(sphereCol);

            bool open = ImGui::CollapsingHeader(s_nameBuf, ImGuiTreeNodeFlags_DefaultOpen);

            if (ImGui::BeginPopupContextItem("remove"))  // 직전 아이템 = 이 헤더
            {
                if (ImGui::MenuItem("Remove Component")) {
                    _pObj->Remove_Component(sphereCol);
                    ImGui::EndPopup();
                    ImGui::PopID();
                    return;
                }
                ImGui::EndPopup();
            }

            if (open) {
                ImGui::AlignTextToFramePadding();
                ImGui::Text("Trigger");
                ImGui::SameLine();
                sprintf_s(s_idBuf, "##Is Trigger%d", i);
                bool bTrigger = sphereCol->GetIsTrigger();
                if (ImGui::Checkbox(s_idBuf, &bTrigger)) {
                    sphereCol->SetIsTrigger(bTrigger);
                }

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Offset");
                ImGui::SameLine();
                sprintf_s(s_idBuf, "##Offset%d", i);
                _vec3 vOffset = sphereCol->Get_Offset();
                if (ImGui::DragFloat3(s_idBuf, vOffset, 0.1f)) {
                    sphereCol->Set_Offset(vOffset);
                }

                ImGui::AlignTextToFramePadding();
                ImGui::Text("Radius");
                ImGui::SameLine();                
                sprintf_s(s_idBuf, "##Radius%d", i);
                _float fRadius = sphereCol->Get_Info().Radius;
                if (ImGui::DragFloat(s_idBuf, &fRadius, 0.1f, 0.01f, FLT_MAX)) {
                    sphereCol->Set_Radius(fRadius);
                }
            }

            ImGui::PopID();
        }
        break;
        }
    }
}

void CInspector::TextureCom(CGameObject* _pObj)
{
    CTexture* pTex = _pObj->Get_Component<CTexture>();

    ImGui::PushID(pTex);

    bool open = ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen);

    if (ImGui::BeginPopupContextItem("remove"))  // 직전 아이템 = 이 헤더
    {
        if (ImGui::MenuItem("Remove Component")) {
            _pObj->Remove_Component(pTex);
            ImGui::EndPopup();
            ImGui::PopID();
            return;
        }
        ImGui::EndPopup();
    }

    if (open) {
        auto& prototypes = CProtoMgr::GetInstance()->Get_Prototypes();
        std::string preview = "(None)";
        for (auto& proto : prototypes) {
            const ProtoRecord& rec = proto.second;
            if (rec.kind != ProtoKind::Texture || rec.proto == nullptr)
                continue;
            if (pTex && !lstrcmp(pTex->Get_ProtoTag(), rec.tag)) {
                preview = ToUtf8(rec.name);
                break;
            }
        }
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Texture");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1.0f);

        if (ImGui::BeginCombo("##Texture", preview.c_str()))
        {
            for (auto& proto : prototypes)
            {
                const ProtoRecord& rec = proto.second;
                if (rec.kind != ProtoKind::Texture)
                    continue;
                const WCHAR* labelW = rec.name;
                std::string label = ToUtf8(labelW);

                bool selected = pTex && rec.proto &&
                    !lstrcmp(pTex->Get_ProtoTag(), rec.tag);

                // 같은 표시 이름이 있을 수 있으니 태그를 id로
                ImGui::PushID(ToUtf8(rec.tag).c_str());
                if (ImGui::Selectable(label.c_str(), selected))
                {
                    // 이미 선택됐을 때는 교체할 필요 없음
                    if (!selected)
                    {
                        // 메시 교체
                        _pObj->Remove_Component(pTex);
                        _pObj->Add_Component(rec.tag, rec.tag);
                        ImGui::PopID();
                        break;
                    }
                }
                if (selected)
                    ImGui::SetItemDefaultFocus(); // 열었을 때 현재 항목으로 스크롤
                ImGui::PopID();
            }
            ImGui::EndCombo();
        }
    }

    ImGui::PopID();
}

void CInspector::Add_Component_Button(CGameObject* _pObj)
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // 중앙 세팅
    const float avail = ImGui::GetContentRegionAvail().x;
    const float btnW = avail * 0.7f;          // 가로의 70%
    ImGui::SetCursorPosX((avail - btnW) * 0.5f);

    if (ImGui::Button("Add Component", ImVec2(btnW, 0)))
        ImGui::OpenPopup("AddComponentPopup");

    ImGui::SetNextWindowSize(ImVec2(250.f, 300.f), ImGuiCond_Always);
    if (!ImGui::BeginPopup("AddComponentPopup"))
        return;

    // 검색
    static char filter[128] = {};
    ImGui::SetNextItemWidth(-1.f);
    ImGui::InputTextWithHint("##filter", "Search...", filter, sizeof(filter));
    ImGui::Separator();

    auto& prototypes = CProtoMgr::GetInstance()->Get_Prototypes();

    // 람다 함수
    auto kindName = [](ProtoKind k) -> const char* {
        switch (k) {
        case ProtoKind::Mesh:     return "Mesh";
        case ProtoKind::Collider: return "Collider";
        case ProtoKind::Texture:  return "Texture";
        default:                  return "Other";
        }
        };

    // 검색어가 있으면 카테고리 없이 평평한 리스트
    const bool searching = filter[0] != '\0';

    if (searching)
    {
        std::string lowerFilter = filter;
        for (auto& c : lowerFilter)
            c = tolower(c);

        for (auto& proto : prototypes)
        {
            const ProtoRecord& rec = proto.second;
            if (!rec.addable)
                continue;

            std::string label = ToUtf8(rec.name[0] ? rec.name : rec.tag);
            std::string lowerLabel = label;
            for (auto& c : lowerLabel)
                c = tolower(c);

            // 검색을 위해서 소문자로 통일
            if(!KMPContain(lowerLabel, lowerFilter))
                continue;

            if (ImGui::Selectable(label.c_str()))
            {
                _pObj->Add_Component(rec.tag, rec.tag);
                ImGui::CloseCurrentPopup();
            }
        }
    }
    else
    {
        // 카테고리(1단) → 프로토타입(2단)
        const ProtoKind kinds[] = {
            ProtoKind::Mesh, ProtoKind::Collider, ProtoKind::Texture
        };

        for (ProtoKind kind : kinds)
        {
            if (!ImGui::BeginMenu(kindName(kind)))
                continue;

            for (auto& proto : prototypes)
            {
                const ProtoRecord& rec = proto.second;
                if (!rec.addable || rec.kind != kind)
                    continue;

                std::string label = ToUtf8(rec.name[0] ? rec.name : rec.tag);
                if (ImGui::MenuItem(label.c_str()))
                {
                    _pObj->Add_Component(rec.tag, rec.tag);
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndMenu();
        }
    }

    ImGui::EndPopup();
}

void CInspector::InvalidateDeviceObjects()
{
}

HRESULT CInspector::Ready_Window()
{
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