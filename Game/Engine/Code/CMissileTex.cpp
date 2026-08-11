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
	// VTXCUBE는 스카이박스를 만들기 위한 용도로 3차원 UV값을 갖는데, 일반적인 물체는 사용하지 않는 포멧
	// 직접 색을 지정하려면 VTXCOL를 쓰고, 텍스처를 입히고 싶으면 VTXTEX를 사용

	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwVtxCnt = 32;
	m_dwTriCnt = 48;

	m_dwFVF = FVF_CUBE;
	m_dwIdxCnt = 144;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCUBE* pVertex = NULL;

	// &pVertex : 버텍스 버퍼에 저장된 버텍스 중 첫번째 버텍스의 주소를 얻어옴
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);


	// 몸통
	pVertex[0].vPosition = { -0.1f,  0.1f,  0.2f };
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = { 0.1f,  0.1f,  0.2f };
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = { 0.1f, -0.1f,  0.2f };
	pVertex[2].vTexUV = pVertex[2].vPosition;

	pVertex[3].vPosition = { -0.1f, -0.1f,  0.2f };
	pVertex[3].vTexUV = pVertex[3].vPosition;

	pVertex[4].vPosition = { -0.1f,  0.1f, -0.15f };
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = { 0.1f,  0.1f, -0.15f };
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = { 0.1f, -0.1f, -0.15f };
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = { -0.1f, -0.1f, -0.15f };
	pVertex[7].vTexUV = pVertex[7].vPosition;


	// 왼쪽 날개
	pVertex[8].vPosition = { -0.2f,  0.05f,  0.05f };
	pVertex[8].vTexUV = pVertex[8].vPosition;

	pVertex[9].vPosition = { -0.1f,  0.05f,  0.05f };
	pVertex[9].vTexUV = pVertex[9].vPosition;

	pVertex[10].vPosition = { -0.1f, -0.05f,  0.05f };
	pVertex[10].vTexUV = pVertex[10].vPosition;

	pVertex[11].vPosition = { -0.2f, -0.05f,  0.05f };
	pVertex[11].vTexUV = pVertex[11].vPosition;

	pVertex[12].vPosition = { -0.2f,  0.05f, -0.1f };
	pVertex[12].vTexUV = pVertex[12].vPosition;

	pVertex[13].vPosition = { -0.1f,  0.05f, -0.1f };
	pVertex[13].vTexUV = pVertex[13].vPosition;

	pVertex[14].vPosition = { -0.1f, -0.05f, -0.1f };
	pVertex[14].vTexUV = pVertex[14].vPosition;

	pVertex[15].vPosition = { -0.2f, -0.05f, -0.1f };
	pVertex[15].vTexUV = pVertex[15].vPosition;


	// 오른쪽 날개
	pVertex[16].vPosition = { 0.1f,  0.05f,  0.05f };
	pVertex[16].vTexUV = pVertex[16].vPosition;

	pVertex[17].vPosition = { 0.2f,  0.05f,  0.05f };
	pVertex[17].vTexUV = pVertex[17].vPosition;

	pVertex[18].vPosition = { 0.2f, -0.05f,  0.05f };
	pVertex[18].vTexUV = pVertex[18].vPosition;

	pVertex[19].vPosition = { 0.1f, -0.05f,  0.05f };
	pVertex[19].vTexUV = pVertex[19].vPosition;

	pVertex[20].vPosition = { 0.1f,  0.05f, -0.1f };
	pVertex[20].vTexUV = pVertex[20].vPosition;

	pVertex[21].vPosition = { 0.2f,  0.05f, -0.1f };
	pVertex[21].vTexUV = pVertex[21].vPosition;

	pVertex[22].vPosition = { 0.2f, -0.05f, -0.1f };
	pVertex[22].vTexUV = pVertex[22].vPosition;

	pVertex[23].vPosition = { 0.1f, -0.05f, -0.1f };
	pVertex[23].vTexUV = pVertex[23].vPosition;


	// 뒤쪽 작은 사각형
	pVertex[24].vPosition = { -0.05f,  0.05f, -0.15f };
	pVertex[24].vTexUV = pVertex[24].vPosition;

	pVertex[25].vPosition = { 0.05f,  0.05f, -0.15f };
	pVertex[25].vTexUV = pVertex[25].vPosition;

	pVertex[26].vPosition = { 0.05f, -0.05f, -0.15f };
	pVertex[26].vTexUV = pVertex[26].vPosition;

	pVertex[27].vPosition = { -0.05f, -0.05f, -0.15f };
	pVertex[27].vTexUV = pVertex[27].vPosition;

	pVertex[28].vPosition = { -0.05f,  0.05f, -0.25f };
	pVertex[28].vTexUV = pVertex[28].vPosition;

	pVertex[29].vPosition = { 0.05f,  0.05f, -0.25f };
	pVertex[29].vTexUV = pVertex[29].vPosition;

	pVertex[30].vPosition = { 0.05f, -0.05f, -0.25f };
	pVertex[30].vTexUV = pVertex[30].vPosition;

	pVertex[31].vPosition = { -0.05f, -0.05f, -0.25f };
	pVertex[31].vTexUV = pVertex[31].vPosition;

	m_pVB->Unlock();


	INDEX32* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);


	// 몸통
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


	// 왼쪽 날개
	pIndex[12]._0 = 9;
	pIndex[12]._1 = 13;
	pIndex[12]._2 = 14;

	pIndex[13]._0 = 9;
	pIndex[13]._1 = 14;
	pIndex[13]._2 = 10;

	pIndex[14]._0 = 12;
	pIndex[14]._1 = 8;
	pIndex[14]._2 = 11;

	pIndex[15]._0 = 12;
	pIndex[15]._1 = 11;
	pIndex[15]._2 = 15;

	pIndex[16]._0 = 12;
	pIndex[16]._1 = 13;
	pIndex[16]._2 = 9;

	pIndex[17]._0 = 12;
	pIndex[17]._1 = 9;
	pIndex[17]._2 = 8;

	pIndex[18]._0 = 11;
	pIndex[18]._1 = 10;
	pIndex[18]._2 = 14;

	pIndex[19]._0 = 11;
	pIndex[19]._1 = 14;
	pIndex[19]._2 = 15;

	pIndex[20]._0 = 15;
	pIndex[20]._1 = 14;
	pIndex[20]._2 = 13;

	pIndex[21]._0 = 15;
	pIndex[21]._1 = 13;
	pIndex[21]._2 = 12;

	pIndex[22]._0 = 8;
	pIndex[22]._1 = 9;
	pIndex[22]._2 = 10;

	pIndex[23]._0 = 8;
	pIndex[23]._1 = 10;
	pIndex[23]._2 = 11;


	// 오른쪽 날개
	pIndex[24]._0 = 17;
	pIndex[24]._1 = 21;
	pIndex[24]._2 = 22;

	pIndex[25]._0 = 17;
	pIndex[25]._1 = 22;
	pIndex[25]._2 = 18;

	pIndex[26]._0 = 20;
	pIndex[26]._1 = 16;
	pIndex[26]._2 = 19;

	pIndex[27]._0 = 20;
	pIndex[27]._1 = 19;
	pIndex[27]._2 = 23;

	pIndex[28]._0 = 20;
	pIndex[28]._1 = 21;
	pIndex[28]._2 = 17;

	pIndex[29]._0 = 20;
	pIndex[29]._1 = 17;
	pIndex[29]._2 = 16;

	pIndex[30]._0 = 19;
	pIndex[30]._1 = 18;
	pIndex[30]._2 = 22;

	pIndex[31]._0 = 19;
	pIndex[31]._1 = 22;
	pIndex[31]._2 = 23;

	pIndex[32]._0 = 23;
	pIndex[32]._1 = 22;
	pIndex[32]._2 = 21;

	pIndex[33]._0 = 23;
	pIndex[33]._1 = 21;
	pIndex[33]._2 = 20;

	pIndex[34]._0 = 16;
	pIndex[34]._1 = 17;
	pIndex[34]._2 = 18;

	pIndex[35]._0 = 16;
	pIndex[35]._1 = 18;
	pIndex[35]._2 = 19;


	// 뒤쪽 작은 사각형
	pIndex[36]._0 = 25;
	pIndex[36]._1 = 29;
	pIndex[36]._2 = 30;

	pIndex[37]._0 = 25;
	pIndex[37]._1 = 30;
	pIndex[37]._2 = 26;

	pIndex[38]._0 = 28;
	pIndex[38]._1 = 24;
	pIndex[38]._2 = 27;

	pIndex[39]._0 = 28;
	pIndex[39]._1 = 27;
	pIndex[39]._2 = 31;

	pIndex[40]._0 = 28;
	pIndex[40]._1 = 29;
	pIndex[40]._2 = 25;

	pIndex[41]._0 = 28;
	pIndex[41]._1 = 25;
	pIndex[41]._2 = 24;

	pIndex[42]._0 = 27;
	pIndex[42]._1 = 26;
	pIndex[42]._2 = 30;

	pIndex[43]._0 = 27;
	pIndex[43]._1 = 30;
	pIndex[43]._2 = 31;

	pIndex[44]._0 = 31;
	pIndex[44]._1 = 30;
	pIndex[44]._2 = 29;

	pIndex[45]._0 = 31;
	pIndex[45]._1 = 29;
	pIndex[45]._2 = 28;

	pIndex[46]._0 = 24;
	pIndex[46]._1 = 25;
	pIndex[46]._2 = 26;

	pIndex[47]._0 = 24;
	pIndex[47]._1 = 26;
	pIndex[47]._2 = 27;

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
