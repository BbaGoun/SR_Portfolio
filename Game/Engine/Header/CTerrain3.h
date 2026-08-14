#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrain3 :
    public CVIBuffer
{
protected:
    explicit CTerrain3();
    explicit CTerrain3(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTerrain3(const CTerrain3& rhs);
    virtual ~CTerrain3() override;

public:
    vector<VTXTC>& GetVertices() { return m_vecVertices; }
    vector<FACE32>& GetFaces() { return m_vecFaces; }

public:
    HRESULT Ready_Buffer() override;
    void Render_Buffer() override;

    static CTerrain3* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CComponent* Clone() override;
    void        Set_SkidMark(_vec3 vPos);
    int         GetVTXCNTX() { return m_iVTXCNTX; }
    int         GetVTXCNTZ() { return m_iVTXCNTZ; }

private:
    _float ComputeShade(_vec3* normal, _vec3* dirToLight);

private:
    vector<VTXTC>       m_vecVertices;
    vector<FACE32>      m_vecFaces;
    LPDIRECT3DTEXTURE9  m_pTexture;
    int                 m_iVTXCNTX;
    int                 m_iVTXCNTZ;

protected:
    virtual void		Free() override;
};

END