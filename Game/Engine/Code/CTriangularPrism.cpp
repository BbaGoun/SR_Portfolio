#include "CTriangularPrism.h"

CTriangularPrism::CTriangularPrism() : CVIBuffer()
{
}

CTriangularPrism::CTriangularPrism(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CTriangularPrism::CTriangularPrism(const CTriangularPrism& rhs) : CVIBuffer(rhs)
{
}

CTriangularPrism::~CTriangularPrism()
{
}

HRESULT CTriangularPrism::Ready_Buffer()
{
    m_dwVtxSize = sizeof(VTXCOL);
    m_dwVtxCnt = 6;
    m_dwTriCnt = 8;
    m_dwFVF = FVF_COL;

    m_dwIdxCnt = m_dwTriCnt * 3;
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    VTXCOL* vertices = nullptr;

    m_pVB->Lock(0, 0, (void**)&vertices, 0);

    vertices[0].vPosition = { -1.f, -1.f, 1.f };
    vertices[0].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

    vertices[1].vPosition = { 1.f, -1.f, 1.f };
    vertices[1].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

    vertices[2].vPosition = { -1.f, 1.f, -1.f };
    vertices[2].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    vertices[3].vPosition = { 1.f, 1.f, -1.f };
    vertices[3].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    vertices[4].vPosition = { 1.f, -1.f, -1.f };
    vertices[4].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    vertices[5].vPosition = { -1.f, -1.f, -1.f };
    vertices[5].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

    m_pVB->Unlock();

    INDEX32* indices = nullptr;

    m_pIB->Lock(0, 0, (void**)&indices, 0);


    indices[0]._0 = 5;
    indices[0]._1 = 2;
    indices[0]._2 = 3;
    
    indices[1]._0 = 5;
    indices[1]._1 = 3;
    indices[1]._2 = 4;
    
    indices[2]._0 = 4;
    indices[2]._1 = 3;
    indices[2]._2 = 1;
    
    indices[3]._0 = 0;
    indices[3]._1 = 2;
    indices[3]._2 = 5;
   
    indices[4]._0 = 5;
    indices[4]._1 = 1;
    indices[4]._2 = 0;
   
    indices[5]._0 = 5;
    indices[5]._1 = 4;
    indices[5]._2 = 1;
   
    indices[6]._0 = 0;
    indices[6]._1 = 3;
    indices[6]._2 = 2;
   
    indices[7]._0 = 0;
    indices[7]._1 = 1;
    indices[7]._2 = 3;

    m_pIB->Unlock();

    return S_OK;
}

CTriangularPrism* CTriangularPrism::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTriangularPrism* pCTriangularPrism = new CTriangularPrism(pGraphicDev);

    if (FAILED(pCTriangularPrism->Ready_Buffer())) {
        Safe_Release(pCTriangularPrism);
        MSG_BOX("CTriangularPrism Create Fail");
        return nullptr;
    }

    return pCTriangularPrism;
}

CComponent* CTriangularPrism::Clone()
{
    CComponent* pComp = new CTriangularPrism(*this);

    return pComp;
}

void CTriangularPrism::Free()
{
    CVIBuffer::Free();
}
