#include "CTerrain3.h"
#include "CHeightMapLoader.h"

CTerrain3::CTerrain3(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
, m_pTexture(nullptr)
{
}

CTerrain3::CTerrain3(const CTerrain3& rhs) : CVIBuffer(rhs)
, m_vecVertices(rhs.m_vecVertices)
, m_vecFaces(rhs.m_vecFaces)
, m_pTexture(rhs.m_pTexture)
, m_iVTXCNTX(rhs.m_iVTXCNTX)
, m_iVTXCNTZ(rhs.m_iVTXCNTZ)
{
	m_pTexture->AddRef();
}

CTerrain3::~CTerrain3()
{
}

HRESULT CTerrain3::Ready_Buffer()
{
	m_iVTXCNTX = VTXCNTX;
	m_iVTXCNTZ = VTXCNTZ;

	m_dwVtxSize = sizeof(VTXTC);
	m_dwVtxCnt = m_iVTXCNTX * m_iVTXCNTZ;
	m_dwTriCnt = (m_iVTXCNTX - 1) * (m_iVTXCNTZ - 1) * 2;
	m_dwFVF = FVF_TC;

	m_dwIdxCnt = m_dwTriCnt * 3;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	//vector<BYTE> heightMapBytes;
	//if (FAILED(CHeightMapLoader::LoadHeightMapByByte(
	//	L"../Bin/Resource/Texture/Terrain/Height1.bmp",
	//	heightMapBytes,
	//	129,
	//	129
	//)))
	//	return E_FAIL;

	VTXTC* vertices = nullptr;

	m_vecVertices.resize(m_dwVtxCnt);
	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < m_iVTXCNTZ; ++i) {
		for (int j = 0; j < m_iVTXCNTX; ++j) {
			int index = i * m_iVTXCNTX + j;
			vertices[index].vPosition = {
				(float)j * VTXITV,
				i < m_iVTXCNTZ * 0.5f ? i * 0.5f: (m_iVTXCNTZ - i) * 0.5f ,
				(float)i * VTXITV };
			vertices[index].vTexUV = { (float)j / (float)(m_iVTXCNTX - 1) ,
										(float)(m_iVTXCNTZ - 1 - i) / (float)(m_iVTXCNTZ - 1) * 5 };
			vertices[index].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			m_vecVertices[index].vPosition = vertices[index].vPosition;
			m_vecVertices[index].vTexUV = vertices[index].vTexUV;
		}
	}

	m_pVB->Unlock();

	m_vecFaces.resize(m_dwTriCnt);
	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < m_iVTXCNTZ - 1; ++i) {
		for (int j = 0; j < m_iVTXCNTX - 1; ++j) {
			// 왼쪽 위 삼각형
			indices[(i * (m_iVTXCNTX - 1) + j) * 2]._0 = (i + 1) * m_iVTXCNTX + j;
			indices[(i * (m_iVTXCNTX - 1) + j) * 2]._1 = (i + 1) * m_iVTXCNTX + (j + 1);
			indices[(i * (m_iVTXCNTX - 1) + j) * 2]._2 = i * m_iVTXCNTX + j;
			m_vecFaces[(i * (m_iVTXCNTX - 1) + j) * 2] = {
				{
					indices[(i * (m_iVTXCNTX - 1) + j) * 2]._0,
					indices[(i * (m_iVTXCNTX - 1) + j) * 2]._1,
					indices[(i * (m_iVTXCNTX - 1) + j) * 2]._2
				},{} };

			// 오른쪽 아래 삼각형
			indices[(i * (m_iVTXCNTX - 1) + j) * 2 + 1]._0 = i * m_iVTXCNTX + (j + 1);
			indices[(i * (m_iVTXCNTX - 1) + j) * 2 + 1]._1 = i * m_iVTXCNTX + j;
			indices[(i * (m_iVTXCNTX - 1) + j) * 2 + 1]._2 = (i + 1) * m_iVTXCNTX + (j + 1);

			m_vecFaces[(i * (m_iVTXCNTX - 1) + j) * 2 + 1] = {
			{
				indices[(i * (m_iVTXCNTX - 1) + j) * 2 + 1]._0,
				indices[(i * (m_iVTXCNTX - 1) + j) * 2 + 1]._1,
				indices[(i * (m_iVTXCNTX - 1) + j) * 2 + 1]._2
			},{} };
		}
	}

	m_pIB->Unlock();

	if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Cart/Terrain/F_road00.png", (LPDIRECT3DTEXTURE9*)&m_pTexture)))
		return E_FAIL;

	return S_OK;
}

