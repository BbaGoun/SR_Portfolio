#include "pch.h"
#include "CInventoryScene.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CBox.h"
#include "CManagement.h"
#include "CRenderer.h"
#include "CBackGround.h"
#include "CCollisionTest.h"
#include "CDInputMgr.h"
#include "CUI_InvenSlot.h"
#include "CInvenSlotCart.h"
#include "CCartBody1.h"
#include "CInvenSlotBG.h"

CInventoryScene::CInventoryScene(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CInventoryScene::~CInventoryScene()
{
}

HRESULT CInventoryScene::Ready_Scene()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_RenderTarget()))
		return E_FAIL;

	if (FAILED(Ready_GameLogic_Layer()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;

	if (FAILED(Ready_UI_Layer()))
		return E_FAIL;

	if (FAILED(Ready_Collision_Matrix()))
		return E_FAIL;

	return S_OK;
}

void CInventoryScene::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
	CScene::FixedUpdate_Scene(fFixedDeltaTime);
}

_int CInventoryScene::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = CScene::Update_Scene(fDeltaTime);
	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_N))
	{
		Engine::CScene* pStage = CCollisionTest::Create(m_pGraphicDev);

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

void CInventoryScene::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CInventoryScene::Render_Scene()
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

}

void CInventoryScene::OnLostDevice()
{
	CScene::OnLostDevice();
	CRenderer::GetInstance()->OnLostDevice();
}

void CInventoryScene::OnResetDevice()
{
	CScene::OnResetDevice();
	CRenderer::GetInstance()->OnResetDevice(m_pGraphicDev);
}

CInventoryScene* CInventoryScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventoryScene* pScene = new CInventoryScene(pGraphicDev);

	if (FAILED(pScene->Ready_Scene())) {
		MSG_BOX("CInventoryScene Create Failed");
		Safe_Release(pScene);
		return nullptr;
	}

	return pScene;
}

HRESULT CInventoryScene::Ready_Prototype()
{
	return S_OK;
}

HRESULT CInventoryScene::Ready_RenderTarget()
{
	CRenderer::GetInstance()->Add_RenderTarget(m_pGraphicDev, L"InvenSlot0", 250, 400);
	CRenderer::GetInstance()->Add_RenderTarget(m_pGraphicDev, L"InvenSlot1", 250, 400);
	return S_OK;
}

HRESULT CInventoryScene::Ready_GameLogic_Layer()
{
	// 게임 오브젝트
	CLayer* pGameObjectLayer = CLayer::Create();

	if (pGameObjectLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"GameLogic", pGameObjectLayer });

	CGameObject* pGameObject = nullptr;
	return S_OK;
}

HRESULT CInventoryScene::Ready_Environment_Layer()
{
	CLayer* pEnvironmentLayer = CLayer::Create();

	if (pEnvironmentLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"Environment", pEnvironmentLayer });

	CGameObject* pEnvObject = nullptr;

	// BackGround
	//pEnvObject = CBackGround::Create(m_pGraphicDev);
	//dynamic_cast<CBackGround*>(pEnvObject)->Change_BackgroundTexture(BACKGROUND_STARTMENU);
	//
	//if (nullptr == pEnvObject)
	//	return E_FAIL;
	//
	//if (FAILED(pEnvironmentLayer->Add_GameObject(L"BackGround", pEnvObject)))
	//	return E_FAIL;
	//
	//pEnvObject->Get_Transform()->Set_Scale({ WINCX, WINCY, 1 });
	//pEnvObject->Get_Transform()->Set_Pos({ 0, 0, 10 });
	return S_OK;
}


HRESULT CInventoryScene::Ready_UI_Layer()
{
	CLayer* pUILayer = CLayer::Create();

	if (pUILayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"UI", pUILayer });

	CGameObject* pUIObject = nullptr;

	// CUI_InvenSlot
	CGameObject* pUIInvenSlot = CUI_InvenSlot::Create(m_pGraphicDev,INVEN_FIRST);
	if (nullptr == pUIInvenSlot)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_InvenSlot", pUIInvenSlot)))
		return E_FAIL;
	pUIInvenSlot->Get_Transform()->Set_Pos({ 0, 100, 1 });

	// InvenSlotCart
	pUIObject = CInvenSlotCart::Create(m_pGraphicDev, INVEN_FIRST);
	
	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"InvenSlotCart", pUIObject)))
		return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot)->SetItem(pUIObject);
	
	// Inven_SlotBackground
	pUIObject = CInvenSlotBG::Create(m_pGraphicDev, INVEN_FIRST);
	
	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"InvenSlotBG", pUIObject)))
		return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot)->SetBG(pUIObject);
	

	// CUI_InvenSlot
	CGameObject* pUIInvenSlot2 = CUI_InvenSlot::Create(m_pGraphicDev, INVEN_SECOND);
	if (nullptr == pUIInvenSlot2)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"UI_InvenSlot2", pUIInvenSlot2)))
		return E_FAIL;
	pUIInvenSlot2->Get_Transform()->Set_Pos({ 250, 100, 1 });

	// InvenSlotCart
	pUIObject = CInvenSlotCart::Create(m_pGraphicDev, INVEN_SECOND);

	if (nullptr == pUIObject)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"InvenSlotCart2", pUIObject)))
		return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot2)->SetItem(pUIObject);

	// Inven_SlotBackground
	pUIObject = CInvenSlotBG::Create(m_pGraphicDev, INVEN_SECOND);

	if (pUIObject == nullptr)
		return E_FAIL;
	if (FAILED(pUILayer->Add_GameObject(L"InvenSlotBG2", pUIObject)))
		return E_FAIL;
	static_cast<CUI_InvenSlot*>(pUIInvenSlot2)->SetBG(pUIObject);
	return S_OK;
}

HRESULT CInventoryScene::Ready_Collision_Matrix()
{

	return S_OK;
}

void CInventoryScene::Free()
{
	CScene::Free();
	CRenderer::GetInstance()->Clear_RenderGroup();
	CRenderer::GetInstance()->Delete_RenderTarget(L"InvenSlot0");
	CRenderer::GetInstance()->Delete_RenderTarget(L"InvenSlot1");
}
