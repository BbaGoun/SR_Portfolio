#include "CGameUI.h"

CGameUI::CGameUI(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev), m_pSprite(nullptr), m_pFont(nullptr)
{
    m_pGraphicDev->AddRef();
}

CGameUI::~CGameUI()
{
}

HRESULT CGameUI::Ready_UI(GAME_UI _eID, CGameUI* _pGameUI)
{
    if (_pGameUI == nullptr)
    {
        return E_FAIL;
    }

    m_GameUIs[_eID] = _pGameUI;

    return S_OK;
}




void CGameUI::Free()
{
}
