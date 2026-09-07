#include "CMeshRibbon.h"
#include "CCalculator.h"

CMeshRibbon::CMeshRibbon(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
, m_eRibbonType(RIBBON_ONEWAY)
{
}

CMeshRibbon::CMeshRibbon(const CMeshRibbon& rhs) : CVIBuffer(rhs)
, m_eRibbonType(rhs.m_eRibbonType)
{
}

CMeshRibbon::~CMeshRibbon()
{
}

HRESULT CMeshRibbon::Ready_Buffer()
{
	m_dwVtxCnt = 0;
	m_dwTriCnt = 0;
	m_dwIdxCnt = 0;

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;
	m_IdxFmt = D3DFMT_INDEX32;
	int idxSize = m_IdxFmt == D3DFMT_INDEX16 ? sizeof(WORD) : sizeof(DWORD);

	switch (m_eRibbonType) {
	case RIBBON_FLAT:
		m_dwTriCnt = m_iReserveSize - 2;
		break;
	case RIBBON_ONEWAY:
		m_dwTriCnt = m_iReserveSize - 2;
		break;
	}

	m_dwIdxCnt = m_dwTriCnt * 3;

	if (m_pVB)
		Safe_Release(m_pVB);
	if (m_pIB)
		Safe_Release(m_pIB);

	m_deqVertices.clear();
	m_deqVertices.resize(m_iReserveSize);

	m_deqFaces.clear();
	m_deqFaces.resize(m_dwTriCnt);

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(
		m_iReserveSize * m_dwVtxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		m_dwFVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0)
	))
		return E_FAIL;


	if (FAILED(m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxCnt * idxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		m_IdxFmt,
		D3DPOOL_DEFAULT,
		&m_pIB,
		0)
	))
		return E_FAIL;
	
	m_dwVtxCnt = 0;
	m_dwTriCnt = 0;
	m_dwIdxCnt = 0;

	return S_OK;
}

void CMeshRibbon::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

void CMeshRibbon::OnLostDevice()
{
	if (m_pVB)
		Safe_Release(m_pVB);
	if (m_pIB)
		Safe_Release(m_pIB);
}

void CMeshRibbon::OnResetDevice()
{
	m_dwVtxCnt = 0;
	m_dwTriCnt = 0;
	m_dwIdxCnt = 0;

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;
	m_IdxFmt = D3DFMT_INDEX32;
	int idxSize = m_IdxFmt == D3DFMT_INDEX16 ? sizeof(WORD) : sizeof(DWORD);

	switch (m_eRibbonType) {
	case RIBBON_FLAT:
		m_dwTriCnt = m_iReserveSize - 2;
		break;
	case RIBBON_ONEWAY:
		m_dwTriCnt = m_iReserveSize - 2;
		break;
	}

	m_dwIdxCnt = m_dwTriCnt * 3;

	if (m_pVB)
		Safe_Release(m_pVB);
	if (m_pIB)
		Safe_Release(m_pIB);

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(
		m_iReserveSize * m_dwVtxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		m_dwFVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0)
	));


	if (FAILED(m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxCnt * idxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		m_IdxFmt,
		D3DPOOL_DEFAULT,
		&m_pIB,
		0)
	));

	m_dwVtxCnt = m_deqVertices.size();
	m_dwTriCnt = m_deqFaces.size();
	m_dwIdxCnt = m_dwTriCnt * 3;


	VTXTEX* vertices = nullptr;

	// D3DLOCK_NOOVERWRITE : 앞부분은 건드리지 않는다.
	m_pVB->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		vertices[i] = m_deqVertices[i];
	}

	m_pVB->Unlock();


	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, D3DLOCK_DISCARD);

	for (int i = 0; i < m_dwTriCnt; ++i) {
		indices[i] = m_deqFaces[i].indices;
	}

	m_pIB->Unlock();
}