void CTerrain3::Render_Buffer()
{
	m_pGraphicDev->SetTexture(0, m_pTexture);
	CVIBuffer::Render_Buffer();
	m_pGraphicDev->SetTexture(0, nullptr);
}

CTerrain3* CTerrain3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain3* pTerrain3 = new CTerrain3(pGraphicDev);

	if (FAILED(pTerrain3->Ready_Buffer())) {
		Safe_Release(pTerrain3);
		MSG_BOX("pTerrain3 Create Fail");
		return nullptr;
	}
	return pTerrain3;
}

CComponent* CTerrain3::Clone()
{
	CComponent* pComp = new CTerrain3(*this);

	return pComp;
}

void CTerrain3::Set_SkidMark(_vec3 vPos)
{
	int col = vPos.x / VTXITV;
	int row = vPos.z / VTXITV;

	float xInPlane = float(vPos.x - col * VTXITV) / VTXITV;
	float zInPlane = float(vPos.z - row * VTXITV) / VTXITV;

	// 왼쪽 위 삼각형
	VTXTC* pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	if (zInPlane - xInPlane > 0) {
		pVertex[(row + 1) * m_iVTXCNTX + col].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);		// 왼쪽 위
		pVertex[(row + 1) * m_iVTXCNTX + col + 1].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);	// 오른쪽 위
		//pVertex[row * m_iVTXCNTX + col].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);			// 왼쪽 아래
	}
	else { // 오른쪽 아래 삼각형
		pVertex[row * m_iVTXCNTX + col + 1].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);			// 오른쪽 아래
		pVertex[row * m_iVTXCNTX + col].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);				// 왼쪽 아래
		//pVertex[(row + 1) * m_iVTXCNTX + col + 1].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);	// 오른쪽 위
	}
	m_pVB->Unlock();
}

D3DXPLANE CTerrain3::GetPlane(_vec3 vPos)
{
	int col = vPos.x / VTXITV;
	int row = vPos.z / VTXITV;

	float xInPlane = float(vPos.x - col * VTXITV) / VTXITV;
	float zInPlane = float(vPos.z - row * VTXITV) / VTXITV;

	_vec3 p0, p1, p2;
	// 왼쪽 위 삼각형
	if (zInPlane - xInPlane > 0) {
		p0 = m_vecVertices[(row + 1) * VTXCNTX + col].vPosition;		// 왼쪽 위
		p1 = m_vecVertices[(row + 1) * VTXCNTX + col + 1].vPosition;	// 오른쪽 위
		p2 = m_vecVertices[row * VTXCNTX + col].vPosition;			// 왼쪽 아래
	}
	else { // 오른쪽 아래 삼각형
		p0 = m_vecVertices[row * VTXCNTX + col + 1].vPosition;		// 오른쪽 아래
		p1 = m_vecVertices[row * VTXCNTX + col].vPosition;			// 왼쪽 아래
		p2 = m_vecVertices[(row + 1) * VTXCNTX + col + 1].vPosition; // 오른쪽 위
	}
	D3DXPLANE plane;
	D3DXPlaneFromPoints(&plane, &p0, &p1, &p2);
	return plane;
}

_float CTerrain3::ComputeShade(_vec3* normal, _vec3* dirToLight)
{
	float cosine = D3DXVec3Dot(normal, dirToLight);

	if (cosine < 0.f)
		cosine = 0.f;

	return cosine;
}

void CTerrain3::Free()
{
	Safe_Release(m_pTexture);
	CVIBuffer::Free();
}
