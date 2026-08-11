#include "CRcTex.h"

CRcTex::CRcTex() : CVIBuffer()
{
}

CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CRcTex::CRcTex(const CRcTex& rhs) : CVIBuffer(rhs)
{
}

CRcTex::~CRcTex()
{
}

HRESULT CRcTex::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;
	m_dwFVF = FVF_TEX;

	m_dwIdxCnt = 6;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0].vPosition = { -1, 1, 0 };
	vertices[0].vTexUV = {0.f, 0.f};

	vertices[1].vPosition = { 1, 1, 0 };
	vertices[1].vTexUV = { 1.f, 0.f };

	vertices[2].vPosition = { 1, -1, 0 };
	vertices[2].vTexUV = { 1.f, 1.f };

	vertices[3].vPosition = { -1, -1, 0 };
	vertices[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	indices[0]._0 = 0;
	indices[0]._1 = 1;
	indices[0]._2 = 2;

	indices[1]._0 = 0;
	indices[1]._1 = 2;
	indices[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

CRcTex* CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex* pCRcTex = new CRcTex(pGraphicDev);

	if (FAILED(pCRcTex->Ready_Buffer())) {
		Safe_Release(pCRcTex);
		MSG_BOX("CRcTex Create Fail");
		return nullptr;
	}
	return pCRcTex;
}

CComponent* CRcTex::Clone()
{
	CComponent* pComp = new CRcTex(*this);

	return pComp;
}

void CRcTex::Free()
{
	CVIBuffer::Free();
}
