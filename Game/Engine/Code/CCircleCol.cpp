#include "CCircleCol.h"

CCircleCol::CCircleCol(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CCircleCol::CCircleCol(const CCircleCol& rhs) : CVIBuffer(rhs)
{
}

CCircleCol::~CCircleCol()
{
}

HRESULT CCircleCol::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 18;
	m_dwTriCnt = 16;
	m_dwFVF = FVF_COL;

	m_dwIdxCnt = m_dwTriCnt * 3;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCOL* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	float fAngle = 0.f;
	for (int i = 0; i <= 16; ++i)
	{
		fAngle = 360.f / 16.f * i;
		vertices[i].vPosition = _vec3({ cosf(D3DXToRadian(fAngle)), 0.f,sinf(D3DXToRadian(fAngle)) }) * 0.5f;
		vertices[i].dwColor = D3DCOLOR_XRGB(0, 0, 255);
	}
	vertices[17].vPosition = _vec3({ 0.f, 0.f,0.f });
	vertices[17].dwColor = D3DCOLOR_XRGB(0, 0, 255);

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < 16; ++i)
	{
		indices[i]._0 = 17;
		indices[i]._1 = i + 1;
		indices[i]._2 = i;
	}

	m_pIB->Unlock();

	return S_OK;
}

CCircleCol* CCircleCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCircleCol* pCCircleCol = new CCircleCol(pGraphicDev);

	if (FAILED(pCCircleCol->Ready_Buffer())) {
		Safe_Release(pCCircleCol);
		MSG_BOX("CCircleCol Create Fail");
		return nullptr;
	}
	return pCCircleCol;
}

CComponent* CCircleCol::Clone()
{
	CComponent* pComp = new CCircleCol(*this);

	return pComp;
}

void CCircleCol::Free()
{
	CVIBuffer::Free();
}
