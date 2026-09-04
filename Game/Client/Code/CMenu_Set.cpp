#include "pch.h"
#include "CMenu_Set.h"
#include "CLoadingThread.h"
#include "CBackGround.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CRcTex.h"

#include "CCollisionTest.h"
#include "CLoading.h"

#include "CScene3_StartBtn.h"
#include "CScene3_CharBtn.h"
#include "CScene3_KartBtn.h"
#include "CScene3_ColorBtn.h"
#include "CScene3_ColorSetBG.h"
#include "CScene3_ColorSet.h"
#include "CScene3_CharSlot.h"
#include "CScene3_CharBG.h"
#include "CScene3_Char.h"
#include "CUI_UnderBar.h"
#include "CUI_XButton.h"
#include "CScene3_Map_ForestValley.h"
#include  "CInvenSlotBG.h"
#include "CUI_InvenSlot.h"
#include "CInvenSlotCart.h"
#include "CInventoryScene.h"
#include "CDinputMgr.h"



CMenu_Set::CMenu_Set(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CMenu_Set::~CMenu_Set()
{
}

HRESULT CMenu_Set::Ready_Scene()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;

	if (FAILED(Ready_RenderTarget()))
		return E_FAIL;

	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;



	return S_OK;
}



_int CMenu_Set::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = CScene::Update_Scene(fDeltaTime);
	//CScene3_ColorSetBG* pSetBG = static_cast<CScene3_ColorSetBG*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_ColorSetBG"));
	//if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_LB))
	//{
	//	pSetBG->Set_Show(false);
	//}

	return iExit;
}





void CMenu_Set::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CMenu_Set::Render_Scene()
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

