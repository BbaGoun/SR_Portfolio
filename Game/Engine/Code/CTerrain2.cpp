#include "CTerrain2.h"
#include "CHeightMapLoader.h"

CTerrain2::CTerrain2() : CVIBuffer()
, m_pTexture(nullptr)
{
}

CTerrain2::CTerrain2(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
, m_pTexture(nullptr)
{
}

CTerrain2::CTerrain2(const CTerrain2& rhs) : CVIBuffer(rhs)
, m_vecVertices(rhs.m_vecVertices), m_pTexture(rhs.m_pTexture)
{
	m_pTexture->AddRef();
}

CTerrain2::~CTerrain2()
{
}

HRESULT CTerrain2::Ready_Buffer()
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

	m_vecVertices.resize(m_dwVtxCnt);
	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < VTXCNTZ; ++i) {
		for (int j = 0; j < VTXCNTX; ++j) {
			int index = i * VTXCNTX + j;
			vertices[index].vPosition = {
				(float)j * VTXITV,
				float(heightMapBytes[index * 4 + 2] * 0.2f),
				(float)i * VTXITV };
			vertices[index].vTexUV = { (float)j / (float)(VTXCNTX - 1), (float)(VTXCNTZ - 1 - i) / (float)(VTXCNTZ - 1) };
			m_vecVertices[index].vPosition = vertices[index].vPosition;
			m_vecVertices[index].vTexUV = vertices[index].vTexUV;
		}
	}

	m_pVB->Unlock();

	m_vecFaces.resize(m_dwTriCnt);
	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < VTXCNTZ - 1; ++i) {
		for (int j = 0; j < VTXCNTX - 1; ++j) {
			// 왼쪽 위 삼각형
			indices[(i * (VTXCNTX - 1) + j) * 2]._0 = (i + 1) * VTXCNTX + j;
			indices[(i * (VTXCNTX - 1) + j) * 2]._1 = (i + 1) * VTXCNTX + (j + 1);
			indices[(i * (VTXCNTX - 1) + j) * 2]._2 = i * VTXCNTX + j;
			m_vecFaces[(i * (VTXCNTX - 1) + j) * 2] = {
				{
					indices[(i * (VTXCNTX - 1) + j) * 2]._0,
					indices[(i * (VTXCNTX - 1) + j) * 2]._1,
					indices[(i * (VTXCNTX - 1) + j) * 2]._2
				},{} };

			// 오른쪽 아래 삼각형
			indices[(i * (VTXCNTX - 1) + j) * 2 + 1]._0 = i * VTXCNTX + (j + 1);
			indices[(i * (VTXCNTX - 1) + j) * 2 + 1]._1 = i * VTXCNTX + j;
			indices[(i * (VTXCNTX - 1) + j) * 2 + 1]._2 = (i + 1) * VTXCNTX + (j + 1);
			m_vecFaces[(i * (VTXCNTX - 1) + j) * 2 + 1] = {
			{
				indices[(i * (VTXCNTX - 1) + j) * 2 + 1]._0,
				indices[(i * (VTXCNTX - 1) + j) * 2 + 1]._1,
				indices[(i * (VTXCNTX - 1) + j) * 2 + 1]._2
			},{} };
		}
	}

	m_pIB->Unlock();

	if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Terrain0.png", (LPDIRECT3DTEXTURE9*)&m_pTexture)))
		return E_FAIL;

	D3DSURFACE_DESC surfaceDesc;
	m_pTexture->GetLevelDesc(0, &surfaceDesc);
	int texWidth = surfaceDesc.Width;
	int texHeight = surfaceDesc.Height;

	// 고로 쉐이딩

	// 1. 각 면의 법선 구하기
	for (int i = 0; i < m_vecFaces.size(); ++i) {
		VTXMESH v0, v1, v2;
		_vec3 p0, p1, p2;
		_vec2 uv0, uv1, uv2;

		v0 = m_vecVertices[m_vecFaces[i].indices._0];
		v1 = m_vecVertices[m_vecFaces[i].indices._1];
		v2 = m_vecVertices[m_vecFaces[i].indices._2];

		p0 = v0.vPosition;
		p1 = v1.vPosition;
		p2 = v2.vPosition;

		// 면 구하기
		D3DXPLANE upperPlane;
		D3DXPlaneFromPoints(&upperPlane, &p1, &p2, &p0);
		m_vecFaces[i].vNoraml = { upperPlane.a, upperPlane.b, upperPlane.c };
	}

	// 2. 정점 법선 구하기
	for (int i = 0; i < VTXCNTZ; ++i) {
		for (int j = 0; j < VTXCNTX; ++j) {
			// 주변 최대 6면의 법선을 고려
			// 시계방향으로 0~5까지의 면
			_vec3 vVtxNormal = { 0, 0, 0 };
			int cnt = 0;

			if (i < VTXCNTZ - 1 && j < VTXCNTX - 1) {
				int index0 = (i * (VTXCNTX - 1) + j) * 2;
				int index1 = (i * (VTXCNTX - 1) + j) * 2 + 1;
				vVtxNormal += m_vecFaces[index0].vNoraml;
				vVtxNormal += m_vecFaces[index1].vNoraml;
				cnt += 2;
			}
			if (i > 0 && j < VTXCNTZ - 1) {
				int index2 = ((i - 1) * (VTXCNTX - 1) + j) * 2;
				vVtxNormal += m_vecFaces[index2].vNoraml;
				++cnt;
			}
			if (i > 0 && j > 0) {
				int index3 = ((i - 1) * (VTXCNTX - 1) + (j - 1)) * 2 + 1;
				int index4 = ((i - 1) * (VTXCNTX - 1) + (j - 1)) * 2;
				vVtxNormal += m_vecFaces[index3].vNoraml;
				vVtxNormal += m_vecFaces[index4].vNoraml;
				cnt += 2;
			}
			if (i < VTXCNTZ - 1 && j>0) {
				int index5 = (i * (VTXCNTX - 1) + (j - 1)) * 2 + 1;
				vVtxNormal += m_vecFaces[index5].vNoraml;
				++cnt;
			}
			
			int index = i * VTXCNTX + j;

			m_vecVertices[index].vNormal = vVtxNormal / cnt;
		}
	}

	// 3. 정점 법선으로 텍셀의 색상에 명암 처리하기
	_vec3 dirToLight = { 1, 1, 1 };
	D3DXVec3Normalize(&dirToLight, &dirToLight);
	
	D3DLOCKED_RECT lockedRect;
	m_pTexture->LockRect(0, &lockedRect, 0, 0);
	DWORD* imageData = (DWORD*)lockedRect.pBits;

	vector<DWORD> original(texHeight* (lockedRect.Pitch / 4));
	memcpy(original.data(), imageData, original.size() * sizeof(DWORD));

	for (int i = 0; i < m_vecFaces.size(); ++i) {
		VTXMESH v0, v1, v2;
		_vec2 uv0, uv1, uv2;
		_vec3 normal0, normal1, normal2;

		v0 = m_vecVertices[m_vecFaces[i].indices._0];
		v1 = m_vecVertices[m_vecFaces[i].indices._1];
		v2 = m_vecVertices[m_vecFaces[i].indices._2];

		uv0 = v0.vTexUV;
		uv1 = v1.vTexUV;
		uv2 = v2.vTexUV;

		normal0 = v0.vNormal;
		normal1 = v1.vNormal;
		normal2 = v2.vNormal;

		// v0, v1, v2가 면을 형성한다
		// uv로부터 텍셀 좌표를 구한다.
		_vec2 A = { uv0.x * texWidth, uv0.y * texHeight };
		_vec2 B = { uv1.x * texWidth, uv1.y * texHeight };
		_vec2 C = { uv2.x * texWidth, uv2.y * texHeight };

		// 바운딩 박스를 계산하여 연속된 영역을 정수 텍셀 격자로 끊기
		int minX = (int)floor	(min(A.x, min(B.x, C.x)));
		int maxX = (int)ceil	(max(A.x, max(B.x, C.x)));
		int minY = (int)floor	(min(A.y, min(B.y, C.y)));
		int maxY = (int)ceil	(max(A.y, max(B.y, C.y)));

		// 텍스처 범위로 clamp
		minX = max(minX, 0);
		maxX = min(maxX, texWidth-1);
		minY = max(minY, 0);
		maxY = min(maxY, texHeight-1);

		// 무게중심좌표를 활용
		// 삼각형 내부의 점 P를 세 꼭짓점 A, B, C가 당기는 영향력의 비율로 표현하는 것
		// P = uA + vB + wC
		
		// 나눌 때 쓰는 분모
		float denom = (B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y);
		if (fabsf(denom) < 0.01) // 퇴화 삼각형 방어
			continue;

		// 바운딩 박스 내부 텍셀을 픽셀 단위로 순회
		for (int py = minY; py <= maxY; ++py) {
			for(int px = minX; px <= maxX; ++px){
				// 텍셀 "중심"을 검사점으로
				float Px = px + 0.5f;
				float Py = py + 0.5f;

				// 3) 무게중심좌표
				float w0 = ((B.y - C.y) * (Px - C.x) + (C.x - B.x) * (Py - C.y)) / denom;
				float w1 = ((C.y - A.y) * (Px - C.x) + (A.x - C.x) * (Py - C.y)) / denom;
				float w2 = 1.f - w0 - w1;

				// 4) 삼각형 내부 판정
				if (w0 < 0.f || w1 < 0.f || w2 < 0.f)
					continue;

				// 5) 노멀 보간 (무게중심좌표가 곧 가중치)
				_vec3 normal = normal0 * w0 + normal1 * w1 + normal2 * w2;
				D3DXVec3Normalize(&normal, &normal);

				// 6) 텍셀 인덱스 (Pitch 사용 주의!)
				int index = py * (lockedRect.Pitch / 4) + px;

				D3DXCOLOR c(original[index]);

				c *= ComputeShade(&normal, &dirToLight);
				imageData[index] = (D3DCOLOR)c;
			}
		}
	}

	m_pTexture->UnlockRect(0);

	if (FAILED(D3DXFilterTexture(m_pTexture, 0, 0, D3DX_DEFAULT)))
		return E_FAIL;

	return S_OK;
}

void CTerrain2::Render_Buffer()
{
	m_pGraphicDev->SetTexture(0, m_pTexture);
	CVIBuffer::Render_Buffer();
}

CTerrain2* CTerrain2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain2* pTerrain2 = new CTerrain2(pGraphicDev);

	if (FAILED(pTerrain2->Ready_Buffer())) {
		Safe_Release(pTerrain2);
		MSG_BOX("pTerrain2 Create Fail");
		return nullptr;
	}

	return pTerrain2;
}

CComponent* CTerrain2::Clone()
{
	CComponent* pComp = new CTerrain2(*this);

	return pComp;
}



_float CTerrain2::ComputeShade(_vec3* normal, _vec3* dirToLight)
{
	float cosine = D3DXVec3Dot(normal, dirToLight);

	if (cosine < 0.f)
		cosine = 0.f;

	return cosine;
}

void CTerrain2::Free()
{
	Safe_Release(m_pTexture);
	CVIBuffer::Free();
}
