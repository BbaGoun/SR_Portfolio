#include "CCartWheelCol.h"
#include "CGameObject.h"
#include "CTexture.h"

CCartWheelCol::CCartWheelCol(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CCartWheelCol::CCartWheelCol(const CCartWheelCol& rhs) : CVIBuffer(rhs)
{
}

CCartWheelCol::~CCartWheelCol()
{
}

HRESULT CCartWheelCol::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXTEX);
	// 옆면 왼쪽 1 + 17
	// 윗면 17 + 17 
	// 옆면 오른쪽 17 + 1
	m_dwVtxCnt = 1 + (m_iSegment + 1) * 4 + 1;
	// 옆면 왼쪽 16
	// 윗면 16 * 2
	// 옆면 오른쪽 16
	m_dwTriCnt = m_iSegment * 4;
	m_dwFVF = FVF_TEX;

	m_dwIdxCnt = m_dwTriCnt * 3;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* vertices = nullptr;
	int offset = 0;

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	// 왼쪽 옆면
	// 1개
	vertices[0].vPosition = { -0.2f, 0, 0 };
	vertices[0].vTexUV = { 0.5f, 0.5f };
	// 17개
	offset = 1;
	for (int i = 0; i <= m_iSegment; ++i)
	{
		float radian = D3DXToRadian(360.f * i / m_iSegment);
		vertices[i + offset].vPosition = { -0.2f,
			cosf(radian),
			sinf(radian)
		};
		vertices[i + offset].vTexUV = { 0.5f - sinf(radian) / 2.f, 0.5f - cosf(radian) / 2.f };
	}

	// 윗면
	// 17개
	offset = 1 + (m_iSegment + 1);
	for (int i = 0; i <= m_iSegment; ++i)
	{
		float radian = D3DXToRadian(360.f * i / m_iSegment);
		vertices[i + offset].vPosition = { -0.2f,
			cosf(radian),
			sinf(radian)
		};
		vertices[i + offset].vTexUV = { 0, float(i) };
	}
	// 17개
	offset = 1 + (m_iSegment + 1) * 2;
	for (int i = 0; i <= m_iSegment; ++i)
	{
		float radian = D3DXToRadian(360.f * i / m_iSegment);
		vertices[i + offset].vPosition = { 0.2f,
			cosf(radian),
			sinf(radian)
		};
		vertices[i + offset].vTexUV = { 1, float(i) };
	}

	// 오른쪽 옆면
	// 17개
	offset = 1 + (m_iSegment + 1) * 3;
	for (int i = 0; i <= m_iSegment; ++i)
	{
		float radian = D3DXToRadian(360.f * i / m_iSegment);
		vertices[i + offset].vPosition = { 0.2f,
			cosf(radian),
			sinf(radian)
		};
		vertices[i + offset].vTexUV = { 0.5f - sinf(radian) / 2.f, 0.5f - cosf(radian) / 2.f };
	}
	// 1개
	vertices[m_dwVtxCnt - 1].vPosition = { 0.2f, 0, 0 };
	vertices[m_dwVtxCnt - 1].vTexUV = { 0.5f, 0.5f };

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(vertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();

	INDEX32* indices = nullptr;

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	// 왼쪽 면 
	// 점 -> 중앙 -> 다음 점
	for (int i = 0; i < m_iSegment; ++i) {
		indices[i]._0 = i + 1;
		indices[i]._1 = 0;
		indices[i]._2 = i + 2;
	}

	//// 윗면
	offset = 1 + (m_iSegment + 1);
	for (int i = 0; i < m_iSegment; ++i) {
		indices[i * 2 + m_iSegment]._0 = offset + i;
		indices[i * 2 + m_iSegment]._1 = offset + i + 1;
		indices[i * 2 + m_iSegment]._2 = offset + i + 1 + (m_iSegment + 1);

		indices[i * 2 + m_iSegment + 1]._0 = offset + i;
		indices[i * 2 + m_iSegment + 1]._1 = offset + i + 1 + (m_iSegment + 1);
		indices[i * 2 + m_iSegment + 1]._2 = offset + i + (m_iSegment + 1);
	}

	// 오른쪽 면
	offset = 1 + (m_iSegment + 1) * 3;
	for (int i = 0; i < m_iSegment; ++i) {
		indices[i + m_iSegment * 3]._0 = offset + i + 1;
		indices[i + m_iSegment * 3]._1 = m_dwVtxCnt - 1;
		indices[i + m_iSegment * 3]._2 = offset + i;
	}

	m_pIB->Unlock();

	return S_OK;
}

void CCartWheelCol::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	if (m_dwFVF != 0)
		m_pGraphicDev->SetFVF(m_dwFVF);
	else
		m_pGraphicDev->SetVertexDeclaration(m_pVtxDecl);
	m_pGraphicDev->SetIndices(m_pIB);

	if (CTexture* pTex = m_pOwner->Get_Component<CTexture>()) 
	{
		// 왼쪽 옆 면
		// 매개변수는 인덱스 offset, 그리는 삼각형 수
		pTex->Set_Texture(0);
		m_pGraphicDev->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			m_dwVtxCnt,
			0,
			m_iSegment);

		// 윗면
		pTex->Set_Texture(1);
		m_pGraphicDev->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			m_dwVtxCnt,
			m_iSegment * 3,
			m_iSegment * 2);

		// 오른쪽 옆면
		pTex->Set_Texture(0);
		m_pGraphicDev->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			m_dwVtxCnt,
			m_iSegment * 9,
			m_iSegment);
	}
}

CCartWheelCol* CCartWheelCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCartWheelCol* pCCartWheelCol = new CCartWheelCol(pGraphicDev);

	if (FAILED(pCCartWheelCol->Ready_Buffer())) {
		Safe_Release(pCCartWheelCol);
		MSG_BOX("CCartWheelCol Create Fail");
		return nullptr;
	}

	return pCCartWheelCol;
}

CComponent* CCartWheelCol::Clone()
{
	CComponent* pComp = new CCartWheelCol(*this);

	return pComp;
}

void CCartWheelCol::Free()
{
	CVIBuffer::Free();
}
