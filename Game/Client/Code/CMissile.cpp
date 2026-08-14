#include "pch.h"
#include "CMissile.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CMissileTex.h"

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

	// m_pTransformCom->m_vScale = { 0.2f, 0.2f, 0.3f };
	// m_pTransformCom->m_vInfo[INFO_POS] = { 3.f, 1.3f, 0.f };
	//m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f });

	m_fSpeed = 120.f;
	m_fAngle = 0.f;

	Engine::CComponent* pComponent = nullptr;

	// 미사일
	pComponent = m_pBufferCom = dynamic_cast<CMissileTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MissileTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CMissile::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	Engine::CTransform* pTransform = nullptr;

	pTransform = dynamic_cast<Engine::CTransform*>(CManagement::GetInstance()->Get_Component(ID_STATIC, L"GameLogic", L"Obj_Box", L"Com_Transform"));
	// 여기선 ID_DYNAMIC이 아닌 ID_STATIC 로 사용
	if (nullptr == pTransform)
		return;

	_vec3 outerBoxPos;	// 박스 주변 도는 목표점 위치
	pTransform->Get_Info(INFO_POS, &outerBoxPos);

	 _vec3 pMissilePos;
	m_pTransformCom->Get_Info(INFO_POS, &pMissilePos);

	_vec3 vDir = outerBoxPos - pMissilePos;
	_float fDistance = D3DXVec3Length(&vDir);

	_float radius = clampT((fDistance-10.f) * 10.f, 0.f, 1500.f);

	_vec3 innerBoxPos = outerBoxPos;	// 실제 박스 중심 위치

	m_fAngle += D3DXToRadian(800.f) * fFixedDeltaTime;

	D3DXMATRIX matRadius;
	D3DXMATRIX matRot;
	D3DXMATRIX matCenter;

	D3DXMatrixTranslation(&matRadius, radius, 0.0f, 0.0f);
	D3DXMatrixRotationZ(&matRot, m_fAngle);
	D3DXMatrixTranslation(&matCenter, outerBoxPos.x, outerBoxPos.y, outerBoxPos.z);

	_matrix matWorld = matRadius * matRot * matCenter;

	_vec3 vOriginPos = { 0.f, 0.f, 0.f };
	D3DXVec3TransformCoord(&outerBoxPos, &vOriginPos, &matWorld);

	if (fDistance > 8.f)
	{
		_vec3 vMoveDir = outerBoxPos - pMissilePos;
		D3DXVec3Normalize(&vMoveDir, &vMoveDir);

		_vec3 vLookDir = innerBoxPos - pMissilePos;
		D3DXVec3Normalize(&vLookDir, &vLookDir);

		_matrix matRot;
		m_pTransformCom->GetFollowRotation(&vLookDir, &matRot);

		_quaternion qRot;
		D3DXQuaternionRotationMatrix(&qRot, &matRot);

		m_pTransformCom->Multiple_Quaternion(&qRot);
		m_pTransformCom->Move_Pos(&vMoveDir,m_fSpeed,fFixedDeltaTime);
	}
	else
	{
		_vec3 vMoveDir = innerBoxPos - pMissilePos;
		D3DXVec3Normalize(&vMoveDir, &vMoveDir);

		_vec3 vLookDir = innerBoxPos - pMissilePos;
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

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA

	return iExit;
}

void CMissile::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CMissile::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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
