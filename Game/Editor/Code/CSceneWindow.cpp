#include "pch.h"
#include "CSceneWindow.h"
#include "CManagement.h"
#include "CCalculator.h"
#include "CCollider.h"
#include "CSpline.h"
#include "CRenderer.h"
#include "CHeightMap.h"

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
    static _vec3 s_vEye = { -4.5f, 6.f, -5.f };
    static float s_yaw = 0.74f, s_pitch = -0.74f;
    static _vec3 s_vUp = { 0.f, 1.f, 0.f };
    _vec3 vLook = { cosf(s_pitch) * sinf(s_yaw), sinf(s_pitch), cosf(s_pitch) * cosf(s_yaw) };
    _vec3 vAt = s_vEye + vLook;

    if (g_bMoveTo) {
        CGameObject* pSel = FindByGuid(g_uSelected, map);
        if (pSel) {
            pSel->Get_Transform()->Get_Info(INFO_POS, &vAt);
            s_vEye = vAt - 5 * vLook;
        }
        g_bMoveTo = false;
    }

    _matrix matView, matInvView;
    D3DXMatrixLookAtLH(&matView, &s_vEye, &vAt, &s_vUp);
    D3DXMatrixInverse(&matInvView, 0, &matView);

    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
    ImGui::Begin("Scene");

    bool sceneFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);
   
    static float s_gizmoSize = 0.1f;
    static float s_moveSpeed = 0.2f;

    if (sceneFocused) {
        if (ImGui::IsKeyDown(ImGuiKey_1))
            g_GizmoOp = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyDown(ImGuiKey_2))
            g_GizmoOp = ImGuizmo::ROTATE;
        if (ImGui::IsKeyDown(ImGuiKey_3))
            g_GizmoOp = ImGuizmo::SCALE;

        _vec3 vRight, vUp;
        memcpy(&vRight, &matInvView.m[0], sizeof(_vec3));
        memcpy(&vUp, &matInvView.m[1], sizeof(_vec3));
        if (ImGui::IsKeyDown(ImGuiKey_W))
        {
            s_vEye += s_moveSpeed * vUp;
        }
        if (ImGui::IsKeyDown(ImGuiKey_A))
        {
            s_vEye -= s_moveSpeed * vRight;
        }
        if (ImGui::IsKeyDown(ImGuiKey_S)) {
            s_vEye -= s_moveSpeed * vUp;
        }
        if (ImGui::IsKeyDown(ImGuiKey_D)) {
            s_vEye += s_moveSpeed * vRight;
        }
        if (ImGui::IsKeyDown(ImGuiKey_Q)) {
            s_vEye += s_moveSpeed * vLook;
        }
        if (ImGui::IsKeyDown(ImGuiKey_E)) {
            s_vEye -= s_moveSpeed * vLook;
        }
    }

    if (ImGui::RadioButton("Translate", g_GizmoOp == ImGuizmo::TRANSLATE))
        g_GizmoOp = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", g_GizmoOp == ImGuizmo::ROTATE)) 
        g_GizmoOp = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", g_GizmoOp == ImGuizmo::SCALE)) g_GizmoOp = ImGuizmo::SCALE;
    ImGui::SameLine();

    ImGui::Text("| Gizmo Size");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.3f);
    ImGui::SliderFloat("##Gizmo Size", &s_gizmoSize, 0.1f, 0.25f);
    ImGui::SameLine();
    ImGui::Text("| Move Speed");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##Move Speed", &s_moveSpeed, 0.1f, 1.f);

    ImVec2 viewPos = ImGui::GetCursorScreenPos(); // content 영역의 좌상단
    ImVec2 viewSize = ImGui::GetContentRegionAvail(); // content 영역의 크기

    // 투영 행렬 세팅
    _matrix matProj, matInvProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.f), viewSize.x / viewSize.y, 0.1f, 1000.f);
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

        CRenderer::GetInstance()->Render_GameObject(m_pGraphicDev);

        // 오브젝트 목록을 그림.
        for (auto& p : map) {
            for (auto& pObj : p.second) {
                if (pObj == nullptr)
                    continue;

                //pObj->Render_GameObject();

                if (g_bSelected && (pObj->GetGuid() == g_uSelected)) {
                    if (!g_bEdit) {
                        Draw_Outline(pObj, D3DXCOLOR{ 0.5f, 0.5f, 0.5f, 1.f });
                        Draw_Collider(pObj);
                    }
                    Control_Spline(pObj);
                }
            }
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
        static bool s_bWasUsingTranslate = false;
        if (pSel && !g_bEdit) {
            s_bWasUsingTranslate = false;
            // 조작용 복사본 (Get_World()가 돌려주는 버퍼를 직접 깨지 않게)
            _matrix matWorld = *pSel->Get_Transform()->Get_World();

            // Manipulate는 월드 행렬을 적용하여 기즈모를 보여주고
            // 기즈모의 조작으로 해당 월드 행렬을 바로 수정한다.
            // 기즈모의 위치는 부모까지 고려된 월드 행렬

            ImGuizmo::SetGizmoSizeClipSpace(s_gizmoSize);
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
                    D3DXMatrixInverse(&matInvParent, 0, &matParent);
                    // local * parent = world  →  local = world * inverse(parent)
                    matLocal = matWorld * matInvParent;
                }

                CTransform* pTF = pSel->Get_Transform();
                pTF->Set_LocalWorld(&matLocal);
            }
        }
        else if (pSel && g_bEdit) {
            if (g_bPointSelected && g_uPointSelected != 0) {
                CSpline* pSpline = pSel->Get_Component<CSpline>();

                ControlPoint* cp = pSpline->Get_ControlPoint(g_uPointSelected);
                
                _vec3 R = cp->R;
                _vec3 U = cp->U;
                ImGuizmo::OPERATION op = g_GizmoOp;
                if (g_GizmoOp == ImGuizmo::ROTATE) {
                    op = ImGuizmo::ROTATE_Z;
                }
                else if (g_GizmoOp == ImGuizmo::SCALE) {
                    op = ImGuizmo::SCALE_X | ImGuizmo::SCALE_Y;
                    R *= cp->width;
                    U *= cp->depth;
                }

                _matrix matCP, matObj;
                D3DXMatrixIdentity(&matCP);
                memcpy(&matCP.m[0], &R, sizeof(_vec3));
                memcpy(&matCP.m[1], &U, sizeof(_vec3));
                memcpy(&matCP.m[2], &cp->T, sizeof(_vec3));
                memcpy(&matCP.m[3], &cp->position, sizeof(_vec3));

                matObj = *pSel->Get_Transform()->Get_World();
                matCP *= matObj;

                ImGuizmo::SetGizmoSizeClipSpace(s_gizmoSize);
                
                ImGuizmo::Manipulate(
                    (float*)matView, (float*)matProj,
                    op, ImGuizmo::WORLD, (float*)&matCP);

                if (ImGuizmo::IsUsing())
                {
                    // 기즈모의 조작은 로컬에 적용되야 하므로 오브젝트의 영향을 다시 없앤다.
       
                    _matrix matInvObj;
                    D3DXMatrixInverse(&matInvObj, 0, &matObj);
                    matCP *= matInvObj;
      
                    if (op == ImGuizmo::TRANSLATE) {
                        memcpy(&cp->position, &matCP.m[3], sizeof(_vec3));
                        s_bWasUsingTranslate = true;
                    }
                    else if (op == ImGuizmo::ROTATE_Z) {
                        // 해당 cp의 right로 up과 bank 계산
                        // R,U,bank만 바뀜 -> 메쉬 다시 생성
                        // 최적화 시에는 이전/다음 cp 사이의 매쉬만 수정
                        _vec3 vRight;
                        memcpy(&vRight, &matCP.m[0], sizeof(_vec3));
                        pSpline->Set_BankByRight(cp, vRight);
                    }
                    else if (op == (ImGuizmo::SCALE_X | ImGuizmo::SCALE_Y)) {
                        // 해당 cp의 width/depth를 바꾼다
                        // -> 매쉬 다시 생성
                        // 최적화 시에는 이전/다음 cp 사이의 매쉬만 수정
                        _vec3 vRight, vUp;
                        memcpy(&vRight, &matCP.m[0], sizeof(_vec3));
                        memcpy(&vUp, &matCP.m[1], sizeof(_vec3));
                        pSpline->Set_WidthDepth(cp, vRight, vUp);
                    }
                }
                else if (s_bWasUsingTranslate) {
                    s_bWasUsingTranslate = false;
                    // CP의 위치가 바뀐다 -> 곡선의 형태가 바뀐다 
                    // -> T,R,U가 바뀐다 -> 메쉬도 전부 다시 생성
                    // 최적화 시에는 +- 2 범위의 곡선만 다시 계산
                    pSpline->Compute_Spline();
                }
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
            D3DXVec3Normalize(&rayDir, &rayDir);

            float bestDist = FLT_MAX;
            uint32_t bestId = 0;
            bool hit = false;

            if (!g_bEdit) {
                // for문으로 모든 오브젝트 탐색
                DirectX::BoundingBox box;

                for (auto& p : map)
                {
                    for (auto& pObj : p.second)
                    {
                        if (!pObj)
                            continue;

                        CVIBuffer* pBuf = pObj->Get_Component<CVIBuffer>();
                        if (pBuf == nullptr)
                            continue;
                        box = *pBuf->GetBoundingBox();

                        _matrix matWorld = *pObj->Get_Transform()->Get_World();
                        _matrix matInv;
                        D3DXMatrixInverse(&matInv, 0, &matWorld);
                        _vec3 oLocal, dLocal;
                        D3DXVec3TransformCoord(&oLocal, &rayOrigin, &matInv);
                        D3DXVec3TransformNormal(&dLocal, &rayDir, &matInv);
                        D3DXVec3Normalize(&dLocal, &dLocal);

                        float dist;
                        if (box.Intersects(ToXMVec(oLocal), ToXMVec(dLocal), dist) && dist < bestDist)
                        {
                            bestDist = dist;
                            bestId = pObj->GetGuid();
                            hit = true;
                        }
                    }
                }
                if (hit) {
                    ::Set_ObjSelected(bestId);
                }
                else {
                    ::Free_ObjSelected();
                }
            }
            else {
                // Spline의 점을 순회
                CGameObject* pSel = FindByGuid(g_uSelected, map);
                if (pSel != nullptr) {
                    // Spline 점 피킹
                    if (CSpline* pSpline = pSel->Get_Component<CSpline>()) {
                        DirectX::BoundingSphere sphere;
                        sphere.Radius = 0.15f;

                        auto& vecP = pSpline->Get_ControlPoints();
                        for (int i = 0; i < vecP.size(); ++i) {
                            ControlPoint& cp = vecP[i];
                            _vec3 vPointPos = cp.position;
                            D3DXVec3TransformCoord(&vPointPos, &vPointPos, pSel->Get_Transform()->Get_World());
                            sphere.Center = ToXMFLOAT3(vPointPos);

                            float dist;
                            if (sphere.Intersects(ToXMVec(rayOrigin), ToXMVec(rayDir), dist) && dist < bestDist) {
                                bestDist = dist;
                                bestId = cp.id;
                                hit = true;
                            }
                        }
                        if (hit) {
                            ::Set_PointSelected(bestId);
                        }
                        else {
                            ::Free_PointSelected();
                        }
                    }
                }
            }
        }


        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            // 카메라 회전 (세로 : Pitch, 가로 : Yaw)
            s_yaw += io.MouseDelta.x / 150.f;
            s_pitch += -io.MouseDelta.y / 150.f;
            s_pitch = clampT(s_pitch, -1.5f, 1.5f);
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
        {
            // 카메라 이동 (가로 : Right, 세로 : Up)
            _vec3 vRight, vUp;
            memcpy(&vRight, &matInvView.m[0], sizeof(_vec3));
            memcpy(&vUp, &matInvView.m[1], sizeof(_vec3));
            s_vEye += -io.MouseDelta.x / 100.f * vRight;
            s_vEye += io.MouseDelta.y / 100.f * vUp;
        }

        if (ImGui::GetIO().MouseWheel != 0.f)
        {
            // 카메라 줌인 줌아웃 (Look으로 거리 조정)
            _vec3 vLook;
            memcpy(&vLook, &matInvView.m[2], sizeof(_vec3));
            s_vEye += io.MouseWheel * vLook;
        }
    }

    ImGui::End();
}

