#include "pch.h"
#include "CMenu_Speed.h"
#include "CLoadingThread.h"
#include "CBackGround.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CRcTex.h"

#include "CLoading.h"

#include "CScene2_ForestValley.h"
#include "CUI_UnderBar.h"
#include "CUI_XButton.h"


CMenu_Speed::CMenu_Speed(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CMenu_Speed::~CMenu_Speed()
{
}

HRESULT CMenu_Speed::Ready_Scene()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;

	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;



	return S_OK;
}



_int CMenu_Speed::Update_Scene(const _float& fDeltaTime)
{

	_int iExit = CScene::Update_Scene(fDeltaTime);


	return iExit;
}





void CMenu_Speed::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CMenu_Speed::Render_Scene()
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

HRESULT CMenu_Speed::Ready_Environment_Layer(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();

	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	// BackGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	dynamic_cast<CBackGround*>(pGameObject)->Change_BackgroundTexture(BACKGROUND_SPEEDMENU);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"BackGround", pGameObject)))
		return E_FAIL;

	pGameObject->Get_Transform()->Set_Scale({ WINCX, WINCY, 1 });
	pGameObject->Get_Transform()->Set_Pos({ 0, 0, 15 });

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMenu_Speed::Ready_Prototype()
{
	return S_OK;
}

HRESULT CMenu_Speed::Ready_UI_Layer()
{
	CLayer* pUILayer = CLayer::Create();
	if (pUILayer == nullptr)
		return E_FAIL;
	m_mapLayer.insert({ L"UI", pUILayer });

	CGameObject* pUIObject = nullptr;


	pUIObject = CScene2_ForestValley::Create(m_pGraphicDev, BACKGROUND_SETSPEEDMENU);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"CScene2_ForestValley", pUIObject)))
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

CMenu_Speed* CMenu_Speed::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMenu_Speed* pMenu = new CMenu_Speed(pGraphicDev);

	if (FAILED(pMenu->Ready_Scene()))
	{
		Safe_Release(pMenu);
		MSG_BOX("Menu_Item Create Failed22");
		return nullptr;
	}

	return pMenu;
}

void CMenu_Speed::Free()
{

	CScene::Free();
}