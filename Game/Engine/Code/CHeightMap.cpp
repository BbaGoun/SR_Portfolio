#include "CHeightMap.h"
#include "CProtoMgr.h"
#include "CCalculator.h"

CHeightMap::CHeightMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
	, m_pTexNormal(nullptr)
	, m_pTexEdit(nullptr)
{
}

CHeightMap::CHeightMap(const CHeightMap& rhs)
	:CVIBuffer(rhs)
	, m_pTexNormal(rhs.m_pTexNormal)
	, m_pTexEdit(rhs.m_pTexEdit)
{
	if(m_pTexNormal)
		m_pTexNormal->AddRef();
	if (m_pTexEdit)
		m_pTexEdit->AddRef();
}

CHeightMap::~CHeightMap()
{
}

HRESULT CHeightMap::Ready_CHeightMap()
{
	m_pTexNormal = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SplinePointNormal"));
	m_pTexEdit = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SplinePointEdit"));
	
	return S_OK;
}

HRESULT CHeightMap::Ready_Buffer()
{
	m_vecVertices.clear();
	m_vecFaces.clear();

	m_dwVtxCnt = m_iCntX * m_iCntZ;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwTriCnt = (m_iCntX - 1) * (m_iCntZ - 1) * 2;
	m_dwIdxCnt = m_dwTriCnt * 3;
	m_IdxFmt = D3DFMT_INDEX32;

	if (m_pVB)
		Safe_Release(m_pVB);
	if (m_pIB)
		Safe_Release(m_pIB);

	// 높이 값 변경 최적화를 위해 동적 버퍼
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(
		m_dwVtxCnt * m_dwVtxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		m_dwFVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0)
	))
		return E_FAIL;

	int idxSize = m_IdxFmt == D3DFMT_INDEX16 ? sizeof(WORD) : sizeof(DWORD);

	// 높이 값이 달라져도 인덱스가 달라지진 않으므로 정적
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxCnt * idxSize,
		D3DUSAGE_WRITEONLY,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		0)
	))
		return E_FAIL;

	VTXTEX* vertices = nullptr;
	m_vecVertices.resize(m_dwVtxCnt);

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < m_iCntZ; ++i) {
		for (int j = 0; j < m_iCntX; ++j) {
			int index = i * m_iCntX + j;
			vertices[index].vPosition = {
				(float)j * m_fItv, 0, (float)i * m_fItv };
			vertices[index].vTexUV = { (float)j / (float)(m_iCntX - 1),
										(float)(m_iCntZ - 1 - i) / (float)(m_iCntZ - 1) };

			m_vecVertices[index].vPosition = vertices[index].vPosition;
			m_vecVertices[index].vTexUV = vertices[index].vTexUV;
		}
	}

	m_minVtx = { FLT_MAX, FLT_MAX, FLT_MAX };
	m_maxVtx = { FLT_MIN, FLT_MIN, FLT_MIN };

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(vertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();

	INDEX32* indices = nullptr;
	m_vecFaces.resize(m_dwTriCnt);

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < m_iCntZ - 1; ++i) {
		for (int j = 0; j < m_iCntX - 1; ++j) {
			// 왼쪽 위 삼각형
			indices[(i * (m_iCntX - 1) + j) * 2]._0 = (i + 1) * m_iCntX + j;
			indices[(i * (m_iCntX - 1) + j) * 2]._1 = (i + 1) * m_iCntX + (j + 1);
			indices[(i * (m_iCntX - 1) + j) * 2]._2 = i * m_iCntX + j;
			m_vecFaces[(i * (m_iCntX - 1) + j) * 2] = {
				{
					indices[(i * (m_iCntX - 1) + j) * 2]._0,
					indices[(i * (m_iCntX - 1) + j) * 2]._1,
					indices[(i * (m_iCntX - 1) + j) * 2]._2
				},{} };

			// 오른쪽 아래 삼각형
			indices[(i * (m_iCntX - 1) + j) * 2 + 1]._0 = i * m_iCntX + (j + 1);
			indices[(i * (m_iCntX - 1) + j) * 2 + 1]._1 = i * m_iCntX + j;
			indices[(i * (m_iCntX - 1) + j) * 2 + 1]._2 = (i + 1) * m_iCntX + (j + 1);
			m_vecFaces[(i * (m_iCntX - 1) + j) * 2 + 1] = {
			{
				indices[(i * (m_iCntX - 1) + j) * 2 + 1]._0,
				indices[(i * (m_iCntX - 1) + j) * 2 + 1]._1,
				indices[(i * (m_iCntX - 1) + j) * 2 + 1]._2
			},{} };
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

void CHeightMap::Adjust_Itv()
{
	VTXTEX* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < m_iCntZ; ++i) {
		for (int j = 0; j < m_iCntX; ++j) {
			int index = i * m_iCntX + j;
			vertices[index].vPosition = {
				(float)j * m_fItv,
				m_vecVertices[index].vPosition.y,
				(float)i * m_fItv };

			m_vecVertices[index].vPosition = vertices[index].vPosition;
		}
	}

	m_minVtx = { FLT_MAX, FLT_MAX, FLT_MAX };
	m_maxVtx = { FLT_MIN, FLT_MIN, FLT_MIN };

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(vertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();
}

void CHeightMap::HeightMap_Edit(_vec3 _pickPos, bool bShift)
{
	// 해당 위치로부터 범위 내에 있는 점들을 파악
	// 해당 점들의 높이를 높히거나 낮춘다
	// 바뀐 높이를 buffer에 적용한다
	float r2 = m_fEditRadius * m_fEditRadius;
	for (auto& p : m_vecVertices) {
		float dx = p.vPosition.x - _pickPos.x;
		float dz = p.vPosition.z - _pickPos.z;
		float d2 = dx * dx + dz * dz;
		
		if (d2 > r2)
			continue;

		float w = 1.f - sqrtf(d2) / m_fEditRadius;   // 선형 폴오프. smoothstep / 가우시안도 가능
		p.vPosition.y += (bShift ? -1.f : +1.f) * m_fEditStrength * w * 0.02f;
	}

	Adjust_Edit();
}

HRESULT CHeightMap::Ready_BufferByVec()
{
	m_dwVtxCnt = m_iCntX * m_iCntZ;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwTriCnt = (m_iCntX - 1) * (m_iCntZ - 1) * 2;
	m_dwIdxCnt = m_dwTriCnt * 3;
	m_IdxFmt = D3DFMT_INDEX32;

	if (m_pVB)
		Safe_Release(m_pVB);
	if (m_pIB)
		Safe_Release(m_pIB);

	// 높이 값 변경 최적화를 위해 동적 버퍼
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(
		m_dwVtxCnt * m_dwVtxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		m_dwFVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0)
	))
		return E_FAIL;

	int idxSize = m_IdxFmt == D3DFMT_INDEX16 ? sizeof(WORD) : sizeof(DWORD);

	// 높이 값이 달라져도 인덱스가 달라지진 않으므로 정적
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxCnt * idxSize,
		D3DUSAGE_WRITEONLY,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		0)
	))
		return E_FAIL;

	VTXTEX* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < m_iCntZ; ++i) {
		for (int j = 0; j < m_iCntX; ++j) {
			int index = i * m_iCntX + j;
			vertices[index].vPosition = m_vecVertices[index].vPosition;
			vertices[index].vTexUV = m_vecVertices[index].vTexUV;
		}
	}

	m_minVtx = { FLT_MAX, FLT_MAX, FLT_MAX };
	m_maxVtx = { FLT_MIN, FLT_MIN, FLT_MIN };

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(vertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < m_iCntZ - 1; ++i) {
		for (int j = 0; j < m_iCntX - 1; ++j) {
			// 왼쪽 위 삼각형
			indices[(i * (m_iCntX - 1) + j) * 2]._0 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2].indices._0;
			indices[(i * (m_iCntX - 1) + j) * 2]._1 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2].indices._1;
			indices[(i * (m_iCntX - 1) + j) * 2]._2 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2].indices._2;

			// 오른쪽 아래 삼각형
			indices[(i * (m_iCntX - 1) + j) * 2 + 1]._0 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2 + 1].indices._0;
			indices[(i * (m_iCntX - 1) + j) * 2 + 1]._1 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2 + 1].indices._1;
			indices[(i * (m_iCntX - 1) + j) * 2 + 1]._2 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2 + 1].indices._2;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

