#include "CSphere.h"

CSphere::CSphere(LPDIRECT3DDEVICE9 pGraphicDev) : CVIBuffer(pGraphicDev)
{
}

CSphere::CSphere(const CSphere& rhs) : CVIBuffer(rhs)
{
}

CSphere::~CSphere()
{
}

HRESULT CSphere::Ready_Buffer()
{
	float radius = 0.5f;
	int slice = 16; // 경도를 나누는 갯수 (xy 차원)
	int stack = 16; // 위도를 나누는 갯수 (z+에서 z-까지 내리는)

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = (slice + 1) * (stack + 1);
	m_dwTriCnt = slice * (stack - 2) * 2 + slice * 2;
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
		float radianA = D3DXToRadian(180 * i / stack);
		// UV 값이 확 바뀌지 않도록 한바퀴 돈 지점에 하나 더 생성
		for (int j = 0; j <= slice; ++j) {
			// 경도
			float radianB = D3DXToRadian(360 * j / slice);
			int index = i * (slice + 1) + j;
			_vec3 pos = radius * _vec3{
				sinf(radianA) * cosf(radianB),
				sinf(radianA) * sinf(radianB),
				cosf(radianA)
			};
			
			vertices[index].vPosition = pos;
			vertices[index].vTexUV = {
				float(j)/float(slice),
				float(i)/float(stack)
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

	for (int i = 0; i <= stack - 1; ++i) {
		for (int j = 0; j < slice; ++j) {
			if (0 < i && i < stack - 1) {
				int index = (i - 1) * (slice * 2) + slice + j * 2;
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
			else if (i == stack - 1) {
				int index = (i - 1) * (slice * 2) + slice + j;
				indices[index]._0 = i * (slice + 1) + j;
				indices[index]._1 = (i + 1) * (slice + 1) + j;
				indices[index]._2 = i * (slice + 1) + (j + 1);
				m_vecFaces[index].indices = {
					indices[index]._0,
					indices[index]._1,
					indices[index]._2
				};
			}
			else {
				indices[j]._0 = j;
				indices[j]._1 = (slice+1) + j;
				indices[j]._2 = (slice+1) + (j + 1);
				m_vecFaces[j].indices = {
					indices[j]._0,
					indices[j]._1,
					indices[j]._2
				};
			}
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

CSphere* CSphere::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphere* pInnerBox = new CSphere(pGraphicDev);

	if (FAILED(pInnerBox->Ready_Buffer())) {
		Safe_Release(pInnerBox);
		MSG_BOX("pInnerBox Create Fail");
		return nullptr;
	}

	return pInnerBox;
}

CComponent* CSphere::Clone()
{
	CComponent* pComp = new CSphere(*this);

	return pComp;
}

void CSphere::Free()
{
	CVIBuffer::Free();
}
