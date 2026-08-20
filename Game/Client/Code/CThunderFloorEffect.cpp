#include "pch.h"
#include "CThunderFloorEffect.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCameraMgr.h"

CThunderFloorEffect::CThunderFloorEffect(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CThunderFloorEffect::CThunderFloorEffect(const CGameObject& rhs) : CGameObject(rhs)
{
}

CThunderFloorEffect::~CThunderFloorEffect()
{
}

HRESULT CThunderFloorEffect::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, D3DXToRadian(90), 0);
	m_pTransformCom->Set_Quaternion(&q);

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Scale({ 10,10,1 });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ThunderFloorEffectTexture"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fTimer = 0;
	m_fFrame = 0;

	return S_OK;
}

_int CThunderFloorEffect::Update_GameObject(const _float& fDeltaTime)
{
	CGameObject* pCart = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart");
	_vec3 vCartPos,vCarLook, vPos, vDir;
	pCart->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
	pCart->Get_Transform()->Get_Info(INFO_LOOK, &vCarLook);

	m_vRotation.y += fDeltaTime;
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, D3DXToRadian(90), 0);
	m_pTransformCom->Set_Quaternion(&q);


	vCartPos += vCarLook;
	m_pTransformCom->Set_Pos(vCartPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CGameObject::Compute_ViewZ(&vPos);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CThunderFloorEffect::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CThunderFloorEffect::Render_GameObject()
{
	_matrix	matWorld, matView;
	matView = CCameraMgr::GetInstance()->GetCameraInfo().matView;
	m_pTransformCom->Set_Billboard(&matView);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();
}

CThunderFloorEffect* CThunderFloorEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CThunderFloorEffect* pObj = new CThunderFloorEffect(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CThunderFloorEffect Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CThunderFloorEffect::Free()
{
	CGameObject::Free();
}
