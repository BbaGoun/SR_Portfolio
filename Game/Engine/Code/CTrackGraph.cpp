#include "CTrackGraph.h"
#include "CProtoMgr.h"
#include "CCalculator.h"

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
	TN.bStart = false;
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

void CTrackGraph::Set_PointPos(TrackEdge* _pTE, ControlPoint* _pCp, _vec3 newPos)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	if (pTE->deqControls.empty())
		return;

	if (&pTE->deqControls.front() == _pCp || &pTE->deqControls.back() == _pCp)
		return;

	ControlPoint* pCp = Get_ControlPoint(pTE, _pCp->id);

	pCp->position = newPos;
}

void CTrackGraph::Finalize_LoadedData()
{
	NodeId maxNodeId = 0;
	EdgeId maxEdgeId = 0;
	uint32_t maxControlId = 0;

	for (auto& node : m_vecNodes)
		maxNodeId = max(maxNodeId, node.id);

	for (auto& edge : m_vecEdges)
	{
		maxEdgeId = max(maxEdgeId, edge.id);
		edge.vecSamples.clear();

		for (auto& control : edge.deqControls)
			maxControlId = max(maxControlId, control.id);
	}

	m_uGenerateNodeId = maxNodeId + 1;
	m_uGenerateEdgeId = maxEdgeId + 1;
	m_uGenerateControlId = maxControlId + 1;
	m_bNodeEdit = false;
	m_EdgeEditId = 0;

	Compute_Graph();
}

void CTrackGraph::Compute_Graph()
{
	// 시작점/끝점 등록
	NodeId start = 0;
	NodeId finish = 0;
	for (auto& tn : m_vecNodes) {
		if (tn.bStart)
			start = tn.id;
		if(tn.bFinish)
			finish = tn.id;
	}
	if (start == 0 || finish == 0)
		return;

	// 인접 리스트 갱신
	// 전역 진행량, 진입 차수 초기화
	for (auto& tn : m_vecNodes) {
		tn.vecInEdgeIds.clear();
		tn.vecOutEdgeIds.clear();
		tn.s_Global = 0;
		tn.in_Degree = 0;
	}

	for (auto& te : m_vecEdges) {
		if (te.fromNode == 0 || te.toNode == 0)
			continue;
		if (TrackNode* pTN_From = Get_TrackNode(te.fromNode))
			pTN_From->vecOutEdgeIds.push_back(te.id);

		if (TrackNode* pTN_To = Get_TrackNode(te.toNode)) {
			pTN_To->vecInEdgeIds.push_back(te.id);
			pTN_To->in_Degree += 1;
		}
	}

	deque<TrackNode*> computeQueue;
	computeQueue.push_back(Get_TrackNode(start));

	m_fLapLength = 0.f;

	while (!computeQueue.empty()) {
		TrackNode* pTN = computeQueue.front();
		computeQueue.pop_front();

		// 들어오는 엣지의 end 설정
		for (auto& edgeId : pTN->vecInEdgeIds) {
			TrackEdge* pTE = Get_TrackEdge(edgeId);
			pTE->sEnd = pTN->s_Global;
		}

		for (auto& edgeId : pTN->vecOutEdgeIds) {
			TrackEdge* pTE = Get_TrackEdge(edgeId);
			pTE->sStart = pTN->s_Global;
			Compute_Edge(pTE);

			TrackNode* pTN_To = Get_TrackNode(pTE->toNode);
			// 다음 노드의 s_Global 갱신
			if (pTN_To->id != finish)
			{
				if (pTN_To->s_Global < pTE->sStart + pTE->fLength)
					pTN_To->s_Global = pTE->sStart + pTE->fLength;
			}
			else {
				if (m_fLapLength < pTE->sStart + pTE->fLength)
					m_fLapLength = pTE->sStart + pTE->fLength;
			}
			
			// 진입 차수가 0이면 큐에 등록
			pTN_To->in_Degree -= 1;
			if (pTN_To->in_Degree == 0 && !pTN_To->bFinish)
				computeQueue.push_back(pTN_To);
		}
	}

	TrackNode* pTN_Finish = Get_TrackNode(finish);
	// 도착 노드의 설정
	for (auto& edgeId : pTN_Finish->vecInEdgeIds) {
		TrackEdge* pTE = Get_TrackEdge(edgeId);
		pTE->sEnd = m_fLapLength;
	}

	for (auto& te : m_vecEdges) {
		// Edge 내의 샘플의 S를 계산
		for (auto& sample : te.vecSamples) {
			sample.s = Lerp(sample.u / te.fLength, te.sStart, te.sEnd);
		}
	}
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

void CTrackGraph::Render_Samples()
{
	for (auto& te : m_vecEdges) {
		if (te.fromNode == 0 || te.toNode == 0)
			continue;

		for (auto& s : te.vecSamples) {
			_vec3 vCorners[8];

			vCorners[0] = s.position + s.T * s.halfL - s.R * s.halfW + s.U * s.halfH;
			vCorners[1] = s.position + s.T * s.halfL + s.R * s.halfW + s.U * s.halfH;
			vCorners[2] = s.position + s.T * s.halfL + s.R * s.halfW - s.U * s.halfH;
			vCorners[3] = s.position + s.T * s.halfL - s.R * s.halfW - s.U * s.halfH;
						  
			vCorners[4] = s.position - s.T * s.halfL - s.R * s.halfW + s.U * s.halfH;
			vCorners[5] = s.position - s.T * s.halfL + s.R * s.halfW + s.U * s.halfH;
			vCorners[6] = s.position - s.T * s.halfL + s.R * s.halfW - s.U * s.halfH;
			vCorners[7] = s.position - s.T * s.halfL - s.R * s.halfW - s.U * s.halfH;

			D3DXCOLOR color = { 0, 1, 0, 1 };

			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[0], vCorners[1], color);
			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[1], vCorners[2], color);
			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[2], vCorners[3], color);
			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[3], vCorners[0], color);
															   
			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[4], vCorners[5], color);
			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[5], vCorners[6], color);
			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[6], vCorners[7], color);
			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[7], vCorners[4], color);
															   
			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[0], vCorners[4], color);
			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[1], vCorners[5], color);
			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[2], vCorners[6], color);
			CCalculator::DrawRayLine(m_pGraphicDev, vCorners[3], vCorners[7], color);
		}
	}
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

