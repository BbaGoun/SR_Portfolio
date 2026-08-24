#include "CManagement.h"
#include "CRenderer.h"
#include "CEditorLoader.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement() : m_pScene(nullptr), m_pNextScene(nullptr)
{
}

CManagement::~CManagement()
{
    Free();
}

CGameObject* CManagement::Find_GameObjectByTag(const _tchar* pLayerTag, const _tchar* pObjTag)
{
    if (nullptr == m_pScene)
        return nullptr;

    return m_pScene->Find_GameObjectByTag(pLayerTag, pObjTag);
}

const vector<CGameObject*>& CManagement::Find_GameObjectsByTag(const _tchar* pLayerTag, const _tchar* pObjTag)
{
    static vector<CGameObject*> s_empty;

    if (nullptr == m_pScene)
        return s_empty;

    return m_pScene->Find_GameObjectsByTag(pLayerTag, pObjTag);
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

const map<const _tchar*, vector<CGameObject*>>& CManagement::Get_GameObjects(const _tchar* pLayerTag)
{
    static map<const _tchar*, vector<CGameObject*>> s_empty;

    if(nullptr == m_pScene)
        return s_empty;

    return m_pScene->Get_GameObjects(pLayerTag);
}

void CManagement::Delete_GameObject(const _tchar* pLayerTag, CGameObject* _pObj, bool bEditor)
{
    if (FAILED(m_pScene->Delete_GameObject(pLayerTag, _pObj, bEditor)))
    {
        MSG_BOX("Delete Failed");
    }
}

const vector<CGameObject*>& CManagement::Get_Roots(const _tchar* pLayerTag)
{
    static const vector<CGameObject*> s_empty;
    if (!m_pScene)
        return s_empty;
    return m_pScene->Get_Roots(pLayerTag);
}

void CManagement::Attach_Root(CGameObject* _pObj)
{
    if (m_pScene)
        m_pScene->Attach_Root(_pObj);
}

void CManagement::Detach_Root(CGameObject* _pObj)
{
    if (m_pScene)
        m_pScene->Detach_Root(_pObj);
}

void CManagement::Insert_Root_Before(CGameObject* _pDst, CGameObject* _pSrc)
{
    if (m_pScene)
        m_pScene->Insert_Root_Before(_pDst, _pSrc);
}

void CManagement::Insert_Root_After(CGameObject* _pDst, CGameObject* _pSrc)
{
    if (m_pScene)
        m_pScene->Insert_Root_After(_pDst, _pSrc);
}

void CManagement::OnLostDevice()
{
    if (m_pScene)
        m_pScene->OnLostDevice();
}

void CManagement::OnResetDevice()
{
    if (m_pScene)
        m_pScene->OnResetDevice();
}


HRESULT CManagement::Set_Scene(CScene* pScene)
{
    if (nullptr == pScene)
        return  E_FAIL;

    Safe_Release(m_pScene);

    m_pScene = pScene;
    m_pNextScene = nullptr;

    return S_OK;
}

void CManagement::Request_Scene(CScene* pScene)
{
    m_pNextScene = pScene;
}

void CManagement::Change_NextScene()
{
    if (m_pNextScene && m_pNextScene != m_pScene)
        Set_Scene(m_pNextScene);
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

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev, bool bEditor)
{
    if(!bEditor)
        CRenderer::GetInstance()->Render_GameObject(pGraphicDev);

    if (nullptr == m_pScene)
        return;

    m_pScene->Render_Scene();
}

void CManagement::Free()
{
    Safe_Release(m_pScene);
}
