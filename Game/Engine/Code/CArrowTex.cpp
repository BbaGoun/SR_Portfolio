#include "CArrowTex.h"

CArrowTex::CArrowTex(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CArrowTex::CArrowTex(const CArrowTex& rhs) : CVIBuffer(rhs)
{
}

CArrowTex::~CArrowTex()
{
}

HRESULT CArrowTex::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;
	m_dwFVF = FVF_COL;

	m_dwIdxCnt = 6;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCOL* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0].vPosition = { 0.f, 0.f,1.f };
	vertices[0].dwColor = D3DCOLOR_XRGB(255,0,0);

	vertices[1].vPosition = { -1.f, 0.f,-1.f };
	vertices[1].dwColor = D3DCOLOR_XRGB(255, 0, 0);

	vertices[2].vPosition = { 0.f, 0.f,-0.5f };
	vertices[2].dwColor = D3DCOLOR_XRGB(255, 0, 0);

	vertices[3].vPosition = { 1.f, 0.f,-1.f };
	vertices[3].dwColor = D3DCOLOR_XRGB(255, 0, 0);

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	indices[0]._0 = 0;
	indices[0]._1 = 2;
	indices[0]._2 = 1;

	indices[1]._0 = 0;
	indices[1]._1 = 3;
	indices[1]._2 = 2;

	m_pIB->Unlock();

	return S_OK;
}

CArrowTex* CArrowTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CArrowTex* pCArrowTex = new CArrowTex(pGraphicDev);

	if (FAILED(pCArrowTex->Ready_Buffer())) {
		Safe_Release(pCArrowTex);
		MSG_BOX("CArrowTex Create Fail");
		return nullptr;
	}
	return pCArrowTex;
}

CComponent* CArrowTex::Clone()
{
	CComponent* pComp = new CArrowTex(*this);

	return pComp;
}

void CArrowTex::Free()
{
	CVIBuffer::Free();
}