bool CTrackGraph::ProjectPosition(const _vec3& worldPos, const TrackLocator& prev, TrackLocator& outLocator, float* outLateral)
{
	_matrix matGraph, matInvGraph;
	matGraph = *m_pOwner->Get_Transform()->Get_World();
	D3DXMatrixInverse(&matInvGraph, 0, &matGraph);
	_vec3 localPos;
	D3DXVec3TransformCoord(&localPos, &worldPos, &matInvGraph);

	if (prev.bValid && prev.edgeId != 0) {
		//근처 계산
		const int range = 5;

		TrackEdge* pTE = Get_TrackEdge(prev.edgeId);
		int sIndex = prev.iSampleIndex;
		
		CheckInfo forwardInfo = Recursive_Forward_CheckInside(localPos, prev, range, pTE, sIndex);
		CheckInfo backInfo = Recursive_Back_CheckInside(localPos, prev, range, pTE, sIndex);

		if (forwardInfo.bestScore > backInfo.bestScore)
			forwardInfo = backInfo;

		if (forwardInfo.bFound) {
			outLocator = forwardInfo.bestLocater;
			if (outLateral)
				*outLateral = forwardInfo.bestLateral;
			return true;
		}
	}
	//전체 계산
	float bestScore = FLT_MAX;
	bool bFound = false;
	float bestLateral;
	TrackLocator bestLocater;

	for (auto& te : m_vecEdges) {
		int n = te.vecSamples.size();
		if (n < 2)
			continue;

		for (int i = 0; i < n; ++i) {
			TrackSample& s = te.vecSamples[i];

			_vec3 delta = localPos - s.position;
			float forward = D3DXVec3Dot(&delta, &s.T);
			
			if (fabsf(forward) > s.halfL)
				continue;
			if (i == 0 && forward < 0)
				continue;
			if (i == n - 1 && forward >= 0)
				continue;

			int iBegin, iEnd;

			if (forward >= 0)
				iBegin = i, iEnd = i + 1;
			else
				iBegin = i - 1, iEnd = i;

			TrackSample& a = te.vecSamples[iBegin];
			TrackSample& b = te.vecSamples[iEnd];

			_vec3 segment = b.position - a.position;
			_vec3 fromA = localPos - a.position;

			float lengthSg = D3DXVec3LengthSq(&segment);
			float t = 0.f;

			if (lengthSg > FLT_EPSILON)
				t = D3DXVec3Dot(&fromA, &segment) / lengthSg;

			t = clampT(t, 0.f, 1.f);

			_vec3 projected = a.position + segment * t;

			_vec3 R = a.R + (b.R - a.R) * t;
			_vec3 U = a.U + (b.U - a.U) * t;

			D3DXVec3Normalize(&R, &R);
			D3DXVec3Normalize(&U, &U);

			delta = localPos - projected;

			float lateral = D3DXVec3Dot(&delta, &R);
			float vertical = D3DXVec3Dot(&delta, &U);

			float halfW = Lerp(t, a.halfW, b.halfW);
			float halfH = Lerp(t, a.halfH, b.halfH);

			bool inside =
				fabsf(lateral) <= halfW &&
				fabsf(vertical) <= halfH;

			float LDW = (lateral / halfW);
			float VDH = (vertical / halfH);
			float score = LDW * LDW + VDH * VDH;

			if (inside && bestScore > score) {
				bestScore = score;
				bFound = true;
				bestLocater.localPos = localPos;
				bestLocater.edgeId = te.id;
				bestLocater.iSampleIndex = iBegin;
				bestLocater.u = Lerp(t, a.u, b.u);
				bestLocater.s = Lerp(t, a.s, b.s);
				bestLocater.iLap = prev.bValid ? prev.iLap : 0;
				bestLocater.bValid = true;
				bestLateral = lateral;
			}
		}
		if (bFound) {
			outLocator = bestLocater;
			if(outLateral)
				*outLateral = bestLateral;
			return true;
		}
	}

	outLocator = prev;
	outLocator.bValid = false;
	return false;
}