void CMeshRibbon::Append_Point(const ControlPoint& _cp)
{
	if (m_dwVtxCnt >= m_iReserveSize) {
		Delete_Tail();
	}

	ControlPoint cp = _cp;
	if (!m_deqControlPoint.empty()) {
		_vec3 before = m_deqControlPoint.back().position;
		_vec3 T = cp.position - before;
		D3DXVec3Normalize(&T, &T);
		cp.T = T;

		_vec3 R0, U0;
		_vec3 worldUp = { 0, 1, 0 };

		D3DXVec3Cross(&R0, &worldUp, &T);
		D3DXVec3Normalize(&R0, &R0);

		D3DXVec3Cross(&U0, &T, &R0);
		D3DXVec3Normalize(&U0, &U0);

		float rad = D3DXToRadian(cp.bank);
		_matrix matRotBank;
		D3DXMatrixRotationAxis(&matRotBank, &T, rad);

		D3DXVec3TransformNormal(&cp.R, &R0, &matRotBank);
		D3DXVec3TransformNormal(&cp.U, &U0, &matRotBank);

		if (m_deqControlPoint.size() == 1) {
			auto& front = m_deqControlPoint.front();
			front.T = T;
			front.R = cp.R;
			front.U = cp.U;
			Append_MeshSegment();
		}
	}

	m_deqControlPoint.push_back(cp);

	if (m_deqControlPoint.size() > 1)
		Append_MeshSegment();
}

void CMeshRibbon::Append_MeshSegment()
{
	if (m_bQuadCopy)
		Append_Quad();
	else
		Append_Line();
}

void CMeshRibbon::Append_Line()
{
	if (m_deqControlPoint.size() < 1)
		return;

	_vec3 pos;
	_vec3 T, R, U;
	VTXTEX v1, v2;
	float width, depth;
	int texUV = m_deqControlPoint.size() - 1;

	ControlPoint cp = m_deqControlPoint.back();

	pos = cp.position;
	width = cp.width;
	depth = cp.depth;
	T = cp.T, R = cp.R, U = cp.U;

	switch (m_eRibbonType) {
	case RIBBON_FLAT:
		v1.vPosition = pos - width * 0.5f * R;
		v1.vTexUV = { 0, float(1 - texUV) };
		v2.vPosition = pos + width * 0.5f * R;
		v2.vTexUV = { 1, float(1 - texUV) };
		break;
	case RIBBON_ONEWAY:
		v1.vPosition = pos;
		v1.vTexUV = { 0, float(1 - texUV) };
		v2.vPosition = pos + width * 0.5f * R;
		v2.vTexUV = { 1, float(1 - texUV) };
		break;
	}

	VTXTEX* vertices = nullptr;

	// D3DLOCK_NOOVERWRITE : 앞부분은 건드리지 않는다.
	m_pVB->Lock(0, 0, (void**)&vertices, D3DLOCK_NOOVERWRITE);

	vertices[m_dwVtxCnt] = v1;
	vertices[m_dwVtxCnt + 1] = v2;

	m_deqVertices[m_dwVtxCnt] = v1;
	m_deqVertices[m_dwVtxCnt + 1] = v2;

	m_pVB->Unlock();

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(m_deqVertices[i].vPosition);
	}

	SetBoundingBox();

	if (m_dwVtxCnt >= 2) {
		INDEX32* indices = nullptr;

		m_pIB->Lock(0, 0, (void**)&indices, D3DLOCK_NOOVERWRITE);

		switch (m_eRibbonType) {
		case RIBBON_FLAT:
		case RIBBON_ONEWAY:
			indices[m_dwTriCnt]._0 = m_dwVtxCnt - 2;
			indices[m_dwTriCnt]._1 = m_dwVtxCnt;
			indices[m_dwTriCnt]._2 = m_dwVtxCnt + 1;
			m_deqFaces[m_dwTriCnt].indices = {
				m_dwVtxCnt - 2,
				m_dwVtxCnt,
				m_dwVtxCnt + 1
			};

			indices[m_dwTriCnt + 1]._0 = m_dwVtxCnt - 2;
			indices[m_dwTriCnt + 1]._1 = m_dwVtxCnt + 1;
			indices[m_dwTriCnt + 1]._2 = m_dwVtxCnt - 1;
			m_deqFaces[m_dwTriCnt + 1].indices = {
				m_dwVtxCnt - 2,
				m_dwVtxCnt + 1,
				m_dwVtxCnt - 1
			};
			break;
		}

		m_pIB->Unlock();
		m_dwTriCnt += 2;
		m_dwIdxCnt += 6;
	}

	m_dwVtxCnt += 2;
}

