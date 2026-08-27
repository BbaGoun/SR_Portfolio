#include "CHeightMap.h"
#include "CProtoMgr.h"

CHeightMap::CHeightMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
	, m_pTexNormal(nullptr)
	, m_pTexEdit(nullptr)
{
}

CHeightMap::CHeightMap(const CHeightMap& rhs)
	:CVIBuffer(rhs)
	, m_pTexNormal(rhs.m_pTexNormal)
	, m_pTexEdit(rhs.m_pTexEdit)
{
	if(m_pTexNormal)
		m_pTexNormal->AddRef();
	if (m_pTexEdit)
		m_pTexEdit->AddRef();
}

CHeightMap::~CHeightMap()
{
}

HRESULT CHeightMap::Ready_CHeightMap()
{
	m_pTexNormal = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SplinePointNormal"));
	m_pTexEdit = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SplinePointEdit"));
	
	return S_OK;
}

HRESULT CHeightMap::Ready_Buffer()
{
	m_vecVertices.clear();
	m_vecFaces.clear();

	m_dwVtxCnt = m_vecVertices.size();
	if (m_dwVtxCnt < 3)
		return E_FAIL;

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxCnt = m_dwTriCnt * 3;
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* vertices = nullptr;
	m_vecVertices.resize(m_dwVtxCnt);

	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		vertices[i] = m_vecVertices[i];
	}

	for (int i = 0; i < m_dwVtxCnt; ++i) {
		UpdateMinMaxVtx(vertices[i].vPosition);
	}
	m_minVtx -= {1, 1, 1};
	m_maxVtx += {1, 1, 1};

	SetBoundingBox();

	m_pVB->Unlock();

	INDEX32* indices = nullptr;
	m_vecFaces.resize(m_dwTriCnt);

	m_pIB->Lock(0, 0, (void**)&indices, 0);


	for (int i = 0; i < m_dwTriCnt; ++i) {
		if (i % 2 == 0)
		{
			indices[i]._0 = i;
			indices[i]._1 = i + 2;
			indices[i]._2 = i + 1;
			m_vecFaces[i].indices = {
				indices[i]._0,
				indices[i]._1,
				indices[i]._2
			};
		}
		else {
			indices[i]._0 = i;
			indices[i]._1 = i + 1;
			indices[i]._2 = i + 2;
			m_vecFaces[i].indices = {
				indices[i]._0,
				indices[i]._1,
				indices[i]._2
			};
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

void CHeightMap::Render_Buffer()
{
	if (m_dwVtxCnt < 3)
		return;

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CVIBuffer::Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
}

void CHeightMap::Render_Points()
{
	PreRender_Points();

	if (m_bEdit)
		m_pTexEdit->Set_Texture(0);
	else
		m_pTexNormal->Set_Texture(0);

	m_minVtx = { FLT_MAX, FLT_MAX, FLT_MAX };
	m_maxVtx = { FLT_MIN, FLT_MIN, FLT_MIN };

	LPDIRECT3DVERTEXBUFFER9 _pVB;
	m_pGraphicDev->CreateVertexBuffer(
		m_vecVertices.size() * sizeof(VTXTEX),
		D3DUSAGE_POINTS,
		FVF_TEX,
		D3DPOOL_MANAGED,
		&_pVB,
		0);

	VTXTEX* vertices = nullptr;
	_pVB->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

	for (int i = 0; i < m_vecVertices.size(); ++i) {
		vertices[i].vPosition = m_vecVertices[i].vPosition;
	}

	_pVB->Unlock();

	m_pGraphicDev->SetStreamSource(0, _pVB, 0, sizeof(VTXTEX));
	m_pGraphicDev->SetFVF(FVF_TEX);
	_matrix* matWorld = m_pOwner->Get_Transform()->Get_World();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);
	m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, 0, m_vecVertices.size());

	_pVB->Release();
	m_pGraphicDev->SetTexture(0, nullptr);

	PostRender_Points();
}

void CHeightMap::PreRender_Points()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(16.f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(8.f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(32.f));

	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
}

void CHeightMap::PostRender_Points()
{
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}


CHeightMap* CHeightMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHeightMap* pCom = new CHeightMap(pGraphicDev);

	if (FAILED(pCom->Ready_CHeightMap()))
	{
		Safe_Release(pCom);
		MSG_BOX("CHeightMap Create Failed");
		return nullptr;
	}

	return pCom;
}

CComponent* CHeightMap::Clone()
{
	CHeightMap* pCom = new CHeightMap(*this);

	return pCom;
}


void CHeightMap::Free()
{
	Safe_Release(m_pTexNormal);
	Safe_Release(m_pTexEdit);
	CVIBuffer::Free();
}