void CHeightMap::Render_Buffer()
{
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CVIBuffer::Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CHeightMap::Render_Points()
{
	PreRender_Points();

	if (m_bEdit)
		m_pTexEdit->Set_Texture(0);
	else
		m_pTexNormal->Set_Texture(0);

	m_minVtx = { FLT_MAX, FLT_MAX, FLT_MAX };
	m_maxVtx = { FLT_MIN, FLT_MIN, FLT_MIN };

	LPDIRECT3DVERTEXBUFFER9 _pVB;
	m_pGraphicDev->CreateVertexBuffer(
		m_vecVertices.size() * sizeof(VTXTEX),
		D3DUSAGE_POINTS,
		FVF_TEX,
		D3DPOOL_MANAGED,
		&_pVB,
		0);

	VTXTEX* vertices = nullptr;
	_pVB->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

	for (int i = 0; i < m_vecVertices.size(); ++i) {
		vertices[i].vPosition = m_vecVertices[i].vPosition;
	}

	_pVB->Unlock();

	m_pGraphicDev->SetStreamSource(0, _pVB, 0, sizeof(VTXTEX));
	m_pGraphicDev->SetFVF(FVF_TEX);
	_matrix* matWorld = m_pOwner->Get_Transform()->Get_World();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);
	m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, 0, m_vecVertices.size());

	Safe_Release(_pVB);
	m_pGraphicDev->SetTexture(0, nullptr);

	PostRender_Points();
}

