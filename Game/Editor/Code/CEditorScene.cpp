#include "pch.h"
#include "CEditorScene.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CWindow.h"
#include "CGameWindow.h"
#include "CSceneWindow.h"
#include "CHierarchy.h"
#include "CInspector.h"
#include "CProject.h"
#include "CSequence.h"
#include "imgui_internal.h"  // DockBuilder* API
#include "CEmpty.h"
#include "CGameObject.h"
#include "CManagement.h"
#include "Engine_Parsing.h"


CEditorScene::CEditorScene(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CEditorScene::~CEditorScene()
{
}

HRESULT CEditorScene::Ready_Scene()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Windows()))
		return E_FAIL;

	if (FAILED(Ready_Layers()))
		return E_FAIL;

	return S_OK;
}

void CEditorScene::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
	CScene::FixedUpdate_Scene(fFixedDeltaTime);

	// 충돌 처리
}

_int CEditorScene::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = CScene::Update_Scene(fDeltaTime);
	return iExit;
}

void CEditorScene::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CEditorScene::Render_Scene()
{
	ImGuiIO& io = ImGui::GetIO();

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene")) { OnNewScene(); }
			if (ImGui::MenuItem("Open Scene")) { OnLoad(); }
			ImGui::Separator();
			if (ImGui::MenuItem("Save")) { OnSave(false); }
			if (ImGui::MenuItem("Save As")) { OnSave(true); }
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Undo", "Ctrl+Z");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject")) {
			if (ImGui::MenuItem("Create Empty")) {
				CGameObject* obj = CEmpty::Create(m_pGraphicDev);
				obj->SetName(L"Empty");
				uint64_t guid = CManagement::GetInstance()->GenerateGuid();
				obj->SetGuid(guid);

				wstring s = std::to_wstring(guid);
				CManagement::GetInstance()->Add_GameObject(L"Default", s.c_str(), obj);
				::Set_ObjSelected(guid);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	InputShortCut();

	SetupDockLayout();

	static bool show_demo_window = false;
	static bool show_another_window = false;

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

	for (auto& wnd : m_windowList)
		wnd->Update_Window();

	// Rendering
	ImGui::EndFrame();
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
	if (m_pGraphicDev->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		m_pGraphicDev->EndScene();
	}

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	HRESULT result = m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);
	if (result == D3DERR_DEVICELOST)
		g_DeviceLost = true;
}

void CEditorScene::OnLostDevice()
{
	for (auto& wnd : m_windowList)
		wnd->InvalidateDeviceObjects();
}

CEditorScene* CEditorScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEditorScene* pScene = new CEditorScene(pGraphicDev);

	if (FAILED(pScene->Ready_Scene())) {
		MSG_BOX("CEditorScene Create Failed");
		Safe_Release(pScene);
		return nullptr;
	}

	return pScene;
}

HRESULT CEditorScene::Ready_Prototype()
{
	return S_OK;
}

HRESULT CEditorScene::Ready_Windows()
{    
	CWindow* pWindow = nullptr;
	pWindow = CHierarchy::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);

	pWindow = CSceneWindow::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);

	pWindow = CGameWindow::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);

	pWindow = CInspector::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);

	pWindow = CSequence::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);

	pWindow = CProject::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);

	return S_OK;
}

HRESULT CEditorScene::Ready_Layers()
{
	CLayer* pLayer = CLayer::Create();

	m_mapLayer.insert({ L"Default", pLayer });

	return S_OK;
}

void CEditorScene::OnNewScene()
{
	Engine::CScene* pScene = CEditorScene::Create(m_pGraphicDev);

	CManagement::GetInstance()->Request_Scene(pScene);
}

void CEditorScene::OnLoad()
{
	if (!OpenLoadSceneDialog(m_scenePath, MAX_PATH))
		return;  // 취소

	Engine::CScene* pScene = CEditorScene::Create(m_pGraphicDev);
	pScene->Set_Path(m_scenePath);
	pScene->Set_Dirty(false);

	LoadSceneFile(m_scenePath, pScene);

	CManagement::GetInstance()->Request_Scene(pScene);
}

bool CEditorScene::OpenLoadSceneDialog(_tchar* outPath, DWORD outChars)
{
	wchar_t fileBuf[MAX_PATH] = {};
	wchar_t initialDir[MAX_PATH] = {};
	GetFullPathNameW(L"../../../Resource/Editor/Scene", MAX_PATH, initialDir, nullptr);
	CreateDirectoryW(initialDir, nullptr);  // 없으면 만들기

	swprintf_s(fileBuf, L"%s\\.Scene", initialDir);

	OPENFILENAMEW ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;
	ofn.lpstrFile = fileBuf;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"Scene (*.scene)\0*.scene\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrDefExt = L"scene";
	ofn.lpstrInitialDir = initialDir;
	ofn.lpstrTitle = L"Open Scene";
	ofn.Flags = OFN_FILEMUSTEXIST   // 없는 파일은 선택 불가
		| OFN_PATHMUSTEXIST
		| OFN_NOCHANGEDIR;

	if (!GetOpenFileNameW(&ofn))
		return false;

	if (!ToRelFromCwd(fileBuf, outPath, outChars))
		wcscpy_s(outPath, outChars, fileBuf);  // 다른 드라이브면 절대경로 유지

	return true;
}

