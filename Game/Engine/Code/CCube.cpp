#include "CCube.h"

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
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 8;
	m_dwTriCnt = 12;
	m_dwFVF = FVF_TEX;

	m_dwIdxCnt = 36;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0].vPosition = { -0.5f, 0.5f, 0.5f };
	//vertices[0].dwColor = 0xff0000ff;
	vertices[0].vTexUV = { 0.f, 0.f };

	vertices[1].vPosition = { 0.5f, 0.5f, 0.5f };
	//vertices[1].dwColor = 0xff0000ff;
	vertices[1].vTexUV = { 1.f, 0.f };

	vertices[2].vPosition = { 0.5f, -0.5f, 0.5f };
	//vertices[2].dwColor = 0xff0000ff;
	vertices[2].vTexUV = { 1.f, 1.f };

	vertices[3].vPosition = { -0.5f, -0.5f, 0.5f };
	//vertices[3].dwColor = 0xff0000ff;
	vertices[3].vTexUV = { 0.f, 1.f };

	vertices[4].vPosition = { -0.5f, 0.5f, -0.5f };
	//vertices[4].dwColor = 0xffff0000;
	vertices[4].vTexUV = { 0.f, 0.f };

	vertices[5].vPosition = { 0.5f, 0.5f, -0.5f };
	//vertices[5].dwColor = 0xffff0000;
	vertices[5].vTexUV = { 1.f, 0.f };

	vertices[6].vPosition = { 0.5f, -0.5f, -0.5f };
	//vertices[6].dwColor = 0xffff0000;
	vertices[6].vTexUV = { 1.f, 1.f };

	vertices[7].vPosition = { -0.5f, -0.5f, -0.5f };
	//vertices[7].dwColor = 0xffff0000;
	vertices[7].vTexUV = { 0.f, 1.f };

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(vertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	// 뒷 면
	indices[0]._0 = 2;
	indices[0]._1 = 1;
	indices[0]._2 = 0;

	indices[1]._0 = 3;
	indices[1]._1 = 2;
	indices[1]._2 = 0;

	// 윗 면
	indices[2]._0 = 1;
	indices[2]._1 = 5;
	indices[2]._2 = 4;

	indices[3]._0 = 0;
	indices[3]._1 = 1;
	indices[3]._2 = 4;

	// 앞 면
	indices[4]._0 = 7;
	indices[4]._1 = 4;
	indices[4]._2 = 5;

	indices[5]._0 = 6;
	indices[5]._1 = 7;
	indices[5]._2 = 5;

	// 아랫 면
	indices[6]._0 = 6;
	indices[6]._1 = 2;
	indices[6]._2 = 3;

	indices[7]._0 = 7;
	indices[7]._1 = 6;
	indices[7]._2 = 3;

	// 왼쪽 면
	indices[8]._0 = 3;
	indices[8]._1 = 0;
	indices[8]._2 = 4;

	indices[9]._0 = 7;
	indices[9]._1 = 3;
	indices[9]._2 = 4;

	// 오른쪽 면
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