void CHeightMap::Render_Brush(_vec3 _pickPos)
{
	int iSegmentRound = 60;
	int iSegmentLine = 10;
	_vec3 firstEnd, beforeEnd, currentEnd;
	_vec3 beforetmp, tmp, worldPickPos;
	_matrix* matWorld = m_pOwner->Get_Transform()->Get_World();
	D3DXVec3TransformCoord(&worldPickPos, &_pickPos, matWorld);

	for (int i = 0; i < iSegmentRound; ++i) {
		float radian = D3DXToRadian(360.f * i / iSegmentRound);
		beforetmp = worldPickPos;
		for (int j = 1; j <= iSegmentLine; ++j) {
			tmp = _pickPos +
				_vec3{ m_fEditRadius * j / iSegmentLine * cosf(radian),
				0,
				m_fEditRadius * j / iSegmentLine * sinf(radian) };
			tmp = GetHeightFromXZ(tmp) + _vec3{ 0, 0.1f, 0 };
			D3DXVec3TransformCoord(&tmp, &tmp, matWorld);
			CCalculator::DrawRayLine(m_pGraphicDev, beforetmp, tmp, D3DXCOLOR(0, 0, 1, 1));
			beforetmp = tmp;
		}
		if (i == iSegmentRound - 1) {
			currentEnd = tmp;
			CCalculator::DrawRayLine(m_pGraphicDev, firstEnd, currentEnd, D3DXCOLOR(0, 0, 1, 1));
		}
		else if(i > 0){
			currentEnd = tmp;
			CCalculator::DrawRayLine(m_pGraphicDev, beforeEnd, currentEnd, D3DXCOLOR(0, 0, 1, 1));
			beforeEnd = currentEnd;
		}
		else {
			firstEnd = tmp;
			beforeEnd = tmp;
		}
	}
}

_vec3 CHeightMap::GetHeightFromXZ(_vec3 vPos)
{
	vPos.x = clampT(vPos.x, 0.f, m_fItv * (m_iCntX - 1));
	vPos.z = clampT(vPos.z, 0.f, m_fItv * (m_iCntZ - 1));

	int col = (int)floor(vPos.x / m_fItv);
	int row = (int)floor(vPos.z / m_fItv);

	col = clampT(col, 0, m_iCntX - 2);
	row = clampT(row, 0, m_iCntZ - 2);

	float xInPlane = float(vPos.x - col * m_fItv) / m_fItv;
	float zInPlane = float(vPos.z - row * m_fItv) / m_fItv;

	_vec3 p0, p1, p2;
	// 왼쪽 위 삼각형
	if (zInPlane - xInPlane > 0) {
		p0 = m_vecVertices[(row + 1) * m_iCntX + col].vPosition;		// 왼쪽 위
		p1 = m_vecVertices[(row + 1) * m_iCntX + col + 1].vPosition;	// 오른쪽 위
		p2 = m_vecVertices[row * m_iCntX + col].vPosition;				// 왼쪽 아래
	}
	else { // 오른쪽 아래 삼각형
		p0 = m_vecVertices[row * m_iCntX + col + 1].vPosition;			// 오른쪽 아래
		p1 = m_vecVertices[row * m_iCntX + col].vPosition;				// 왼쪽 아래
		p2 = m_vecVertices[(row + 1) * m_iCntX + col + 1].vPosition;	// 오른쪽 위
	}
	D3DXPLANE plane;
	D3DXPlaneFromPoints(&plane, &p0, &p1, &p2);

	// normal·p + d = 0
	// ax + by + cz + d = 0
	// by = -(ax + cz + d)
	// y = -(ax + cz + d) / b
	float y = -(plane.a * vPos.x + plane.c * vPos.z + plane.d) / plane.b;

	return _vec3{ vPos.x, y, vPos.z };
}

