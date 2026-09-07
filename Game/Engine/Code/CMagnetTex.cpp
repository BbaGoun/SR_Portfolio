#include "CMagnetTex.h"

CMagnetTex::CMagnetTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CMagnetTex::CMagnetTex(const CMagnetTex& rhs)
	: CVIBuffer(rhs)
{
}

CMagnetTex::~CMagnetTex()
{
}

HRESULT CMagnetTex::Ready_Buffer()
{
	// XZ ??? ?? ??? Y? ??? ? ??? ?? ??.
	const _uint iCurveSegmentCount = 16;
	const _uint iSegmentCount = iCurveSegmentCount + 4;

	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = iSegmentCount * 8;
	m_dwTriCnt = iSegmentCount * 8 + 4;
	m_dwFVF = FVF_COL;
	m_dwIdxCnt = m_dwTriCnt * 3;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCOL* pVertex = nullptr;
	INDEX32* pIndex = nullptr;
	if (FAILED(m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertex), 0)))
		return E_FAIL;
	if (FAILED(m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndex), 0)))
	{
		m_pVB->Unlock();
		return E_FAIL;
	}

	_uint iVertexCursor = 0;
	_uint iTriangleCursor = 0;

	auto AddTriangle = [&](const _uint i0, const _uint i1, const _uint i2)
		{
			pIndex[iTriangleCursor]._0 = i0;
			pIndex[iTriangleCursor]._1 = i1;
			pIndex[iTriangleCursor]._2 = i2;
			++iTriangleCursor;
		};

	auto SetVertex = [&](const _uint i, const _float x, const _float y,
		const _float z, const D3DCOLOR color)
		{
			pVertex[i].vPosition = { x, y, z };
			pVertex[i].dwColor = color;
			UpdateMinMaxVtx(pVertex[i].vPosition);
		};

	// p0/p1? ?? ??, n0/n1? XZ ???? ?? ?? ????.
	auto AddSegment = [&](const _float x0, const _float z0, const _float nx0, const _float nz0,
		const _float x1, const _float z1, const _float nx1, const _float nz1,
		const D3DCOLOR topColor, const D3DCOLOR bottomColor)
		{
			const _float fHalfWidth = 0.07f;
			const _float fHalfHeight = 0.10f;
			const _uint b = iVertexCursor;

			SetVertex(b + 0, x0 + nx0 * fHalfWidth,  fHalfHeight, z0 + nz0 * fHalfWidth, topColor);
			SetVertex(b + 1, x0 - nx0 * fHalfWidth,  fHalfHeight, z0 - nz0 * fHalfWidth, topColor);
			SetVertex(b + 2, x1 + nx1 * fHalfWidth,  fHalfHeight, z1 + nz1 * fHalfWidth, topColor);
			SetVertex(b + 3, x1 - nx1 * fHalfWidth,  fHalfHeight, z1 - nz1 * fHalfWidth, topColor);
			SetVertex(b + 4, x0 + nx0 * fHalfWidth, -fHalfHeight, z0 + nz0 * fHalfWidth, bottomColor);
			SetVertex(b + 5, x0 - nx0 * fHalfWidth, -fHalfHeight, z0 - nz0 * fHalfWidth, bottomColor);
			SetVertex(b + 6, x1 + nx1 * fHalfWidth, -fHalfHeight, z1 + nz1 * fHalfWidth, bottomColor);
			SetVertex(b + 7, x1 - nx1 * fHalfWidth, -fHalfHeight, z1 - nz1 * fHalfWidth, bottomColor);

			// ?, ??, ???, ?? ?.
			AddTriangle(b + 0, b + 1, b + 2);
			AddTriangle(b + 1, b + 3, b + 2);
			AddTriangle(b + 4, b + 6, b + 5);
			AddTriangle(b + 5, b + 6, b + 7);
			AddTriangle(b + 0, b + 2, b + 4);
			AddTriangle(b + 2, b + 6, b + 4);
			AddTriangle(b + 1, b + 5, b + 3);
			AddTriangle(b + 3, b + 5, b + 7);
			iVertexCursor += 8;
		};

	const D3DCOLOR grayTop = D3DCOLOR_XRGB(105, 105, 105);
	const D3DCOLOR grayBottom = D3DCOLOR_XRGB(55, 55, 55);
	const D3DCOLOR redTop = D3DCOLOR_XRGB(235, 35, 30);
	const D3DCOLOR redBottom = D3DCOLOR_XRGB(135, 10, 10);
	const D3DCOLOR blueTop = D3DCOLOR_XRGB(45, 45, 235);
	const D3DCOLOR blueBottom = D3DCOLOR_XRGB(15, 15, 130);
	const _float fRadius = 0.18f;
	const _float fTipZ = 0.30f;
	const _float fPoleStartZ = 0.16f;

	// ?? ?? ?? ?? ??.
	AddSegment(-fRadius, fTipZ, -1.f, 0.f,
		-fRadius, fPoleStartZ, -1.f, 0.f, redTop, redBottom);
	AddSegment(-fRadius, fPoleStartZ, -1.f, 0.f,
		-fRadius, 0.f, -1.f, 0.f, grayTop, grayBottom);

	// ???? ?? ?? ????? ???? ?? ??.
	for (_uint i = 0; i < iCurveSegmentCount; ++i)
	{
		const _float theta0 = D3DX_PI - D3DX_PI * static_cast<_float>(i) / iCurveSegmentCount;
		const _float theta1 = D3DX_PI - D3DX_PI * static_cast<_float>(i + 1) / iCurveSegmentCount;
		const _float cos0 = cosf(theta0);
		const _float sin0 = sinf(theta0);
		const _float cos1 = cosf(theta1);
		const _float sin1 = sinf(theta1);

		AddSegment(fRadius * cos0, -fRadius * sin0, cos0, -sin0,
			fRadius * cos1, -fRadius * sin1, cos1, -sin1,
			grayTop, grayBottom);
	}

	// ??? ?? ??? ?? ?.
	AddSegment(fRadius, 0.f, 1.f, 0.f,
		fRadius, fPoleStartZ, 1.f, 0.f, grayTop, grayBottom);
	AddSegment(fRadius, fPoleStartZ, 1.f, 0.f,
		fRadius, fTipZ, 1.f, 0.f, blueTop, blueBottom);

	// ?? ?? ?? ??? ???.
	AddTriangle(0, 4, 1);
	AddTriangle(1, 4, 5);
	const _uint iLastBase = (iSegmentCount - 1) * 8;
	AddTriangle(iLastBase + 2, iLastBase + 3, iLastBase + 6);
	AddTriangle(iLastBase + 3, iLastBase + 7, iLastBase + 6);

	m_pIB->Unlock();
	m_pVB->Unlock();
	SetBoundingBox();
	return S_OK;
}

void CMagnetTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CMagnetTex* CMagnetTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMagnetTex* pCubeTex = new CMagnetTex(pGraphicDev);

	if (FAILED(pCubeTex->Ready_Buffer()))
	{
		Safe_Release(pCubeTex);
		MSG_BOX("pCubeTex Create Failed");
		return nullptr;
	}

	return pCubeTex;
}

CComponent* CMagnetTex::Clone()
{
	return new CMagnetTex(*this);
}

void CMagnetTex::Free()
{
	CVIBuffer::Free();
}
