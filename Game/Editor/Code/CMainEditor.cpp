#include "pch.h"
#include "CMainEditor.h"
#include "CFontMgr.h"
#include "CLoading.h"
#include "CTimerMgr.h"
#include "CFrameMgr.h"

CMainEditor::CMainEditor()
	: m_pGraphicDev(nullptr)
    , m_pManagementClass(CManagement::GetInstance())
{
}

CMainEditor::~CMainEditor()
{
}

void CMainEditor::GameLoop()
{
	CTimerMgr::GetInstance()->Set_TimeDelta(L"Timer_Global");
	_float fGlobal_TimeDelta = CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_Global");

	if (CFrameMgr::GetInstance()->IsPermit_Call(L"Frame60", fGlobal_TimeDelta))
	{
		CTimerMgr::GetInstance()->Set_TimeDelta(L"Timer_FPS60");
		_float fFPS60_DeltaTime = CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");
		_float fFixed_DeltaTime;
		int fixedStep = CTimerMgr::GetInstance()->Get_FixedStep(L"Timer_FPS60", &fFixed_DeltaTime);

		m_pManagementClass->Change_NextScene();

		for (int i = 0; i < fixedStep; ++i)
			FixedUpdate_MainEditor(fFixed_DeltaTime);

		// 이 부분이 FixedUpdate 말한대로 이번 프레임에 FixedUpdate를 몇번 호출해야하나 횟수를 얻어서 여러번
		// 실행한다.

		Update_MainEditor(fFPS60_DeltaTime);
		LateUpdate_MainEditor(fFPS60_DeltaTime);
		Render_MainEditor();
	}
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

    m_pGraphicDev = g_pd3dDevice;
    g_pd3dDevice->AddRef();

	if (FAILED(Ready_DefaultSetting(&m_pGraphicDev)))
		return E_FAIL;

	if (FAILED(Ready_Scene(m_pGraphicDev)))
		return E_FAIL;

	if (FAILED(Ready_IMGUI()))
		return E_FAIL;

	return S_OK;
}


void CMainEditor::FixedUpdate_MainEditor(const float& fFixedDeltaTime)
{
    m_pManagementClass->FixedUpdate_Scene(fFixedDeltaTime);
}

int CMainEditor::Update_MainEditor(const float& fDeltaTime)
{
    m_pManagementClass->Update_Scene(fDeltaTime);
	return 0;
}

void CMainEditor::LateUpdate_MainEditor(const float& fDeltaTime)
{
    m_pManagementClass->LateUpdate_Scene(fDeltaTime);
}

void CMainEditor::Render_MainEditor()
{
	m_pManagementClass->Render_Scene(m_pGraphicDev, true);
}

void CMainEditor::Reset_MainEditor()
{
	// D3DPOOL_DEFAULT로 선언된 리소스 해제
	m_pManagementClass->OnLostDevice();

	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	
	m_pManagementClass->OnResetDevice();
}

HRESULT CMainEditor::Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CMainEditor::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
    Engine::CScene* pLoading = CLoading::Create(pGraphicDev);

    if (nullptr == pLoading)
        return E_FAIL;

    if (FAILED(m_pManagementClass->Set_Scene(pLoading)))
    {
        Safe_Release(pLoading);

        MSG_BOX("Logo Create Failed");
        return E_FAIL;
    }

	return S_OK;
}

HRESULT CMainEditor::Ready_IMGUI()
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
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphicDev);

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

	//g_pFontBold = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeuib.ttf", 16.0f);

	ImFont* font = io.Fonts->AddFontFromFileTTF(
		"C:\\Windows\\Fonts\\malgun.ttf", 16.0f);
	IM_ASSERT(font != nullptr);

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

    m_pManagementClass->DestroyInstance();
}
