#include "CTrackGraph.h"
#include "CProtoMgr.h"

CTrackGraph::CTrackGraph(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
	, m_pTexNormal(nullptr)
	, m_pTexEdit(nullptr)
{
	m_eKind = CK_TRACKGRAPH;
}

CTrackGraph::CTrackGraph(const CTrackGraph& rhs)
	:CComponent(rhs)
	, m_uGenerateNodeId(rhs.m_uGenerateNodeId)
	, m_uGenerateEdgeId(rhs.m_uGenerateEdgeId)
	, m_vecNodes(rhs.m_vecNodes)
	, m_vecEdges(rhs.m_vecEdges)
	, m_pTexNormal(rhs.m_pTexNormal)
	, m_pTexEdit(rhs.m_pTexEdit)
{
	m_eKind = CK_TRACKGRAPH;

	if (m_pTexNormal)
		m_pTexNormal->AddRef();
	if (m_pTexEdit)
		m_pTexEdit->AddRef();
}

CTrackGraph::~CTrackGraph()
{
}

HRESULT CTrackGraph::Ready_CTrackGraphCom()
{
	m_vecNodes.reserve(10);
	m_vecEdges.reserve(10);

	m_pTexNormal = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SplinePointNormal"));
	m_pTexEdit = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SplinePointEdit"));

	return S_OK;
}

void CTrackGraph::Add_Node()
{
	TrackNode TN;
	TN.id = GenerateNodeId();
	TN.position = { 0, 0, 0 };
	TN.s_Global = 0.f;
	TN.bFinish = false;
	m_vecNodes.push_back(TN);
}

void CTrackGraph::Del_Node(TrackNode* _pTN)
{
	TrackNode* pTN = Find_TrackNode(_pTN);
	if (!pTN)
		return;

	// 해당 노드와 연결된 엣지 삭제
	m_vecEdges.erase(remove_if(m_vecEdges.begin(), m_vecEdges.end(),
		[&](TrackEdge& TE)->bool {
			return TE.fromNode == pTN->id || TE.toNode == pTN->id;
		}), m_vecEdges.end());

	// 해당 노드 삭제
	m_vecNodes.erase(remove_if(m_vecNodes.begin(), m_vecNodes.end(),
		[&](TrackNode& TN)->bool {
			return TN.id == pTN->id;
		}), m_vecNodes.end());
}

void CTrackGraph::Set_NodePos(TrackNode* _pTN, _vec3 newPos)
{
	TrackNode* pTN = Find_TrackNode(_pTN);
	if (!pTN)
		return;

	pTN->position = newPos;
	
	for (auto& TE : m_vecEdges) {
		if (TE.fromNode == pTN->id) {
			TE.deqControls.front().position = newPos;
		}
		else if (TE.toNode == pTN->id) {
			TE.deqControls.back().position = newPos;
		}
	}
}

void CTrackGraph::Add_Edge()
{
	TrackEdge TE;
	TE.id = GenerateEdgeId();
	TE.fromNode = 0, TE.toNode = 0;
	TE.sStart = 0, TE.sEnd = 0;
	TE.fLength = 0;
	TE.fWidthDefault = 10, TE.fHeightDefault = 10;
	TE.fCostBias = 0;
	m_vecEdges.push_back(TE);
}

void CTrackGraph::Del_Edge(TrackEdge* _pTE)
{
	m_vecEdges.erase(remove_if(m_vecEdges.begin(), m_vecEdges.end(),
		[&](TrackEdge& TE)->bool {
			return TE.id == _pTE->id;
		}), m_vecEdges.end());
}

void CTrackGraph::Set_EdgeFrom(TrackEdge* _pTE, TrackNode* _pTN)
{
	TrackNode* pTN = Find_TrackNode(_pTN);
	if (!pTN)
		return;

	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	if (pTE->fromNode == pTN->id)
		return;

	if (pTE->fromNode != 0)
		pTE->deqControls.pop_front();
	ControlPoint cp;
	cp.id = GenerateControlId();
	cp.position = pTN->position;
	cp.bank = 0;
	cp.width = pTE->fWidthDefault;
	cp.depth = pTE->fHeightDefault;
	pTE->deqControls.push_front(cp);

	pTE->fromNode = pTN->id;

	Compute_Graph();
}

void CTrackGraph::Set_EdgeTo(TrackEdge* _pTE, TrackNode* _pTN)
{
	TrackNode* pTN = Find_TrackNode(_pTN);
	if (!pTN)
		return;

	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	if (pTE->toNode == pTN->id)
		return;

	if (pTE->toNode != 0)
		pTE->deqControls.pop_back();
	ControlPoint cp;
	cp.id = GenerateControlId();
	cp.position = pTN->position;
	cp.bank = 0;
	cp.width = pTE->fWidthDefault;
	cp.depth = pTE->fHeightDefault;
	pTE->deqControls.push_back(cp);

	pTE->toNode = pTN->id;

	Compute_Graph();
}

void CTrackGraph::Add_Point(TrackEdge* _pTE)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	if (pTE->fromNode == 0 || pTE->toNode == 0)
		return;

	if (pTE->deqControls.size() < 3) {
		ControlPoint cp;
		cp.id = GenerateControlId();
		cp.position = { 1, 1, 1 };
		cp.bank = 0;
		cp.width = pTE->fWidthDefault;
		cp.depth = pTE->fHeightDefault;
		pTE->deqControls.insert(pTE->deqControls.end() - 1, cp);
	}
	else {
		ControlPoint cp;
		_vec3 posBeforeTo = (pTE->deqControls.end() - 2)->position;
		_vec3 pos2BeforeTo = (pTE->deqControls.end() - 3)->position;
		_vec3 gap = posBeforeTo - pos2BeforeTo;
		cp.id = GenerateControlId();
		cp.position = posBeforeTo + gap;
		cp.bank = 0;
		cp.width = pTE->fWidthDefault;
		cp.depth = pTE->fHeightDefault;
		pTE->deqControls.insert(pTE->deqControls.end()-1, cp);
	}

	Compute_Graph();
}

