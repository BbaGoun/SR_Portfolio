#include "pch.h"
#include "CSceneWindow.h"
#include "CManagement.h"

CSceneWindow::CSceneWindow() : CWindow()
, m_pSceneTex(nullptr)
, m_pSceneDepth(nullptr)
, m_rtW(0), m_rtH(0)
{
}

CSceneWindow::CSceneWindow(LPDIRECT3DDEVICE9 pGraphicDev) : CWindow(pGraphicDev)
, m_pSceneTex(nullptr)
, m_pSceneDepth(nullptr)
, m_rtW(0), m_rtH(0)
{
}

CSceneWindow::CSceneWindow(const CSceneWindow& rhs) : CWindow(rhs)
, m_pSceneTex(rhs.m_pSceneTex)
, m_pSceneDepth(rhs.m_pSceneDepth)
, m_rtW(rhs.m_rtW), m_rtH(rhs.m_rtH)
{
}

CSceneWindow::~CSceneWindow()
{
}

void CSceneWindow::Update_Window()
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));
    ImGuiIO& io = ImGui::GetIO();

    // 오브젝트 목록 가져오기
    const auto& map = CManagement::GetInstance()->Get_GameObjects(L"Default");

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
    ImGui::Begin("Scene");

    bool sceneFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);

    if (sceneFocused) {
        if (ImGui::IsKeyDown(ImGuiKey_Q))
            g_GizmoOp = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyDown(ImGuiKey_W))
            g_GizmoOp = ImGuizmo::ROTATE;
        if (ImGui::IsKeyDown(ImGuiKey_E))
            g_GizmoOp = ImGuizmo::SCALE;
    }

    if (ImGui::RadioButton("Translate", g_GizmoOp == ImGuizmo::TRANSLATE))
        g_GizmoOp = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", g_GizmoOp == ImGuizmo::ROTATE)) 
        g_GizmoOp = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", g_GizmoOp == ImGuizmo::SCALE)) g_GizmoOp = ImGuizmo::SCALE;


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

        // 오브젝트 목록을 그림.
        for (auto& p : map) {
            if (p.second == nullptr)
                continue;

            m_pGraphicDev->SetTransform(D3DTS_WORLD, p.second->Get_Transform()->Get_World());
            if (p.second->Get_Component(ID_STATIC, L"Com_Buffer") != nullptr)
                p.second->Render_GameObject();
            else
                m_pCubeBuffer->Render_Buffer();
        }

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

    // ImGuizmo는 조작 상태(mbUsing 등)를 전역 Context + ID로 관리합니다.
    // 공식 예제도 여러 오브젝트일 때 반드시 PushID / PopID를 씁니다.

    // 오브젝트의 guid로 선택
    if (g_bSelected) {
        CGameObject* pSel = FindByGuid(g_uSelected, map);
        if (pSel) {
            // 조작용 복사본 (Get_World()가 돌려주는 버퍼를 직접 깨지 않게)
            _matrix matWorld = *pSel->Get_Transform()->Get_World();

            // Manipulate는 월드 행렬을 적용하여 기즈모를 보여주고
            // 기즈모의 조작으로 해당 월드 행렬을 바로 수정한다.
            // 기즈모의 위치는 부모까지 고려된 월드 행렬
            ImGuizmo::Manipulate(
                (float*)matView, (float*)matProj,
                g_GizmoOp, g_GizmoMode, (float*)&matWorld);

            if (ImGuizmo::IsUsing())
            {
                // 기즈모의 조작은 로컬에 적용되야 하므로 부모의 영향을 다시 없앤다.
                _matrix matLocal = matWorld;
                if (CGameObject* pParent = pSel->Get_Parent())
                {
                    _matrix matParent = *pParent->Get_Transform()->Get_World();
                    _matrix matInvParent;
                    D3DXMatrixInverse(&matInvParent, nullptr, &matParent);
                    // local * parent = world  →  local = world * inverse(parent)
                    matLocal = matWorld * matInvParent;
                }

                CTransform* pTF = pSel->Get_Transform();
                pTF->Set_LocalWorld(&matLocal);
            }
        }
    }

    ImVec2 mouse = ImGui::GetMousePos();
    ImVec2 local(mouse.x - viewPos.x, mouse.y - viewPos.y);

    const bool windowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_None);
    const bool insideRect = ImGui::IsMouseHoveringRect(
        viewPos,
        ImVec2(viewPos.x + viewSize.x, viewPos.y + viewSize.y),
        false);
    const bool viewHovered = windowHovered && insideRect;

    const bool usingGizmo = ImGuizmo::IsUsing();
    const bool overGizmo = ImGuizmo::IsOver();

    if (viewHovered && !usingGizmo) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !overGizmo)
        {
            bool isPicked = false;

            // 오브젝트 픽킹 → selected 갱신
            // 뷰 포트 -> 투영
            _vec3 pickPos = { 0, 0, 1 };
            pickPos.x = (local.x - viewSize.x / 2.f) / (viewSize.x / 2.f);
            pickPos.y = -(local.y - viewSize.y / 2.f) / (viewSize.y / 2.f);

            // 투영 -> 뷰
            D3DXVec3TransformCoord(&pickPos, &pickPos, &matInvProj);

            // 뷰에서 레이저 생성
            _vec3 rayOrigin = { 0, 0, 0 };
            _vec3 rayDir = pickPos;

            // ray를 뷰 -> 월드
            D3DXVec3TransformCoord(&rayOrigin, &rayOrigin, &matInvView);
            D3DXVec3TransformNormal(&rayDir, &rayDir, &matInvView);

            // for문으로 모든 오브젝트 탐색
            float bestDist = FLT_MAX;
            uint64_t bestGuid = 0;
            bool hit = false;
            DirectX::BoundingBox box;

            for (auto& p : map)
            {
                if (!p.second) 
                    continue;
                
                _matrix matWorld = *p.second->Get_Transform()->Get_World();
                _matrix matInv;
                D3DXMatrixInverse(&matInv, 0, &matWorld);
                _vec3 oLocal, dLocal;
                D3DXVec3TransformCoord(&oLocal, &rayOrigin, &matInv);
                D3DXVec3TransformNormal(&dLocal, &rayDir, &matInv);
                D3DXVec3Normalize(&dLocal, &dLocal);

                CComponent* pBuf = p.second->Get_Component(ID_STATIC, L"Com_Buffer");
                if (pBuf == nullptr)
                    m_pCubeBuffer->GetBoundingBox(&box);
                else {
                    static_cast<CVIBuffer*>(pBuf)->GetBoundingBox(&box);
                }

                float dist;
                if (box.Intersects(ToXMVec(oLocal), ToXMVec(dLocal), dist) && dist < bestDist)
                {
                    bestDist = dist;
                    bestGuid = p.second->GetGuid();
                    hit = true;
                }
            }
            if (hit) {
                g_bSelected = true;
                g_uSelected = bestGuid;
            }
            else {
                g_bSelected = false;
                g_uSelected = 0;
            }
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            // 카메라 회전 (세로 : Pitch, 가로 : Yaw)
            yaw += io.MouseDelta.x / 150.f;
            pitch += -io.MouseDelta.y / 150.f;
            pitch = clampT(pitch, -1.5f, 1.5f);
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
        {
            // 카메라 이동 (가로 : Right, 세로 : Up)
            _vec3 vRight, vUp;
            memcpy(&vRight, &matInvView.m[0], sizeof(_vec3));
            memcpy(&vUp, &matInvView.m[1], sizeof(_vec3));
            vEye += -io.MouseDelta.x / 100.f * vRight;
            vEye += io.MouseDelta.y / 100.f * vUp;
        }

        if (ImGui::GetIO().MouseWheel != 0.f)
        {
            // 카메라 줌인 줌아웃 (Look으로 거리 조정)
            _vec3 vLook;
            memcpy(&vLook, &matInvView.m[2], sizeof(_vec3));
            vEye += io.MouseWheel * vLook;
        }
    }

    ImGui::End();
}

void CSceneWindow::InvalidateDeviceObjects()
{
    Safe_Release(m_pSceneTex);
    Safe_Release(m_pSceneDepth);
    m_rtW = 0; m_rtH = 0;
}

HRESULT CSceneWindow::Ready_Window()
{
    m_pCubeBuffer = Engine::CCube::Create(m_pGraphicDev);

    return S_OK;
}

CWindow* CSceneWindow::Clone()
{
    CWindow* pWindow = new CSceneWindow(*this);
    return pWindow;
}

CSceneWindow* CSceneWindow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSceneWindow* pWindow = new CSceneWindow(pGraphicDev);

    if (FAILED(pWindow->Ready_Window()))
    {
        MSG_BOX("CSceneWindow Create Failed");
        Safe_Release(pWindow);
        return nullptr;
    }

    return pWindow;
}

void CSceneWindow::Free()
{
    InvalidateDeviceObjects();
    CWindow::Free();
}