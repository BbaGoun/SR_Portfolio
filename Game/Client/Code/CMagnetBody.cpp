#include "pch.h"
#include "CMagnetBody.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CMagnetTex.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"

CMagnetBody::CMagnetBody(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CMagnetBody::~CMagnetBody()
{
}

HRESULT CMagnetBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 8.f, 8.f, 7.f });

	m_fTimer = 0;

	pComponent = m_pBufferCom = dynamic_cast<CMagnetTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MagnetTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 1.f, 1.f, 1.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

_int CMagnetBody::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	CCart* pCart = dynamic_cast<CCart*>(CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart"));
	CGameObject* pTarget = pCart->GetMagnetTarget();

	_vec3 vPos, vCartPos, vLook, vDir, vTargetPos, vCartLook, vCartUp, vMyPos;
	
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

	pCart->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
	pCart->Get_Transform()->Get_Info(INFO_UP, &vCartUp);
	pCart->Get_Transform()->Get_Info(INFO_LOOK, &vCartLook);

	m_pTransformCom->Set_Pos(vCartPos + vCartLook * 3.f + vCartUp * 5);
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);

	vDir = vTargetPos - vMyPos;

	if (D3DXVec3Length(&vDir) <= 0.001f)
		return 0;

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);

	_quaternion qRot;
	float fAngle = acosf(D3DXVec3Dot(&vLook, &vDir));
	_vec3 vAxis;
	D3DXVec3Cross(&vAxis, &vLook, &vDir);
	D3DXQuaternionRotationAxis(&qRot, &vAxis, fAngle);
	m_pTransformCom->Set_Quaternion(&qRot);

	m_fTimer += fDeltaTime;

	if (m_fTimer > 3.5f)
	{
		m_pLayer->Delete_GameObject(this);
	}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMagnetBody::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMagnetBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	// m_pColliderCom->Render_Component(D3DXCOLOR({ 1,0,0,1 }));
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CMagnetBody::CollisionEnter(CCollider* pOtherCollider)
{
}

void CMagnetBody::TriggerEnter(CCollider* pOtherCollider)
{
}

CMagnetBody* CMagnetBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMagnetBody* pObj = new CMagnetBody(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CMagnetBody Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CMagnetBody::Free()
{
	CGameObject::Free();
}