void CTrackGraph::Del_Point(TrackEdge* _pTE, ControlPoint* _pCp)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	if (_pCp == &(*pTE->deqControls.begin())
		|| _pCp == &(*(pTE->deqControls.end()-1)))
		return;

	pTE->deqControls.erase(
		remove_if(pTE->deqControls.begin(), pTE->deqControls.end(), [&](ControlPoint& cp)->bool {
			return cp.id == _pCp->id;
			})
		, pTE->deqControls.end());

	Compute_Graph();
}

void CTrackGraph::Compute_Edge(TrackEdge* _pTE)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	if (pTE->fromNode == 0 || pTE->toNode == 0)
		return;

	ComputeV(_pTE);
	ComputeTRU(_pTE);
	Compute_Sample(_pTE);
}

void CTrackGraph::Compute_Sample(TrackEdge* _pTE)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	if (pTE->fromNode == 0 || pTE->toNode == 0)
		return;

	TrackNode* pTN_From = Get_TrackNode(pTE->fromNode);
	TrackNode* pTN_To = Get_TrackNode(pTE->toNode); 

	pTE->vecSamples.clear();

	// 곡선마다 50등분을 하여 거리를 누적할 생각
	int n = pTE->deqControls.size();
	float splineSum = 0.f;
	float distSum = 0.f;
	float dist;
	_vec3 before, cur, dir;
	ControlPoint start, end;
	_vec3 A, D, vA, vD;
	float bank, width, depth, rad;
	_vec3 T, R, U;
	_vec3 worldUp = { 0, 1, 0 };
	_matrix matRotBank;
	TrackSample TS;
	_vec3 beforeSample;

	// 시작점
	start = pTE->deqControls.front();
	cur = start.position;
	width = start.width; depth = start.depth;
	T = start.T, R = start.R; U = start.U;
	
	TS.position = cur;
	TS.T = T, TS.R = R, TS.U = U;
	TS.u = 0.f;
	TS.s = pTN_From->s_Global;
	TS.halfW = width * 0.5f;
	TS.halfH = depth * 0.5f;
	// 샘플 유닛보다 조금 더 크게
	TS.halfL = (m_fSampleUnit * 0.55f);
	TS.curvature = 1.f;
	pTE->vecSamples.push_back(TS);

	before = cur;
	beforeSample = cur;
	float spline_t = 0.02f;

	while (spline_t < n - 1) {
		int index = floor(spline_t);
		float local_t = spline_t - index;

		start = pTE->deqControls[index];
		end = pTE->deqControls[index + 1];
		A = start.position; D = end.position;
		vA = start.V; vD = end.V;

		cur = Cubic_Hermite_Curve(local_t, A, D, vA, vD);
		dir = cur - before;
		dist = D3DXVec3Length(&dir);

		splineSum += dist;
		distSum += dist;

		// 샘플 생성
		if (distSum >= m_fSampleUnit) {
			T = Cubic_Hermite_Curve_Derivative(local_t, A, D, vA, vD);
			D3DXVec3Normalize(&T, &T);

			D3DXVec3Cross(&R, &worldUp, &T);
			D3DXVec3Normalize(&R, &R);

			D3DXVec3Cross(&U, &T, &R);
			D3DXVec3Normalize(&U, &U);

			bank = Lerp(local_t, start.bank, end.bank);
			width = Lerp(local_t, start.width, end.width);
			depth = Lerp(local_t, start.depth, end.depth);

			rad = D3DXToRadian(bank);
			D3DXMatrixRotationAxis(&matRotBank, &T, rad);

			D3DXVec3TransformNormal(&R, &R, &matRotBank);
			D3DXVec3TransformNormal(&U, &U, &matRotBank);

			TS.position = cur;
			TS.T = T, TS.R = R, TS.U = U;
			
			// s랑 u 설정 필요
			// TS.s = 
			// TS.u = 
			TS.halfW = width * 0.5f;
			TS.halfH = depth * 0.5f;
			// 샘플 유닛보다 조금 더 크게
			dir = cur - beforeSample;
			dist = D3DXVec3Length(&dir);
			TS.halfL = dist * 0.55f;
			// 오르막, 내리막, 커브에 따라 계산 필요
			TS.curvature = 1.f;
			pTE->vecSamples.push_back(TS);

			distSum -= m_fSampleUnit;
			beforeSample = cur;
		}
		before = cur;
		spline_t += 0.02f;
	}

	// 끝점
	start = pTE->deqControls.back();
	cur = start.position;
	width = start.width; depth = start.depth;
	T = start.T, R = start.R; U = start.U;

	TS.position = cur;
	TS.T = T, TS.R = R, TS.U = U;
	TS.u = 0.f;
	TS.s = pTN_From->s_Global;
	TS.halfW = width * 0.5f;
	TS.halfH = depth * 0.5f;
	// 샘플 유닛보다 조금 더 크게
	dir = cur - beforeSample;
	dist = D3DXVec3Length(&dir);
	TS.halfL = dist * 0.55f;
	TS.curvature = 1.f;
	pTE->vecSamples.push_back(TS);

	pTE->fLength = splineSum;
	// 이게 아니야 단순 엣지 하나의 계산으로 이루어질 수 없다
	// 노드와 노드 사이의 엣지들이 있고, 이를 그래프 탐색으로 누적해야
	// 하나의 그래프가 완성된다.
}

