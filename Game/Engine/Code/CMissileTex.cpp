#include "CMissileTex.h"

CMissileTex::CMissileTex()
{
}

CMissileTex::CMissileTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CMissileTex::CMissileTex(const CMissileTex& rhs)
	: CVIBuffer(rhs)
{
}

CMissileTex::~CMissileTex()
{
}

HRESULT CMissileTex::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCUBE);	 
	m_dwVtxCnt = 16;
	m_dwTriCnt = 16;

	m_dwFVF = FVF_CUBE;	  
	m_dwIdxCnt = 48;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCUBE* pVertex = NULL;

	// &pVertex : 버텍스 버퍼에 저장된 버텍스 중 첫번째 버텍스의 주소를 얻어옴
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { -0.5f,  0.5f, -2.f };
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = { 0.5f,  0.5f, -2.f };
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = { 0.5f, -0.5f, -2.f };
	pVertex[2].vTexUV = pVertex[2].vPosition;

	pVertex[3].vPosition = { -0.5f, -0.5f, -2.f };
	pVertex[3].vTexUV = pVertex[3].vPosition;

	pVertex[4].vPosition = { -0.5f,  0.5f,  2.f };
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = { 0.5f,  0.5f,  2.f };
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = { 0.5f, -0.5f,  2.f };
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = { -0.5f, -0.5f,  2.f };
	pVertex[7].vTexUV = pVertex[7].vPosition;

	pVertex[8].vPosition = { -0.5f, 0.f, -1.5f };
	pVertex[8].vTexUV = pVertex[8].vPosition;

	pVertex[9].vPosition = { -2.f, 0.f, -1.5f };
	pVertex[9].vTexUV = pVertex[9].vPosition;

	pVertex[10].vPosition = { -2.f, 0.f, -0.5f };
	pVertex[10].vTexUV = pVertex[10].vPosition;

	pVertex[11].vPosition = { -0.5f, 0.f, -0.5f };
	pVertex[11].vTexUV = pVertex[11].vPosition;

	pVertex[12].vPosition = { 0.5f, 0.f, -1.5f };
	pVertex[12].vTexUV = pVertex[12].vPosition;

	pVertex[13].vPosition = { 2.f, 0.f, -1.5f };
	pVertex[13].vTexUV = pVertex[13].vPosition;

	pVertex[14].vPosition = { 2.f, 0.f, -0.5f };
	pVertex[14].vTexUV = pVertex[14].vPosition;

	pVertex[15].vPosition = { 0.5f, 0.f, -0.5f };
	pVertex[15].vTexUV = pVertex[15].vPosition;

	m_pVB->Unlock();

	INDEX32* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	pIndex[12]._0 = 8;
	pIndex[12]._1 = 9;
	pIndex[12]._2 = 10;

	pIndex[13]._0 = 8;
	pIndex[13]._1 = 10;
	pIndex[13]._2 = 11;


	pIndex[14]._0 = 12;
	pIndex[14]._1 = 14;
	pIndex[14]._2 = 13;

	pIndex[15]._0 = 12;
	pIndex[15]._1 = 15;
	pIndex[15]._2 = 14;

	m_pIB->Unlock();

	return S_OK;
}


void CMissileTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CMissileTex* CMissileTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMissileTex* pCubeTex = new CMissileTex(pGraphicDev);

	if (FAILED(pCubeTex->Ready_Buffer()))
	{
		Safe_Release(pCubeTex);
		MSG_BOX("pCubeTex Create Failed");
		return nullptr;
	}

	return pCubeTex;
}

CComponent* CMissileTex::Clone()
{
	return new CMissileTex(*this);
}

void CMissileTex::Free()
{
	CVIBuffer::Free();
}