void CSceneWindow::Draw_Outline(CGameObject* pObj, D3DXCOLOR color)
{
    CVIBuffer* buffer = pObj->Get_Component<CVIBuffer>();
    DirectX::BoundingBox bbox;
    if (buffer != nullptr)
        bbox = *buffer->GetBoundingBox();
    else
        bbox = *m_pCubeBuffer->GetBoundingBox();
    
    _matrix* matWorld = pObj->Get_Transform()->Get_World();

    DirectX::XMFLOAT3 xmfCorners[8];

    bbox.GetCorners(xmfCorners);
    _vec3 vCorners[8];

    for (int i = 0; i < 8; ++i) {
        vCorners[i] = ToVec3(xmfCorners[i]);
        D3DXVec3TransformCoord(&vCorners[i], &vCorners[i], matWorld);
    }

    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[0], vCorners[1], color);
    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[1], vCorners[2], color);
    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[2], vCorners[3], color);
    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[3], vCorners[0], color);

    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[4], vCorners[5], color);
    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[5], vCorners[6], color);
    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[6], vCorners[7], color);
    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[7], vCorners[4], color);

    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[0], vCorners[4], color);
    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[1], vCorners[5], color);
    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[2], vCorners[6], color);
    CCalculator::DrawRayLine(m_pGraphicDev, vCorners[3], vCorners[7], color);
}

void CSceneWindow::Draw_Collider(CGameObject* pObj)
{
    auto colliders = pObj->Get_Components<CCollider>();

    for (auto& col : colliders) {
        col->Render_Component(D3DXCOLOR(0, 1, 0, 1));
    }
}

void CSceneWindow::Control_Spline(CGameObject* pObj)
{
    CSpline* pSpline = pObj->Get_Component<CSpline>();
    if (!pSpline)
        return;

    pSpline->Render_Points();
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