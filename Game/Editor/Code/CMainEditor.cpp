#include "pch.h"
#include "CMainEditor.h"
#include "CCalculator.h"

CMainEditor::CMainEditor()
	: m_pDeviceClass(nullptr), m_pGraphicDev(nullptr)
{
}

CMainEditor::~CMainEditor()
{
}

HRESULT CMainEditor::Ready_MainEditor()
{
#ifdef _DEBUG

	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

#endif // _DEBUG

	if (FAILED(Ready_DefaultSetting(&m_pGraphicDev)))
		return E_FAIL;

	if (FAILED(Ready_Scene(m_pGraphicDev)))
		return E_FAIL;

	return S_OK;
}


void CMainEditor::FixedUpdate_MainEditor()
{
}

int CMainEditor::Update_MainEditor()
{
    ImGuiIO& io = ImGui::GetIO();

    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // Hierarchy 창
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));

        ImGui::Begin("Hierarchy");
        ImGui::End();
    }

    // Inspector 창
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));

        ImGui::Begin("Inspector");
        ImGui::End();
    }

    // Scene 창
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));

        ImGui::Begin("Scene");
        ImGui::End();
    }

    // Game 창
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));

        ImGui::Begin("Game");
        ImGui::End();
    }

    // Project 창
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));

        ImGui::Begin("Project");
        ImGui::End();
    }

    // Sequence 창
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(250.0f, 250.0f), ImVec2(FLT_MAX, FLT_MAX));

        ImGui::Begin("Sequence");
        ImGui::End();
    }

    // ===== ImGuizmo 시각 테스트 =====
    {
        static int selected = 0;

        // 오브젝트의 월드 행렬
        static float objectMatrix[16] = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            -1.25,1,0,1
        };
        static float objectMatrix2[16] = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            1.25,1,0,1
        };
        // Transform 조작 창의 설정
        static ImGuizmo::OPERATION op = ImGuizmo::TRANSLATE;
        static ImGuizmo::OPERATION op2 = ImGuizmo::TRANSLATE;
        static ImGuizmo::MODE mode = ImGuizmo::WORLD;
        static ImGuizmo::MODE mode2 = ImGuizmo::WORLD;

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
        ImGui::Begin("Gizmo Viewport");

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
                g_pd3dDevice,
                viewSize.x, viewSize.y,
                1,
                D3DUSAGE_RENDERTARGET,
                D3DFMT_A8R8G8B8,
                D3DPOOL_DEFAULT,
                &m_pSceneTex);

            g_pd3dDevice->CreateDepthStencilSurface(
                viewSize.x, viewSize.y,
                D3DFMT_D24S8,
                D3DMULTISAMPLE_NONE, 0, TRUE,
                &m_pSceneDepth, nullptr);

            m_rtW = viewSize.x; m_rtH = viewSize.y;
        }

        if (m_pSceneTex) {
            LPDIRECT3DSURFACE9 pOldRT = nullptr;
            LPDIRECT3DSURFACE9 pOldDepth = nullptr;
            g_pd3dDevice->GetRenderTarget(0, &pOldRT);
            g_pd3dDevice->GetDepthStencilSurface(&pOldDepth);

            LPDIRECT3DSURFACE9 pRT = nullptr;
            m_pSceneTex->GetSurfaceLevel(0, &pRT);

            g_pd3dDevice->SetRenderTarget(0, pRT);
            g_pd3dDevice->SetDepthStencilSurface(m_pSceneDepth);

            g_pd3dDevice->Clear(0, nullptr,
                D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                D3DCOLOR_XRGB(40, 40, 40), 1.f, 0);

            g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
            g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

            g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
            g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

            // 큐브 1
            g_pd3dDevice->SetTransform(D3DTS_WORLD, (_matrix*)objectMatrix);
            m_pCubeBuffer->Render_Buffer();

            // 큐브 2
            g_pd3dDevice->SetTransform(D3DTS_WORLD, (_matrix*)objectMatrix2);
            m_pCubeBuffer->Render_Buffer();

            // 복구
            g_pd3dDevice->SetRenderTarget(0, pOldRT);
            g_pd3dDevice->SetDepthStencilSurface(pOldDepth);
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
        //ImGuizmo::DrawCubes((const float*)matView, (const float*)matProj, objectMatrix, 1);
        //ImGuizmo::DrawCubes((const float*)matView, (const float*)matProj, objectMatrix2, 1);

        // ImGuizmo는 조작 상태(mbUsing 등)를 전역 Context + ID로 관리합니다.
        // 공식 예제도 여러 오브젝트일 때 반드시 PushID / PopID를 씁니다.

        if(selected == 1)
            ImGuizmo::Manipulate((const float*)matView, (const float*)matProj, op, mode, objectMatrix);
        else if(selected == 2)
            ImGuizmo::Manipulate((const float*)matView, (const float*)matProj, op2, mode2, objectMatrix2);

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

                _vec3 rayOrigin1;
                _vec3 rayDir1;
                _matrix matWorld1(objectMatrix);
                _matrix matInvWorld1;
                D3DXMatrixInverse(&matInvWorld1, 0, &matWorld1);
                D3DXVec3TransformCoord(&rayOrigin1, &rayOrigin, &matInvWorld1);
                D3DXVec3TransformNormal(&rayDir1, &rayDir, &matInvWorld1);
                D3DXVec3Normalize(&rayDir1, &rayDir1);
                // BOUNDING BOX로 RAY CHECK
                // 이후 Check에 따라 selected가 바뀜
                // select에 따라 Manipulate 대상이 바뀜
                DirectX::BoundingBox box;
                m_pCubeBuffer->GetBoundingBox(&box);
                float dist1;
                if (box.Intersects(ToXMVec(rayOrigin1), ToXMVec(rayDir1), dist1)) {
                    isPicked = true;
                    selected = 1;
                }

                _vec3 rayOrigin2;
                _vec3 rayDir2;
                _matrix matWorld2(objectMatrix2);
                _matrix matInvWorld2;
                D3DXMatrixInverse(&matInvWorld2, 0, &matWorld2);
                D3DXVec3TransformCoord(&rayOrigin2, &rayOrigin, &matInvWorld2);
                D3DXVec3TransformNormal(&rayDir2, &rayDir, &matInvWorld2);
                D3DXVec3Normalize(&rayDir2, &rayDir2);
                
                float dist2;
                if (box.Intersects(ToXMVec(rayOrigin2), ToXMVec(rayDir2), dist2)) {
                    isPicked = true;
                    selected = 2;
                }

                if (!isPicked)
                    selected = 0;
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

        ImGui::Begin("Gizmo Controls");
        if (ImGui::RadioButton("Translate", op == ImGuizmo::TRANSLATE)) op = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", op == ImGuizmo::ROTATE)) op = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", op == ImGuizmo::SCALE)) op = ImGuizmo::SCALE;
        if (op != ImGuizmo::SCALE)
        {
            if (ImGui::RadioButton("Local", mode == ImGuizmo::LOCAL)) mode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mode == ImGuizmo::WORLD)) mode = ImGuizmo::WORLD;
        }
        float t[3], r[3], s[3];
        ImGuizmo::DecomposeMatrixToComponents(objectMatrix, t, r, s);
        if (ImGui::InputFloat3("Tr", t))
            ImGuizmo::RecomposeMatrixFromComponents(t, r, s, objectMatrix);
        if (ImGui::InputFloat3("Rt", r))
            ImGuizmo::RecomposeMatrixFromComponents(t, r, s, objectMatrix);
        if (ImGui::InputFloat3("Sc", s))
            ImGuizmo::RecomposeMatrixFromComponents(t, r, s, objectMatrix);
        ImGui::End();
    }

    // Rendering
    ImGui::EndFrame();
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
    g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
    if (g_pd3dDevice->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        g_pd3dDevice->EndScene();
    }

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
    if (result == D3DERR_DEVICELOST)
        g_DeviceLost = true;

	return 0;
}

