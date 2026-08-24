#include "pch.h"
#include "CStartMenu.h"
#include "CLoadingThread.h"
#include "CBackGround.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

#include "CManagement.h"
#include "CRcTex.h"
#include "CMenu_Item.h"
#include "CLoading.h"
#include "CScene1_Item.h"
#include "CScene1_Speed.h"
#include "CScene1_Replay.h"
#include "CUI_XButton.h"
#include "CUI_UnderBar.h"
#include "CUI_Menu.h"
#include "CInventoryScene.h"
#include "CRacingScene.h"
#include "CDInputMgr.h"

CStartMenu::CStartMenu(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CStartMenu::~CStartMenu()
{
}

HRESULT CStartMenu::Ready_Scene()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;

	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;



	return S_OK;
}



_int CStartMenu::Update_Scene(const _float& fDeltaTime)
{
	
	_int iExit = CScene::Update_Scene(fDeltaTime);

	if (GetAsyncKeyState('M'))
	{

		//Engine::CScene* pStage = CInventoryScene::Create(m_pGraphicDev);
		//Engine::CScene* pStage = CCollisionTest::Create(m_pGraphicDev);
		Engine::CScene* pStage = CRacingScene::Create(m_pGraphicDev, MAP_TEST);

		if (nullptr == pStage)
			return E_FAIL;

		if (FAILED(CManagement::GetInstance()->Set_Scene(pStage)))
		{
			MSG_BOX("Stage Create Failed");
			return -1;
		}
	}

	return iExit;
}





void CStartMenu::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CStartMenu::Render_Scene()
{
	_matrix matView, matProj;
	_vec3 vEye, vAt, vUp;
	vEye = { 0, 0, -2 };
	vAt = { 0, 0, 1 };
	vUp = { 0, 1, 0 };
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	D3DVIEWPORT9 vp;

	m_pGraphicDev->GetViewport(&vp);
	D3DXMatrixOrthoLH(&matProj, float(vp.Width), float(vp.Height), 1.f, 1000.f);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);


	//CScene::Render_Scene();
}

HRESULT CStartMenu::Ready_Environment_Layer(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();

	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	// BackGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	dynamic_cast<CBackGround*>(pGameObject)->Change_BackgroundTexture(BACKGROUND_STARTMENU);
	
	if (nullptr == pGameObject)
		return E_FAIL;
	
	if (FAILED(pLayer->Add_GameObject(L"BackGround", pGameObject)))
		return E_FAIL;
	
	pGameObject->Get_Transform()->Set_Scale({ WINCX, WINCY, 1 });
	pGameObject->Get_Transform()->Set_Pos({ 0, 0, 15 });

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStartMenu::Ready_Prototype()
{
	return S_OK;
}

HRESULT CStartMenu::Ready_UI_Layer()
{
	CLayer* pUILayer = CLayer::Create();
	if (pUILayer == nullptr)
		return E_FAIL;
	m_mapLayer.insert({ L"UI", pUILayer });
	
	CGameObject* pUIObject = nullptr;


	pUIObject = CScene1_Item::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"Scene1_Item", pUIObject)))
		return E_FAIL;

	pUIObject = CScene1_Speed::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"Scene1_Speed", pUIObject)))
		return E_FAIL;

	pUIObject = CScene1_Replay::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"CScene1_Replay", pUIObject)))
		return E_FAIL;

	pUIObject = CUI_UnderBar::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_UnderBar", pUIObject)))
		return E_FAIL;

	pUIObject = CUI_XButton::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_XButton", pUIObject)))
		return E_FAIL;

	

	return S_OK;


}

CStartMenu* CStartMenu::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStartMenu* pStartMenu = new CStartMenu(pGraphicDev);

	if (FAILED(pStartMenu->Ready_Scene()))
	{
		Safe_Release(pStartMenu);
		MSG_BOX("StartMenu Create Failed22");
		return nullptr;
	}

	return pStartMenu;
}

void CStartMenu::Free()
{

	CScene::Free();
}