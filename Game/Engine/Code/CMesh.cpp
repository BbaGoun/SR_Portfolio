#include "CMesh.h"

CMesh::CMesh() : CComponent() 
, m_pVB(nullptr), m_pIB(nullptr)
, m_dwVtxSize(0), m_dwVtxCnt(0)
, m_dwTriCnt(0), m_dwFVF(0)
, m_pVtxDecl(nullptr)
, m_dwIdxCnt(0), m_IdxFmt(D3DFMT_INDEX32)
, m_pBaseColorMap(nullptr)
{
}

CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev) : CComponent(pGraphicDev)
, m_pVB(nullptr), m_pIB(nullptr)
, m_dwVtxSize(0), m_dwVtxCnt(0)
, m_dwTriCnt(0), m_dwFVF(0)
, m_pVtxDecl(nullptr)
, m_dwIdxCnt(0), m_IdxFmt(D3DFMT_INDEX32)
, m_pBaseColorMap(nullptr)
{
}

CMesh::CMesh(const CMesh& rhs) : CComponent(rhs)
, m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB)
, m_dwVtxSize(rhs.m_dwVtxSize), m_dwVtxCnt(rhs.m_dwVtxCnt)
, m_dwTriCnt(rhs.m_dwTriCnt), m_dwFVF(rhs.m_dwFVF)
, m_pVtxDecl(rhs.m_pVtxDecl)
, m_dwIdxCnt(rhs.m_dwIdxCnt), m_IdxFmt(rhs.m_IdxFmt)
, m_pBaseColorMap(rhs.m_pBaseColorMap)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

CMesh::~CMesh()
{
}

HRESULT CMesh::LoadMesh(const WCHAR* path)
{
	HANDLE hFile = CreateFile(path,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		WCHAR	wMessage[256];
		swprintf_s(wMessage, L"Load Mesh CreateFile Fail : %s", path);
		MSG_BOXF(wMessage);
		return E_FAIL;
	}

	DWORD dwByte(0);
	_uint magic;
	_int version;

	ReadFile(hFile, &magic, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &version, sizeof(_int), &dwByte, nullptr);

	if (magic != 0x534B4D48) {
		WCHAR	wMessage[256];
		swprintf_s(wMessage, L"Load Mesh Magic Fail : %d", magic);
		MSG_BOXF(wMessage);
		return E_FAIL;
	}

	_int verticesLength;
	_int indicesLength;

	ReadFile(hFile, &verticesLength, sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &indicesLength, sizeof(_int), &dwByte, nullptr);

	m_dwVtxSize = sizeof(VTXMESH);
	m_dwVtxCnt = verticesLength;
	m_dwTriCnt = indicesLength / 3;
	//m_dwFVF = 0;

	m_dwIdxCnt = indicesLength;
	//m_IdxFmt = D3DFMT_INDEX32;

	m_pGraphicDev->CreateVertexBuffer(
		m_dwVtxCnt * m_dwVtxSize,
		D3DUSAGE_WRITEONLY,
		m_dwFVF, // D3D DECLARATION을 사용
		D3DPOOL_MANAGED,
		&m_pVB,
		0
	);

	m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxCnt * sizeof(DWORD),
		D3DUSAGE_WRITEONLY,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		0
	);

	// vertex
	_vec3 position;
	_vec3 normal;
	_vec2 texCoord;
	_float boneWeight[4];
	_int boneIndices[4];

	VTXMESH* vertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < verticesLength; ++i) {
		ReadFile(hFile, &position, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &normal, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &texCoord, sizeof(_vec2), &dwByte, nullptr);
		ReadFile(hFile, &boneWeight, sizeof(boneWeight), &dwByte, nullptr);
		ReadFile(hFile, &boneIndices, sizeof(boneIndices), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		VTXMESH vtxMesh{};
		vtxMesh.vPosition = position;
		vtxMesh.vNormal = normal;
		//vtxMesh.vTexUV = texCoord;
		vtxMesh.vTexUV = { texCoord.x, 1.0f - texCoord.y };
		for (int j = 0; j < 4; ++j) {
			vtxMesh.aBoneWeights[j] = boneWeight[j];
			vtxMesh.aBoneIndices[j] = static_cast<BYTE>(boneIndices[j]);
		}

		vertices[i] = vtxMesh;
	}

	m_pVB->Unlock();

	// index
	DWORD index;

	DWORD* indices = nullptr;
	m_pIB->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < indicesLength; ++i) {
		ReadFile(hFile, &index, sizeof(DWORD), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		indices[i] = index;
	}

	m_pIB->Unlock();

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMesh::Ready_Mesh()
{
	return S_OK;
}

void CMesh::Render_Mesh()
{
	m_pGraphicDev->SetTexture(0, m_pBaseColorMap);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetVertexDeclaration(m_pVtxDecl);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

CComponent* CMesh::Clone()
{
    return nullptr;
}

void CMesh::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pVtxDecl);

    CComponent::Free();
}
