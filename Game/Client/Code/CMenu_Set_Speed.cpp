#include "pch.h"
#include "CMenu_Set_Speed.h"
#include "CLoadingThread.h"
#include "CBackGround.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CRcTex.h"

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
#include "CDinputMgr.h"
#include "CSlotMgr.h"
#include "CButtonMgr.h"
#include "CLoadMgr.h"

CMenu_Set_Speed::CMenu_Set_Speed(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CMenu_Set_Speed::~CMenu_Set_Speed()
{
}

HRESULT CMenu_Set_Speed::Ready_Scene()
{
	LoadSceneFromFile();

	return S_OK;
}
HRESULT CMenu_Set_Speed::PostReady_Scene()
{
	CScene::PostReady_Scene();
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
HRESULT CMenu_Set_Speed::LoadSceneFromFile()
{

	// 일단 넣어두기
	CLayer* pGameObjectLayer = CLayer::Create();

	if (pGameObjectLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"GameLogic", pGameObjectLayer });

	const _tchar* path = nullptr;

	path = L"../Bin/Resource/Editor/Scene/MenuSet.scene";

	int a;
	FILE* fp = nullptr;
	if (_wfopen_s(&fp, path, L"r, ccs=UTF-8") != 0 || !fp)
		return E_FAIL;

	FileReadState st;
	st.fp = fp;

	wchar_t* t = nullptr;
	CGameObject* pRoot = nullptr;
	while (true) {
		if (st.Next(t) && !wcscmp(t, L"OBJECT"))
			pRoot = CLoadMgr::GetInstance()->LoadGameObjectClient(st, m_pGraphicDev, nullptr, this);
		else
			break;
	}
	fclose(fp);

	return S_OK;
}
_int CMenu_Set_Speed::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = CScene::Update_Scene(fDeltaTime);


	return iExit;
}

void CMenu_Set_Speed::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CMenu_Set_Speed::Render_Scene()
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

HRESULT CMenu_Set_Speed::Ready_Environment_Layer(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();

	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	// BackGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	dynamic_cast<CBackGround*>(pGameObject)->Change_BackgroundTexture(BACKGROUND_SETSPEEDMENU);

	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"BackGround", pGameObject)))
		return E_FAIL;

	pGameObject->Get_Transform()->Set_Scale({ WINCX, WINCY, 1 });
	pGameObject->Get_Transform()->Set_Pos({ 0, 0, 15 });

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMenu_Set_Speed::Ready_Prototype()
{
	return S_OK;
}

HRESULT CMenu_Set_Speed::Ready_RenderTarget()
{
	int a;
	CRenderer::GetInstance()->Add_RenderTarget(m_pGraphicDev, L"InvenSlot0", 250, 400);
	CRenderer::GetInstance()->Add_RenderTarget(m_pGraphicDev, L"InvenSlot1", 250, 400);
	CRenderer::GetInstance()->Add_RenderTarget(m_pGraphicDev, L"CharSlot0", 200, 200);
	return S_OK;
}