bool CTrackGraph::EvaluatePose(const TrackLocator& prev, float u, TrackPose& outPose)
{
	if (!prev.bValid)
		return false;
	
	TrackEdge* pTE = Get_TrackEdge(prev.edgeId);
	float localU = prev.u + u;
	
	while (localU > pTE->fLength) {
		localU -= pTE->fLength;
		TrackNode* pTN = Get_TrackNode(pTE->toNode);
		float maxBias = -FLT_MAX;
		for (EdgeId id : pTN->vecOutEdgeIds) {
			TrackEdge* pTE_To = Get_TrackEdge(id);
			if (maxBias < pTE_To->fCostBias) {
				maxBias = pTE_To->fCostBias;
				pTE = pTE_To;
			}
		}
	}

	int n = pTE->vecSamples.size();
	for (int i = 1; i < n; ++i) {
		TrackSample& b = pTE->vecSamples[i];
		TrackSample& a = pTE->vecSamples[i - 1];

		if (a.u < localU && localU <= b.u) {
			_vec3 segment = b.position - a.position;
			float segmentU = b.u - a.u;

			float deltaU = localU - a.u;

			float t = clampT(deltaU / segmentU, 0.f, 1.f);

			_vec3 R = a.R + (b.R - a.R) * t;
			_vec3 U = a.U + (b.U - a.U) * t;
			_vec3 T = a.T + (b.T - a.T) * t;

			D3DXVec3Normalize(&R, &R);
			D3DXVec3Normalize(&U, &U);
			D3DXVec3Normalize(&T, &T);

			_vec3 localPos = a.position + segment * t;
			_vec3 worldPos;
			D3DXVec3TransformCoord(&worldPos, &localPos, m_pOwner->Get_Transform()->Get_World());

			float s_global = Lerp(t, a.s, b.s);
			float speed = Lerp(t, a.speed, b.speed);
			float halfW = Lerp(t, a.halfW, b.halfW);

			outPose.position = worldPos;
			outPose.R = R;
			outPose.U = U;
			outPose.T = T;
			outPose.edgeId = pTE->id;
			outPose.u = localU;
			outPose.s = s_global;
			outPose.bValid = true;
			outPose.halfW = halfW;
			outPose.speed = speed;
			return true;
		}
	}
	
	return false;
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
	Compute_Sample_Speed(_pTE);
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

void CTrackGraph::Compute_Sample(TrackEdge* _pTE)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	if (pTE->fromNode == 0 || pTE->toNode == 0)
		return;

	int n = pTE->deqControls.size();

	vector<ArcLengthEntry> lut;
	Make_LUT(pTE, lut);
	pTE->fLength = lut.back().length;

	pTE->vecSamples.clear();

	_vec3 cur;
	ControlPoint start, end;
	_vec3 A, D, vA, vD;
	float bank, width, depth, rad;
	_vec3 T, R, U;
	_vec3 worldUp = { 0, 1, 0 };
	_matrix matRotBank;

	TrackSample TS;

	// 시작점
	start = pTE->deqControls.front();
	cur = start.position;
	width = start.width; depth = start.depth;
	T = start.T, R = start.R; U = start.U;

	TS.position = cur;
	TS.T = T, TS.R = R, TS.U = U;
	TS.u = 0.f;
	TS.halfW = width * 0.5f;
	TS.halfH = depth * 0.5f;
	// 샘플 유닛보다 조금 더 크게
	TS.halfL = m_fSampleUnit * 0.55f;

	pTE->vecSamples.push_back(TS);

	for (float target = m_fSampleUnit; target < pTE->fLength; target += m_fSampleUnit) {
		float globalT = Find_GlobalT(lut, target);

		int segment = int(floorf(globalT));
		float localT = globalT - segment;

		start = pTE->deqControls[segment];
		end = pTE->deqControls[segment + 1];
		A = start.position; D = end.position;
		vA = start.V; vD = end.V;

		cur = Cubic_Hermite_Curve(localT, A, D, vA, vD);

		T = Cubic_Hermite_Curve_Derivative(localT, A, D, vA, vD);
		D3DXVec3Normalize(&T, &T);

		D3DXVec3Cross(&R, &worldUp, &T);
		D3DXVec3Normalize(&R, &R);

		D3DXVec3Cross(&U, &T, &R);
		D3DXVec3Normalize(&U, &U);

		bank = Lerp(localT, start.bank, end.bank);
		width = Lerp(localT, start.width, end.width);
		depth = Lerp(localT, start.depth, end.depth);

		rad = D3DXToRadian(bank);
		D3DXMatrixRotationAxis(&matRotBank, &T, rad);

		D3DXVec3TransformNormal(&R, &R, &matRotBank);
		D3DXVec3TransformNormal(&U, &U, &matRotBank);

		TS.position = cur;
		TS.T = T, TS.R = R, TS.U = U;

		TS.u = target;
		TS.halfW = width * 0.5f;
		TS.halfH = depth * 0.5f;
		TS.halfL = m_fSampleUnit * 0.55f;
		pTE->vecSamples.push_back(TS);
	}

	// 끝점
	start = pTE->deqControls.back();
	cur = start.position;
	width = start.width; depth = start.depth;
	T = start.T, R = start.R; U = start.U;

	TS.position = cur;
	TS.T = T, TS.R = R, TS.U = U;
	TS.u = pTE->fLength;
	TS.halfW = width * 0.5f;
	TS.halfH = depth * 0.5f;
	// 샘플 유닛보다 조금 더 크게
	TS.halfL = fmodf(pTE->fLength, m_fSampleUnit) * 0.55f;
	pTE->vecSamples.push_back(TS);
}

