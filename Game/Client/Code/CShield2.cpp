#include "pch.h"
#include "CShield2.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"
#include "SoundMgr.h"
#include "CMissileTarget.h"

CShield2::CShield2(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CShield2::CShield2(const CGameObject& rhs) : CGameObject(rhs)
{
}

CShield2::~CShield2()
{
}

HRESULT CShield2::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer = 0;
	m_bShow = false;

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Pos({ 0.f,1.5f,0.f });
	m_pTransformCom->Set_Scale({ 8.f, 8.f, 8.f });

	pComponent = m_pBufferCom = dynamic_cast<CSphere*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Sphere"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SecondShieldEffect"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CShield2::Update_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == true)
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
		m_fTimer += fDeltaTime;
		if (m_fTimer >= 2.f)
		{
			m_fTimer = 0;
			m_bShow = false;
			m_fScale = 1.f;
			m_fAlpha = 10.f;
		}
		if (m_fScale > 0.4) m_fScale -= 2 * fDeltaTime;
		m_pTransformCom->Set_Scale(_vec3({ 20.f, 20.f, 20.f }) * m_fScale);

		if (m_fAlpha < 127) m_fAlpha += 300 * fDeltaTime;
		return CGameObject::Update_GameObject(fDeltaTime);
	}
}

void CShield2::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CShield2::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	// 1. 임의로 지정할 알파값 설정 (예: 0x80 = 128, 약 50% 투명도)
// ARGB 형식이며, 여기서는 RGB 값은 상관없고 A(Alpha) 값만 사용됩니다.
	// 투명도를 점점 낮추기
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((int)m_fAlpha, 0, 0, 0));

	// 2. 색상(RGB)은 원래 텍스처의 색상을 그대로 사용
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	// 3. 알파(A)는 텍스처 대신 위에서 설정한 TEXTUREFACTOR(TFACTOR)를 강제로 사용
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

	// [그리기 수행] DrawPrimitive 또는 Mesh->DrawSubset 등
	m_pTextureCom->Set_Texture(2);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetTexture(0, nullptr);


	// 4. 그리기 종료 후 다음 물체를 위해 알파 설정을 원래대로 복구 (기본값: 텍스처 알파 사용)
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

}

CShield2* CShield2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShield2* pObj = new CShield2(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CShield2 Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CShield2::Free()
{
	CGameObject::Free();
}
