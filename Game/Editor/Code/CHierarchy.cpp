#include "pch.h"
#include "CHierarchy.h"
#include "CEmpty.h"
#include "CManagement.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CCube_Collider.h"
#include "CSphere_Collider.h"
#include "CTransform.h"

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
        ::Free_ObjSelected();
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

    const char* icon = "\xE2\x97\x8F"; // ●
    if (pObj->Get_PrefabPath()[0] != L'\0')
        icon = "\xE2\x96\xA0"; // ■
    else if (pObj->Get_Belong())
        icon = "\xE2\x96\xA1"; // □

    if(pObj->Get_Belong())
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.45f, 0.72f, 1.0f, 1.0f)); // 유니티 프리팹에 가까운 하늘색

    // 전달하는 Guid를 ID로 하여 노드가 분리됨.
    bool open = ImGui::TreeNodeEx(
        (void*)(uintptr_t)pObj->GetGuid(),
        flags,
        "%s %s", icon, bRenaming ? "" : label.c_str());

    if (pObj->Get_Belong())
        ImGui::PopStyleColor();

    if (!bRenaming && ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        ::Set_ObjSelected(pObj->GetGuid());
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
            if (ImGui::Selectable("Save Prefab")) {
                OnSave(pObj, false);
            }
            if (ImGui::Selectable("Save Prefab As")) {
                OnSave(pObj, true);
            }
            if (ImGui::Selectable("Create Empty Child")) {
                CGameObject* pChild = CEmpty::Create(m_pGraphicDev);
                pChild->SetName(L"Empty");
                uint64_t guid = CManagement::GetInstance()->GenerateGuid();
                pChild->SetGuid(guid);

                wstring s = std::to_wstring(guid);
                CManagement::GetInstance()->Add_GameObject(L"Default", s.c_str(), pChild);
                ::Set_ObjSelected(guid);

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
                CManagement::GetInstance()->Delete_GameObject(L"Default", pObj, true);
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
            ::Set_ObjSelected(guid);
        }
        if (ImGui::Selectable("Load Prefab")) {
            OnLoad();
        }
        ImGui::EndPopup();
    }
}

void CHierarchy::OnLoad()
{
    _tchar prefabPath[MAX_PATH] = L"\0";

    if (!OpenLoadPrefabDialog(prefabPath, MAX_PATH))
        return;  // 취소

    CreatePrefabFromFile(prefabPath);
}

bool CHierarchy::OpenLoadPrefabDialog(_tchar* outPath, DWORD outChars)
{
    wchar_t fileBuf[MAX_PATH] = {};
    wchar_t initialDir[MAX_PATH] = {};
    GetFullPathNameW(L"../../../Resource/Editor/Prefab", MAX_PATH, initialDir, nullptr);
    CreateDirectoryW(initialDir, nullptr);  // 없으면 만들기

    swprintf_s(fileBuf, L"%s\\.prefab", initialDir);

    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = g_hWnd;
    ofn.lpstrFile = fileBuf;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"Prefab (*.prefab)\0*.prefab\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = L"prefab";
    ofn.lpstrInitialDir = initialDir;
    ofn.lpstrTitle = L"Open Prefab";
    ofn.Flags = OFN_FILEMUSTEXIST   // 없는 파일은 선택 불가
        | OFN_PATHMUSTEXIST
        | OFN_NOCHANGEDIR;

    if (!GetOpenFileNameW(&ofn))
        return false;

    if (!ToRelFromCwd(fileBuf, outPath, outChars))
        wcscpy_s(outPath, outChars, fileBuf);  // 다른 드라이브면 절대경로 유지

    return true;
}