void CMeshRibbon::Append_Quad()
{
	if (m_deqControlPoint.size() < 2)
		return;

	const DWORD base = m_dwVtxCnt;
	const float fOffset = CCalculator::RandInt() / 99.f;
	const float v0 = fOffset;
	const float v1 = fOffset + 1.f;  // 세그먼트 = 텍스처 1장
	
	ControlPoint cp = m_deqControlPoint.back();

	VTXTEX vtx0, vtx1, vtx2, vtx3;

	if (m_dwVtxCnt < 4)
	{
		ControlPoint firstCp = m_deqControlPoint.front();
		switch (m_eRibbonType) {
		case RIBBON_FLAT:
			vtx0.vPosition = firstCp.position - firstCp.width * 0.5f * firstCp.R;
			vtx0.vTexUV = { 0, v1 };
			vtx1.vPosition = firstCp.position + firstCp.width * 0.5f * firstCp.R;
			vtx1.vTexUV = { 1, v1 };
			break;
		case RIBBON_ONEWAY:
			vtx0.vPosition = firstCp.position;
			vtx0.vTexUV = { 0, v1 };
			vtx1.vPosition = firstCp.position + firstCp.width * 0.5f * firstCp.R;
			vtx1.vTexUV = { 1, v1 };
			break;
		}
	}

	switch (m_eRibbonType) {
	case RIBBON_FLAT:
		vtx2.vPosition = cp.position - cp.width * 0.5f * cp.R;
		vtx2.vTexUV = { 0, v1 };
		vtx3.vPosition = cp.position + cp.width * 0.5f * cp.R;
		vtx3.vTexUV = { 1, v1 };
		break;
	case RIBBON_ONEWAY:
		vtx2.vPosition = cp.position;
		vtx2.vTexUV = { 0, v1 };
		vtx3.vPosition = cp.position + cp.width * 0.5f * cp.R;
		vtx3.vTexUV = { 1, v1 };
		break;
	}

	VTXTEX* vertices = nullptr;

	// D3DLOCK_NOOVERWRITE : 앞부분은 건드리지 않는다.
	m_pVB->Lock(0, 0, (void**)&vertices, D3DLOCK_NOOVERWRITE);

	if (m_dwVtxCnt < 4)
	{
		vertices[base + 0] = vtx0;
		vertices[base + 1] = vtx1;
		vertices[base + 2] = vtx2;
		vertices[base + 3] = vtx3;

		m_deqVertices[base + 0] = vtx0;
		m_deqVertices[base + 1] = vtx1;
		m_deqVertices[base + 2] = vtx2;
		m_deqVertices[base + 3] = vtx3;
	}
	else {
		vtx0 = { m_deqVertices[base - 2].vPosition, { 0, v0 } };
		vtx1 = { m_deqVertices[base - 1].vPosition, { 1, v0 } };

		vertices[base + 0] = vtx0;
		vertices[base + 1] = vtx1;
		vertices[base + 2] = vtx2;
		vertices[base + 3] = vtx3;

		m_deqVertices[base + 0] = vtx0;
		m_deqVertices[base + 1] = vtx1;
		m_deqVertices[base + 2] = vtx2;
		m_deqVertices[base + 3] = vtx3;
	}

	m_pVB->Unlock();

	m_minVtx = { FLT_MAX, FLT_MAX, FLT_MAX };
	m_maxVtx = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(m_deqVertices[i].vPosition);
	}

	SetBoundingBox();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, D3DLOCK_NOOVERWRITE);

	switch (m_eRibbonType) {
	case RIBBON_FLAT:
	case RIBBON_ONEWAY:
		indices[m_dwTriCnt]._0 = base + 0;
		indices[m_dwTriCnt]._1 = base + 2;
		indices[m_dwTriCnt]._2 = base + 3;
		m_deqFaces[m_dwTriCnt].indices = {
			base + 0,
			base + 2,
			base + 3,
		};

		indices[m_dwTriCnt + 1]._0 = base + 0;
		indices[m_dwTriCnt + 1]._1 = base + 3;
		indices[m_dwTriCnt + 1]._2 = base + 1;
		m_deqFaces[m_dwTriCnt + 1].indices = {
			base + 0,
			base + 3,
			base + 1
		};
		break;
	}

	m_pIB->Unlock();
	m_dwVtxCnt += 4;
	m_dwTriCnt += 2;
	m_dwIdxCnt += 6;
}