void CTrackGraph::Make_LUT(TrackEdge* _pTE, vector<ArcLengthEntry>& lut)
{
	int n = _pTE->deqControls.size();

	constexpr int SUBDIVISIONS = 1000;
	float totalLength = 0.f;

	lut.push_back({ 0.f, 0.f });

	for (int segment = 0; segment < n - 1; ++segment)
	{
		const ControlPoint& start = _pTE->deqControls[segment];
		const ControlPoint& end = _pTE->deqControls[segment + 1];

		_vec3 previous = Cubic_Hermite_Curve(
			0.f,
			start.position,
			end.position,
			start.V,
			end.V);

		for (int j = 1; j <= SUBDIVISIONS; ++j)
		{
			float localT = static_cast<float>(j) / SUBDIVISIONS;

			_vec3 current = Cubic_Hermite_Curve(
				localT,
				start.position,
				end.position,
				start.V,
				end.V);

			_vec3 delta = current - previous;
			totalLength += D3DXVec3Length(&delta);

			lut.push_back({
				static_cast<float>(segment) + localT,
				totalLength
				});

			previous = current;
		}
	}
}

float CTrackGraph::Find_GlobalT(const vector<ArcLengthEntry>& lut, float targetLength)
{
	if (targetLength <= 0.f)
		return lut.front().globalT;

	if (targetLength >= lut.back().length)
		return lut.back().globalT;

	auto upper = std::lower_bound(
		lut.begin(),
		lut.end(),
		targetLength,
		[](const ArcLengthEntry& entry, float value)
		{
			return entry.length < value;
		});

	auto lower = upper - 1;

	float lengthRange = upper->length - lower->length;

	float alpha = (targetLength - lower->length) / lengthRange;

	return Lerp(alpha, lower->globalT, upper->globalT);
}

