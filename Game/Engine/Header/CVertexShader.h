#pragma once

#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVertexShader :
    public CComponent
{
protected:
    explicit CVertexShader(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CVertexShader(const CVertexShader& rhs);
    virtual ~CVertexShader() override;

public:
    IDirect3DVertexShader9* Get_VS() { return m_pVS; }

protected:
    virtual HRESULT Ready_VertexShader(const wchar_t* pFilePath);

protected:
    IDirect3DVertexShader9* m_pVS = nullptr;
    ID3DXConstantTable* m_pCT = nullptr;

public:
    CComponent* Clone() override;

public:
    virtual void Free();
};

END