void CHierarchy::CreatePrefabFromFile(const wchar_t* path)
{
    if (path == nullptr || path[0] == L'\0')
        return;

    FILE* fp = nullptr;
    if (_wfopen_s(&fp, path, L"r, ccs=UTF-8") != 0 || !fp)
        return;

    FileReadState st;
    st.fp = fp;

    wchar_t* t = nullptr;
    CGameObject* pRoot = nullptr;
    if (st.Next(t) && !wcscmp(t, L"OBJECT"))
        pRoot = LoadPrefabObject(st, m_pGraphicDev, nullptr, true, path);

    fclose(fp);

    if (pRoot == nullptr)
        return;

    ::Set_ObjSelected(pRoot->GetGuid());
    CManagement::GetInstance()->Set_SceneDirty(true);
}

CGameObject* CHierarchy::LoadPrefabObject(FileReadState& st, LPDIRECT3DDEVICE9 pGraphicDev,
    CGameObject* pParent, bool bRoot, const wchar_t* prefabPath)
{
    CGameObject* pObj = CEmpty::Create(pGraphicDev);
    if (pObj == nullptr)
        return nullptr;

    const uint32_t guid = CManagement::GetInstance()->GenerateGuid();
    pObj->SetGuid(guid);

    wstring key = to_wstring(guid);
    CManagement::GetInstance()->Add_GameObject(L"Default", key.c_str(), pObj);
    if (pParent)
        pParent->Set_Child(pObj);

    wchar_t* t = nullptr;
    while (st.Next(t))
    {
        if (!wcscmp(t, L"ENDOBJECT"))
            break;

        if (!wcscmp(t, L"OBJECT"))
        {
            LoadPrefabObject(st, pGraphicDev, pObj, false, prefabPath);
            continue;
        }

        if (StartsWith(t, L"name="))
            pObj->SetName(t + 5);
        else if (StartsWith(t, L"type="))
            pObj->SetType(t + 5);
        else if (StartsWith(t, L"tag="))
            pObj->SetTag(t + 4);
        else if (StartsWith(t, L"belong="))
            pObj->Set_Belong(_wtoi(t + 7) != 0);
        else if (StartsWith(t, L"collisionLayer="))
            pObj->Set_CollisionLayer((COLLISION_LAYER)_wtoi(t + 15));
        else if (StartsWith(t, L"cullDistance="))
            pObj->Set_CullDistance(_wtoi(t + 13));
        else if (StartsWith(t, L"COMP "))
        {
            wchar_t kind[64] = {};
            wchar_t proto[256] = {};
            wchar_t mapTag[256] = {};
            ExtractCompField(t, L"kind", kind, 64);
            ExtractCompField(t, L"proto", proto, 256);
            ExtractCompField(t, L"mapTag", mapTag, 256);

            CComponent* pCom = nullptr;
            if (!wcscmp(kind, L"Transform"))
            {
                pCom = pObj->Get_Transform();
            }
            else
            {
                const WCHAR* interned = InternProtoTag(proto);
                if (interned == nullptr)
                    interned = InternProtoTag(mapTag);
                if (interned)
                {
                    pObj->Add_Component(interned, interned);
                    for (auto& pairCom : pObj->Get_ComponentMap())
                    {
                        if (!lstrcmp(pairCom.first, interned))
                        {
                            pCom = pairCom.second;
                            break;
                        }
                    }
                }
            }

            ApplyCompProperties(st, pObj, pCom);
        }
    }

    if (bRoot && prefabPath && prefabPath[0] != L'\0')
        pObj->Set_PrefabPath(prefabPath);

    return pObj;
}

void CHierarchy::OnSave(CGameObject* pObj, bool bSaveAs)
{
    _tchar prefabPath[MAX_PATH] = L"\0";
    wcscpy_s(prefabPath, MAX_PATH, pObj->Get_PrefabPath());
    if (bSaveAs || prefabPath[0] == L'\0')
    {
        if (!OpenSavePrefabDialog(prefabPath, MAX_PATH))
            return;  // 취소 → 여기서 끝, 파일 없음
        pObj->Set_PrefabPath(prefabPath);
    }

    BelongPrefab(pObj);
    SavePrefabFile(pObj, prefabPath);  // 대화상자가 닫힌 직후
}

