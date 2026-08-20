#include "CVIBuffer.h"
#include "CGraphicDev.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev):CComponent(pGraphicDev)
, m_pVB(nullptr), m_pIB(nullptr)
, m_dwVtxSize(0), m_dwVtxCnt(0)
, m_dwTriCnt(0), m_dwFVF(0)
, m_pVtxDecl(nullptr)
, m_dwIdxCnt(0), m_IdxFmt(D3DFMT_INDEX32)
, m_minVtx({ FLT_MAX, FLT_MAX, FLT_MAX })
, m_maxVtx({ FLT_MIN, FLT_MIN, FLT_MIN })
{
	m_eID = ID_STATIC;
	m_eKind = CK_MESH;
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs) : CComponent(rhs)
, m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB)
, m_dwVtxSize(rhs.m_dwVtxSize), m_dwVtxCnt(rhs.m_dwVtxCnt)
, m_dwTriCnt(rhs.m_dwTriCnt), m_dwFVF(rhs.m_dwFVF)
, m_pVtxDecl(rhs.m_pVtxDecl)
, m_dwIdxCnt(rhs.m_dwIdxCnt), m_IdxFmt(rhs.m_IdxFmt)
, m_minVtx(rhs.m_minVtx)
, m_maxVtx(rhs.m_maxVtx)
, m_boundingBox(rhs.m_boundingBox)
{
	m_eID = ID_STATIC;
	m_eKind = CK_MESH;

	if (m_pVB)			m_pVB->AddRef();
	if (m_pIB)			m_pIB->AddRef();
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT CVIBuffer::Ready_Buffer()
{
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(
		m_dwVtxCnt * m_dwVtxSize,
		D3DUSAGE_WRITEONLY,
		m_dwFVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		0)
	))
		return E_FAIL;

	int idxSize = m_IdxFmt == D3DFMT_INDEX16 ? sizeof(WORD) : sizeof(DWORD);

	if(FAILED(m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxCnt * idxSize,
		D3DUSAGE_WRITEONLY,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		0)
	))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	if (m_dwFVF != 0)
		m_pGraphicDev->SetFVF(m_dwFVF);
	else
		m_pGraphicDev->SetVertexDeclaration(m_pVtxDecl);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void CVIBuffer::SetBoundingBox()
{
	DirectX::XMVECTOR vMin = ToXMVec(m_minVtx);
	DirectX::XMVECTOR vMax = ToXMVec(m_maxVtx);
	DirectX::BoundingBox::CreateFromPoints(m_boundingBox, vMin, vMax);
}

void CVIBuffer::UpdateMinMaxVtx(_vec3 position)
{
	if (m_minVtx.x > position.x)
		m_minVtx.x = position.x;
	if (m_minVtx.y > position.y)
		m_minVtx.y = position.y;
	if (m_minVtx.z > position.z)
		m_minVtx.z = position.z;

	if (m_maxVtx.x < position.x)
		m_maxVtx.x = position.x;
	if (m_maxVtx.y < position.y)
		m_maxVtx.y = position.y;
	if (m_maxVtx.z < position.z)
		m_maxVtx.z = position.z;
}

CComponent* CVIBuffer::Clone()
{
	CComponent* pComp(this);

	return pComp;
}

void CVIBuffer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pVtxDecl);
	CComponent::Free();
}