void CMeshRibbon::Delete_Line()
{
	m_deqControlPoint.pop_front();

	m_dwVtxCnt -= 2;
	m_dwTriCnt -= 2;
	m_dwIdxCnt -= 6;

	VTXTEX* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		vertices[i] = m_deqVertices[i + 2];
	}
	m_deqVertices.pop_front();
	m_deqVertices.pop_front();
	m_deqVertices.push_back({});
	m_deqVertices.push_back({});

	m_minVtx = { FLT_MAX, FLT_MAX, FLT_MAX };
	m_maxVtx = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(m_deqVertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < m_dwTriCnt; ++i) {
		m_deqFaces[i + 2].indices._0 -= 2;
		m_deqFaces[i + 2].indices._1 -= 2;
		m_deqFaces[i + 2].indices._2 -= 2;
		indices[i] = m_deqFaces[i + 2].indices;
	}
	m_deqFaces.pop_front();
	m_deqFaces.pop_front();
	m_deqFaces.push_back({});
	m_deqFaces.push_back({});

	m_pIB->Unlock();
}

void CMeshRibbon::Delete_Quad()
{
	m_deqControlPoint.pop_front();

	m_dwVtxCnt -= 4;
	m_dwTriCnt -= 2;
	m_dwIdxCnt -= 6;

	VTXTEX* vertices = nullptr;

	// D3DLOCK_DISCARD : 기존 데이터를 삭제
	m_pVB->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		vertices[i] = m_deqVertices[i + 4];
	}
	m_deqVertices.pop_front();
	m_deqVertices.pop_front();
	m_deqVertices.pop_front();
	m_deqVertices.pop_front();
	m_deqVertices.push_back({});
	m_deqVertices.push_back({});
	m_deqVertices.push_back({});
	m_deqVertices.push_back({});

	m_minVtx = { FLT_MAX, FLT_MAX, FLT_MAX };
	m_maxVtx = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(m_deqVertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < m_dwTriCnt; ++i) {
		// 앞의 정점 4개를 삭제했기 때문에 빼준다.
		m_deqFaces[i + 2].indices._0 -= 4;
		m_deqFaces[i + 2].indices._1 -= 4;
		m_deqFaces[i + 2].indices._2 -= 4;
		indices[i] = m_deqFaces[i + 2].indices;
	}
	m_deqFaces.pop_front();
	m_deqFaces.pop_front();
	m_deqFaces.push_back({});
	m_deqFaces.push_back({});

	m_pIB->Unlock();
}

void CMeshRibbon::Update_Wave()
{
	VTXTEX* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	DWORD quaterVtxCnt = m_dwVtxCnt >> 2;
	for (int i = 0; i < quaterVtxCnt; ++i) {
		float fOffset = CCalculator::RandInt() / 99.f;
		switch (m_eRibbonType) {
		case RIBBON_FLAT:
		case RIBBON_ONEWAY:
			vertices[i * 4].vTexUV = { 0, fOffset };
			vertices[i * 4 + 1].vTexUV = { 1, fOffset };
			vertices[i * 4 + 2].vTexUV = { 0, fOffset + 1 };
			vertices[i * 4 + 3].vTexUV = { 1, fOffset + 1 };
			break;
		}
	}

	m_pVB->Unlock();
}

void CMeshRibbon::Delete_Tail()
{
	if (m_bQuadCopy)
		Delete_Quad();
	else
		Delete_Line();
}

CMeshRibbon* CMeshRibbon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMeshRibbon* pRibbon = new CMeshRibbon(pGraphicDev);

	return pRibbon;
}

CComponent* CMeshRibbon::Clone()
{
	CComponent* pComp = new CMeshRibbon(*this);

	// 클론할 때 버퍼를 생성
	static_cast<CMeshRibbon*>(pComp)->Ready_Buffer();

	return pComp;
}

void CMeshRibbon::Free()
{
	CVIBuffer::Free();
}
