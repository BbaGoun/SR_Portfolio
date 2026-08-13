#include "CCube.h"

CCube::CCube() : CVIBuffer()
{
}

CCube::CCube(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CCube::CCube(const CCube& rhs) : CVIBuffer(rhs)
{
}

CCube::~CCube()
{
}

HRESULT CCube::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 8;
	m_dwTriCnt = 12;
	m_dwFVF = FVF_COL;

	m_dwIdxCnt = 36;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCOL* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0].vPosition = { -1, 1, 1 };
	vertices[0].dwColor = 0xff0000ff;

	vertices[1].vPosition = { 1, 1, 1 };
	vertices[1].dwColor = 0xff0000ff;

	vertices[2].vPosition = { 1, -1, 1 };
	vertices[2].dwColor = 0xff0000ff;

	vertices[3].vPosition = { -1, -1, 1 };
	vertices[3].dwColor = 0xff0000ff;

	vertices[4].vPosition = { -1, 1, -1 };
	vertices[4].dwColor = 0xffff0000;

	vertices[5].vPosition = { 1, 1, -1 };
	vertices[5].dwColor = 0xffff0000;

	vertices[6].vPosition = { 1, -1, -1 };
	vertices[6].dwColor = 0xffff0000;

	vertices[7].vPosition = { -1, -1, -1 };
	vertices[7].dwColor = 0xffff0000;

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(vertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	// µÞ ¸é
	indices[0]._0 = 2;
	indices[0]._1 = 1;
	indices[0]._2 = 0;

	indices[1]._0 = 3;
	indices[1]._1 = 2;
	indices[1]._2 = 0;

	// À­ ¸é
	indices[2]._0 = 1;
	indices[2]._1 = 5;
	indices[2]._2 = 4;

	indices[3]._0 = 0;
	indices[3]._1 = 1;
	indices[3]._2 = 4;

	// ¾Õ ¸é
	indices[4]._0 = 7;
	indices[4]._1 = 4;
	indices[4]._2 = 5;

	indices[5]._0 = 6;
	indices[5]._1 = 7;
	indices[5]._2 = 5;

	// ¾Æ·§ ¸é
	indices[6]._0 = 6;
	indices[6]._1 = 2;
	indices[6]._2 = 3;

	indices[7]._0 = 7;
	indices[7]._1 = 6;
	indices[7]._2 = 3;

	// ¿ÞÂÊ ¸é
	indices[8]._0 = 3;
	indices[8]._1 = 0;
	indices[8]._2 = 4;

	indices[9]._0 = 7;
	indices[9]._1 = 3;
	indices[9]._2 = 4;

	// ¿À¸¥ÂÊ ¸é
	indices[10]._0 = 6;
	indices[10]._1 = 5;
	indices[10]._2 = 1;

	indices[11]._0 = 2;
	indices[11]._1 = 6;
	indices[11]._2 = 1;

	m_pIB->Unlock();

	return S_OK;
}

CCube* CCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCube* pInnerBox = new CCube(pGraphicDev);

	if (FAILED(pInnerBox->Ready_Buffer())) {
		Safe_Release(pInnerBox);
		MSG_BOX("pInnerBox Create Fail");
		return nullptr;
	}

	return pInnerBox;
}

CComponent* CCube::Clone()
{
	CComponent* pComp = new CCube(*this);

	return pComp;
}

void CCube::Free()
{
	CVIBuffer::Free();
}
