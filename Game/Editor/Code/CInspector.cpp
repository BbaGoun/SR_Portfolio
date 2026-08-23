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
        if (ImGui::IsKeyDown(ImGuiKey_1))
            g_GizmoOp = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyDown(ImGuiKey_2))
            g_GizmoOp = ImGuizmo::ROTATE;
        if (ImGui::IsKeyDown(ImGuiKey_3))
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

    GameObjectInfo(pObj);
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

void CInspector::GameObjectInfo(CGameObject* _pObj)
{
    if (ImGui::CollapsingHeader("GameObject", ImGuiTreeNodeFlags_DefaultOpen)) {
        static char s_nameBuf[128];
        static char s_tagBuf[128];
        static char s_typeBuf[128];
        strcpy_s(s_nameBuf, ToUtf8(_pObj->GetName()).c_str());
        strcpy_s(s_tagBuf, ToUtf8(_pObj->GetTag()).c_str());

        // Name
        ImGuiLabel("Name        ");
        if (ImGui::InputText("##Name", s_nameBuf, sizeof(s_nameBuf),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            _pObj->SetName(FromUtf8(s_nameBuf).c_str());
        }

        // Type
        ImGuiLabel("Type        ");
        if (ImGui::InputText("##Type", s_typeBuf, sizeof(s_typeBuf))) {
            _pObj->SetType(FromUtf8(s_typeBuf).c_str());
        }

        // GUID
        ImGui::Text("GUID: %llu", _pObj->GetGuid());

        // Tag
        ImGuiLabel("Tag         ");
        if (ImGui::InputText("##Tag", s_tagBuf, sizeof(s_tagBuf))) {
            _pObj->SetTag(FromUtf8(s_tagBuf).c_str());
        }

        // Layer
        ImGuiLabel("Layer       ");

        std::string preview = "";
        for (int i = 0; i < CL_END; ++i) {
            if (_pObj->Get_CollisionLayer() == i)
                preview += string(GetLayerName((COLLISION_LAYER)i));
        }

        if (ImGui::BeginCombo("##Layer", preview.c_str()))
        {
            for (int i = 0; i < CL_END; ++i)
            {
                bool selected = _pObj->Get_CollisionLayer() == i;

                // 같은 표시 이름이 있을 수 있으니 태그를 id로
                ImGui::PushID(i);
                if (ImGui::Selectable(GetLayerName((COLLISION_LAYER)i), selected))
                {
                    // 이미 선택됐을 때는 교체할 필요 없음
                    if (!selected)
                    {
                        // 레이어 교체
                        _pObj->Set_CollisionLayer((COLLISION_LAYER)i);
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

        // CullDistance
        static float s_cullDistance;
        s_cullDistance = _pObj->Get_CullDistance();
        ImGuiLabel("Cull Distance");

        if (ImGui::DragFloat("##Cull Distance", &s_cullDistance, 1.f, 0, FLT_MAX))
            _pObj->Set_CullDistance(s_cullDistance);
    }
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

    if (ImGui::BeginPopupContextItem("remove"))  // 직전 아이템 = 헤더
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
            if (rec.proto->Get_Kind() != CK_MESH || rec.proto == nullptr)
                continue;
            if (pBuf && typeid(*pBuf) == typeid(*rec.proto)) {
                preview = ToUtf8(rec.name);
                break;
            }
        }

        ImGuiLabel("Mesh");
        if (ImGui::BeginCombo("##Mesh", preview.c_str()))
        {
            for (auto& proto : prototypes)
            {
                const ProtoRecord& rec = proto.second;
                if (rec.proto->Get_Kind() != CK_MESH)
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
        SplineCom(_pObj);
    }

    ImGui::PopID();
}

void CInspector::SplineCom(CGameObject* _pObj)
{
    CSpline* pSpline = _pObj->Get_Component<CSpline>();
    if (pSpline) {
        float availX = ImGui::GetContentRegionAvail().x;
        float btnW = availX * 0.4f;

        const bool bEdit = pSpline->Get_Edit();
       
        if (bEdit)
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
        ImGui::SetCursorPosX((availX - btnW * 2) * 0.5f);
        if (ImGui::Button("Edit", ImVec2(btnW, 0)))
        {
            pSpline->Set_Edit(!bEdit);
            g_bEdit = !bEdit;
        }
        if (bEdit)
            ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::SetNextItemWidth(btnW);
        if (ImGui::Button("Add", ImVec2(btnW, 0))) {
            pSpline->Add_Point();
        }
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
            | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        bool open = ImGui::TreeNodeEx(
            (void*)(uintptr_t)pSpline,
            flags,
            "Control Points");
        if (open) {
            ImGui::Indent();
            char buf[256];
            auto& vecP = pSpline->Get_ControlPoints();
            for(int i=0; i< vecP.size(); ++i){
                ControlPoint& cp = vecP[i];
                bool openPoint = ImGui::TreeNodeEx(
                    (void*)cp.id, flags, "%d", i
                );
                ImGui::PushID(cp.id);
                if (ImGui::BeginPopupContextItem("ControlPoint_Menu")) {
                    if (ImGui::Selectable("Delete")) {
                        pSpline->Del_Point(&cp);
                        i -= 1;
                        ImGui::EndPopup();
                        ImGui::PopID();
                        continue;
                    }
                    ImGui::EndPopup();
                }
                ImGui::PopID();

                if (openPoint) {
                    float tr[3];
                    tr[0] = cp.position.x, tr[1] = cp.position.y, tr[2] = cp.position.z;
                    float bank = cp.bank, width = cp.width, depth = cp.depth;
                    
                    sprintf_s(buf, sizeof(buf), "##pos%d", cp.id);
                    ImGuiLabel("Pos");
                    if (ImGui::DragFloat3(buf, tr, 0.1f)) {
                        // CP의 위치가 바뀐다 -> 곡선의 형태가 바뀐다 
                        // -> T,R,U가 바뀐다 -> 메쉬도 전부 다시 생성
                        // 최적화 시에는 +- 2 범위의 곡선만 다시 계산
                        memcpy(&cp.position, tr, sizeof(_vec3));
                        pSpline->Compute_Spline();
                    }
                    
                    sprintf_s(buf, sizeof(buf), "##bank%d", cp.id);
                    ImGuiLabel("Bank");
                    if (ImGui::DragFloat(buf, &bank, 0.5f, -180.f, 180.f)) {
                        // 해당 cp의 right로 up과 bank 계산
                        // R,U,bank만 바뀜 -> 메쉬 다시 생성
                        // 최적화 시에는 이전/다음 cp 사이의 매쉬만 수정
                        pSpline->Set_Bank(&cp, bank);
                    }
                    
                    sprintf_s(buf, sizeof(buf), "T : %f, %f, %f", cp.T.x, cp.T.y, cp.T.z);
                    ImGui::Text(buf);
                    sprintf_s(buf, sizeof(buf), "R : %f, %f, %f", cp.R.x, cp.R.y, cp.R.z);
                    ImGui::Text(buf);
                    sprintf_s(buf, sizeof(buf), "U : %f, %f, %f", cp.U.x, cp.U.y, cp.U.z);
                    ImGui::Text(buf);

                    sprintf_s(buf, sizeof(buf), "##width%d", cp.id);
                    ImGuiLabel("Width");
                    if (ImGui::DragFloat(buf, &width, 0.1f, 0.f, FLT_MAX)) {
                        // 해당 cp의 width를 바꾼다
                        // -> 매쉬 다시 생성
                        // 최적화 시에는 이전/다음 cp 사이의 매쉬만 수정
                        cp.width = width;
                        pSpline->Compute_Mesh();
                    }
                    
                    sprintf_s(buf, sizeof(buf), "##depth%d", cp.id);
                    ImGuiLabel("Depth");
                    if (ImGui::DragFloat(buf, &depth, 0.1f, 0.f, FLT_MAX)) {
                        // 해당 cp의 width를 바꾼다
                        // -> 매쉬 다시 생성
                        // 최적화 시에는 이전/다음 cp 사이의 매쉬만 수정
                        cp.depth = depth;
                        pSpline->Compute_Mesh();
                    }
                }
                ImGui::Separator();
            }
            ImGui::Unindent();
        }
        ImGui::Separator();
        float fSampleUnit = pSpline->Get_SampleUnit();
        ImGuiLabel("Sample Unit");
        if (ImGui::DragFloat("##Sample Unit", &fSampleUnit, 0.25f, 1.f, FLT_MAX))
        {
            pSpline->Set_SampleUnit(fSampleUnit);
        }
        float fTextureUnit = pSpline->Get_TextureUnit();
        ImGuiLabel("Texture Unit");
        if (ImGui::DragFloat("##Texture Unit", &fTextureUnit, 0.25f, 1.f, FLT_MAX))
        {
            pSpline->Set_TextureUnit(fTextureUnit);
        }
    }
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
            if (rec.proto->Get_Kind() != CK_TEXTURE || rec.proto == nullptr)
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
                if (rec.proto->Get_Kind() != CK_TEXTURE)
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
    auto kindName = [](COMPONENTKIND k) -> const char* {
        switch (k) {
        case CK_MESH:     return "Mesh";
        case CK_COLLIDER: return "Collider";
        case CK_TEXTURE:  return "Texture";
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
        const COMPONENTKIND kinds[] = {
            CK_MESH, CK_COLLIDER, CK_TEXTURE
        };

        for (COMPONENTKIND kind : kinds)
        {
            if (!ImGui::BeginMenu(kindName(kind)))
                continue;

            for (auto& proto : prototypes)
            {
                const ProtoRecord& rec = proto.second;
                if (!rec.addable || rec.proto->Get_Kind() != kind)
                    continue;

                std::string label = ToUtf8(rec.name[0] ? rec.name : rec.tag);
                if (ImGui::MenuItem(label.c_str()))
                {
                    CComponent* pCom = _pObj->Add_Component(rec.tag, rec.tag);
                    if (CSpline* pSpline = dynamic_cast<CSpline*>(pCom))
                        pSpline->Create_New();
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