void CMainEditor::LateUpdate_MainEditor()
{
}

void CMainEditor::Render_MainEditor()
{
}

HRESULT CMainEditor::Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); //(void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
    io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If fonts are not explicitly loaded, Dear ImGui will select an embedded font: either AddFontDefaultVector() or AddFontDefaultBitmap().
    //   This selection is based on (style.FontSizeBase * style.FontScaleMain * style.FontScaleDpi) reaching a small threshold.
    // - You can load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - If a file cannot be loaded, AddFont functions will return a nullptr. Please handle those errors in your code (e.g. use an assertion, display an error and quit).
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use FreeType for higher quality font rendering.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //style.FontSizeBase = 20.0f;
    //io.Fonts->AddFontDefaultVector();
    //io.Fonts->AddFontDefaultBitmap();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);

	return S_OK;
}

HRESULT CMainEditor::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
    m_pCubeBuffer = Engine::CCube::Create(g_pd3dDevice);

	return S_OK;
}

CMainEditor* CMainEditor::Create()
{
	CMainEditor* pMainEditor = new CMainEditor;

	if (FAILED(pMainEditor->Ready_MainEditor()))
	{
		MSG_BOX("MainEditor Create Failed");
		Safe_Release(pMainEditor);
		return nullptr;
	}

	return pMainEditor;
}

void CMainEditor::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);
    Safe_Release(m_pCubeBuffer);
}
