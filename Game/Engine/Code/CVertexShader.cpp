#include "CVertexShader.h"

CVertexShader::CVertexShader(LPDIRECT3DDEVICE9 pGraphicDev) 
    : CComponent(pGraphicDev)
{
}

CVertexShader::CVertexShader(const CVertexShader& rhs)
    : CComponent(rhs), m_pVS(rhs.m_pVS), m_pCT(rhs.m_pCT)
{
    m_eID = ID_STATIC;

    if (m_pVS) m_pVS->AddRef();
    if (m_pCT) m_pCT->AddRef();
}

CVertexShader::~CVertexShader()
{
}

HRESULT CVertexShader::Ready_VertexShader(const wchar_t* pFilePath)
{
    ID3DXBuffer* shader = 0;
    ID3DXBuffer* errorBuffer = 0;

    HRESULT hr = D3DXCompileShaderFromFile(
        pFilePath,
        0,
        0,
        "Main",
        "vs_2_0",
        D3DXSHADER_DEBUG,
        &shader,
        &errorBuffer,
        &m_pCT);

    if (errorBuffer) {
        TCHAR buf[256];
        swprintf_s(buf, 256, L"%hs", (char*)errorBuffer->GetBufferPointer());
        MSG_BOXF(buf);
        Safe_Release(errorBuffer);
    }
    if (FAILED(hr)) {
        MSG_BOX("D3DXCompileShaderFromFile - FAILED");
        return E_FAIL;
    }

    hr = m_pGraphicDev->CreateVertexShader(
        (DWORD*)shader->GetBufferPointer(),
        &m_pVS);

    if (FAILED(hr))
    {
        MSG_BOX("CreateVertexShader - FAILED");
        return E_FAIL;
    }

    Safe_Release(shader);
}

CComponent* CVertexShader::Clone()
{
    CComponent* pComp = new CVertexShader(*this);

    return pComp;
}

void CVertexShader::Free()
{
    Safe_Release(m_pVS);
    Safe_Release(m_pCT);
    CComponent::Free();
}
