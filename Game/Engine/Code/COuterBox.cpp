#include "COuterBox.h"

COuterBox::COuterBox(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

COuterBox::COuterBox(const COuterBox& rhs) : CVIBuffer(rhs)
{
}

COuterBox::~COuterBox()
{
}

HRESULT COuterBox::Ready_Buffer()
{
	//m_dwVtxSize = sizeof(VTXTEX);
	//m_dwVtxCnt = 14;
	//m_dwTriCnt = 12;
	//m_dwFVF = FVF_TEX;

	//m_dwIdxCnt = 36;
	//m_IdxFmt = D3DFMT_INDEX32;

	//if (FAILED(CVIBuffer::Ready_Buffer()))
	//	return E_FAIL;

	//VTXTEX* vertices = nullptr;

	//m_pVB->Lock(0, 0, (void**)&vertices, 0);

	//vertices[0].vPosition = { -1, 1, 1 };
	//vertices[0].vTexUV = { 0.25, 0.33333 };

	//vertices[1].vPosition = { 1, 1, 1 };
	//vertices[1].vTexUV = {0.5, 0.33333 };

	//vertices[2].vPosition = { 1, -1, 1 };
	//vertices[2].vTexUV = {0.5, 0.66666 };

	//vertices[3].vPosition = { -1, -1, 1 };
	//vertices[3].vTexUV = {0.25, 0.66666};

	//vertices[4].vPosition = { -1, 1, -1 };
	//vertices[4].vTexUV = {0, 0.33333};

	//vertices[5].vPosition = { 1, 1, -1 };
	//vertices[5].vTexUV = {0.75, 0.33333};

	//vertices[6].vPosition = { 1, -1, -1 };
	//vertices[6].vTexUV = {0.75, 0.66666};

	//vertices[7].vPosition = { -1, -1, -1 };
	//vertices[7].vTexUV = {0, 0.66666};

	//// 전개도를 닫기 위한 여분
	//vertices[8].vPosition = { -1, 1, -1 };
	//vertices[8].vTexUV = { 0.25, 0 };

	//vertices[9].vPosition = { 1, 1, -1 };
	//vertices[9].vTexUV = { 0.5, 0 };

	//vertices[10].vPosition = { 1, -1, -1 };
	//vertices[10].vTexUV = { 0.5, 0.99999 };

	//vertices[11].vPosition = { -1, -1, -1 };
	//vertices[11].vTexUV = {0.25, 0.99999 };

	////
	//vertices[12].vPosition = { -1, 1, -1 };
	//vertices[12].vTexUV = { 0.99999, 0.33333 };

	//vertices[13].vPosition = { -1, -1, -1 };
	//vertices[13].vTexUV = { 0.99999, 0.66666};

	//m_pVB->Unlock();

	//INDEX32* indices = nullptr;

	//m_pIB->Lock(0, 0, (void**)&indices, 0);

	//// (0, 0)면
	//indices[0]._0 = 0;
	//indices[0]._1 = 1;
	//indices[0]._2 = 2;

	//indices[1]._0 = 0;
	//indices[1]._1 = 2;
	//indices[1]._2 = 3;

	//// (1, 0)면
	//indices[2]._0 = 4;
	//indices[2]._1 = 0;
	//indices[2]._2 = 3;

	//indices[3]._0 = 4;
	//indices[3]._1 = 3;
	//indices[3]._2 = 7;

	//// (1, 2)면
	//indices[4]._0 = 1;
	//indices[4]._1 = 5;
	//indices[4]._2 = 6;

	//indices[5]._0 = 1;
	//indices[5]._1 = 6;
	//indices[5]._2 = 2;

	//// (0, 1)면
	//indices[6]._0 = 8;
	//indices[6]._1 = 9;
	//indices[6]._2 = 1;

	//indices[7]._0 = 8;
	//indices[7]._1 = 1;
	//indices[7]._2 = 0;

	//// (2, 1)면
	//indices[8]._0 = 3;
	//indices[8]._1 = 2;
	//indices[8]._2 = 10;

	//indices[9]._0 = 3;
	//indices[9]._1 = 10;
	//indices[9]._2 = 11;

	//// (1, 3)면
	//indices[10]._0 = 5;
	//indices[10]._1 = 12;
	//indices[10]._2 = 13;

	//indices[11]._0 = 5;
	//indices[11]._1 = 13;
	//indices[11]._2 = 6;

	//m_pIB->Unlock();

	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwVtxCnt = 8;
	m_dwTriCnt = 12;
	m_dwFVF = FVF_CUBE;

	m_dwIdxCnt = 36;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCUBE* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0].vPosition = { -1, 1, 1 };
	vertices[0].vTexUV = { -1, 1, 1 };

	vertices[1].vPosition = { 1, 1, 1 };
	vertices[1].vTexUV = { 1, 1, 1 };

	vertices[2].vPosition = { 1, -1, 1 };
	vertices[2].vTexUV = { 1, -1, 1 };

	vertices[3].vPosition = { -1, -1, 1 };
	vertices[3].vTexUV = { -1, -1, 1 };

	vertices[4].vPosition = { -1, 1, -1 };
	vertices[4].vTexUV = { -1, 1, -1 };

	vertices[5].vPosition = { 1, 1, -1 };
	vertices[5].vTexUV = { 1, 1, -1 };

	vertices[6].vPosition = { 1, -1, -1 };
	vertices[6].vTexUV = { 1, -1, -1 };

	vertices[7].vPosition = { -1, -1, -1 };
	vertices[7].vTexUV = { -1, -1, -1 };

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

COuterBox* COuterBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	COuterBox* pOuterBox = new COuterBox(pGraphicDev);

	if (FAILED(pOuterBox->Ready_Buffer())) {
		Safe_Release(pOuterBox);
		MSG_BOX("pOuterBox Create Fail");
		return nullptr;
	}

	return pOuterBox;
}

CComponent* COuterBox::Clone()
{
	CComponent* pComp = new COuterBox(*this);

	return pComp;
}

void COuterBox::Free()
{
	CVIBuffer::Free();
}