HRESULT CMenu_Set_Speed::Ready_UI_Layer()
{
	CLayer* pUILayer = CLayer::Create();
	if (pUILayer == nullptr)
		return E_FAIL;
	m_mapLayer.insert({ L"UI", pUILayer });


	CGameObject* pBasicCart = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Basic_Cart");
	CGameObject* pCottonCart = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CottonCart");
	CGameObject* pChar = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Bazzi");
	if (pBasicCart) {
		pBasicCart->Get_Transform()->Set_Pos({ 0.f, -4.f, 2.f });
	}
	if (pCottonCart) {
		pCottonCart->Get_Transform()->Set_Pos({ 0.f, -4.f, 2.f });
		pCottonCart->Get_Transform()->Set_Scale({ 3.f, 3.f, 3.f });
	}
	if (pChar) {
		pChar->Get_Transform()->Set_Pos({ 0.f, 2.f, 2.f });
		pChar->Get_Transform()->Set_Scale({ 2.f, 2.f, 2.f });
	}

	CGameObject* pUIObject = nullptr;

	pUIObject = CScene3_Map_ForestValley::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_Map_ForestValley", pUIObject)))
		return E_FAIL;


	// ��ư
	pUIObject = CScene3_StartBtn::Create(m_pGraphicDev, MAP_SPEED);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_StartBtn", pUIObject)))
		return E_FAIL;
	//CButtonMgr::GetInstance()->AddBtntoVec(pUIObject);


	CGameObject* pCharBtnObject = CScene3_CharBtn::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_CharBtn", pCharBtnObject)))
		return E_FAIL;
	CButtonMgr::GetInstance()->AddBtntoVec(pCharBtnObject);


	pUIObject = CScene3_KartBtn::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_KartBtn", pUIObject)))
		return E_FAIL;
	CButtonMgr::GetInstance()->AddBtntoVec(pUIObject);


	pUIObject = CScene3_ColorBtn::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_ColorBtn", pUIObject)))
		return E_FAIL;
	CButtonMgr::GetInstance()->AddBtntoVec(pUIObject);


	pUIObject = CScene3_ColorSetBG::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_ColorSetBG", pUIObject)))
		return E_FAIL;
	//CButtonMgr::GetInstance()->AddBtntoVec(pUIObject);

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


	// īƮ ����1
	CGameObject* pUIInvenSlot = CUI_InvenSlot::Create(m_pGraphicDev, INVEN_FIRST);
	if (nullptr == pUIInvenSlot)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_InvenSlot", pUIInvenSlot)))
		return E_FAIL;
	pUIInvenSlot->Get_Transform()->Set_Pos({ -330, 10, 1 });
	pUIInvenSlot->Get_Transform()->Set_Scale({ 100,150,1 });


	pUIObject = CInvenSlotBG::Create(m_pGraphicDev, INVEN_FIRST);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"InvenSlotBG", pUIObject)))
		return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot)->SetBG(pUIObject);

	//pUIObject = CInvenSlotCart::Create(m_pGraphicDev, INVEN_FIRST);
	//if (pUIObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"InvenSlotCart", pUIObject)))
	//	return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot)->SetItem(pBasicCart);

	CSlotMgr::GetInstance()->AddCartSlot(pUIInvenSlot, INVEN_FIRST);



	// īƮ ����2
	CGameObject* pUIInvenSlot2 = CUI_InvenSlot::Create(m_pGraphicDev, INVEN_SECOND);
	if (nullptr == pUIInvenSlot2)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_InvenSlot2", pUIInvenSlot2)))
		return E_FAIL;
	pUIInvenSlot2->Get_Transform()->Set_Pos({ -230, 10, 1 });
	pUIInvenSlot2->Get_Transform()->Set_Scale({ 100,150,1 });


	pUIObject = CInvenSlotBG::Create(m_pGraphicDev, INVEN_SECOND);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"InvenSlotBG2", pUIObject)))
		return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot2)->SetBG(pUIObject);

	//pUIObject = CInvenSlotCart::Create(m_pGraphicDev, INVEN_SECOND);
	//if (pUIObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"InvenSlotCart2", pUIObject)))
	//	return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot2)->SetItem(pCottonCart);

	CSlotMgr::GetInstance()->AddCartSlot(pUIInvenSlot2, INVEN_SECOND);


	// ĳ���� ����1
	CGameObject* pUICharSlot = CScene3_CharSlot::Create(m_pGraphicDev, CHAR_BAZZI);
	if (pUICharSlot == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"CharSlot1", pUICharSlot)))
		return E_FAIL;
	pUICharSlot->Get_Transform()->Set_Pos({ -330, 120, 1 });
	pUICharSlot->Get_Transform()->Set_Scale({ 120,120,1 });

	pUIObject = CScene3_CharBG::Create(m_pGraphicDev, CHAR_BAZZI);
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"CharSlotBG", pUIObject)))
		return E_FAIL;
	static_cast<CScene3_CharSlot*>(pUICharSlot)->SetBG(pUIObject);

	//pUIObject = CScene3_Char::Create(m_pGraphicDev, CHAR_BAZZI);
	//if (pUIObject == nullptr)
	//	return E_FAIL;
	//if (FAILED(pUILayer->Add_GameObject(L"CharSlotChar", pUIObject)))
	//	return E_FAIL;
	static_cast<CScene3_CharSlot*>(pUICharSlot)->SetChar(pChar);

	CSlotMgr::GetInstance()->AddCharSlot(pUICharSlot, CHAR_BAZZI);



	//// ĳ���� ����2
	//	CGameObject* pUICharSlot2 = CScene3_CharSlot::Create(m_pGraphicDev, CHAR_DAO);
	//	if (pUICharSlot == nullptr)
	//		return E_FAIL;
	//	if (FAILED(pUILayer->Add_GameObject(L"CharSlot2", pUICharSlot2)))
	//		return E_FAIL;
	//	pUICharSlot2->Get_Transform()->Set_Pos({ -230, 120, 1 });
	//	pUICharSlot2->Get_Transform()->Set_Scale({ 120,120,1 });
	//	
	//	pUIObject = CScene3_CharBG::Create(m_pGraphicDev, CHAR_DAO);
	//	if (pUIObject == nullptr)
	//		return E_FAIL;
	//	if (FAILED(pUILayer->Add_GameObject(L"CharSlotBG2", pUIObject)))
	//		return E_FAIL;
	//	static_cast<CScene3_CharSlot*>(pUICharSlot2)->SetBG(pUIObject);
	//
	//	pUIObject = CScene3_Char::Create(m_pGraphicDev, CHAR_DAO);
	//	if (pUIObject == nullptr)
	//		return E_FAIL;
	//	if (FAILED(pUILayer->Add_GameObject(L"CharSlotChar2", pUIObject)))
	//		return E_FAIL;
	//	static_cast<CScene3_CharSlot*>(pUICharSlot2)->SetChar(pUIObject);
	//
	//	CSlotMgr::GetInstance()->AddCharSlot(pUICharSlot2, CHAR_DAO);
	//	
	//	pUIObject = CUI_UnderBar::Create(m_pGraphicDev);
	//	if (nullptr == pUIObject)
	//		return E_FAIL;
	//	if (FAILED(pUILayer->Add_GameObject(L"UI_UnderBar", pUIObject)))
	//		return E_FAIL;

	// ���� ��ư
	pUIObject = CUI_XButton::Create(m_pGraphicDev);
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_XButton", pUIObject)))
		return E_FAIL;

	return S_OK;


}



CMenu_Set_Speed* CMenu_Set_Speed::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMenu_Set_Speed* pMenu = new CMenu_Set_Speed(pGraphicDev);

	if (FAILED(pMenu->Ready_Scene()))
	{
		Safe_Release(pMenu);
		MSG_BOX("CMenu_Set_Speed Create Failed22");
		return nullptr;
	}

	return pMenu;
}

void CMenu_Set_Speed::Free()
{
	CSlotMgr::GetInstance()->Clear();
	CButtonMgr::GetInstance()->Clear();

	CScene::Free();
	CRenderer::GetInstance()->Clear_RenderGroup();
	CRenderer::GetInstance()->Delete_RenderTarget(L"InvenSlot0");
	CRenderer::GetInstance()->Delete_RenderTarget(L"InvenSlot1");
	CRenderer::GetInstance()->Delete_RenderTarget(L"CharSlot0");
	CRenderer::GetInstance()->Delete_RenderTarget(L"CharSlot1");
}