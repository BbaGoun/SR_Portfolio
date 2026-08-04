#include "pch.h"
#include "CGOCody.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CKeyMgr.h"
#include "CCameraMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"

CGOCody::CGOCody(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
, m_bFix(true)
, m_bCheck(false)
, m_bTarget(false)
{
}

CGOCody::CGOCody(const CGameObject& rhs) : CGameObject(rhs)
, m_bFix(true)
, m_bCheck(false)
, m_bTarget(false)
{
}

CGOCody::~CGOCody()
{
}

HRESULT CGOCody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fSpeed = 20;

	return S_OK;
}

void CGOCody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (m_bJump) {
		m_fJumpTime += fFixedDeltaTime;
		m_vForce.y = max(-19.6f,
			m_vForce.y - (9.8f * m_fJumpTime * m_fJumpTime));
	}
	else
		m_vForce.y = 0.f;

	_vec3 pos;
	m_pTransformCom->Get_Info(INFO_POS, &pos);
	pos += m_vForce * fFixedDeltaTime;
	AdjustPosY(pos);
}

_int CGOCody::Update_GameObject(const _float& fDeltaTime)
{
	Key_Input();
	Mouse_Input();
	FollowTarget();
	
	Mouse_Fix();

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CGOCody::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CGOCody::Key_Input()
{
	m_vForce.x = 0;
	m_vForce.z = 0;

	CAMERA_STATE camState = CCameraMgr::GetInstance()->GetCamerState();
	if (camState == CAMERA_FIRST || camState == CAMERA_BACK_THIRD || camState == CAMERA_FRONT_THIRD)
	{
		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_D))
		{
			_vec3	vRight;
			m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

			_vec3	vLength = vRight * m_fSpeed;

			m_vForce += vLength;
			m_bTarget = false;
		}

		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_A))
		{
			_vec3	vRight;
			m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

			_vec3	vLength = vRight * m_fSpeed;

			m_vForce -= vLength;
			m_bTarget = false;
		}

		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_W))
		{
			_vec3	vLook;
			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

			_vec3	vLength = vLook * m_fSpeed;

			m_vForce += vLength;
			m_bTarget = false;
		}

		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_S))
		{
			_vec3	vLook;
			m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

			_vec3	vLength = vLook * m_fSpeed;

			m_vForce -= vLength;
			m_bTarget = false;
		}

		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_SPACE)) {
			if (!m_bJump) {
				m_bJump = true;
				m_vForce.y = 15;
				m_fJumpTime = 0;
				m_bTarget = false;
			}
		}
		if (CDInputMgr::GetInstance()->Get_DIKeyUp(DIK_SPACE)) {
			if (m_bJump) {
				m_vForce.y = -19.6f;
			}
		}

		if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIK_TAB)) {
			m_bFix = !m_bFix;
		}
	}

	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIK_F5)) {
		CAMERA_STATE state = CCameraMgr::GetInstance()->GetCamerState();
		switch (state) {
		case CAMERA_FIRST:
			CCameraMgr::GetInstance()->SetMainCamera(CAMERA_BACK_THIRD);
			break;
		case CAMERA_BACK_THIRD:
			CCameraMgr::GetInstance()->SetMainCamera(CAMERA_FRONT_THIRD);
			break;
		case CAMERA_FRONT_THIRD:
			CCameraMgr::GetInstance()->SetMainCamera(CAMERA_TOP_VIEW);
			break;
		case CAMERA_TOP_VIEW:
			CCameraMgr::GetInstance()->SetMainCamera(CAMERA_FIRST);
			break;
		}
	}
}

void CGOCody::Mouse_Input()
{
	_matrix* matWorld = m_pTransformCom->Get_World();

	_long	dwMouseMove(0);

	CAMERA_STATE camState = CCameraMgr::GetInstance()->GetCamerState();
	if (camState == CAMERA_FIRST || camState == CAMERA_BACK_THIRD || camState == CAMERA_FRONT_THIRD)
	{
		if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_X))
		{
			m_pTransformCom->Rotate(QUATER_YAW, dwMouseMove / 10.f);
		}
	}
}