bool CHierarchy::OpenSavePrefabDialog(_tchar* outPath, DWORD outChars)
{
    wchar_t fileBuf[MAX_PATH] = {};
    wchar_t initialDir[MAX_PATH] = {};
    GetFullPathNameW(L"../../../Resource/Editor/Prefab", MAX_PATH, initialDir, nullptr);
    CreateDirectoryW(initialDir, nullptr);  // 없으면 만들기

    if (outPath[0] != L'\0')
        ToAbsPath(outPath, fileBuf, MAX_PATH);  // 상대 → 절대 (대화상자용)
    else
        swprintf_s(fileBuf, L"%s\\.prefab", initialDir);

    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = g_hWnd;        // 에디터 창을 부모로
    ofn.lpstrFile = fileBuf;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"Prefab (*.prefab)\0*.prefab\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = L"prefab";      // 확장자 안 붙이면 .scene 자동
    ofn.lpstrInitialDir = (outPath[0] == L'\0') ? initialDir : nullptr;
    ofn.lpstrTitle = L"Save Prefab";
    ofn.Flags = OFN_OVERWRITEPROMPT  // 같은 이름이면 "덮어쓸까요?"
        | OFN_PATHMUSTEXIST
        | OFN_NOCHANGEDIR;     // 작업 디렉터리 안 바뀌게 (리소스 경로 보호)
    if (!GetSaveFileNameW(&ofn))
        return false;                // 취소 또는 에러

    if (!ToRelFromCwd(fileBuf, outPath, outChars))
        wcscpy_s(outPath, outChars, fileBuf);  // 다른 드라이브면 절대경로 유지

    return true;
}

void CHierarchy::SavePrefabFile(CGameObject* pObj, const wchar_t* path)
{
    if (pObj == nullptr || path == nullptr || path[0] == L'\0')
        return;

    FILE* fp = nullptr;
    if (_wfopen_s(&fp, path, L"w, ccs=UTF-8") != 0 || !fp)
        return;

    SaveGameObject(fp, pObj, 0, true);

    fclose(fp);
}