void CHeightMap::Adjust_Edit()
{
	VTXTEX* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < m_iCntZ; ++i) {
		for (int j = 0; j < m_iCntX; ++j) {
			int index = i * m_iCntX + j;
			vertices[index].vPosition = m_vecVertices[index].vPosition;
		}
	}

	m_minVtx = { FLT_MAX, FLT_MAX, FLT_MAX };
	m_maxVtx = { FLT_MIN, FLT_MIN, FLT_MIN };

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(vertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();
}

void CHeightMap::PreRender_Points()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(16.f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(8.f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(32.f));

	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
}

void CHeightMap::PostRender_Points()
{
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}


CHeightMap* CHeightMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHeightMap* pCom = new CHeightMap(pGraphicDev);

	if (FAILED(pCom->Ready_CHeightMap()))
	{
		Safe_Release(pCom);
		MSG_BOX("CHeightMap Create Failed");
		return nullptr;
	}

	return pCom;
}

CComponent* CHeightMap::Clone()
{
	CHeightMap* pCom = new CHeightMap(*this);

	return pCom;
}


void CHeightMap::OnLostDevice()
{
	if (m_pVB)
		Safe_Release(m_pVB);
	if (m_pIB)
		Safe_Release(m_pIB);
}

void CHeightMap::OnResetDevice()
{
	if (m_pVB)
		Safe_Release(m_pVB);
	if (m_pIB)
		Safe_Release(m_pIB);

	m_dwVtxCnt = m_iCntX * m_iCntZ;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwTriCnt = (m_iCntX - 1) * (m_iCntZ - 1)*2;
	m_dwIdxCnt = m_dwTriCnt * 3;
	m_IdxFmt = D3DFMT_INDEX32;

	// 높이 값 변경 최적화를 위해 동적 버퍼
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(
		m_dwVtxCnt * m_dwVtxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		m_dwFVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0)
	))
		MSG_BOX("Reset HeightMap VB Fail");


	int idxSize = m_IdxFmt == D3DFMT_INDEX16 ? sizeof(WORD) : sizeof(DWORD);

	// 높이 값이 달라져도 인덱스가 달라지진 않으므로 정적
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxCnt * idxSize,
		D3DUSAGE_WRITEONLY,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		0)
	))
		MSG_BOX("Reset HeightMap IB Fail");

	VTXTEX* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < m_iCntZ; ++i) {
		for (int j = 0; j < m_iCntX; ++j) {
			int index = i * m_iCntX + j;
			vertices[index].vPosition = m_vecVertices[index].vPosition;
			vertices[index].vTexUV = m_vecVertices[index].vTexUV;
		}
	}

	m_pVB->Unlock();

	INDEX32* indices = nullptr;
	m_vecFaces.resize(m_dwTriCnt);

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < m_iCntZ - 1; ++i) {
		for (int j = 0; j < m_iCntX - 1; ++j) {
			// 왼쪽 위 삼각형
			indices[(i * (m_iCntX - 1) + j) * 2]._0 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2].indices._0;
			indices[(i * (m_iCntX - 1) + j) * 2]._1 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2].indices._1;
			indices[(i * (m_iCntX - 1) + j) * 2]._2 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2].indices._2;

			// 오른쪽 아래 삼각형
			indices[(i * (m_iCntX - 1) + j) * 2 + 1]._0 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2 + 1].indices._0;
			indices[(i * (m_iCntX - 1) + j) * 2 + 1]._1 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2 + 1].indices._1;
			indices[(i * (m_iCntX - 1) + j) * 2 + 1]._2 = m_vecFaces[(i * (m_iCntX - 1) + j) * 2 + 1].indices._2;
		}
	}

	m_pIB->Unlock();
}

void CHeightMap::Free()
{
	Safe_Release(m_pTexNormal);
	Safe_Release(m_pTexEdit);
	CVIBuffer::Free();
}