void CTrackGraph::Compute_Graph()
{

}

void CTrackGraph::Set_Bank(TrackEdge* _pTE, ControlPoint* _pCp, float fBank)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	ControlPoint* pCp = Find_ControlPoint(pTE, _pCp);
	if (!pCp)
		return;

	_vec3 T = pCp->T;
	_vec3 worldUp = { 0.f, 1.f, 0.f };
	_vec3 R0, U0;
	D3DXVec3Cross(&R0, &worldUp, &T);
	D3DXVec3Normalize(&R0, &R0);

	D3DXVec3Cross(&U0, &T, &R0);
	D3DXVec3Normalize(&U0, &U0);

	pCp->bank = fBank;
	float rad = D3DXToRadian(fBank);
	_matrix matRotBank;
	D3DXMatrixRotationAxis(&matRotBank, &pCp->T, rad);

	D3DXVec3TransformNormal(&pCp->R, &R0, &matRotBank);
	D3DXVec3TransformNormal(&pCp->U, &U0, &matRotBank);

	Compute_Graph();
}

void CTrackGraph::Set_BankByRight(TrackEdge* _pTE, ControlPoint* _pCp, _vec3 vRight)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	ControlPoint* pCp = Find_ControlPoint(pTE, _pCp);
	if (!pCp)
		return;

	_vec3 T = pCp->T;
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

	pCp->R = R;
	pCp->U = U;

	float bankRad = -atan2f(D3DXVec3Dot(&U, &R0), D3DXVec3Dot(&U, &U0));
	float bankDeg = D3DXToDegree(bankRad);

	pCp->bank = bankDeg;

	Compute_Graph();
}

