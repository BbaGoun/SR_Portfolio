#include "pch.h"
#include "CMapTest.h"

CMapTest::CMapTest(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{

}

CMapTest::~CMapTest()
{

}

HRESULT CMapTest::Ready_Scene()
{
	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;

	if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
		return E_FAIL;

	if (FAILED(Ready_UI_Layer(L"UI_Layer")))
		return E_FAIL;

	return S_OK;
}

void CMapTest::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
	CScene::FixedUpdate_Scene(fFixedDeltaTime);
}

_int CMapTest::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = CScene::Update_Scene(fDeltaTime);

	return iExit;
}

void CMapTest::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);
}

void CMapTest::Render_Scene()
{

}

HRESULT CMapTest::Ready_Environment_Layer(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();

	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMapTest::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();

	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMapTest::Ready_UI_Layer(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();

	if (nullptr == pLayer)
		return E_FAIL;

	CGameObject* pGameObject = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMapTest::Ready_Prototype()
{
	return S_OK;
}

CMapTest* CMapTest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMapTest* pMapTest = new CMapTest(pGraphicDev);

	if (FAILED(pMapTest->Ready_Scene()))
	{
		Safe_Release(pMapTest);
		MSG_BOX("pMapTest Create Failed");
		return nullptr;
	}

	return pMapTest;
}

void CMapTest::Free()
{
	CScene::Free();
}