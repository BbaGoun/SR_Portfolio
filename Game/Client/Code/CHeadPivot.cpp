#include "pch.h"
#include "CHeadPivot.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CCameraMgr.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include "CGOCody.h"
#include "CCalculator.h"
#include "CRenderer.h"

CHeadPivot::CHeadPivot(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CHeadPivot::CHeadPivot(const CGameObject& rhs) : CGameObject(rhs)
{
}

CHeadPivot::~CHeadPivot()
{
}

HRESULT CHeadPivot::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 0.f, 8.f, 1.5f });

	m_fPitch = 0;

	return S_OK;
}

_int CHeadPivot::Update_GameObject(const _float& fDeltaTime)
{
	Mouse_Input(fDeltaTime);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CHeadPivot::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
}

void CHeadPivot::Mouse_Input(const _float& fDeltaTime)
{
	_matrix* matWorld = m_pTransformCom->Get_World();

	_long	dwMouseMove(0);

	CAMERA_STATE camState = CCameraMgr::GetInstance()->GetCamerState();
	if (camState == CAMERA_FIRST || camState == CAMERA_BACK_THIRD || camState == CAMERA_FRONT_THIRD)
	{
		if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Y))
		{
			m_fPitch = clampT(m_fPitch + dwMouseMove / 10.f, -89.f, 89.f);
			_vec3	vRight = { 1, 0, 0 };
			D3DXQUATERNION q;
			D3DXQuaternionRotationAxis(&q, &vRight, D3DXToRadian(m_fPitch));

			m_pTransformCom->Set_Quaternion(&q);
		}
	}

	// 1인칭 카메라일때만 가능
	//if (camState == CAMERA_FIRST) {
	//	if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_LB)) {
	//		ClickTarget();
	//	}
	//}
}

void CHeadPivot::ClickTarget()
{
	CComponent* pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment", L"Env_Land2", L"Com_Buffer");
	if (pCom == nullptr)
		return;
	vector<VTXMESH> vertices = static_cast<CTerrain2*>(pCom)->GetVertices();
	vector<FACE32> faces = static_cast<CTerrain2*>(pCom)->GetFaces();

	// ray를 지형의 로컬 스페이스로 변환
	pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment", L"Env_Land2", L"Com_Transform");
	if (pCom == nullptr)
		return;

	_matrix* pMatWorld = static_cast<CTransform*>(pCom)->Get_World();
	_matrix matInvWorld;

	D3DXMatrixInverse(&matInvWorld, 0, pMatWorld);

	_vec3 rOrigin, rDir;
	m_pTransformCom->Get_Info(INFO_POS, &rOrigin);
	m_pTransformCom->Get_Info(INFO_LOOK, &rDir);

	D3DXVec3TransformCoord(&rOrigin, &rOrigin, &matInvWorld);
	D3DXVec3TransformNormal(&rDir, &rDir, &matInvWorld);

	_bool bFirst = true;
	_float fMinDist;
	_vec3 vMinPos;

	for (int i = 0; i < faces.size(); ++i) {
		VTXMESH v0, v1, v2;
		_vec3 p0, p1, p2;
		float u, v, dist;

		v0 = vertices[faces[i].indices._0];
		v1 = vertices[faces[i].indices._1];
		v2 = vertices[faces[i].indices._2];

		p0 = v0.vPosition;
		p1 = v1.vPosition;
		p2 = v2.vPosition;

		if (D3DXIntersectTri(&p0, &p1, &p2, &rOrigin, &rDir, &u, &v, &dist))
		{
			if (!bFirst)
			{
				if (fMinDist > dist) {
					fMinDist = dist;
					vMinPos = p0 + u * (p1 - p0) + v * (p2 - p0);
				}
			}
			else {
				bFirst = false;
				fMinDist = dist;
				vMinPos = p0 + u * (p1 - p0) + v * (p2 - p0);
			}
		}
	}

	if (!bFirst) {
		D3DXVec3TransformCoord(&vMinPos, &vMinPos, pMatWorld);
		static_cast<CGOCody*>(m_pParent)->Set_Target(vMinPos);
	}
}

void CHeadPivot::LayTest()
{
	CComponent* pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment", L"Env_Land2", L"Com_Buffer");
	if (pCom == nullptr)
		return;
	vector<VTXMESH> vertices = static_cast<CTerrain2*>(pCom)->GetVertices();
	vector<FACE32> faces = static_cast<CTerrain2*>(pCom)->GetFaces();

	// ray를 지형의 로컬 스페이스로 변환
	pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment", L"Env_Land2", L"Com_Transform");
	if (pCom == nullptr)
		return;

	_matrix* pMatWorld = static_cast<CTransform*>(pCom)->Get_World();
	_matrix matInvWorld;

	D3DXMatrixInverse(&matInvWorld, 0, pMatWorld);

	_vec3 rOrigin, rDir;
	_vec3 rOrigin2, rDir2;
	m_pTransformCom->Get_Info(INFO_POS, &rOrigin);
	m_pTransformCom->Get_Info(INFO_LOOK, &rDir);
	rOrigin2 = rOrigin;
	rDir2 = rDir;

	D3DXVec3TransformCoord(&rOrigin, &rOrigin, &matInvWorld);
	D3DXVec3TransformNormal(&rDir, &rDir, &matInvWorld);

	_bool bFirst = true;
	_float fMinDist;
	_vec3 vMinPos;

	for (int i = 0; i < faces.size(); ++i) {
		VTXMESH v0, v1, v2;
		_vec3 p0, p1, p2;
		float u, v, dist;

		v0 = vertices[faces[i].indices._0];
		v1 = vertices[faces[i].indices._1];
		v2 = vertices[faces[i].indices._2];

		p0 = v0.vPosition;
		p1 = v1.vPosition;
		p2 = v2.vPosition;

		if (D3DXIntersectTri(&p0, &p1, &p2, &rOrigin, &rDir, &u, &v, &dist))
		{
			if (!bFirst)
			{
				if (fMinDist > dist) {
					fMinDist = dist;
					vMinPos = p0 + u * (p1 - p0) + v * (p2 - p0);
				}
			}
			else {
				bFirst = false;
				fMinDist = dist;
				vMinPos = p0 + u * (p1 - p0) + v * (p2 - p0);
			}
		}
	}

	if (!bFirst) {
		if (fMinDist < 20.f) {
			D3DXCOLOR red(1.f, 0.f, 0.f, 1.f);
			CCalculator::DrawRayLine(m_pGraphicDev, rOrigin2, rOrigin2 + rDir2 * 20, red);
		}
		else {
			D3DXCOLOR green(0.f, 1.f, 0.f, 1.f);
			CCalculator::DrawRayLine(m_pGraphicDev, rOrigin2, rOrigin2 + rDir2 * 20, green);
		}
	}
	else {
		D3DXCOLOR green(0.f, 1.f, 0.f, 1.f);
		CCalculator::DrawRayLine(m_pGraphicDev, rOrigin2, rOrigin2 + rDir2 * 20, green);
	}
}

CHeadPivot* CHeadPivot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHeadPivot* pObj = new CHeadPivot(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Obj_Test Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CHeadPivot::Free()
{
	CGameObject::Free();
}
