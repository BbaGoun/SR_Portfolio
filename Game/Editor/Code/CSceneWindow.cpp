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
    _vec3 vLook = { cosf(m_pitch) * sinf(m_yaw), sinf(m_pitch), cosf(m_pitch) * cosf(m_yaw) };
    _vec3 vAt = m_vEye + vLook;

    if (g_bMoveTo) {
        CGameObject* pSel = FindByGuid(g_uSelected, map);
        if (pSel) {
            pSel->Get_Transform()->Get_Info(INFO_POS, &vAt);
            m_vEye = vAt - 5 * vLook;
        }
        g_bMoveTo = false;
    }

    D3DXMatrixLookAtLH(&m_matView, &m_vEye, &vAt, &m_vUp);
    D3DXMatrixInverse(&m_matInvView, 0, &m_matView);

    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
    ImGui::Begin("Scene");

    bool sceneFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_None);

    if (sceneFocused) {
        if (ImGui::IsKeyDown(ImGuiKey_1))
            g_GizmoOp = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyDown(ImGuiKey_2))
            g_GizmoOp = ImGuizmo::ROTATE;
        if (ImGui::IsKeyDown(ImGuiKey_3))
            g_GizmoOp = ImGuizmo::SCALE;

        _vec3 vRight, vUp;
        memcpy(&vRight, &m_matInvView.m[0], sizeof(_vec3));
        memcpy(&vUp, &m_matInvView.m[1], sizeof(_vec3));
        if (ImGui::IsKeyDown(ImGuiKey_W))
        {
            m_vEye += m_moveSpeed * vUp;
        }
        if (ImGui::IsKeyDown(ImGuiKey_A))
        {
            m_vEye -= m_moveSpeed * vRight;
        }
        if (ImGui::IsKeyDown(ImGuiKey_S)) {
            m_vEye -= m_moveSpeed * vUp;
        }
        if (ImGui::IsKeyDown(ImGuiKey_D)) {
            m_vEye += m_moveSpeed * vRight;
        }
        if (ImGui::IsKeyDown(ImGuiKey_Q)) {
            m_vEye += m_moveSpeed * vLook;
        }
        if (ImGui::IsKeyDown(ImGuiKey_E)) {
            m_vEye -= m_moveSpeed * vLook;
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
    ImGui::SliderFloat("##Gizmo Size", &m_gizmoSize, 0.1f, 0.25f);
    ImGui::SameLine();
    ImGui::Text("| Move Speed");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##Move Speed", &m_moveSpeed, 0.1f, 3.f);

    ImVec2 viewPos = ImGui::GetCursorScreenPos(); // content 영역의 좌상단
    ImVec2 viewSize = ImGui::GetContentRegionAvail(); // content 영역의 크기

    // 투영 행렬 세팅
    D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(45.f), viewSize.x / viewSize.y, 0.1f, 100000.f);
    D3DXMatrixInverse(&m_matInvProj, 0, &m_matProj);

    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(
        viewPos.x,
        viewPos.y,
        viewSize.x,
        viewSize.y);

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
        // 오브젝트 픽킹 → selected 갱신
            // 뷰 포트 -> 투영
        _vec3 pickPos = { 0, 0, 1 };
        pickPos.x = (local.x - viewSize.x / 2.f) / (viewSize.x / 2.f);
        pickPos.y = -(local.y - viewSize.y / 2.f) / (viewSize.y / 2.f);

        // 투영 -> 뷰
        D3DXVec3TransformCoord(&pickPos, &pickPos, &m_matInvProj);

        // 뷰에서 레이저 생성
        _vec3 rayOrigin = { 0, 0, 0 };
        _vec3 rayDir = pickPos;

        // ray를 뷰 -> 월드
        D3DXVec3TransformCoord(&rayOrigin, &rayOrigin, &m_matInvView);
        D3DXVec3TransformNormal(&rayDir, &rayDir, &m_matInvView);
        D3DXVec3Normalize(&rayDir, &rayDir);

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !overGizmo)
        {
            if (!g_bSplineEdit && !g_bGraphNodeEdit && g_uGraphEdgeEdit == 0) {
                Object_Pick(map, rayOrigin, rayDir);
            }
            else if(g_bSplineEdit){
                Spline_Pick(map, rayOrigin, rayDir);
            }
            else if (g_bGraphNodeEdit) {
                GraphNode_Pick(map, rayOrigin, rayDir);
            }
            else if (g_uGraphEdgeEdit) {
                GraphPoint_Pick(map, rayOrigin, rayDir);
            }
        }

        // 높이맵 조정
        if (g_bSplineEdit) {
            if (CGameObject* pSel = FindByGuid(g_uSelected, map)) {
                if (CHeightMap* pHM = pSel->Get_Component<CHeightMap>()) {
                    HeightMap_Pick(pHM, rayOrigin, rayDir);

                    if (g_bHMPick) {
                        if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
                            pHM->HeightMap_Edit(g_vHMPickPos, true);
                        }
                        else if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                            pHM->HeightMap_Edit(g_vHMPickPos, false);
                        }
                    }
                }
                else
                    Free_HMPick();
            }
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            // 카메라 회전 (세로 : Pitch, 가로 : Yaw)
            m_yaw += io.MouseDelta.x / 150.f;
            m_pitch += -io.MouseDelta.y / 150.f;
            m_pitch = clampT(m_pitch, -1.5f, 1.5f);
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
        {
            // 카메라 이동 (가로 : Right, 세로 : Up)
            _vec3 vRight, vUp;
            memcpy(&vRight, &m_matInvView.m[0], sizeof(_vec3));
            memcpy(&vUp, &m_matInvView.m[1], sizeof(_vec3));
            m_vEye += -io.MouseDelta.x / 100.f * vRight;
            m_vEye += io.MouseDelta.y / 100.f * vUp;
        }

        if (ImGui::GetIO().MouseWheel != 0.f)
        {
            // 카메라 줌인 줌아웃 (Look으로 거리 조정)
            _vec3 vLook;
            memcpy(&vLook, &m_matInvView.m[2], sizeof(_vec3));
            m_vEye += io.MouseWheel * vLook;
        }
    }

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

        m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
        m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

        CRenderer::GetInstance()->Render_GameObject(m_pGraphicDev);

        // 오브젝트 목록을 그림.
        for (auto& p : map) {
            for (auto& pObj : p.second) {
                if (pObj == nullptr)
                    continue;

                if (g_bSelected && (pObj->GetGuid() == g_uSelected)) {
                    if (!g_bSplineEdit && !g_bGraphNodeEdit && g_uGraphEdgeEdit == 0) {
                        Draw_Outline(pObj, D3DXCOLOR{ 0.5f, 0.5f, 0.5f, 1.f });
                        Draw_Collider(pObj);
                    }
                    if (g_bSplineEdit) {
                        Draw_SplinePoints(pObj);
                        Draw_HeightMap(pObj);
                    }
                    if (g_bGraphNodeEdit || g_uGraphEdgeEdit != 0) {
                        Draw_Graph(pObj);
                    }
                    Draw_GraphOBB(pObj);
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

    // ImGuizmo는 조작 상태(mbUsing 등)를 전역 Context + ID로 관리합니다.
    // 공식 예제도 여러 오브젝트일 때 반드시 PushID / PopID를 씁니다.

    // 오브젝트의 guid로 선택
    if (g_bSelected) {
        CGameObject* pSel = FindByGuid(g_uSelected, map);
        if (pSel && !g_bSplineEdit && !g_bGraphNodeEdit && g_uGraphEdgeEdit == 0) {
            Manipulate_Object(pSel);
        }
        else if (pSel && g_bSplineEdit) {
            if (g_bPointSelected && g_uPointSelected != 0)
                Manipulate_Spline(pSel);
        }
        else if (pSel && g_bGraphNodeEdit) {
            if (g_bPointSelected && g_uPointSelected != 0)
                Manipulate_GraphNode(pSel);
        }
        else if (pSel && g_uGraphEdgeEdit != 0) {
            if (g_bPointSelected && g_uPointSelected != 0)
                Manipulate_GraphPoint(pSel);
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

void CSceneWindow::Draw_SplinePoints(CGameObject* pObj)
{
    CSpline* pSpline = pObj->Get_Component<CSpline>();
    if (!pSpline)
        return;

    pSpline->Render_Points();
}

void CSceneWindow::Draw_HeightMap(CGameObject* pObj)
{
    CHeightMap* pHeightMap = pObj->Get_Component<CHeightMap>();
    if (!pHeightMap)
        return;

    //pHeightMap->Render_Points();
    if (g_bHMPick)
        pHeightMap->Render_Brush(g_vHMPickPos);
}

void CSceneWindow::Draw_Graph(CGameObject* pObj)
{
    CTrackGraph* pTGraph = pObj->Get_Component<CTrackGraph>();
    if (!pTGraph)
        return;

    pTGraph->Render_Points();
}

void CSceneWindow::Draw_GraphOBB(CGameObject* pObj)
{
    CTrackGraph* pTGraph = pObj->Get_Component<CTrackGraph>();
    if (!pTGraph)
        return;

    pTGraph->Render_Samples();
}

void CSceneWindow::Object_Pick(const map<const _tchar*, vector<CGameObject*>>& map, _vec3 worldRayOrigin, _vec3 worldRayDir)
{
    float bestDist = FLT_MAX;
    uint32_t bestId = 0;
    bool hit = false;

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
            D3DXVec3TransformCoord(&oLocal, &worldRayOrigin, &matInv);
            D3DXVec3TransformNormal(&dLocal, &worldRayDir, &matInv);
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

void CSceneWindow::Spline_Pick(const map<const _tchar*, vector<CGameObject*>>& map, _vec3 worldRayOrigin, _vec3 worldRayDir)
{
    float bestDist = FLT_MAX;
    uint32_t bestId = 0;
    bool hit = false;

    // Spline의 점을 순회
    if (CGameObject* pSel = FindByGuid(g_uSelected, map)) {
        // Spline 점 피킹
        if (CSpline* pSpline = pSel->Get_Component<CSpline>()) {
            DirectX::BoundingSphere sphere;
            sphere.Radius = 0.25f;

            auto& vecP = pSpline->Get_ControlPoints();
            for (int i = 0; i < vecP.size(); ++i) {
                ControlPoint& cp = vecP[i];
                _vec3 vPointPos = cp.position;
                D3DXVec3TransformCoord(&vPointPos, &vPointPos, pSel->Get_Transform()->Get_World());
                sphere.Center = ToXMFLOAT3(vPointPos);

                float dist;
                if (sphere.Intersects(ToXMVec(worldRayOrigin), ToXMVec(worldRayDir), dist) && dist < bestDist) {
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

void CSceneWindow::HeightMap_Pick(CHeightMap* pHM, _vec3 worldRayOrigin, _vec3 worldRayDir)
{
    vector<VTXTEX>& vertices = pHM->GetVertices();
    vector<FACE32>& faces = pHM->GetFaces();

    _matrix* pMatWorld = pHM->Get_Owner()->Get_Transform()->Get_World();
    _matrix matInvWorld;

    D3DXMatrixInverse(&matInvWorld, 0, pMatWorld);

    _vec3 oLocal, dLocal;

    D3DXVec3TransformCoord(&oLocal, &worldRayOrigin, &matInvWorld);
    D3DXVec3TransformNormal(&dLocal, &worldRayDir, &matInvWorld);

    bool bHit = false;
    _float fClosestDist = FLT_MAX;
    _vec3 vClosestPos;

    for (int i = 0; i < faces.size(); ++i) {
        VTXTEX v0, v1, v2;
        _vec3 p0, p1, p2;
        float u, v, dist;

        v0 = vertices[faces[i].indices._0];
        v1 = vertices[faces[i].indices._1];
        v2 = vertices[faces[i].indices._2];

        p0 = v0.vPosition;
        p1 = v1.vPosition;
        p2 = v2.vPosition;

        if (D3DXIntersectTri(&p0, &p1, &p2, &oLocal, &dLocal, &u, &v, &dist))
        {
            if (fClosestDist > dist) {
                bHit = true;
                fClosestDist = dist;
                vClosestPos = p0 + u * (p1 - p0) + v * (p2 - p0);
            }
        }
    }

    if (bHit) {
        Set_HMPick(vClosestPos);
    }
    else
        Free_HMPick();
}

void CSceneWindow::GraphNode_Pick(const map<const _tchar*, vector<CGameObject*>>& map, _vec3 worldRayOrigin, _vec3 worldRayDir)
{
    float bestDist = FLT_MAX;
    uint32_t bestId = 0;
    bool hit = false;

    // Spline의 점을 순회
    if (CGameObject* pSel = FindByGuid(g_uSelected, map)) {
        // Graph의 Node 점 피킹
        if (CTrackGraph* pTGraph = pSel->Get_Component<CTrackGraph>()) {
            DirectX::BoundingSphere sphere;
            sphere.Radius = 0.25f;

            auto& vecTN = pTGraph->Get_Nodes();
            for (int i = 0; i < vecTN.size(); ++i) {
                TrackNode& tn = vecTN[i];
                _vec3 vPointPos = tn.position;
                D3DXVec3TransformCoord(&vPointPos, &vPointPos, pSel->Get_Transform()->Get_World());
                sphere.Center = ToXMFLOAT3(vPointPos);

                float dist;
                if (sphere.Intersects(ToXMVec(worldRayOrigin), ToXMVec(worldRayDir), dist) && dist < bestDist) {
                    bestDist = dist;
                    bestId = tn.id;
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

void CSceneWindow::GraphPoint_Pick(const map<const _tchar*, vector<CGameObject*>>& map, _vec3 worldRayOrigin, _vec3 worldRayDir)
{
    float bestDist = FLT_MAX;
    uint32_t bestId = 0;
    bool hit = false;

    // Spline의 점을 순회
    if (CGameObject* pSel = FindByGuid(g_uSelected, map)) {
        // Graph의 Node 점 피킹
        if (CTrackGraph* pTGraph = pSel->Get_Component<CTrackGraph>()) {
            DirectX::BoundingSphere sphere;
            sphere.Radius = 0.25f;

            TrackEdge* pTE = pTGraph->Get_TrackEdge(g_uGraphEdgeEdit);
            
            auto& vecCp = pTE->deqControls;
            for (int i = 0; i < vecCp.size(); ++i) {
                ControlPoint& cp = vecCp[i];
                _vec3 vPointPos = cp.position;
                D3DXVec3TransformCoord(&vPointPos, &vPointPos, pSel->Get_Transform()->Get_World());
                sphere.Center = ToXMFLOAT3(vPointPos);

                float dist;
                if (sphere.Intersects(ToXMVec(worldRayOrigin), ToXMVec(worldRayDir), dist) && dist < bestDist) {
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

void CSceneWindow::Manipulate_Object(CGameObject* pSel)
{
    m_bWasUsingTranslate = false;
    // 조작용 복사본 (Get_World()가 돌려주는 버퍼를 직접 깨지 않게)
    _matrix matWorld = *pSel->Get_Transform()->Get_World();

    // Manipulate는 월드 행렬을 적용하여 기즈모를 보여주고
    // 기즈모의 조작으로 해당 월드 행렬을 바로 수정한다.
    // 기즈모의 위치는 부모까지 고려된 월드 행렬

    ImGuizmo::SetGizmoSizeClipSpace(m_gizmoSize);
    ImGuizmo::Manipulate(
        (float*)m_matView, (float*)m_matProj,
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

void CSceneWindow::Manipulate_Spline(CGameObject* pSel)
{
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

    ImGuizmo::SetGizmoSizeClipSpace(m_gizmoSize);

    ImGuizmo::Manipulate(
        (float*)m_matView, (float*)m_matProj,
        op, ImGuizmo::WORLD, (float*)&matCP);

    if (ImGuizmo::IsUsing())
    {
        // 기즈모의 조작은 로컬에 적용되야 하므로 오브젝트의 영향을 다시 없앤다.

        _matrix matInvObj;
        D3DXMatrixInverse(&matInvObj, 0, &matObj);
        matCP *= matInvObj;

        if (op == ImGuizmo::TRANSLATE) {
            memcpy(&cp->position, &matCP.m[3], sizeof(_vec3));
            m_bWasUsingTranslate = true;
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
    else if (m_bWasUsingTranslate) {
        m_bWasUsingTranslate = false;
        // CP의 위치가 바뀐다 -> 곡선의 형태가 바뀐다 
        // -> T,R,U가 바뀐다 -> 메쉬도 전부 다시 생성
        // 최적화 시에는 +- 2 범위의 곡선만 다시 계산
        pSpline->Compute_Spline();
    }
}

void CSceneWindow::Manipulate_GraphNode(CGameObject* pSel)
{
    CTrackGraph* pTGraph = pSel->Get_Component<CTrackGraph>();

    TrackNode* pTN = pTGraph->Get_TrackNode(g_uPointSelected);

    _matrix matNode, matObj;
    D3DXMatrixIdentity(&matNode);
    matNode.m[0][0] = 1;
    matNode.m[1][1] = 1;
    matNode.m[2][2] = 1;
    memcpy(&matNode.m[3], &pTN->position, sizeof(_vec3));

    matObj = *pSel->Get_Transform()->Get_World();
    matNode *= matObj;

    ImGuizmo::SetGizmoSizeClipSpace(m_gizmoSize);

    ImGuizmo::Manipulate(
        (float*)m_matView, (float*)m_matProj,
        g_GizmoOp, ImGuizmo::WORLD, (float*)&matNode);

    if (ImGuizmo::IsUsing())
    {
        // 기즈모의 조작은 로컬에 적용되야 하므로 오브젝트의 영향을 다시 없앤다.
        _matrix matInvObj;
        D3DXMatrixInverse(&matInvObj, 0, &matObj);
        matNode *= matInvObj;

        if (g_GizmoOp == ImGuizmo::TRANSLATE) {
            _vec3 newPos;
            memcpy(&newPos, &matNode.m[3], sizeof(_vec3));
            pTGraph->Set_NodePos(pTN, newPos);
            m_bWasUsingTranslate = true;
        }
    }
    else if (m_bWasUsingTranslate) {
        m_bWasUsingTranslate = false;
        pTGraph->Compute_Graph();
    }
}

void CSceneWindow::Manipulate_GraphPoint(CGameObject* pSel)
{
    CTrackGraph* pTGraph = pSel->Get_Component<CTrackGraph>();

    TrackEdge* pTE = pTGraph->Get_TrackEdge(g_uGraphEdgeEdit);
    ControlPoint* pCp = pTGraph->Get_ControlPoint(pTE, g_uPointSelected);

    _vec3 R = pCp->R;
    _vec3 U = pCp->U;
    ImGuizmo::OPERATION op = g_GizmoOp;
    if (g_GizmoOp == ImGuizmo::ROTATE) {
        op = ImGuizmo::ROTATE_Z;
    }
    else if (g_GizmoOp == ImGuizmo::SCALE) {
        op = ImGuizmo::SCALE_X | ImGuizmo::SCALE_Y;
        R *= pCp->width;
        U *= pCp->depth;
    }

    _matrix matCP, matObj;
    D3DXMatrixIdentity(&matCP);
    memcpy(&matCP.m[0], &R, sizeof(_vec3));
    memcpy(&matCP.m[1], &U, sizeof(_vec3));
    memcpy(&matCP.m[2], &pCp->T, sizeof(_vec3));
    memcpy(&matCP.m[3], &pCp->position, sizeof(_vec3));

    matObj = *pSel->Get_Transform()->Get_World();
    matCP *= matObj;

    ImGuizmo::SetGizmoSizeClipSpace(m_gizmoSize);

    ImGuizmo::Manipulate(
        (float*)m_matView, (float*)m_matProj,
        op, ImGuizmo::WORLD, (float*)&matCP);

    if (ImGuizmo::IsUsing())
    {
        // 기즈모의 조작은 로컬에 적용되야 하므로 오브젝트의 영향을 다시 없앤다.

        _matrix matInvObj;
        D3DXMatrixInverse(&matInvObj, 0, &matObj);
        matCP *= matInvObj;

        if (op == ImGuizmo::TRANSLATE) {
            _vec3 newPos;
            memcpy(&newPos, &matCP.m[3], sizeof(_vec3));
            pTGraph->Set_PointPos(pTE, pCp, newPos);
            m_bWasUsingTranslate = true;
        }
        else if (op == ImGuizmo::ROTATE_Z) {
            // 해당 cp의 right로 up과 bank 계산
            // R,U,bank만 바뀜 -> 메쉬 다시 생성
            // 최적화 시에는 이전/다음 cp 사이의 매쉬만 수정
            _vec3 vRight;
            memcpy(&vRight, &matCP.m[0], sizeof(_vec3));
            pTGraph->Set_BankByRight(pTE, pCp, vRight);
        }
        else if (op == (ImGuizmo::SCALE_X | ImGuizmo::SCALE_Y)) {
            // 해당 cp의 width/depth를 바꾼다
            // -> 매쉬 다시 생성
            // 최적화 시에는 이전/다음 cp 사이의 매쉬만 수정
            _vec3 vRight, vUp;
            memcpy(&vRight, &matCP.m[0], sizeof(_vec3));
            memcpy(&vUp, &matCP.m[1], sizeof(_vec3));
            pTGraph->Set_WidthDepth(pTE, pCp, vRight, vUp);
        }
    }
    else if (m_bWasUsingTranslate) {
        m_bWasUsingTranslate = false;
        pTGraph->Compute_Graph();
    }
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