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
				g_bSelected = true;
				g_uSelected = guid;
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
			pRoot = LoadGameObject(st, m_pGraphicDev, nullptr, true, pScene);
		else
			break;
	}

	fclose(fp);
}

CGameObject* CEditorScene::LoadGameObject(FileReadState& st, LPDIRECT3DDEVICE9 pGraphicDev, 
	CGameObject* pParent, bool bRoot, CScene* pScene)
{
	CGameObject* pObj = CEmpty::Create(pGraphicDev);
	if (pObj == nullptr)
		return nullptr;

	const uint32_t guid = pScene->GenerateGuid();
	pObj->SetGuid(guid);

	wstring key = to_wstring(guid);
	pScene->Add_GameObject(L"Default", key.c_str(), pObj);
	if (pParent)
		pParent->Set_Child(pObj);

	wchar_t* t = nullptr;
	while (st.Next(t))
	{
		if (!wcscmp(t, L"ENDOBJECT"))
			break;

		if (!wcscmp(t, L"OBJECT"))
		{
			LoadGameObject(st, pGraphicDev, pObj, false, pScene);
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
		else if (StartsWith(t, L"prefabPath="))
			pObj->Set_PrefabPath(t + 11);
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

	return pObj;
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
		SaveGameObject(pf, pObj, 0, true);
	}

	fclose(pf);
}

void CEditorScene::SaveGameObject(FILE* pf, CGameObject* pObj, int depth, bool bRoot)
{
	auto writeIndent = [](FILE* f, int depth)
		{
			for (int i = 0; i < depth; ++i)
				fwprintf(f, L"  ");
		};

	writeIndent(pf, depth);
	fwprintf(pf, L"OBJECT\n");

	if (bRoot) {
		writeIndent(pf, depth + 1);
		fwprintf(pf, L"prefabPath=%s\n", pObj->Get_PrefabPath());
	}
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
	CScene::Free();
}