void CTrackGraph::Set_WidthDepth(TrackEdge* _pTE, ControlPoint* _pCp, _vec3 vRight, _vec3 vUp)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	ControlPoint* pCp = Find_ControlPoint(pTE, _pCp);
	if (!pCp)
		return;

	pCp->width = D3DXVec3Length(&vRight);
	pCp->depth = D3DXVec3Length(&vUp);

	Compute_Graph();
}

void CTrackGraph::Render_Points()
{
	if (!m_bNodeEdit && m_EdgeEditId == 0)
		return;

	PreRender_Points();

	m_pTexEdit->Set_Texture(0);

	LPDIRECT3DVERTEXBUFFER9 _pVB;
	
	if (m_bNodeEdit) {
		if (m_vecNodes.empty())
			return;

		m_pGraphicDev->CreateVertexBuffer(
			m_vecNodes.size() * sizeof(VTXTEX),
			D3DUSAGE_POINTS,
			FVF_TEX,
			D3DPOOL_MANAGED,
			&_pVB,
			0);

		VTXTEX* vertices = nullptr;
		_pVB->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

		for (int i = 0; i < m_vecNodes.size(); ++i) {
			vertices[i].vPosition = m_vecNodes[i].position;
		}

		_pVB->Unlock();

		m_pGraphicDev->SetStreamSource(0, _pVB, 0, sizeof(VTXTEX));
		m_pGraphicDev->SetFVF(FVF_TEX);
		_matrix* matWorld = m_pOwner->Get_Transform()->Get_World();
		m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);
		m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, 0, m_vecNodes.size());

		Safe_Release(_pVB);
		m_pGraphicDev->SetTexture(0, nullptr);
	}
	else if (m_EdgeEditId != 0) {
		TrackEdge* pTE = Get_TrackEdge(m_EdgeEditId);
		if (!pTE)
			return;

		if (pTE->deqControls.empty())
			return;

		m_pGraphicDev->CreateVertexBuffer(
			pTE->deqControls.size() * sizeof(VTXTEX),
			D3DUSAGE_POINTS,
			FVF_TEX,
			D3DPOOL_MANAGED,
			&_pVB,
			0);

		VTXTEX* vertices = nullptr;
		_pVB->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

		for (int i = 0; i < pTE->deqControls.size(); ++i) {
			vertices[i].vPosition = pTE->deqControls[i].position;
		}

		_pVB->Unlock();

		m_pGraphicDev->SetStreamSource(0, _pVB, 0, sizeof(VTXTEX));
		m_pGraphicDev->SetFVF(FVF_TEX);
		_matrix* matWorld = m_pOwner->Get_Transform()->Get_World();
		m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);
		m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, 0, pTE->deqControls.size());

		Safe_Release(_pVB);
		m_pGraphicDev->SetTexture(0, nullptr);
	}


	PostRender_Points();
}

TrackNode* CTrackGraph::Get_TrackNode(NodeId id)
{
	auto it = find_if(m_vecNodes.begin(), m_vecNodes.end(), [&](TrackNode& TN)->bool {
		return TN.id == id;
		});

	if (it != m_vecNodes.end())
		return &(*it);
	return nullptr;
}

TrackEdge* CTrackGraph::Get_TrackEdge(EdgeId id)
{
	auto it = find_if(m_vecEdges.begin(), m_vecEdges.end(), [&](TrackEdge& TE)->bool {
		return TE.id == id;
		});

	if (it != m_vecEdges.end())
		return &(*it);
	return nullptr;
}

ControlPoint* CTrackGraph::Get_ControlPoint(TrackEdge* _pTE, uint32_t cpId)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return nullptr;

	auto it = find_if(pTE->deqControls.begin(), pTE->deqControls.end(), [&](ControlPoint& cp)->bool {
		return cp.id == cpId;
		});
	if (it != pTE->deqControls.end())
		return &(*it);
	return nullptr;
}

TrackNode* CTrackGraph::Find_TrackNode(TrackNode* pTN)
{
	auto itTN = find_if(m_vecNodes.begin(), m_vecNodes.end(), [&](TrackNode& TN)->bool {
		return TN.id == pTN->id;
		});
	
	if (itTN == m_vecNodes.end())
		return nullptr;
	else
		return &(*itTN);
}