void CTrackGraph::Compute_Sample_Speed(TrackEdge* _pTE)
{
	TrackEdge* pTE = Find_TrackEdge(_pTE);
	if (!pTE)
		return;

	float fMax = 120.f, fMin = 40.f; // 평지 상하한
	
	// aLat = 원하는 코너 속력² / 코너 반경
	float aLat = 75.f;
	
	// aBrake = (현재속력² - 목표속력²) / (2 × 감속거리)
	float aBrake = 375.f; 
	
	// aAccel = (목표속력² - 현재속력²) / (2 × 증가거리)
	float aAccel = 50.f; 
	float kUp = 0.7f; // 오르막에서 목표 속력을 깎는 정도

	auto& vecS = pTE->vecSamples;
	int n = vecS.size();

	vector<float> fLocal(n);

	// 각 샘플의 로컬 제한 속력 계산
	for (int i = 0; i < n; ++i) {
		float kappa = 0;
		if (i != 0) {
			_vec3 beforeXZ, XZ;

			beforeXZ = vecS[i - 1].T;
			beforeXZ.y = 0;
			D3DXVec3Normalize(&beforeXZ, &beforeXZ);

			XZ = vecS[i].T;
			XZ.y = 0;
			D3DXVec3Normalize(&XZ, &XZ);

			float du = vecS[i].u - vecS[i - 1].u;

			kappa = acosf(D3DXVec3Dot(&beforeXZ, &XZ)) / du;
		}

		float fTurn = sqrtf(aLat / max(kappa, 0.001));
		fTurn = min(fTurn, fMax);

		float gradient = vecS[i].T.y;
		// 음수면 가속, 양수면 감속
		fTurn *= (1 - kUp * gradient);

		fLocal[i] = clampT(fTurn, fMin, fMax);
	}

	vecS[n - 1].speed = fLocal[n - 1];

	// 다음 속력을 고려하여 이전에 브레이크 전파
	for (int i = n - 2; i >= 0; --i) {
		float du = vecS[i + 1].u - vecS[i].u;
		float nextSpeed = vecS[i + 1].speed;
		float fFromNext = sqrtf(
			nextSpeed * nextSpeed + 2 * aBrake * du);

		vecS[i].speed = min(fLocal[i], fFromNext);
	}

	// 코너 출구에서 값이 확 바뀌지 않고 점진적으로 바뀌도록
	for (int i = 0; i < n - 1; ++i) {
		float du = vecS[i + 1].u - vecS[i].u;
		float reachable = sqrtf(
			vecS[i].speed * vecS[i].speed + 2.f * aAccel * du);

		vecS[i + 1].speed = min(vecS[i + 1].speed, reachable);
	}
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

CheckInfo CTrackGraph::Recursive_Forward_CheckInside(const _vec3& localPos, const TrackLocator& prev, int remainStep, TrackEdge* _pTE, int _iSampleIndex)
{
	CheckInfo info;

	auto& vecS = _pTE->vecSamples;
	int n = vecS.size();
	int sIndex = _iSampleIndex;

	TrackSample& s = vecS[sIndex];

	_vec3 delta = localPos - s.position;
	float forward = D3DXVec3Dot(&delta, &s.T);

	bool hasSegment = !(sIndex == 0 && forward < 0.f) &&
		!(sIndex == n - 1 && forward >= 0.f);

	if (fabsf(forward) < s.halfL) {
		if(hasSegment) 
		{
			int iBegin, iEnd;

			if (forward >= 0)
				iBegin = sIndex, iEnd = sIndex + 1;
			else
				iBegin = sIndex - 1, iEnd = sIndex;

			TrackSample& a = vecS[iBegin];
			TrackSample& b = vecS[iEnd];

			_vec3 segment = b.position - a.position;
			_vec3 fromA = localPos - a.position;

			float lengthSg = D3DXVec3LengthSq(&segment);
			float t = 0.f;

			if (lengthSg > FLT_EPSILON)
				t = D3DXVec3Dot(&fromA, &segment) / lengthSg;

			t = clampT(t, 0.f, 1.f);

			_vec3 projected = a.position + segment * t;

			_vec3 R = a.R + (b.R - a.R) * t;
			_vec3 U = a.U + (b.U - a.U) * t;

			D3DXVec3Normalize(&R, &R);
			D3DXVec3Normalize(&U, &U);

			delta = localPos - projected;

			float lateral = D3DXVec3Dot(&delta, &R);
			float vertical = D3DXVec3Dot(&delta, &U);

			float halfW = Lerp(t, a.halfW, b.halfW);
			float halfH = Lerp(t, a.halfH, b.halfH);

			bool inside =
				fabsf(lateral) <= halfW &&
				fabsf(vertical) <= halfH;

			float LDW = (lateral / halfW);
			float VDH = (vertical / halfH);
			float score = LDW * LDW + VDH * VDH;

			if (inside && info.bestScore > score) {
				info.bestScore = score;
				info.bFound = true;
				info.bestLocater.localPos = localPos;
				info.bestLocater.edgeId = _pTE->id;
				info.bestLocater.iSampleIndex = iBegin;
				info.bestLocater.u = Lerp(t, a.u, b.u);
				info.bestLocater.s = Lerp(t, a.s, b.s);
				info.bestLocater.iLap = prev.iLap;

				delta = localPos - prev.localPos;
				forward = D3DXVec3Dot(&delta, &s.T);
				if (prev.s > info.bestLocater.s && forward > 0)
					info.bestLocater.iLap += 1;
				else if (prev.s < info.bestLocater.s && forward < 0)
					info.bestLocater.iLap -= 1;

				info.bestLocater.bValid = true;
				info.bestLateral = lateral;
			}
		}
	}

	if (remainStep > 0) {
		bool bNextEdge = _iSampleIndex == n - 1;

		if (!bNextEdge) {
			// 이 엣지 내
			CheckInfo nextInfo = Recursive_Forward_CheckInside(localPos, prev, remainStep - 1, _pTE, sIndex + 1);
			if (nextInfo.bFound) {
				if (info.bestScore > nextInfo.bestScore) {
					info = nextInfo;
				}
			}
		}
		else {
			// 엣지 간의 이동
			TrackNode* pTN_To = Get_TrackNode(_pTE->toNode);

			for (EdgeId id : pTN_To->vecOutEdgeIds) {
				TrackEdge* pTE_To = Get_TrackEdge(id);

				CheckInfo nextInfo = Recursive_Forward_CheckInside(localPos, prev, remainStep - 1, pTE_To, 0);
				if (nextInfo.bFound) {
					if (info.bestScore > nextInfo.bestScore) {
						info = nextInfo;
					}
				}
			}
		}
	}
	return info;
}

CheckInfo CTrackGraph::Recursive_Back_CheckInside(const _vec3& localPos, const TrackLocator& prev, int remainStep, TrackEdge* _pTE, int _iSampleIndex)
{
	CheckInfo info;

	auto& vecS = _pTE->vecSamples;
	int n = vecS.size();
	int sIndex = _iSampleIndex;

	TrackSample& s = vecS[sIndex];

	_vec3 delta = localPos - s.position;
	float forward = D3DXVec3Dot(&delta, &s.T);
	
	bool hasSegment = !(sIndex == 0 && forward < 0.f) &&
		!(sIndex == n - 1 && forward >= 0.f);

	if (fabsf(forward) < s.halfL) {
		if (hasSegment)
		{
			int iBegin, iEnd;

			if (forward >= 0)
				iBegin = sIndex, iEnd = sIndex + 1;
			else
				iBegin = sIndex - 1, iEnd = sIndex;

			TrackSample& a = vecS[iBegin];
			TrackSample& b = vecS[iEnd];

			_vec3 segment = b.position - a.position;
			_vec3 fromA = localPos - a.position;

			float lengthSg = D3DXVec3LengthSq(&segment);
			float t = 0.f;

			if (lengthSg > FLT_EPSILON)
				t = D3DXVec3Dot(&fromA, &segment) / lengthSg;

			t = clampT(t, 0.f, 1.f);

			_vec3 projected = a.position + segment * t;

			_vec3 R = a.R + (b.R - a.R) * t;
			_vec3 U = a.U + (b.U - a.U) * t;

			D3DXVec3Normalize(&R, &R);
			D3DXVec3Normalize(&U, &U);

			delta = localPos - projected;

			float lateral = D3DXVec3Dot(&delta, &R);
			float vertical = D3DXVec3Dot(&delta, &U);

			float halfW = Lerp(t, a.halfW, b.halfW);
			float halfH = Lerp(t, a.halfH, b.halfH);

			bool inside =
				fabsf(lateral) <= halfW &&
				fabsf(vertical) <= halfH;

			float LDW = (lateral / halfW);
			float VDH = (vertical / halfH);
			float score = LDW * LDW + VDH * VDH;

			if (inside && info.bestScore > score) {
				info.bestScore = score;
				info.bFound = true;
				info.bestLocater.localPos = localPos;
				info.bestLocater.edgeId = _pTE->id;
				info.bestLocater.iSampleIndex = iBegin;
				info.bestLocater.u = Lerp(t, a.u, b.u);
				info.bestLocater.s = Lerp(t, a.s, b.s);
				info.bestLocater.iLap = prev.iLap;

				delta = localPos - prev.localPos;
				forward = D3DXVec3Dot(&delta, &s.T);
				if (prev.s > info.bestLocater.s && forward > 0)
					info.bestLocater.iLap += 1;
				else if (prev.s < info.bestLocater.s && forward < 0)
					info.bestLocater.iLap -= 1;

				info.bestLocater.bValid = true;
				info.bestLateral = lateral;
			}
		}
	}

	if (remainStep > 0) {
		bool bNextEdge = _iSampleIndex == 0;

		if (!bNextEdge) {
			// 이 엣지 내
			CheckInfo nextInfo = Recursive_Back_CheckInside(localPos, prev, remainStep - 1, _pTE, sIndex - 1);
			if (nextInfo.bFound) {
				if (info.bestScore > nextInfo.bestScore) {
					info = nextInfo;
				}
			}
		}
		else {
			// 엣지 간의 이동
			TrackNode* pTN_From = Get_TrackNode(_pTE->fromNode);

			for (EdgeId id : pTN_From->vecInEdgeIds) {
				TrackEdge* pTE_From = Get_TrackEdge(id);

				int m = pTE_From->vecSamples.size();
				CheckInfo nextInfo = Recursive_Back_CheckInside(localPos, prev, remainStep - 1, pTE_From, m - 1);
				if (nextInfo.bFound) {
					if (info.bestScore > nextInfo.bestScore) {
						info = nextInfo;
					}
				}
			}
		}
	}
	return info;
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