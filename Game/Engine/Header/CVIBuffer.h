#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer :
    public CComponent
{
protected:
    explicit CVIBuffer();
    explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CVIBuffer(const CVIBuffer& rhs);
    virtual ~CVIBuffer() override;

public:
    LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() { return m_pVB; }
    LPDIRECT3DINDEXBUFFER9 GetIndexBuffer() { return m_pIB; }

    virtual HRESULT Ready_Buffer();
    virtual void Render_Buffer();

    DirectX::BoundingBox* GetBoundingBox(DirectX::BoundingBox* pBoundingBox) {
        DirectX::XMVECTOR vMin = ToXMVec(m_minVtx);
        DirectX::XMVECTOR vMax = ToXMVec(m_maxVtx);
        DirectX::BoundingBox::CreateFromPoints(*pBoundingBox, vMin, vMax);
        return pBoundingBox;
    }

protected:
    void UpdateMinMaxVtx(_vec3 position);

public:
    CComponent* Clone() override;

protected:
    LPDIRECT3DVERTEXBUFFER9		m_pVB;

    DWORD						m_dwVtxSize;
    DWORD						m_dwVtxCnt;
    DWORD						m_dwTriCnt;
    DWORD						m_dwFVF;

    LPDIRECT3DVERTEXDECLARATION9 m_pVtxDecl;

    LPDIRECT3DINDEXBUFFER9		m_pIB;
    DWORD						m_dwIdxCnt;
    D3DFORMAT					m_IdxFmt;

    _vec3                       m_minVtx;
    _vec3                       m_maxVtx;

protected:
    virtual void		Free();
};

END