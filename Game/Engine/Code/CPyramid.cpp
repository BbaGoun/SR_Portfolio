#include "CPyramid.h"
#include "CTexture.h"
#include "CGameObject.h"

CPyramid::CPyramid(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CPyramid::CPyramid(const CPyramid& rhs) : CVIBuffer(rhs)
{
}

CPyramid::~CPyramid()
{
}

HRESULT CPyramid::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 13;
	m_dwTriCnt = 6;
	m_dwFVF = FVF_TEX;

	m_dwIdxCnt = 18;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* vertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0].vPosition = { 0, 1, 0 };
	vertices[0].vTexUV = { 0.4444f, 0.f };

	vertices[1].vPosition = { -1, -1, 1 };
	vertices[1].vTexUV = { 0.8888f, 0.8888f };

	vertices[2].vPosition = { 1, -1, 1 };
	vertices[2].vTexUV = { 0.f, 0.8888f };

	vertices[3].vPosition = { 1, -1, 1 };
	vertices[3].vTexUV = { 0.8888f, 0.8888f };

	vertices[4].vPosition = { 1, -1, -1 };
	vertices[4].vTexUV = { 0.f, 0.8888f };

	vertices[5].vPosition = { 1, -1, -1 };
	vertices[5].vTexUV = { 0.8888f, 0.8888f };

	vertices[6].vPosition = { -1, -1, -1 };
	vertices[6].vTexUV = { 0.f, 0.8888f };

	vertices[7].vPosition = { -1, -1, -1 };
	vertices[7].vTexUV = { 0.8888f, 0.8888f };

	vertices[8].vPosition = { -1, -1, 1 };
	vertices[8].vTexUV = { 0.f, 0.8888f };

	vertices[9].vPosition = { -1, -1, 1 };
	vertices[9].vTexUV = { 0.2f, 0.75f };

	vertices[10].vPosition = { 1, -1, 1 };
	vertices[10].vTexUV = { 0.8f, 0.75f };

	vertices[11].vPosition = { 1, -1, -1 };
	vertices[11].vTexUV = { 0.8f, 0.15f };

	vertices[12].vPosition = { -1, -1, -1 };
	vertices[12].vTexUV = { 0.2f, 0.15f };

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	indices[0]._0 = 0;
	indices[0]._1 = 1;
	indices[0]._2 = 2;

	indices[1]._0 = 0;
	indices[1]._1 = 3;
	indices[1]._2 = 4;

	indices[2]._0 = 0;
	indices[2]._1 = 5;
	indices[2]._2 = 6;

	indices[3]._0 = 0;
	indices[3]._1 = 7;
	indices[3]._2 = 8;

	indices[4]._0 = 12;
	indices[4]._1 = 11;
	indices[4]._2 = 10;

	indices[5]._0 = 12;
	indices[5]._1 = 10;
	indices[5]._2 = 9;

	m_pIB->Unlock();

	return S_OK;
}

void CPyramid::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	if (m_dwFVF != 0)
		m_pGraphicDev->SetFVF(m_dwFVF);
	else
		m_pGraphicDev->SetVertexDeclaration(m_pVtxDecl);
	m_pGraphicDev->SetIndices(m_pIB);

	CTexture* pTexture = static_cast<CTexture*>(m_pOwner->Get_Component(ID_STATIC, L"Com_Texture"));

	// 옆면: 인덱스 0~11 (정점 0~8), 삼각형 4개
	pTexture->Set_Texture(0);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 9, 0, 4);

	// 바닥: 인덱스 12~17 (정점 9~12), 삼각형 2개
	pTexture->Set_Texture(1);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 9, 4, 12, 2);
}

CPyramid* CPyramid::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPyramid* pCPyramid = new CPyramid(pGraphicDev);

	if (FAILED(pCPyramid->Ready_Buffer())) {
		Safe_Release(pCPyramid);
		MSG_BOX("pPyramid Create Fail");
		return nullptr;
	}

	return pCPyramid;
}

CComponent* CPyramid::Clone()
{
	CComponent* pComp = new CPyramid(*this);

	return pComp;
}

void CPyramid::Free()
{
	CVIBuffer::Free();
}
