#include "pch.h"
#include "CWaterBombThrow.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCameraMgr.h"
#include "CTrackMgr.h"
#include "CWaterBomb.h"
#include "CWaterBombBody.h"
#include "SoundMgr.h"

CWaterBombThrow::CWaterBombThrow(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CWaterBombThrow::CWaterBombThrow(const CGameObject& rhs) : CGameObject(rhs)
{
}

CWaterBombThrow::~CWaterBombThrow()
{
}

HRESULT CWaterBombThrow::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer = 0.f;

	m_fThrowHeight = 0.f;

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 2.f , 3.f, 1.f });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ThrowWaterBomb"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CWaterBombThrow::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (m_fThrowHeight > 50.f)
	{
		TrackPose TP = CTrackMgr::GetInstance()->Compute_TargetPose(m_pParent, 150, false);

		if (TP.bValid) {
			CWaterBomb* pWaterBomb = CWaterBomb::Create(m_pGraphicDev);
			if (pWaterBomb == nullptr)
				return;
			if (FAILED(m_pLayer->Add_GameObject(L"Obj_WaterBomb", pWaterBomb)))
				return;

			CWaterBombBody* pWaterBombBody = CWaterBombBody::Create(m_pGraphicDev);
			if (pWaterBomb == nullptr)
				return;
			if (FAILED(m_pLayer->Add_GameObject(L"Obj_WaterBombBody", pWaterBombBody)))
				return;

			SoundMgr::GetInstance().PlaySound(L"Effect/Item_waterBomb/set.ogg", SOUND_WATERBOMB, 0.4f);

			pWaterBomb->Set_Child(pWaterBombBody);
			pWaterBomb->Get_Transform()->Set_Pos(TP.position);

			_matrix	matRot;
			D3DXMatrixIdentity(&matRot);
			memcpy(&matRot.m[0], &TP.R, sizeof(_vec3));
			memcpy(&matRot.m[1], &TP.U, sizeof(_vec3));
			memcpy(&matRot.m[2], &TP.T, sizeof(_vec3));

			_quaternion q;
			D3DXQuaternionRotationMatrix(&q, &matRot);
			pWaterBomb->Get_Transform()->Set_Quaternion(&q);
		}
		m_pLayer->Delete_GameObject(this);
		return;
	}
	m_fThrowHeight += fFixedDeltaTime * 50.f;

	m_pTransformCom->Set_Pos({ 1.f, m_fThrowHeight, 0 });
}

_int CWaterBombThrow::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CWaterBombThrow::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CWaterBombThrow::Render_GameObject()
{
	_matrix	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pTransformCom->Set_Billboard(&matView);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	//m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWaterBombThrow::CollisionEnter(CCollider* pOtherCollider)
{
}

void CWaterBombThrow::TriggerEnter(CCollider* pOtherCollider)
{
}

CWaterBombThrow* CWaterBombThrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterBombThrow* pObj = new CWaterBombThrow(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CWaterBombThrow Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CWaterBombThrow::Free()
{
	CGameObject::Free();
}