void CGOCody::Mouse_Fix()
{
	if (!m_bFix)
		return;

	CAMERA_STATE camState = CCameraMgr::GetInstance()->GetCamerState();
	if (camState == CAMERA_FIRST || camState == CAMERA_BACK_THIRD || camState == CAMERA_FRONT_THIRD)
	{
		POINT center = { WINCX >> 1, WINCY >> 1 };
		ClientToScreen(g_hWnd, &center);
		SetCursorPos(center.x, center.y);
	}
}

void CGOCody::AdjustPosY(_vec3 pos)
{
	CComponent* pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment", L"Env_Land2", L"Com_Buffer");
	if (pCom == nullptr)
		return;
	vector<VTXMESH> vertices = static_cast<CTerrain2*>(pCom)->GetVertices();

	// 플레이어를 지형의 로컬 스페이스로 변환
	pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment", L"Env_Land2", L"Com_Transform");
	if (pCom == nullptr)
		return;

	_matrix* pMatWorld = static_cast<CTransform*>(pCom)->Get_World();
	_matrix matInvWorld;
	_vec3 originPos = pos;

	D3DXMatrixInverse(&matInvWorld, 0, pMatWorld);
	D3DXVec3TransformCoord(&pos, &pos, &matInvWorld);

	if ((0 <= pos.x && pos.x < VTXITV * (VTXCNTX - 1)) &&
		(0 <= pos.z && pos.z < VTXITV * (VTXCNTZ - 1))) 
	{
		int col = pos.x / VTXITV;
		int row = pos.z / VTXITV;

		float xInPlane = float(pos.x - col * VTXITV) / VTXITV;
		float zInPlane = float(pos.z - row * VTXITV) / VTXITV;

		_vec3 p0, p1, p2;
		// 왼쪽 위 삼각형
		if (zInPlane - xInPlane > 0) {
			p0 = vertices[(row + 1) * VTXCNTX + col].vPosition;		// 왼쪽 위
			p1 = vertices[(row + 1) * VTXCNTX + col+1].vPosition;	// 오른쪽 위
			p2 = vertices[row * VTXCNTX + col].vPosition;			// 왼쪽 아래
		}
		else { // 오른쪽 아래 삼각형
			p0 = vertices[row * VTXCNTX + col+1].vPosition;			// 오른쪽 아래
			p1 = vertices[row * VTXCNTX + col].vPosition;			// 왼쪽 아래
			p2 = vertices[(row + 1) * VTXCNTX + col + 1].vPosition; // 오른쪽 위
		}
		D3DXPLANE plane;
		D3DXPlaneFromPoints(&plane, &p0, &p1, &p2);

		// normal·p + d = 0
		// ax + by + cz + d = 0
		// by = -(ax + cz + d)
		// y = -(ax + cz + d) / b
		float y = -(plane.a * pos.x + plane.c * pos.z + plane.d) / plane.b;
		
		if (!m_bJump && pos.y <= y+0.5f) {
			_vec3 newPos = { pos.x, y, pos.z };
			D3DXVec3TransformCoord(&newPos, &newPos, pMatWorld);
			m_pTransformCom->Set_Pos(newPos);
		}
		if (m_bJump && pos.y <= y)
		{
			m_bJump = false;
			_vec3 newPos = { pos.x, y, pos.z };
			D3DXVec3TransformCoord(&newPos, &newPos, pMatWorld);
			m_pTransformCom->Set_Pos(newPos);
		}
		else {
			if (!m_bJump) {
				m_bJump = true;
				m_vForce.y = -19.6f;
				m_fJumpTime = 0;
			}
			m_pTransformCom->Set_Pos(originPos);
		}
	}
	else {
		m_pTransformCom->Set_Pos({ originPos.x, 0, originPos.z });
	}
}

void CGOCody::FollowTarget()
{
	if (m_bTarget) {
		_vec3 pos;
		m_pTransformCom->Get_Info(INFO_POS, &pos);
		_vec3 dir = m_vTargetPos - pos;
		if (D3DXVec3Length(&dir) < 0.5f)
		{
			m_bTarget = false;
			return;
		}
		D3DXVec3Normalize(&dir, &dir);
		m_vForce = { (dir * m_fSpeed).x, m_vForce.y, (dir * m_fSpeed).z };
	}
}

CGOCody* CGOCody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGOCody* pObj = new CGOCody(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Obj_Test Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CGOCody::Free()
{
	CGameObject::Free();
}
