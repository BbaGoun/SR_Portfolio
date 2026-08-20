#include "CCartBodyCol.h"

CCartBodyCol::CCartBodyCol(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CCartBodyCol::CCartBodyCol(const CCartBodyCol& rhs) : CVIBuffer(rhs)
{
}

CCartBodyCol::~CCartBodyCol()
{
}

HRESULT CCartBodyCol::Ready_Buffer()
{
    m_dwVtxSize = sizeof(VTXCOL);
    m_dwVtxCnt = 8;
    m_dwTriCnt = 12;
    m_dwFVF = FVF_COL;

    m_dwIdxCnt = m_dwTriCnt * 3;
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    VTXCOL* vertices = nullptr;

    m_pVB->Lock(0, 0, (void**)&vertices, 0);

    vertices[0].vPosition = { -2.5f, 1.f, 5.f };
    vertices[0].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    vertices[1].vPosition = { 2.5f, 1.f, 5.f };
    vertices[1].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    vertices[2].vPosition = { 2.5f, -1.f, 5.f };
    vertices[2].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    vertices[3].vPosition = { -2.5f, -1.f, 5.f };
    vertices[3].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

    vertices[4].vPosition = { -2.5f, 1.f, -5.f };
    vertices[4].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    vertices[5].vPosition = { 2.5f, 1.f, -5.f };
    vertices[5].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    vertices[6].vPosition = { 2.5f, -1.f, -5.f };
    vertices[6].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    vertices[7].vPosition = { -2.5f, -1.f, -5.f };
    vertices[7].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

    m_pVB->Unlock();

    INDEX32* indices = nullptr;

    m_pIB->Lock(0, 0, (void**)&indices, 0);


    indices[0]._0 = 3;
    indices[0]._1 = 1;
    indices[0]._2 = 0;

    indices[1]._0 = 3;
    indices[1]._1 = 2;
    indices[1]._2 = 1;

    indices[2]._0 = 7;
    indices[2]._1 = 4;
    indices[2]._2 = 5;

    indices[3]._0 = 7;
    indices[3]._1 = 5;
    indices[3]._2 = 6;

    indices[4]._0 = 6;
    indices[4]._1 = 5;
    indices[4]._2 = 1;

    indices[5]._0 = 6;
    indices[5]._1 = 1;
    indices[5]._2 = 2;

    indices[6]._0 = 7;
    indices[6]._1 = 0;
    indices[6]._2 = 4;

    indices[7]._0 = 7;
    indices[7]._1 = 3;
    indices[7]._2 = 0;

    indices[8]._0 = 4;
    indices[8]._1 = 0;
    indices[8]._2 = 1;

    indices[9]._0 = 4;
    indices[9]._1 = 1;
    indices[9]._2 = 5;

    indices[10]._0 = 7;
    indices[10]._1 = 2;
    indices[10]._2 = 3;

    indices[11]._0 = 7;
    indices[11]._1 = 6;
    indices[11]._2 = 2;

    m_pIB->Unlock();

    return S_OK;
}

CCartBodyCol* CCartBodyCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CCartBodyCol* pCCartBodyCol = new CCartBodyCol(pGraphicDev);

    if (FAILED(pCCartBodyCol->Ready_Buffer())) {
        Safe_Release(pCCartBodyCol);
        MSG_BOX("CCartBodyCol Create Fail");
        return nullptr;
    }

    return pCCartBodyCol;
}

CComponent* CCartBodyCol::Clone()
{
    CComponent* pComp = new CCartBodyCol(*this);

    return pComp;
}

void CCartBodyCol::Free()
{
    CVIBuffer::Free();
}