void CEditorScene::LoadSceneFile(const _tchar* path, CScene* pScene)
{
	FILE* fp = nullptr;
	if (_wfopen_s(&fp, path, L"r, ccs=UTF-8") != 0 || !fp)
		return;

	FileReadState st;
	st.fp = fp;

	wchar_t* t = nullptr;
	CGameObject* pRoot = nullptr;
	while (true) {
		if (st.Next(t) && !wcscmp(t, L"OBJECT"))
			pRoot = ::LoadGameObject(st, m_pGraphicDev, nullptr, pScene);
		else
			break;
	}

	fclose(fp);
}

void CEditorScene::OnSave(bool bSaveAs)
{
	if (bSaveAs || m_scenePath[0] == L'\0')
	{
		if (!OpenSaveSceneDialog(m_scenePath, MAX_PATH))
			return;  // 취소 → 여기서 끝, 파일 없음
	}

	SaveSceneFile(m_scenePath);  // 대화상자가 닫힌 직후
	CManagement::GetInstance()->Set_SceneDirty(false);
}

bool CEditorScene::OpenSaveSceneDialog(_tchar* outPath, DWORD outChars)
{
	wchar_t fileBuf[MAX_PATH] = {};
	wchar_t initialDir[MAX_PATH] = {};
	GetFullPathNameW(L"../../../Resource/Editor/Scene", MAX_PATH, initialDir, nullptr);
	CreateDirectoryW(initialDir, nullptr);  // 없으면 만들기

	if (outPath[0] != L'\0')
		ToAbsPath(outPath, fileBuf, MAX_PATH);  // 상대 → 절대 (대화상자용)
	else
		swprintf_s(fileBuf, L"%s\\.scene", initialDir);

	OPENFILENAMEW ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;        // 에디터 창을 부모로
	ofn.lpstrFile = fileBuf;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"Scene (*.scene)\0*.scene\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrDefExt = L"scene";      // 확장자 안 붙이면 .scene 자동
	ofn.lpstrInitialDir = (outPath[0] == L'\0') ? initialDir : nullptr;
	ofn.lpstrTitle = L"Save Scene";
	ofn.Flags = OFN_OVERWRITEPROMPT  // 같은 이름이면 "덮어쓸까요?"
		| OFN_PATHMUSTEXIST
		| OFN_NOCHANGEDIR;     // 작업 디렉터리 안 바뀌게 (리소스 경로 보호)
	if (!GetSaveFileNameW(&ofn))
		return false;                // 취소 또는 에러

	if (!ToRelFromCwd(fileBuf, outPath, outChars))
		wcscpy_s(outPath, outChars, fileBuf);  // 다른 드라이브면 절대경로 유지
	
	return true;
}

void CEditorScene::SaveSceneFile(const _tchar* path)
{
	FILE* pf = nullptr;
	if (_wfopen_s(&pf, path, L"w, ccs=UTF-8") != 0 || !pf)
		return;

	auto& vecRoot = CManagement::GetInstance()->Get_Roots(L"Default");

	for (auto& pObj : vecRoot) {
		::SaveGameObject(pf, pObj, 0);
	}

	fclose(pf);
}

void CEditorScene::InputShortCut()
{
	if (!ImGui::IsAnyItemActive()) {
		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_Z, ImGuiInputFlags_RouteGlobal))
			DoUndo();
		if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_Y, ImGuiInputFlags_RouteGlobal))
			DoRedo();
	}
}

void CEditorScene::DoUndo()
{
}

void CEditorScene::DoRedo()
{
}

void CEditorScene::SetupDockLayout()
{
	ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(
		0, ImGui::GetMainViewport());

	static bool bFirst = true;
	if (!bFirst)
		return;
	bFirst = false;

	//if (ImGui::DockBuilderGetNode(dockspace_id) != nullptr)
	//	return;

	ImGui::DockBuilderRemoveNode(dockspace_id);
	ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->WorkSize);

	ImGuiID dock_main = dockspace_id;
	ImGuiID dock_top, dock_bottom;
	ImGuiID dock_top_left, dock_top_center, dock_top_right;
	ImGuiID dock_bottom_left, dock_bottom_right;

	// 위 아래 분단 : 75% / 25%
	ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Up, 0.75f, &dock_top, &dock_bottom);

	// 위 분단 : 20%(Hierarchy) / 60%(Scene/Game) / 24%(Inspector) 
	ImGui::DockBuilderSplitNode(dock_top, ImGuiDir_Left, 0.20f, &dock_top_left, &dock_top_center);
	ImGui::DockBuilderSplitNode(dock_top_center, ImGuiDir_Right, 0.30f, &dock_top_right, &dock_top_center);

	// 아래 분단 : 50%(Sequence) / 50%(Project)	
	ImGui::DockBuilderSplitNode(dock_bottom, ImGuiDir_Left, 0.50f, &dock_bottom_left, &dock_bottom_right);

	ImGui::DockBuilderDockWindow("Hierarchy", dock_top_left);
	ImGui::DockBuilderDockWindow("Game", dock_top_center);
	ImGui::DockBuilderDockWindow("Scene", dock_top_center); // 같은 노드 = 탭
	ImGui::DockBuilderDockWindow("Inspector", dock_top_right);
	ImGui::DockBuilderDockWindow("Sequence", dock_bottom_left); 
	ImGui::DockBuilderDockWindow("Project", dock_bottom_right);

	ImGui::DockBuilderFinish(dockspace_id);
}

void CEditorScene::Free()
{
	for (auto& wnd : m_windowList)
		Safe_Release(wnd);
	m_windowList.clear();
	CScene::Free();
}
