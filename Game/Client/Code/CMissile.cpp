#include "pch.h"
#include "CMissile.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CMissileTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"

CMissile::CMissile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMissile::~CMissile()
{
}

HRESULT CMissile::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fSpeed = 180.f;
	m_fAngle = 0.f;

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->SetColliderType(CUBE_COLLIDER);
	m_pColliderCom->Set_Extents({ 1.f, 1.f, 1.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CMissile::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	Engine::CTransform* pTransform = nullptr;
	pTransform = dynamic_cast<Engine::CTransform*>(CManagement::GetInstance()->Get_Component(ID_STATIC, L"GameLogic", L"Obj_MissileTarget", L"Com_Transform"));
	// 여기선 ID_DYNAMIC이 아닌 ID_STATIC 로 사용
	if (nullptr == pTransform)
		return;

	_vec3 vBoxPos;
	pTransform->Get_Info(INFO_POS, &vBoxPos);

	 _vec3 vMissilePos;
	m_pTransformCom->Get_Info(INFO_POS, &vMissilePos);
	//cout << vMissilePos.x << " | " << vMissilePos.y << " | " << vMissilePos.z << " | " << "\n";

	_vec3 vDir = vBoxPos - vMissilePos;
	_float fDistance = D3DXVec3Length(&vDir);

	_float radius = clampT((fDistance-10.f) * 8.f, 0.f, 1500.f);

	m_fAngle += D3DXToRadian(800.f) * fFixedDeltaTime;

	D3DXMATRIX matOffset;
	D3DXMATRIX matRot;
	D3DXMATRIX matCenter;

	D3DXMatrixTranslation(&matOffset, radius, 0.0f, 0.0f);
	D3DXMatrixRotationZ(&matRot, m_fAngle);
	D3DXMatrixTranslation(&matCenter, vBoxPos.x, vBoxPos.y, vBoxPos.z);

	_matrix matOrbit = matOffset * matRot * matCenter;

	_vec3 vBoxOrbit;
	_vec3 vOriginPos = { 0.f, 0.f, 0.f };
	D3DXVec3TransformCoord(&vBoxOrbit, &vOriginPos, &matOrbit);

	if (fDistance > 8.f)
	{
		_vec3 vMoveDir = vBoxOrbit - vMissilePos;
		D3DXVec3Normalize(&vMoveDir, &vMoveDir);
		vMoveDir += vDir * 0.01f;

		_vec3 vLookDir = vBoxPos - vMissilePos;
		vLookDir.y = 0;
		D3DXVec3Normalize(&vLookDir, &vLookDir);

		_matrix matRot;
		m_pTransformCom->GetFollowRotation(&vLookDir, &matRot);

		_quaternion qRot;
		D3DXQuaternionRotationMatrix(&qRot, &matRot);

		m_pTransformCom->Multiple_Quaternion(&qRot);
		m_pTransformCom->Move_Pos(&vMoveDir,m_fSpeed ,fFixedDeltaTime);
	}
	else if(fDistance > 1.f)
	{
		_vec3 vMoveDir = vBoxPos - vMissilePos;
		D3DXVec3Normalize(&vMoveDir, &vMoveDir);

		_vec3 vLookDir = vBoxPos - vMissilePos;
		vLookDir.y = 0;
		D3DXVec3Normalize(&vLookDir, &vLookDir);

		_matrix matRot;
		m_pTransformCom->GetFollowRotation(&vLookDir, &matRot);

		_quaternion qRot;
		D3DXQuaternionRotationMatrix(&qRot, &matRot);

		m_pTransformCom->Multiple_Quaternion(&qRot);
		m_pTransformCom->Move_Pos(&vMoveDir,m_fSpeed,fFixedDeltaTime);
	}
}

_int CMissile::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CMissile::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CMissile::Render_GameObject()
{
	m_pColliderCom->Render_Component(D3DXCOLOR({ 1,0,0,1 }));
}

void CMissile::CollisionEnter(CCollider* pOtherCollider)
{
}

void CMissile::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcscmp(wOtherTag, L"Obj_MissileTarget") == 0)
	{

		m_pLayer->Delete_GameObject(this);
	}
}

CMissile* CMissile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMissile* pMissile = new CMissile(pGraphicDev);

	if (FAILED(pMissile->Ready_GameObject()))
	{
		Safe_Release(pMissile);
		MSG_BOX("pMissile Create Failed");
		return nullptr;
	}

	return pMissile;
}

void CMissile::Free()
{

	CGameObject::Free();
}
