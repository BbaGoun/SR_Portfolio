#include "pch.h"
#include "CItem.h"
#include "CMissile.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_Scene()
{
	if (FAILED(Ready_GameLogic_Layer()))
		return E_FAIL;

	return S_OK;
}

void CItem::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
	CScene::FixedUpdate_Scene(fFixedDeltaTime);
}

_int CItem::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = CScene::Update_Scene(fDeltaTime);
	return iExit;
}

void CItem::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CItem::Render_Scene()
{
	//CScene::Render_Scene();
}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CItem* pScene = new CItem(pGraphicDev);

	if (FAILED(pScene->Ready_Scene())) {
		MSG_BOX("CItem Create Failed");
		Safe_Release(pScene);
		return nullptr;
	}

	return pScene;
}

HRESULT CItem::Ready_GameLogic_Layer()
{
	CLayer* pGameObjectLayer = CLayer::Create();

	if (pGameObjectLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"GameLogic", pGameObjectLayer });

	CGameObject* pMissile = CMissile::Create(m_pGraphicDev);

	if (pMissile == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Missile", pMissile)))
		return E_FAIL;

	return S_OK;
}

void CItem::Free()
{
	CScene::Free();
}