TrackEdge* CTrackGraph::Find_TrackEdge(TrackEdge* pTE)
{
	auto itTE = find_if(m_vecEdges.begin(), m_vecEdges.end(), [&](TrackEdge& TE)->bool {
		return TE.id == pTE->id;
		});

	if (itTE == m_vecEdges.end())
		return nullptr;
	else
		return &(*itTE);
}

ControlPoint* CTrackGraph::Find_ControlPoint(TrackEdge* _pTE, ControlPoint* _pCp)
{
	auto itCp = find_if(_pTE->deqControls.begin(), _pTE->deqControls.end(), [&](ControlPoint& cp)->bool {
		return cp.id == _pCp->id;
		});

	if (itCp == _pTE->deqControls.end())
		return nullptr;
	else
		return &(*itCp);
}

void CTrackGraph::ComputeV(TrackEdge* _pTE)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	// 초기화
	for (auto& cp : pTE->deqControls)
		cp.V = { 0, 0, 0 };

	// 각 Control Point의 V를 계산
	for (auto it = pTE->deqControls.begin(); it != pTE->deqControls.end()-2; ++it) {
		_vec3 start, end, v;
		start = it->position, end = (it+2)->position;
		v = (end - start) * 0.5f;
		(it+1)->V = v;
	}
}

void CTrackGraph::ComputeTRU(TrackEdge* _pTE)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	// Editor에서 보여주기 위하여 각 Control Point의 T, R, U를 계산
	_vec3 A, D, vA, vD;
	_vec3 R0, U0;
	_vec3 dir;
	float epsilon = 0.001f;

	for (auto it = pTE->deqControls.begin(); it != pTE->deqControls.end()-1; ++it) {
		auto next = it+1;
		A = it->position, D = next->position;
		vA = it->V, vD = next->V;
		dir = Cubic_Hermite_Curve(epsilon, A, D, vA, vD) - A;
		D3DXVec3Normalize(&dir, &dir);
		it->T = dir;
		_vec3 worldUp = { 0, 1, 0 };

		D3DXVec3Cross(&R0, &worldUp, &it->T);
		D3DXVec3Normalize(&R0, &R0);

		D3DXVec3Cross(&U0, &it->T, &R0);
		D3DXVec3Normalize(&U0, &U0);

		float rad = D3DXToRadian(it->bank);
		_matrix matRotBank;
		D3DXMatrixRotationAxis(&matRotBank, &it->T, rad);

		D3DXVec3TransformNormal(&it->R, &R0, &matRotBank);
		D3DXVec3TransformNormal(&it->U, &U0, &matRotBank);
	}

	auto beforeLast = pTE->deqControls.end()-2;
	auto last = pTE->deqControls.end()-1;
	A = beforeLast->position, D = last->position;
	vA = beforeLast->V, vD = last->V;
	dir = Cubic_Hermite_Curve(1, A, D, vA, vD)
		- Cubic_Hermite_Curve(1 - epsilon, A, D, vA, vD);
	D3DXVec3Normalize(&dir, &dir);
	last->T = dir;
	_vec3 worldUp = { 0, 1, 0 };

	D3DXVec3Cross(&R0, &worldUp, &last->T);
	D3DXVec3Normalize(&R0, &R0);

	D3DXVec3Cross(&U0, &last->T, &R0);
	D3DXVec3Normalize(&U0, &U0);

	float rad = D3DXToRadian(last->bank);
	_matrix matRotBank;
	D3DXMatrixRotationAxis(&matRotBank, &last->T, rad);

	D3DXVec3TransformNormal(&last->R, &R0, &matRotBank);
	D3DXVec3TransformNormal(&last->U, &U0, &matRotBank);
}

void CTrackGraph::PreRender_Points()
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

void CTrackGraph::PostRender_Points()
{
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}


CTrackGraph* CTrackGraph::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrackGraph* pCom = new CTrackGraph(pGraphicDev);

	if (FAILED(pCom->Ready_CTrackGraphCom()))
	{
		Safe_Release(pCom);
		MSG_BOX("CTrackGraph Create Failed");
		return nullptr;
	}

	return pCom;
}

CComponent* CTrackGraph::Clone()
{
	CTrackGraph* pCom = new CTrackGraph(*this);

	return pCom;
}

void CTrackGraph::Free()
{
	Safe_Release(m_pTexNormal);
	Safe_Release(m_pTexEdit);
	CComponent::Free();
}