#include "CDiffuseShader.h"
#include "CRenderer.h"

CDiffuseShader::CDiffuseShader(LPDIRECT3DDEVICE9 pGraphicDev)
    : CVertexShader(pGraphicDev)
{
}

CDiffuseShader::CDiffuseShader(const CDiffuseShader& rhs)
    : CVertexShader(rhs),
    m_WorldMatHandle(rhs.m_WorldMatHandle),
    m_ViewMatHandle(rhs.m_ViewMatHandle),
    m_ProjMatHandle(rhs.m_ProjMatHandle),
    m_AmbientMtrlHandle(rhs.m_AmbientMtrlHandle),
    m_DiffuseMtrlHandle(rhs.m_DiffuseMtrlHandle),
    m_LightDirHandle(rhs.m_LightDirHandle)
{
}

CDiffuseShader::~CDiffuseShader()
{
}

void CDiffuseShader::SetConstTable(_matrix* matWorld, _matrix* matView, _matrix* matProj)
{
    m_pCT->SetMatrix(m_pGraphicDev, m_WorldMatHandle, matWorld);
    m_pCT->SetMatrix(m_pGraphicDev, m_ViewMatHandle, matView);
    m_pCT->SetMatrix(m_pGraphicDev, m_ProjMatHandle, matProj);

    D3DXVECTOR4 dirToLight = { -0.57f, 0.57f, -0.57f, 0.f };

    _matrix R;
    if (CRenderer::GetInstance()->Get_Mirror_Pass(R)) {
        D3DXVec4Transform(&dirToLight, &dirToLight, &R);
    }

    m_pCT->SetVector(m_pGraphicDev, m_LightDirHandle, &dirToLight);
}

HRESULT CDiffuseShader::Ready_VertexShader(const wchar_t* pFilePath)
{
    CVertexShader::Ready_VertexShader(pFilePath);

    m_WorldMatHandle = m_pCT->GetConstantByName(0, "WorldMatrix");
    m_ViewMatHandle = m_pCT->GetConstantByName(0, "ViewMatrix");
    m_ProjMatHandle = m_pCT->GetConstantByName(0, "ProjMatrix");
    m_AmbientMtrlHandle = m_pCT->GetConstantByName(0, "AmbientMtrl");
    m_DiffuseMtrlHandle = m_pCT->GetConstantByName(0, "DiffuseMtrl");
    m_LightDirHandle = m_pCT->GetConstantByName(0, "LightDirection");

    D3DXVECTOR4 ambientMtrl = { 1.f, 1.f, 1.f, 1.f };
    D3DXVECTOR4 diffuseMtrl = { 0.2f, 0.2f, 0.2f, 1.f };
    m_pCT->SetVector(m_pGraphicDev, m_AmbientMtrlHandle, &ambientMtrl);
    m_pCT->SetVector(m_pGraphicDev, m_DiffuseMtrlHandle, &diffuseMtrl);
    m_pCT->SetDefaults(m_pGraphicDev);

    return S_OK;
}

CDiffuseShader* CDiffuseShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wchar_t* pFilePath)
{
    CDiffuseShader* pInstance = new CDiffuseShader(pGraphicDev);

    if (FAILED(pInstance->Ready_VertexShader(pFilePath))) {
        MSG_BOX("CDiffuseShader Fail Ready_VertexShader");
        return nullptr;
    }

    return pInstance;
}

CComponent* CDiffuseShader::Clone()
{
    return new CDiffuseShader(*this);
}

void CDiffuseShader::Free()
{
    CVertexShader::Free();
}
