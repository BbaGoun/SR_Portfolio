#include "pch.h"
#include "CSkyDome.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CCameraMgr.h"
#include "CRenderer.h"

CSkyDome::CSkyDome(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CSkyDome::CSkyDome(const CGameObject& rhs) : CGameObject(rhs)
{
}

CSkyDome::~CSkyDome()
{
}

HRESULT CSkyDome::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Scale({ 1000.f, 1000, 1000 });
	m_pTransformCom->Set_Pos({ 0, -200, 0});

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CHalfSphere*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_HalfSphere"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SkyDome"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CSkyDome::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CSkyDome::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CCamera* p_Camera = CCameraMgr::GetInstance()->GetMainCamera();
	_vec3 pos;
	p_Camera->Get_Transform()->Get_Info(INFO_POS, &pos);
	pos.y -= 200.f;
	m_pTransformCom->Set_Pos(pos);
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CSkyDome::Render_GameObject()
{
	D3DXMATRIX* matWorld;

	matWorld = m_pTransformCom->Get_World();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	// Z버퍼에 Z값을 저장할 지 묻는 옵션

	m_pTextureCom->Set_Texture(0);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	// Z버퍼에 Z값을 저장할 지 묻는 옵션
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CSkyDome* CSkyDome::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyDome* pObj = new CSkyDome(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Obj_Test Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CSkyDome::Free()
{
	CGameObject::Free();
}
