#include "CHalfSphere.h"

CHalfSphere::CHalfSphere(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CHalfSphere::CHalfSphere(const CHalfSphere& rhs) : CVIBuffer(rhs)
{
}

CHalfSphere::~CHalfSphere()
{
}

HRESULT CHalfSphere::Ready_Buffer()
{
	float radius = 0.5f;
	int slice = 16; // 경도를 나누는 갯수 (xy 차원)
	int stack = 8; // 위도를 나누는 갯수 (z+에서 z-까지 내리는)

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = (slice + 1) * (stack + 1); //한 개씩 더 찍음(원이라 돌아오는 것을 예외처리하는 것보다 이 방법이 더 좋은듯)
	m_dwTriCnt = slice * stack * 2;			// slice 하나당 삼각형 두 개씩
	m_dwFVF = FVF_TEX;

	m_dwIdxCnt = m_dwTriCnt * 3;
	m_IdxFmt = D3DFMT_INDEX32;
	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* vertices = nullptr;
	m_vecVertices.resize(m_dwVtxCnt);

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i <= stack; ++i) {
		// 위도
		float radianB = D3DXToRadian(90 * i / stack);
		// UV 값이 확 바뀌지 않도록 한바퀴 돈 지점에 하나 더 생성
		for (int j = 0; j <= slice; ++j) {
			// 경도
			float radianA = D3DXToRadian(360 * j / slice);
			int index = i * (slice + 1) + j;
			_vec3 pos = radius * _vec3{
				cosf(radianB) * cosf(radianA),
				sinf(radianB),
				cosf(radianB)* sinf(radianA)
				
			};

			vertices[index].vPosition = pos;
			vertices[index].vTexUV = {
				float(j) / float(slice),
				float(i) / float(stack)
			};

			m_vecVertices[index] = vertices[index];
		}
	}

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(vertices[i].vPosition);
	}

	SetBoundingBox();

	m_pVB->Unlock();

	INDEX32* indices = nullptr;
	m_vecFaces.resize(m_dwTriCnt);

	m_pIB->Lock(0, 0, (void**)&indices, 0);

	for (int i = 0; i < stack; ++i) {
		for (int j = 0; j < slice; ++j) {
			int index = i * (slice * 2) + j * 2;

			indices[index]._0 = i * (slice + 1) + j;
			indices[index]._1 = (i + 1) * (slice + 1) + j;
			indices[index]._2 = (i + 1) * (slice + 1) + (j + 1);
			m_vecFaces[index].indices = {
					indices[index]._0,
					indices[index]._1,
					indices[index]._2
			};

			indices[index + 1]._0 = i * (slice + 1) + j;
			indices[index + 1]._1 = (i + 1) * (slice + 1) + (j + 1);
			indices[index + 1]._2 = i * (slice + 1) + (j + 1);
			m_vecFaces[index + 1].indices = {
					indices[index + 1]._0,
					indices[index + 1]._1,
					indices[index + 1]._2
			};
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

CHalfSphere* CHalfSphere::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHalfSphere* pInnerBox = new CHalfSphere(pGraphicDev);

	if (FAILED(pInnerBox->Ready_Buffer())) {
		Safe_Release(pInnerBox);
		MSG_BOX("pInnerBox Create Fail");
		return nullptr;
	}

	return pInnerBox;
}

CComponent* CHalfSphere::Clone()
{
	CComponent* pComp = new CHalfSphere(*this);

	return pComp;
}

void CHalfSphere::Free()
{
	CVIBuffer::Free();
}
