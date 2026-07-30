#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrain2 :
    public CVIBuffer
{
protected:
    explicit CTerrain2();
    explicit CTerrain2(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTerrain2(const CTerrain2& rhs);
    virtual ~CTerrain2() override;

public:
    vector<VTXMESH>& GetVertices() { return m_vecVertices; }
    vector<FACE32>& GetFaces() { return m_vecFaces; }

public:
    HRESULT Ready_Buffer() override;
    void Render_Buffer() override;

    static CTerrain2* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;

private:
    _float ComputeShade(_vec3* normal, _vec3* dirToLight);

private:
    vector<VTXMESH>      m_vecVertices;
    vector<FACE32>      m_vecFaces;
    LPDIRECT3DTEXTURE9  m_pTexture;

protected:
    virtual void		Free() override;
};

END