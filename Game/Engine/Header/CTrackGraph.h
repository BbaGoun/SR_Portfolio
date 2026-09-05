#pragma once
#include "CComponent.h"
#include "CTexture.h"

BEGIN(Engine)

class ENGINE_DLL CTrackGraph :
	public CComponent
{
protected:
	explicit CTrackGraph(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrackGraph(const CTrackGraph& rhs);
	virtual ~CTrackGraph();

public:
	virtual _int Update_Component(const _float& fTimeDelta) { return 0; }
	virtual void LateUpdate_Component() {}

public:
	HRESULT			Ready_CTrackGraphCom();

	void			Add_Node();
	void			Del_Node(TrackNode* _pTN);
	void			Set_NodePos(TrackNode* _pTN, _vec3 newPos);

	void			Add_Edge();
	void			Del_Edge(TrackEdge* _pTE);
	
	void			Set_EdgeFrom(TrackEdge* _pTE, TrackNode* _pTN);
	void			Set_EdgeTo(TrackEdge* _pTE, TrackNode* _pTN);

	void			Add_Point(TrackEdge* _pTE);
	void			Del_Point(TrackEdge* _pTE, ControlPoint* _pCp);
	void			Set_PointPos(TrackEdge* _pTE, ControlPoint* _pCp, _vec3 newPos);
	void			Insert_Point(TrackEdge* _pTE, ControlPoint* _pCp);

	void			Compute_Graph();
	void			Finalize_LoadedData();

	void			Set_Bank(TrackEdge* _pTE, ControlPoint* _pCp, float fBank);
	void			Set_BankByRight(TrackEdge* _pTE, ControlPoint* _pCp, _vec3 vRight);
	void			Set_WidthDepth(TrackEdge* _pTE, ControlPoint* _pCp, _vec3 vRight, _vec3 vUp);

	void			Render_Points();
	void			Render_Samples();

	TrackNode*		Get_TrackNode(NodeId id);
	TrackEdge*		Get_TrackEdge(EdgeId id);
	ControlPoint*	Get_ControlPoint(TrackEdge* _pTE, uint32_t cpId);

public:
	bool ProjectPosition(
		const _vec3& worldPos,
		const TrackLocator& prev,
		TrackLocator& outLocator,
		float* outLateral = nullptr);

	bool EvaluatePose(
		const TrackLocator& prev,
		float u,
		TrackPose& outPose);

	/*bool Advance(
		const TrackLocator& from,
		float distance,
		const RoutePlan& route,
		TrackPose& outPose);

	float GetRouteDistance(
		const TrackLocator& from,
		const TrackLocator& to,
		const RoutePlan& route);*/

private:
	TrackNode*		Find_TrackNode(TrackNode* pTN);
	TrackEdge*		Find_TrackEdge(TrackEdge* pTE);
	ControlPoint*	Find_ControlPoint(TrackEdge* _pTE, ControlPoint* _pCp);

	void			Compute_Edge(TrackEdge* _pTE);
	void			ComputeV(TrackEdge* _pTE);
	void			ComputeTRU(TrackEdge* _pTE);
	
	void			Compute_Sample(TrackEdge* _pTE);
	void			Make_LUT(TrackEdge* _pTE, vector<ArcLengthEntry>& lut);
	float			Find_GlobalT(const vector<ArcLengthEntry>& lut, float targetLength);
	
	void			Compute_Sample_Speed(TrackEdge* _pTE);

	void			PreRender_Points();
	void			PostRender_Points();

	CheckInfo		Recursive_Forward_CheckInside(
		const _vec3& localPos, const TrackLocator& prev, 
		int	remainStep, TrackEdge* _pTE, int _iSampleIndex);
	CheckInfo		Recursive_Back_CheckInside(
		const _vec3& localPos, const TrackLocator& prev, 
		int remainStep, TrackEdge* _pTE, int _iSampleIndex);

public:
	static			CTrackGraph* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			CComponent* Clone(); //PURE로 설정

public:
	vector<TrackNode>& Get_Nodes() { return m_vecNodes; }
	vector<TrackEdge>& Get_Edges() { return m_vecEdges; }

	void	Set_NodeEdit(bool _b) { 
		if (m_EdgeEditId != 0 && _b)
			return;
		m_bNodeEdit = _b;
	}
	bool	Get_NodeEdit() { return m_bNodeEdit; }
	
	void	Set_EdgeEdit(EdgeId id) { 
		if (m_bNodeEdit && id != 0)
			return;
		m_EdgeEditId = id;
	}
	EdgeId	Get_EdgeEdit() { return m_EdgeEditId; }

	void	Set_SampleUnit(float _fSampleUnit) { m_fSampleUnit = _fSampleUnit; }
	float	Get_SampleUnit() { return m_fSampleUnit; }

	float	Get_LapLength() { return m_fLapLength; }

	NodeId GenerateNodeId() { return m_uGenerateNodeId++; }
	EdgeId GenerateEdgeId() { return m_uGenerateEdgeId++; }
	uint32_t GenerateControlId() { return m_uGenerateControlId++; }

protected:
	NodeId m_uGenerateNodeId = 1;
	EdgeId m_uGenerateEdgeId = 1;
	uint32_t m_uGenerateControlId = 1;
	CTexture* m_pTexNormal;
	CTexture* m_pTexEdit;

	vector<TrackNode>		m_vecNodes;
	vector<TrackEdge>		m_vecEdges;

	bool	m_bNodeEdit = false;
	EdgeId	m_EdgeEditId = 0;
	float	m_fSampleUnit = 1.f;
	float	m_fLapLength;

protected:
	virtual		void		Free() override;
};

END
