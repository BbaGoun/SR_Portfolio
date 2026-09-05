#include "pch.h"
#include "CEmp_Band.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CMissileTarget.h"
#include "CCameraMgr.h"

CEmp_Band::CEmp_Band(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CEmp_Band::~CEmp_Band()
{
}

HRESULT CEmp_Band::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fUfoHitTimer		   = 0.f;

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 7.5f, 7.5f,1.f });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_EmpBand"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 1,1,1 });
	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CEmp_Band::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	CMissileTarget* pTarget = dynamic_cast <CMissileTarget*>(Get_Parent());

	_vec3 vScale = m_pTransformCom->Get_Scale();

	if (m_pParent != nullptr && pTarget->GetUfoHit() == false)
	{
		m_pTransformCom->Set_Pos({ 0.f, 5.f, 0.f });
	}

	m_pTransformCom->Set_Scale(vScale);


	if (pTarget != nullptr && pTarget->GetUfoHit())
	{
		m_fUfoHitTimer += fFixedDeltaTime;

		if (m_fUfoHitTimer >= 0.5f)
		{
			_vec3 vUp = { 0.f, 1.f, 0.f };

			m_pTransformCom->Move_Pos(&vUp, 5.f, fFixedDeltaTime);
		}
	}
	
	else
	{
		m_fUfoHitTimer = 0.f;
	}

	_quaternion q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, 0.f, 0.f);
	m_pTransformCom->Set_Quaternion(&q);
}

_int CEmp_Band::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CEmp_Band::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CEmp_Band::Render_GameObject()
{
	_matrix	matWorld, matView;
	matView = CCameraMgr::GetInstance()->GetCameraInfo().matView;
	m_pTransformCom->Set_Billboard(&matView);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	// m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEmp_Band::CollisionEnter(CCollider* pOtherCollider)
{
}

void CEmp_Band::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcscmp(wOtherTag, L"Obj_Ufo") == 0)
	{
		CMissileTarget* pTarget = dynamic_cast<CMissileTarget*>(Get_Parent());

		if (pTarget != nullptr)
			pTarget->ClearEmpBand();

		To_Root();
		m_pLayer->Delete_GameObject(this);
	}
}

CEmp_Band* CEmp_Band::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEmp_Band* pObj = new CEmp_Band(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CEmp_Band Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CEmp_Band::Free()
{
	CGameObject::Free();
}
