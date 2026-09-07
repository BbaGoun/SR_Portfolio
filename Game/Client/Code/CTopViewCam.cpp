#include "pch.h"
#include "CTopViewCam.h"
#include "CCameraMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CTerrain2.h"

CTopViewCam::CTopViewCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}

CTopViewCam::CTopViewCam(const CTopViewCam& rhs)
	: CCamera(rhs)
{
}

CTopViewCam::~CTopViewCam()
{
}

HRESULT CTopViewCam::Ready_GameObject(const _vec3& pEye,
										const _vec3& pAt,
										const _vec3& pUp,
										const _float& fFov, 
										const _float& fAspect, 
										const _float& fNear, 
										const _float& fFar)
{
	m_vEye = pEye;
	m_vAt  = pAt;
	m_vUp  = pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	if (FAILED(CCamera::Ready_GameObject()))
		return E_FAIL;

	m_fYaw = 0;
	m_fDistScale = 1;

	return S_OK;
}

_int CTopViewCam::Update_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		Mouse_Input();
	}
	return 0;
}

void CTopViewCam::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (CCameraMgr::GetInstance()->CheckIsMainCamera(this)) {
		CComponent* pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"GameLogic", L"Obj_Player", L"Com_Transform");
		if (pCom == nullptr)
			return;

		CTransform* pTrans = static_cast<CTransform*>(pCom);
		_vec3 pos;
		pTrans->Get_Info(INFO_POS, &pos);

		_vec3 offset = { 0.f, 30.f, -30.f };
		offset *= m_fDistScale;
		_matrix matRot;
		D3DXMatrixRotationY(&matRot, m_fYaw);
		D3DXVec3TransformCoord(&offset, &offset, &matRot);

		m_vEye = pos + offset;
		m_pTransformCom->Set_Pos(m_vEye);
		m_vAt = pos;
		m_vUp = { 0, 1, 0 };

		CCamera::LateUpdate_GameObject(fDeltaTime);
		CCameraMgr::GetInstance()->UpdateMainCameraInfo(&m_matView, &m_matProj);
	}
}

void CTopViewCam::Mouse_Input()
{
	_long	dwMouseMove(0);

	if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_MB)) {
		if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_X))
		{
			m_fYaw += D3DXToRadian(dwMouseMove / 10.f);
		}
	}

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Z))
	{
		m_fDistScale = clampT(m_fDistScale - dwMouseMove / 1000.f, 0.5f, 3.f);
	}

	if (CDInputMgr::GetInstance()->Get_DIMouseKeyDown(DIM_RB)) {
		ClickTarget();
	}
}

void CTopViewCam::ClickTarget()
{
	CComponent* pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment", L"Env_Land2", L"Com_Buffer");
	if (pCom == nullptr)
		return;
	vector<VTXMESH> vertices = static_cast<CTerrain2*>(pCom)->GetVertices();
	vector<FACE32> faces = static_cast<CTerrain2*>(pCom)->GetFaces();

	// 1. 마우스 위치 얻기
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(g_hWnd, &mouse);

	// 1. 뷰포트 -> 투영
	_vec3 vMouse;
	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	vMouse.x = (mouse.x) / (vp.Width * 0.5f) - 1;
	vMouse.y = (mouse.y) / -(vp.Height * 0.5f) + 1;
	vMouse.z = 1;

	// 2. 투영 -> 뷰
	_matrix matProj, matInvProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matInvProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matInvProj);

	// 3. 뷰 스페이스에서 레이저 생성
	_vec3 rOrigin, rDir;
	rOrigin = { 0, 0, 0 };  // 뷰 스페이스 원점
	rDir = vMouse;  // 위치 벡터 - 원점이라 그냥 입력

	// 4. 뷰 스페이스 -> 월드
	_matrix matView, matInvView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, 0, &matView);

	D3DXVec3TransformCoord(&rOrigin, &rOrigin, &matInvView);
	D3DXVec3TransformNormal(&rDir, &rDir, &matInvView);

	// 5. ray를 지형의 로컬 스페이스로 변환
	pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment", L"Env_Land2", L"Com_Transform");
	if (pCom == nullptr)
		return;

	_matrix* pMatWorld = static_cast<CTransform*>(pCom)->Get_World();
	_matrix matInvWorld;

	D3DXMatrixInverse(&matInvWorld, 0, pMatWorld);

	D3DXVec3TransformCoord(&rOrigin, &rOrigin, &matInvWorld);
	D3DXVec3TransformNormal(&rDir, &rDir, &matInvWorld);

	// 레이 충돌 검사

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
		// 레이를 다시 월드로 올림
		D3DXVec3TransformCoord(&vMinPos, &vMinPos, pMatWorld);
		pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"GameLogic", L"Obj_Player", L"Com_Transform");
		//static_cast<CGOCody*>(pCom->Get_Owner())->Set_Target(vMinPos);
	}
}

CTopViewCam* CTopViewCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& pEye, const _vec3& pAt, const _vec3& pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CTopViewCam* pCamera = new CTopViewCam(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CTopViewCam::Free()
{
	CCamera::Free();
}
