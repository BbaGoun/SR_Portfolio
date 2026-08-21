#include "CSpline.h"
#include "CProtoMgr.h"

CSpline::CSpline(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
	m_eKind = CK_MESH;
}

CSpline::CSpline(const CSpline& rhs)
	:CVIBuffer(rhs)
{
	m_eKind = CK_MESH;
}

CSpline::~CSpline()
{
}


HRESULT CSpline::Ready_CSplineCom()
{
	m_vecControlPoint.reserve(100);
	m_vecControlPoint.push_back({
		{-2.5, 0, 0}, GenerateId(),
		0, 10, 10
		});
	m_vecControlPoint.push_back({
		{2.5, 0, 0}, GenerateId(),
		0, 10, 10
		});

	m_pTexNormal = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SplinePointNormal"));
	m_pTexEdit = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SplinePointEdit"));
	
	return S_OK;
}

HRESULT CSpline::Ready_Buffer()
{
	return S_OK;
}

void CSpline::Render_Buffer()
{
	if (m_dwVtxCnt < 3)
		return;
	CVIBuffer::Render_Buffer();
}

void CSpline::Add_Point()
{
	_vec3 gap = (m_vecControlPoint.end() - 1)->position - (m_vecControlPoint.end() - 2)->position;
	ControlPoint cp;
	cp.position = (m_vecControlPoint.end() - 1)->position + gap;
	cp.id = GenerateId();
	cp.bank = (m_vecControlPoint.end() - 1)->bank;
	cp.width = (m_vecControlPoint.end() - 1)->width;
	cp.depth = (m_vecControlPoint.end() - 1)->depth;

	m_vecControlPoint.push_back(cp);
}

void CSpline::Del_Point(ControlPoint* pCp)
{
	if (m_vecControlPoint.size() <= 2)
		return;

	m_vecControlPoint.erase(
		remove_if(m_vecControlPoint.begin(), m_vecControlPoint.end(), [&](ControlPoint cp)->bool {
			return cp.id == pCp->id;
			})
		, m_vecControlPoint.end());
}

void CSpline::Render_Points()
{
	PreRender_Points();

	if (m_bEdit)
		m_pTexEdit->Set_Texture(0);
	else
		m_pTexNormal->Set_Texture(0);

	LPDIRECT3DVERTEXBUFFER9 _pVB;
	m_pGraphicDev->CreateVertexBuffer(
		m_vecControlPoint.size() * sizeof(VTXTEX),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		FVF_TEX,
		D3DPOOL_DEFAULT,
		&_pVB,
		0);

	VTXTEX* vertices = nullptr;
	_pVB->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

	for (int i = 0; i < m_vecControlPoint.size(); ++i) {
		vertices[i].vPosition = m_vecControlPoint[i].position;
	}

	_pVB->Unlock();

	m_pGraphicDev->SetStreamSource(0, _pVB, 0, sizeof(VTXTEX));
	m_pGraphicDev->SetFVF(FVF_TEX);
	_matrix* matWorld = m_pOwner->Get_Transform()->Get_World();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);
	m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, 0, m_vecControlPoint.size());

	_pVB->Release();
	m_pGraphicDev->SetTexture(0, nullptr);

	PostRender_Points();
}

ControlPoint* CSpline::Get_ControlPoint(uint32_t cpId)
{
	auto it = find_if(m_vecControlPoint.begin(), m_vecControlPoint.end(), [&](ControlPoint& cp)->bool {
		return cp.id == cpId;
		});
	if (it != m_vecControlPoint.end())
		return &(*it);
	return nullptr;
}

_int CSpline::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

void CSpline::LateUpdate_Component()
{

}

CSpline* CSpline::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpline* pCom = new CSpline(pGraphicDev);

	if (FAILED(pCom->Ready_CSplineCom()))
	{
		Safe_Release(pCom);
		MSG_BOX("CSpline Create Failed");
		return nullptr;
	}

	return pCom;
}


void CSpline::Free()
{
	CVIBuffer::Free();
}

CComponent* CSpline::Clone()
{
	CSpline* pCom = new CSpline(*this);

	if (FAILED(pCom->Ready_CSplineCom()))
	{
		Safe_Release(pCom);
		MSG_BOX("CSpline Create Failed");
		return nullptr;
	}

	return pCom;
}

void CSpline::PreRender_Points()
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

void CSpline::PostRender_Points()
{
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}
