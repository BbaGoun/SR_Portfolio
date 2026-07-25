#include "CPskFile.h"

CPskFile::CPskFile() : CVIBuffer()
, m_pBaseColorMap(nullptr)
{
}

CPskFile::CPskFile(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
, m_pBaseColorMap(nullptr)
{
}

CPskFile::CPskFile(const CPskFile& rhs) : CVIBuffer(rhs)
, m_pBaseColorMap(rhs.m_pBaseColorMap)
, m_vecBones(rhs.m_vecBones)
, m_vecMaterials(rhs.m_vecMaterials)
{
	if (m_pVB)				m_pVB->AddRef();
	if (m_pIB)				m_pIB->AddRef();
	if (m_pVtxDecl)			m_pVtxDecl->AddRef();
	if (m_pBaseColorMap)	m_pBaseColorMap->AddRef();
}

CPskFile::~CPskFile()
{
}

HRESULT CPskFile::LoadPsk(const WCHAR* pPskPath)
{
	HANDLE hFile = CreateFile(pPskPath,
		GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		WCHAR wMsg[256];
		swprintf_s(wMsg, L"LoadPsk Open Fail : %s", pPskPath);
		MSG_BOXF(wMsg);
		return E_FAIL;
	}

	DWORD dwByte(0);
	VChunkHeader header{};

	// ── ACTRHEAD ──────────────────────────────────
	ReadFile(hFile, &header, sizeof(VChunkHeader), &dwByte, nullptr);
	if (strncmp(header.ChunkID, "ACTRHEAD", 8) != 0)
	{
		CloseHandle(hFile);
		MSG_BOX("LoadPsk : Invalid PSK (ACTRHEAD)");
		return E_FAIL;
	}

	// ── PNTS0000 ──────────────────────────────────
	ReadFile(hFile, &header, sizeof(VChunkHeader), &dwByte, nullptr);
	vector<_vec3> points(header.DataCount);
	if (header.DataCount > 0)
		ReadFile(hFile, points.data(), header.DataSize * header.DataCount, &dwByte, nullptr);

	uint32_t numPoints = (uint32_t)points.size();

	// ── 오른손 좌표계 -> 왼손 좌표계 ──────────────────────
	// 모델들은 +x를 바라보며, +y가 오른쪽이고, +z가 머리 방향인 좌표계를 사용한다.
	// 이를 DirectX에 맞도록 변형해야한다.

	for (auto& p : points) {
		float oldX = p.x, oldY = p.y, oldZ = p.z;
		p.x = oldY; // 오른쪽이 x에 오게
		p.y = oldZ; // 머리가 y에 오게
		p.z = oldX; // 바라보는 방향이 z에 오게
	}

	// ── VTXW0000 ──────────────────────────────────
	ReadFile(hFile, &header, sizeof(VChunkHeader), &dwByte, nullptr);
	uint32_t wedgeCount = header.DataCount;

	vector<uint32_t>	wdgPointIdx(wedgeCount);
	vector<float>		wdgU(wedgeCount), wdgV(wedgeCount);

	if (numPoints <= 65536)
	{
		vector<PSKWedgeL> wedges(wedgeCount);
		if (wedgeCount > 0)
			ReadFile(hFile, wedges.data(), header.DataSize * wedgeCount, &dwByte, nullptr);
		for (uint32_t i = 0; i < wedgeCount; ++i)
		{
			wdgPointIdx[i]	= wedges[i].PointIndex;
			wdgU[i]			= wedges[i].U;
			wdgV[i]			= wedges[i].V;
		}
	}
	else
	{
		vector<PSKWedgeG> wedges(wedgeCount);
		if (wedgeCount > 0)
			ReadFile(hFile, wedges.data(), header.DataSize * wedgeCount, &dwByte, nullptr);
		for (uint32_t i = 0; i < wedgeCount; ++i)
		{
			wdgPointIdx[i]	= wedges[i].PointIndex;
			wdgU[i]			= wedges[i].U;
			wdgV[i]			= wedges[i].V;
		}
	}

	// ── FACE0000 / FACE0032 ──────────────────────
	ReadFile(hFile, &header, sizeof(VChunkHeader), &dwByte, nullptr);
	uint32_t triCount = header.DataCount;
	bool bFace32 = (strncmp(header.ChunkID, "FACE0032", 8) == 0);

	vector<DWORD> faceIdx;
	faceIdx.reserve(triCount * 3);

	if (bFace32)
	{
		vector<PSKTriangle32> faces(triCount);
		if (triCount > 0)
			ReadFile(hFile, faces.data(), header.DataSize * triCount, &dwByte, nullptr);
		for (uint32_t i = 0; i < triCount; ++i)
		{
			faceIdx.push_back(faces[i].WedgeIndex[0]);
			faceIdx.push_back(faces[i].WedgeIndex[1]);
			faceIdx.push_back(faces[i].WedgeIndex[2]);
		}
	}
	else
	{
		vector<PSKTriangle00> faces(triCount);
		if (triCount > 0)
			ReadFile(hFile, faces.data(), header.DataSize * triCount, &dwByte, nullptr);
		for (uint32_t i = 0; i < triCount; ++i)
		{
			faceIdx.push_back((DWORD)faces[i].WedgeIndex[0]);
			faceIdx.push_back((DWORD)faces[i].WedgeIndex[1]);
			faceIdx.push_back((DWORD)faces[i].WedgeIndex[2]);
		}
	}

	// ── MATT0000 ──────────────────────────────────
	ReadFile(hFile, &header, sizeof(VChunkHeader), &dwByte, nullptr);
	m_vecMaterials.resize(header.DataCount);
	if (header.DataCount > 0)
		ReadFile(hFile, m_vecMaterials.data(), header.DataSize * header.DataCount, &dwByte, nullptr);

	// ── REFSKELT ──────────────────────────────────
	ReadFile(hFile, &header, sizeof(VChunkHeader), &dwByte, nullptr);
	m_vecBones.resize(header.DataCount);
	if (header.DataCount > 0)
		ReadFile(hFile, m_vecBones.data(), header.DataSize * header.DataCount, &dwByte, nullptr);

	// ── RAWWEIGHTS ────────────────────────────────
	ReadFile(hFile, &header, sizeof(VChunkHeader), &dwByte, nullptr);
	vector<PSKRawBoneWeight> rawWeights(header.DataCount);
	if (header.DataCount > 0)
		ReadFile(hFile, rawWeights.data(), header.DataSize * header.DataCount, &dwByte, nullptr);

	CloseHandle(hFile);

	// ── Per-point bone weight 수집 ────────────────
	// 각 포인트별로 (boneIndex, weight) 쌍을 모아 상위 4개만 보존
	vector<vector<pair<uint32_t, float>>> ptWeights(numPoints);

	for (const auto& rw : rawWeights)
	{
		if (rw.PointIndex < numPoints)
			ptWeights[rw.PointIndex].emplace_back(rw.BoneIndex, rw.Weight);
	}

	for (auto& pw : ptWeights)
	{
		sort(pw.begin(), pw.end(),
			[](const pair<uint32_t, float>& a, const pair<uint32_t, float>& b) {
				return a.second > b.second;
			});

		if (pw.size() > 4)
			pw.resize(4);

		float fSum = 0.f;
		for (auto& p : pw) fSum += p.second;
		if (fSum > 0.f)
			for (auto& p : pw) p.second /= fSum;
	}

	// ── Wedge → VTXMESH 변환 ─────────────────────
	// 웨지 1개 = 최종 버텍스 1개 (포인트 위치 + UV + 본 웨이트)
	vector<VTXMESH> vecVtx(wedgeCount);

	for (uint32_t i = 0; i < wedgeCount; ++i)
	{
		uint32_t pi = wdgPointIdx[i];
		VTXMESH& v	= vecVtx[i];

		v.vPosition = { points[pi].x, points[pi].y, points[pi].z };
		v.vNormal	= { 0.f, 0.f, 0.f };
		v.vTexUV	= { wdgU[i], wdgV[i] };

		memset(v.aBoneWeights, 0, sizeof(v.aBoneWeights));
		memset(v.aBoneIndices, 0, sizeof(v.aBoneIndices));

		if (pi < numPoints)
		{
			const auto& pw = ptWeights[pi];
			for (size_t j = 0; j < pw.size() && j < 4; ++j)
			{
				v.aBoneWeights[j] = pw[j].second;
				v.aBoneIndices[j] = static_cast<BYTE>(pw[j].first);
			}
		}
	}

	// ── 노말 계산 (면 법선 누적 → 정규화) ────────
	// PSK 포맷은 노말을 저장하지 않으므로 직접 계산
	for (uint32_t i = 0; i < triCount; ++i)
	{
		DWORD i0 = faceIdx[i * 3 + 0];
		DWORD i1 = faceIdx[i * 3 + 1];
		DWORD i2 = faceIdx[i * 3 + 2];

		_vec3 e1, e2, fn;
		D3DXVec3Subtract(&e1, &vecVtx[i1].vPosition, &vecVtx[i0].vPosition);
		D3DXVec3Subtract(&e2, &vecVtx[i2].vPosition, &vecVtx[i0].vPosition);
		D3DXVec3Cross(&fn, &e1, &e2);

		vecVtx[i0].vNormal += fn;
		vecVtx[i1].vNormal += fn;
		vecVtx[i2].vNormal += fn;
	}

	for (auto& v : vecVtx)
		D3DXVec3Normalize(&v.vNormal, &v.vNormal);

	// ── D3D 리소스 생성 ──────────────────────────
	m_dwVtxSize = sizeof(VTXMESH);
	m_dwVtxCnt	= wedgeCount;
	m_dwTriCnt	= triCount;
	m_dwIdxCnt	= (DWORD)faceIdx.size();
	m_IdxFmt	= D3DFMT_INDEX32;

	if (FAILED(m_pGraphicDev->CreateVertexDeclaration(MeshVertexElements, &m_pVtxDecl)))
		return E_FAIL;

	// ── 정점 버퍼 ───────────────────────────────
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(
		m_dwVtxCnt * m_dwVtxSize,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pVB, 0)))
		return E_FAIL;

	void* pData = nullptr;
	m_pVB->Lock(0, 0, &pData, 0);
	memcpy(pData, vecVtx.data(), m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();

	// ── 인덱스 버퍼 ──────────────────────────────
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxCnt * sizeof(DWORD),
		D3DUSAGE_WRITEONLY,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB, 0)))
		return E_FAIL;

	m_pIB->Lock(0, 0, &pData, 0);
	memcpy(pData, faceIdx.data(), sizeof(DWORD) * m_dwIdxCnt);
	m_pIB->Unlock();

	return S_OK;
}

CPskFile* CPskFile::Create(LPDIRECT3DDEVICE9 pGraphicDev, const WCHAR* pPskPath)
{
	CPskFile* pPsk = new CPskFile(pGraphicDev);

	if (FAILED(pPsk->LoadPsk(pPskPath)))
	{
		Safe_Release(pPsk);
		MSG_BOX("CPskFile Create Fail");
		return nullptr;
	}

	return pPsk;
}

HRESULT CPskFile::Ready_Buffer()
{
	return S_OK;
}

void CPskFile::Render_Buffer()
{


	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetVertexDeclaration(m_pVtxDecl);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

CComponent* CPskFile::Clone()
{
	return new CPskFile(*this);
}

void CPskFile::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pVtxDecl);
	Safe_Release(m_pBaseColorMap);

	CComponent::Free();
}
