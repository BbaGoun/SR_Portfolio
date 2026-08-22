#include "CSpline.h"
#include "CProtoMgr.h"

CSpline::CSpline(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
	m_eKind = CK_MESH;
}

CSpline::CSpline(const CSpline& rhs)
	:CVIBuffer(rhs)
	, m_uGenerateId(rhs.m_uGenerateId)
	, m_vecControlPoint(rhs.m_vecControlPoint)
	, m_pTexNormal(rhs.m_pTexNormal)
	, m_pTexEdit(rhs.m_pTexEdit)
{
	m_pTexNormal->AddRef();
	m_pTexEdit->AddRef();
	m_eKind = CK_MESH;
}

CSpline::~CSpline()
{
}


HRESULT CSpline::Ready_CSplineCom()
{
	m_vecControlPoint.reserve(100);
	ControlPoint cp;
	cp.position = { 0, 0 ,-2.5f };
	cp.id = GenerateId();
	cp.bank = 0;
	cp.width = 10;
	cp.depth = 10;
	cp.V = { 0, 0, 0 };

	m_vecControlPoint.push_back(cp);

	cp.position = { 0, 0, 2.5f };
	cp.id = GenerateId();
	m_vecControlPoint.push_back(cp);

	m_pTexNormal = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SplinePointNormal"));
	m_pTexEdit = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SplinePointEdit"));
	
	Compute_Spline();

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

	Compute_Spline();
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

	Compute_Spline();
}

void CSpline::Compute_Spline()
{
	ComputeV();
	ComputeTRU();
	Compute_Mesh();
}

void CSpline::ComputeV()
{
	// 초기화
	for (auto& cp : m_vecControlPoint)
		cp.V = { 0, 0, 0 };

	// 각 Control Point의 V를 계산
	for (auto it = m_vecControlPoint.begin(); it != m_vecControlPoint.end() - 2; ++it) {
		_vec3 start, end, v;
		start = it->position, end = (it + 2)->position;
		v = (end - start) * 0.5f;
		(it + 1)->V = v;
	}
}

void CSpline::ComputeTRU()
{
	// Editor에서 보여주기 위하여 각 Control Point의 T, R, U를 계산
	_vec3 A, D, vA, vD;
	_vec3 R0, U0;
	_vec3 dir;
	float epsilon = 0.001f;

	for (auto it = m_vecControlPoint.begin(); it != m_vecControlPoint.end() - 1; ++it) {
		auto next = it + 1;
		A = it->position, D = next->position;
		vA = it->V, vD = next->V;
		dir = Cubic_Hermite_Curve(epsilon, A, D, vA, vD)
			- A;
		D3DXVec3Normalize(&dir, &dir);
		it->T = dir;
		_vec3 worldUp = { 0, 1, 0 };

		D3DXVec3Cross(&R0, &worldUp, &it->T);
		D3DXVec3Normalize(&R0, &R0);

		D3DXVec3Cross(&U0, &it->T, &it->R);
		D3DXVec3Normalize(&U0, &U0);

		float rad = D3DXToRadian(it->bank);
		_matrix matRotBank;
		D3DXMatrixRotationAxis(&matRotBank, &it->T, rad);

		D3DXVec3TransformNormal(&it->R, &R0, &matRotBank);
		D3DXVec3TransformNormal(&it->U, &U0, &matRotBank);
	}

	auto beforeLast = m_vecControlPoint.end() - 2;
	auto last = m_vecControlPoint.end() - 1;
	A = beforeLast->position, D = last->position;
	vA = beforeLast->V, vD = last->V;
	dir = Cubic_Hermite_Curve(1, A, D, vA, vD)
		- Cubic_Hermite_Curve(1 - epsilon, A, D, vA, vD);
	D3DXVec3Normalize(&dir, &dir);
	last->T = dir;
	_vec3 worldUp = { 0, 1, 0 };

	D3DXVec3Cross(&R0, &worldUp, &last->T);
	D3DXVec3Normalize(&R0, &R0);

	D3DXVec3Cross(&U0, &last->T, &last->R);
	D3DXVec3Normalize(&U0, &U0);

	float rad = D3DXToRadian(last->bank);
	_matrix matRotBank;
	D3DXMatrixRotationAxis(&matRotBank, &last->T, rad);

	D3DXVec3TransformNormal(&last->R, &R0, &matRotBank);
	D3DXVec3TransformNormal(&last->U, &U0, &matRotBank);
}

void CSpline::Compute_Mesh()
{
}

void CSpline::Set_Bank(ControlPoint* pCp, float fBank)
{
	auto it = find_if(m_vecControlPoint.begin(), m_vecControlPoint.end(), [&](ControlPoint cp)->bool {
		return cp.id == pCp->id;
		});

	if (it == m_vecControlPoint.end())
		return;

	_vec3 T = it->T;
	_vec3 worldUp = { 0.f, 1.f, 0.f };
	_vec3 R0, U0;
	D3DXVec3Cross(&R0, &worldUp, &T);
	D3DXVec3Normalize(&R0, &R0);

	D3DXVec3Cross(&U0, &T, &R0);
	D3DXVec3Normalize(&U0, &U0);

	it->bank = fBank;
	float rad = D3DXToRadian(fBank);
	_matrix matRotBank;
	D3DXMatrixRotationAxis(&matRotBank, &it->T, rad);

	D3DXVec3TransformNormal(&it->R, &R0, &matRotBank);
	D3DXVec3TransformNormal(&it->U, &U0, &matRotBank);

	Compute_Mesh();
}

void CSpline::Set_BankByRight(ControlPoint* pCp, _vec3 vRight)
{
	auto it = find_if(m_vecControlPoint.begin(), m_vecControlPoint.end(), [&](ControlPoint cp)->bool {
		return cp.id == pCp->id;
		});

	if (it == m_vecControlPoint.end())
		return;

	_vec3 T = it->T;
	_vec3 worldUp = { 0.f, 1.f, 0.f };
	_vec3 R0, U0;
	D3DXVec3Cross(&R0, &worldUp, &T);
	D3DXVec3Normalize(&R0, &R0);

	D3DXVec3Cross(&U0, &T, &R0);
	D3DXVec3Normalize(&U0, &U0);

	_vec3 R = vRight;
	R = R - T * D3DXVec3Dot(&R, &T); // T에 수직인 성분만 남기기
	D3DXVec3Normalize(&R, &R);

	_vec3 U;
	D3DXVec3Cross(&U, &T, &R);
	D3DXVec3Normalize(&U, &U);

	it->R = R;
	it->U = U;

	float bankRad = atan2f(D3DXVec3Dot(&U, &R0), D3DXVec3Dot(&U, &U0));
	float bankDeg = D3DXToDegree(bankRad);

	it->bank = bankDeg;

	Compute_Mesh();
}

void CSpline::Set_WidthDepth(ControlPoint* pCp, _vec3 vRight, _vec3 vUp)
{
	auto it = find_if(m_vecControlPoint.begin(), m_vecControlPoint.end(), [&](ControlPoint cp)->bool {
		return cp.id == pCp->id;
		});

	if (it == m_vecControlPoint.end())
		return;

	it->width = D3DXVec3Length(&vRight);
	it->depth = D3DXVec3Length(&vUp);

	Compute_Mesh();
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
