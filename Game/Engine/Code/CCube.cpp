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
	m_dwVtxSize = sizeof(VTXTEXNOR);
	m_dwVtxCnt = 8;
	m_dwTriCnt = 12;
	m_dwFVF = FVF_TEXNOR;

	m_dwIdxCnt = 36;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEXNOR* vertices = nullptr;
	VTXTEXNOR tmp_vertices[8];

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0].vPosition = { -0.5f, 0.5f, 0.5f };
	//vertices[0].dwColor = 0xff0000ff;
	vertices[0].vTexUV = { 0.f, 0.f };
	tmp_vertices[0].vPosition = { -0.5f, 0.5f, 0.5f };

	vertices[1].vPosition = { 0.5f, 0.5f, 0.5f };
	//vertices[1].dwColor = 0xff0000ff;
	vertices[1].vTexUV = { 1.f, 0.f };
	tmp_vertices[1].vPosition = { 0.5f, 0.5f, 0.5f };

	vertices[2].vPosition = { 0.5f, -0.5f, 0.5f };
	//vertices[2].dwColor = 0xff0000ff;
	vertices[2].vTexUV = { 1.f, 1.f };
	tmp_vertices[2].vPosition = { 0.5f, -0.5f, 0.5f };

	vertices[3].vPosition = { -0.5f, -0.5f, 0.5f };
	//vertices[3].dwColor = 0xff0000ff;
	vertices[3].vTexUV = { 0.f, 1.f };
	tmp_vertices[3].vPosition = { -0.5f, -0.5f, 0.5f };

	vertices[4].vPosition = { -0.5f, 0.5f, -0.5f };
	//vertices[4].dwColor = 0xffff0000;
	vertices[4].vTexUV = { 0.f, 0.f };
	tmp_vertices[4].vPosition = { -0.5f, 0.5f, -0.5f };

	vertices[5].vPosition = { 0.5f, 0.5f, -0.5f };
	//vertices[5].dwColor = 0xffff0000;
	vertices[5].vTexUV = { 1.f, 0.f };
	tmp_vertices[5].vPosition = { 0.5f, 0.5f, -0.5f };

	vertices[6].vPosition = { 0.5f, -0.5f, -0.5f };
	//vertices[6].dwColor = 0xffff0000;
	vertices[6].vTexUV = { 1.f, 1.f };
	tmp_vertices[6].vPosition = { 0.5f, -0.5f, -0.5f };

	vertices[7].vPosition = { -0.5f, -0.5f, -0.5f };
	//vertices[7].dwColor = 0xffff0000;
	vertices[7].vTexUV = { 0.f, 1.f };
	tmp_vertices[7].vPosition = { -0.5f, -0.5f, -0.5f };

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(vertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();

	INDEX32* indices = nullptr;
	FACE32 faces[12];

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	// 뒷 면
	indices[0]._0 = 2;
	indices[0]._1 = 1;
	indices[0]._2 = 0;
	faces[0].indices = { 2, 1, 0 };

	indices[1]._0 = 3;
	indices[1]._1 = 2;
	indices[1]._2 = 0;
	faces[1].indices = { 3, 2, 0 };

	// 윗 면
	indices[2]._0 = 1;
	indices[2]._1 = 5;
	indices[2]._2 = 4;
	faces[2].indices = { 1, 5, 4 };

	indices[3]._0 = 0;
	indices[3]._1 = 1;
	indices[3]._2 = 4;
	faces[3].indices = { 0, 1, 4 };

	// 앞 면
	indices[4]._0 = 7;
	indices[4]._1 = 4;
	indices[4]._2 = 5;
	faces[4].indices = { 7, 4, 5 };

	indices[5]._0 = 6;
	indices[5]._1 = 7;
	indices[5]._2 = 5;
	faces[5].indices = { 6, 7, 5 };

	// 아랫 면
	indices[6]._0 = 6;
	indices[6]._1 = 2;
	indices[6]._2 = 3;
	faces[6].indices = { 6, 2, 3 };

	indices[7]._0 = 7;
	indices[7]._1 = 6;
	indices[7]._2 = 3;
	faces[7].indices = { 7, 6, 3 };

	// 왼쪽 면
	indices[8]._0 = 3;
	indices[8]._1 = 0;
	indices[8]._2 = 4;
	faces[8].indices = { 3, 0, 4 };

	indices[9]._0 = 7;
	indices[9]._1 = 3;
	indices[9]._2 = 4;
	faces[9].indices = { 7, 3, 4 };

	// 오른쪽 면
	indices[10]._0 = 6;
	indices[10]._1 = 5;
	indices[10]._2 = 1;
	faces[10].indices = { 6, 5, 1 };

	indices[11]._0 = 2;
	indices[11]._1 = 6;
	indices[11]._2 = 1;
	faces[11].indices = { 2, 6, 1 };

	m_pIB->Unlock();

	for (int i = 0; i < 12; ++i) {
		D3DXPLANE plane;
		_vec3 p0 = tmp_vertices[faces[i].indices._0].vPosition;
		_vec3 p1 = tmp_vertices[faces[i].indices._1].vPosition;
		_vec3 p2 = tmp_vertices[faces[i].indices._2].vPosition;

		D3DXPlaneFromPoints(&plane, &p0, &p1, &p2);

		faces[i].vNormal = { plane.a, plane.b, plane.c };
		tmp_vertices[faces[i].indices._0].vNormal += faces[i].vNormal;
		tmp_vertices[faces[i].indices._1].vNormal += faces[i].vNormal;
		tmp_vertices[faces[i].indices._2].vNormal += faces[i].vNormal;
	}


	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < 8; ++i) {
		D3DXVec3Normalize(&vertices[i].vNormal, &tmp_vertices[i].vNormal);
	}

	m_pVB->Unlock();

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
