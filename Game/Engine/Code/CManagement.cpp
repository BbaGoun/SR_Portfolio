#include "CManagement.h"
#include "CRenderer.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement() : m_pScene(nullptr)
{
}

CManagement::~CManagement()
{
    Free();
}

CComponent* CManagement::Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
    if (nullptr == m_pScene)
        return nullptr;

    return m_pScene->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

void CManagement::Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* pGameObject)
{
    if (pGameObject == nullptr)
    {
        MSG_BOX("Add_GameObject nullptr");
        return;
    }

    if(FAILED(m_pScene->Add_GameObject(pLayerTag, pObjTag, pGameObject)))
        MSG_BOX("Add_GameObject Fail");
}

HRESULT CManagement::Set_Scene(CScene* pScene)
{
    if (nullptr == pScene)
        return  E_FAIL;

    Safe_Release(m_pScene);

    m_pScene = pScene;

    return S_OK;
}

void CManagement::FixedUpdate_Scene(const _float& fFixedDeltaTime)
{
    if (nullptr == m_pScene)
        return;

    m_pScene->FixedUpdate_Scene(fFixedDeltaTime);
}

_int CManagement::Update_Scene(const _float& fDeltaTime)
{
    if (nullptr == m_pScene)
        return -1;

    return m_pScene->Update_Scene(fDeltaTime);
}

void CManagement::LateUpdate_Scene(const _float& fDeltaTime)
{
    if (nullptr == m_pScene)
        return;

    m_pScene->LateUpdate_Scene(fDeltaTime);
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRenderer::GetInstance()->Render_GameObject(pGraphicDev);

    if (nullptr == m_pScene)
        return;

    m_pScene->Render_Scene();
}

void CManagement::Free()
{
    Safe_Release(m_pScene);
}
