#include "pch.h"
#include "CBoostEffect.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCart.h"
#include "CCameraMgr.h"
CBoostEffect::CBoostEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoostEffect::CBoostEffect(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CBoostEffect::~CBoostEffect()
{
}

HRESULT CBoostEffect::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Scale({ 5,5,0 });
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BoosterEffect"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CBoostEffect::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CBoostEffect::Update_GameObject(const _float& fDeltaTime)
{
	if (dynamic_cast<CCart*>(m_pParent)->GetBoost())
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

		m_fFrame += 45.f * fDeltaTime;
		if (m_fFrame > 2.f)
			m_fFrame = 0;

		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		CGameObject::Compute_ViewZ(&vPos);
		return CGameObject::Update_GameObject(fDeltaTime);
	}
}

void CBoostEffect::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (dynamic_cast<CCart*>(m_pParent)->GetBoost())
		CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CBoostEffect::Render_GameObject()
{
	if (dynamic_cast<CCart*>(m_pParent)->GetBoost())
	{
		_matrix	matBill, matWorld, matView, matParent;
		
		matWorld = *m_pTransformCom->Get_World();
		matParent = *m_pParent->Get_Transform()->Get_World();
		matView = CCameraMgr::GetInstance()->GetCameraInfo().matView;
		
		// 부모의 y축 회전 반영X
		D3DXMatrixIdentity(&matBill);

		matBill._11 = matParent._11;
		matBill._13 = matParent._13;
		matBill._31 = matParent._31;
		matBill._33 = matParent._33;

		D3DXMatrixInverse(&matBill, 0, &matBill);
		matWorld = matBill * matWorld;

		// 카메라의 y축 회전 반영X
		D3DXMatrixIdentity(&matBill);

		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		D3DXMatrixInverse(&matBill, 0, &matBill);
		matWorld = matBill * matWorld;

		// 빌보드 적용(크기가 달라질 경우 계산식 변경)
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		//m_pTextureCom->Set_Texture(0);
		m_pTextureCom->Set_Texture((_uint)m_fFrame);

		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

CBoostEffect* CBoostEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoostEffect* pObj = new CBoostEffect(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CBoostEffect Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CBoostEffect::Free()
{
	CGameObject::Free();
}
