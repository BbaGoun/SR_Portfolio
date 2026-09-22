#pragma once
#include "CVertexShader.h"

class CDiffuseShader :
    public CVertexShader
{
protected:
    explicit CDiffuseShader(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CDiffuseShader(const CDiffuseShader& c);
    virtual ~CDiffuseShader();

private:
    virtual HRESULT Ready_VertexShader(const wchar_t* pFilePath) override;

private:
    D3DXHANDLE ViewMatHandle = 0;
    D3DXHANDLE ViewProjMatHandle = 0;
    D3DXHANDLE AmbientMtrlHandle = 0;
    D3DXHANDLE DiffuseMtrlHandle = 0;
    D3DXHANDLE LightDirHandle = 0;

public:
    static CDiffuseShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wchar_t* pFilePath);

public:
    virtual void Free() override;
};

