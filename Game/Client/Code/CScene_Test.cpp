#include "pch.h"
#include "CScene_Test.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTransform.h"
#include "CEnemy.h"
#include "CSkyBox.h"
#include "CPyramid.h"
#include "CJet.h"
#include "CCamera.h"
#include "CAirplane.h"
#include "CLand.h"
#include "CGOCody.h"

CScene_Test::CScene_Test(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
}

CScene_Test::~CScene_Test()
{
}

HRESULT CScene_Test::Ready_Scene()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_GameObject_Layer()))
		return E_FAIL;

	if (FAILED(Ready_Environment_Layer()))
		return E_FAIL;

	return S_OK;
}

_int CScene_Test::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::Update_Scene(fTimeDelta);
	return iExit;
}

void CScene_Test::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CScene_Test::Render_Scene()
{
	CScene::Render_Scene();
}

CScene_Test* CScene_Test::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Test* pScene = new CScene_Test(pGraphicDev);

	if (FAILED(pScene->Ready_Scene())) {
		MSG_BOX("CScene_Test Create Failed");
		Safe_Release(pScene);
		return nullptr;
	}

	return pScene;
}

HRESULT CScene_Test::Ready_Prototype()
{
	return S_OK;
}

HRESULT CScene_Test::Ready_GameObject_Layer()
{
	// 게임 오브젝트
	CLayer* pGameObjectLayer = CLayer::Create();

	if (pGameObjectLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"GameObject", pGameObjectLayer });

	CGameObject* pGameObject = nullptr;

	pGameObject = CEnemy::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Enemy", pGameObject)))
		return E_FAIL;

	pGameObject = CGOCody::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(pGameObjectLayer->Add_GameObject(L"Obj_Player", pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Test::Ready_Environment_Layer()
{
	CLayer* pEnvironmentLayer = CLayer::Create();

	if (pEnvironmentLayer == nullptr)
		return E_FAIL;

	m_mapLayer.insert({ L"Environment", pEnvironmentLayer });

	CGameObject* pEnvObject = nullptr;
	pEnvObject = CSkyBox::Create(m_pGraphicDev);

	if (pEnvObject == nullptr)
		return E_FAIL;

	if (FAILED(pEnvironmentLayer->Add_GameObject(L"Env_SkyBox", pEnvObject)))
		return E_FAIL;

	pEnvObject = CLand::Create(m_pGraphicDev);

	if (pEnvObject == nullptr)
		return E_FAIL;

	if (FAILED(pEnvironmentLayer->Add_GameObject(L"Env_Land", pEnvObject)))
		return E_FAIL;

	return S_OK;
}

void CScene_Test::Free()
{
	CScene::Free();
}
