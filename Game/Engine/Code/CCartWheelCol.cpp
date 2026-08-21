#include "CCartWheelCol.h"

CCartWheelCol::CCartWheelCol(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CCartWheelCol::CCartWheelCol(const CCartWheelCol& rhs) : CVIBuffer(rhs)
{
}

CCartWheelCol::~CCartWheelCol()
{
}

HRESULT CCartWheelCol::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 34;
	m_dwTriCnt = 64;
	m_dwFVF = FVF_COL;

	m_dwIdxCnt = m_dwTriCnt * 3;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCOL* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < 16; ++i)
	{
		vertices[i].vPosition = { -0.2, sinf(2 * D3DX_PI / 16 * i), cosf(2 * D3DX_PI / 16 * i) };
		if(i % 4 == 0)
			vertices[i].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		else
			vertices[i].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	}
	for (int i = 16; i < 32; ++i)
	{
		vertices[i].vPosition = { 0.2, sinf(2 * D3DX_PI / 16 * i), cosf(2 * D3DX_PI / 16 * i) };
		if (i % 4 == 2)
			vertices[i].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		else
			vertices[i].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	}
	vertices[32].vPosition = { -0.2, 0, 0 };
	vertices[32].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	vertices[33].vPosition = { 0.2, 0, 0 };
	vertices[33].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(vertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	// -x 방향 원: 삼각형 16개
	for (int i = 0; i < 16; ++i)
	{
		indices[i]._0 = 32;
		indices[i]._1 = i;
		indices[i]._2 = (i + 1) % 16;
	}

	// +x 방향 원: 삼각형 16개
	for (int i = 16; i < 32; ++i)
	{
		indices[i]._0 = i;
		indices[i]._1 = 33;
		indices[i]._2 = i + 1;
	}
	indices[31]._2 = 16;

	// 옆면
	int index = 32;
	for (int i = 0; i < 16; ++i)
	{
		if (i == 15)
		{
			indices[index]._0 = i;
			indices[index]._1 = i + 16;
			indices[index]._2 = 16;
			++index;

			indices[index]._0 = i;
			indices[index]._1 = 16;
			indices[index]._2 = 0;
			++index;
			break;
		}
		indices[index]._0 = i;
		indices[index]._1 = i + 16;
		indices[index]._2 = i + 17;
		++index;

		indices[index]._0 = i;
		indices[index]._1 = i + 17;
		indices[index]._2 = i + 1;
		++index;
	}

	m_pIB->Unlock();

	return S_OK;
}

CCartWheelCol* CCartWheelCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCartWheelCol* pCCartWheelCol = new CCartWheelCol(pGraphicDev);

	if (FAILED(pCCartWheelCol->Ready_Buffer())) {
		Safe_Release(pCCartWheelCol);
		MSG_BOX("CCartWheelCol Create Fail");
		return nullptr;
	}

	return pCCartWheelCol;
}

CComponent* CCartWheelCol::Clone()
{
	CComponent* pComp = new CCartWheelCol(*this);

	return pComp;
}

void CCartWheelCol::Free()
{
	CVIBuffer::Free();
}