void CHierarchy::SaveGameObject(FILE* pf, CGameObject* pObj, int depth, bool bRoot)
{
    auto writeIndent = [](FILE* f, int depth)
        {
            for (int i = 0; i < depth; ++i)
                fwprintf(f, L"  ");
        };

    writeIndent(pf, depth);
    fwprintf(pf, L"OBJECT\n");

    writeIndent(pf, depth + 1);
    fwprintf(pf, L"belong=%d\n", pObj->Get_Belong() ? 1 : 0);
    writeIndent(pf, depth + 1);
    fwprintf(pf, L"type=%s\n", pObj->GetType());
    writeIndent(pf, depth + 1);
    fwprintf(pf, L"name=%s\n", pObj->GetName());
    writeIndent(pf, depth + 1);
    fwprintf(pf, L"tag=%s\n", pObj->GetTag());
    writeIndent(pf, depth + 1);
    fwprintf(pf, L"collisionLayer=%d\n", (int)pObj->Get_CollisionLayer());
    writeIndent(pf, depth + 1);
    fwprintf(pf, L"cullDistance=%d\n", pObj->Get_CullDistance());

    for (auto& pairCom : pObj->Get_ComponentMap())
    {
        CComponent* pCom = pairCom.second;
        if (pCom == nullptr)
            continue;

        const WCHAR* mapTag = pairCom.first ? pairCom.first : L"";
        const WCHAR* protoTag = pCom->Get_ProtoTag();
        if (protoTag == nullptr)
            protoTag = L"";

        auto writeCompHead = [&](const WCHAR* kind)
            {
                writeIndent(pf, depth + 1);
                fwprintf(pf, L"COMP kind=%s proto=%s mapTag=%s\n", kind, protoTag, mapTag);
            };

        switch (pCom->Get_Kind()) {
        case CK_TRANSFORM:
        {
            CTransform* pTF = static_cast<CTransform*>(pCom);

            _matrix matLocal = *pTF->Get_LocalWorld();
            float t[3], r[3], s[3];
            ImGuizmo::DecomposeMatrixToComponents((float*)&matLocal, t, r, s);
            D3DXQUATERNION q = pTF->Get_Quaternion();
            writeCompHead(L"Transform");
            writeIndent(pf, depth + 2);
            fwprintf(pf, L"pos=%f %f %f\n", t[0], t[1], t[2]);
            writeIndent(pf, depth + 2);
            fwprintf(pf, L"rot=%f %f %f\n", r[0], r[1], r[2]);
            writeIndent(pf, depth + 2);
            fwprintf(pf, L"quat=%f %f %f %f\n", q.x, q.y, q.z, q.w);
            writeIndent(pf, depth + 2);
            fwprintf(pf, L"scale=%f %f %f\n", s[0], s[1], s[2]);
        }
            break;
        case CK_MESH:
        {
            CVIBuffer* pBuf = static_cast<CVIBuffer*>(pCom);

            for (auto& proto : CProtoMgr::GetInstance()->Get_Prototypes())
            {
                const ProtoRecord& rec = proto.second;
                if (rec.proto->Get_Kind() == CK_MESH && rec.proto &&
                    typeid(*pBuf) == typeid(*rec.proto))
                {
                    protoTag = rec.tag;
                    break;
                }
            }
            writeCompHead(L"Mesh");
        }
            break;
        case CK_TEXTURE:
        {
            CTexture* pTex = static_cast<CTexture*>(pCom);
            writeCompHead(L"Texture");
        }
            break;
        case CK_COLLIDER:
        {
            CCollider* pCol = static_cast<CCube_Collider*>(pCom);
            if (pCol->GetColliderType() == CUBE_COLLIDER) {
                CCube_Collider* pBox = static_cast<CCube_Collider*>(pCom);
                
                _vec3 offset = pBox->Get_Offset();
                _vec3 extents = ToVec3(pBox->Get_Info().Extents);
                writeCompHead(L"CubeCollider");
                writeIndent(pf, depth + 2);
                fwprintf(pf, L"trigger=%d\n", pBox->GetIsTrigger() ? 1 : 0);
                writeIndent(pf, depth + 2);
                fwprintf(pf, L"offset=%f %f %f\n", offset.x, offset.y, offset.z);
                writeIndent(pf, depth + 2);
                fwprintf(pf, L"extents=%f %f %f\n", extents.x, extents.y, extents.z);
            }
            else if (pCol->GetColliderType() == SPHERE_COLLIDER) {
                CSphere_Collider* pSphere = static_cast<CSphere_Collider*>(pCom);

                _vec3 offset = pSphere->Get_Offset();
                writeCompHead(L"SphereCollider");
                writeIndent(pf, depth + 2);
                fwprintf(pf, L"trigger=%d\n", pSphere->GetIsTrigger() ? 1 : 0);
                writeIndent(pf, depth + 2);
                fwprintf(pf, L"offset=%f %f %f\n", offset.x, offset.y, offset.z);
                writeIndent(pf, depth + 2);
                fwprintf(pf, L"radius=%f\n", pSphere->Get_Info().Radius);
            }
        }
            break;
        }
    }

    for (CGameObject* pChild : pObj->Get_Children())
    {
        if (pChild)
            SaveGameObject(pf, pChild, depth + 1, false);
    }

    writeIndent(pf, depth);
    fwprintf(pf, L"ENDOBJECT\n");
}

void CHierarchy::BelongPrefab(CGameObject* _pObj)
{
    _pObj->Set_Belong(true);
    for (auto& pObj : _pObj->Get_Children())
        BelongPrefab(pObj);
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