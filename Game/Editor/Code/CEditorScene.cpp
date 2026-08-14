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

void CEditorScene::InvalidateDeviceObjects()
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
	pWindow = CGameWindow::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);

	pWindow = CHierarchy::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);

	pWindow = CInspector::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);

	pWindow = CProject::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);

	pWindow = CSceneWindow::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);

	pWindow = CSequence::Create(m_pGraphicDev);
	m_windowList.push_back(pWindow);
	return S_OK;
}

HRESULT CEditorScene::Ready_Layers()
{
	CLayer* pLayer = CLayer::Create();

	m_mapLayer.insert({ L"Default", pLayer });

	return S_OK;
}

void CEditorScene::Free()
{
	CScene::Free();
}
