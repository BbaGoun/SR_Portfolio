#include "CTerrain.h"
#include "CHeightMapLoader.h"

CTerrain::CTerrain() : CVIBuffer()
{
}

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CTerrain::CTerrain(const CTerrain& rhs) : CVIBuffer(rhs)
, m_vecPoints(rhs.m_vecPoints)
{
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = VTXCNTX * VTXCNTZ;
	m_dwTriCnt = (VTXCNTX - 1) * (VTXCNTZ - 1) * 2;
	m_dwFVF = FVF_TEX;

	m_dwIdxCnt = m_dwTriCnt * 3;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	vector<BYTE> heightMapBytes;
	if (FAILED(CHeightMapLoader::LoadHeightMapByByte(
		L"../Bin/Resource/Texture/Terrain/Height1.bmp",
		heightMapBytes,
		129,
		129
	)))
		return E_FAIL;

	VTXTEX* vertices = nullptr;

	m_vecPoints.resize(m_dwVtxCnt);
	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < VTXCNTZ; ++i) {
		for (int j = 0; j < VTXCNTX; ++j) {
			int index = i * VTXCNTX + j;
			vertices[index].vPosition = { 
				(float)j * VTXITV, 
				float(heightMapBytes[index*4+2]*0.1f),
				(float)i * VTXITV };
			m_vecPoints[index] = vertices[index].vPosition;
			vertices[index].vTexUV = { (float)j / (float)(VTXCNTX - 1), (float)(VTXCNTZ - 1 - i) / (float)(VTXCNTZ - 1) };
		}
	}

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < VTXCNTZ-1; ++i) {
		for (int j = 0; j < VTXCNTX-1; ++j) {
			// 왼쪽 위 삼각형
			indices[(i * (VTXCNTX - 1) + j) * 2]._0 = i * VTXCNTX + j;
			indices[(i * (VTXCNTX - 1) + j) * 2]._1 = (i + 1) * VTXCNTX + j;
			indices[(i * (VTXCNTX - 1) + j) * 2]._2 = (i + 1) * VTXCNTX + (j + 1);

			// 오른쪽 아래 삼각형
			indices[(i * (VTXCNTX - 1) + j) * 2 + 1]._0 = i * VTXCNTX + j;
			indices[(i * (VTXCNTX - 1) + j) * 2 + 1]._1 = (i + 1) * VTXCNTX + (j + 1);
			indices[(i * (VTXCNTX - 1) + j) * 2 + 1]._2 = i * VTXCNTX + (j + 1);
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain* pTerrain = new CTerrain(pGraphicDev);

	if (FAILED(pTerrain->Ready_Buffer())) {
		Safe_Release(pTerrain);
		MSG_BOX("pTerrain Create Fail");
		return nullptr;
	}

	return pTerrain;
}

CComponent* CTerrain::Clone()
{
	CComponent* pComp = new CTerrain(*this);

	return pComp;
}

void CTerrain::Free()
{
	CVIBuffer::Free();
}
