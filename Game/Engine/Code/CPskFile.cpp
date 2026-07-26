#include "CPskFile.h"
#include <fstream>
#include <string>

namespace
{
	std::wstring ToWide(const std::string& str)
	{
		if (str.empty())
			return std::wstring();

		int iLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
		if (iLen <= 1)
			return std::wstring();

		std::wstring result(iLen - 1, L'\0');
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &result[0], iLen);
		return result;
	}

	std::string ReadMatValue(const std::wstring& matPath, const char* key)
	{
		std::ifstream fin(matPath);
		if (!fin.is_open())
			return std::string();

		std::string prefix = std::string(key) + "=";
		std::string line;

		while (std::getline(fin, line))
		{
			if (line.compare(0, prefix.size(), prefix) == 0) {
				std::string value = line.substr(prefix.size());
				while (!value.empty() && (value.back() == '\r' || value.back() == ' '))
					value.pop_back();
				return value;
			}
		}

		return std::string();
	}
}

CPskFile::CPskFile() : CVIBuffer()
{
}

CPskFile::CPskFile(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CPskFile::CPskFile(const CPskFile& rhs) : CVIBuffer(rhs)
, m_vecSubsets(rhs.m_vecSubsets)
, m_mapTextures(rhs.m_mapTextures)
, m_vecBones(rhs.m_vecBones)
, m_vecMaterials(rhs.m_vecMaterials)
{
	if (m_pVB)				m_pVB->AddRef();
	if (m_pIB)				m_pIB->AddRef();
	if (m_pVtxDecl)			m_pVtxDecl->AddRef();
	
	for (auto& pair : m_mapTextures)
		pair.second->AddRef();
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
	vector<_vec3> vecPoints(header.DataCount);
	if (header.DataCount > 0)
		ReadFile(hFile, vecPoints.data(), header.DataSize * header.DataCount, &dwByte, nullptr);

	uint32_t numPoints = (uint32_t)vecPoints.size();

	// ── 오른손 좌표계 -> 왼손 좌표계 ──────────────────────
	// 모델들은 +x를 바라보며, +y가 오른쪽이고, +z가 머리 방향인 좌표계를 사용한다.
	// 이를 DirectX에 맞도록 변형해야한다.

	for (auto& p : vecPoints) {
		float oldX = p.x, oldY = p.y, oldZ = p.z;
		p.x = oldY; // 오른쪽이 x에 오게
		p.y = oldZ; // 머리가 y에 오게
		p.z = oldX; // 바라보는 방향이 z에 오게
	}

	// ── VTXW0000 ──────────────────────────────────
	ReadFile(hFile, &header, sizeof(VChunkHeader), &dwByte, nullptr);
	uint32_t wedgeCount = header.DataCount;

	vector<PSKWedgeG>	vecPskWedges(wedgeCount);

	if (numPoints <= 65536)
	{
		vector<PSKWedgeL> wedges(wedgeCount);
		if (wedgeCount > 0)
			ReadFile(hFile, wedges.data(), header.DataSize * wedgeCount, &dwByte, nullptr);
		for (uint32_t i = 0; i < wedgeCount; ++i)
		{
			vecPskWedges[i].PointIndex	= (uint32_t)wedges[i].PointIndex;
			vecPskWedges[i].U			= wedges[i].U;
			vecPskWedges[i].V			= wedges[i].V;
			vecPskWedges[i].MatIndex	= (uint32_t)wedges[i].MatIndex;
		}
	}
	else
	{
		vector<PSKWedgeG> wedges(wedgeCount);
		if (wedgeCount > 0)
			ReadFile(hFile, wedges.data(), header.DataSize * wedgeCount, &dwByte, nullptr);
		for (uint32_t i = 0; i < wedgeCount; ++i)
		{
			vecPskWedges[i].PointIndex	= wedges[i].PointIndex;
			vecPskWedges[i].U			= wedges[i].U;
			vecPskWedges[i].V			= wedges[i].V;
			vecPskWedges[i].MatIndex	= wedges[i].MatIndex;
		}
	}

	// ── FACE0000 / FACE0032 ──────────────────────
	ReadFile(hFile, &header, sizeof(VChunkHeader), &dwByte, nullptr);
	uint32_t triCount = header.DataCount;
	bool bFace00 = (strncmp(header.ChunkID, "FACE0000", 8) == 0);

	vector<PSKTriangle32> vecPskFaces(triCount);

	if (bFace00)
	{
		vector<PSKTriangle00> faces(triCount);
		if (triCount > 0)
			ReadFile(hFile, faces.data(), header.DataSize * triCount, &dwByte, nullptr);
		for (uint32_t i = 0; i < triCount; ++i)
		{
			// 인덱스 순서가 반대여서 뒤집음
			vecPskFaces[i].WedgeIndex[0]		= (uint32_t)faces[i].WedgeIndex[2];
			vecPskFaces[i].WedgeIndex[1]		= (uint32_t)faces[i].WedgeIndex[1];
			vecPskFaces[i].WedgeIndex[2]		= (uint32_t)faces[i].WedgeIndex[0];
			vecPskFaces[i].MatIndex			= faces[i].MatIndex;
			vecPskFaces[i].AuxMatIndex		= faces[i].AuxMatIndex;
			vecPskFaces[i].SmoothingGroups	= faces[i].SmoothingGroups;
		}
	}
	else
	{
		vector<PSKTriangle32> faces(triCount);
		if (triCount > 0)
			ReadFile(hFile, faces.data(), header.DataSize * triCount, &dwByte, nullptr);
		for (uint32_t i = 0; i < triCount; ++i)
		{
			// 인덱스 순서가 반대여서 뒤집음
			vecPskFaces[i].WedgeIndex[0]		= faces[i].WedgeIndex[2];
			vecPskFaces[i].WedgeIndex[1]		= faces[i].WedgeIndex[1];
			vecPskFaces[i].WedgeIndex[2]		= faces[i].WedgeIndex[0];
			vecPskFaces[i].MatIndex			= faces[i].MatIndex;
			vecPskFaces[i].AuxMatIndex		= faces[i].AuxMatIndex;
			vecPskFaces[i].SmoothingGroups	= faces[i].SmoothingGroups;
		}
	}

	// ── MATT0000 ──────────────────────────────────
	ReadFile(hFile, &header, sizeof(VChunkHeader), &dwByte, nullptr);
	m_vecMaterials.resize(header.DataCount);
	if (header.DataCount > 0)
		ReadFile(hFile, m_vecMaterials.data(), header.DataSize * header.DataCount, &dwByte, nullptr);

	// 매핑하여 실제 경로의 텍스처를 가져와 Com객체를 생성해야함.

    std::wstring pskPath = pPskPath;
    size_t slash = pskPath.find_last_of(L"/\\");
    std::wstring baseDir = pskPath.substr(0, slash + 1);

    // baseDir + L"Materials\\Cody_Head.mat"
    // baseDir + L"Textures\\Head_mat_C1.png"

	for (size_t i = 0; i < m_vecMaterials.size(); ++i)
	{
		const PSKMaterial& material = m_vecMaterials[i];

		// Name[64]가 null 종료되지 않을 가능성에 대비해 길이를 제한한다.
		size_t nameLength = strnlen_s(material.Name, sizeof(material.Name));
		std::string materialName(material.Name, nameLength);

		std::wstring matPath =
			baseDir + L"Materials\\" + ToWide(materialName) + L".mat";

		// .mat에서 "Other[0]=" 뒤의 값을 읽는다.
		std::string textureStem = ReadMatValue(matPath, "Other[0]");
		if (textureStem.empty())
			continue;

		std::wstring texturePath =
			baseDir + L"Textures\\" + ToWide(textureStem) + L".png";

		LPDIRECT3DTEXTURE9 texture = nullptr;

		HRESULT hr = D3DXCreateTextureFromFileW(
			m_pGraphicDev,
			texturePath.c_str(),
			&texture);

		if (SUCCEEDED(hr))
			m_mapTextures.emplace(static_cast<uint8_t>(i), texture);
	}


    // ── 서브셋 생성을 위해 MatIndex를 기준으로 정렬 ────────────────
	sort(vecPskFaces.begin(), vecPskFaces.end(),
		[](PSKTriangle32& a, PSKTriangle32& b) {
			return a.MatIndex < b.MatIndex;
		});

	if (!vecPskFaces.empty())
	{
		uint8_t currentMaterial = vecPskFaces[0].MatIndex;
		DWORD firstFace = 0;

		for (DWORD i = 1; i < vecPskFaces.size(); ++i)
		{
			if (vecPskFaces[i].MatIndex != currentMaterial)
			{
				m_vecSubsets.push_back({
					firstFace * 3,
					i - firstFace,
					currentMaterial
					});

				firstFace = i;
				currentMaterial = vecPskFaces[i].MatIndex;
			}
		}

		m_vecSubsets.push_back({
			firstFace * 3,
			static_cast<DWORD>(vecPskFaces.size()) - firstFace,
			currentMaterial
			});
	}

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
		uint32_t pi = vecPskWedges[i].PointIndex;
		VTXMESH& v	= vecVtx[i];

		if (pi >= numPoints)
			continue;

		v.vPosition = { vecPoints[pi].x, vecPoints[pi].y, vecPoints[pi].z };
		v.vNormal	= { 0.f, 0.f, 0.f };
		v.vTexUV	= { vecPskWedges[i].U, vecPskWedges[i].V };

		memset(v.aBoneWeights, 0, sizeof(v.aBoneWeights));
		memset(v.aBoneIndices, 0, sizeof(v.aBoneIndices));

		const auto& pw = ptWeights[pi];
		for (size_t j = 0; j < pw.size() && j < 4; ++j)
		{
			v.aBoneWeights[j] = pw[j].second;
			v.aBoneIndices[j] = static_cast<BYTE>(pw[j].first);
		}
	}

	// ── 노말 계산 (면 법선 누적 → 정규화) ────────
	// PSK 포맷은 노말을 저장하지 않으므로 직접 계산
	for (uint32_t i = 0; i < triCount; ++i)
	{
		DWORD i0 = vecPskFaces[i].WedgeIndex[0];
		DWORD i1 = vecPskFaces[i].WedgeIndex[1];
		DWORD i2 = vecPskFaces[i].WedgeIndex[2];

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
	m_dwIdxCnt = (DWORD)triCount * 3;
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

	INDEX32* pIndices;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (uint32_t i = 0; i < vecPskFaces.size(); ++i) {
		pIndices[i]._0 = vecPskFaces[i].WedgeIndex[0];
		pIndices[i]._1 = vecPskFaces[i].WedgeIndex[1];
		pIndices[i]._2 = vecPskFaces[i].WedgeIndex[2];
	}
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

	for (auto& subset : m_vecSubsets)
	{
		auto iter = m_mapTextures.find(subset.MatIndex);
		m_pGraphicDev->SetTexture(
			0,
			iter != m_mapTextures.end() ? iter->second : nullptr
		);

		m_pGraphicDev->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			m_dwVtxCnt,
			subset.dwStartIndex,
			subset.dwPrimCount);
	}
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

	for (auto& pair : m_mapTextures)
		Safe_Release(pair.second);

	m_mapTextures.clear();

	CComponent::Free();
}
