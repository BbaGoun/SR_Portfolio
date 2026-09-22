#pragma once
#include "CVertexShader.h"

BEGIN(Engine)

class ENGINE_DLL CDiffuseShader :
    public CVertexShader
{
protected:
    explicit CDiffuseShader(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CDiffuseShader(const CDiffuseShader& rhs);
    virtual ~CDiffuseShader();

public:
    void SetConstTable(_matrix* matWorld, _matrix* matView, _matrix* matProj);

private:
    virtual HRESULT Ready_VertexShader(const wchar_t* pFilePath) override;

private:
    D3DXHANDLE m_WorldMatHandle = 0;
    D3DXHANDLE m_ViewMatHandle = 0;
    D3DXHANDLE m_ProjMatHandle = 0;
    D3DXHANDLE m_AmbientMtrlHandle = 0;
    D3DXHANDLE m_DiffuseMtrlHandle = 0;
    D3DXHANDLE m_LightDirHandle = 0;

public:
    static CDiffuseShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wchar_t* pFilePath);
    CComponent* Clone() override;

public:
    virtual void Free() override;
};

END
