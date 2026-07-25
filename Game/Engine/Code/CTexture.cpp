#include "CTexture.h"

CTexture::CTexture()
{
}

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
    :CComponent(pGraphicDev)
{
}

CTexture::CTexture(const CTexture& rhs)
    :CComponent(rhs)
{
    size_t iSize = rhs.m_vecTexture.size();

    m_vecTexture.reserve(iSize);

    m_vecTexture = rhs.m_vecTexture;

    for (size_t i = 0; i < iSize; ++i)
        m_vecTexture[i]->AddRef();

}

CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(TEXTUREID eID, const _tchar* pPath, const _uint& iCnt)
{
    m_vecTexture.reserve(iCnt);

    IDirect3DBaseTexture9* pTexture = nullptr;

    for (_uint i = 0; i < iCnt; ++i)
    {
        TCHAR   szFileName[128] = L"";

        wsprintf(szFileName, pPath, i);

        switch (eID)
        {
        case TEX_NORMAL:
            if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
                return E_FAIL;

            break;

        case TEX_CUBE:
            if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
                return E_FAIL;
            break;
        }

        m_vecTexture.push_back(pTexture);
    }

    return S_OK;

}

void CTexture::Set_Texture(const _uint& iIndex)
{
    if (m_vecTexture.size() <= iIndex)
        return;

    m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);

}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
    TEXTUREID eID, const _tchar* pPath, const _uint& iCnt)
{
    CTexture* pTexture = new CTexture(pGraphicDev);

    if (FAILED(pTexture->Ready_Texture(eID, pPath, iCnt)))
    {
        Safe_Release(pTexture);
        MSG_BOX("Texture Create Failed");
        return nullptr;
    }

    return pTexture;
}

CComponent* CTexture::Clone()
{
    return new CTexture(*this);
}

void CTexture::Free()
{
    CComponent::Free();

    for_each(m_vecTexture.begin(), m_vecTexture.end(), Safe_Release<IDirect3DBaseTexture9*>);
    m_vecTexture.clear();
}
