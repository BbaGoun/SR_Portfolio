#include "pch.h"
#include "CDiffuseShader.h"

CDiffuseShader::CDiffuseShader(LPDIRECT3DDEVICE9 pGraphicDev)
    : CVertexShader(pGraphicDev)
{
}

CDiffuseShader::~CDiffuseShader()
{
}

HRESULT CDiffuseShader::Ready_VertexShader(const wchar_t* pFilePath)
{
    CVertexShader::Ready_VertexShader
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

void CDiffuseShader::Free()
{
    CVertexShader::Free();
}