HRESULT CMenu_Set::Ready_Environment_Layer(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();

	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	// BackGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	dynamic_cast<CBackGround*>(pGameObject)->Change_BackgroundTexture(BACKGROUND_SETMENU);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"BackGround", pGameObject)))
		return E_FAIL;

	pGameObject->Get_Transform()->Set_Scale({ WINCX, WINCY, 1 });
	pGameObject->Get_Transform()->Set_Pos({ 0, 0, 15 });

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMenu_Set::Ready_Prototype()
{
	return S_OK;
}

HRESULT CMenu_Set::Ready_RenderTarget()
{
	
		CRenderer::GetInstance()->Add_RenderTarget(m_pGraphicDev, L"InvenSlot0", 250, 400);
		CRenderer::GetInstance()->Add_RenderTarget(m_pGraphicDev, L"InvenSlot1", 250, 400);
		CRenderer::GetInstance()->Add_RenderTarget(m_pGraphicDev, L"CharSlot0", 200, 200);
		CRenderer::GetInstance()->Add_RenderTarget(m_pGraphicDev, L"CharSlot1", 200, 200);
		return S_OK;
	
}

HRESULT CMenu_Set::Ready_UI_Layer()
{
	CLayer* pUILayer = CLayer::Create();
	if (pUILayer == nullptr)
		return E_FAIL;
	m_mapLayer.insert({ L"UI", pUILayer });
	
	CGameObject* pUIObject = nullptr;
	
	pUIObject = CScene3_Map_ForestValley::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_Map_ForestValley", pUIObject)))
		return E_FAIL;
	
	
	
	pUIObject = CScene3_StartBtn::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_StartBtn", pUIObject)))
		return E_FAIL;
	
	pUIObject = CScene3_CharBtn::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_CharBtn", pUIObject)))
		return E_FAIL;
	
	pUIObject = CScene3_KartBtn::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_KartBtn", pUIObject)))
		return E_FAIL;
	
	pUIObject = CScene3_ColorBtn::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_ColorBtn", pUIObject)))
		return E_FAIL;

	pUIObject = CScene3_ColorSetBG::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_ColorSetBG", pUIObject)))
		return E_FAIL;

	pUIObject = CScene3_ColorSet::Create(m_pGraphicDev, COLOR_RED);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_ColorSet1", pUIObject)))
		return E_FAIL;
	pUIObject = CScene3_ColorSet::Create(m_pGraphicDev, COLOR_GREEN);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_ColorSet2", pUIObject)))
		return E_FAIL;

	
	
	CGameObject* pUIInvenSlot = CUI_InvenSlot::Create(m_pGraphicDev, INVEN_FIRST);
	if (nullptr == pUIInvenSlot)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_InvenSlot", pUIInvenSlot)))
		return E_FAIL;
	pUIInvenSlot->Get_Transform()->Set_Pos({ -350, 0, 1 });
	pUIInvenSlot->Get_Transform()->Set_Scale({ 100,150,1 });
	
	
	pUIObject = CInvenSlotBG::Create(m_pGraphicDev, INVEN_FIRST);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"InvenSlotBG", pUIObject)))
		return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot)->SetBG(pUIObject);
	
	pUIObject = CInvenSlotCart::Create(m_pGraphicDev, INVEN_FIRST);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"InvenSlotCart", pUIObject)))
		return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot)->SetItem(pUIObject);

	CGameObject* pUIInvenSlot2 = CUI_InvenSlot::Create(m_pGraphicDev, INVEN_SECOND);
	if (nullptr == pUIInvenSlot2)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_InvenSlot2", pUIInvenSlot2)))
		return E_FAIL;
	pUIInvenSlot2->Get_Transform()->Set_Pos({ -250, 0, 1 });
	pUIInvenSlot2->Get_Transform()->Set_Scale({ 100,150,1 });


	pUIObject = CInvenSlotBG::Create(m_pGraphicDev, INVEN_SECOND);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"InvenSlotBG2", pUIObject)))
		return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot2)->SetBG(pUIObject);

	pUIObject = CInvenSlotCart::Create(m_pGraphicDev, INVEN_SECOND);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"InvenSlotCart2", pUIObject)))
		return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot2)->SetItem(pUIObject);
	
	CGameObject* pUICharSlot = CScene3_CharSlot::Create(m_pGraphicDev, CHAR_BAZZI);
	if (pUICharSlot == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"CharSlot1", pUICharSlot)))
		return E_FAIL;
	pUICharSlot->Get_Transform()->Set_Pos({ -275, 100, 1 });
	pUICharSlot->Get_Transform()->Set_Scale({ 200,200,1 });

	pUIObject = CScene3_CharBG::Create(m_pGraphicDev, CHAR_BAZZI);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"CharSlotBG", pUIObject)))
		return E_FAIL;
	static_cast<CScene3_CharSlot*>(pUICharSlot)->SetBG(pUIObject);
	
	pUIObject = CScene3_Char::Create(m_pGraphicDev, CHAR_BAZZI);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"CharSlotChar", pUIObject)))
		return E_FAIL;
	static_cast<CScene3_CharSlot*>(pUICharSlot)->SetChar(pUIObject);







	CGameObject* pUICharSlot2 = CScene3_CharSlot::Create(m_pGraphicDev, CHAR_DAO);
	if (pUICharSlot == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"CharSlot2", pUICharSlot2)))
		return E_FAIL;
	pUICharSlot2->Get_Transform()->Set_Pos({ -100, 100, 1 });
	pUICharSlot2->Get_Transform()->Set_Scale({ 200,200,1 });

	pUIObject = CScene3_CharBG::Create(m_pGraphicDev, CHAR_DAO);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"CharSlotBG2", pUIObject)))
		return E_FAIL;
	static_cast<CScene3_CharSlot*>(pUICharSlot2)->SetBG(pUIObject);

	pUIObject = CScene3_Char::Create(m_pGraphicDev, CHAR_DAO);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"CharSlotChar2", pUIObject)))
		return E_FAIL;
	static_cast<CScene3_CharSlot*>(pUICharSlot2)->SetChar(pUIObject);

	
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

CMenu_Set* CMenu_Set::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMenu_Set* pMenu = new CMenu_Set(pGraphicDev);

	if (FAILED(pMenu->Ready_Scene()))
	{
		Safe_Release(pMenu);
		MSG_BOX("CMenu_Set Create Failed22");
		return nullptr;
	}

	return pMenu;
}

void CMenu_Set::Free()
{

	CScene::Free();
	CRenderer::GetInstance()->Clear_RenderGroup();
	CRenderer::GetInstance()->Delete_RenderTarget(L"InvenSlot0");
	CRenderer::GetInstance()->Delete_RenderTarget(L"InvenSlot1");
	CRenderer::GetInstance()->Delete_RenderTarget(L"CharSlot0");
	CRenderer::GetInstance()->Delete_RenderTarget(L"CharSlot1");
}